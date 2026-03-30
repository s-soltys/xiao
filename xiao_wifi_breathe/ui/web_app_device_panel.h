#pragma once

const char kWebAppDevicePanel[] PROGMEM = R"HTML(
      function DeviceAppPanel({ deviceState }) {
        return (
          <section className="grid gap-6">
            <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
              <MetricCard
                label="Chip Temperature"
                value={deviceState?.temperatureC === null || deviceState?.temperatureC === undefined ? '-' : `${deviceState.temperatureC.toFixed(1)} °C`}
                hint="Internal sensor exposed by the ESP32 Arduino core"
              />
              <MetricCard label="Uptime" value={formatUptime(deviceState?.uptimeMs)} hint="Since last boot" />
              <MetricCard
                label="Wi-Fi RSSI"
                value={deviceState?.wifiRssi === null || deviceState?.wifiRssi === undefined ? '-' : `${deviceState.wifiRssi} dBm`}
                hint={deviceState?.wifiConnected ? 'Current station signal strength' : 'Not connected'}
              />
              <MetricCard label="Free Heap" value={formatBytes(deviceState?.freeHeap)} hint={`Min ${formatBytes(deviceState?.minFreeHeap)}`} />
            </section>

            <section className="grid gap-3 md:grid-cols-2 xl:grid-cols-3">
              <MetricCard label="Chip" value={deviceState ? `${deviceState.chipModel} rev ${deviceState.chipRevision}` : '-'} hint={`${deviceState?.chipCores || '-'} core(s)`} />
              <MetricCard label="SDK" value={deviceState?.sdkVersion || '-'} hint={`${deviceState?.cpuFreqMHz || '-'} MHz CPU`} />
              <MetricCard label="Flash" value={formatBytes(deviceState?.flashChipSize)} hint={`Sketch ${formatBytes(deviceState?.sketchSize)}`} />
              <MetricCard label="Free Sketch Space" value={formatBytes(deviceState?.freeSketchSpace)} hint="Remaining OTA/app slot space" />
              <MetricCard label="eFuse MAC" value={deviceState?.efuseMac || '-'} hint={deviceState?.ip || 'No current IP'} />
              <MetricCard label="BLE Stack" value={deviceState?.bleStack || '-'} hint={deviceState?.bleReady ? 'Bluetooth initialized' : 'Bluetooth unavailable'} />
            </section>

            <div className="rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
              <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Sensor Notes</div>
              <h2 className="mt-2 text-2xl font-bold text-slate-900">Internal Device Status</h2>
              <p className="mt-3 text-sm leading-6 text-slate-600">
                On this firmware build, the main internal sensor exposed directly by the Arduino ESP32 core is the chip temperature sensor. The rest of this page shows device telemetry such as memory, flash usage, radio state, and chip metadata.
              </p>
            </div>
          </section>
        );
      }
)HTML";
