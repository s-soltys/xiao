#pragma once

const char kWebAppDocumentStart[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <title>XIAO Device Console</title>
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
      const { useEffect, useRef, useState } = React;
)HTML";
