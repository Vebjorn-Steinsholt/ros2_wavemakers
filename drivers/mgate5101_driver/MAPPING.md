# Mapping PROFIBUS variables into Modbus registers (MGate 5101-PBM-MN)

## 1. Who is master of what

```
  your PC                 MGate 5101-PBM-MN                 field devices
 ┌──────────┐            ┌────────────────────┐            ┌─────────────┐
 │ Modbus   │  Ethernet  │ Modbus TCP SERVER  │            │ PROFIBUS DP │
 │ TCP      │───────────▶│        +           │───────────▶│ slave  #4   │
 │ CLIENT   │  port 502  │ internal I/O memory│  PROFIBUS  │ slave  #6   │
 │ (driver) │◀───────────│        +           │◀───────────│     ...     │
 └──────────┘            │ PROFIBUS DP MASTER │            └─────────────┘
                         └────────────────────┘
```

Two consequences that trip people up:

- Your PC is the **Modbus client/master**. The gateway is the **Modbus slave**. You poll it.
- The GSD files are **the slaves'**, not the gateway's. The MGate is a PROFIBUS *master*, so
  you import one GSD per field device into MGate Manager / the web console. Those GSD files
  are what decide, byte for byte, how much I/O each device contributes.

The gateway runs the PROFIBUS cycle on its own, continuously, whether or not you are
connected. Its internal memory is just a mailbox between the two cycles. Nothing you do over
Modbus is synchronous with the PROFIBUS bus cycle.

## 2. The two data areas

| Area | Direction | Modbus access | Typical function codes |
|---|---|---|---|
| **Input data** | PROFIBUS slave → gateway → PC | read only | FC4 (input registers), usually also mirrored to FC3 |
| **Output data** | PC → gateway → PROFIBUS slave | read/write | FC3 to read back, FC6/FC16 to write |

Plus a **status/diagnostic** area (slave live list, per-slave comm status, gateway state).
Useful, but its address is model- and firmware-specific — read it off the address map page.

> **Get the base addresses from the device, do not guess them.**
> Web console → *Protocol Settings → Modbus TCP → Modbus address map*
> (MGate Manager shows the same table). It prints, per slave and per module, the internal
> byte offset and the Modbus address the gateway assigned. That table is the ground truth
> and it changes whenever you add, remove, or reorder a slave or a module.

> **Off-by-one warning.** Documentation writes holding registers as `4xxxxx` and input
> registers as `3xxxxx`, 1-based. On the wire they are 0-based. `40001` → address `0x0000`.
> `30001` → address `0x0000`. Subtract one before putting it in `GatewayConfig`.

## 3. How the GSD decides the allocation

For each PROFIBUS slave, the gateway walks the configured modules **in slot order** and
appends each module's input bytes to the input area and its output bytes to the output area.

In a GSD file, each module is a line like:

```
Module = "2 AI x 16 Bit"  0x51, 0x51      ; two identifier bytes
EndModule
```

The identifier bytes encode length and direction (DP-V0 "compact" format, bits 4-5:
`01` = input, `10` = output, `11` = both; bit 6 = word vs byte; low nibble = length − 1).
You rarely need to decode this by hand — MGate Manager shows the resulting byte count next
to each module you place in a slot. Use that number.

Rules that matter:

1. Allocation is in **bytes**, back to back, in slot order. There is no per-module padding.
2. An input-only module consumes nothing in the output area, and vice versa.
3. Most gateways start each **slave** on an even (word) boundary, inserting a pad byte if the
   previous slave ended on an odd count. **Verify this on the address map page** — if your
   firmware packs slaves tightly instead, every offset after the first odd-length slave shifts.
4. Modules therefore routinely land on odd byte offsets, so analog words straddle Modbus
   register boundaries. This is normal and is why the driver addresses bytes, not registers.

## 4. The formula

With `byte_offset` taken from the address map and `base` the area's first register:

```
register        = base + (byte_offset / 2)          // integer division
byte in register= (byte_offset % 2 == 0) ? HIGH : LOW
```

A 16-bit PROFIBUS value at an **odd** offset spans two registers: low half of
`base + offset/2` and high half of `base + offset/2 + 1`.

The driver never does this arithmetic per tag. It reads the whole area, writes each register
back out high-byte-first into a flat `std::vector<uint8_t>`, and that buffer is a byte-exact
copy of the gateway's internal memory. Tags then index it by PROFIBUS byte offset directly.

## 5. Worked example

PROFIBUS configuration:

| Slave | Device | Slot | GSD module | In bytes | Out bytes |
|---|---|---|---|---|---|
| 4 | ET 200S | 1 | 4 DI | 1 | – |
| 4 | | 2 | 4 DO | – | 1 |
| 4 | | 3 | 2 AI × 16 bit | 4 | – |
| 4 | | 4 | 2 AO × 16 bit | – | 4 |
| 6 | VFD | 1 | PPO type 3 | 8 | 8 |

Slave 4 contributes 5 input bytes (offsets 0–4) and 5 output bytes (offsets 0–4). Slave 6 is
word aligned, so it starts at offset **6**; offset 5 is a pad byte. Totals: 14 bytes in,
14 bytes out = **7 registers each**.

### Input area — base `0x0000`, read with FC4

