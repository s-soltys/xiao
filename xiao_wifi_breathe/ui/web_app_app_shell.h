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
        const [bluetoothState, setBluetoothState] = useState(null);
        const [deviceState, setDeviceState] = useState(null);
        const [loading, setLoading] = useState(true);
        const [refreshing, setRefreshing] = useState(false);
        const [moodBusyId, setMoodBusyId] = useState('');
        const [messageBusy, setMessageBusy] = useState(false);
        const [matrixBusy, setMatrixBusy] = useState(false);
        const [matrixBrightnessSyncing, setMatrixBrightnessSyncing] = useState(false);
        const [scanBusy, setScanBusy] = useState(false);
        const [error, setError] = useState('');
        const [messageInput, setMessageInput] = useState('');
        const [matrixColor, setMatrixColor] = useState('#22c55e');
        const [matrixBrightnessSlider, setMatrixBrightnessSlider] = useState(() => sliderValueFromMatrixBrightness(48));
        const [matrixMapping, setMatrixMapping] = useState('cols-bl');
        const [matrixInputsReady, setMatrixInputsReady] = useState(false);
        const matrixRequestSeqRef = useRef(0);

        const visibleApps = appRegistry.length ? appRegistry : fallbackAppRegistry;

        function syncMatrixInputsFromPayload(payload, options = {}) {
          if (options.includeColor !== false) {
            setMatrixColor(payload.color || '#22c55e');
          }
          setMatrixBrightnessSlider(sliderValueFromMatrixBrightness(payload.brightness ?? 48));
          if (options.includeMapping !== false) {
            setMatrixMapping(payload.mappingId || 'cols-bl');
          }
          setMatrixInputsReady(true);
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
          if (syncInputs || !matrixInputsReady) {
            syncMatrixInputsFromPayload(payload);
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

          const results = await Promise.allSettled([
            refreshAppRegistry(),
            fetchSystemState(),
            fetchMoodState(),
            fetchMessageState(),
            fetchMatrixState(true),
            fetchBluetoothState(),
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

          syncMatrixInputsFromPayload(matrixState);
        }, [activeApp]);

        async function updateMatrixSettings(changes, options = {}) {
          const requestSeq = matrixRequestSeqRef.current + 1;
          matrixRequestSeqRef.current = requestSeq;

          if (options.setBusy !== false) {
            setMatrixBusy(true);
            setMatrixBrightnessSyncing(false);
          }
          if (options.trackBrightnessSync) {
            setMatrixBrightnessSyncing(true);
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
            });
            if (options.refreshDependentApps !== false) {
              fetchMoodState().catch(() => {});
              fetchMessageState().catch(() => {});
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
              if (options.trackBrightnessSync) {
                setMatrixBrightnessSyncing(false);
              }
            }
          }
        }

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
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the scrolling message.');
          } finally {
            setMessageBusy(false);
          }
        }

        async function applyMatrixControls(event) {
          event.preventDefault();
          await updateMatrixSettings({
            color: matrixColor,
            brightness: matrixBrightnessFromSliderValue(matrixBrightnessSlider),
            mappingId: matrixMapping,
          });
        }

        useEffect(() => {
          if (!matrixInputsReady || !matrixState?.available) {
            return undefined;
          }

          const nextBrightness = matrixBrightnessFromSliderValue(matrixBrightnessSlider);
          if (nextBrightness === (matrixState?.brightness ?? nextBrightness)) {
            return undefined;
          }

          const timeoutId = window.setTimeout(() => {
            updateMatrixSettings(
              { brightness: nextBrightness },
              {
                setBusy: false,
                trackBrightnessSync: true,
                includeColor: false,
                includeMapping: false,
                refreshDependentApps: false,
                errorMessage: 'Unable to update the matrix brightness.',
              }
            ).catch(() => {});
          }, 120);

          return () => window.clearTimeout(timeoutId);
        }, [matrixBrightnessSlider, matrixInputsReady, matrixState?.available, matrixState?.brightness]);

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
                matrixBrightnessSyncing={matrixBrightnessSyncing}
                matrixColor={matrixColor}
                onMatrixColorChange={setMatrixColor}
                matrixBrightnessSlider={matrixBrightnessSlider}
                onBrightnessSliderChange={setMatrixBrightnessSlider}
                matrixBrightnessValue={matrixBrightnessFromSliderValue(matrixBrightnessSlider)}
                matrixMapping={matrixMapping}
                onMatrixMappingChange={setMatrixMapping}
                onApplyMatrixControls={applyMatrixControls}
                onSelectPattern={(patternId) => updateMatrixSettings({ patternId })}
              />
            );
          }

          if (activeApp === 'bluetooth') {
            return <BluetoothAppPanel bluetoothState={bluetoothState} scanBusy={scanBusy} onStartScan={startBluetoothScan} />;
          }

          return <DeviceAppPanel deviceState={deviceState} />;
        }

        const activeAppConfig = visibleApps.find((app) => app.id === activeApp) || visibleApps[0] || null;
        const activeAppLabel = activeAppConfig?.label || 'Device Console';

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
                <section className="grid gap-6 lg:grid-cols-[1.4fr_1fr]">
                  <div>
                    <p className="text-xs font-medium uppercase tracking-[0.35em] text-teal-700">App Registry Device Console</p>
                    <h1 className="mt-3 text-4xl font-bold leading-tight text-slate-900 sm:text-5xl">XIAO Device Console</h1>
                    <p className="mt-4 max-w-3xl text-base text-slate-600 sm:text-lg">
                      Switch between device apps exposed by the firmware registry. Each panel still talks to its dedicated API routes, but the shell no longer hardcodes the app list.
                    </p>
                  </div>

                  <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-1">
                    <StatusCard
                      label="Reachability"
                      value={systemState?.connected ? systemState.hostname : 'Waiting for Wi-Fi'}
                      tone={systemState?.connected ? 'online' : 'offline'}
                    />
                    <StatusCard
                      label="Network"
                      value={systemState?.connected ? `${systemState.ssid} / ${systemState.ip}` : 'STA mode retrying'}
                      tone={systemState?.connected ? 'neutral' : 'offline'}
                    />
                    <StatusCard
                      label="Hardware"
                      value={deviceState ? `${deviceState.chipModel} rev ${deviceState.chipRevision}` : 'Loading chip info'}
                      tone="neutral"
                    />
                  </div>
                </section>

                <section className="grid gap-3 lg:grid-cols-3 2xl:grid-cols-5">
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
