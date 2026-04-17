# Scheduler Management

Use this skill when the user needs to inspect or control timer-based schedule rules.

## When to use
- The user asks to list current schedules.
- The user asks to add a new schedule.
- The user asks to pause, resume, enable, disable, trigger, or reload a schedule.
- The user asks for a timer-based reminder, automation, periodic check, or timed agent wake-up.

## Available capabilities
- `scheduler_list`: list all scheduler entries and runtime state.
- `scheduler_get`: get one scheduler entry by id.
- `scheduler_add`: add a new scheduler entry from `schedule_json` string.
- `scheduler_enable`: enable one scheduler entry by id.
- `scheduler_disable`: disable one scheduler entry by id.
- `scheduler_pause`: pause one scheduler entry by id.
- `scheduler_resume`: resume one scheduler entry by id.
- `scheduler_trigger_now`: trigger one scheduler entry immediately.
- `scheduler_reload`: reload scheduler definitions from disk.

## `scheduler_add` input format
- `schedule_json` is a JSON string, not an object.
- Use a stable, unique `id`. `scheduler_add` fails if the id already exists.

```json
{
  "schedule_json": "<JSON string of one scheduler entry object>"
}
```

## `schedule_json` object fields
- `id`: schedule unique id.
- `enabled`: whether schedule is active. Defaults to `true`.
- `kind`: `interval`, or `cron`.
- `interval_ms`: interval period for `interval`.
- `cron_expr`: 5-field cron expression for `cron` (`minute hour mday month wday`), without seconds.
- `event_type`: event type published to the router. Defaults to `schedule`.
- `event_key`: logical key for router matching and tracing. Defaults to `id`.
- `source_channel`: event source channel (for example `time`, `qq`, `telegram`).
- `chat_id`: target chat id when the router action needs one.
- `content_type`: event content type. Defaults to `trigger`.
- `session_policy`: session policy (`trigger`, `chat`, `global`, `ephemeral`, `nosave`). Defaults to `trigger`.
- `text`: event text payload. For agent wake-up rules, write this as the prompt the agent should reason about.
- `payload_json`: user structured payload. Defaults to `{}`. At trigger time the scheduler wraps it under `user_payload` and also adds schedule metadata.
- `max_runs`: max trigger count. `0` means unlimited.

## Time semantics
- Choose `interval` for relative periodic execution, such as "every 10 seconds", "every 5 minutes", "once per hour after startup", or general periodic polling, or when there are offline running needs.
- Choose `cron` for calendar-based repeated execution, such as "every day at 08:00", "every Monday", "on the 1st day of each month", or "every 3 minutes aligned to wall-clock time". `cron` uses the device time zone stored in NVS and depends on valid wall-clock time. Supported field forms are `*`, `*/N`, or one explicit number in range; for example, `*/3 * * * *`.


## MUST work with Event Router
- Scheduler entries only publish events; they do not define post-trigger behavior.
- For normal scheduled work, set `event_type` to `schedule` and add a matching router rule using `match.event_type` + `match.event_key`.
- Router rule operations and formats are documented in `cap_router_mgr.md` (`add_router_rule` / `update_router_rule`).
- Always choose one router action strategy before adding the schedule between deterministic actions and agent wake-up.

## Router action strategy

### Strategy 1: direct deterministic actions
- Default strategy: use this for fixed reminders, fixed outbound messages, voice reminders, periodic local automation, scripts, or fixed capability calls and other deterministic work.
- Keep the scheduler event as `event_type: "schedule"` and `content_type: "trigger"`.
- Add a router rule whose match is exactly the schedule event: `{"event_type":"schedule","event_key":"<event_key>"}`.
- Use deterministic router actions such as `send_message`, `call_cap`, `run_script`.
- Put the final **user-facing message** in the router action, not in the scheduler `text`, unless the action intentionally renders `{{event.text}}`.

### Strategy 2: send message to wake up agent
- Expensive strategy: use it when the scheduled task requires the agent to reason from current context, decide what to do, choose skills/tools, summarize/check a changing situation, or handle complicated tasks.
- Keep the scheduler event as `event_type: "schedule"` and add a matching router rule with action type `run_agent`.
- Put the agent instruction in `run_agent.input.text` or pass through `{{event.text}}`.
- Set `target_channel` and `target_chat_id` in the `run_agent` input when the eventual agent response should be delivered to a chat.
- Prefer `session_policy: "trigger"` for an independent recurring task thread. Use `chat` only when the scheduled run should share the chat session history.

## Recommended workflow
1. Use `scheduler_list` / `scheduler_get` and `list_router_rules` / `get_router_rule` to inspect current state and avoid id conflicts.
2. Choose the time kind: `interval`, or `cron`.
3. Choose the router action strategy: direct deterministic action or agent wake-up.
4. Add one scheduler entry with a stable `event_key`.
5. Add or update the matching router rule through `cap_router_mgr`.

## Common failure causes
- Adding only the schedule and forgetting the matching router rule.
- Using `event_type: "message"` for a simple reminder, causing unnecessary LLM usage.
- Mismatched `event_key` between the schedule and `match.event_key`.
- Using a 6-field cron expression with seconds; only 5 fields are supported.
- Omitting `chat_id`, `target_channel`, or `target_chat_id` when the action must produce a visible chat message.

## Examples

### `interval + send_message`

Scheduler entry: trigger every 30 seconds, 3 times in total.

```json
{
  "schedule_json": "{\"id\":\"drink_reminder_30s\",\"enabled\":true,\"kind\":\"interval\"，\"interval_ms\":30000,\"cron_expr\":\"\",\"event_type\":\"schedule\",\"event_key\":\"drink_reminder\",\"source_channel\":\"time\",\"chat_id\":\"a_certain_QQ_chat_ID\",\"content_type\":\"trigger\",\"session_policy\":\"trigger\",\"text\":\"drink reminder tick\",\"payload_json\":\"{\\\"message\\\":\\\"time to drink water\\\"}\",\"max_runs\":3}"
}
```

Matching router rule: `send_message`.

### `cron + run_agent`

Scheduler entry: trigger every day at 08:00 and publish a schedule event to wake up the agent.

```json
{
  "schedule_json": "{\"id\":\"daily_agent_check\",\"enabled\":true,\"kind\":\"cron\",\"interval_ms\":0,\"cron_expr\":\"0 8 * * *\",\"event_type\":\"schedule\",\"event_key\":\"daily_agent_check\",\"source_channel\":\"time\",\"chat_id\":\"a_certain_QQ_chat_ID\",\"content_type\":\"trigger\",\"session_policy\":\"trigger\",\"text\":\"Please check the weather today and tell me what I should prepare for going out.\",\"payload_json\":\"{}\",\"max_runs\":0}"
}
```

Matching router action: `run_agent`.

### Pause a schedule

```json
{
  "id": "sedentary_reminder"
}
```
