-- Deterministic replay for tgm2p under MAME: scripted inputs, screen hashes
-- at fixed checkpoints, and an optional log of data reads.
--
-- Environment:
--   REPLAY_MODE    attract | gameplay           (default attract)
--   REPLAY_FRAMES  frames to run                (default 20000)
--   REPLAY_EVERY   checkpoint interval, frames  (default 100)
--   REPLAY_OUT     file for "frame hash" lines  (required)
--   REPLAY_SEED    input sequence seed          (default 1)
--   REPLAY_READS   optional file: "addr width count" lines aggregating every
--                  read (instruction fetches included) in REPLAY_READ_RANGES;
--                  needs MAME's -nodrc, since the SH-2 recompiler reads RAM
--                  through direct pointers that bypass taps
--                  Each 32-bit read of a value that could be an address is
--                  also remembered for the next 16 reads; when one of those
--                  lands within 0x200 bytes after the value, the word it came
--                  from is logged as "deref ADDR" (a pointer the game followed)
--                  and "frame N reads" lines count logged reads per frame,
--                  a sequence two identically behaving images share
--   REPLAY_READS_FROM  first frame to log reads from (default 0)
--   REPLAY_READS_UNTIL last frame to log reads in (default: all)
--   REPLAY_TRACE   "FRAME:FILE": write MAME's instruction trace (with r0-r15
--                  and T) for that one frame; needs mame -debug -debugger none
--   REPLAY_READS_PC    1: also log "pc ADDR PC" for the first read of each
--                  address by each instruction (slow; use with a narrow window)
--   REPLAY_READS_SKIP_BOOT  1: ignore reads made by the ROM boot block
--                  (pc < 0x780): its RAM test and copy/checksum loops read
--                  every byte and say nothing about how the game uses data
--   REPLAY_READ_RANGES  "lo-hi,lo-hi" hex address ranges to log (default:
--                  ROM 0x400-0xfffff and RAM 0x06000000-0x0605ffff, i.e. the
--                  boot code, the program image, the code and initialized
--                  data copied to RAM)
--
--   REPLAY_PIN_IDLE  1: every read of g_606000c, the idle-loop counter of the
--                  frame wait in f_2b4e8, returns 40000. The system random
--                  f_2beca multiplies by it, so without the pin the game's
--                  random sequence depends on how many cycles each frame took;
--                  needs -nodrc (the recompiler bypasses taps on RAM)
--
-- The screen hash is FNV-1a (64-bit) over the raw 320x240 pixel buffer, so it
-- compares exactly what MAME rendered. Inputs depend only on the frame number
-- and the seed, never on game state, so two ROMs that behave identically see
-- identical inputs.
local m = manager.machine
local mode = os.getenv("REPLAY_MODE") or "attract"
local total = tonumber(os.getenv("REPLAY_FRAMES") or "20000")
local every = tonumber(os.getenv("REPLAY_EVERY") or "100")
local outpath = assert(os.getenv("REPLAY_OUT"), "REPLAY_OUT unset")
local seed = tonumber(os.getenv("REPLAY_SEED") or "1")
local readpath = os.getenv("REPLAY_READS")
local reads_from = tonumber(os.getenv("REPLAY_READS_FROM") or "0") or 0
local reads_until = tonumber(os.getenv("REPLAY_READS_UNTIL") or "") or math.huge
local per_frame, frame_reads = {}, 0
local skip_boot = os.getenv("REPLAY_READS_SKIP_BOOT") == "1"
local log_pc = os.getenv("REPLAY_READS_PC") == "1"
local pcs = {}
local trace_frame, trace_file = string.match(os.getenv("REPLAY_TRACE") or "", "^(%d+):(.+)$")
trace_frame = tonumber(trace_frame)
local snaps = {}  -- REPLAY_SNAPS="1000,5000": PNG snapshots at these frames
for f in string.gmatch(os.getenv("REPLAY_SNAPS") or "", "%d+") do snaps[tonumber(f)] = true end

local out = assert(io.open(outpath, "w"))
local fields = {}
for _, port in pairs(m.ioport.ports) do
	for name, field in pairs(port.fields) do fields[name] = field end
end
local screen = m.screens[":screen"]

-- 64-bit FNV-1a over the pixel string, eight bytes at a time.
local function hash(s)
	local h = -3750763034362895579 -- 0xcbf29ce484222325
	local prime = 1099511628211
	local n = #s
	local i = 1
	while i + 7 <= n do
		local v = string.unpack("<i8", s, i)
		h = (h ~ v) * prime
		i = i + 8
	end
	while i <= n do
		h = (h ~ s:byte(i)) * prime
		i = i + 1
	end
	return string.format("%016x", h)
end

-- Inputs: a 32-bit LCG keyed by the seed picks a held direction or button.
local state = seed
local function rnd()
	state = (state * 1103515245 + 12345) & 0x7fffffff
	return state
