# Lua Script Editing

Use this skill when the user wants to create or modify a Lua script.

## Rules
- Before writing or revising Lua code, first get the board hardware information. When information is lacking, **don't guess**, ask the user.
- Activate the relevant `lua_module_xxx` skills first. They are the source of truth for available Lua modules and function names.
- Only use modules documented by those skills. Do not invent APIs or assume extra Lua packages beyond the runtime's built-ins.
- Write scripts through `lua_write_script`, not `cap_cli`.
- `path` must be a relative `.lua` path under the configured Lua base directory, for example `demo.lua` or `temp/demo.lua`.
- New or unconfirmed scripts must be written under `temp/`.
- When the user confirms the script should be kept, save the final version under `user/`.
- `overwrite` defaults to `true`; set it to `false` only when the user explicitly wants create-only behavior.

## Guidance
- Prefer small scripts with direct use of the documented modules.
- Reuse the same `temp/*.lua` path while iterating on an unconfirmed script.
- When the user keeps the script, move or rewrite it from `temp/` into the matching path under `user/`.
- When revising an existing confirmed script, keep the same path under `user/` unless the user asks to move it.
