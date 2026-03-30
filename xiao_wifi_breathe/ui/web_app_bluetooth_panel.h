#pragma once

const char kWebAppBluetoothPanel[] PROGMEM = R"HTML(
      function BluetoothAppPanel({ bluetoothState, scanBusy, onStartScan }) {
        const sortedBleResults = (bluetoothState?.results || []).slice().sort((left, right) => right.rssi - left.rssi);

        return (
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
                onClick={onStartScan}
                disabled={scanBusy || bluetoothState?.scanning || !bluetoothState?.available}
                className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
              >
                {bluetoothState?.scanning ? 'Scanning...' : scanBusy ? 'Starting Scan...' : 'Start BLE Scan'}
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
              <StatusCard label="Results" value={`${bluetoothState?.results?.length || 0} devices`} tone="neutral" />
              <StatusCard label="Last Complete" value={formatRelativeMs(bluetoothState?.lastCompletedAtMs)} tone="neutral" />
            </section>

            {bluetoothState?.error ? (
              <div className="rounded-2xl border border-amber-200 bg-amber-50 px-4 py-3 text-sm font-medium text-amber-800">{bluetoothState.error}</div>
            ) : null}

            <section className="grid gap-3">
              {sortedBleResults.length === 0 ? (
                <div className="rounded-[1.75rem] border border-dashed border-slate-300 bg-white/70 px-5 py-10 text-center text-sm text-slate-500">
                  {bluetoothState?.scanning
                    ? 'Scanning for nearby BLE devices...'
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
                      value={device.hasTxPower ? `${device.txPower} dBm` : '-'}
                      hint={device.scannable ? 'Scannable' : 'Not scannable'}
                    />
                    <MetricCard label="Address Type" value={device.addressType} hint={device.legacy ? 'Legacy advertisement' : 'Extended capable'} />
                    <MetricCard label="Mode" value={device.connectable ? 'Connectable' : 'Observer'} hint={device.name ? 'Name broadcast present' : 'Name missing'} />
                  </div>
                ))
              )}
            </section>
          </section>
        );
      }
)HTML";
