# XIAO ESP32-C6 Device Console

This project turns a Seeed Studio XIAO ESP32-C6 into a Wi-Fi connected LED controller with:

- a web app served directly from the device
- a React frontend loaded from CDN
- Tailwind styling loaded from CDN
- a multi-app layout with Mood, Message, RGB Matrix, Bluetooth, and Device Info sections
- a solid onboard status LED while the device is powered
- a 6x10 WS2812B RGB matrix controller on `A0 / D0 / GPIO 0`
- 41 selectable RGB matrix effects including mood icons and scrolling text
- a BLE scanner
- a device-status app with internal temperature and system telemetry
- matrix effect, brightness, base color, mood, and message persistence across power cycles
- automatic Wi-Fi station startup on boot
- local discovery at `http://xiao.local/` after Wi-Fi connects

## What It Does

On boot, the firmware:

- enables the XIAO ESP32-C6 Wi-Fi hardware
- initializes the BLE scanner
- turns the onboard LED on as a power indicator
- loads the saved RGB matrix effect, brightness, base color, mood, and message
- initializes the WS2812B matrix driver on `A0 / D0 / GPIO 0`
- starts Wi-Fi in station mode using `wifi_config.h`
- starts an HTTP server on port `80` after Wi-Fi connects
- starts mDNS and advertises the device as `xiao.local`
- keeps the RGB matrix running in a non-blocking loop while the onboard LED stays on

The mood app lets you switch between single-color matrix mood icons instantly.
The message app lets you save a short text message that scrolls from right to left across the panel in a loop.
The matrix app lets you switch RGB panel effects instantly, adjust brightness, pick a base color for color-driven effects, and correct the physical matrix mapping when the panel wiring order differs.

## Project Layout

- [xiao_wifi_breathe/xiao_wifi_breathe.ino](xiao_wifi_breathe/xiao_wifi_breathe.ino): thin firmware entrypoint that wires together catalogs, services, apps, and runtime setup
- [xiao_wifi_breathe/apps/](xiao_wifi_breathe/apps): per-app HTTP handlers and JSON builders plus the central app registry
- [xiao_wifi_breathe/services/](xiao_wifi_breathe/services): shared platform services for LED patterns, matrix rendering, BLE, Wi-Fi, and HTTP helpers
- [xiao_wifi_breathe/catalogs/](xiao_wifi_breathe/catalogs): static pattern, mapping, mood, and glyph tables
- [xiao_wifi_breathe/ui/](xiao_wifi_breathe/ui): embedded frontend chunks for the app shell and per-app panels
- [xiao_wifi_breathe/web_app.h](xiao_wifi_breathe/web_app.h): HTML streaming wrapper that serves the chunked embedded frontend
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

The frontend is now split into an app shell and per-app panels, and the shell discovers its visible tabs from `GET /api/apps` instead of hardcoding the app list in one monolithic component.

## Apps

The web UI is split into five visible apps:

- `Mood App`: shows one saved mood icon on the RGB matrix
- `Message App`: scrolls a saved text message right-to-left on the RGB matrix
- `RGB Matrix`: controls a 6x10 WS2812B panel connected to `A0 / D0 / GPIO 0`
- `Bluetooth Scanner`: scans nearby BLE advertisers and shows scan results
- `Device Info`: shows internal chip temperature and device telemetry

The firmware also keeps the legacy onboard LED state routes (`/api/state`, `/api/pattern`, `/api/morse`) as system endpoints even though they are not shown as a visible tab in the current UI.

## HTTP API

### `GET /`

Returns the web app HTML.

### `GET /api/apps`

Returns the ordered app registry used by the frontend shell.

Example:

