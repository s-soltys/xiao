#pragma once

const char kWebAppAppShell[] PROGMEM = R"HTML(
      function AppShell() {
        const { fetchJson } = useDeviceApi();
        const { appRegistry, refreshAppRegistry } = useAppRegistry(fetchJson);
        const [activeApp, setActiveApp] = useState('matrix');
        const [systemState, setSystemState] = useState(null);
        const [moodState, setMoodState] = useState(null);
        const [messageState, setMessageState] = useState(null);
        const [matrixState, setMatrixState] = useState(null);
        const [glowState, setGlowState] = useState(null);
        const [bluetoothState, setBluetoothState] = useState(null);
        const [wifiState, setWifiState] = useState(null);
        const [deviceState, setDeviceState] = useState(null);
        const [loading, setLoading] = useState(true);
        const [refreshing, setRefreshing] = useState(false);
        const [moodBusyId, setMoodBusyId] = useState('');
        const [messageBusy, setMessageBusy] = useState(false);
        const [matrixBusy, setMatrixBusy] = useState(false);
        const [matrixOutputSyncing, setMatrixOutputSyncing] = useState(false);
        const [glowBusy, setGlowBusy] = useState(false);
        const [scanBusy, setScanBusy] = useState(false);
        const [wifiBusy, setWifiBusy] = useState(false);
        const [wifiScanBusy, setWifiScanBusy] = useState(false);
        const [error, setError] = useState('');
        const [messageInput, setMessageInput] = useState('');
        const [wifiForm, setWifiForm] = useState({ ssid: '', password: '' });
        const [glowColor, setGlowColor] = useState('#22c55e');
        const [matrixBrightnessSlider, setMatrixBrightnessSlider] = useState(() => sliderValueFromMatrixBrightness(48));
        const [matrixAnimationSpeedSlider, setMatrixAnimationSpeedSlider] = useState('100');
        const [matrixMapping, setMatrixMapping] = useState('cols-bl');
        const [matrixMappingReady, setMatrixMappingReady] = useState(false);
        const [matrixOutputInputsReady, setMatrixOutputInputsReady] = useState(false);
        const matrixRequestSeqRef = useRef(0);

        const visibleApps = appRegistry.length ? appRegistry : fallbackAppRegistry;
        const matrixOutputBusy = matrixBusy || glowBusy || matrixOutputSyncing;
        const matrixOutputAvailable = Boolean(matrixState?.available);

        function syncMatrixInputsFromPayload(payload, options = {}) {
          if (options.includeColor !== false) {
            setGlowColor(payload.color || '#22c55e');
          }
          if (options.includeMapping !== false) {
            setMatrixMapping(payload.mappingId || 'cols-bl');
            setMatrixMappingReady(true);
          }
          if (options.includeOutput !== false) {
            setMatrixBrightnessSlider(sliderValueFromMatrixBrightness(payload.brightness ?? 48));
            setMatrixAnimationSpeedSlider(String(payload.animationSpeed ?? 100));
            setMatrixOutputInputsReady(true);
          }
        }

        async function fetchSystemState() {
          const payload = await fetchJson('/api/state', { cache: 'no-store' });
          setSystemState(payload);
          return payload;
        }

        async function fetchMoodState() {
          const payload = await fetchJson('/api/mood', { cache: 'no-store' });
          setMoodState(payload);
          return payload;
        }

        async function fetchMessageState() {
          const payload = await fetchJson('/api/message', { cache: 'no-store' });
          setMessageState(payload);
          setMessageInput(payload.text || '');
          return payload;
        }

        async function fetchMatrixState(syncInputs = false) {
          const payload = await fetchJson('/api/matrix', { cache: 'no-store' });
          setMatrixState(payload);
          if (syncInputs || !matrixMappingReady || !matrixOutputInputsReady) {
            syncMatrixInputsFromPayload(payload);
          }
          return payload;
        }

        async function fetchGlowState() {
          const payload = await fetchJson('/api/glow', { cache: 'no-store' });
          setGlowState(payload);
          setGlowColor(payload.color || '#22c55e');
          return payload;
        }

        async function fetchBluetoothState() {
          const payload = await fetchJson('/api/bluetooth', { cache: 'no-store' });
          setBluetoothState(payload);
          return payload;
        }

        async function fetchWifiState() {
          const payload = await fetchJson('/api/wifi', { cache: 'no-store' });
          setWifiState(payload);
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

          const results = await Promise.allSettled([
            refreshAppRegistry(),
            fetchSystemState(),
            fetchMoodState(),
            fetchMessageState(),
            fetchMatrixState(true),
            fetchGlowState(),
            fetchBluetoothState(),
            fetchWifiState(),
            fetchDeviceState(),
          ]);

          const firstError = results.find((result) => result.status === 'rejected');
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
          if (visibleApps.length > 0 && !visibleApps.some((app) => app.id === activeApp)) {
            setActiveApp(visibleApps[0].id);
          }
        }, [activeApp, visibleApps]);

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
          if (activeApp !== 'wifi' && !wifiState?.scanning && !wifiState?.connecting) {
            return undefined;
          }

          const intervalId = window.setInterval(() => {
            fetchWifiState()
              .then(() => fetchSystemState())
              .catch((requestError) => {
                setError(requestError.message || 'Unable to refresh Wi-Fi state.');
              });
          }, 1500);

          return () => window.clearInterval(intervalId);
        }, [activeApp, wifiState?.connecting, wifiState?.scanning]);

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
          if (activeApp !== 'mood' && activeApp !== 'message') {
            return undefined;
          }

          const intervalId = window.setInterval(() => {
            fetchMatrixState(false).catch(() => {});
          }, 2500);

          return () => window.clearInterval(intervalId);
        }, [activeApp]);

        useEffect(() => {
          if (activeApp !== 'matrix' || !matrixState) {
            return;
          }

          syncMatrixInputsFromPayload(matrixState, {
            includeColor: false,
            includeOutput: false,
          });
        }, [activeApp, matrixState]);

        async function updateMatrixSettings(changes, options = {}) {
          const requestSeq = matrixRequestSeqRef.current + 1;
          matrixRequestSeqRef.current = requestSeq;

          if (options.setBusy !== false) {
            setMatrixBusy(true);
          }
          if (options.trackOutputSync) {
            setMatrixOutputSyncing(true);
          }
          setError('');

          try {
            const payload = await fetchJson('/api/matrix', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify(changes),
            });

            if (requestSeq !== matrixRequestSeqRef.current) {
              return payload;
            }

            setMatrixState(payload);
            syncMatrixInputsFromPayload(payload, {
              includeColor: options.includeColor,
              includeMapping: options.includeMapping,
              includeOutput: options.includeOutput,
            });

            if (options.refreshDependentApps !== false) {
              fetchMoodState().catch(() => {});
              fetchMessageState().catch(() => {});
              fetchGlowState().catch(() => {});
            }
            return payload;
          } catch (requestError) {
            if (requestSeq === matrixRequestSeqRef.current) {
              setError(requestError.message || options.errorMessage || 'Unable to update the RGB matrix.');
            }
            return null;
          } finally {
            if (requestSeq === matrixRequestSeqRef.current) {
              if (options.setBusy !== false) {
                setMatrixBusy(false);
              }
              if (options.trackOutputSync) {
                setMatrixOutputSyncing(false);
              }
            }
          }
        }

        useEffect(() => {
          if (!matrixOutputInputsReady || !matrixState?.available) {
            return undefined;
          }

          const nextBrightness = matrixBrightnessFromSliderValue(matrixBrightnessSlider);
          const nextAnimationSpeed = matrixAnimationSpeedFromSliderValue(matrixAnimationSpeedSlider);
          const changes = {};

          if (nextBrightness !== (matrixState?.brightness ?? nextBrightness)) {
            changes.brightness = nextBrightness;
          }

          if (nextAnimationSpeed !== (matrixState?.animationSpeed ?? nextAnimationSpeed)) {
            changes.animationSpeed = nextAnimationSpeed;
          }

          if (Object.keys(changes).length === 0) {
            return undefined;
          }

          const timeoutId = window.setTimeout(() => {
            updateMatrixSettings(changes, {
              setBusy: false,
              trackOutputSync: true,
              includeColor: false,
              includeMapping: false,
              refreshDependentApps: false,
              errorMessage: 'Unable to update the shared matrix controls.',
            }).catch(() => {});
          }, 120);

          return () => window.clearTimeout(timeoutId);
        }, [
          matrixBrightnessSlider,
          matrixAnimationSpeedSlider,
          matrixOutputInputsReady,
          matrixState?.available,
          matrixState?.brightness,
          matrixState?.animationSpeed,
        ]);

        async function activateMood(moodId) {
          setMoodBusyId(moodId);
          setError('');

          try {
            const payload = await fetchJson('/api/mood', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ id: moodId }),
            });
            setMoodState(payload);
            await fetchMatrixState(false);
            fetchGlowState().catch(() => {});
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the mood icon.');
          } finally {
            setMoodBusyId('');
          }
        }

        async function applyMessage(event) {
          event.preventDefault();
          setMessageBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/message', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ text: messageInput }),
            });
            setMessageState(payload);
            setMessageInput(payload.text || '');
            await fetchMatrixState(false);
            fetchGlowState().catch(() => {});
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the scrolling message.');
          } finally {
            setMessageBusy(false);
          }
        }

        async function applyMatrixControls(event) {
          event.preventDefault();
          await updateMatrixSettings(
            {
              mappingId: matrixMapping,
            },
            {
              includeColor: false,
              includeOutput: false,
            }
          );
        }

        async function applyGlow(event) {
          event.preventDefault();
          setGlowBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/glow', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ color: glowColor }),
            });
            setGlowState(payload);
            setGlowColor(payload.color || '#22c55e');
            await fetchMatrixState(false);
            fetchMoodState().catch(() => {});
            fetchMessageState().catch(() => {});
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the solid glow color.');
          } finally {
            setGlowBusy(false);
          }
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

        function updateWifiForm(field, value) {
          setWifiForm((current) => ({
            ...current,
            [field]: value,
          }));
        }

        async function applyWifiConnect(event) {
          event.preventDefault();
          setWifiBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/wifi/connect', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({
                ssid: wifiForm.ssid,
                password: wifiForm.password,
              }),
            });
            setWifiState(payload);
            setWifiForm((current) => ({
              ...current,
              password: '',
            }));
            await fetchSystemState();
          } catch (requestError) {
            setError(requestError.message || 'Unable to save and connect to that Wi-Fi network.');
          } finally {
            setWifiBusy(false);
          }
        }

        async function startWifiScan() {
          setWifiScanBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/wifi/scan', {
              method: 'POST',
            });
            setWifiState(payload);
          } catch (requestError) {
            setError(requestError.message || 'Unable to start the Wi-Fi scan.');
          } finally {
            setWifiScanBusy(false);
          }
        }

        async function moveSavedWifiNetwork(ssid, direction) {
          const savedNetworks = (wifiState?.savedNetworks || []).slice();
          const currentIndex = savedNetworks.findIndex((network) => network.ssid === ssid);
          if (currentIndex < 0) {
            return;
          }

          const targetIndex = direction === 'up' ? currentIndex - 1 : currentIndex + 1;
          if (targetIndex < 0 || targetIndex >= savedNetworks.length) {
            return;
          }

          const reordered = savedNetworks.map((network) => network.ssid);
          const currentSsid = reordered[currentIndex];
          reordered[currentIndex] = reordered[targetIndex];
          reordered[targetIndex] = currentSsid;

          setWifiBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/wifi/reorder', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ ssids: reordered }),
            });
            setWifiState(payload);
          } catch (requestError) {
            setError(requestError.message || 'Unable to reorder the saved Wi-Fi networks.');
          } finally {
            setWifiBusy(false);
          }
        }

        async function forgetWifiNetwork(ssid) {
          setWifiBusy(true);
          setError('');

          try {
            const payload = await fetchJson('/api/wifi/forget', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ ssid }),
            });
            setWifiState(payload);
            setWifiForm((current) =>
              current.ssid === ssid
                ? {
                    ssid: '',
                    password: '',
                  }
                : current
            );
            await fetchSystemState();
          } catch (requestError) {
            setError(requestError.message || 'Unable to forget the saved Wi-Fi network.');
          } finally {
            setWifiBusy(false);
          }
        }

        function renderActivePanel() {
          if (activeApp === 'mood') {
            return <MoodAppPanel moodState={moodState} matrixState={matrixState} moodBusyId={moodBusyId} onSelectMood={activateMood} />;
          }

          if (activeApp === 'message') {
            return (
              <MessageAppPanel
                messageState={messageState}
                matrixState={matrixState}
                messageInput={messageInput}
                onMessageInputChange={setMessageInput}
                onApplyMessage={applyMessage}
                messageBusy={messageBusy}
              />
            );
          }

          if (activeApp === 'matrix') {
            return (
              <MatrixAppPanel
                matrixState={matrixState}
                matrixBusy={matrixBusy}
                matrixMapping={matrixMapping}
                onMatrixMappingChange={setMatrixMapping}
                onApplyMatrixControls={applyMatrixControls}
                onSelectPattern={(patternId) =>
                  updateMatrixSettings(
                    { patternId },
                    {
                      includeColor: false,
                      includeOutput: false,
                      errorMessage: 'Unable to switch the RGB matrix effect.',
                    }
                  )
                }
              />
            );
          }

          if (activeApp === 'glow') {
            return (
              <GlowAppPanel
                glowState={glowState}
                matrixState={matrixState}
                glowColor={glowColor}
                onGlowColorChange={setGlowColor}
                onApplyGlow={applyGlow}
                glowBusy={glowBusy}
              />
            );
          }

          if (activeApp === 'bluetooth') {
            return <BluetoothAppPanel bluetoothState={bluetoothState} scanBusy={scanBusy} onStartScan={startBluetoothScan} />;
          }

          if (activeApp === 'wifi') {
            return (
              <WifiAppPanel
                wifiState={wifiState}
                wifiBusy={wifiBusy}
                wifiScanBusy={wifiScanBusy}
                wifiForm={wifiForm}
                onWifiFormChange={updateWifiForm}
                onApplyWifiConnect={applyWifiConnect}
                onStartWifiScan={startWifiScan}
                onMoveSavedNetwork={moveSavedWifiNetwork}
                onForgetWifiNetwork={forgetWifiNetwork}
              />
            );
          }

          return <DeviceAppPanel deviceState={deviceState} />;
        }

        const activeAppConfig = visibleApps.find((app) => app.id === activeApp) || visibleApps[0] || null;
        const activeAppLabel = activeAppConfig?.label || 'Device Console';
        const matrixOutputEnabled = Boolean(matrixState?.enabled);
        const matrixBrightnessValue = matrixBrightnessFromSliderValue(matrixBrightnessSlider);
        const matrixAnimationSpeedValue = matrixAnimationSpeedFromSliderValue(matrixAnimationSpeedSlider);

        if (loading && !systemState) {
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

        return (
          <main className="mx-auto flex min-h-screen max-w-7xl px-5 py-8 sm:py-10">
            <div className="w-full rounded-[2rem] border border-white/80 bg-white/75 p-6 shadow-panel backdrop-blur sm:p-8">
              <div className="flex flex-col gap-8">
                <section className="grid gap-6 lg:grid-cols-[1.2fr_1fr]">
                  <div>
                    <p className="text-xs font-medium uppercase tracking-[0.35em] text-teal-700">App Registry Device Console</p>
                    <h1 className="mt-3 text-4xl font-bold leading-tight text-slate-900 sm:text-5xl">XIAO Device Console</h1>
                    <p className="mt-4 max-w-3xl text-base text-slate-600 sm:text-lg">
                      Switch between device apps exposed by the firmware registry. Global matrix power, brightness, and animation speed stay in the header so they are always within reach.
                    </p>
                  </div>

                  <div className="grid gap-3 sm:grid-cols-2">
                    <StatusCard
                      label="Reachability"
                      value={systemState?.connected ? systemState.hostname : 'Waiting for Wi-Fi'}
                      tone={systemState?.connected ? 'online' : 'offline'}
                    />
                    <StatusCard
                      label="Network"
                      value={
                        systemState?.connected
                          ? `${systemState.ssid} / ${systemState.ip}`
                          : wifiState?.targetSsid
                            ? `Connecting to ${wifiState.targetSsid}`
                            : 'STA mode retrying'
                      }
                      tone={systemState?.connected ? 'neutral' : wifiState?.lastError ? 'offline' : 'neutral'}
                      hint="Open Wi-Fi Config"
                      onClick={() => setActiveApp('wifi')}
                    />
                    <StatusCard
                      label="Hardware"
                      value={deviceState ? `${deviceState.chipModel} rev ${deviceState.chipRevision}` : 'Loading chip info'}
                      tone="neutral"
                    />

                    <div className="sm:col-span-2 rounded-[1.6rem] border border-slate-200 bg-white/85 p-4 shadow-sm">
                      <div className="flex items-center justify-between gap-4">
                        <div>
                          <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Global Matrix Control</div>
                          <div className="mt-1 text-xl font-bold text-slate-900">{matrixOutputEnabled ? 'Output enabled' : 'Output disabled'}</div>
                        </div>
                        <button
                          type="button"
                          role="switch"
                          aria-checked={matrixOutputEnabled}
                          onClick={() =>
                            updateMatrixSettings(
                              { enabled: !matrixOutputEnabled },
                              {
                                includeColor: false,
                                includeMapping: false,
                                errorMessage: 'Unable to toggle the matrix output.',
                              }
                            )
                          }
                          disabled={matrixOutputBusy || !matrixOutputAvailable}
                          className={[
                            'relative inline-flex h-11 w-20 items-center rounded-full border transition',
                            matrixOutputEnabled ? 'border-slate-950 bg-slate-950' : 'border-slate-300 bg-slate-200',
                            matrixOutputBusy ? 'opacity-60' : '',
                          ].join(' ')}
                        >
                          <span
                            className={[
                              'inline-block h-8 w-8 rounded-full bg-white shadow transition',
                              matrixOutputEnabled ? 'translate-x-10' : 'translate-x-1',
                            ].join(' ')}
                          />
                        </button>
                      </div>

                      <div className="mt-4 grid gap-4">
                        <div className="grid gap-2">
                          <div className="flex items-center justify-between gap-3 text-sm text-slate-600">
                            <span className="font-medium text-slate-900">Brightness</span>
                            <span>{matrixBrightnessValue}/255</span>
                          </div>
                          <input
                            type="range"
                            min="0"
                            max={kMatrixBrightnessSliderMax}
                            step="1"
                            value={matrixBrightnessSlider}
                            onChange={(event) => setMatrixBrightnessSlider(event.target.value)}
                            disabled={matrixOutputBusy || !matrixOutputAvailable}
                            className="w-full accent-teal-600 disabled:opacity-60"
                          />
                          <div className="flex items-center justify-between text-xs text-slate-500">
                            <span>Low-end tuned response curve</span>
                            <span>{matrixOutputSyncing ? 'Updating...' : 'Live output'}</span>
                          </div>
                        </div>

                        <div className="grid gap-2">
                          <div className="flex items-center justify-between gap-3 text-sm text-slate-600">
                            <span className="font-medium text-slate-900">Animation Speed</span>
                            <span>{matrixAnimationSpeedValue}%</span>
                          </div>
                          <input
                            type="range"
                            min={kMatrixAnimationSpeedMin}
                            max={kMatrixAnimationSpeedMax}
                            step="1"
                            value={matrixAnimationSpeedSlider}
                            onChange={(event) => setMatrixAnimationSpeedSlider(event.target.value)}
                            disabled={matrixOutputBusy || !matrixOutputAvailable}
                            className="w-full accent-teal-600 disabled:opacity-60"
                          />
                          <div className="flex items-center justify-between text-xs text-slate-500">
                            <span>Affects all live matrix animations</span>
                            <span>{matrixState?.selectedPatternLabel || 'RGB Matrix'}</span>
                          </div>
                        </div>
                      </div>
                    </div>
                  </div>
                </section>

                <section className="grid gap-3 lg:grid-cols-3 2xl:grid-cols-6">
                  {visibleApps.map((app) => (
                    <AppTab key={app.id} app={app} activeApp={activeApp} onSelect={setActiveApp} />
                  ))}
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
                    {refreshing ? 'Refreshing...' : 'Refresh Apps'}
                  </button>
                </section>

                {error ? (
                  <div className="rounded-2xl border border-rose-200 bg-rose-50 px-4 py-3 text-sm font-medium text-rose-800">{error}</div>
                ) : null}

                {renderActivePanel()}
              </div>
            </div>
          </main>
        );
      }

      ReactDOM.createRoot(document.getElementById('root')).render(<AppShell />);
)HTML";
