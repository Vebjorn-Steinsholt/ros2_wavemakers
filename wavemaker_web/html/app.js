'use strict';

const PLOT_SECONDS = 20;
const STALE_MS = 1000;  // no update for this long counts as disconnected
const GAP_SECONDS = 0.25;  // longer pauses between samples break the line

const $ = (id) => document.getElementById(id);
const series = { setpoint: [], position: [] };
let state = 'unknown';
let serverTime = 0;
let lastUpdate = 0;

function connected() {
  return Date.now() - lastUpdate < STALE_MS;
}

function render() {
  const online = connected();
  $('link').textContent = online ? 'connected' : 'disconnected';
  $('link').className = 'badge ' + (online ? 'ok' : 'bad');
  $('state').textContent = online ? state : 'unknown';
  $('state').className = 'badge ' + (
    !online ? '' : state === 'idle' ? 'ok' :
      ['starting', 'running', 'stopping'].includes(state) ? 'busy' : 'bad');
  $('start').disabled = !(online && state === 'idle');
  $('stop').disabled = !online;
  $('return').disabled = !(online && state === 'idle');
}

function onUpdate(update) {
  lastUpdate = Date.now();
  state = update.state;
  serverTime = update.time;
  $('namespace').textContent = update.namespace;
  $('message').textContent = update.message || '—';
  $('notice').hidden = !update.notice;
  $('notice').textContent = update.notice;
  for (const [t, kind, value] of update.samples) {
    series[kind].push([t, value]);
  }
  for (const points of Object.values(series)) {
    while (points.length && points[0][0] < serverTime - PLOT_SECONDS) points.shift();
  }
  render();
  drawPlot();
}

async function post(path, body) {
  const response = await fetch(path, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body || {}),
  });
  return response.json();
}

function connect() {
  const events = new EventSource('api/events');
  events.onmessage = (event) => {
    // Every acknowledged event keeps the bridge heartbeat going.
    fetch('api/ack', { method: 'POST' }).catch(() => {});
    onUpdate(JSON.parse(event.data));
  };
  events.onerror = () => render();
}

function drawPlot() {
  const canvas = $('plot');
  const ratio = window.devicePixelRatio || 1;
  const width = canvas.clientWidth;
  const height = canvas.clientHeight;
  if (canvas.width !== Math.round(width * ratio)) {
    canvas.width = Math.round(width * ratio);
    canvas.height = Math.round(height * ratio);
  }
  const ctx = canvas.getContext('2d');
  ctx.setTransform(ratio, 0, 0, ratio, 0, 0);
  ctx.clearRect(0, 0, width, height);

  const css = getComputedStyle(document.documentElement);
  const color = (name) => css.getPropertyValue(name).trim();
  const left = 52, right = 8, top = 8, bottom = 22;
  const plotWidth = width - left - right;
  const plotHeight = height - top - bottom;

  let extent = 0.01;
  for (const points of Object.values(series)) {
    for (const [, value] of points) extent = Math.max(extent, Math.abs(value));
  }
  extent *= 1.1;
  const x = (t) => left + (t - (serverTime - PLOT_SECONDS)) / PLOT_SECONDS * plotWidth;
  const y = (v) => top + (1 - (v + extent) / (2 * extent)) * plotHeight;

  ctx.font = '11px system-ui, sans-serif';
  ctx.fillStyle = color('--muted');
  ctx.strokeStyle = color('--grid');
  ctx.lineWidth = 1;
  for (const fraction of [-1, -0.5, 0, 0.5, 1]) {
    const value = fraction * extent / 1.1;
    ctx.beginPath();
    ctx.moveTo(left, y(value));
    ctx.lineTo(width - right, y(value));
    ctx.stroke();
    ctx.textAlign = 'right';
    ctx.fillText(value.toFixed(3), left - 6, y(value) + 4);
  }
  for (let s = 0; s <= PLOT_SECONDS; s += 5) {
    ctx.textAlign = s === 0 ? 'left' : s === PLOT_SECONDS ? 'right' : 'center';
    ctx.fillText(s === PLOT_SECONDS ? 'now' : `-${PLOT_SECONDS - s} s`,
      left + s / PLOT_SECONDS * plotWidth, height - 6);
  }

  for (const [kind, name] of [['setpoint', '--setpoint'], ['position', '--position']]) {
    const points = series[kind];
    if (points.length < 2) continue;
    ctx.strokeStyle = color(name);
    ctx.lineWidth = 2;
    ctx.beginPath();
    points.forEach(([t, value], i) => {
      // Nothing is published while the paddle holds still; do not bridge those gaps.
      if (i === 0 || t - points[i - 1][0] > GAP_SECONDS) ctx.moveTo(x(t), y(value));
      else ctx.lineTo(x(t), y(value));
    });
    ctx.stroke();
  }
}

$('waveForm').addEventListener('submit', async (event) => {
  event.preventDefault();
  const height = parseFloat($('height').value);
  const period = parseFloat($('period').value);
  $('commandResult').textContent = 'Starting…';
  try {
    const result = await post('api/start', { height, period });
    $('commandResult').textContent = result.message;
  } catch (error) {
    $('commandResult').textContent = 'No answer from the server.';
  }
});

$('stop').addEventListener('click', async () => {
  try {
    const result = await post('api/stop');
    $('commandResult').textContent = result.message;
  } catch (error) {
    $('commandResult').textContent = 'No answer from the server.';
  }
});

$('return').addEventListener('click', async () => {
  $('return').disabled = true;
  $('returnResult').textContent = 'Returning to upright…';
  try {
    const result = await post('api/return');
    $('returnResult').textContent = result.message;
  } catch (error) {
    $('returnResult').textContent = 'No answer from the server.';
  }
  render();
});

render();
setInterval(render, 500);
window.addEventListener('resize', drawPlot);
connect();
