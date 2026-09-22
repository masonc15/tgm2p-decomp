-- Scenario-driven coverage capture for tgm2p.
--
-- Runs a scenario file (a Lua chunk returning a table) that lists input events
-- and trace windows by frame number. Trace windows stream the debugger trace
-- into COV_FIFO, which tools/tracecov.c consumes. Snapshots are optional.
--
-- Scenario format:
--   return {
--     length = 3600,                        -- frames to run
--     trace  = { {60, 30}, {1800, 60} },    -- {start_frame, frames}
--     snap   = { 600, 1200 },               -- frames to snapshot
--     input  = { {1500, "Coin 1", 4}, ... } -- {frame, field name, hold frames}
--     random = { from = 2000, to = 9000, fields = {"P1 Left", ...}, seed = 1 },
--   }
local m = manager.machine
local dbg = m.debugger
local scenario = dofile(assert(os.getenv("COV_SCENARIO"), "COV_SCENARIO unset"))
local fifo = os.getenv("COV_FIFO")

local fields = {}
for _, port in pairs(m.ioport.ports) do
	for name, field in pairs(port.fields) do fields[name] = field end
end

local held = {} -- field name -> release frame
local function press(name, frames, now)
	local field = assert(fields[name], "no input field " .. name)
	field:set_value(1)
	held[name] = now + frames
end

local starts, stops, snaps = {}, {}, {}
for _, w in ipairs(scenario.trace or {}) do
	starts[w[1]] = true
	stops[w[1] + w[2]] = true
end
for _, f in ipairs(scenario.snap or {}) do snaps[f] = true end

local events = {}
for _, e in ipairs(scenario.input or {}) do
	events[e[1]] = events[e[1]] or {}
	table.insert(events[e[1]], e)
end

local rnd = scenario.random
if rnd then math.randomseed(rnd.seed or 1) end

local frame = 0
local tracing = false
dbg:command("go")
sub = emu.add_machine_frame_notifier(function()
	frame = frame + 1
	for name, release in pairs(held) do
		if frame >= release then
			fields[name]:set_value(0)
			held[name] = nil
		end
	end
	for _, e in ipairs(events[frame] or {}) do press(e[2], e[3] or 4, frame) end
	if rnd and frame >= rnd.from and frame < rnd.to and frame % (rnd.every or 6) == 0 then
		local name = rnd.fields[math.random(#rnd.fields)]
		if not held[name] then press(name, rnd.hold or 3, frame) end
	end
	if stops[frame] and tracing then
		dbg:command("trace off")
		tracing = false
	end
	if starts[frame] and fifo and not tracing then
		dbg:command("trace " .. fifo .. ",maincpu,noloop")
		tracing = true
	end
	if snaps[frame] then m.video:snapshot() end
	if frame >= scenario.length then
		if tracing then dbg:command("trace off") end
		m:exit()
	end
end)
