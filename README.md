# XIAO ESP32-C6 Device Console

This project turns a Seeed Studio XIAO ESP32-C6 into a Wi-Fi connected LED controller with:

- a web app served directly from the device
- a React frontend loaded from CDN
- Tailwind styling loaded from CDN
- a multi-app layout with RGB Matrix, Solid Glow, Mood, Message, Bluetooth, and Device Info sections
- a Wi-Fi Access Point app for reviewing the hosted SSID, IP address, security mode, and client count
- a solid onboard status LED while the device is powered
- a 6x10 WS2812B RGB matrix controller on `A0 / D0 / GPIO 0`
- 84 selectable RGB matrix effects plus dedicated Solid Glow, Mood, and Message modes
- a BLE scanner
- a device-status app with internal temperature and system telemetry
- matrix effect, enabled state, brightness level, animation speed, dedicated glow color, mood, and message persistence across power cycles
- hosted Wi-Fi access point startup on boot using credentials from `wifi_config.h`
- direct access at `http://192.168.4.1/` once the access point starts

## What It Does

On boot, the firmware:

- enables the XIAO ESP32-C6 Wi-Fi hardware
- initializes the BLE scanner
- turns the onboard LED on as a power indicator
- loads the saved RGB matrix effect, enabled state, brightness, animation speed, dedicated glow color, mood, and message
- initializes the WS2812B matrix driver on `A0 / D0 / GPIO 0`
- starts Wi-Fi in access-point mode using `wifi_config.h`
- starts an HTTP server on port `80` once the hosted network is up
- keeps the RGB matrix running in a non-blocking loop while the onboard LED stays on

The mood app lets you switch between single-color matrix mood icons instantly.
The message app lets you save a short text message that scrolls from right to left across the panel in a loop.
The solid glow app lets you pick a dedicated glow color and fill the whole panel with it immediately.
The matrix app lets you switch RGB panel effects instantly and correct the physical matrix mapping when the panel wiring order differs, while shared matrix power, brightness, and animation speed stay in the shell header.

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

The firmware now hosts its own Wi-Fi access point instead of joining another network.

1. Copy [wifi_config.example.h](xiao_wifi_breathe/wifi_config.example.h) to `xiao_wifi_breathe/wifi_config.h` if needed.
2. Set the hosted network credentials:

```cpp
constexpr char kWifiSsid[] = "xiao-console";
constexpr char kWifiPassword[] = "";
```

3. Rebuild and upload the firmware.
4. Join the hosted SSID from your phone or laptop, then open `http://192.168.4.1/`.

Notes:

- if `kWifiSsid` is empty, the firmware falls back to `xiao-console`
- if `kWifiPassword` is empty, the hosted network is open
- if `kWifiPassword` is between 1 and 7 characters, the firmware logs a warning and starts an open network instead
- the browser may still need cached CDN assets or a separate internet path because the UI shell still references React, ReactDOM, Tailwind, and Babel from CDNs

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

After the device starts its hosted Wi-Fi, open:

- `http://192.168.4.1/`

The browser loads:

- React 18 from CDN
- ReactDOM 18 from CDN
- Tailwind from CDN
- Babel standalone from CDN

The device serves the HTML shell itself, but the browser still needs internet access, cached assets, or a second uplink for those CDN assets.

The frontend is now split into an app shell and per-app panels, and the shell discovers its visible tabs from `GET /api/apps` instead of hardcoding the app list in one monolithic component.

## Apps

The web UI is split into seven visible apps:

- `RGB Matrix`: controls a 6x10 WS2812B panel connected to `A0 / D0 / GPIO 0`
- `Solid Glow`: picks one dedicated glow color and fills the entire matrix with it
- `Mood App`: shows one saved mood icon on the RGB matrix
- `Message App`: scrolls a saved text message right-to-left on the RGB matrix
- `Bluetooth Scanner`: scans nearby BLE advertisers and shows scan results
- `Wi-Fi Access Point`: shows the hosted SSID, IP address, security mode, and current client count
- `Device Info`: shows internal chip temperature and device telemetry

