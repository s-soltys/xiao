# AGENTS.md

## Project Purpose

This repo contains firmware for a Seeed Studio XIAO ESP32-C6 that:

- connects to Wi-Fi in STA mode
- advertises itself as `xiao.local`
- serves a multi-app web UI from the device
- drives a 6x10 WS2812B RGB matrix on `A0 / D0 / GPIO 0`
- exposes HTTP endpoints for matrix control, BLE scanning, and device telemetry
- keeps the onboard LED constantly on while the device has power
- provides Mood, Message, RGB Matrix, Bluetooth Scanner, and Device Info apps
- supports a custom Morse-code text mode
- supports BLE scanning
- exposes device telemetry including chip temperature
- stores the selected LED and matrix state in NVS (`Preferences`)

## Current Architecture

- Main firmware: [xiao_wifi_breathe/xiao_wifi_breathe.ino](xiao_wifi_breathe/xiao_wifi_breathe.ino)
- Embedded frontend HTML: [xiao_wifi_breathe/web_app.h](xiao_wifi_breathe/web_app.h)
- Local Wi-Fi config template: [xiao_wifi_breathe/wifi_config.example.h](xiao_wifi_breathe/wifi_config.example.h)
- Ignored real Wi-Fi config: [xiao_wifi_breathe/wifi_config.h](xiao_wifi_breathe/wifi_config.h)

Important implementation choices:

- No filesystem hosting: the UI lives in PROGMEM.
- No ArduinoJson dependency: JSON is built manually as strings.
- No async web server: this uses `WebServer` from the ESP32 Arduino core.
- No AP fallback: Wi-Fi is STA-only.
- No auth layer: UI and API are unauthenticated on the LAN.
- The matrix uses column-oriented mapping profiles only; row-oriented mapping profiles were dropped for this hardware.

## Local Tooling

Use the repo-local Arduino CLI and config:

- CLI: `./bin/arduino-cli`
- Config: `./.arduino-cli.yaml`
- FQBN: `esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app`

Preferred commands:

```sh
./bin/arduino-cli compile --config-file .arduino-cli.yaml --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app --output-dir build/xiao_wifi_breathe xiao_wifi_breathe
./bin/arduino-cli upload --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app xiao_wifi_breathe
./bin/arduino-cli monitor --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app
```

## Behavior Contracts

- The active pattern must switch immediately on `POST /api/pattern`.
- The custom Morse text must switch immediately on `POST /api/morse`.
- The active matrix effect, mood, message, color, brightness, and mapping must switch immediately on their respective matrix routes.
- BLE scans must not block the LED loop or the HTTP server.
- The active pattern must persist across reboot/power cycle.
- The custom Morse text must persist when Morse mode is active.
- The active matrix state must persist across reboot/power cycle.
- The device should only become reachable at `xiao.local` after a successful Wi-Fi STA connection.
- The LED loop must stay non-blocking so HTTP handling and Wi-Fi retries continue to work.
- The browser depends on CDN-hosted React, ReactDOM, Tailwind, and Babel. The device serves only the HTML shell.
- The current UI is organized into five apps: Mood, Message, RGB Matrix, Bluetooth Scanner, and Device Info.
- The onboard LED is a solid power indicator in the current product flow; Blink is not a user-facing app anymore.
- The selected matrix mapping must apply consistently to mood rendering, message rendering, preview frames, and all animated matrix effects.
- The message app scroll direction is right-to-left and must stay consistent across firmware, API metadata, UI text, and README copy.

## API Contract

