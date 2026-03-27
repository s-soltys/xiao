#pragma once

const char kWebAppHtml[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>XIAO Pattern Console</title>
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

      function App() {
        const [state, setState] = useState(null);
        const [loading, setLoading] = useState(true);
        const [busyId, setBusyId] = useState('');
        const [error, setError] = useState('');
        const [morseInput, setMorseInput] = useState('');

        async function fetchState() {
          setLoading(true);
          setError('');

          try {
            const response = await fetch('/api/state', { cache: 'no-store' });
            if (!response.ok) {
              throw new Error(`State request failed with ${response.status}`);
            }
            const payload = await response.json();
            setState(payload);
            setMorseInput(payload.morseText || '');
          } catch (fetchError) {
            setError(fetchError.message || 'Unable to load device state.');
          } finally {
            setLoading(false);
          }
        }

        async function activateMorse(event) {
          event.preventDefault();
          setBusyId('morse-text');
          setError('');

          try {
            const response = await fetch('/api/morse', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ text: morseInput }),
            });

            const payload = await response.json();
            if (!response.ok) {
              throw new Error(payload.error || `Morse update failed with ${response.status}`);
            }

            setState(payload);
            setMorseInput(payload.morseText || '');
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the Morse pattern.');
          } finally {
            setBusyId('');
          }
        }

        useEffect(() => {
          fetchState();
        }, []);

        async function activatePattern(patternId) {
          setBusyId(patternId);
          setError('');

          try {
            const response = await fetch('/api/pattern', {
              method: 'POST',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({ id: patternId }),
            });

            const payload = await response.json();
            if (!response.ok) {
              throw new Error(payload.error || `Pattern change failed with ${response.status}`);
            }

            setState(payload);
          } catch (requestError) {
            setError(requestError.message || 'Unable to update the active pattern.');
          } finally {
            setBusyId('');
          }
        }

        if (loading && !state) {
          return (
            <main className="mx-auto flex min-h-screen max-w-6xl items-center px-5 py-10">
              <div className="w-full rounded-[2rem] border border-white/70 bg-white/70 p-8 shadow-panel backdrop-blur">
                <p className="text-sm uppercase tracking-[0.3em] text-teal-700">Bootstrapping</p>
                <h1 className="mt-3 text-3xl font-bold sm:text-5xl">XIAO Pattern Console</h1>
                <p className="mt-4 max-w-2xl text-base text-slate-600 sm:text-lg">
                  Waiting for the device state endpoint to respond.
                </p>
              </div>
            </main>
          );
        }

        return (
          <main className="mx-auto flex min-h-screen max-w-6xl px-5 py-8 sm:py-10">
            <div className="w-full rounded-[2rem] border border-white/80 bg-white/75 p-6 shadow-panel backdrop-blur sm:p-8">
              <div className="flex flex-col gap-8">
                <section className="grid gap-6 lg:grid-cols-[1.4fr_1fr]">
                  <div>
                    <p className="text-xs font-medium uppercase tracking-[0.35em] text-teal-700">Local LED Control</p>
                    <h1 className="mt-3 text-4xl font-bold leading-tight text-slate-900 sm:text-5xl">
                      XIAO Pattern Console
                    </h1>
                    <p className="mt-4 max-w-2xl text-base text-slate-600 sm:text-lg">
                      Select one of ten stored LED behaviors. The active pattern switches immediately and
                      survives power cycles.
                    </p>
                  </div>

                  <div className="grid gap-3 sm:grid-cols-2 lg:grid-cols-1">
                    <StatusCard
                      label="Reachability"
                      value={state?.connected ? state.hostname : 'Waiting for Wi-Fi'}
                      tone={state?.connected ? 'online' : 'offline'}
                    />
                    <StatusCard
                      label="Network"
                      value={state?.connected ? `${state.ssid} • ${state.ip}` : 'STA mode retrying'}
                      tone={state?.connected ? 'neutral' : 'offline'}
                    />
                  </div>
                </section>

                <section className="grid gap-4 lg:grid-cols-[1fr_auto] lg:items-end">
                  <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-4 text-white">
                    <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">
                      Active Pattern
                    </div>
                    <div className="mt-2 text-2xl font-bold sm:text-3xl">
                      {state?.selectedPatternLabel || 'Unknown'}
                    </div>
                    <div className="mt-2 text-sm text-slate-300">
                      Hostname: <span className="font-semibold text-white">{state?.hostname || 'xiao.local'}</span>
                    </div>
                    {state?.selectedPatternId === 'morse-text' && state?.morseText ? (
                      <div className="mt-3 rounded-2xl border border-white/10 bg-white/5 px-3 py-2 text-sm text-slate-200">
                        Message: <span className="font-semibold text-white">{state.morseText}</span>
                      </div>
                    ) : null}
                  </div>

                  <button
                    type="button"
                    onClick={fetchState}
                    className="inline-flex items-center justify-center rounded-full border border-slate-300 bg-white px-5 py-3 text-sm font-semibold text-slate-800 transition hover:border-slate-900 hover:text-slate-900"
                  >
                    Refresh State
                  </button>
                </section>

                {error ? (
                  <div className="rounded-2xl border border-rose-200 bg-rose-50 px-4 py-3 text-sm font-medium text-rose-800">
                    {error}
                  </div>
                ) : null}

                <section className="grid gap-4 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm lg:grid-cols-[1.3fr_auto] lg:items-end">
                  <form className="grid gap-3" onSubmit={activateMorse}>
                    <div>
                      <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">
                        Custom Morse
                      </div>
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
                      disabled={Boolean(busyId)}
                      className="w-full rounded-2xl border border-slate-300 bg-white px-4 py-3 text-base text-slate-900 outline-none transition placeholder:text-slate-400 focus:border-teal-500 focus:ring-2 focus:ring-teal-200 disabled:opacity-60"
                    />
                  </form>

                  <button
                    type="button"
                    onClick={(event) => activateMorse(event)}
                    disabled={Boolean(busyId)}
                    className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                  >
                    {busyId === 'morse-text' ? 'Saving Morse…' : 'Blink Morse'}
                  </button>
                </section>

                <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-5">
                  {state?.patterns?.map((pattern, index) => {
                    const isActive = pattern.id === state.selectedPatternId;
                    const isBusy = busyId === pattern.id;

                    return (
                      <button
                        key={pattern.id}
                        type="button"
                        onClick={() => activatePattern(pattern.id)}
                        disabled={Boolean(busyId)}
                        className={[
                          'group min-h-[132px] rounded-3xl border px-4 py-4 text-left transition duration-200',
                          'focus:outline-none focus:ring-2 focus:ring-teal-400 focus:ring-offset-2',
                          isActive
                            ? 'border-slate-950 bg-slate-950 text-white shadow-panel'
                            : 'border-white/70 bg-white/80 text-slate-900 hover:-translate-y-0.5 hover:border-slate-300 hover:bg-white',
                          busyId && !isBusy ? 'opacity-60' : '',
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
