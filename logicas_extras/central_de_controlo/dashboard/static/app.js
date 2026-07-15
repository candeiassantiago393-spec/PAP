const MAX_POINTS = 120;
const GAS_LIMIT = 50;
const FIRE_LIMIT = 50;
const QUAKE_LIMIT = 50;
const SCR_NAMES = ["HOME", "GRAFICOS", "BARRAS", "GAS", "FOGO", "SISMO"];
const SCR_LABELS = {
  0: "Smart Home Lab",
  1: "Gráficos temp/hum",
  2: "Barras ambiente",
  3: "Gás MQ",
  4: "Fogo KY-026",
  5: "Sismo KY-002",
};

const tempData = [];
const humData = [];
const gasData = [];
const fireData = [];
const quakeData = [];
const labels = [];

let chartTemp, chartHum, chartGas, chartFire, chartQuake;

function fmt(v, suffix = "") {
  if (v === null || v === undefined || Number.isNaN(v)) return "—";
  return `${Number(v).toFixed(1)}${suffix}`;
}

function fmtUptime(ms) {
  if (!ms && ms !== 0) return "—";
  const s = Math.floor(Number(ms) / 1000);
  const m = Math.floor(s / 60);
  const h = Math.floor(m / 60);
  if (h > 0) return `${h}h ${m % 60}m`;
  if (m > 0) return `${m}m ${s % 60}s`;
  return `${s}s`;
}

function setBadge(el, text, kind) {
  el.textContent = text;
  el.className = "badge" + (kind ? ` ${kind}` : "");
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

function limitDataset(color) {
  return {
    label: "Limite",
    data: [],
    borderColor: color,
    borderDash: [6, 4],
    borderWidth: 1.5,
    pointRadius: 0,
    fill: false,
    tension: 0,
  };
}

function chartOptions(main, label, yMax, limitLine) {
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
    plugins: { legend: { display: false } },
  };
  if (yMax) opts.scales.y.max = yMax;

  const datasets = [{
    label,
    data: [...main.data],
    borderColor: main.line,
    backgroundColor: main.fill,
    fill: true,
    tension: 0.35,
    pointRadius: 0,
    borderWidth: 2,
  }];

  if (limitLine != null) {
    datasets.push(limitDataset(limitLine));
  }

  return {
    type: "line",
    data: { labels: [...labels], datasets },
    options: opts,
  };
}

function initCharts() {
  chartTemp = new Chart(document.getElementById("chartTemp"), chartOptions(
    { data: tempData, line: "#3d8bfd", fill: "rgba(61,139,253,0.15)" },
    "Temp °C"
  ));
  chartHum = new Chart(document.getElementById("chartHum"), chartOptions(
    { data: humData, line: "#3dba7a", fill: "rgba(61,186,122,0.15)" },
    "Hum %",
    100
  ));
  chartGas = new Chart(document.getElementById("chartGas"), chartOptions(
    { data: gasData, line: "#d4a017", fill: "rgba(212,160,23,0.15)" },
    "Gás %",
    100,
    "rgba(232,93,106,0.55)"
  ));
  chartFire = new Chart(document.getElementById("chartFire"), chartOptions(
    { data: fireData, line: "#ff6b4a", fill: "rgba(255,107,74,0.15)" },
    "Fogo %",
    100,
    "rgba(232,93,106,0.55)"
  ));
  chartQuake = new Chart(document.getElementById("chartQuake"), chartOptions(
    { data: quakeData, line: "#b07aff", fill: "rgba(176,122,255,0.15)" },
    "Sismo %",
    100,
    "rgba(212,160,23,0.55)"
  ));
}

function refreshChart(chart, dataArr, limit) {
  chart.data.labels = [...labels];
  chart.data.datasets[0].data = [...dataArr];
  if (limit != null && chart.data.datasets[1]) {
    chart.data.datasets[1].data = labels.map(() => limit);
  }
  chart.update("none");
}

