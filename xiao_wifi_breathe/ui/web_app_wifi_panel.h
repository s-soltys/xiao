#pragma once

const char kWebAppWifiPanel[] PROGMEM = R"HTML(
      function WifiAppPanel({
        wifiState,
        wifiBusy,
        wifiScanBusy,
        wifiForm,
        onWifiFormChange,
        onApplyWifiConnect,
        onStartWifiScan,
        onMoveSavedNetwork,
        onForgetWifiNetwork,
      }) {
        const savedNetworks = wifiState?.savedNetworks || [];
        const scanResults = (wifiState?.scanResults || []).slice().sort((left, right) => right.rssi - left.rssi);
        const currentNetworkLabel = wifiState?.connected ? wifiState?.ssid || 'Connected' : wifiState?.targetSsid || 'No active target';

        return (
          <section className="grid gap-6">
            <section className="grid gap-4 lg:grid-cols-[1.08fr_0.92fr] lg:items-start">
              <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-5 text-white">
                <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">Station Networking</div>
                <div className="mt-2 text-2xl font-bold sm:text-3xl">{currentNetworkLabel}</div>
                <p className="mt-3 max-w-2xl text-sm leading-6 text-slate-300">
                  Save Wi-Fi credentials on the device, reorder their connection priority, and move the XIAO to another nearby network without reflashing firmware.
                </p>
                <div className="mt-4 flex flex-wrap gap-2 text-xs font-medium uppercase tracking-[0.2em] text-slate-300">
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{formatWifiStatusLabel(wifiState?.status)}</span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{formatWifiSourceLabel(wifiState?.activeSource)}</span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">
                    {wifiState?.connected ? wifiState?.ip || 'No IP yet' : wifiState?.targetSsid ? `Target ${wifiState.targetSsid}` : 'Waiting for target'}
                  </span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{savedNetworks.length} saved network(s)</span>
                </div>
              </div>

              <form className="grid gap-4 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm" onSubmit={onApplyWifiConnect}>
                <div>
                  <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Connect To Wi-Fi</div>
                  <h2 className="mt-2 text-2xl font-bold text-slate-900">Pick a scanned SSID or enter one manually</h2>
                  <p className="mt-2 text-sm text-slate-600">
                    Tapping a nearby network fills the SSID here. Submitting saves that network at the top of the priority list and starts a connection attempt immediately.
                  </p>
                </div>

                <div className="grid gap-2">
                  <label className="text-[11px] font-medium uppercase tracking-[0.24em] text-slate-500">SSID</label>
                  <input
                    type="text"
                    value={wifiForm.ssid}
                    onChange={(event) => onWifiFormChange('ssid', event.target.value)}
                    placeholder="My Hidden Network"
                    disabled={wifiBusy}
                    className="w-full rounded-2xl border border-slate-300 bg-white px-4 py-3 text-base text-slate-900 outline-none transition placeholder:text-slate-400 focus:border-teal-500 focus:ring-2 focus:ring-teal-200 disabled:opacity-60"
                  />
                </div>

                <div className="grid gap-2">
                  <label className="text-[11px] font-medium uppercase tracking-[0.24em] text-slate-500">Password</label>
                  <input
                    type="password"
                    value={wifiForm.password}
                    onChange={(event) => onWifiFormChange('password', event.target.value)}
                    placeholder="Leave blank for open networks"
                    disabled={wifiBusy}
                    className="w-full rounded-2xl border border-slate-300 bg-white px-4 py-3 text-base text-slate-900 outline-none transition placeholder:text-slate-400 focus:border-teal-500 focus:ring-2 focus:ring-teal-200 disabled:opacity-60"
                  />
                </div>

                <div className="grid gap-3 sm:grid-cols-2">
                  <button
                    type="submit"
                    disabled={wifiBusy || !wifiForm.ssid.trim()}
                    className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                  >
                    {wifiBusy ? 'Saving And Connecting...' : 'Save And Connect'}
                  </button>

                  <button
                    type="button"
                    onClick={onStartWifiScan}
                    disabled={wifiBusy || wifiScanBusy || wifiState?.scanning}
                    className="inline-flex min-h-[52px] items-center justify-center rounded-full border border-slate-300 bg-white px-6 py-3 text-sm font-semibold text-slate-800 transition hover:border-slate-900 hover:text-slate-900 disabled:cursor-not-allowed disabled:opacity-60"
                  >
                    {wifiState?.scanning ? 'Scanning...' : wifiScanBusy ? 'Starting Scan...' : 'Scan Nearby'}
                  </button>
                </div>

                <div className="rounded-3xl border border-slate-200 bg-slate-50 px-4 py-4 text-sm text-slate-600">
                  Compile-time fallback:
                  <div className="mt-2 text-slate-900">
                    {wifiState?.fallbackConfigured
                      ? 'If no saved network connects, firmware falls back to wifi_config.h.'
                      : 'No wifi_config.h fallback is configured right now.'}
                  </div>
                </div>
              </form>
            </section>

            <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-5">
              <StatusCard
                label="Status"
                value={formatWifiStatusLabel(wifiState?.status)}
                tone={wifiState?.connected ? 'online' : wifiState?.lastError ? 'offline' : 'neutral'}
              />
              <StatusCard label="Current Network" value={currentNetworkLabel} tone={wifiState?.connected ? 'online' : 'neutral'} />
              <StatusCard label="IP Address" value={wifiState?.connected ? wifiState?.ip || '-' : 'Not connected'} tone="neutral" />
              <StatusCard label="Source" value={formatWifiSourceLabel(wifiState?.activeSource)} tone="neutral" />
              <StatusCard label="Saved Priority" value={`${savedNetworks.length} stored`} tone="neutral" />
            </section>

            {wifiState?.lastError ? (
              <div className="rounded-2xl border border-amber-200 bg-amber-50 px-4 py-3 text-sm font-medium text-amber-800">{wifiState.lastError}</div>
            ) : null}

            <section className="grid gap-6 xl:grid-cols-[0.95fr_1.05fr]">
              <div className="grid gap-3">
                <div className="rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
                  <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Saved Networks</div>
                  <h2 className="mt-2 text-2xl font-bold text-slate-900">Priority order</h2>
                  <p className="mt-2 text-sm text-slate-600">
                    The device retries these saved networks from top to bottom before it falls back to `wifi_config.h`.
                  </p>
                </div>

                {savedNetworks.length === 0 ? (
                  <div className="rounded-[1.75rem] border border-dashed border-slate-300 bg-white/70 px-5 py-10 text-center text-sm text-slate-500">
                    No saved Wi-Fi networks yet. Use the form above or tap a scanned SSID to store one.
                  </div>
                ) : (
                  savedNetworks.map((network, index) => (
                    <div key={network.ssid} className="grid gap-4 rounded-[1.5rem] border border-white/70 bg-white/85 px-5 py-4 shadow-sm">
                      <div className="flex flex-wrap items-start justify-between gap-3">
                        <div>
                          <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-700">Priority {network.priority}</div>
                          <div className="mt-2 text-xl font-bold text-slate-900">{network.ssid}</div>
                          <div className="mt-2 text-sm text-slate-500">
                            {network.connected ? 'Connected now' : index === 0 ? 'First retry target' : 'Saved retry target'}
                          </div>
                        </div>
                        <div className="rounded-full bg-slate-100 px-3 py-1 text-xs font-semibold uppercase tracking-[0.2em] text-slate-700">
                          {network.connected ? 'Live' : 'Saved'}
                        </div>
                      </div>

                      <div className="grid gap-2 sm:grid-cols-3">
                        <button
                          type="button"
                          onClick={() => onMoveSavedNetwork(network.ssid, 'up')}
                          disabled={wifiBusy || index === 0}
                          className="inline-flex min-h-[44px] items-center justify-center rounded-full border border-slate-300 bg-white px-4 py-2 text-sm font-semibold text-slate-800 transition hover:border-slate-900 hover:text-slate-900 disabled:cursor-not-allowed disabled:opacity-60"
                        >
                          Move Up
                        </button>
                        <button
                          type="button"
                          onClick={() => onMoveSavedNetwork(network.ssid, 'down')}
                          disabled={wifiBusy || index === savedNetworks.length - 1}
                          className="inline-flex min-h-[44px] items-center justify-center rounded-full border border-slate-300 bg-white px-4 py-2 text-sm font-semibold text-slate-800 transition hover:border-slate-900 hover:text-slate-900 disabled:cursor-not-allowed disabled:opacity-60"
                        >
                          Move Down
                        </button>
                        <button
                          type="button"
                          onClick={() => onForgetWifiNetwork(network.ssid)}
                          disabled={wifiBusy}
                          className="inline-flex min-h-[44px] items-center justify-center rounded-full border border-rose-200 bg-rose-50 px-4 py-2 text-sm font-semibold text-rose-700 transition hover:border-rose-400 hover:text-rose-800 disabled:cursor-not-allowed disabled:opacity-60"
                        >
                          Forget
                        </button>
                      </div>
                    </div>
                  ))
                )}
              </div>

              <div className="grid gap-3">
                <div className="rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
                  <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Nearby Networks</div>
                  <h2 className="mt-2 text-2xl font-bold text-slate-900">Scan results</h2>
                  <p className="mt-2 text-sm text-slate-600">
                    Click a network to load its SSID into the form above, then enter the password and connect.
                  </p>
                </div>

                {scanResults.length === 0 ? (
                  <div className="rounded-[1.75rem] border border-dashed border-slate-300 bg-white/70 px-5 py-10 text-center text-sm text-slate-500">
                    {wifiState?.scanning ? 'Scanning nearby networks...' : 'No Wi-Fi scan results yet. Start a scan to list nearby SSIDs.'}
                  </div>
                ) : (
                  scanResults.map((network) => (
                    <button
                      key={network.ssid}
                      type="button"
                      onClick={() => {
                        onWifiFormChange('ssid', network.ssid);
                        onWifiFormChange('password', '');
                      }}
                      disabled={wifiBusy}
                      className="grid gap-3 rounded-[1.5rem] border border-white/70 bg-white/85 px-5 py-4 text-left shadow-sm transition hover:-translate-y-0.5 hover:border-slate-300 hover:bg-white disabled:cursor-not-allowed disabled:opacity-60 md:grid-cols-[1.2fr_repeat(3,minmax(0,1fr))]"
                    >
                      <div>
                        <div className="text-lg font-bold text-slate-900">{network.ssid}</div>
                        <div className="mt-2 flex flex-wrap gap-2 text-xs font-medium uppercase tracking-[0.18em] text-slate-500">
                          <span className="rounded-full bg-slate-100 px-3 py-1">{network.encryptionType}</span>
                          <span className={`rounded-full px-3 py-1 ${network.known ? 'bg-teal-50 text-teal-700' : 'bg-slate-100 text-slate-700'}`}>
                            {network.known ? 'Saved' : 'New'}
                          </span>
                        </div>
                      </div>
                      <MetricCard label="Signal" value={`${network.rssi} dBm`} hint={network.rssi >= -60 ? 'Strong' : network.rssi >= -75 ? 'Moderate' : 'Weak'} />
                      <MetricCard label="Security" value={network.encryptionType} hint={network.known ? 'Already stored' : 'Tap to connect'} />
                      <MetricCard label="Action" value="Connect" hint="Loads the SSID into the form" />
                    </button>
                  ))
                )}
              </div>
            </section>
          </section>
        );
      }
)HTML";
