# AGENTS.md

## Project Purpose

This repo contains firmware for a Seeed Studio XIAO ESP32-C6 that:

- connects to Wi-Fi in STA mode
- advertises itself as `xiao.local`
- serves a web UI from the device
- exposes HTTP endpoints for LED pattern control
- supports a custom Morse-code text mode
- stores the selected pattern in NVS (`Preferences`)

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

## Local Tooling

Use the repo-local Arduino CLI and config:

- CLI: `./bin/arduino-cli`
- Config: `./.arduino-cli.yaml`
- FQBN: `esp32:esp32:XIAO_ESP32C6`

Preferred commands:

```sh
./bin/arduino-cli compile --config-file .arduino-cli.yaml --fqbn esp32:esp32:XIAO_ESP32C6 --output-dir build/xiao_wifi_breathe xiao_wifi_breathe
./bin/arduino-cli upload --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6 xiao_wifi_breathe
./bin/arduino-cli monitor --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6
```

## Behavior Contracts

- The active pattern must switch immediately on `POST /api/pattern`.
- The custom Morse text must switch immediately on `POST /api/morse`.
- The active pattern must persist across reboot/power cycle.
- The custom Morse text must persist when Morse mode is active.
- The device should only become reachable at `xiao.local` after a successful Wi-Fi STA connection.
- The LED loop must stay non-blocking so HTTP handling and Wi-Fi retries continue to work.
- The browser depends on CDN-hosted React, ReactDOM, Tailwind, and Babel. The device serves only the HTML shell.

## API Contract

- `GET /`: returns the HTML UI
- `GET /api/state`: returns hostname, Wi-Fi state, IP, selected pattern label/id, pattern list, and current Morse text
- `POST /api/pattern`: accepts JSON body with `"id"` and persists the new pattern
- `POST /api/morse`: accepts JSON body with `"text"` and persists the active Morse message

Avoid changing these routes or the response shape unless the README is updated too.

## Persistence

`Preferences` storage:

- namespace: `xiao-app`
- key: `pattern`
- key: `morseText`

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

## Verification Expectations

After firmware changes, at minimum:

1. compile successfully for `esp32:esp32:XIAO_ESP32C6`
2. if relevant, upload to `/dev/cu.usbmodem1101`
3. verify serial boot output for obvious failures
4. if Wi-Fi credentials are present, verify `xiao.local` and the HTTP API

## Known Constraints

- `xiao.local` depends on mDNS support on the client network
- The UI needs browser internet access because frontend assets are CDN-hosted
- There is no fallback AP for recovery
- The JSON parser for `POST /api/pattern` is intentionally minimal and assumes a simple request body
- The JSON parser for `POST /api/morse` is also intentionally minimal; avoid introducing quote-heavy or deeply nested payload shapes without replacing the parser