function refreshCharts() {
  refreshChart(chartTemp, tempData);
  refreshChart(chartHum, humData);
  refreshChart(chartGas, gasData, GAS_LIMIT);
  refreshChart(chartFire, fireData, FIRE_LIMIT);
  refreshChart(chartQuake, quakeData, QUAKE_LIMIT);
}

function envStatus(temp, hum) {
  if (temp == null || hum == null) return { t: "wait", h: "wait", tLabel: "A ler", hLabel: "A ler" };
  const t = Number(temp);
  const h = Number(hum);
  return {
    t: t > 32 ? "warn" : t < 15 ? "warn" : "ok",
    h: h > 75 ? "warn" : h < 25 ? "warn" : "ok",
    tLabel: t > 32 ? "Quente" : t < 15 ? "Frio" : "OK",
    hLabel: h > 75 ? "Alta" : h < 25 ? "Baixa" : "OK",
  };
}

function updateUI(data) {
  const connected = !!data.connected;
  const gasAlarm = !!data.alarm;
  const fireAlarm = !!data.fa;
  const quakeAlarm = !!data.qa;
  const anyAlarm = fireAlarm || quakeAlarm || gasAlarm;

  const dot = document.getElementById("statusDot");
  const statusText = document.getElementById("statusText");
  dot.className = "status-dot" + (connected ? (anyAlarm ? " alarm" : " on") : "");
  statusText.textContent = connected
    ? (anyAlarm ? "ALARME ACTIVO" : "Sistema online")
    : "Mega desligada";

  document.getElementById("oledScreen").textContent = connected
    ? `${SCR_NAMES[data.scr] ?? "?"} — ${SCR_LABELS[data.scr] ?? "—"}`
    : "—";
  document.getElementById("buzzerState").textContent = connected
    ? (anyAlarm ? "🔊 LIGADO" : "Silencioso")
    : "—";
  document.getElementById("buzzerState").className = "sys-value" + (anyAlarm ? " buzzer-on" : "");
  document.getElementById("serialPort").textContent = connected ? (data.port || "COM?") : "—";
  document.getElementById("uptime").textContent = connected ? fmtUptime(data.ms) : "—";

  if (data.ts) {
    const d = new Date(data.ts * 1000);
    document.getElementById("lastUpdate").textContent =
      `Atualizado ${d.toLocaleTimeString("pt-PT")}`;
  }

  document.getElementById("valTemp").innerHTML =
    `${fmt(data.t)}<span class="card-unit"> °C</span>`;
  document.getElementById("valHum").innerHTML =
    `${fmt(data.h)}<span class="card-unit"> %</span>`;
  document.getElementById("valGas").innerHTML =
    `${data.gas ?? 0}<span class="card-unit"> %</span>`;
  document.getElementById("valFire").innerHTML =
    `${data.fire ?? 0}<span class="card-unit"> %</span>`;
  document.getElementById("valQuake").innerHTML =
    `${data.qk ?? 0}<span class="card-unit"> %</span>`;

  const gasPct = Number(data.gas) || 0;
  const firePct = Number(data.fire) || 0;
  const quakePct = Number(data.qk) || 0;
  const humPct = Math.min(Number(data.h) || 0, 100);
  const tempPct = Math.min((Number(data.t) || 0) * 2, 100);

  document.getElementById("barGas").style.width = `${Math.min(gasPct, 100)}%`;
  document.getElementById("barGas").classList.toggle("gas-alarm", gasAlarm);
  document.getElementById("barFire").style.width = `${Math.min(firePct, 100)}%`;
  document.getElementById("barFire").classList.toggle("fire-alarm", fireAlarm);
  document.getElementById("barQuake").style.width = `${Math.min(quakePct, 100)}%`;
  document.getElementById("barQuake").classList.toggle("quake-alarm", quakeAlarm);
  document.getElementById("barHum").style.width = `${humPct}%`;
  document.getElementById("barTemp").style.width = `${tempPct}%`;

  const env = envStatus(data.t, data.h);
  setBadge(document.getElementById("badgeTemp"), env.tLabel, env.t);
  setBadge(document.getElementById("badgeHum"), env.hLabel, env.h);
  setBadge(document.getElementById("badgeGas"), gasAlarm ? "ALARME" : "OK", gasAlarm ? "alarm" : "ok");
  setBadge(document.getElementById("badgeFire"), fireAlarm ? "ALARME" : "OK", fireAlarm ? "alarm" : "ok");
  setBadge(document.getElementById("badgeQuake"), quakeAlarm ? "ALARME" : "OK", quakeAlarm ? "alarm" : "ok");
  setBadge(document.getElementById("badgeAlarm"), anyAlarm ? "BUZZER ON" : "OK", anyAlarm ? "alarm" : "ok");

  document.getElementById("gasCard").classList.toggle("alarm", gasAlarm);
  document.getElementById("fireCard").classList.toggle("alarm", fireAlarm);
  document.getElementById("quakeCard").classList.toggle("quake-alarm", quakeAlarm);
  document.getElementById("alarmCard").classList.toggle("alarm", anyAlarm);

  let alarmText = "Sistema OK — todos os sensores normais";
  if (fireAlarm && quakeAlarm && gasAlarm) alarmText = "⚠ FOGO + SISMO + GÁS — buzzer activo";
  else if (fireAlarm && quakeAlarm) alarmText = "⚠ FOGO + SISMO — buzzer activo";
  else if (fireAlarm && gasAlarm) alarmText = "⚠ FOGO + GÁS — buzzer activo";
  else if (quakeAlarm && gasAlarm) alarmText = "⚠ SISMO + GÁS — buzzer activo";
  else if (fireAlarm) alarmText = "⚠ CHAMA DETETADA — buzzer activo";
  else if (quakeAlarm) alarmText = "⚠ VIBRAÇÃO DETETADA — buzzer activo";
  else if (gasAlarm) alarmText = "⚠ FUGA DE GÁS — buzzer activo";
  document.getElementById("alarmText").textContent = alarmText;

  document.getElementById("metaRaw").textContent =
    `raw ${data.gr ?? "—"} · base ${data.gb ?? "—"} · limite ${GAS_LIMIT}%`;
  document.getElementById("metaFire").textContent =
    `AO ${data.fr ?? "—"} · base ${data.fbl ?? "—"} · drop ${data.fd ?? "—"} · limite ${FIRE_LIMIT}%`;
  document.getElementById("metaQuake").textContent =
    `sinal ${data.qr ?? "—"} · limite ${QUAKE_LIMIT}% · pin A3`;

  if (connected && (data.t != null || data.h != null || data.gas != null || data.fire != null || data.qk != null)) {
    pushLabel();
    pushHistory(tempData, data.t);
    pushHistory(humData, data.h);
    pushHistory(gasData, data.gas);
    pushHistory(fireData, data.fire);
    pushHistory(quakeData, data.qk);
    refreshCharts();
  }
}

function exportCsv() {
  if (labels.length === 0) {
    alert("Sem dados para exportar. Espera a Mega ligar e enviar leituras.");
    return;
  }
  const rows = ["hora,temp_c,hum_pct,gas_pct,fogo_pct,sismo_pct"];
  for (let i = 0; i < labels.length; i++) {
    const t = tempData[i] ?? "";
    const h = humData[i] ?? "";
    const g = gasData[i] ?? "";
    const f = fireData[i] ?? "";
    const q = quakeData[i] ?? "";
    rows.push(`${labels[i]},${t},${h},${g},${f},${q}`);
  }
  const blob = new Blob([rows.join("\n")], { type: "text/csv;charset=utf-8" });
  const url = URL.createObjectURL(blob);
  const a = document.createElement("a");
  const stamp = new Date().toISOString().slice(0, 19).replace(/[:T]/g, "-");
  a.href = url;
  a.download = `smart-home-lab-${stamp}.csv`;
  a.click();
  URL.revokeObjectURL(url);
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
  document.getElementById("btnExportCsv").addEventListener("click", exportCsv);
});