end
local held = {}
local function press(name, frames, now)
	local f = fields[name]
	if f then
		f:set_value(1)
		held[name] = now + frames
	end
end
local moves = { "P1 Left", "P1 Right", "P1 Down", "P1 Button 1", "P1 Button 2", "P1 Button 3",
	"P1 Left", "P1 Right", "P1 Down" }
local function gameplay(frame)
	-- a coin and a start press every 1200 frames keep a game running across
	-- game overs; they are harmless while a game is in progress
	local phase = frame % 1200
	if phase == 300 then press("Coin 1", 6, frame) end
	if phase == 360 or phase == 420 then press("1 Player Start", 6, frame) end
	if frame >= 480 and frame % 8 == 0 then
		local r = rnd()
		local name = moves[(r >> 8) % #moves + 1]
		if not held[name] then press(name, 2 + (r >> 4) % 6, frame) end
	end
end

local reads
local frame = 0
if os.getenv("REPLAY_PIN_IDLE") == "1" then
	local space = m.devices[":maincpu"].spaces["program"]
	pin_tap = space:install_read_tap(0x0606000c, 0x0606000f, "pin-idle",
		function(offset, data, mask) return 40000 end)
end
if readpath then
	reads = {}
	local space = m.devices[":maincpu"].spaces["program"]
	local pcstate = m.devices[":maincpu"].state["PC"]
	derefs = {}
	local pv, ps, pn = {}, {}, 16
	for k = 1, pn do pv[k] = -1; ps[k] = 0 end
	local pk = 1
	local function note(offset, mask, data)
		if frame < reads_from or frame > reads_until then return end
		if skip_boot and pcstate.value < 0x780 then return end
		-- the bus is 32 bits wide and big-endian: the mask locates the access
		local width, shift = 4, 0
		if mask == 0xffffffff then width = 4
		elseif mask == 0xffff0000 then width, shift = 2, 0
		elseif mask == 0x0000ffff then width, shift = 2, 2
		elseif mask == 0xff000000 then width, shift = 1, 0
		elseif mask == 0x00ff0000 then width, shift = 1, 1
		elseif mask == 0x0000ff00 then width, shift = 1, 2
		elseif mask == 0x000000ff then width, shift = 1, 3
		end
		local addr = (offset & 0x0fffffff) + shift
		local key = addr * 8 + width
		reads[key] = (reads[key] or 0) + 1
		frame_reads = frame_reads + 1
		if log_pc then
			local k = addr * 0x100000000 + pcstate.value
			pcs[k] = true
		end
		for k = 1, pn do
			local v = pv[k]
			if addr >= v and addr < v + 0x200 then derefs[ps[k]] = true end
		end
		if width == 4 then
			local v = data & 0x0fffffff
			if (v >= 0x313fc and v < 0x100000) or (v >= 0x06000000 and v < 0x06060000) then
				pv[pk] = v; ps[pk] = addr
				pk = pk % pn + 1
			end
		end
	end
	-- taps are kept in a global table: a collected handler removes itself
	read_taps = {}
	local spec = os.getenv("REPLAY_READ_RANGES") or ""
	if spec == "" then spec = "400-fffff,6000000-605ffff" end
	for lo, hi in string.gmatch(spec, "(%x+)-(%x+)") do
		local tap = space:install_read_tap(tonumber(lo, 16) & ~3, tonumber(hi, 16) | 3, "replay-" .. lo,
			function(offset, data, mask) note(offset, mask, data) end)
		table.insert(read_taps, tap)
	end
end

replay_sub = emu.add_machine_frame_notifier(function()
	if reads then per_frame[frame] = frame_reads; frame_reads = 0 end
	frame = frame + 1
	for name, release in pairs(held) do
		if frame >= release then
			fields[name]:set_value(0)
			held[name] = nil
		end
	end
	if mode == "gameplay" then gameplay(frame) end
	if trace_frame and frame == trace_frame then
		m.debugger:command("trace " .. trace_file .. ",maincpu,noloop,{tracelog \"%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %x|\",r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,sr&1}")
	elseif trace_frame and frame == trace_frame + 1 then
		m.debugger:command("trace off")
	end
	if snaps[frame] then m.video:snapshot() end
	if frame % every == 0 then
		out:write(string.format("%d %s\n", frame, hash(screen:pixels())))
		out:flush()
	end
	if frame >= total then
		out:close()
		if reads then
			local f = assert(io.open(readpath, "w"))
			for key, n in pairs(reads) do
				f:write(string.format("%08x %d %d\n", key // 8, key % 8, n))
			end
			for a in pairs(derefs) do f:write(string.format("deref %08x\n", a)) end
			for k in pairs(pcs) do
				f:write(string.format("pc %08x %08x\n", k // 0x100000000, k % 0x100000000))
			end
			for k = 0, frame - 1 do
				if per_frame[k] then f:write(string.format("frame %d %d\n", k, per_frame[k])) end
			end
			f:close()
		end
		m:exit()
	end
end)
