# XIAO ESP32-C6 Device Console

This project turns a Seeed Studio XIAO ESP32-C6 into a Wi-Fi connected LED controller with:

- a web app served directly from the device
- a React frontend loaded from CDN
- Tailwind styling loaded from CDN
- a multi-app layout with Blink, RGB Matrix, Bluetooth, and Device Info sections
- 10 selectable LED patterns
- a custom Morse-code message mode
- a 6x10 WS2812B RGB matrix controller on `A0 / D0 / GPIO 0`
- 18 selectable RGB matrix effects
- a BLE scanner
- a device-status app with internal temperature and system telemetry
- pattern persistence across power cycles
- matrix effect, brightness, and base-color persistence across power cycles
- automatic Wi-Fi station startup on boot
- local discovery at `http://xiao.local/` after Wi-Fi connects

## What It Does

On boot, the firmware:

- enables the XIAO ESP32-C6 Wi-Fi hardware
- initializes the BLE scanner
- loads the last selected LED pattern from NVS (`Preferences`)
- loads the saved RGB matrix effect, brightness, and base color
- initializes the WS2812B matrix driver on `A0 / D0 / GPIO 0`
- starts Wi-Fi in station mode using `wifi_config.h`
- starts an HTTP server on port `80` after Wi-Fi connects
- starts mDNS and advertises the device as `xiao.local`
- keeps both the onboard LED pattern and the RGB matrix running in a non-blocking loop

The web app lets you switch patterns instantly. The selected pattern is saved and restored after reset or power loss.
It also lets you type a custom message that the LED will replay in Morse code. That custom text is stored too.
The matrix app lets you switch RGB panel effects instantly, adjust brightness, and pick a base color for color-driven effects.

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

Important: this firmware requires the `Huge APP (3MB No OTA/1MB SPIFFS)` partition scheme because the default 1.2MB app partition is too small once BLE scanning and the larger multi-app UI are included.

Install the matrix dependency once:

```sh
./bin/arduino-cli lib install --config-file .arduino-cli.yaml "Adafruit NeoPixel"
```

Compile:

```sh
./bin/arduino-cli compile --config-file .arduino-cli.yaml --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app --output-dir build/xiao_wifi_breathe xiao_wifi_breathe
```

Upload:

```sh
./bin/arduino-cli upload --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app xiao_wifi_breathe
```

Serial monitor:

```sh
./bin/arduino-cli monitor --config-file .arduino-cli.yaml -p /dev/cu.usbmodem1101 --fqbn esp32:esp32:XIAO_ESP32C6:PartitionScheme=huge_app
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

## Apps

The web UI is split into four apps:

- `Blink App`: preset LED patterns plus custom Morse playback
- `RGB Matrix`: controls a 6x10 WS2812B panel connected to `A0 / D0 / GPIO 0`
- `Bluetooth Scanner`: scans nearby BLE advertisers and shows scan results
- `Device Info`: shows internal chip temperature and device telemetry

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
  "selectedPatternLabel": "Breathing",
  "morseText": "HELLO XIAO",
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

### `POST /api/morse`

Request body:

```json
{ "text": "HELLO XIAO" }
```

Effect:

- normalizes the supported characters
- converts the text into a looping Morse sequence
- switches the LED immediately into Morse playback
- stores both the active mode and the normalized text in NVS
- returns the updated state JSON

### `GET /api/bluetooth`

Returns BLE scanner state:

- whether Bluetooth scanning is available
- whether a scan is in progress
- last scan timestamps
- scanner error state
- discovered BLE devices and their signal/advertisement details

### `POST /api/bluetooth/scan`

Starts a BLE scan and returns the updated scanner state JSON.

### `GET /api/device`

Returns device telemetry including:

- chip model and revision
- SDK version and CPU frequency
- uptime
- internal chip temperature
- heap and flash stats
- sketch size and free sketch space
- eFuse MAC, IP, Wi-Fi RSSI
- BLE stack status

### `GET /api/matrix`

Returns RGB matrix state including:

- driver availability and error state
- data pin and GPIO number
- matrix dimensions and pixel count
- selected matrix pattern id/label
- persisted base color and brightness
- available matrix effects
- a 6x10 preview frame snapshot

Example:

```json
{
  "available": true,
  "error": "",
  "dataPin": "A0/D0",
  "gpio": 0,
  "rows": 6,
  "columns": 10,
  "pixelCount": 60,
  "selectedPatternId": "rainbow",
  "selectedPatternLabel": "Rainbow Sweep",
  "brightness": 48,
  "color": "#22c55e",
  "patterns": [
    { "id": "rainbow", "label": "Rainbow Sweep" }
  ],
  "frame": ["#001100"]
}
```

### `POST /api/matrix`

Request body accepts one or more of:

```json
{
  "patternId": "scanner",
  "color": "#22c55e",
  "brightness": 64
}
```

Effect:

- updates the RGB matrix effect immediately
- updates brightness and/or base color immediately
- stores the selected matrix settings in NVS
- returns the updated matrix state JSON

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

In addition, the web UI supports a custom `morse-text` mode that is activated by submitting text through the Morse input field.

## RGB Matrix Effects

The matrix app exposes these effects for the 6x10 WS2812B panel:

1. `off`
2. `solid`
3. `rainbow`
4. `scanner`
5. `chase`
6. `pulse`
7. `checker`
8. `sparkle`
9. `plasma`
10. `prism`
11. `ripple`
12. `meteor`
13. `pinwheel`
14. `aurora`
15. `confetti`
16. `static`
17. `glitch`
18. `lava`

The active pattern is persisted in:

- namespace: `xiao-app`
- key: `pattern`

The custom Morse text is persisted in:

- namespace: `xiao-app`
- key: `morseText`

The RGB matrix settings are persisted in:

- namespace: `xiao-app`
- key: `matrixPattern`
- key: `matrixBright`
- key: `matrixColor`

## Implementation Notes

- The onboard LED is driven as active-low.
- The onboard LED engine is non-blocking and uses `millis()`.
- The RGB matrix uses `Adafruit NeoPixel` with a serpentine 6x10 mapping.
- The web app is embedded in flash via PROGMEM; there is no SPIFFS or LittleFS dependency.
- Wi-Fi is STA-only.
- mDNS is started only after Wi-Fi connects.
- The hostname shown and advertised is `xiao.local`.
- Morse input supports letters, digits, spaces, and `. , ? ! - / @ ( )`.
- The matrix firmware assumes the WS2812B data input is wired to `A0 / D0 / GPIO 0`.
- BLE scanning is BLE-only, not classic Bluetooth discovery.
- The default partition scheme is too small; use `PartitionScheme=huge_app`.

## Troubleshooting

If `http://xiao.local/` does not open:

- confirm `wifi_config.h` contains valid credentials
- confirm the board joined your LAN in serial output
- try the device IP shown in the serial log or `/api/state`
- confirm your client OS/network supports mDNS `.local` resolution

If the page loads but looks broken:

- confirm the browser has internet access to fetch the CDN assets

If the RGB matrix does not light:

- confirm the panel data input is wired to `A0 / D0 / GPIO 0`
- confirm the panel has suitable power and shared ground
- open the `RGB Matrix` app and verify `/api/matrix` reports `available: true`

If upload fails:

- verify the board is on `/dev/cu.usbmodem1101`
- close any serial monitor using the same port

## Git Notes

[xiao_wifi_breathe/wifi_config.h](xiao_wifi_breathe/wifi_config.h) is intentionally ignored by git. Do not commit real Wi-Fi credentials.
