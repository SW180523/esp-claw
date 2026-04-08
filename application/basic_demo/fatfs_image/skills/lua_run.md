# Lua Script Execution

Use this skill when the user wants to see existing Lua scripts, run one, or inspect async execution jobs.

## Command Rule
- The LLM should run Lua through `cap_cli`, the command name is `lua`.
- Operations are selected by flags such as `--list`, `--run`, `--run-async`, `--jobs`, and `--job`.
- ESP-IDF console argument parsing only recognizes double quotes. Do not wrap JSON in single quotes.

## Running a Script Synchronously
Use `cap_cli` when the user wants immediate output.
- Required: `--run --path <path>`
- Optional: `--args-json "<json>"`, `--timeout-ms <ms>`
- Prefer relative paths such as `hello.lua`

Examples:
```text
lua --run --path hello.lua
lua --run --path hello.lua --args-json "{\"pin\":2}" --timeout-ms 3000
```

Example `cap_cli` input:
```json
{
  "command_line": "lua --run --path blink.lua --args-json \"{\\\"pin\\\":2}\" --timeout-ms 3000"
}
```

If the script expects structured inputs, pass them through `--args-json`. The runtime exposes them to Lua as the global `args`.

## Running a Script Asynchronously
Use `cap_cli` with `lua --run-async` for long-running or continuous scripts.
- Required: `--run-async --path <path>`
- Optional: `--args-json "<json>"`, `--timeout-ms <ms>`

Examples:
```text
lua --run-async --path blink.lua
lua --run-async --path blink.lua --args-json "{\"pin\":2}" --timeout-ms 3000
```

Example `cap_cli` input:
```json
{
  "command_line": "lua --run-async --path blink.lua --args-json \"{\\\"pin\\\":2}\" --timeout-ms 3000"
}
```

After starting an async script:
- Use `cap_cli` with `lua --jobs`
- Use `cap_cli` with `lua --jobs --status running`
- Use `cap_cli` with `lua --job <job_id>`

## Execution Notes
- Paths must resolve under `/spiffs/lua` and end with `.lua`.
- `--timeout-ms` must be a positive integer when provided.
- Prefer synchronous run for short scripts that should finish and return text.
- Prefer async run for loops, animations, watchers, or long-running device behaviors.
- If the user asks to run a script that does not exist yet, switch to the Lua authoring flow first.