- `GET /`: returns the HTML UI
- `GET /api/state`: returns hostname, Wi-Fi state, IP, selected pattern label/id, pattern list, and current Morse text
- `POST /api/pattern`: accepts JSON body with `"id"` and persists the new pattern
- `POST /api/morse`: accepts JSON body with `"text"` and persists the active Morse message
- `GET /api/matrix`: returns RGB matrix state, available effects, mappings, mood/message data, and a preview frame
- `POST /api/matrix`: updates matrix pattern, color, brightness, and/or mapping
- `GET /api/mood`: returns mood state and available moods
- `POST /api/mood`: accepts JSON body with `"id"` and switches the matrix into mood mode
- `GET /api/message`: returns message state, supported characters, max length, and scroll direction
- `POST /api/message`: accepts JSON body with `"text"` and switches the matrix into message mode
- `GET /api/bluetooth`: returns BLE scanner status and results
- `POST /api/bluetooth/scan`: starts a BLE scan
- `GET /api/device`: returns internal temperature and device telemetry

Avoid changing these routes or the response shape unless the README is updated too.

## Persistence

`Preferences` storage:

- namespace: `xiao-app`
- key: `pattern`
- key: `morseText`
- key: `matrixPattern`
- key: `matrixBright`
- key: `matrixColor`
- key: `matrixMap`
- key: `matrixMood`
- key: `matrixMessage`

If you rename the namespace or key, treat it as a migration-sensitive change.

## Wi-Fi Config Rules

- Real credentials belong only in `xiao_wifi_breathe/wifi_config.h`
- That file is gitignored and should stay ignored
- Never put real credentials in tracked files, docs, examples, or commits
- Keep `wifi_config.example.h` as the tracked template

## Editing Guidance

- Preserve the current board target: XIAO ESP32-C6
- Preserve active-low LED handling unless hardware evidence shows otherwise
- Prefer small, local changes over introducing new libraries
- If adding frontend behavior, keep it embedded in `web_app.h` unless there is a strong reason to add an asset pipeline
- If changing pattern timing or adding patterns, keep the pattern table and API/state output in sync
- If changing Morse support, keep the accepted character set, UI hint text, and request validation in sync
- If changing matrix effects or layout logic, keep the matrix pattern table, API output, README, and UI labels in sync
- Preserve the current column-only matrix mapping model unless the hardware wiring is revalidated
- Default matrix mapping is `cols-bl`; if you change that, document why and verify on real hardware
- Do not render matrix visuals by raw LED index when the output is meant to respect the selected mapping
- Prefer logical `row`/`column` rendering or explicit step-to-coordinate helpers for matrix effects
- Keep mood icons sparse and single-color on this 6x10 panel; dense face outlines and multicolor shading do not survive well on the real hardware
- Keep message glyphs compact and visually validate punctuation, especially `-`, on-device
- If changing BLE scanning, keep the scanner route and Bluetooth app UI in sync
- If changing device telemetry, keep the Device Info app and README field descriptions in sync

## Verification Expectations

After firmware changes, at minimum:

1. compile successfully for `esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app`
2. if relevant, upload to `/dev/cu.usbmodem1101`
3. verify serial boot output for obvious failures
4. if Wi-Fi credentials are present, verify `xiao.local` and the HTTP API
5. if BLE changes are included, verify the Bluetooth scan route returns a clean scan state

For matrix-related changes, also:

6. verify `GET /api/matrix` still reports the expected mapping and pattern list
7. validate at least one structured layout effect such as `pixel-spectrum`, `zigzag-trace`, `row-fill`, or `column-fill`
8. if message or mood rendering changed, visually confirm the actual panel shape, not just the API frame dump

## Known Constraints

- `xiao.local` depends on mDNS support on the client network
- The UI needs browser internet access because frontend assets are CDN-hosted
- There is no fallback AP for recovery
- The JSON parser for `POST /api/pattern` is intentionally minimal and assumes a simple request body
- The JSON parser for `POST /api/morse` is also intentionally minimal; avoid introducing quote-heavy or deeply nested payload shapes without replacing the parser
- BLE scanning uses the ESP32 Arduino BLE stack and adds significant flash usage; keep an eye on partition fit
- The 6x10 matrix is too coarse for detailed emoji-style artwork; expression-first shapes work better than full faces
- Layout bugs can hide if one mode writes by logical coordinates and another writes by physical LED index; keep all matrix modes on the same coordinate model
