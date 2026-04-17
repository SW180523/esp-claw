# Router Rule Management

Use this skill when the user needs to inspect or change event router automation rules.

## When to use
- The user asks to list, view, add, update, delete, or reload automation rules.
- The user asks to create a rule that reacts to `event_type`/`event_key` and performs actions.
- The user asks to route schedule/trigger/message events to caps, agent, or outbound message actions.

## Available capabilities
- `list_router_rules`: list all rules as JSON array.
- `get_router_rule`: get one rule by `id`.
- `add_router_rule`: add one rule by passing `rule_json` string.
- `update_router_rule`: replace one rule by passing `rule_json` string.
- `delete_router_rule`: delete one rule by `id`.
- `reload_router_rules`: reload rules from disk.

## Calling rules
- Use direct router manager capabilities. Do not route through `run_cli_command` unless explicitly requested.
- `add_router_rule` and `update_router_rule` input must be:
  - `{"rule_json":"<JSON string of one rule object>"}`.
  - `rule_json` is a string, not an object.
- Inside `rule_json`, one rule object must include:
  - `id` (string, required)
  - `match` (object, required)
  - `actions` (non-empty array, required)
  - `match.event_type` (string, required)
- Common optional fields:
  - rule-level: `description`, `enabled`, `consume_on_match`, `ack`, `vars`
  - match-level: `event_key`, `source_cap`, `source_channel` (or `channel`), `chat_id`, `content_type`, `text`
- Action `type` must be one of: `call_cap`, `run_agent`, `run_script`, `send_message`, `emit_event`, `drop`
  - Action requirements:
    `call_cap`: requires `cap` + `input` object
    `run_agent`: `input` object recommended (empty object allowed)
    `run_script`/`send_message`/`emit_event`: require `input` object
    `drop`: no special input needed

## Common failure causes
- Put `event_type` at rule top-level instead of `match.event_type`.
- Pass `rule_json` as object instead of string.
- Missing `actions`, or `actions` is empty.
- Invalid action `type`, or missing required action fields.
- `add_router_rule` used with an existing `id` (use `update_router_rule` instead).

## Recommended workflow
1. Call `list_router_rules` first to understand current rules and avoid id conflicts.
2. Add or update exactly one rule.
3. Call `get_router_rule` with that id to verify final shape.
4. If needed, call `reload_router_rules`.

## Examples

### Action `type`: `send_message`
Add one router rule for schedule published event: when `event_key=drink_reminder`, send QQ message.

```json
{
  "rule_json": "{\"id\":\"drink_reminder_to_qq\",\"description\":\"Send drink reminder to QQ\",\"enabled\":true,\"consume_on_match\":true,\"match\":{\"event_type\":\"schedule\",\"event_key\":\"drink_reminder\"},\"actions\":[{\"type\":\"send_message\",\"input\":{\"channel\":\"qq\",\"chat_id\":\"a_certain_QQ_chat_ID\",\"message\":\"It's time to drink water!\"}}]}"
}
```

### Action `type`: `run_agent`
Add one router rule for schedule published event: when `event_key=daily_agent_check`, wake the agent and let it reason and then respond via QQ.

```json
{
  "rule_json": "{\"id\":\"daily_agent_check_agent\",\"description\":\"Wake agent for daily scheduled check\",\"enabled\":true,\"consume_on_match\":true,\"match\":{\"event_type\":\"schedule\",\"event_key\":\"daily_agent_check\"},\"actions\":[{\"type\":\"run_agent\",\"input\":{\"target_channel\":\"qq\",\"target_chat_id\":\"a_certain_QQ_chat_ID\",\"session_policy\":\"chat\"}}]}"
}
```

### Get one rule

```json
{
  "id": "drink_reminder_qq"
}
```

### Delete one rule

```json
{
  "id": "drink_reminder_qq"
}
```
