"""Minimal Modbus/TCP server standing in for an MGate 5101, to exercise the C++ driver.

Input registers  (FC4) at 0x0000: the MAPPING.md example input area, 7 regs.
Holding registers(FC3/6/16) at 0x0800: the output area, 7 regs.
Prints whatever the driver writes so we can check the encoding.
"""
import socket
import struct
import sys
import threading

INPUT_BASE, INPUT_N = 0x0000, 7
OUTPUT_BASE, OUTPUT_N = 0x0800, 7

input_regs = [0x0A36, 0x00FC, 0x1800, 0x0F37, 0x09C4, 0x007B, 0x01C8]
holding = {OUTPUT_BASE + i: 0 for i in range(OUTPUT_N)}
writes_seen = []


def handle(conn):
    buf = b""
    while True:
        chunk = conn.recv(4096)
        if not chunk:
            return
        buf += chunk
        while len(buf) >= 6:
            tid, pid, ln = struct.unpack(">HHH", buf[:6])
            if len(buf) < 6 + ln:
                break
            frame, buf = buf[: 6 + ln], buf[6 + ln :]
            unit = frame[6]
            pdu = frame[7:]
            fc = pdu[0]
            try:
                if fc in (3, 4):
                    addr, cnt = struct.unpack(">HH", pdu[1:5])
                    if fc == 4:
                        if addr < INPUT_BASE or addr + cnt > INPUT_BASE + INPUT_N:
                            raise IndexError
                        vals = input_regs[addr - INPUT_BASE : addr - INPUT_BASE + cnt]
                    else:
                        vals = [holding.get(addr + i, 0) for i in range(cnt)]
                    body = bytes([fc, cnt * 2]) + b"".join(struct.pack(">H", v) for v in vals)
                elif fc == 6:
                    addr, val = struct.unpack(">HH", pdu[1:5])
                    holding[addr] = val
                    writes_seen.append((addr, [val]))
                    body = pdu[:5]
                elif fc == 16:
                    addr, cnt, nb = struct.unpack(">HHB", pdu[1:6])
                    vals = list(struct.unpack(">" + "H" * cnt, pdu[6 : 6 + nb]))
                    for i, v in enumerate(vals):
                        holding[addr + i] = v
                    writes_seen.append((addr, vals))
                    print("FC16 @0x%04X: %s" % (addr, " ".join("%04X" % v for v in vals)), flush=True)
                    body = struct.pack(">BHH", 16, addr, cnt)
                else:
                    body = bytes([fc | 0x80, 0x01])
            except Exception:
                body = bytes([fc | 0x80, 0x02])
            conn.sendall(struct.pack(">HHH", tid, 0, len(body) + 1) + bytes([unit]) + body)


def main():
    srv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind(("127.0.0.1", 1502))
    srv.listen(4)
    print("stub listening on 127.0.0.1:1502", flush=True)

    def serve():
        while True:
            conn, _ = srv.accept()
            threading.Thread(target=handle, args=(conn,), daemon=True).start()

    threading.Thread(target=serve, daemon=True).start()
    try:
        threading.Event().wait(20)
    finally:
        print("writes received:", flush=True)
        for addr, vals in writes_seen:
            print("  0x%04X: %s" % (addr, " ".join("%04X" % v for v in vals)), flush=True)


if __name__ == "__main__":
    main()