```json
{
  "apps": [
    {
      "id": "matrix",
      "label": "RGB Matrix",
      "description": "Control the 6x10 WS2812B panel on A0/D0 with animated effects.",
      "routeBase": "/api/matrix",
      "stateRoute": "/api/matrix",
      "actionRoute": "/api/matrix",
      "available": true
    }
  ]
}
```

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
- persisted matrix mapping id/label
- saved mood id
- saved scrolling message text
- available matrix effects
- available wiring/mapping profiles
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
  "mappingId": "cols-bl",
  "mappingLabel": "Columns • Bottom Left",
  "moodId": "happy",
  "messageText": "HELLO",
  "mappings": [
    { "id": "cols-bl", "label": "Columns • Bottom Left" }
  ],
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
  "mappingId": "cols-bl",
  "brightness": 64
}
```

Effect:

- updates the RGB matrix effect immediately
- updates brightness and/or base color immediately
- updates the physical matrix wiring map used to place pixels
- stores the selected matrix settings in NVS
- returns the updated matrix state JSON

### `GET /api/mood`

Returns Mood app state including:

- matrix availability
- the currently saved mood id and label
- whether the matrix is actively showing mood mode
- the list of available moods

### `POST /api/mood`

Request body:

```json
{ "id": "happy" }
```

Effect:

- validates the mood id
- switches the RGB matrix immediately into Mood mode
- stores the selected mood in NVS
- returns the updated mood state JSON

### `GET /api/message`

Returns Message app state including:

- matrix availability
- the saved normalized text
- the active matrix mode
- max message length
- supported characters
- scroll direction

### `POST /api/message`

Request body:

```json
{ "text": "HELLO XIAO" }
```

Effect:

- normalizes text to supported uppercase characters
- switches the RGB matrix immediately into Message mode
- stores the normalized message in NVS
- returns the updated message state JSON

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

In addition, the firmware supports a custom `morse-text` mode that is activated by submitting text to `POST /api/morse`.

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
19. `spiral`
20. `pong`
21. `marquee`
22. `equalizer`
23. `radar`
24. `orbit`
25. `wavefront`
26. `cross`
27. `helix`
28. `tiles`
29. `pixel-spectrum`
30. `raster-trace`
31. `zigzag-trace`
32. `row-spectrum`
33. `column-spectrum`
34. `row-fill`
35. `column-fill`
36. `diagonal-wipe`
37. `spiral-trace`
38. `border-fill`
39. `rain-drops`
40. `mood`
41. `message`

The Mood app ships with these 10 saved mood ids:

1. `happy`
2. `sad`
3. `excited`
4. `cool`
5. `love`
6. `sleepy`
7. `angry`
8. `surprised`
9. `winky`
10. `silly`

The Message app supports `A-Z`, `0-9`, spaces, and `. , ! ? -` with a maximum length of `64` characters.

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
- key: `matrixMap`
- key: `matrixMood`
- key: `matrixMessage`

## Implementation Notes

- The onboard LED is driven as active-low.
- The onboard LED is used as a solid power indicator; it is not exposed as a blinking app in the current UI.
- The RGB matrix uses `Adafruit NeoPixel` on a logical 6x10 grid with selectable column-based wiring maps.
- The web app is embedded in flash via PROGMEM; there is no SPIFFS or LittleFS dependency.
- Wi-Fi is STA-only.
- mDNS is started only after Wi-Fi connects.
- The hostname shown and advertised is `xiao.local`.
- Morse input supports letters, digits, spaces, and `. , ? ! - / @ ( )`.
- Message input supports `A-Z`, `0-9`, spaces, and `. , ! ? -`.
- The matrix firmware assumes the WS2812B data input is wired to `A0 / D0 / GPIO 0`.
- BLE scanning is BLE-only, not classic Bluetooth discovery.
- The default partition scheme is too small; use `PartitionScheme=huge_app`.

## Lessons From This Session

- This hardware setup currently works with column-based mapping profiles only. The default mapping is `cols-bl`.
- Matrix rendering should always happen in logical `row` and `column` space. Direct writes by raw LED index can bypass the selected mapping and make some effects look correct while others are skewed.
- If a matrix change affects layout, validate multiple modes, not just one. Mood icons, message glyphs, previews, and animated effects should all respect the same mapping.
- On a 6x10 panel, mood icons work better as sparse single-color expressions than as full outlined or multicolor emoji faces.
- Compact message glyphs need to be tuned for this panel. Punctuation, especially `-`, should be checked visually because wider symbols may need special handling.
- The message app currently scrolls right-to-left, and that direction should stay in sync between firmware behavior, API metadata, and UI copy.
- Structured effects such as `pixel-spectrum`, `raster-trace`, `zigzag-trace`, `row-fill`, and `column-fill` are the best first-pass tools for checking whether the physical layout is mapped correctly.

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

If matrix shapes look scrambled or some modes look correct while others do not:

- confirm the selected mapping in the `RGB Matrix` app; `cols-bl` is the current default
- test with structured effects such as `pixel-spectrum`, `zigzag-trace`, `row-fill`, and `column-fill`
- treat any mode that only looks correct under one effect as a likely mapping/rendering bug rather than just bad artwork

If upload fails:

- verify the board is on `/dev/cu.usbmodem1101`
- close any serial monitor using the same port

## Git Notes

[xiao_wifi_breathe/wifi_config.h](xiao_wifi_breathe/wifi_config.h) is intentionally ignored by git. Do not commit real Wi-Fi credentials.
