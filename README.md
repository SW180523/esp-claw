# ESP-Claw: An AI Agent Framework for IoT Devices

![LOGO](./docs/static/ESP-CLAW-LOGO.jpg)

<p style="text-align: center;">
  <a href="./README.md">English</a> |
  <a href="./README_CN.md">中文</a> |
  <a href="./README_JP.md">日本語</a>
</p>

**ESP-Claw** is an AI agent framework for IoT devices. With a modular architecture, it provides reusable agent capabilities and a runtime foundation for embedded systems. Built on the OpenClaw concept, ESP-Claw adds the following features:

- **Event-driven:** Any event can trigger the Agent Loop and other actions, not just user messages
- **Lua runtime:** Lets the LLM actively plan hardware logic and execution flows
- **Structured memory management:** Organizes memories so they can accumulate and stay useful over time
- **MCP communication:** Supports both standard MCP devices and traditional IoT hardware
- **Ready out of the box:** Fast configuration through Board Manager with one-click flashing
- **Modularized:** Every module can be included or trimmed on demand

![block](./docs/static/block.png)

## Why ESP-Claw: From Cloud-Centric IoT to Edge AI

When an ESP32 gains an Agent brain for reasoning, memory, and decision-making, then pairs it with a Lua cerebellum for orchestration, event nerves for real-time response, and MCP tentacles for sensing and execution, the device is no longer a cloud-dependent puppet. It becomes an independent intelligent agent that manages both data and decisions locally.

- **Decentralized:** from an instruction receiver to an edge-side decision maker
- **Standardized protocols:** eliminate protocol silos through MCP
- **Localized data:** build a physical barrier for privacy
- **Autonomous logic:** move from hard-coded behavior to a dynamic canvas

The following table shows how ESP-Claw differs from traditional IoT devices:

| **Dimension** | **Traditional IoT (Cloud-Centric)** | **ESP-Claw (Edge AI)** |
| --- | --- | --- |
| **Core scenario** | Device connectivity and remote control | Physical-world sensing, decision-making, and control |
| **Processing logic** | Rule triggering (If-This-Then-That) | External event -> execute action |
| **Execution engine** | Rule engine | LLM + Lua + Router (three-tier event handling) |
| **Control center** | Cloud server | Edge node (ESP chip) |
| **Device protocol** | MQTT / Matter / proprietary SDK | MCP as a unified language + multi-protocol bridging |
| **Inter-device communication** | Strong dependence on cloud relay | Direct local links + MCP abstraction |
| **Memory management** | Cloud data storage | Local structured memory (JSONL + tags) |
| **Interaction model** | App / control panel | IM chat (Telegram / WeChat / Feishu) |
| **Extensibility** | Closed ecosystem, high development barrier | Plug-and-play MCP Tools |
| **Intelligence** | Preset automation | LLM + local rules for physical closed loops |

## Event Router: Respond to Events, Drive Actions

Unlike OpenClaw, which primarily reacts to user messages, embedded devices must handle many kinds of external events. Responding to those events and making decisions based on them is one of the core capabilities of ESP-Claw.

In this design, the passive executor concept of a **tool** is strengthened and extended into a **capability**, which works seamlessly with the LLM. A **capability** can both emit events and execute actions. For example:

- IM can deliver user commands, invoke the agent for processing, and send results back to the user
- MCP can receive sensor data, ask the agent to analyze it, and then use MCP again to control actuators
- Lua scripts can call the agent for analysis and store the results locally

To coordinate events and actions among different **capabilities**, the system includes a built-in **event router**, while the agent can dynamically define routing rules.

![event_router](./docs/static/event_router.png)

This makes tasks like the following possible:

- Take photos on a schedule and sync them to an instant messaging app
- Run precise LLM analysis when sensor data becomes abnormal, then send a report to your phone

## Lua Runtime: Self-Evolving Behavior

ESP-Claw embeds a Lua interpreter. The agent can edit, debug, and run Lua scripts on the device, breaking away from the limits of fixed C-only behavior and making self-evolution possible on ESP32 for the first time. With just a natural-language request, ESP-Claw can generate animations or games, optimize control algorithms and parameters, and keep iterating over time.