| Byte | Register | Half | Variable | Type |
|---|---|---|---|---|
| 0 | 0x0000 | high | S4 digital inputs, bits 0–3 | 4 × BOOL |
| 1–2 | 0x0000 lo + 0x0001 hi | **straddles** | S4.AI0 | INT16 |
| 3–4 | 0x0001 lo + 0x0002 hi | **straddles** | S4.AI1 | INT16 |
| 5 | 0x0002 | low | padding | – |
| 6–7 | 0x0003 | both | VFD status word (ZSW) | UINT16 |
| 8–9 | 0x0004 | both | VFD actual speed | INT16 |
| 10–11 | 0x0005 | both | VFD actual current | INT16 |
| 12–13 | 0x0006 | both | VFD actual torque | INT16 |

### Output area — base `0x0800`, written with FC16

| Byte | Register | Half | Variable | Type |
|---|---|---|---|---|
| 0 | 0x0800 | high | S4 digital outputs, bits 0–3 | 4 × BOOL |
| 1–2 | 0x0800 lo + 0x0801 hi | **straddles** | S4.AO0 | INT16 |
| 3–4 | 0x0801 lo + 0x0802 hi | **straddles** | S4.AO1 | INT16 |
| 5 | 0x0802 | low | padding | – |
| 6–7 | 0x0803 | both | VFD control word (STW) | UINT16 |
| 8–9 | 0x0804 | both | VFD speed setpoint | INT16 |
| 10–11 | 0x0805 | both | VFD ramp up | UINT16 |
| 12–13 | 0x0806 | both | VFD ramp down | UINT16 |

Note the two straddling analog words. Register-indexed code gets these wrong every time;
byte-indexed code does not have to care. Expressing the same table in the driver:

```cpp
cfg.input_base_reg    = 0x0000;  cfg.input_word_count  = 7;
cfg.output_base_reg   = 0x0800;  cfg.output_word_count = 7;

drv.add_tags({
    {"S4.DI0", Area::Input,  DataType::Bit, 0, 0},
    {"S4.AI0", Area::Input,  DataType::I16, 1},   // odd offset, handled
    {"VFD.StatusWord",   Area::Input,  DataType::U16, 6},
    {"VFD.ControlWord",  Area::Output, DataType::U16, 6},
    {"VFD.SpeedSetpoint",Area::Output, DataType::I16, 8, 0, 0, ByteOrder::BigEndian, 0.01, 0.0, "Hz"},
});
```

## 6. Byte order

PROFIBUS is **big-endian**. Modbus transmits each register big-endian. So a 16-bit PROFIBUS
value arrives in the correct order with no swapping: `ByteOrder::BigEndian` is the right
default and you should not need anything else.

Where it does bite:

- **32-bit values (DINT, REAL)** span two registers, and the *register* order is a convention,
  not a standard. A Siemens REAL on PROFIBUS is ABCD; many gateways and SCADA tools default
  to CDAB. If a float reads as garbage but its magnitude looks vaguely plausible, try
  `ByteOrder::BigEndianSwap`.
- The MGate has its own **swap setting** on the Modbus side (byte swap / word swap). If it is
  enabled there, disable it and do the ordering in the driver instead, or you will be
  compensating twice. Pick one place.

The four orderings in `ByteOrder`, for bytes `A B C D` as stored by the PROFIBUS device:

| Enum | Wire result | Use for |
|---|---|---|
| `BigEndian` | A B C D | default; correct for 16-bit, and 32-bit from Siemens |
| `BigEndianSwap` | C D A B | 32-bit where the gateway/tool swapped register order |
| `LittleEndianSwap` | B A D C | byte swap within registers |
| `LittleEndian` | D C B A | full reverse |

## 7. Checklist before you trust the numbers

- [ ] Address map printed from the gateway, not from this document.
- [ ] `4xxxx`/`3xxxx` converted to 0-based wire addresses.
- [ ] Modbus unit ID matches what is configured on the gateway (default 1).
- [ ] Confirmed whether the input area is on FC4, FC3, or both.
- [ ] Confirmed the gateway's byte/word swap setting, and that you are not also swapping in code.
- [ ] Confirmed whether slaves are word aligned; found the pad bytes.
- [ ] Re-exported the map after any change to the PROFIBUS configuration — offsets move.
- [ ] Checked the gateway's max I/O size against your total. Adding one slave can push you over.
- [ ] Decided what the gateway should drive onto PROFIBUS when Modbus goes quiet. The MGate has
      a configurable fault action (hold last value / clear to zero). Default behaviour on a
      dropped TCP connection is a safety decision, not a networking one — set it deliberately.
- [ ] Tested with a single known tag first: toggle one DO, watch one DI. Then trust the rest.

## 8. Gotchas in the driver itself

- Writes are **staged**, not immediate. `write()` updates the local output image and marks the
  byte range dirty; the poll thread sends it on the next cycle (≤ `poll_interval_ms` later).
- After a reconnect the whole output area is re-sent, because the gateway may have applied its
  fault action while you were away.
- `online()` reflects the **Modbus** link only. A healthy Modbus connection tells you nothing
  about whether a PROFIBUS slave is still exchanging data — read the gateway's status area for
  that, and treat a missing slave as stale data rather than good data.
- `poll_interval_ms = 50` is a reasonable start. Going below the PROFIBUS cycle time only
  re-reads the same values; check the actual bus cycle in the gateway's diagnostics.
