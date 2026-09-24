"""probe_map.py - find the MGate's Modbus register areas by asking it.

READ ONLY: uses FC3 (read holding registers) and FC4 (read input registers)
and nothing else. Never writes.

For each function code it walks the 0..0xFFFF address space in blocks,
records which ranges the gateway accepts (vs. exception 02 "illegal data
address"), then dumps the start of each accepted range.

    python tests/probe_map.py [host] [port] [unit_id]
"""
import socket
import struct
import sys

HOST = sys.argv[1] if len(sys.argv) > 1 else "192.168.127.254"
PORT = int(sys.argv[2]) if len(sys.argv) > 2 else 502
UNIT = int(sys.argv[3]) if len(sys.argv) > 3 else 1

_tid = 0


def recv_exact(s, n):
    buf = b""
    while len(buf) < n:
        chunk = s.recv(n - len(buf))
        if not chunk:
            raise ConnectionError("gateway closed the connection")
        buf += chunk
    return buf


def read(s, fc, addr, count):
    """Return list of register values, or an int exception code."""
    global _tid
    _tid = (_tid + 1) & 0xFFFF
    pdu = struct.pack(">BHH", fc, addr, count)
    s.sendall(struct.pack(">HHHB", _tid, 0, len(pdu) + 1, UNIT) + pdu)
    _, _, length, _ = struct.unpack(">HHHB", recv_exact(s, 7))
    body = recv_exact(s, length - 1)
    if body[0] & 0x80:
        return body[1]
    return list(struct.unpack(">%dH" % (body[1] // 2), body[2:2 + body[1]]))


def find_ranges(s, fc, block=64):
    """Coarse scan in blocks, then refine each valid block's edges word by word."""
    valid = []
    for start in range(0, 0x10000, block):
        cnt = min(block, 0x10000 - start)
        r = read(s, fc, start, cnt)
        if isinstance(r, list):
            valid.append((start, start + cnt))
        else:
            # Partially valid block? Check single registers.
            for a in range(start, start + cnt):
                if isinstance(read(s, fc, a, 1), list):
                    valid.append((a, a + 1))
    merged = []
    for a, b in valid:
        if merged and merged[-1][1] == a:
            merged[-1] = (merged[-1][0], b)
        else:
            merged.append((a, b))
    return merged


def main():
    print(f"Probing {HOST}:{PORT} unit {UNIT} (read only)\n")
    with socket.create_connection((HOST, PORT), timeout=3) as s:
        for fc, name in ((4, "FC4 input registers"), (3, "FC3 holding registers")):
            ranges = find_ranges(s, fc)
            print(f"== {name}: {len(ranges)} valid range(s)")
            for a, b in ranges:
                print(f"   0x{a:04X} .. 0x{b - 1:04X}  ({b - a} registers)")
                vals = read(s, fc, a, min(16, b - a))
                if isinstance(vals, list):
                    print("     first words: " + " ".join(f"{v:04X}" for v in vals))
            print()


if __name__ == "__main__":
    main()