Shared matrix power, brightness, and animation speed live in the header and apply to RGB Matrix, Solid Glow, Mood, and Message together. The header network card opens the `Wi-Fi Access Point` app directly.

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
      "description": "Control the 6x10 WS2812B panel on A0/D0 with animated effects and mapping.",
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
  "hostname": "xiao-ap",
  "address": "192.168.4.1",
  "connected": true,
  "ssid": "xiao-console",
  "ip": "192.168.4.1",
  "selectedPatternId": "breathing",
  "selectedPatternLabel": "Breathing",
  "morseText": "HELLO XIAO",
  "patterns": [
    { "id": "breathing", "label": "Breathing" }
  ]
}
```

`ssid` reports the hosted access-point SSID. `address` and `ip` both point to the hosted device address.

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

### `GET /api/wifi`

Returns Wi-Fi app state including:

- hosted Wi-Fi mode, status, source, and last error
- hosted SSID, address, and IP
- hosted security mode and whether a password is configured
- current and maximum client count

Example:

```json
{
  "available": true,
  "mode": "ap",
  "status": "hosting",
  "connected": true,
  "connecting": false,
  "scanning": false,
  "hostname": "xiao-ap",
  "address": "192.168.4.1",
  "ssid": "xiao-console",
  "targetSsid": "xiao-console",
  "ip": "192.168.4.1",
  "activeSource": "softap",
  "security": "open",
  "passwordConfigured": false,
  "clientCount": 1,
  "maxClients": 4,
  "lastError": "",
  "savedNetworks": [],
  "scanResults": []
}
```

### `POST /api/wifi/scan`

Returns `405` because station scans are disabled in hosted access-point mode.

### `POST /api/wifi/connect`

Returns `405` because the hosted SSID and password are configured in `wifi_config.h`.

### `POST /api/wifi/reorder`

Returns `405` because saved-network priority does not exist in access-point mode.

### `POST /api/wifi/forget`

Returns `405` because there are no runtime-saved station credentials in access-point mode.

### `GET /api/device`

Returns device telemetry including:

- chip model and revision
- SDK version and CPU frequency
- uptime
- internal chip temperature
- heap and flash stats
- sketch size and free sketch space
- eFuse MAC, hosted IP, Wi-Fi mode, security, and client count
- BLE stack status

### `GET /api/matrix`

Returns RGB matrix state including:

- driver availability and error state
- global output enabled state
- data pin and GPIO number
- matrix dimensions and pixel count
- selected matrix pattern id/label
- persisted brightness and stored Solid Glow color
- persisted global animation speed percentage
- persisted matrix mapping id/label
- saved mood id
- saved scrolling message text
- available RGB Matrix app effects
- active fixed matrix mapping profile
- a 6x10 preview frame snapshot

Example:

```json
{
  "available": true,
  "enabled": true,
  "error": "",
  "dataPin": "A0/D0",
  "gpio": 0,
  "rows": 6,
  "columns": 10,
  "pixelCount": 60,
  "selectedPatternId": "scanner",
  "selectedPatternLabel": "Scanner",
  "brightness": 48,
  "animationSpeed": 100,
  "color": "#22c55e",
  "mappingId": "cols-linear-tr",
  "mappingLabel": "Columns Linear • Top Right",
  "moodId": "happy",
  "messageText": "HELLO",
  "mappings": [
    { "id": "cols-linear-tr", "label": "Columns Linear • Top Right" }
  ],
  "patterns": [
    { "id": "scanner", "label": "Scanner" }
  ],
  "frame": ["#001100"]
}
```

### `POST /api/matrix`

Request body accepts one or more of:

```json
{
  "patternId": "scanner",
  "mappingId": "cols-linear-tr",
  "brightness": 64,
  "animationSpeed": 125,
  "enabled": true
}
```

Effect:

- updates the RGB matrix effect immediately
- updates shared output power, brightness, and animation speed immediately
- updates the physical matrix wiring map used to place pixels
- does not change the Solid Glow color
- stores the selected matrix settings in NVS
- returns the updated matrix state JSON

### `GET /api/glow`

Returns Solid Glow app state including:

- matrix availability
- global output enabled state
- current brightness value
- current global animation speed percentage
- dedicated Solid Glow color
- whether Solid Glow is currently active on the panel
- the active matrix pattern id/label

Example:

```json
{
  "available": true,
  "enabled": true,
  "brightness": 48,
  "animationSpeed": 100,
  "color": "#22c55e",
  "glowActive": false,
  "selectedPatternId": "scanner",
  "selectedPatternLabel": "Scanner"
}
```

### `POST /api/glow`

Request body:

```json
{ "color": "#22c55e" }
```

Effect:

- validates and stores the dedicated Solid Glow color
- switches the RGB matrix immediately into full-panel solid glow mode
- returns the updated Solid Glow state JSON

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

The RGB Matrix app exposes these effects for the 6x10 WS2812B panel:

1. `scanner`
2. `spectrum-scan` (`Spectrum Scan`, random color and 4-way sweep directions)
3. `spectrum-storm` (`Spectrum Storm`, super-fast whole-line sweep with random colors, random starting points, and random cardinal directions)
4. `lightning` (`Lightning Trees`)
5. `storm-ripple` (`Storm Ripple`, drifting ripple flash with random colors, origins, and travel directions)
6. `storm-raster` (`Storm Raster`, raster flash with random colors, starting points, and forward/reverse travel)
7. `pride-wave` (`Pride Wave`, waving LGBT flag)
8. `heart-throb` (`Heart Throb`, center double-beat pulse)
9. `beat-wave` (`Beat Wave`, mirrored double-beat column wave)
10. `tunnel-bloom` (`Tunnel Bloom`)
11. `shard-rain` (`Shard Rain`)
12. `orbit-dots` (`Orbit Dots`)
13. `pinwheel` (`Pinwheel`)
14. `lattice-bloom` (`Lattice Bloom`)
15. `sunburst` (`Sunburst`)
16. `circuit-trace` (`Circuit Trace`)
17. `vortex` (`Vortex Well`)
18. `skyline-pulse` (`Skyline Pulse`)
19. `crystal-scan` (`Crystal Scan`)
20. `pulse-diamond` (`Pulse Diamond`)
21. `comet-orbit` (`Comet Orbit`)
22. `chase`
23. `checker`
24. `sparkle`
25. `plasma`
26. `ripple` (`Neon Ripple`)
27. `ripple-dual` (`Dual Ripple`)
28. `ripple-core` (`Pulse Core`)
29. `ripple-surge` (`Neon Surge`)
30. `meteor`
31. `confetti` (`Confetti Burst`)
32. `confetti-pop` (`Confetti Pop`)
33. `confetti-fizz` (`Confetti Fizz`)
34. `confetti-drift` (`Confetti Drift`)
35. `confetti-shower` (`Confetti Shower`)
36. `lava`
37. `spiral`
38. `radar`
39. `helix`
40. `raster-trace`
41. `zigzag-trace`
42. `spiral-trace`
43. `aurora-borealis` (`Aurora Borealis`, brighter shimmering polar wash)
44. `crosscurrent` (`Crosscurrent`, crossing water flow)
45. `singularity-burst` (`Singularity Burst`, pulsing center detonation ring)
46. `ashfall` (`Ashfall`, drifting cooling embers)
47. `prism-fan` (`Prism Fan`, rotating refracted spokes)
48. `swarm-burst` (`Swarm Burst`, outward swarm detonations)
49. `eclipse-ring` (`Eclipse Ring`, shadowed disk with chromatic rim)
50. `data-rain` (`Data Rain`, vertical digital falls)
51. `dune-vault` (`Dune Vault`, arched desert ceiling)
52. `fracture-field` (`Fracture Field`, multiple glowing fractures)
53. `sonar-net` (`Sonar Net`, crossing scan lattice)
54. `hyperspace` (`Hyperspace`, faster tunnel warp)
55. `strobe-tunnel` (`Strobe Tunnel`, pulsing center tunnel)
56. `caustic-flow` (`Caustic Flow`, watery shimmer caustics)

Dedicated matrix-only modes outside that effect picker:

- `solid`: used by the `Solid Glow` app
- `mood`: used by the `Mood App`
- `message`: used by the `Message App`

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
- key: `matrixEnabled`
- key: `matrixBright`
- key: `matrixSpeed`
- key: `matrixColor`
- key: `matrixMap`
- key: `matrixMood`
- key: `matrixMessage`

## Implementation Notes

- The onboard LED is driven as active-low.
- The onboard LED is used as a solid power indicator; it is not exposed as a blinking app in the current UI.
- The RGB matrix uses `Adafruit NeoPixel` on a logical 6x10 grid with selectable column-based wiring maps.
- Shared matrix power, brightness, and animation speed apply to every matrix-facing app.
- The web app is embedded in flash via PROGMEM; there is no SPIFFS or LittleFS dependency.
- Wi-Fi now runs in AP-only mode.
- The hosted SSID comes from `wifi_config.h`, with `xiao-console` as the fallback default if `kWifiSsid` is empty.
- The hosted password must be empty or at least 8 characters; otherwise the firmware logs a warning and starts an open network.
- The browser-facing address is the SoftAP IP, typically `192.168.4.1`.
- Morse input supports letters, digits, spaces, and `. , ? ! - / @ ( )`.
- Message input supports `A-Z`, `0-9`, spaces, and `. , ! ? -`.
- The matrix firmware assumes the WS2812B data input is wired to `A0 / D0 / GPIO 0`.
- BLE scanning is BLE-only, not classic Bluetooth discovery.
- The default partition scheme is too small; use `PartitionScheme=huge_app`.

## Lessons From This Session

- The firmware now uses a single fixed matrix mapping: `cols-linear-tr` (`Columns Linear • Top Right`).
- Matrix rendering should always happen in logical `row` and `column` space. Direct writes by raw LED index can bypass the selected mapping and make some effects look correct while others are skewed.
- If a matrix change affects layout, validate multiple modes, not just one. Mood icons, message glyphs, previews, and animated effects should all respect the same mapping.
- On a 6x10 panel, mood icons work better as sparse single-color expressions than as full outlined or multicolor emoji faces.
- Compact message glyphs need to be tuned for this panel. Punctuation, especially `-`, should be checked visually because wider symbols may need special handling.
- The message app currently scrolls right-to-left, and that direction should stay in sync between firmware behavior, API metadata, and UI copy.
- Structured effects such as `scanner`, `raster-trace`, `zigzag-trace`, and `spiral-trace` are the best first-pass tools for checking whether the physical layout is mapped correctly.

## Troubleshooting

If `http://192.168.4.1/` does not open:

- confirm your client joined the hosted XIAO SSID
- confirm the serial log shows the hosted SSID and SoftAP IP
- confirm `kWifiPassword` is either blank or at least 8 characters
- try `/api/state` directly at the IP shown in the serial log

If the page loads but looks broken:

- confirm the browser has internet access, cached CDN assets, or another uplink to fetch the CDN assets

If the RGB matrix does not light:

- confirm the panel data input is wired to `A0 / D0 / GPIO 0`
- confirm the panel has suitable power and shared ground
- open the `RGB Matrix` app and verify `/api/matrix` reports `available: true`
- confirm matrix output is enabled in the shell header

If matrix shapes look scrambled or some modes look correct while others do not:

- confirm the fixed mapping reported in the `RGB Matrix` app is `cols-linear-tr`
- test with structured effects such as `scanner`, `raster-trace`, `zigzag-trace`, and `spiral-trace`
- treat any mode that only looks correct under one effect as a likely mapping/rendering bug rather than just bad artwork

If upload fails:

- verify the board is on `/dev/cu.usbmodem1101`
- close any serial monitor using the same port

## Git Notes

[xiao_wifi_breathe/wifi_config.h](xiao_wifi_breathe/wifi_config.h) is intentionally ignored by git. Do not commit real Wi-Fi credentials.
