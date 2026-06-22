#!/usr/bin/env python3
"""
Gráfico em tempo real — 4 sensores Hall (PAP elevador).
Lê CSV serial p1,p2,p3,p4 (0–100) do Arduino Mega.

Uso:
  python plot_hall.py
  python plot_hall.py COM5
  python plot_hall.py --list
"""

from __future__ import annotations

import argparse
import re
import sys
import time
from collections import deque

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import serial.tools.list_ports

BAUD = 115200
HISTORY_SEC = 20.0
INTERVAL_MS = 50
MAX_POINTS = int(HISTORY_SEC * 1000 / INTERVAL_MS) + 10
MAG_THRESHOLD = 55

SENSORS = [
    {"name": "Piso 1", "pin": "D8", "color": "#E63946"},
    {"name": "Piso 2", "pin": "D11", "color": "#1D3557"},
    {"name": "Piso 3", "pin": "D12", "color": "#2A9D8F"},
    {"name": "Piso 4", "pin": "D13", "color": "#E9C46A"},
]

LINE_RE = re.compile(r"^(\d{1,3}),(\d{1,3}),(\d{1,3}),(\d{1,3})\s*$")


def list_ports() -> None:
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("Nenhuma porta COM encontrada.")
        return
    for p in ports:
        print(f"  {p.device}\t{p.description}")


def guess_port() -> str | None:
    keywords = ("arduino", "mega", "ch340", "wch", "usb-serial", "2341")
    for p in serial.tools.list_ports.comports():
        blob = f"{p.description} {p.manufacturer or ''}".lower()
        if any(k in blob for k in keywords):
            return p.device
    return serial.tools.list_ports.comports()[0].device if serial.tools.list_ports.comports() else None


def parse_line(line: str) -> list[int] | None:
    m = LINE_RE.match(line.strip())
    if not m:
        return None
    vals = [min(100, max(0, int(g))) for g in m.groups()]
    return vals


def open_serial(port: str) -> serial.Serial:
    ser = serial.Serial(port, BAUD, timeout=0.05)
    time.sleep(2.0)
    ser.reset_input_buffer()
    return ser


def main() -> int:
    parser = argparse.ArgumentParser(description="Gráfico Hall ×4 — PAP")
    parser.add_argument("port", nargs="?", help="Porta COM (ex: COM5)")
    parser.add_argument("--list", action="store_true", help="Listar portas COM")
    args = parser.parse_args()

    if args.list:
        list_ports()
        return 0

    port = args.port or guess_port()
    if not port:
        print("Erro: indica a porta — python plot_hall.py COM5")
        list_ports()
        return 1

    print(f"Ligado a {port} @ {BAUD} baud. Fecha a janela do gráfico para sair.")

    try:
        ser = open_serial(port)
    except serial.SerialException as exc:
        print(f"Erro ao abrir {port}: {exc}")
        return 1

    history: list[deque[float]] = [deque(maxlen=MAX_POINTS) for _ in range(4)]
    times: deque[float] = deque(maxlen=MAX_POINTS)
    t0 = time.monotonic()
    latest = [0.0, 0.0, 0.0, 0.0]

    plt.style.use("seaborn-v0_8-whitegrid")
    fig, ax = plt.subplots(figsize=(11, 6), facecolor="#FAFAFA")
    fig.canvas.manager.set_window_title("PAP — Sensores Hall ×4")

    lines = []
    fills = []
    for s in SENSORS:
        (ln,) = ax.plot([], [], color=s["color"], linewidth=2.2, label=s["name"], zorder=3)
        fl = ax.fill_between([], [], alpha=0.22, color=s["color"], zorder=2)
        lines.append(ln)
        fills.append(fl)

    ax.set_xlim(0, HISTORY_SEC)
    ax.set_ylim(-2, 108)
    ax.set_xlabel("Tempo (s)", fontsize=11)
    ax.set_ylabel("Nível detetado (%)", fontsize=11)
    ax.set_title("Sensores Hall — bancada PAP", fontsize=14, fontweight="bold", pad=12)
    ax.axhline(MAG_THRESHOLD, color="#888888", linestyle="--", linewidth=1, alpha=0.7, label="Limiar ~55%")
    ax.legend(loc="upper right", framealpha=0.95, fontsize=10)
    ax.set_yticks([0, 25, 50, 75, 100])

    status_text = ax.text(
        0.01,
        0.02,
        "",
        transform=ax.transAxes,
        fontsize=10,
        family="monospace",
        verticalalignment="bottom",
        bbox=dict(boxstyle="round,pad=0.35", facecolor="white", edgecolor="#CCCCCC", alpha=0.92),
    )

    def animate(_frame: int):
        nonlocal fills
        while ser.in_waiting:
            raw = ser.readline().decode("utf-8", errors="ignore")
            vals = parse_line(raw)
            if vals is None:
                continue
            t = time.monotonic() - t0
            times.append(t)
            for i, v in enumerate(vals):
                latest[i] = float(v)
                history[i].append(latest[i])

        if not times:
            return lines

        t_list = list(times)
        t_min = max(0, t_list[-1] - HISTORY_SEC)
        ax.set_xlim(t_min, t_min + HISTORY_SEC)

        for i, ln in enumerate(lines):
            y = list(history[i])
            ln.set_data(t_list[-len(y) :], y)
            mag = latest[i] >= MAG_THRESHOLD
            ln.set_linewidth(3.6 if mag else 2.0)
            ln.set_alpha(1.0 if mag else 0.75)

        for i, fl in enumerate(fills):
            fl.remove()
        t_plot = t_list[-len(history[0]) :]
        for i in range(4):
            y = list(history[i])
            if len(y) < 2:
                continue
            fills[i] = ax.fill_between(
                t_plot,
                y,
                0,
                alpha=0.35 if latest[i] >= MAG_THRESHOLD else 0.08,
                color=SENSORS[i]["color"],
                zorder=1,
            )

        parts = []
        for i, s in enumerate(SENSORS):
            v = latest[i]
            tag = "MAG" if v >= MAG_THRESHOLD else "—"
            parts.append(f"{s['name']} ({s['pin']}): {v:3.0f}% {tag}")
        status_text.set_text("  |  ".join(parts))

        return lines

    ani = animation.FuncAnimation(fig, animate, interval=INTERVAL_MS, cache_frame_data=False)

    try:
        plt.tight_layout()
        plt.show()
    finally:
        ser.close()

    return 0


if __name__ == "__main__":
    sys.exit(main())