The repository also provides rich built-in C peripheral bindings and graphics libraries for Lua, allowing users to combine them freely for higher efficiency and more creative applications.

Try ideas like these:

- Connect an LED strip and let the agent generate dynamic lighting effects
- Ask the agent to create a pixel-art mini game
- Let a balancing robot iteratively improve its own algorithm so it runs faster and more steadily
- Build a debugger that collects logs and controls the device for you

## Local Memory System: Learns More About You Over Time

Traditional AI agents usually keep memory only inside the conversation window, which means context is easily lost once the session ends. ESP-Claw implements a complete **structured long-term memory system** directly on the device.

**Five memory types:** user profile (`profile`), user preference (`preference`), factual knowledge (`fact`), device event (`event`), and behavior rule (`rule`)

**Lightweight retrieval:** instead of relying on a vector database, ESP-Claw uses a **summary tag** mechanism. Each memory entry carries 1 to 3 keyword tags. At request time, the system injects the tag pool so the LLM can selectively recall full memory content, enabling efficient retrieval under MCU resource constraints.

**Automatic evolution:** memories keep accumulating through three paths: dialogue extraction, event archiving, and behavior rule consolidation. More importantly, the LLM can discover patterns from those memories and **proactively suggest automations**.

## Rich Peripheral Access: Built for Information Processing

ESP32 is naturally suited for collecting, analyzing, transmitting, and acting on information. ESP-Claw supports cameras, microphones, and many other sensors. Most drivers can be obtained through the ESP-IDF Component Registry, making integration straightforward once the corresponding component is selected. As a result, the device can hear sound and observe the world around it. ESP-Claw also provides a TTS component, allowing it to remind you at the right moment or even play the music you want to hear.

## MCP as a Unified Protocol: Make Devices AI-Native Objects

MCP (Model Context Protocol) is the unified device language of ESP-Claw. The gateway's core responsibility is to **hide all protocol differences** from the upper-layer agent. What the agent always sees is a standardized list of MCP Tools.

**Three-step device onboarding path:**

1. **Discovery** - After power-on, devices broadcast capability information through BLE advertising without requiring a connection. Wi-Fi devices can supplement this with mDNS, and the gateway passively recognizes them.
2. **Registration** - The gateway fetches the device's built-in JSON manifest, automatically generates MCP Tools, and registers them into the tool list. OTA upgrades refresh the registrations incrementally.
3. **Execution** - The AI sends commands through standard MCP Tool Calls. After execution, the device updates its BLE advertising state within 10 ms. The gateway captures the change and pushes it to the agent through SSE, achieving end-to-end latency of roughly 50-220 ms.

**Compatible with existing devices:** for legacy devices that do not support MCP directly, such as Zigbee or Thread devices, the gateway attaches a **Shadow Server** (a virtual MCP Server) internally and performs protocol translation through Lua drivers. Supporting a new protocol only requires implementing and registering the standard `device_driver_t` interface, with no need to change the core modules.

**AI-native semantic interface:** tool naming follows a verb-noun pattern such as `turn_on` and `get_temperature`, while return values include units and freshness metadata. This lets the AI understand and invoke tools without external documentation. Once all devices appear in a unified MCP Tool form, the AI can orchestrate multi-step workflows across devices and unlock **emergent tool-composition behavior**.

## Available Offline

Automation systems, the Lua runtime, and other capabilities continue to work even when the network is unavailable, and they also support **power-loss recovery**. You can assign specific tasks and let the device perform them quietly and reliably in the background:

- People counting: use local AI to recognize faces and count passersby
- Automation gateway: let the agent predefine offline command words and wake words for stable degraded control

## Focused on Specific Workloads

ESP-Claw includes the full OpenClaw workflow. Compared with Claw systems running on a PC, it can stay **more focused** on the tasks you assign in many real-world scenarios. Combined with the memory system, it can keep evolving along with your habits. You can connect it to your most frequently used messaging app and experience how it continuously learns from your usage. **Privacy is not a concern** because all memories stay on the device.

You can ask it to do things like:

