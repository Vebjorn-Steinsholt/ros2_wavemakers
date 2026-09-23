#include "mgate/process_image.h"

#include <cmath>
#include <cstring>
#include <stdexcept>

namespace mgate {
namespace {

// Converts between wire order and normalised big-endian order. All four
// orderings are their own inverse, so one function serves both directions.
void reorder(const std::uint8_t* src, std::uint8_t* dst, std::size_t n, ByteOrder o) {
    const std::size_t nreg = n / 2;
    switch (o) {
        case ByteOrder::BigEndian:
            std::memcpy(dst, src, n);
            break;
        case ByteOrder::LittleEndian:
            for (std::size_t i = 0; i < n; ++i) dst[i] = src[n - 1 - i];
            break;
        case ByteOrder::BigEndianSwap:
            for (std::size_t r = 0; r < nreg; ++r) {
                const std::size_t s = 2 * (nreg - 1 - r);
                dst[2 * r]     = src[s];
                dst[2 * r + 1] = src[s + 1];
            }
            break;
        case ByteOrder::LittleEndianSwap:
            for (std::size_t r = 0; r < nreg; ++r) {
                dst[2 * r]     = src[2 * r + 1];
                dst[2 * r + 1] = src[2 * r];
            }
            break;
    }
}

std::uint64_t be_to_u64(const std::uint8_t* p, std::size_t n) {
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < n; ++i) v = (v << 8) | p[i];
    return v;
}

void u64_to_be(std::uint64_t v, std::uint8_t* p, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) p[n - 1 - i] = static_cast<std::uint8_t>(v >> (8 * i));
}

}  // namespace

std::size_t type_size(DataType t, std::size_t raw_len) {
    switch (t) {
        case DataType::Bit:
        case DataType::U8:
        case DataType::I8:  return 1;
        case DataType::U16:
        case DataType::I16: return 2;
        case DataType::U32:
        case DataType::I32:
        case DataType::F32: return 4;
        case DataType::F64: return 8;
        case DataType::Raw: return raw_len;
    }
    return 0;
}

void ProcessImage::check(std::size_t off, std::size_t len) const {
    if (off + len > buf_.size()) {
        throw std::out_of_range("mgate: process image access at byte " + std::to_string(off) + " (+" +
                                std::to_string(len) + ") exceeds area size " + std::to_string(buf_.size()));
    }
}

// --- getters --------------------------------------------------------------

bool ProcessImage::get_bit(std::size_t off, std::uint8_t bit) const {
    check(off, 1);
    if (bit > 7) throw std::out_of_range("mgate: bit index must be 0..7");
    return ((buf_[off] >> bit) & 0x01) != 0;
}

std::uint8_t ProcessImage::get_u8(std::size_t off) const {
    check(off, 1);
    return buf_[off];
}

std::int8_t ProcessImage::get_i8(std::size_t off) const {
    return static_cast<std::int8_t>(get_u8(off));
}

std::uint16_t ProcessImage::get_u16(std::size_t off, ByteOrder o) const {
    check(off, 2);
    std::uint8_t tmp[2];
    reorder(buf_.data() + off, tmp, 2, o);
    return static_cast<std::uint16_t>(be_to_u64(tmp, 2));
}

std::int16_t ProcessImage::get_i16(std::size_t off, ByteOrder o) const {
    return static_cast<std::int16_t>(get_u16(off, o));
}

std::uint32_t ProcessImage::get_u32(std::size_t off, ByteOrder o) const {
    check(off, 4);
    std::uint8_t tmp[4];
    reorder(buf_.data() + off, tmp, 4, o);
    return static_cast<std::uint32_t>(be_to_u64(tmp, 4));
}

std::int32_t ProcessImage::get_i32(std::size_t off, ByteOrder o) const {
    return static_cast<std::int32_t>(get_u32(off, o));
}

float ProcessImage::get_f32(std::size_t off, ByteOrder o) const {
    const std::uint32_t bits = get_u32(off, o);
    float               f;
    std::memcpy(&f, &bits, sizeof(f));
    return f;
}

double ProcessImage::get_f64(std::size_t off, ByteOrder o) const {
    check(off, 8);
    std::uint8_t tmp[8];
    reorder(buf_.data() + off, tmp, 8, o);
    const std::uint64_t bits = be_to_u64(tmp, 8);
    double              d;
    std::memcpy(&d, &bits, sizeof(d));
    return d;
}

void ProcessImage::get_raw(std::size_t off, std::uint8_t* dst, std::size_t len) const {
    check(off, len);
    std::memcpy(dst, buf_.data() + off, len);
}

// --- setters --------------------------------------------------------------

void ProcessImage::set_bit(std::size_t off, std::uint8_t bit, bool v) {
    check(off, 1);
    if (bit > 7) throw std::out_of_range("mgate: bit index must be 0..7");
    const std::uint8_t mask = static_cast<std::uint8_t>(1u << bit);
    if (v) {
        buf_[off] = static_cast<std::uint8_t>(buf_[off] | mask);
    } else {
        buf_[off] = static_cast<std::uint8_t>(buf_[off] & ~mask);
    }
}

void ProcessImage::set_u8(std::size_t off, std::uint8_t v) {
    check(off, 1);
    buf_[off] = v;
}

void ProcessImage::set_i8(std::size_t off, std::int8_t v) {
    set_u8(off, static_cast<std::uint8_t>(v));
}

void ProcessImage::set_u16(std::size_t off, std::uint16_t v, ByteOrder o) {
    check(off, 2);
    std::uint8_t be[2];
    u64_to_be(v, be, 2);
    reorder(be, buf_.data() + off, 2, o);
}

