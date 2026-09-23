// Verifies the PROFIBUS-byte <-> Modbus-register mapping from MAPPING.md.
#include <cassert>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include "mgate/process_image.h"

using namespace mgate;

int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); ++failures; } } while (0)

int main() {
    // Registers exactly as the gateway would return them for the MAPPING.md
    // input area (7 registers, base 0x0000, FC4).
    //   byte0      = DI bits            0x0A  (DI1 and DI3 set)
    //   byte1-2    = AI0 = 13824 (0x3600)  <-- straddles reg0/reg1
    //   byte3-4    = AI1 = -1000 (0xFC18)  <-- straddles reg1/reg2
    //   byte5      = pad                0x00
    //   byte6-7    = ZSW = 0x0F37
    //   byte8-9    = speed = 2500
    //   byte10-11  = current = 123
    //   byte12-13  = torque = 456
    const std::uint16_t regs[7] = {0x0A36, 0x00FC, 0x1800, 0x0F37, 0x09C4, 0x007B, 0x01C8};

    ProcessImage in(14);
    in.load_from_registers(regs, 7, 0);

    // Flattening must reproduce the gateway's byte buffer exactly.
    const std::uint8_t expect[14] = {0x0A, 0x36, 0x00, 0xFC, 0x18, 0x00, 0x0F,
                                     0x37, 0x09, 0xC4, 0x00, 0x7B, 0x01, 0xC8};
    for (int i = 0; i < 14; ++i) CHECK(in.data()[i] == expect[i]);

    // Digital inputs, byte 0.
    CHECK(in.get_bit(0, 0) == false);
    CHECK(in.get_bit(0, 1) == true);
    CHECK(in.get_bit(0, 2) == false);
    CHECK(in.get_bit(0, 3) == true);

    // The two straddling analog words - the whole point of byte addressing.
    CHECK(in.get_i16(1, ByteOrder::BigEndian) == 13824);
    CHECK(in.get_i16(3, ByteOrder::BigEndian) == -1000);

    // Word-aligned drive data.
    CHECK(in.get_u16(6, ByteOrder::BigEndian) == 0x0F37);
    CHECK(in.get_i16(8, ByteOrder::BigEndian) == 2500);
    CHECK(in.get_i16(10, ByteOrder::BigEndian) == 123);
    CHECK(in.get_i16(12, ByteOrder::BigEndian) == 456);

    // Scaling via a Tag, including at the odd offset.
    Tag ai0{"S4.AI0", Area::Input, DataType::I16, 1, 0, 0, ByteOrder::BigEndian, 100.0 / 27648.0, 0.0};
    CHECK(std::fabs(in.get_scaled(ai0) - 50.0) < 1e-9);

    Tag spd{"VFD.ActualSpeed", Area::Input, DataType::I16, 8, 0, 0, ByteOrder::BigEndian, 0.01, 0.0};
    CHECK(std::fabs(in.get_scaled(spd) - 25.0) < 1e-9);

    // Writing at an odd offset must land across the register boundary and
    // survive the trip back out to registers.
    ProcessImage out(14);
    Tag ao0{"S4.AO0", Area::Output, DataType::I16, 1, 0, 0, ByteOrder::BigEndian, 100.0 / 27648.0, 0.0};
    out.set_bit(0, 0, true);
    out.set_scaled(ao0, 50.0);
    std::uint16_t back[7] = {};
    out.store_to_registers(back, 7, 0);
    CHECK(back[0] == 0x0136);  // DO0 set in the high byte, AI high byte in the low
    CHECK(back[1] == 0x0000);
    ProcessImage rt(14);
    rt.load_from_registers(back, 7, 0);
    CHECK(rt.get_i16(1, ByteOrder::BigEndian) == 13824);
    CHECK(rt.get_bit(0, 0) == true);

    // Byte-order variants for a 32-bit value: A=0x11 B=0x22 C=0x33 D=0x44.
    {
        ProcessImage p(4);
        p.set_u32(0, 0x11223344u, ByteOrder::BigEndian);
        CHECK(p.data()[0] == 0x11 && p.data()[1] == 0x22 && p.data()[2] == 0x33 && p.data()[3] == 0x44);

        p.set_u32(0, 0x11223344u, ByteOrder::BigEndianSwap);  // CDAB
        CHECK(p.data()[0] == 0x33 && p.data()[1] == 0x44 && p.data()[2] == 0x11 && p.data()[3] == 0x22);

        p.set_u32(0, 0x11223344u, ByteOrder::LittleEndianSwap);  // BADC
        CHECK(p.data()[0] == 0x22 && p.data()[1] == 0x11 && p.data()[2] == 0x44 && p.data()[3] == 0x33);

        p.set_u32(0, 0x11223344u, ByteOrder::LittleEndian);  // DCBA
        CHECK(p.data()[0] == 0x44 && p.data()[1] == 0x33 && p.data()[2] == 0x22 && p.data()[3] == 0x11);

        // Every ordering must round-trip.
        for (ByteOrder o : {ByteOrder::BigEndian, ByteOrder::LittleEndian, ByteOrder::BigEndianSwap,
                            ByteOrder::LittleEndianSwap}) {
            p.set_u32(0, 0xDEADBEEFu, o);
            CHECK(p.get_u32(0, o) == 0xDEADBEEFu);
            p.set_f32(0, -12.5f, o);
            CHECK(p.get_f32(0, o) == -12.5f);
        }
    }

    // IEEE-754 big-endian float: 3.14159f == 0x40490FD0.
    {
        ProcessImage p(4);
        const std::uint16_t r[2] = {0x4049, 0x0FD0};
        p.load_from_registers(r, 2, 0);
        CHECK(std::fabs(p.get_f32(0, ByteOrder::BigEndian) - 3.14159f) < 1e-5f);
        // Same value delivered word-swapped by a CDAB gateway.
        const std::uint16_t rs[2] = {0x0FD0, 0x4049};
        p.load_from_registers(rs, 2, 0);
        CHECK(std::fabs(p.get_f32(0, ByteOrder::BigEndianSwap) - 3.14159f) < 1e-5f);
    }

    // Bounds checking must throw, not corrupt memory.
    {
        ProcessImage p(4);
        bool threw = false;
        try { p.get_u32(2, ByteOrder::BigEndian); } catch (const std::out_of_range&) { threw = true; }
        CHECK(threw);
    }

    std::printf(failures ? "%d FAILURE(S)\n" : "all mapping tests passed\n", failures);
    return failures ? 1 : 0;
}
