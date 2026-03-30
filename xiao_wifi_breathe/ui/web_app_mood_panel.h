#pragma once

const char kWebAppMoodPanel[] PROGMEM = R"HTML(
      function MoodAppPanel({ moodState, matrixState, moodBusyId, onSelectMood }) {
        return (
          <section className="grid gap-6">
            <div className="grid gap-4 lg:grid-cols-[1fr_1fr]">
              <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-5 text-white">
                <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">Mood Display</div>
                <div className="mt-2 text-2xl font-bold sm:text-3xl">{moodState?.selectedMoodLabel || 'Loading mood'}</div>
                <p className="mt-3 max-w-2xl text-sm leading-6 text-slate-300">
                  Shows a handmade emoji-style icon on the 6x10 WS2812B panel. Matrix power, brightness, and animation speed stay shared across every matrix app.
                </p>
                <div className="mt-4 inline-flex rounded-full border border-white/10 bg-white/5 px-3 py-1 text-xs font-medium uppercase tracking-[0.2em] text-slate-300">
                  {moodState?.selectedPatternId === 'mood' ? 'Showing on matrix now' : 'Saved and ready'}
                </div>
              </div>

              <div className="grid gap-3 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm">
                <div>
                  <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Mood Notes</div>
                  <h2 className="mt-2 text-2xl font-bold text-slate-900">Pick a face for the matrix</h2>
                  <p className="mt-2 text-sm text-slate-600">
                    Saving a mood switches the matrix immediately into Mood mode. Shared power, brightness, and animation speed stay global, while shared color comes from the Solid Glow tab.
                  </p>
                </div>

                <section className="grid gap-3 sm:grid-cols-2">
                  <StatusCard
                    label="Matrix Driver"
                    value={moodState?.available ? 'Ready' : 'Unavailable'}
                    tone={moodState?.available ? 'online' : 'offline'}
                  />
                  <StatusCard label="Output" value={`${matrixState?.enabled ? 'Enabled' : 'Disabled'} • ${matrixState?.brightness ?? 0}/255 • ${matrixState?.animationSpeed ?? 100}%`} tone="neutral" />
                </section>
              </div>
            </div>

            <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-5">
              {moodState?.moods?.map((mood, index) => {
                const isSelected = mood.id === moodState.selectedMoodId;
                const isBusy = moodBusyId === mood.id;

                return (
                  <button
                    key={mood.id}
                    type="button"
                    onClick={() => onSelectMood(mood.id)}
                    disabled={Boolean(moodBusyId) || !moodState?.available}
                    className={[
                      'group min-h-[132px] rounded-3xl border px-4 py-4 text-left transition duration-200',
                      'focus:outline-none focus:ring-2 focus:ring-teal-400 focus:ring-offset-2',
                      isSelected
                        ? 'border-slate-950 bg-slate-950 text-white shadow-panel'
                        : 'border-white/70 bg-white/80 text-slate-900 hover:-translate-y-0.5 hover:border-slate-300 hover:bg-white',
                      moodBusyId && !isBusy ? 'opacity-60' : '',
                    ].join(' ')}
                  >
                    <div className="text-[11px] font-medium uppercase tracking-[0.28em] opacity-70">
                      {String(index + 1).padStart(2, '0')}
                    </div>
                    <div className="mt-4 text-xl font-bold">{mood.label}</div>
                    <div className={`mt-3 text-sm ${isSelected ? 'text-teal-300' : 'text-slate-500'}`}>
                      {isBusy ? 'Switching now...' : isSelected && moodState?.selectedPatternId === 'mood' ? 'Showing now' : isSelected ? 'Saved mood' : 'Tap to show'}
                    </div>
                  </button>
                );
              })}
            </section>
          </section>
        );
      }
)HTML";
