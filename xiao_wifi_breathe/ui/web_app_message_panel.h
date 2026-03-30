#pragma once

const char kWebAppMessagePanel[] PROGMEM = R"HTML(
      function MessageAppPanel({ messageState, matrixState, messageInput, onMessageInputChange, onApplyMessage, messageBusy }) {
        return (
          <section className="grid gap-6">
            <div className="grid gap-4 lg:grid-cols-[1fr_1fr]">
              <div className="rounded-3xl border border-slate-200 bg-slate-950 px-5 py-5 text-white">
                <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-teal-300">Scrolling Message</div>
                <div className="mt-2 text-2xl font-bold sm:text-3xl">{messageState?.text || 'No message saved'}</div>
                <p className="mt-3 max-w-2xl text-sm leading-6 text-slate-300">
                  The message enters from the right, moves across the matrix, exits on the left, and loops continuously using its own fixed high-contrast text color.
                </p>
                <div className="mt-4 inline-flex rounded-full border border-white/10 bg-white/5 px-3 py-1 text-xs font-medium uppercase tracking-[0.2em] text-slate-300">
                  {messageState?.selectedPatternId === 'message' ? 'Looping on matrix now' : 'Saved and ready'}
                </div>
              </div>

              <form className="grid gap-4 rounded-[1.75rem] border border-slate-200 bg-white/85 p-5 shadow-sm" onSubmit={onApplyMessage}>
                <div>
                  <div className="text-[11px] font-medium uppercase tracking-[0.28em] text-teal-700">Message Input</div>
                  <h2 className="mt-2 text-2xl font-bold text-slate-900">Type a message for the matrix</h2>
                  <p className="mt-2 text-sm text-slate-600">
                    Supported characters: <span className="font-medium">{messageState?.supportedCharacters || 'A-Z, 0-9, space, . , ! ? -'}</span>.
                  </p>
                </div>

                <input
                  type="text"
                  value={messageInput}
                  onChange={(event) => onMessageInputChange(event.target.value)}
                  maxLength={messageState?.maxLength || 64}
                  placeholder="HELLO XIAO"
                  disabled={messageBusy || !messageState?.available}
                  className="w-full rounded-2xl border border-slate-300 bg-white px-4 py-3 text-base text-slate-900 outline-none transition placeholder:text-slate-400 focus:border-teal-500 focus:ring-2 focus:ring-teal-200 disabled:opacity-60"
                />

                <button
                  type="submit"
                  disabled={messageBusy || !messageState?.available}
                  className="inline-flex min-h-[52px] items-center justify-center rounded-full bg-slate-950 px-6 py-3 text-sm font-semibold text-white transition hover:bg-slate-800 disabled:cursor-not-allowed disabled:opacity-60"
                >
                  {messageBusy ? 'Saving Message...' : 'Show Message'}
                </button>
              </form>
            </div>

            <section className="grid gap-3 sm:grid-cols-2 xl:grid-cols-5">
              <StatusCard
                label="Scroll"
                value={messageState?.scrollDirection === 'right-to-left' ? 'Right to Left' : '-'}
                tone="neutral"
              />
              <StatusCard label="Max Length" value={messageState?.maxLength || 64} tone="neutral" />
              <StatusCard label="Mode" value={messageState?.selectedPatternId === 'message' ? 'Message Active' : 'Message Saved'} tone="neutral" />
              <StatusCard label="Output" value={`${matrixState?.enabled ? 'Enabled' : 'Disabled'} • ${matrixState?.brightness ?? 0}/255 • ${matrixState?.animationSpeed ?? 100}%`} tone="neutral" />
            </section>
          </section>
        );
      }
)HTML";
