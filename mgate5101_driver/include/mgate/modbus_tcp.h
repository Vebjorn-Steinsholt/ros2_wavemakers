// modbus_tcp.h - minimal, dependency-free Modbus/TCP client for Windows (Winsock2).
//
// Deliberately small: no libmodbus, no Boost. Blocking sockets with SO_RCVTIMEO,
// one outstanding transaction at a time. Not thread-safe; own it from one thread
// (MGateDriver does exactly that).

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace mgate {

enum class Status : int {
    Ok = 0,
    NotConnected,
    SocketError,
    Timeout,
    ProtocolError,
    ModbusException,  // the gateway answered with an exception response
    BadArgument,
};

const char* status_name(Status s);
const char* modbus_exception_name(std::uint8_t code);

struct Result {
    Status status = Status::Ok;
    // Meaningful only when status == ModbusException.
    // NOT named exception_code: <excpt.h>, pulled in by <windows.h>, #defines that.
    std::uint8_t mb_exception = 0;

    explicit operator bool() const noexcept { return status == Status::Ok; }
    std::string message() const;
};

class ModbusTcpClient {
public:
    ModbusTcpClient();
    ~ModbusTcpClient();

    ModbusTcpClient(const ModbusTcpClient&)            = delete;
    ModbusTcpClient& operator=(const ModbusTcpClient&) = delete;

    Result connect(const std::string& host, std::uint16_t port, int timeout_ms);
    void   disconnect();
    bool   is_connected() const noexcept;

    void set_unit_id(std::uint8_t id) noexcept { unit_id_ = id; }
    void set_timeout(int ms) noexcept { timeout_ms_ = ms; }

    // FC 0x03 - read holding registers (max 125)
    Result read_holding_registers(std::uint16_t addr, std::uint16_t count, std::uint16_t* dst);
    // FC 0x04 - read input registers (max 125)
    Result read_input_registers(std::uint16_t addr, std::uint16_t count, std::uint16_t* dst);
    // FC 0x06 - write single register
    Result write_single_register(std::uint16_t addr, std::uint16_t value);
    // FC 0x10 - write multiple registers (max 123)
    Result write_multiple_registers(std::uint16_t addr, std::uint16_t count, const std::uint16_t* src);
    // FC 0x17 - read/write multiple registers in one round trip.
    // Check your gateway's supported-function-code list before enabling this.
    Result read_write_multiple_registers(std::uint16_t read_addr, std::uint16_t read_count,
                                         std::uint16_t* dst,
                                         std::uint16_t write_addr, std::uint16_t write_count,
                                         const std::uint16_t* src);

private:
    Result transact(const std::uint8_t* pdu, std::size_t pdu_len, std::uint8_t fc,
                    std::uint8_t* resp, std::size_t resp_cap, std::size_t* resp_len);
    Result read_registers(std::uint8_t fc, std::uint16_t addr, std::uint16_t count, std::uint16_t* dst);

    bool send_all(const std::uint8_t* p, std::size_t n);
    int  recv_exact(std::uint8_t* p, std::size_t n);  // 1 = ok, 0 = timeout, -1 = error/closed
    void drop();

    std::uintptr_t sock_;
    std::uint16_t  tid_        = 0;
    std::uint8_t   unit_id_    = 1;
    int            timeout_ms_ = 1000;
};

}  // namespace mgate
