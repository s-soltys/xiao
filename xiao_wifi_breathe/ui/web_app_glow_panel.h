#pragma once

const char kWebAppGlowPanel[] PROGMEM = R"HTML(
      function GlowAppPanel({ glowState, matrixState, glowColor, onGlowColorChange, onApplyGlow, glowBusy }) {
        return (
          <section className="grid gap-6">
            <div className="grid gap-4 lg:grid-cols-[1fr_1fr]">
              <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-5 text-white">
                <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">Solid Glow</div>
                <div className="mt-2 text-2xl font-bold sm:text-3xl">{glowState?.color || '#22c55e'}</div>
                <p className="mt-3 max-w-2xl text-sm leading-6 text-slate-300">
                  Fill the whole 6x10 panel with one dedicated color. This picker now belongs only to Solid Glow and does not tint the RGB effects, mood icons, or scrolling message.
                </p>
                <div className="mt-4 flex flex-wrap gap-2 text-xs font-medium uppercase tracking-[0.2em] text-slate-300">
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">
                    {glowState?.glowActive ? 'Glowing on matrix now' : 'Glow color saved'}
                  </span>
                  <span className="rounded-full border border-white/10 bg-white/5 px-3 py-1">
                    {matrixState?.enabled ? 'Matrix enabled' : 'Matrix disabled'}
                  </span>
                </div>
              </div>

              <form className="grid gap-4 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm" onSubmit={onApplyGlow}>
                <div>
                  <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Glow Color</div>
                  <h2 className="mt-2 text-2xl font-bold text-slate-900">Pick one solid matrix color</h2>
                  <p className="mt-2 text-sm text-slate-600">
                    Saving here switches the matrix into full-panel glow mode immediately. This setting only affects the Solid Glow app.
                  </p>
                </div>

                <div className="grid gap-4 sm:grid-cols-[auto_1fr] sm:items-center">
                  <input
                    type="color"
                    value={glowColor}
                    onChange={(event) => onGlowColorChange(event.target.value)}
                    disabled={glowBusy || !glowState?.available}
                    className="h-20 w-full cursor-pointer rounded-3xl border border-slate-300 bg-white p-2 disabled:cursor-not-allowed disabled:opacity-60 sm:w-28"
                  />
                  <div className="grid gap-2">
                    <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-slate-500">Selected Color</div>
                    <div className="text-3xl font-bold text-slate-900">{glowColor}</div>
                    <div className="text-sm text-slate-500">Global output stays at {matrixState?.brightness ?? 0}/255 with animation speed at {matrixState?.animationSpeed ?? 100}%.</div>
                  </div>
                </div>

                <button
                  type="submit"
                  disabled={glowBusy || !glowState?.available}
                  className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                >
                  {glowBusy ? 'Applying Glow...' : 'Show Solid Glow'}
                </button>
              </form>
            </div>

            <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-4">
              <StatusCard label="Matrix Output" value={matrixState?.enabled ? 'Enabled' : 'Disabled'} tone={matrixState?.enabled ? 'online' : 'offline'} />
              <StatusCard label="Output" value={`${matrixState?.brightness ?? 0}/255 • ${matrixState?.animationSpeed ?? 100}%`} tone="neutral" />
              <StatusCard label="Glow Color" value={glowState?.color || '#22c55e'} tone="neutral" />
              <StatusCard label="Current Pattern" value={glowState?.selectedPatternLabel || 'Loading'} tone="neutral" />
            </section>
          </section>
        );
      }
)HTML";
