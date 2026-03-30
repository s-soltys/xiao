#pragma once

const char kWebAppSharedComponents[] PROGMEM = R"HTML(
      const fallbackAppRegistry = [
        {
          id: 'matrix',
          label: 'RGB Matrix',
          description: 'Control the 6x10 WS2812B panel on A0/D0 with animated effects.',
          routeBase: '/api/matrix',
          stateRoute: '/api/matrix',
          actionRoute: '/api/matrix',
          available: true,
        },
        {
          id: 'mood',
          label: 'Mood App',
          description: 'Show one handmade emoji-style mood icon on the RGB matrix.',
          routeBase: '/api/mood',
          stateRoute: '/api/mood',
          actionRoute: '/api/mood',
          available: true,
        },
        {
          id: 'message',
          label: 'Message App',
          description: 'Send a message that loops right-to-left across the RGB matrix.',
          routeBase: '/api/message',
          stateRoute: '/api/message',
          actionRoute: '/api/message',
          available: true,
        },
        {
          id: 'bluetooth',
          label: 'Bluetooth Scanner',
          description: 'Scan nearby BLE advertisers and inspect signal details.',
          routeBase: '/api/bluetooth',
          stateRoute: '/api/bluetooth',
          actionRoute: '/api/bluetooth/scan',
          available: true,
        },
        {
          id: 'device',
          label: 'Device Info',
          description: 'Chip telemetry, internal temperature, memory, flash, and radio status.',
          routeBase: '/api/device',
          stateRoute: '/api/device',
          actionRoute: '',
          available: true,
        },
      ];

      function useDeviceApi() {
        async function fetchJson(url, options) {
          const response = await fetch(url, options);
          const payload = await response.json();
          if (!response.ok) {
            throw new Error(payload.error || `Request failed with ${response.status}`);
          }
          return payload;
        }

        return { fetchJson };
      }

      function useAppRegistry(fetchJson) {
        const [appRegistry, setAppRegistry] = useState([]);

        async function refreshAppRegistry() {
          const payload = await fetchJson('/api/apps', { cache: 'no-store' });
          setAppRegistry(Array.isArray(payload.apps) ? payload.apps : []);
          return payload;
        }

        return {
          appRegistry,
          refreshAppRegistry,
        };
      }

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

      function AppTab({ app, activeApp, onSelect }) {
        const active = activeApp === app.id;
        const availabilityTone = app.available ? 'text-teal-700' : 'text-amber-700';

        return (
          <button
            type="button"
            onClick={() => onSelect(app.id)}
            className={[
              'rounded-3xl border px-4 py-4 text-left transition',
              active
                ? 'border-slate-950 bg-slate-950 text-white shadow-panel'
                : 'border-white/70 bg-white/80 text-slate-900 hover:border-slate-300 hover:bg-white',
            ].join(' ')}
          >
            <div className="flex items-center justify-between gap-3">
              <div className="text-sm font-bold">{app.label}</div>
              <div
                className={[
                  'rounded-full px-2 py-1 text-[10px] font-semibold uppercase tracking-[0.22em]',
                  active ? 'bg-white/10 text-teal-300' : app.available ? 'bg-teal-50 text-teal-700' : 'bg-amber-50 text-amber-700',
                ].join(' ')}
              >
                {app.available ? 'Ready' : 'Unavailable'}
              </div>
            </div>
            <div className={`mt-2 text-sm ${active ? 'text-slate-300' : 'text-slate-500'}`}>{app.description}</div>
            <div className={`mt-3 text-[11px] font-medium uppercase tracking-[0.24em] ${active ? 'text-teal-300' : availabilityTone}`}>
              {app.routeBase}
            </div>
          </button>
        );
      }

      function MetricCard({ label, value, hint }) {
        return (
          <div className="rounded-3xl border border-white/70 bg-white/85 px-4 py-4 shadow-sm">
            <div className="text-[11px] font-medium uppercase tracking-[0.24em] text-slate-500">{label}</div>
            <div className="mt-2 text-2xl font-bold text-slate-900">{value}</div>
            {hint ? <div className="mt-2 text-sm text-slate-500">{hint}</div> : null}
          </div>
        );
      }

      function formatBytes(value) {
        if (value === null || value === undefined) {
          return '-';
        }

        if (value >= 1024 * 1024) {
          return `${(value / (1024 * 1024)).toFixed(2)} MB`;
        }

        if (value >= 1024) {
          return `${(value / 1024).toFixed(1)} KB`;
        }

        return `${value} B`;
      }

      function formatUptime(ms) {
        if (!ms && ms !== 0) {
          return '-';
        }

        const totalSeconds = Math.floor(ms / 1000);
        const hours = Math.floor(totalSeconds / 3600);
        const minutes = Math.floor((totalSeconds % 3600) / 60);
        const seconds = totalSeconds % 60;
        return `${hours}h ${minutes}m ${seconds}s`;
      }

      function formatRelativeMs(ms) {
        if (!ms) {
          return '-';
        }

        return formatUptime(ms);
      }

      const kMatrixBrightnessSliderMax = 255;
      const kMatrixBrightnessCurve = 2.4;

      function clampNumber(value, min, max) {
        return Math.min(max, Math.max(min, value));
      }

      function matrixBrightnessFromSliderValue(sliderValue) {
        const normalized = clampNumber(Number(sliderValue), 0, kMatrixBrightnessSliderMax) / kMatrixBrightnessSliderMax;
        return Math.round(Math.pow(normalized, kMatrixBrightnessCurve) * 255);
      }

      function sliderValueFromMatrixBrightness(brightnessValue) {
        const normalized = clampNumber(Number(brightnessValue), 0, 255) / 255;
        return String(Math.round(Math.pow(normalized, 1 / kMatrixBrightnessCurve) * kMatrixBrightnessSliderMax));
      }

      function isDedicatedMatrixAppPattern(patternId) {
        return patternId === 'mood' || patternId === 'message';
      }
)HTML";
