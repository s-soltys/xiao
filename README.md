# XIAO ESP32-C6 Pattern Console

This project turns a Seeed Studio XIAO ESP32-C6 into a Wi-Fi connected LED controller with:

- a web app served directly from the device
- a React frontend loaded from CDN
- Tailwind styling loaded from CDN
- 10 selectable LED patterns
- pattern persistence across power cycles
- automatic Wi-Fi station startup on boot
- local discovery at `http://xiao.local/` after Wi-Fi connects

## What It Does

On boot, the firmware:

- enables the XIAO ESP32-C6 Wi-Fi hardware
- loads the last selected LED pattern from NVS (`Preferences`)
- starts Wi-Fi in station mode using `wifi_config.h`
- starts an HTTP server on port `80` after Wi-Fi connects
- starts mDNS and advertises the device as `xiao.local`
- keeps the LED pattern running in a non-blocking loop

The web app lets you switch patterns instantly. The selected pattern is saved and restored after reset or power loss.

## Project Layout

- [xiao_wifi_breathe/xiao_wifi_breathe.ino](xiao_wifi_breathe/xiao_wifi_breathe.ino): main firmware, pattern engine, Wi-Fi, HTTP API, mDNS, persistence
- [xiao_wifi_breathe/web_app.h](xiao_wifi_breathe/web_app.h): inline HTML page with React and Tailwind loaded from CDN
- [xiao_wifi_breathe/wifi_config.example.h](xiao_wifi_breathe/wifi_config.example.h): Wi-Fi config template
- [xiao_wifi_breathe/wifi_config.h](xiao_wifi_breathe/wifi_config.h): local ignored Wi-Fi credentials file
- [.arduino-cli.yaml](.arduino-cli.yaml): local Arduino CLI config for this workspace

## Wi-Fi Configuration

The firmware reads credentials from [wifi_config.h](xiao_wifi_breathe/wifi_config.h), which is ignored by git.

1. Copy [wifi_config.example.h](xiao_wifi_breathe/wifi_config.example.h) to `xiao_wifi_breathe/wifi_config.h` if needed.
2. Set your Wi-Fi credentials:

```cpp
constexpr char kWifiSsid[] = "your-ssid";
constexpr char kWifiPassword[] = "your-password";
```

3. Rebuild and upload the firmware.

If `wifi_config.h` is empty or invalid:

- the LED patterns still work
- the last saved pattern still loads
- no fallback access point is started
- the device will not expose the web app until it joins Wi-Fi

## Build And Upload

This repo uses a local Arduino CLI install under `./bin` and local package/cache directories under `./.arduino`.

Compile:

```sh
./bin/arduino-cli compile --config-file .arduino-cli.yaml --fqbn esp32:esp32:XIAO_ESP32C6 --output-dir build/xiao_wifi_breathe xiao_wifi_breathe
```

Upload:

```sh
./bin/arduino-cli upload --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6 xiao_wifi_breathe
```

Serial monitor:

```sh
./bin/arduino-cli monitor --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6
```

## Web App

After the device joins Wi-Fi, open:

- `http://xiao.local/`

The browser loads:

- React 18 from CDN
- ReactDOM 18 from CDN
- Tailwind from CDN
- Babel standalone from CDN

The device serves the HTML shell itself, but the browser still needs internet access for those CDN assets.

## HTTP API

### `GET /`

Returns the web app HTML.

### `GET /api/state`

Returns current device state:

```json
{
  "hostname": "xiao.local",
  "connected": true,
  "ssid": "your-ssid",
  "ip": "192.168.1.10",
  "selectedPatternId": "breathing",
  "patterns": [
    { "id": "breathing", "label": "Breathing" }
  ]
}
```

### `POST /api/pattern`

Request body:

```json
{ "id": "sos" }
```

Effect:

- applies the new pattern immediately
- stores the new pattern in NVS
- returns the updated state JSON

## Available Patterns

The firmware exposes these 10 patterns:

1. `breathing`
2. `sos`
3. `slow-blink`
4. `fast-blink`
5. `heartbeat`
6. `beacon`
7. `double-blink`
8. `triple-blink`
9. `ramp-pulse`
10. `strobe`

The active pattern is persisted in:

- namespace: `xiao-app`
- key: `pattern`

## Implementation Notes

- The onboard LED is driven as active-low.
- The LED engine is non-blocking and uses `millis()`.
- The web app is embedded in flash via PROGMEM; there is no SPIFFS or LittleFS dependency.
- Wi-Fi is STA-only.
- mDNS is started only after Wi-Fi connects.
- The hostname shown and advertised is `xiao.local`.

## Troubleshooting

If `http://xiao.local/` does not open:

- confirm `wifi_config.h` contains valid credentials
- confirm the board joined your LAN in serial output
- try the device IP shown in the serial log or `/api/state`
- confirm your client OS/network supports mDNS `.local` resolution

If the page loads but looks broken:

- confirm the browser has internet access to fetch the CDN assets

If upload fails:

- verify the board is on `/dev/cu.usbmodem1101`
- close any serial monitor using the same port

## Git Notes

[xiao_wifi_breathe/wifi_config.h](xiao_wifi_breathe/wifi_config.h) is intentionally ignored by git. Do not commit real Wi-Fi credentials.
