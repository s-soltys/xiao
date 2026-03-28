#pragma once

const char kWebAppHtml[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>XIAO Device Console</title>
    <link rel="preconnect" href="https://fonts.googleapis.com" />
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin />
    <link
      href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@400;500;700&display=swap"
      rel="stylesheet"
    />
    <script>
      tailwind = window.tailwind || {};
      tailwind.config = {
        theme: {
          extend: {
            fontFamily: {
              sans: ['Space Grotesk', 'sans-serif'],
            },
            boxShadow: {
              panel: '0 24px 60px -30px rgba(15, 23, 42, 0.4)',
            },
          },
        },
      };
    </script>
    <script src="https://cdn.tailwindcss.com"></script>
    <script crossorigin src="https://unpkg.com/react@18/umd/react.production.min.js"></script>
    <script crossorigin src="https://unpkg.com/react-dom@18/umd/react-dom.production.min.js"></script>
    <script src="https://unpkg.com/@babel/standalone/babel.min.js"></script>
  </head>
  <body class="min-h-screen bg-[radial-gradient(circle_at_top_left,_rgba(13,148,136,0.16),_transparent_34%),radial-gradient(circle_at_bottom_right,_rgba(245,158,11,0.18),_transparent_30%),linear-gradient(180deg,#fff7ed_0%,#fffbf5_46%,#f8fafc_100%)] text-slate-900">
    <div id="root"></div>
    <script type="text/babel">
      const { useEffect, useState } = React;

      function StatusCard({ label, value, tone }) {
        const tones = {
          neutral: 'border-slate-200 bg-white/70 text-slate-700',
          online: 'border-teal-200 bg-teal-50/80 text-teal-800',
          offline: 'border-amber-200 bg-amber-50/80 text-amber-800',
        };

        return (
          <div className={`rounded-2xl border px-4 py-3 shadow-sm ${tones[tone || 'neutral']}`}>
            <div className="text-[11px] font-medium uppercase tracking-[0.22em] opacity-70">{label}</div>
            <div className="mt-1 text-sm font-semibold sm:text-base">{value}</div>
          </div>
        );
      }

      function AppTab({ id, activeApp, onSelect, label, description }) {
        const active = activeApp === id;
        return (
          <button
            type="button"
            onClick={() => onSelect(id)}
            className={[
              'rounded-3xl border px-4 py-4 text-left transition',
              active
                ? 'border-slate-950 bg-slate-950 text-white shadow-panel'
                : 'border-white/70 bg-white/80 text-slate-900 hover:border-slate-300 hover:bg-white',
            ].join(' ')}
          >
            <div className="text-sm font-bold">{label}</div>
            <div className={`mt-2 text-sm ${active ? 'text-slate-300' : 'text-slate-500'}`}>{description}</div>
          </button>
        );
      }

      function MetricCard({ label, value, hint }) {
        return (
          <div className="rounded-3xl border border-white/70 bg-white/85 px-4 py-4 shadow-sm">
            <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-slate-500">{label}</div>
            <div className="mt-2 text-2xl font-bold text-slate-900">{value}</div>
            {hint ? <div className="mt-2 text-sm text-slate-500">{hint}</div> : null}
          </div>
        );
      }

      function formatBytes(value) {
        if (value === null || value === undefined) {
          return '—';
        }

        if (value >= 1024 * 1024) {
          return `${(value / (1024 * 1024)).toFixed(2)} MB`;
        }

        if (value >= 1024) {
          return `${(value / 1024).toFixed(1)} KB`;
        }

        return `${value} B`;
      }

      function formatUptime(ms) {
        if (!ms && ms !== 0) {
          return '—';
        }

        const totalSeconds = Math.floor(ms / 1000);
        const hours = Math.floor(totalSeconds / 3600);
        const minutes = Math.floor((totalSeconds % 3600) / 60);
        const seconds = totalSeconds % 60;
        return `${hours}h ${minutes}m ${seconds}s`;
      }

      function formatRelativeMs(ms) {
        if (!ms) {
          return '—';
        }

        return formatUptime(ms);
      }

      function labelForApp(appId) {
        if (appId === 'blink') {
          return 'Blink App';
        }
        if (appId === 'matrix') {
          return 'RGB Matrix';
        }
        if (appId === 'bluetooth') {
          return 'Bluetooth Scanner';
        }
        return 'Device Info';
      }

      function App() {
        const [activeApp, setActiveApp] = useState('blink');
        const [blinkState, setBlinkState] = useState(null);
        const [matrixState, setMatrixState] = useState(null);
        const [bluetoothState, setBluetoothState] = useState(null);
        const [deviceState, setDeviceState] = useState(null);
        const [loading, setLoading] = useState(true);
        const [refreshing, setRefreshing] = useState(false);
        const [blinkBusyId, setBlinkBusyId] = useState('');
        const [matrixBusy, setMatrixBusy] = useState(false);
        const [scanBusy, setScanBusy] = useState(false);
        const [error, setError] = useState('');
        const [morseInput, setMorseInput] = useState('');
        const [matrixColor, setMatrixColor] = useState('#22c55e');
        const [matrixBrightness, setMatrixBrightness] = useState('48');
        const [matrixInputsReady, setMatrixInputsReady] = useState(false);

        async function fetchJson(url, options) {
          const response = await fetch(url, options);
          const payload = await response.json();
          if (!response.ok) {
            throw new Error(payload.error || `Request failed with ${response.status}`);
          }
          return payload;
        }

        async function fetchBlinkState() {
          const payload = await fetchJson('/api/state', { cache: 'no-store' });
          setBlinkState(payload);
          setMorseInput(payload.morseText || '');
          return payload;
        }

        async function fetchMatrixState(syncInputs = false) {
          const payload = await fetchJson('/api/matrix', { cache: 'no-store' });
          setMatrixState(payload);
          if (syncInputs || !matrixInputsReady) {
            setMatrixColor(payload.color || '#22c55e');
            setMatrixBrightness(String(payload.brightness ?? 48));
            setMatrixInputsReady(true);
          }
          return payload;
        }

        async function fetchBluetoothState() {
          const payload = await fetchJson('/api/bluetooth', { cache: 'no-store' });
          setBluetoothState(payload);
          return payload;
        }

        async function fetchDeviceState() {
          const payload = await fetchJson('/api/device', { cache: 'no-store' });
          setDeviceState(payload);
          return payload;
        }

        async function refreshAll(initial = false) {
          if (initial) {
            setLoading(true);
          }
          setRefreshing(true);
          setError('');

          const [blinkResult, matrixResult, bluetoothResult, deviceResult] = await Promise.allSettled([
            fetchBlinkState(),
            fetchMatrixState(true),
            fetchBluetoothState(),
            fetchDeviceState(),
          ]);

          const firstError = [blinkResult, matrixResult, bluetoothResult, deviceResult].find((result) => result.status === 'rejected');
          if (firstError) {
            setError(firstError.reason?.message || 'Unable to refresh one or more apps.');
          }

          setLoading(false);
          setRefreshing(false);
        }

        useEffect(() => {
          refreshAll(true);
        }, []);

        useEffect(() => {
          if (!bluetoothState?.scanning) {
            return undefined;
          }

          const intervalId = window.setInterval(() => {
            fetchBluetoothState().catch((requestError) => {
              setError(requestError.message || 'Unable to refresh Bluetooth state.');
            });
          }, 1500);

          return () => window.clearInterval(intervalId);
        }, [bluetoothState?.scanning]);

        useEffect(() => {
          if (activeApp !== 'device') {
            return undefined;
          }

          const intervalId = window.setInterval(() => {
            fetchDeviceState().catch(() => {});
          }, 5000);

          return () => window.clearInterval(intervalId);
        }, [activeApp]);

        useEffect(() => {
          if (activeApp !== 'matrix') {
            return undefined;
          }

          const intervalId = window.setInterval(() => {
            fetchMatrixState(false).catch(() => {});
          }, 700);

          return () => window.clearInterval(intervalId);
        }, [activeApp]);

        async function activatePattern(patternId) {
          setBlinkBusyId(patternId);
          setError('');

          try {
            const payload = await fetchJson('/api/pattern', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ id: patternId }),
            });
            setBlinkState(payload);
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the active pattern.');
          } finally {
            setBlinkBusyId('');
          }
        }

        async function activateMorse(event) {
          event.preventDefault();
          setBlinkBusyId('morse-text');
          setError('');

          try {
            const payload = await fetchJson('/api/morse', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ text: morseInput }),
            });
            setBlinkState(payload);
            setMorseInput(payload.morseText || '');
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the Morse pattern.');
          } finally {
            setBlinkBusyId('');
          }
        }

        async function updateMatrixSettings(changes) {
          setMatrixBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/matrix', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify(changes),
            });
            setMatrixState(payload);
            setMatrixColor(payload.color || '#22c55e');
            setMatrixBrightness(String(payload.brightness ?? 48));
            setMatrixInputsReady(true);
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the RGB matrix.');
          } finally {
            setMatrixBusy(false);
          }
        }

        async function applyMatrixControls(event) {
          event.preventDefault();
          const brightnessValue = Number(matrixBrightness);
          if (!Number.isFinite(brightnessValue) || brightnessValue < 0 || brightnessValue > 255) {
            setError('Matrix brightness must be between 0 and 255.');
            return;
          }

          await updateMatrixSettings({
            color: matrixColor,
            brightness: Math.round(brightnessValue),
          });
        }

        async function startBluetoothScan() {
          setScanBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/bluetooth/scan', {
              method: 'POST',
            });
            setBluetoothState(payload);
          } catch (requestError) {
            setError(requestError.message || 'Unable to start the Bluetooth scan.');
          } finally {
            setScanBusy(false);
          }
        }

        if (loading && !blinkState) {
          return (
            <main className="mx-auto flex min-h-screen max-w-6xl items-center px-5 py-10">
              <div className="w-full rounded-[2rem] border border-white/70 bg-white/70 p-8 shadow-panel backdrop-blur">
                <p className="text-sm uppercase tracking-[0.3em] text-teal-700">Bootstrapping</p>
                <h1 className="mt-3 text-3xl font-bold sm:text-5xl">XIAO Device Console</h1>
                <p className="mt-4 max-w-2xl text-base text-slate-600 sm:text-lg">
                  Waiting for the device apps to report their state.
                </p>
              </div>
            </main>
          );
        }

        const sortedBleResults = (bluetoothState?.results || []).slice().sort((left, right) => right.rssi - left.rssi);
        const activeAppLabel = labelForApp(activeApp);
        const previewFrame =
          matrixState?.frame || Array.from({ length: (matrixState?.rows || 6) * (matrixState?.columns || 10) }, () => '#000000');
        const previewColumns = matrixState?.columns || 10;

        return (
          <main className="mx-auto flex min-h-screen max-w-7xl px-5 py-8 sm:py-10">
            <div className="w-full rounded-[2rem] border border-white/80 bg-white/75 p-6 shadow-panel backdrop-blur sm:p-8">
              <div className="flex flex-col gap-8">
                <section className="grid gap-6 lg:grid-cols-[1.4fr_1fr]">
                  <div>
                    <p className="text-xs font-medium uppercase tracking-[0.35em] text-teal-700">Multi-App Device Console</p>
                    <h1 className="mt-3 text-4xl font-bold leading-tight text-slate-900 sm:text-5xl">
                      XIAO Device Console
                    </h1>
                    <p className="mt-4 max-w-3xl text-base text-slate-600 sm:text-lg">
                      Switch between Blink, RGB Matrix, Bluetooth Scanner, and Device Info. All four apps are served
                      directly from the XIAO ESP32-C6.
                    </p>
                  </div>

                  <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-1">
                    <StatusCard
                      label="Reachability"
                      value={blinkState?.connected ? blinkState.hostname : 'Waiting for Wi-Fi'}
                      tone={blinkState?.connected ? 'online' : 'offline'}
                    />
                    <StatusCard
                      label="Network"
                      value={blinkState?.connected ? `${blinkState.ssid} • ${blinkState.ip}` : 'STA mode retrying'}
                      tone={blinkState?.connected ? 'neutral' : 'offline'}
                    />
                    <StatusCard
                      label="Hardware"
                      value={deviceState ? `${deviceState.chipModel} rev ${deviceState.chipRevision}` : 'Loading chip info'}
                      tone="neutral"
                    />
                  </div>
                </section>

                <section className="grid gap-3 lg:grid-cols-4">
                  <AppTab
                    id="blink"
                    activeApp={activeApp}
                    onSelect={setActiveApp}
                    label="Blink App"
                    description="Preset onboard LED patterns plus custom Morse playback."
                  />
                  <AppTab
                    id="matrix"
                    activeApp={activeApp}
                    onSelect={setActiveApp}
                    label="RGB Matrix"
                    description="Control the 6x10 WS2812B panel on A0/D0 with animated effects."
                  />
                  <AppTab
                    id="bluetooth"
                    activeApp={activeApp}
                    onSelect={setActiveApp}
                    label="Bluetooth Scanner"
                    description="Scan nearby BLE advertisers and inspect signal details."
                  />
                  <AppTab
                    id="device"
                    activeApp={activeApp}
                    onSelect={setActiveApp}
                    label="Device Info"
                    description="Chip telemetry, internal temperature, memory, flash, and radio status."
                  />
                </section>

                <section className="flex flex-wrap items-center justify-between gap-3">
                  <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-4 text-white">
                    <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">Current Focus</div>
                    <div className="mt-2 text-2xl font-bold sm:text-3xl">{activeAppLabel}</div>
                  </div>

                  <button
                    type="button"
                    onClick={() => refreshAll(false)}
                    disabled={refreshing}
                    className="inline-flex items-center justify-center rounded-full border border-slate-300 bg-white px-5 py-3 text-sm font-semibold text-slate-800 transition hover:border-slate-900 hover:text-slate-900 disabled:opacity-60"
                  >
                    {refreshing ? 'Refreshing…' : 'Refresh Apps'}
                  </button>
                </section>

                {error ? (
                  <div className="rounded-2xl border border-rose-200 bg-rose-50 px-4 py-3 text-sm font-medium text-rose-800">
                    {error}
                  </div>
                ) : null}

                {activeApp === 'blink' ? (
                  <section className="grid gap-6">
                    <div className="grid gap-4 lg:grid-cols-[1fr_1fr]">
                      <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-4 text-white">
                        <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">Active Pattern</div>
                        <div className="mt-2 text-2xl font-bold sm:text-3xl">
                          {blinkState?.selectedPatternLabel || 'Unknown'}
                        </div>
                        {blinkState?.selectedPatternId === 'morse-text' && blinkState?.morseText ? (
                          <div className="mt-3 rounded-2xl border border-white/10 bg-white/5 px-3 py-2 text-sm text-slate-200">
                            Message: <span className="font-semibold text-white">{blinkState.morseText}</span>
                          </div>
                        ) : null}
                      </div>

                      <form
                        className="grid gap-3 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm"
                        onSubmit={activateMorse}
                      >
                        <div>
                          <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Custom Morse</div>
                          <h2 className="mt-2 text-2xl font-bold text-slate-900">Type a message to blink in Morse code</h2>
                          <p className="mt-2 text-sm text-slate-600">
                            Supported characters: letters, digits, spaces, and <span className="font-medium">. , ? ! - / @ ( )</span>.
                          </p>
                        </div>

                        <input
                          type="text"
                          value={morseInput}
                          onChange={(event) => setMorseInput(event.target.value)}
                          maxLength={64}
                          placeholder="HELLO XIAO"
                          disabled={Boolean(blinkBusyId)}
                          className="w-full rounded-2xl border border-slate-300 bg-white px-4 py-3 text-base text-slate-900 outline-none transition placeholder:text-slate-400 focus:border-teal-500 focus:ring-2 focus:ring-teal-200 disabled:opacity-60"
                        />

                        <button
                          type="submit"
                          disabled={Boolean(blinkBusyId)}
                          className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                        >
                          {blinkBusyId === 'morse-text' ? 'Saving Morse…' : 'Blink Morse'}
                        </button>
                      </form>
                    </div>

                    <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-5">
                      {blinkState?.patterns?.map((pattern, index) => {
                        const isActive = pattern.id === blinkState.selectedPatternId;
                        const isBusy = blinkBusyId === pattern.id;

                        return (
                          <button
                            key={pattern.id}
                            type="button"
                            onClick={() => activatePattern(pattern.id)}
                            disabled={Boolean(blinkBusyId)}
                            className={[
                              'group min-h-[132px] rounded-3xl border px-4 py-4 text-left transition duration-200',
                              'focus:outline-none focus:ring-2 focus:ring-teal-400 focus:ring-offset-2',
                              isActive
                                ? 'border-slate-950 bg-slate-950 text-white shadow-panel'
                                : 'border-white/70 bg-white/80 text-slate-900 hover:-translate-y-0.5 hover:border-slate-300 hover:bg-white',
                              blinkBusyId && !isBusy ? 'opacity-60' : '',
                            ].join(' ')}
                          >
                            <div className="text-[11px] font-medium uppercase tracking-[0.28em] opacity-70">
                              {String(index + 1).padStart(2, '0')}
                            </div>
                            <div className="mt-4 text-xl font-bold">{pattern.label}</div>
                            <div className={`mt-3 text-sm ${isActive ? 'text-teal-300' : 'text-slate-500'}`}>
                              {isBusy ? 'Switching now…' : isActive ? 'Stored and running' : 'Tap to activate'}
                            </div>
                          </button>
                        );
                      })}
                    </section>
                  </section>
                ) : null}

                {activeApp === 'matrix' ? (
                  <section className="grid gap-6">
                    <div className="grid gap-4 lg:grid-cols-[1.08fr_0.92fr]">
                      <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-5 text-white">
                        <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">RGB Matrix Output</div>
                        <div className="mt-2 text-2xl font-bold sm:text-3xl">
                          {matrixState?.selectedPatternLabel || 'Loading effect'}
                        </div>
                        <p className="mt-3 max-w-2xl text-sm leading-6 text-slate-300">
                          Drives a 6x10 WS2812B panel from the XIAO ESP32-C6 on <span className="font-semibold text-white">A0/D0</span>.
                          The preview below refreshes while this app is open.
                        </p>
                        <div className="mt-4 flex flex-wrap gap-2 text-xs font-medium uppercase tracking-[0.2em] text-slate-300">
                          <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{matrixState?.pixelCount || 60} pixels</span>
                          <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">
                            Brightness {matrixState?.brightness ?? 0}/255
                          </span>
                          <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{matrixState?.dataPin || 'A0/D0'}</span>
                        </div>
                      </div>

                      <form
                        className="grid gap-4 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm"
                        onSubmit={applyMatrixControls}
                      >
                        <div>
                          <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Panel Controls</div>
                          <h2 className="mt-2 text-2xl font-bold text-slate-900">Color and brightness</h2>
                          <p className="mt-2 text-sm text-slate-600">
                            Pick a base color for the effect palette and tune output intensity from 0 to 255.
                          </p>
                        </div>

                        <div className="grid gap-4 sm:grid-cols-[auto_1fr] sm:items-center">
                          <input
                            type="color"
                            value={matrixColor}
                            onChange={(event) => setMatrixColor(event.target.value)}
                            disabled={matrixBusy || !matrixState?.available}
                            className="h-16 w-full cursor-pointer rounded-2xl border border-slate-300 bg-white p-2 disabled:cursor-not-allowed disabled:opacity-60 sm:w-24"
                          />
                          <div className="grid gap-2">
                            <label className="text-[11px] font-medium uppercase tracking-[0.24em] text-slate-500">Brightness</label>
                            <input
                              type="range"
                              min="0"
                              max="255"
                              step="1"
                              value={matrixBrightness}
                              onChange={(event) => setMatrixBrightness(event.target.value)}
                              disabled={matrixBusy || !matrixState?.available}
                              className="w-full accent-teal-600 disabled:opacity-60"
                            />
                            <div className="flex items-center justify-between text-sm text-slate-500">
                              <span>{matrixColor}</span>
                              <span>{matrixBrightness}/255</span>
                            </div>
                          </div>
                        </div>

                        <button
                          type="submit"
                          disabled={matrixBusy || !matrixState?.available}
                          className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                        >
                          {matrixBusy ? 'Applying Matrix…' : 'Apply Matrix Controls'}
                        </button>
                      </form>
                    </div>

                    <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
                      <StatusCard
                        label="Matrix Driver"
                        value={matrixState?.available ? 'Ready' : 'Unavailable'}
                        tone={matrixState?.available ? 'online' : 'offline'}
                      />
                      <StatusCard
                        label="Geometry"
                        value={`${matrixState?.rows || 6} x ${matrixState?.columns || 10}`}
                        tone="neutral"
                      />
                      <StatusCard
                        label="Base Color"
                        value={matrixState?.color || '#22c55e'}
                        tone="neutral"
                      />
                      <StatusCard
                        label="Active Effect"
                        value={matrixState?.selectedPatternLabel || 'Loading'}
                        tone="neutral"
                      />
                    </section>

                    {matrixState?.error ? (
                      <div className="rounded-2xl border border-amber-200 bg-amber-50 px-4 py-3 text-sm font-medium text-amber-800">
                        {matrixState.error}
                      </div>
                    ) : null}

                    <div className="grid gap-4 xl:grid-cols-[1.15fr_0.85fr]">
                      <div className="rounded-[1.75rem] border border-slate-200 bg-slate-950 p-5 shadow-sm">
                        <div className="flex flex-wrap items-center justify-between gap-3">
                          <div>
                            <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-300">Live Preview</div>
                            <h2 className="mt-2 text-2xl font-bold text-white">6x10 panel snapshot</h2>
                          </div>
                          <div className="rounded-full border border-white/10 bg-white/5 px-3 py-1 text-xs font-medium uppercase tracking-[0.2em] text-slate-300">
                            Refreshes every 0.7s
                          </div>
                        </div>

                        <div
                          className="mt-5 grid gap-1 rounded-[1.4rem] border border-white/10 bg-black/40 p-4"
                          style={{ gridTemplateColumns: `repeat(${previewColumns}, minmax(0, 1fr))` }}
                        >
                          {previewFrame.map((color, index) => (
                            <div
                              key={index}
                              className="aspect-square rounded-[0.7rem] border border-white/5"
                              style={{
                                backgroundColor: color,
                                boxShadow: color === '#000000' ? 'inset 0 1px 1px rgba(255,255,255,0.05)' : `0 0 14px ${color}55`,
                              }}
                            />
                          ))}
                        </div>
                      </div>

                      <div className="grid gap-3">
                        <MetricCard
                          label="GPIO"
                          value={matrixState?.gpio === null || matrixState?.gpio === undefined ? '—' : matrixState.gpio}
                          hint={matrixState?.dataPin || 'A0/D0 data output'}
                        />
                        <MetricCard
                          label="Pixels"
                          value={matrixState?.pixelCount || 60}
                          hint="WS2812B 1010 LEDs in a 6x10 grid"
                        />
                        <MetricCard
                          label="Brightness"
                          value={`${matrixState?.brightness ?? 0}/255`}
                          hint="Applied after the effect frame is rendered"
                        />
                        <div className="rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
                          <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Wiring Note</div>
                          <h2 className="mt-2 text-2xl font-bold text-slate-900">Panel input</h2>
                          <p className="mt-3 text-sm leading-6 text-slate-600">
                            This firmware assumes the matrix data input is connected to <span className="font-medium text-slate-900">A0 / D0 / GPIO 0</span>.
                            If the panel looks mirrored or snakes the wrong way, the firmware mapping can be adjusted later.
                          </p>
                        </div>
                      </div>
                    </div>

                    <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
                      {matrixState?.patterns?.map((pattern, index) => {
                        const isActive = pattern.id === matrixState.selectedPatternId;
                        return (
                          <button
                            key={pattern.id}
                            type="button"
                            onClick={() => updateMatrixSettings({ patternId: pattern.id })}
                            disabled={matrixBusy || !matrixState?.available}
                            className={[
                              'group min-h-[128px] rounded-3xl border px-4 py-4 text-left transition duration-200',
                              'focus:outline-none focus:ring-2 focus:ring-teal-400 focus:ring-offset-2',
                              isActive
                                ? 'border-slate-950 bg-slate-950 text-white shadow-panel'
                                : 'border-white/70 bg-white/80 text-slate-900 hover:-translate-y-0.5 hover:border-slate-300 hover:bg-white',
                              matrixBusy ? 'opacity-60' : '',
                            ].join(' ')}
                          >
                            <div className="text-[11px] font-medium uppercase tracking-[0.28em] opacity-70">
                              {String(index + 1).padStart(2, '0')}
                            </div>
                            <div className="mt-4 text-xl font-bold">{pattern.label}</div>
                            <div className={`mt-3 text-sm ${isActive ? 'text-teal-300' : 'text-slate-500'}`}>
                              {isActive ? 'Running on the panel' : 'Tap to activate'}
                            </div>
                          </button>
                        );
                      })}
                    </section>
                  </section>
                ) : null}

                {activeApp === 'bluetooth' ? (
                  <section className="grid gap-6">
                    <section className="grid gap-4 lg:grid-cols-[1fr_auto] lg:items-end">
                      <div className="rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
                        <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Nearby BLE Devices</div>
                        <h2 className="mt-2 text-3xl font-bold text-slate-900">Bluetooth Scanner</h2>
                        <p className="mt-2 text-sm text-slate-600">
                          Runs a {bluetoothState?.durationSeconds || 5}-second BLE scan and lists nearby advertisers.
                        </p>
                      </div>

                      <button
                        type="button"
                        onClick={startBluetoothScan}
                        disabled={scanBusy || bluetoothState?.scanning || !bluetoothState?.available}
                        className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                      >
                        {bluetoothState?.scanning ? 'Scanning…' : scanBusy ? 'Starting Scan…' : 'Start BLE Scan'}
                      </button>
                    </section>

                    <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
                      <StatusCard
                        label="Scanner"
                        value={bluetoothState?.available ? 'Ready' : 'Unavailable'}
                        tone={bluetoothState?.available ? 'online' : 'offline'}
                      />
                      <StatusCard
                        label="State"
                        value={bluetoothState?.scanning ? 'Scan in progress' : 'Idle'}
                        tone={bluetoothState?.scanning ? 'online' : 'neutral'}
                      />
                      <StatusCard
                        label="Results"
                        value={`${bluetoothState?.results?.length || 0} devices`}
                        tone="neutral"
                      />
                      <StatusCard
                        label="Last Complete"
                        value={formatRelativeMs(bluetoothState?.lastCompletedAtMs)}
                        tone="neutral"
                      />
                    </section>

                    {bluetoothState?.error ? (
                      <div className="rounded-2xl border border-amber-200 bg-amber-50 px-4 py-3 text-sm font-medium text-amber-800">
                        {bluetoothState.error}
                      </div>
                    ) : null}

                    <section className="grid gap-3">
                      {sortedBleResults.length === 0 ? (
                        <div className="rounded-[1.75rem] border border-dashed border-slate-300 bg-white/70 px-5 py-10 text-center text-sm text-slate-500">
                          {bluetoothState?.scanning
                            ? 'Scanning for nearby BLE devices…'
                            : 'No BLE devices captured yet. Start a scan to populate this list.'}
                        </div>
                      ) : (
                        sortedBleResults.map((device) => (
                          <div
                            key={device.address}
                            className="grid gap-3 rounded-[1.5rem] border border-white/70 bg-white/85 px-5 py-4 shadow-sm lg:grid-cols-[1.4fr_repeat(4,minmax(0,1fr))]"
                          >
                            <div>
                              <div className="text-lg font-bold text-slate-900">{device.name || 'Unnamed advertiser'}</div>
                              <div className="mt-1 text-sm text-slate-500">{device.address}</div>
                              {device.serviceUuid ? (
                                <div className="mt-2 inline-flex rounded-full bg-slate-100 px-3 py-1 text-xs font-medium text-slate-700">
                                  Service {device.serviceUuid}
                                </div>
                              ) : null}
                            </div>
                            <MetricCard label="RSSI" value={`${device.rssi} dBm`} hint={device.connectable ? 'Connectable' : 'Broadcast only'} />
                            <MetricCard
                              label="TX Power"
                              value={device.hasTxPower ? `${device.txPower} dBm` : '—'}
                              hint={device.scannable ? 'Scannable' : 'Not scannable'}
                            />
                            <MetricCard label="Address Type" value={device.addressType} hint={device.legacy ? 'Legacy advertisement' : 'Extended capable'} />
                            <MetricCard label="Mode" value={device.connectable ? 'Connectable' : 'Observer'} hint={device.name ? 'Name broadcast present' : 'Name missing'} />
                          </div>
                        ))
                      )}
                    </section>
                  </section>
                ) : null}

                {activeApp === 'device' ? (
                  <section className="grid gap-6">
                    <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
                      <MetricCard
                        label="Chip Temperature"
                        value={deviceState?.temperatureC === null || deviceState?.temperatureC === undefined ? '—' : `${deviceState.temperatureC.toFixed(1)} °C`}
                        hint="Internal sensor exposed by the ESP32 Arduino core"
                      />
                      <MetricCard label="Uptime" value={formatUptime(deviceState?.uptimeMs)} hint="Since last boot" />
                      <MetricCard
                        label="Wi-Fi RSSI"
                        value={deviceState?.wifiRssi === null || deviceState?.wifiRssi === undefined ? '—' : `${deviceState.wifiRssi} dBm`}
                        hint={deviceState?.wifiConnected ? 'Current station signal strength' : 'Not connected'}
                      />
                      <MetricCard label="Free Heap" value={formatBytes(deviceState?.freeHeap)} hint={`Min ${formatBytes(deviceState?.minFreeHeap)}`} />
                    </section>

                    <section className="grid gap-3 md:grid-cols-2 xl:grid-cols-3">
                      <MetricCard label="Chip" value={deviceState ? `${deviceState.chipModel} rev ${deviceState.chipRevision}` : '—'} hint={`${deviceState?.chipCores || '—'} core(s)`} />
                      <MetricCard label="SDK" value={deviceState?.sdkVersion || '—'} hint={`${deviceState?.cpuFreqMHz || '—'} MHz CPU`} />
                      <MetricCard label="Flash" value={formatBytes(deviceState?.flashChipSize)} hint={`Sketch ${formatBytes(deviceState?.sketchSize)}`} />
                      <MetricCard label="Free Sketch Space" value={formatBytes(deviceState?.freeSketchSpace)} hint="Remaining OTA/app slot space" />
                      <MetricCard label="eFuse MAC" value={deviceState?.efuseMac || '—'} hint={deviceState?.ip || 'No current IP'} />
                      <MetricCard label="BLE Stack" value={deviceState?.bleStack || '—'} hint={deviceState?.bleReady ? 'Bluetooth initialized' : 'Bluetooth unavailable'} />
                    </section>

                    <div className="rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
                      <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Sensor Notes</div>
                      <h2 className="mt-2 text-2xl font-bold text-slate-900">Internal Device Status</h2>
                      <p className="mt-3 text-sm leading-6 text-slate-600">
                        On this firmware build, the main internal sensor exposed directly by the Arduino ESP32 core is the chip
                        temperature sensor. The rest of this page shows device telemetry such as memory, flash usage, radio state,
                        and chip metadata.
                      </p>
                    </div>
                  </section>
                ) : null}
              </div>
            </div>
          </main>
        );
      }

      ReactDOM.createRoot(document.getElementById('root')).render(<App />);
    </script>
  </body>
</html>
)HTML";
