#pragma once

const char kWebAppDocumentStart[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>XIAO Device Console</title>
    <style>
      :root {
        color-scheme: light;
        --bg-a: #fff7ed;
        --bg-b: #f8fafc;
        --panel: rgba(255, 255, 255, 0.94);
        --panel-border: rgba(148, 163, 184, 0.25);
        --text: #0f172a;
        --muted: #475569;
        --accent: #0f766e;
        --accent-strong: #0f172a;
        --danger: #b91c1c;
        --danger-bg: #fff1f2;
        --ok-bg: #ecfeff;
      }

      * {
        box-sizing: border-box;
      }

      html,
      body {
        margin: 0;
        min-height: 100%;
        font-family: "Segoe UI", Helvetica, Arial, sans-serif;
        background:
          radial-gradient(circle at top left, rgba(13, 148, 136, 0.16), transparent 34%),
          radial-gradient(circle at bottom right, rgba(245, 158, 11, 0.18), transparent 30%),
          linear-gradient(180deg, var(--bg-a) 0%, #fffbf5 46%, var(--bg-b) 100%);
        color: var(--text);
      }

      a {
        color: var(--accent);
      }

      button,
      select,
      input {
        font: inherit;
      }

      #offline-fallback {
        min-height: 100vh;
        padding: 24px;
      }

      .offline-shell {
        max-width: 1180px;
        margin: 0 auto;
        display: grid;
        gap: 20px;
      }

      .offline-card {
        background: var(--panel);
        border: 1px solid var(--panel-border);
        border-radius: 28px;
        padding: 20px;
        box-shadow: 0 24px 60px -30px rgba(15, 23, 42, 0.4);
        backdrop-filter: blur(16px);
      }

      .offline-hero {
        display: grid;
        gap: 16px;
      }

      .offline-eyebrow {
        font-size: 11px;
        font-weight: 700;
        letter-spacing: 0.28em;
        text-transform: uppercase;
        color: var(--accent);
      }

      .offline-title {
        margin: 0;
        font-size: clamp(30px, 5vw, 48px);
        line-height: 1.05;
      }

      .offline-copy {
        margin: 0;
        color: var(--muted);
        line-height: 1.6;
      }

      .offline-grid {
        display: grid;
        gap: 12px;
        grid-template-columns: repeat(auto-fit, minmax(160px, 1fr));
      }

      .offline-metric {
        border-radius: 22px;
        border: 1px solid var(--panel-border);
        background: rgba(248, 250, 252, 0.9);
        padding: 14px 16px;
      }

      .offline-metric-label {
        font-size: 11px;
        font-weight: 700;
        letter-spacing: 0.22em;
        text-transform: uppercase;
        color: #64748b;
      }

      .offline-metric-value {
        margin-top: 8px;
        font-size: 20px;
        font-weight: 700;
        word-break: break-word;
      }

      .offline-layout {
        display: grid;
        gap: 20px;
        grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      }

      .offline-section-title {
        margin: 0;
        font-size: 24px;
      }

      .offline-stack {
        display: grid;
        gap: 12px;
      }

      .offline-row {
        display: grid;
        gap: 10px;
      }

      .offline-row.inline {
        grid-template-columns: 1fr auto;
        align-items: end;
      }

      .offline-label {
        font-size: 11px;
        font-weight: 700;
        letter-spacing: 0.22em;
        text-transform: uppercase;
        color: #64748b;
      }

      .offline-input,
      .offline-select,
      .offline-button {
        min-height: 48px;
        border-radius: 18px;
        border: 1px solid rgba(148, 163, 184, 0.4);
        padding: 12px 14px;
      }

      .offline-input,
      .offline-select {
        width: 100%;
        background: #fff;
        color: var(--text);
      }

      .offline-button {
        cursor: pointer;
        background: var(--accent-strong);
        color: #fff;
        font-weight: 700;
      }

      .offline-button.secondary {
        background: #fff;
        color: var(--text);
      }

      .offline-button.ghost {
        background: #f8fafc;
        color: var(--text);
      }

      .offline-actions,
      .offline-links,
      .offline-moods {
        display: flex;
        flex-wrap: wrap;
        gap: 10px;
      }

      .offline-links a {
        display: inline-flex;
        align-items: center;
        justify-content: center;
        min-height: 42px;
        padding: 0 14px;
        border-radius: 999px;
        border: 1px solid rgba(148, 163, 184, 0.35);
        background: #fff;
        text-decoration: none;
        color: var(--text);
      }

      .offline-note {
        border-radius: 22px;
        padding: 14px 16px;
        border: 1px solid rgba(148, 163, 184, 0.28);
        background: rgba(248, 250, 252, 0.88);
        color: var(--muted);
        line-height: 1.55;
      }

      .offline-feedback {
        display: none;
        border-radius: 20px;
        padding: 14px 16px;
        font-weight: 600;
      }

      .offline-feedback.info {
        display: block;
        background: var(--ok-bg);
        color: #155e75;
      }

      .offline-feedback.error {
        display: block;
        background: var(--danger-bg);
        color: var(--danger);
      }

      @media (max-width: 720px) {
        #offline-fallback {
          padding: 16px;
        }

        .offline-card {
          border-radius: 22px;
          padding: 16px;
        }

        .offline-row.inline {
          grid-template-columns: 1fr;
        }
      }
    </style>
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
    <div id="offline-fallback">
      <main class="offline-shell">
        <section class="offline-card offline-hero">
          <div class="offline-eyebrow">Offline Device Access</div>
          <h1 class="offline-title">XIAO Device Console</h1>
          <p class="offline-copy" id="offline-status-copy">
            Loading the hosted device UI. If the full app does not appear, the controls below are served directly by the XIAO and work without React or Tailwind CDNs.
          </p>
          <div class="offline-grid">
            <div class="offline-metric">
              <div class="offline-metric-label">Hosted SSID</div>
              <div class="offline-metric-value" id="offline-ssid">Loading...</div>
            </div>
            <div class="offline-metric">
              <div class="offline-metric-label">Address</div>
              <div class="offline-metric-value" id="offline-address">Loading...</div>
            </div>
            <div class="offline-metric">
              <div class="offline-metric-label">Security</div>
              <div class="offline-metric-value" id="offline-security">Loading...</div>
            </div>
            <div class="offline-metric">
              <div class="offline-metric-label">Clients</div>
              <div class="offline-metric-value" id="offline-clients">Loading...</div>
            </div>
          </div>
          <div class="offline-actions">
            <button class="offline-button" type="button" id="offline-refresh">Refresh Device State</button>
            <button class="offline-button secondary" type="button" id="offline-retry-ui">Try Full App Again</button>
          </div>
          <div class="offline-links">
            <a href="/api/state" target="_blank" rel="noreferrer">Open /api/state</a>
            <a href="/api/matrix" target="_blank" rel="noreferrer">Open /api/matrix</a>
            <a href="/api/device" target="_blank" rel="noreferrer">Open /api/device</a>
          </div>
        </section>

        <div id="offline-feedback" class="offline-feedback"></div>

        <section class="offline-layout">
          <article class="offline-card offline-stack">
            <div class="offline-eyebrow">Status LED</div>
            <h2 class="offline-section-title">Patterns and Morse</h2>
            <div class="offline-row inline">
              <div class="offline-row">
                <label class="offline-label" for="offline-pattern-select">Pattern</label>
                <select id="offline-pattern-select" class="offline-select"></select>
              </div>
              <button class="offline-button" type="button" id="offline-apply-pattern">Apply Pattern</button>
            </div>
            <div class="offline-row inline">
              <div class="offline-row">
                <label class="offline-label" for="offline-morse-input">Morse Text</label>
                <input id="offline-morse-input" class="offline-input" type="text" placeholder="HELLO XIAO" maxlength="64" />
              </div>
              <button class="offline-button" type="button" id="offline-apply-morse">Send Morse</button>
            </div>
          </article>

          <article class="offline-card offline-stack">
            <div class="offline-eyebrow">RGB Matrix</div>
            <h2 class="offline-section-title">Effects, moods, and message</h2>
            <div class="offline-row inline">
              <div class="offline-row">
                <label class="offline-label" for="offline-matrix-select">Effect</label>
                <select id="offline-matrix-select" class="offline-select"></select>
              </div>
              <button class="offline-button" type="button" id="offline-apply-matrix">Apply Effect</button>
            </div>
            <div class="offline-row">
              <div class="offline-label">Moods</div>
              <div class="offline-moods" id="offline-moods"></div>
            </div>
            <div class="offline-row inline">
              <div class="offline-row">
                <label class="offline-label" for="offline-message-input">Message</label>
                <input id="offline-message-input" class="offline-input" type="text" placeholder="HELLO" maxlength="64" />
              </div>
              <button class="offline-button" type="button" id="offline-apply-message">Send Message</button>
            </div>
          </article>

          <article class="offline-card offline-stack">
            <div class="offline-eyebrow">Bluetooth</div>
            <h2 class="offline-section-title">Scanner and diagnostics</h2>
            <div class="offline-grid">
              <div class="offline-metric">
                <div class="offline-metric-label">BLE Status</div>
                <div class="offline-metric-value" id="offline-ble-status">Loading...</div>
              </div>
              <div class="offline-metric">
                <div class="offline-metric-label">Scan Results</div>
                <div class="offline-metric-value" id="offline-ble-count">Loading...</div>
              </div>
            </div>
            <div class="offline-actions">
              <button class="offline-button" type="button" id="offline-start-ble-scan">Start BLE Scan</button>
            </div>
            <div class="offline-note">
              If the full React interface does not load on the hosted Wi-Fi, this fallback view still lets you reach the main APIs and control the device directly.
            </div>
          </article>
        </section>
      </main>
    </div>

    <div id="root" style="display: none"></div>

    <script>
      (function() {
        const offlineState = {
          uiLoaded: false,
        };

        function byId(id) {
          return document.getElementById(id);
        }

        function setText(id, value) {
          const element = byId(id);
          if (element) {
            element.textContent = value;
          }
        }

        function setFeedback(message, tone) {
          const element = byId('offline-feedback');
          if (!element) {
            return;
          }

          if (!message) {
            element.className = 'offline-feedback';
            element.textContent = '';
            return;
          }

          element.className = 'offline-feedback ' + (tone || 'info');
          element.textContent = message;
        }

        function populateSelect(id, items, selectedValue) {
          const select = byId(id);
          if (!select) {
            return;
          }

          const options = Array.isArray(items) ? items : [];
          select.innerHTML = '';

          options.forEach(function(item) {
            const option = document.createElement('option');
            option.value = item.id || '';
            option.textContent = item.label || item.id || 'Unknown';
            if ((item.id || '') === selectedValue) {
              option.selected = true;
            }
            select.appendChild(option);
          });
        }

        function renderMoodButtons(moods, selectedMoodId) {
          const container = byId('offline-moods');
          if (!container) {
            return;
          }

          container.innerHTML = '';
          (Array.isArray(moods) ? moods : []).forEach(function(mood) {
            const button = document.createElement('button');
            button.type = 'button';
            button.className = 'offline-button' + ((mood.id || '') === selectedMoodId ? '' : ' ghost');
            button.textContent = mood.label || mood.id || 'Mood';
            button.addEventListener('click', function() {
              applyMood(mood.id || '');
            });
            container.appendChild(button);
          });
        }

        async function fetchJson(url, options) {
          const response = await fetch(url, options);
          const payload = await response.json().catch(function() {
            return {};
          });

          if (!response.ok) {
            throw new Error(payload.error || ('Request failed with ' + response.status));
          }

          return payload;
        }

        async function postJson(url, body) {
          return fetchJson(url, {
            method: 'POST',
            headers: {
              'Content-Type': 'application/json',
            },
            body: JSON.stringify(body),
          });
        }

        async function refreshOfflineDashboard() {
          setFeedback('', '');

          try {
            const results = await Promise.all([
              fetchJson('/api/state', { cache: 'no-store' }),
              fetchJson('/api/wifi', { cache: 'no-store' }),
              fetchJson('/api/matrix', { cache: 'no-store' }),
              fetchJson('/api/mood', { cache: 'no-store' }),
              fetchJson('/api/message', { cache: 'no-store' }),
              fetchJson('/api/bluetooth', { cache: 'no-store' }),
            ]);

            const systemState = results[0] || {};
            const wifiState = results[1] || {};
            const matrixState = results[2] || {};
            const moodState = results[3] || {};
            const messageState = results[4] || {};
            const bluetoothState = results[5] || {};

            setText('offline-ssid', wifiState.ssid || 'Unavailable');
            setText('offline-address', wifiState.address || wifiState.ip || systemState.address || systemState.ip || 'Unavailable');
            setText('offline-security', wifiState.security === 'open' ? 'Open network' : 'WPA2 password');
            setText('offline-clients', String(wifiState.clientCount || 0));
            setText('offline-ble-status', bluetoothState.scanning ? 'Scanning' : (bluetoothState.available ? 'Ready' : 'Unavailable'));
            setText('offline-ble-count', String((bluetoothState.results || []).length));

            const hostedAddress = wifiState.address || wifiState.ip || systemState.address || systemState.ip || 'http://192.168.4.1/';
            setText(
              'offline-status-copy',
              offlineState.uiLoaded
                ? 'The full app loaded successfully.'
                : 'Full app assets are still loading. The device is reachable at ' + hostedAddress + '. If the full UI stays blank, use the controls below.'
            );

            populateSelect('offline-pattern-select', systemState.patterns, systemState.selectedPatternId);
            populateSelect('offline-matrix-select', matrixState.patterns, matrixState.selectedPatternId);
            renderMoodButtons(moodState.moods, moodState.selectedMoodId);

            const morseInput = byId('offline-morse-input');
            if (morseInput && document.activeElement !== morseInput) {
              morseInput.value = systemState.morseText || '';
            }

            const messageInput = byId('offline-message-input');
            if (messageInput && document.activeElement !== messageInput) {
              messageInput.value = messageState.text || '';
            }
          } catch (error) {
            setFeedback(error.message || 'Unable to refresh the fallback device controls.', 'error');
          }
        }

        async function applyPattern() {
          const select = byId('offline-pattern-select');
          if (!select || !select.value) {
            return;
          }

          try {
            await postJson('/api/pattern', { id: select.value });
            setFeedback('Status LED pattern updated.', 'info');
            await refreshOfflineDashboard();
          } catch (error) {
            setFeedback(error.message || 'Unable to update the status LED pattern.', 'error');
          }
        }

        async function applyMorse() {
          const input = byId('offline-morse-input');
          if (!input) {
            return;
          }

          try {
            await postJson('/api/morse', { text: input.value });
            setFeedback('Morse text updated.', 'info');
            await refreshOfflineDashboard();
          } catch (error) {
            setFeedback(error.message || 'Unable to update Morse text.', 'error');
          }
        }

        async function applyMatrixPattern() {
          const select = byId('offline-matrix-select');
          if (!select || !select.value) {
            return;
          }

          try {
            await postJson('/api/matrix', { patternId: select.value });
            setFeedback('RGB matrix effect updated.', 'info');
            await refreshOfflineDashboard();
          } catch (error) {
            setFeedback(error.message || 'Unable to update the RGB matrix effect.', 'error');
          }
        }

        async function applyMessage() {
          const input = byId('offline-message-input');
          if (!input) {
            return;
          }

          try {
            await postJson('/api/message', { text: input.value });
            setFeedback('Scrolling message updated.', 'info');
            await refreshOfflineDashboard();
          } catch (error) {
            setFeedback(error.message || 'Unable to update the message.', 'error');
          }
        }

        async function applyMood(moodId) {
          if (!moodId) {
            return;
          }

          try {
            await postJson('/api/mood', { id: moodId });
            setFeedback('Mood updated.', 'info');
            await refreshOfflineDashboard();
          } catch (error) {
            setFeedback(error.message || 'Unable to update the mood.', 'error');
          }
        }

        async function startBluetoothScan() {
          try {
            await postJson('/api/bluetooth/scan', {});
            setFeedback('Bluetooth scan started.', 'info');
            await refreshOfflineDashboard();
          } catch (error) {
            setFeedback(error.message || 'Unable to start the Bluetooth scan.', 'error');
          }
        }

        window.__xiaoMarkUiLoaded = function() {
          offlineState.uiLoaded = true;
          const fallback = byId('offline-fallback');
          const root = byId('root');
          if (fallback) {
            fallback.style.display = 'none';
          }
          if (root) {
            root.style.display = 'block';
          }
        };

        window.addEventListener('DOMContentLoaded', function() {
          const refreshButton = byId('offline-refresh');
          const retryButton = byId('offline-retry-ui');
          const applyPatternButton = byId('offline-apply-pattern');
          const applyMorseButton = byId('offline-apply-morse');
          const applyMatrixButton = byId('offline-apply-matrix');
          const applyMessageButton = byId('offline-apply-message');
          const startBleScanButton = byId('offline-start-ble-scan');

          if (refreshButton) {
            refreshButton.addEventListener('click', refreshOfflineDashboard);
          }
          if (retryButton) {
            retryButton.addEventListener('click', function() {
              window.location.reload();
            });
          }
          if (applyPatternButton) {
            applyPatternButton.addEventListener('click', applyPattern);
          }
          if (applyMorseButton) {
            applyMorseButton.addEventListener('click', applyMorse);
          }
          if (applyMatrixButton) {
            applyMatrixButton.addEventListener('click', applyMatrixPattern);
          }
          if (applyMessageButton) {
            applyMessageButton.addEventListener('click', applyMessage);
          }
          if (startBleScanButton) {
            startBleScanButton.addEventListener('click', startBluetoothScan);
          }

          refreshOfflineDashboard();

          window.setTimeout(function() {
            if (offlineState.uiLoaded) {
              return;
            }

            setText(
              'offline-status-copy',
              'The full React app did not load. This usually means the hosted Wi-Fi client could not fetch CDN assets. The controls on this page are served directly by the XIAO and continue to work offline.'
            );
          }, 1800);
        });
      })();
    </script>

    <script type="text/babel">
      const { useEffect, useRef, useState } = React;
)HTML";
