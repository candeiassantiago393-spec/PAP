const MAX_POINTS = 120;
const GAS_LIMIT = 50;
const SCR_NAMES = ["HOME", "GRAFICOS", "BARRAS", "GAS"];

const tempData = [];
const humData = [];
const gasData = [];
const labels = [];

let chartTemp, chartHum, chartGas;

function fmt(v, suffix = "") {
  if (v === null || v === undefined || Number.isNaN(v)) return "—";
  return `${Number(v).toFixed(1)}${suffix}`;
}

function pushHistory(arr, val) {
  if (val === null || val === undefined) return;
  arr.push(Number(val));
  if (arr.length > MAX_POINTS) arr.shift();
}

function pushLabel() {
  const d = new Date();
  labels.push(d.toLocaleTimeString("pt-PT"));
  if (labels.length > MAX_POINTS) labels.shift();
}

function chartOptions(color, label, yMax) {
  const opts = {
    responsive: true,
    maintainAspectRatio: false,
    animation: { duration: 300 },
    scales: {
      x: {
        ticks: { color: "#6b7385", maxTicksLimit: 6 },
        grid: { color: "rgba(42,48,64,0.5)" },
      },
      y: {
        min: 0,
        ticks: { color: "#6b7385" },
        grid: { color: "rgba(42,48,64,0.5)" },
      },
    },
    plugins: {
      legend: { display: false },
    },
  };
  if (yMax) opts.scales.y.max = yMax;
  return {
    type: "line",
    data: {
      labels: [...labels],
      datasets: [{
        label,
        data: [...color.data],
        borderColor: color.line,
        backgroundColor: color.fill,
        fill: true,
        tension: 0.35,
        pointRadius: 0,
        borderWidth: 2,
      }],
    },
    options: opts,
  };
}

function initCharts() {
  const ctxT = document.getElementById("chartTemp");
  const ctxH = document.getElementById("chartHum");
  const ctxG = document.getElementById("chartGas");

  chartTemp = new Chart(ctxT, chartOptions(
    { data: tempData, line: "#3d8bfd", fill: "rgba(61,139,253,0.15)" },
    "Temp °C"
  ));
  chartHum = new Chart(ctxH, chartOptions(
    { data: humData, line: "#3dba7a", fill: "rgba(61,186,122,0.15)" },
    "Hum %",
    100
  ));
  chartGas = new Chart(ctxG, chartOptions(
    { data: gasData, line: "#d4a017", fill: "rgba(212,160,23,0.15)" },
    "Gás %",
    100
  ));
}

function refreshCharts() {
  chartTemp.data.labels = [...labels];
  chartTemp.data.datasets[0].data = [...tempData];
  chartTemp.update("none");

  chartHum.data.labels = [...labels];
  chartHum.data.datasets[0].data = [...humData];
  chartHum.update("none");

  chartGas.data.labels = [...labels];
  chartGas.data.datasets[0].data = [...gasData];
  chartGas.update("none");
}

function updateUI(data) {
  const connected = !!data.connected;
  const alarm = !!data.alarm;

  const dot = document.getElementById("statusDot");
  const statusText = document.getElementById("statusText");
  dot.className = "status-dot" + (connected ? (alarm ? " alarm" : " on") : "");
  statusText.textContent = connected
    ? `${data.port || "COM"} · ${SCR_NAMES[data.scr] || "—"}`
    : "Mega desligada";

  document.getElementById("valTemp").innerHTML =
    `${fmt(data.t)}<span class="card-unit"> °C</span>`;
  document.getElementById("valHum").innerHTML =
    `${fmt(data.h)}<span class="card-unit"> %</span>`;
  document.getElementById("valGas").innerHTML =
    `${data.gas ?? 0}<span class="card-unit"> %</span>`;

  const gasPct = Number(data.gas) || 0;
  document.getElementById("barGas").style.width = `${Math.min(gasPct, 100)}%`;
  document.getElementById("barGas").classList.toggle("gas-alarm", alarm);

  const humPct = Math.min(Number(data.h) || 0, 100);
  document.getElementById("barHum").style.width = `${humPct}%`;

  const tempPct = Math.min((Number(data.t) || 0) * 2, 100);
  document.getElementById("barTemp").style.width = `${tempPct}%`;

  const alarmCard = document.getElementById("alarmCard");
  alarmCard.classList.toggle("alarm", alarm);
  document.getElementById("alarmText").textContent = alarm
    ? "FUGA DE GÁS — BUZZER ON"
    : "Sistema OK";

  document.getElementById("metaRaw").textContent =
    `raw=${data.gr ?? "—"} base=${data.gb ?? "—"} limite=${GAS_LIMIT}%`;

  if (connected && (data.t != null || data.h != null || data.gas != null)) {
    pushLabel();
    pushHistory(tempData, data.t);
    pushHistory(humData, data.h);
    pushHistory(gasData, data.gas);
    refreshCharts();
  }
}

function connectWs() {
  const proto = location.protocol === "https:" ? "wss" : "ws";
  const ws = new WebSocket(`${proto}://${location.host}/ws`);

  ws.onmessage = (ev) => {
    try {
      updateUI(JSON.parse(ev.data));
    } catch (_) {}
  };

  ws.onclose = () => setTimeout(connectWs, 1500);
}

document.addEventListener("DOMContentLoaded", () => {
  initCharts();
  connectWs();
});
