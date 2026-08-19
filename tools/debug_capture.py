#!/usr/bin/env python3
"""Capture agent NDJSON logs from Serial into debug-57affc.log."""

from __future__ import annotations

import json
import re
import sys
import time
from pathlib import Path

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    print("Need pyserial: pip install pyserial", file=sys.stderr)
    sys.exit(1)

LOG_PATH = Path(__file__).resolve().parent.parent / "debug-57affc.log"
INFO_PREFIX = "[INFO] "


def normalize_port(raw: str) -> str:
    value = raw.strip()
    if re.fullmatch(r"\d+", value):
        return f"COM{value}"
    if value.lower().startswith("com") and value[3:].isdigit():
        return f"COM{value[3:]}"
    return value


def list_serial_ports() -> list[str]:
    return [p.device for p in list_ports.comports()]


def resolve_port(raw: str | None) -> str:
    if raw:
        return normalize_port(raw)

    ports = list_serial_ports()
    if len(ports) == 1:
        print(f"Auto-selected port: {ports[0]}")
        return ports[0]

    if ports:
        print("Available ports:", ", ".join(ports))
        return ports[0]

    return "COM4"


def main() -> int:
    port = resolve_port(sys.argv[1] if len(sys.argv) > 1 else None)
    duration = float(sys.argv[2]) if len(sys.argv) > 2 else 20.0

    LOG_PATH.parent.mkdir(parents=True, exist_ok=True)
    deadline = time.time() + duration
    captured = 0

    try:
        ser = serial.Serial(port, 115200, timeout=1)
    except serial.SerialException as exc:
        ports = list_serial_ports()
        print(f"Cannot open port {port!r}: {exc}", file=sys.stderr)
        if ports:
            print("Available ports:", ", ".join(ports), file=sys.stderr)
            print(f"Example: python tools/debug_capture.py {ports[0]} 20", file=sys.stderr)
        else:
            print("No serial ports found. Check USB cable and drivers.", file=sys.stderr)
        return 1

    with ser, LOG_PATH.open("a", encoding="utf-8") as out:
        print(f"Capturing from {port} for {duration:.0f}s -> {LOG_PATH}")
        while time.time() < deadline:
            raw = ser.readline()
            if not raw:
                continue
            try:
                line = raw.decode("utf-8", errors="ignore").strip()
            except UnicodeDecodeError:
                continue
            if not line.startswith(INFO_PREFIX):
                continue
            payload = line[len(INFO_PREFIX):]
            if "\"sessionId\":\"57affc\"" not in payload:
                continue
            try:
                json.loads(payload)
            except json.JSONDecodeError:
                continue
            out.write(payload + "\n")
            out.flush()
            captured += 1
            print(payload)

    print(f"Captured {captured} log lines")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
