#pragma once

const char kWebAppMatrixPanel[] PROGMEM = R"HTML(
      function MatrixAppPanel({ matrixState, matrixBusy, matrixMapping, onMatrixMappingChange, onApplyMatrixControls, onSelectPattern }) {
        const matrixOwnershipNote =
          matrixState?.selectedPatternId === 'mood'
            ? 'Controlled by Mood App'
            : matrixState?.selectedPatternId === 'message'
              ? 'Controlled by Message App'
              : matrixState?.selectedPatternId === 'solid'
                ? 'Controlled by Solid Glow'
                : 'Effects switch immediately';

        return (
          <section className="grid gap-6">
            <div className="grid gap-4 lg:grid-cols-[1.08fr_0.92fr]">
              <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-5 text-white">
                <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">RGB Matrix Output</div>
                <div className="mt-2 text-2xl font-bold sm:text-3xl">{matrixState?.selectedPatternLabel || 'Loading effect'}</div>
                <p className="mt-3 max-w-2xl text-sm leading-6 text-slate-300">
                  Drives a 6x10 WS2812B panel from the XIAO ESP32-C6 on <span className="font-semibold text-white">A0/D0</span>. Shared output power, brightness, and animation speed now live in the header, while solid color has its own dedicated tab.
                </p>
                <div className="mt-4 flex flex-wrap gap-2 text-xs font-medium uppercase tracking-[0.2em] text-slate-300">
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{matrixState?.pixelCount || 60} pixels</span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">Brightness {matrixState?.brightness ?? 0}/255</span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">Speed {matrixState?.animationSpeed ?? 100}%</span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{matrixState?.dataPin || 'A0/D0'}</span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">{matrixOwnershipNote}</span>
                </div>
              </div>

              <form className="grid gap-4 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm" onSubmit={onApplyMatrixControls}>
                <div>
                  <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Panel Controls</div>
                  <h2 className="mt-2 text-2xl font-bold text-slate-900">Effect routing and mapping</h2>
                  <p className="mt-2 text-sm text-slate-600">
                    Pick animated effects here, then change the matrix wiring map if text or faces look scrambled on the real panel.
                  </p>
                </div>

                <div className="rounded-3xl border border-slate-200 bg-slate-50 px-4 py-4 text-sm text-slate-600">
                  Shared output controls are global:
                  <div className="mt-2 text-slate-900">Power is {matrixState?.enabled ? 'enabled' : 'disabled'}, brightness is {matrixState?.brightness ?? 0}/255, speed is {matrixState?.animationSpeed ?? 100}%, and shared color comes from the Solid Glow tab.</div>
                </div>

                <div className="grid gap-2">
                  <label className="text-[11px] font-medium uppercase tracking-[0.24em] text-slate-500">Pixel Mapping</label>
                  <select
                    value={matrixMapping}
                    onChange={(event) => onMatrixMappingChange(event.target.value)}
                    disabled={matrixBusy || !matrixState?.available}
                    className="w-full rounded-2xl border border-slate-300 bg-white px-4 py-3 text-base text-slate-900 outline-none transition focus:border-teal-500 focus:ring-2 focus:ring-teal-200 disabled:opacity-60"
                  >
                    {(matrixState?.mappings || []).map((mapping) => (
                      <option key={mapping.id} value={mapping.id}>
                        {mapping.label}
                      </option>
                    ))}
                  </select>
                </div>

                <button
                  type="submit"
                  disabled={matrixBusy || !matrixState?.available}
                  className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                >
                  {matrixBusy ? 'Applying Mapping...' : 'Apply Mapping'}
                </button>
              </form>
            </div>

            <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
              <StatusCard
                label="Matrix Driver"
                value={matrixState?.available ? 'Ready' : 'Unavailable'}
                tone={matrixState?.available ? 'online' : 'offline'}
              />
              <StatusCard label="Geometry" value={`${matrixState?.rows || 6} x ${matrixState?.columns || 10}`} tone="neutral" />
              <StatusCard label="Shared Color" value={matrixState?.color || '#22c55e'} tone="neutral" />
              <StatusCard label="Mapping" value={matrixState?.mappingLabel || 'Loading'} tone="neutral" />
              <StatusCard label="Active Effect" value={matrixState?.selectedPatternLabel || 'Loading'} tone="neutral" />
            </section>

            {matrixState?.error ? (
              <div className="rounded-2xl border border-amber-200 bg-amber-50 px-4 py-3 text-sm font-medium text-amber-800">{matrixState.error}</div>
            ) : null}

            <section className="grid gap-3 lg:grid-cols-2 2xl:grid-cols-4">
              <MetricCard
                label="GPIO"
                value={matrixState?.gpio === null || matrixState?.gpio === undefined ? '-' : matrixState.gpio}
                hint={matrixState?.dataPin || 'A0/D0 data output'}
              />
              <MetricCard label="Pixels" value={matrixState?.pixelCount || 60} hint="WS2812B LEDs in a 6x10 grid" />
              <MetricCard
                label="Output"
                value={`${matrixState?.enabled ? 'Enabled' : 'Disabled'} • ${matrixState?.brightness ?? 0}/255 • ${matrixState?.animationSpeed ?? 100}%`}
                hint="Shared across RGB Matrix, Solid Glow, Mood, and Message"
              />
              <div className="rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
                <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Wiring Note</div>
                <h2 className="mt-2 text-2xl font-bold text-slate-900">Panel input</h2>
                <p className="mt-3 text-sm leading-6 text-slate-600">
                  This firmware assumes the matrix data input is connected to <span className="font-medium text-slate-900">A0 / D0 / GPIO 0</span>. If the panel looks mirrored or scrambled, use the mapping selector above before changing firmware again.
                </p>
              </div>
            </section>

            <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
              {(matrixState?.patterns || []).map((pattern, index) => {
                const isActive = pattern.id === matrixState.selectedPatternId;

                return (
                  <button
                    key={pattern.id}
                    type="button"
                    onClick={() => onSelectPattern(pattern.id)}
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
                    <div className="text-[11px] font-medium uppercase tracking-[0.28em] opacity-70">{String(index + 1).padStart(2, '0')}</div>
                    <div className="mt-4 text-xl font-bold">{pattern.label}</div>
                    <div className={`mt-3 text-sm ${isActive ? 'text-teal-300' : 'text-slate-500'}`}>
                      {isActive ? 'Running on the panel' : 'Tap to activate'}
                    </div>
                  </button>
                );
              })}
            </section>
          </section>
        );
      }
)HTML";