- Mount it on your water bottle and use sensors to track your daily water intake
- Help you build habits such as reading or fitness
- Act as a plant-care assistant that manages water and fertilizer while observing the condition of your plants

## Code Architecture

The project follows an **"application example + reusable components"** structure. `application/basic_demo` is a ready-to-build ESP-IDF sample project that assembles Wi-Fi, the configuration page, Lua modules, and various capabilities into a complete device. `components` contains reusable runtime cores, capability plugins, and hardware/script extension modules so the same building blocks can be reused across different boards and scenarios.

The current codebase can be understood as four layers:

- **Application assembly layer:** `application/basic_demo/main`, responsible for the startup entry, network connection, parameter configuration, HTTP config page, and demo-level module registration
- **Capability layer:** `components/claw_capabilities`, responsible for external-facing capabilities including IM communication, MCP Client/Server, the Lua runtime, scheduling, files, time, web search, and more
- **Runtime core layer:** `components/claw_modules`, responsible for agent infrastructure including the core context, capability registration, event routing, memory management, and skill management
- **Device and script extension layer:** `components/lua_modules`, responsible for exposing peripherals such as displays, cameras, audio, buttons, GPIO, storage, and more to Lua and upper-layer agents

The main directory structure of the current repository is as follows:

```text
esp-claw/
├── application/
│   └── basic_demo/
│       ├── main/
│       │   ├── main.c                    # Firmware entry
│       │   ├── app_claw.c                # App bootstrap and assembly
│       │   ├── basic_demo_wifi.*         # Wi-Fi connection and network setup
│       │   ├── basic_demo_settings.*     # Local settings persistence
│       │   ├── config_http_server.*      # Web configuration service
│       │   ├── basic_demo_lua_modules.*  # Lua module registration
│       │   └── web/                      # Frontend assets for device config
│       └── README.md
├── components/
│   ├── claw_modules/
│   │   ├── claw_core/          # Core runtime context
│   │   ├── claw_cap/           # Capability abstraction and registration
│   │   ├── claw_event_router/  # Deterministic event routing
│   │   ├── claw_memory/        # Structured memory management
│   │   └── claw_skill/         # Skill metadata and loading
│   ├── claw_capabilities/
│   │   ├── cap_im_feishu / cap_im_qq / cap_im_tg / cap_im_wechat
│   │   ├── cap_mcp_client / cap_mcp_server
│   │   ├── cap_lua / cap_skill / cap_scheduler / cap_router_mgr
│   │   ├── cap_files / cap_time / cap_web_search / cap_cli
│   │   └── ...                 # More agent-facing capabilities
│   └── lua_modules/
│       ├── lua_module_display / lua_module_camera / lua_module_audio
│       ├── lua_module_button / lua_module_gpio / lua_module_led_strip
│       ├── lua_module_storage / lua_module_delay / lua_module_event_publisher
│       └── esp_painter         # Lightweight drawing support
├── docs/                       # Images and supplementary docs
├── README.md
└── README_CN.md
```

## How to Deploy and Use

### Ready Out of the Box

Configuration and firmware download can be completed directly through the web page. There is no need to compile firmware or install extra software before flashing and using the device.

With the modular architecture of ESP-BoardManager, the project can adapt to and switch across multiple development board configurations.

### Build from Source

[`basic_demo`](./application/basic_demo) provides a foundational example. For detailed build and flashing instructions, please refer to its [README](./application/basic_demo/README.md).

### Notes

- The project is still under active development. If you run into issues, feel free to open an issue at any time.
- Features such as self-programming depend on strong reasoning models. GPT-5.4 or a model with similar capability is recommended for the best experience.

## Follow Us

If this project inspires or helps you, please consider giving it a star. ⭐⭐⭐⭐⭐

Your support is always the biggest motivation for future updates.

## Acknowledgements

ESP-Claw is inspired by [OpenClaw](https://github.com/openclaw/openclaw).

Its implementation of Agent Loop and IM communication on ESP32 was also informed by [MimiClaw](https://github.com/memovai/mimiclaw).

MimiClaw also helped demonstrate the feasibility of running OpenClaw-style agent workflows on ESP32-S3.