void ProcessImage::set_i16(std::size_t off, std::int16_t v, ByteOrder o) {
    set_u16(off, static_cast<std::uint16_t>(v), o);
}

void ProcessImage::set_u32(std::size_t off, std::uint32_t v, ByteOrder o) {
    check(off, 4);
    std::uint8_t be[4];
    u64_to_be(v, be, 4);
    reorder(be, buf_.data() + off, 4, o);
}

void ProcessImage::set_i32(std::size_t off, std::int32_t v, ByteOrder o) {
    set_u32(off, static_cast<std::uint32_t>(v), o);
}

void ProcessImage::set_f32(std::size_t off, float v, ByteOrder o) {
    std::uint32_t bits;
    std::memcpy(&bits, &v, sizeof(bits));
    set_u32(off, bits, o);
}

void ProcessImage::set_f64(std::size_t off, double v, ByteOrder o) {
    check(off, 8);
    std::uint64_t bits;
    std::memcpy(&bits, &v, sizeof(bits));
    std::uint8_t be[8];
    u64_to_be(bits, be, 8);
    reorder(be, buf_.data() + off, 8, o);
}

void ProcessImage::set_raw(std::size_t off, const std::uint8_t* src, std::size_t len) {
    check(off, len);
    std::memcpy(buf_.data() + off, src, len);
}

// --- tag access -----------------------------------------------------------

std::int64_t ProcessImage::get_int(const Tag& t) const {
    switch (t.type) {
        case DataType::Bit: return get_bit(t.byte_offset, t.bit) ? 1 : 0;
        case DataType::U8:  return get_u8(t.byte_offset);
        case DataType::I8:  return get_i8(t.byte_offset);
        case DataType::U16: return get_u16(t.byte_offset, t.order);
        case DataType::I16: return get_i16(t.byte_offset, t.order);
        case DataType::U32: return static_cast<std::int64_t>(get_u32(t.byte_offset, t.order));
        case DataType::I32: return get_i32(t.byte_offset, t.order);
        case DataType::F32: return static_cast<std::int64_t>(get_f32(t.byte_offset, t.order));
        case DataType::F64: return static_cast<std::int64_t>(get_f64(t.byte_offset, t.order));
        case DataType::Raw: break;
    }
    throw std::invalid_argument("mgate: tag '" + t.name + "' has no integer representation");
}

double ProcessImage::get_scaled(const Tag& t) const {
    double raw;
    if (t.type == DataType::F32) {
        raw = static_cast<double>(get_f32(t.byte_offset, t.order));
    } else if (t.type == DataType::F64) {
        raw = get_f64(t.byte_offset, t.order);
    } else {
        raw = static_cast<double>(get_int(t));
    }
    return raw * t.scale + t.bias;
}

bool ProcessImage::get_bool(const Tag& t) const {
    if (t.type == DataType::Bit) return get_bit(t.byte_offset, t.bit);
    return get_int(t) != 0;
}

void ProcessImage::set_int(const Tag& t, std::int64_t raw) {
    switch (t.type) {
        case DataType::Bit: set_bit(t.byte_offset, t.bit, raw != 0); return;
        case DataType::U8:  set_u8(t.byte_offset, static_cast<std::uint8_t>(raw)); return;
        case DataType::I8:  set_i8(t.byte_offset, static_cast<std::int8_t>(raw)); return;
        case DataType::U16: set_u16(t.byte_offset, static_cast<std::uint16_t>(raw), t.order); return;
        case DataType::I16: set_i16(t.byte_offset, static_cast<std::int16_t>(raw), t.order); return;
        case DataType::U32: set_u32(t.byte_offset, static_cast<std::uint32_t>(raw), t.order); return;
        case DataType::I32: set_i32(t.byte_offset, static_cast<std::int32_t>(raw), t.order); return;
        case DataType::F32: set_f32(t.byte_offset, static_cast<float>(raw), t.order); return;
        case DataType::F64: set_f64(t.byte_offset, static_cast<double>(raw), t.order); return;
        case DataType::Raw: break;
    }
    throw std::invalid_argument("mgate: tag '" + t.name + "' has no integer representation");
}

void ProcessImage::set_scaled(const Tag& t, double engineering_value) {
    const double raw = (t.scale != 0.0) ? (engineering_value - t.bias) / t.scale : 0.0;
    if (t.type == DataType::F32) {
        set_f32(t.byte_offset, static_cast<float>(raw), t.order);
    } else if (t.type == DataType::F64) {
        set_f64(t.byte_offset, raw, t.order);
    } else {
        set_int(t, static_cast<std::int64_t>(std::llround(raw)));
    }
}

void ProcessImage::set_bool(const Tag& t, bool v) {
    if (t.type == DataType::Bit) {
        set_bit(t.byte_offset, t.bit, v);
    } else {
        set_int(t, v ? 1 : 0);
    }
}

// --- register conversion --------------------------------------------------

void ProcessImage::load_from_registers(const std::uint16_t* regs, std::size_t count, std::size_t byte_offset) {
    check(byte_offset, count * 2);
    for (std::size_t i = 0; i < count; ++i) {
        buf_[byte_offset + 2 * i]     = static_cast<std::uint8_t>(regs[i] >> 8);
        buf_[byte_offset + 2 * i + 1] = static_cast<std::uint8_t>(regs[i] & 0xFF);
    }
}

void ProcessImage::store_to_registers(std::uint16_t* regs, std::size_t count, std::size_t byte_offset) const {
    check(byte_offset, count * 2);
    for (std::size_t i = 0; i < count; ++i) {
        regs[i] = static_cast<std::uint16_t>((buf_[byte_offset + 2 * i] << 8) | buf_[byte_offset + 2 * i + 1]);
    }
}

}  // namespace mgate
