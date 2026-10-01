#!/usr/bin/env python3
"""Smoke-test the ghidra MCP server the way Claude Code runs it (stdio).

usage: uv run --with mcp tools/ghidra/mcp_smoke.py [--list] [TOOL JSON_ARGS ...]

With no tool calls it decompiles f_14b10 (RAM 0x6014390) and lists its
callers. --list prints every tool with its parameters.
"""
import asyncio
import json
import sys
import time

from mcp import ClientSession, StdioServerParameters
from mcp.client.stdio import stdio_client

# Same command `claude mcp add ghidra` registers: the server runs on nuada.
SERVER = StdioServerParameters(
    command="ssh",
    args=["-o", "BatchMode=yes", "nuada", "/drive2/tgm2p/ghidra/reva-venv/bin/python",
          "/home/colin/workspace/tgm2p-decomp/tools/ghidra/reva_mcp.py"],
)
DEFAULT = [
    ("get-decompilation", {"programPath": "/prog.bin", "functionNameOrAddress": "0x06014390", "limit": 40}),
    ("find-cross-references", {"programPath": "/prog.bin", "location": "0x06014390", "direction": "to"}),
]


def text(result) -> str:
    return "\n".join(getattr(c, "text", str(c)) for c in result.content)


async def main(argv: list[str]) -> None:
    t0 = time.time()
    async with stdio_client(SERVER) as (r, w), ClientSession(r, w) as s:
        await s.initialize()
        tools = (await s.list_tools()).tools
        print(f"# connected in {time.time() - t0:.1f}s, {len(tools)} tools", flush=True)
        if argv[:1] == ["--list"]:
            for t in tools:
                schema = getattr(t, "inputSchema", None) or getattr(t, "input_schema", {})
                props, req = schema.get("properties", {}), set(schema.get("required", []))
                print(f"{t.name}({', '.join(p + ('*' if p in req else '') for p in props)})")
            return
        calls = [(argv[i], json.loads(argv[i + 1])) for i in range(0, len(argv), 2)] if argv else DEFAULT
        for name, args in calls:
            print(f"\n## {name} {json.dumps(args)}", flush=True)
            print(text(await s.call_tool(name, args)), flush=True)


if __name__ == "__main__":
    asyncio.run(main(sys.argv[1:]))
