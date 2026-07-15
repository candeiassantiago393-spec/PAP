"""Central de Controlo — dashboard local (Serial Mega → WebSocket → browser)."""

from __future__ import annotations

import asyncio
import json
import os
import re
import threading
import time
from pathlib import Path
from typing import Any

import serial
import serial.tools.list_ports
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import FileResponse
from fastapi.staticfiles import StaticFiles

ROOT = Path(__file__).resolve().parent
STATIC = ROOT / "static"
BAUD = 9600
HOST = "127.0.0.1"
PORT = int(os.environ.get("CENTRAL_DASH_PORT", "8765"))
COM_PORT = os.environ.get("CENTRAL_COM", "COM7")

clients: set[WebSocket] = set()
latest: dict[str, Any] = {
    "connected": False,
    "port": None,
    "t": None,
    "h": None,
    "gas": 0,
    "gr": 0,
    "gb": 0,
    "alarm": 0,
    "fire": 0,
    "fr": 0,
    "fbl": 0,
    "fd": 0,
    "fdo": 0,
    "fa": 0,
    "fw": None,
    "qk": 0,
    "qr": 0,
    "qa": 0,
    "scr": 0,
    "ms": 0,
    "ts": None,
}
lock = threading.Lock()

JSON_LINE = re.compile(r"^\s*\{")


def find_mega_port(preferred: str) -> str | None:
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        if p.device.upper() == preferred.upper():
            return p.device
    for p in ports:
        desc = (p.description or "").lower()
        if "mega" in desc or "arduino" in desc:
            return p.device
    return ports[0].device if ports else None


def parse_line(line: str) -> dict[str, Any] | None:
    line = line.strip()
    if not line or not JSON_LINE.match(line):
        return None
    try:
        data = json.loads(line)
    except json.JSONDecodeError:
        return None
    if not isinstance(data, dict):
        return None
    return data


def serial_reader(port_name: str) -> None:
    global latest
    ser = None
    while True:
        try:
            if ser is None or not ser.is_open:
                ser = serial.Serial(port_name, BAUD, timeout=1)
                with lock:
                    latest["connected"] = True
                    latest["port"] = port_name
            raw = ser.readline()
            if not raw:
                continue
            try:
                text = raw.decode("utf-8", errors="ignore")
            except UnicodeDecodeError:
                continue
            payload = parse_line(text)
            if payload is None:
                continue
            with lock:
                latest.update(payload)
                latest["connected"] = True
                latest["port"] = port_name
                latest["ts"] = time.time()
        except (serial.SerialException, OSError):
            with lock:
                latest["connected"] = False
            if ser is not None:
                try:
                    ser.close()
                except OSError:
                    pass
                ser = None
            time.sleep(2)
        except Exception:
            time.sleep(0.5)


app = FastAPI(title="Central de Controlo Dashboard")
app.mount("/static", StaticFiles(directory=STATIC), name="static")


@app.get("/")
async def index() -> FileResponse:
    return FileResponse(
        STATIC / "index.html",
        headers={"Cache-Control": "no-store"},
    )


@app.get("/api/state")
async def api_state() -> dict[str, Any]:
    with lock:
        return dict(latest)


@app.websocket("/ws")
async def ws_endpoint(websocket: WebSocket) -> None:
    await websocket.accept()
    clients.add(websocket)
    try:
        while True:
            with lock:
                payload = dict(latest)
            await websocket.send_json(payload)
            await asyncio.sleep(0.35)
    except WebSocketDisconnect:
        pass
    finally:
        clients.discard(websocket)


def main() -> None:
    port = find_mega_port(COM_PORT)
    if not port:
        print("ERRO: nenhuma porta serial encontrada. Liga a Mega e tenta outra vez.")
        print(f"  Ou define: $env:CENTRAL_COM='COM7'")
        raise SystemExit(1)

    print(f"Dashboard: http://{HOST}:{PORT}")
    print(f"Serial:    {port} @ {BAUD} baud")
    print("Fecha o Serial Monitor antes de iniciar.")

    thread = threading.Thread(target=serial_reader, args=(port,), daemon=True)
    thread.start()

    import uvicorn

    uvicorn.run(app, host=HOST, port=PORT, log_level="warning")


if __name__ == "__main__":
    main()
