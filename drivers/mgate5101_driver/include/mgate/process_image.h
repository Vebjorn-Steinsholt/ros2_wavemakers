// process_image.h - byte-addressed view of the gateway's PROFIBUS I/O buffers.
//
// Why byte-addressed and not register-addressed:
// PROFIBUS modules are allocated in BYTES, back to back, in GSD module order.
// A 1-byte digital module followed by a 2-byte analog module puts that analog
// word across a Modbus register boundary. If you index by register you will
// silently read two half-values. So: pull the registers down, flatten them
// back into the gateway's byte buffer (hi byte first), and address tags by
// PROFIBUS byte offset - exactly the offset the MGate's mapping page shows.

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace mgate {

// Applies to multi-byte values only. "Register" below means one 16-bit Modbus
// register, i.e. one byte pair.
enum class ByteOrder : std::uint8_t {
    BigEndian,        // ABCD - PROFIBUS/Siemens native. Correct default.
    LittleEndian,     // DCBA - full reverse
    BigEndianSwap,    // CDAB - register order reversed, bytes within kept
    LittleEndianSwap, // BADC - register order kept, bytes within swapped
};

enum class Area : std::uint8_t {
    Input,   // PROFIBUS slave -> gateway -> your PC   (read only)
    Output,  // your PC -> gateway -> PROFIBUS slave   (read/write)
};

enum class DataType : std::uint8_t { Bit, U8, I8, U16, I16, U32, I32, F32, F64, Raw };

// Size in bytes a tag occupies in the process image. Bit occupies 1.
std::size_t type_size(DataType t, std::size_t raw_len = 0);

struct Tag {
    std::string  name;
    Area         area        = Area::Input;
    DataType     type        = DataType::U16;
    std::size_t  byte_offset = 0;  // offset within the input or output area
    std::uint8_t bit         = 0;  // 0..7, only for DataType::Bit
    std::size_t  raw_len     = 0;  // only for DataType::Raw
    ByteOrder    order       = ByteOrder::BigEndian;
    double       scale       = 1.0;  // engineering = raw * scale + bias
    double       bias        = 0.0;
    std::string  unit;
    std::string  comment;

    std::size_t size_bytes() const { return type_size(type, raw_len); }
};

class ProcessImage {
public:
    ProcessImage() = default;
    explicit ProcessImage(std::size_t size_bytes) : buf_(size_bytes, 0) {}

    void        resize(std::size_t n) { buf_.assign(n, 0); }
    std::size_t size() const noexcept { return buf_.size(); }
    bool        empty() const noexcept { return buf_.empty(); }

    const std::uint8_t*              data() const noexcept { return buf_.data(); }
    std::uint8_t*                    data() noexcept { return buf_.data(); }
    const std::vector<std::uint8_t>& bytes() const noexcept { return buf_; }

    // --- raw typed access by PROFIBUS byte offset -------------------------
    bool          get_bit(std::size_t off, std::uint8_t bit) const;
    std::uint8_t  get_u8(std::size_t off) const;
    std::int8_t   get_i8(std::size_t off) const;
    std::uint16_t get_u16(std::size_t off, ByteOrder o) const;
    std::int16_t  get_i16(std::size_t off, ByteOrder o) const;
    std::uint32_t get_u32(std::size_t off, ByteOrder o) const;
    std::int32_t  get_i32(std::size_t off, ByteOrder o) const;
    float         get_f32(std::size_t off, ByteOrder o) const;
    double        get_f64(std::size_t off, ByteOrder o) const;
    void          get_raw(std::size_t off, std::uint8_t* dst, std::size_t len) const;

    void set_bit(std::size_t off, std::uint8_t bit, bool v);
    void set_u8(std::size_t off, std::uint8_t v);
    void set_i8(std::size_t off, std::int8_t v);
    void set_u16(std::size_t off, std::uint16_t v, ByteOrder o);
    void set_i16(std::size_t off, std::int16_t v, ByteOrder o);
    void set_u32(std::size_t off, std::uint32_t v, ByteOrder o);
    void set_i32(std::size_t off, std::int32_t v, ByteOrder o);
    void set_f32(std::size_t off, float v, ByteOrder o);
    void set_f64(std::size_t off, double v, ByteOrder o);
    void set_raw(std::size_t off, const std::uint8_t* src, std::size_t len);

    // --- tag-oriented access ---------------------------------------------
    double       get_scaled(const Tag& t) const;  // applies scale/bias
    std::int64_t get_int(const Tag& t) const;     // raw integer, no scaling
    bool         get_bool(const Tag& t) const;

    void set_scaled(const Tag& t, double engineering_value);
    void set_int(const Tag& t, std::int64_t raw);
    void set_bool(const Tag& t, bool v);

    // --- Modbus register <-> byte buffer ----------------------------------
    // Registers are laid down high-byte-first, which reproduces the gateway's
    // internal byte order exactly.
    void load_from_registers(const std::uint16_t* regs, std::size_t count, std::size_t byte_offset = 0);
    void store_to_registers(std::uint16_t* regs, std::size_t count, std::size_t byte_offset = 0) const;

private:
    void check(std::size_t off, std::size_t len) const;

    std::vector<std::uint8_t> buf_;
};

}  // namespace mgate
