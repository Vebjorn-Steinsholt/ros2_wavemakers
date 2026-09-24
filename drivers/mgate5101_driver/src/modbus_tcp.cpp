#include "mgate/modbus_tcp.h"

#ifdef _WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
#else
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <netinet/tcp.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <cerrno>
  using SOCKET = int;
  constexpr int INVALID_SOCKET = -1;
  constexpr int SOCKET_ERROR   = -1;
#endif

#include <cstring>

namespace mgate {
namespace {

constexpr std::uintptr_t kInvalid = static_cast<std::uintptr_t>(INVALID_SOCKET);

#ifdef _WIN32
struct WsaInit {
    bool ok = false;
    WsaInit() {
        WSADATA d{};
        ok = (::WSAStartup(MAKEWORD(2, 2), &d) == 0);
    }
    ~WsaInit() {
        if (ok) ::WSACleanup();
    }
};

bool ensure_wsa() {
    static WsaInit g;
    return g.ok;
}
#else
bool ensure_wsa() { return true; }  // no-op on POSIX
#endif

// --- Platform shims for the parts that differ between Winsock and POSIX ---

inline void close_socket(SOCKET s) {
#ifdef _WIN32
    ::closesocket(s);
#else
    ::close(s);
#endif
}

inline void shutdown_socket(SOCKET s) {
#ifdef _WIN32
    ::shutdown(s, SD_BOTH);
#else
    ::shutdown(s, SHUT_RDWR);
#endif
}

inline bool set_nonblocking(SOCKET s, bool enable) {
#ifdef _WIN32
    u_long mode = enable ? 1 : 0;
    return ::ioctlsocket(s, FIONBIO, &mode) == 0;
#else
    int flags = ::fcntl(s, F_GETFL, 0);
    if (flags < 0) return false;
    flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    return ::fcntl(s, F_SETFL, flags) == 0;
#endif
}

inline bool connect_in_progress() {
#ifdef _WIN32
    return ::WSAGetLastError() == WSAEWOULDBLOCK;
#else
    return errno == EINPROGRESS;
#endif
}

inline bool recv_timed_out() {
#ifdef _WIN32
    return ::WSAGetLastError() == WSAETIMEDOUT;
#else
    return errno == EWOULDBLOCK || errno == EAGAIN;
#endif
}

inline void set_recv_send_timeout(SOCKET s, int timeout_ms) {
#ifdef _WIN32
    DWORD to = static_cast<DWORD>(timeout_ms);
    ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&to), sizeof(to));
    ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char*>(&to), sizeof(to));
#else
    timeval tv{};
    tv.tv_sec  = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;
    ::setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    ::setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
#endif
}

inline std::uint8_t hi(std::uint16_t v) { return static_cast<std::uint8_t>(v >> 8); }
inline std::uint8_t lo(std::uint16_t v) { return static_cast<std::uint8_t>(v & 0xFF); }

}  // namespace

const char* status_name(Status s) {
    switch (s) {
        case Status::Ok:              return "ok";
        case Status::NotConnected:    return "not connected";
        case Status::SocketError:     return "socket error";
        case Status::Timeout:         return "timeout";
        case Status::ProtocolError:   return "protocol error";
        case Status::ModbusException: return "modbus exception";
        case Status::BadArgument:     return "bad argument";
    }
    return "unknown";
}

const char* modbus_exception_name(std::uint8_t code) {
    switch (code) {
        case 0x01: return "illegal function";
        case 0x02: return "illegal data address";
        case 0x03: return "illegal data value";
        case 0x04: return "slave device failure";
        case 0x05: return "acknowledge";
        case 0x06: return "slave device busy";
        case 0x08: return "memory parity error";
        case 0x0A: return "gateway path unavailable";
        case 0x0B: return "gateway target device failed to respond";
        default:   return "unknown exception";
    }
}

std::string Result::message() const {
    std::string m = status_name(status);
    if (status == Status::ModbusException) {
        m += " 0x";
        const char* digits = "0123456789ABCDEF";
        m += digits[(mb_exception >> 4) & 0x0F];
        m += digits[mb_exception & 0x0F];
        m += " (";
        m += modbus_exception_name(mb_exception);
        m += ")";
    }
    return m;
}

ModbusTcpClient::ModbusTcpClient() : sock_(kInvalid) {}

ModbusTcpClient::~ModbusTcpClient() { disconnect(); }

bool ModbusTcpClient::is_connected() const noexcept { return sock_ != kInvalid; }

void ModbusTcpClient::drop() {
    if (sock_ != kInvalid) {
        close_socket(static_cast<SOCKET>(sock_));
        sock_ = kInvalid;
    }
}

void ModbusTcpClient::disconnect() {
    if (sock_ != kInvalid) {
        shutdown_socket(static_cast<SOCKET>(sock_));
        drop();
    }
}

Result ModbusTcpClient::connect(const std::string& host, std::uint16_t port, int timeout_ms) {
    disconnect();
    if (!ensure_wsa()) return {Status::SocketError};

    timeout_ms_ = timeout_ms;

    addrinfo hints{};
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    addrinfo*         res  = nullptr;
    const std::string port_s = std::to_string(port);
    if (::getaddrinfo(host.c_str(), port_s.c_str(), &hints, &res) != 0 || res == nullptr) {
        return {Status::SocketError};
    }

    Status last = Status::SocketError;
    for (addrinfo* ai = res; ai != nullptr; ai = ai->ai_next) {
        SOCKET s = ::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        if (s == INVALID_SOCKET) continue;

        set_nonblocking(s, true);

        int rc = ::connect(s, ai->ai_addr, static_cast<int>(ai->ai_addrlen));
        if (rc == SOCKET_ERROR) {
            if (!connect_in_progress()) {
                close_socket(s);
                continue;
            }
            fd_set wfds;
            FD_ZERO(&wfds);
            FD_SET(s, &wfds);
            fd_set efds = wfds;
            timeval tv{};
            tv.tv_sec  = timeout_ms / 1000;
            tv.tv_usec = (timeout_ms % 1000) * 1000;

#ifdef _WIN32
            rc = ::select(0, nullptr, &wfds, &efds, &tv);
#else
            rc = ::select(s + 1, nullptr, &wfds, &efds, &tv);
#endif
            if (rc <= 0) {
                close_socket(s);
                last = (rc == 0) ? Status::Timeout : Status::SocketError;
                continue;
            }
            int       err = 0;
            socklen_t len = sizeof(err);
#ifdef _WIN32
            if (::getsockopt(s, SOL_SOCKET, SO_ERROR, reinterpret_cast<char*>(&err),
                              reinterpret_cast<int*>(&len)) != 0 || err != 0) {
#else
            if (::getsockopt(s, SOL_SOCKET, SO_ERROR, &err, &len) != 0 || err != 0) {
#endif
                close_socket(s);
                continue;
            }
        }

        set_nonblocking(s, false);

        int one = 1;
        ::setsockopt(s, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&one), sizeof(one));
        set_recv_send_timeout(s, timeout_ms);

        sock_ = static_cast<std::uintptr_t>(s);
        ::freeaddrinfo(res);
        return {};
    }

    ::freeaddrinfo(res);
    return {last};
}

bool ModbusTcpClient::send_all(const std::uint8_t* p, std::size_t n) {
    SOCKET      s    = static_cast<SOCKET>(sock_);
    std::size_t sent = 0;
    while (sent < n) {
        int r = ::send(s, reinterpret_cast<const char*>(p) + sent, static_cast<int>(n - sent), 0);
        if (r <= 0) return false;
        sent += static_cast<std::size_t>(r);
    }
    return true;
}

int ModbusTcpClient::recv_exact(std::uint8_t* p, std::size_t n) {
    SOCKET      s   = static_cast<SOCKET>(sock_);
    std::size_t got = 0;
    while (got < n) {
        int r = ::recv(s, reinterpret_cast<char*>(p) + got, static_cast<int>(n - got), 0);
        if (r > 0) {
            got += static_cast<std::size_t>(r);
            continue;
        }
        if (r == 0) return -1;  // peer closed
        if (recv_timed_out()) return 0;
        return -1;
    }
    return 1;
}

Result ModbusTcpClient::transact(const std::uint8_t* pdu, std::size_t pdu_len, std::uint8_t fc,
                                 std::uint8_t* resp, std::size_t resp_cap, std::size_t* resp_len) {
    if (!is_connected()) return {Status::NotConnected};
    if (pdu_len == 0 || pdu_len > 253) return {Status::BadArgument};

    const std::uint16_t tid = ++tid_;

    std::uint8_t frame[260];
    frame[0] = hi(tid);
    frame[1] = lo(tid);
    frame[2] = 0;  // protocol id = 0
    frame[3] = 0;
    const std::uint16_t mbap_len = static_cast<std::uint16_t>(pdu_len + 1);
    frame[4] = hi(mbap_len);
    frame[5] = lo(mbap_len);
    frame[6] = unit_id_;
    std::memcpy(frame + 7, pdu, pdu_len);

    if (!send_all(frame, 7 + pdu_len)) {
        drop();
        return {Status::SocketError};
    }

    // A late reply to a previous, timed-out request can still be sitting in the
    // socket. Skip frames whose transaction id does not match ours.
    for (int attempt = 0; attempt < 4; ++attempt) {
        std::uint8_t hdr[7];
        int          r = recv_exact(hdr, sizeof(hdr));
        if (r == 0) return {Status::Timeout};
        if (r < 0) {
            drop();
            return {Status::SocketError};
        }

        const std::uint16_t rtid  = static_cast<std::uint16_t>((hdr[0] << 8) | hdr[1]);
        const std::uint16_t proto = static_cast<std::uint16_t>((hdr[2] << 8) | hdr[3]);
        const std::uint16_t rlen  = static_cast<std::uint16_t>((hdr[4] << 8) | hdr[5]);
        if (proto != 0 || rlen < 2 || rlen > 254) {
            drop();
            return {Status::ProtocolError};
        }

        std::uint8_t body[256];
        r = recv_exact(body, static_cast<std::size_t>(rlen) - 1);
        if (r == 0) return {Status::Timeout};
        if (r < 0) {
            drop();
            return {Status::SocketError};
        }

        if (rtid != tid) continue;  // stale frame, keep looking

        const std::uint8_t rfc = body[0];
        if (rfc == static_cast<std::uint8_t>(fc | 0x80)) {
            Result res{Status::ModbusException};
            res.mb_exception = (rlen >= 3) ? body[1] : 0;
            return res;
        }
        if (rfc != fc) {
            drop();
            return {Status::ProtocolError};
        }

        const std::size_t n = static_cast<std::size_t>(rlen) - 2;
        if (n > resp_cap) {
            drop();
            return {Status::ProtocolError};
        }
        std::memcpy(resp, body + 1, n);
        if (resp_len) *resp_len = n;
        return {};
    }
    return {Status::ProtocolError};
}

Result ModbusTcpClient::read_registers(std::uint8_t fc, std::uint16_t addr, std::uint16_t count,
                                       std::uint16_t* dst) {
    if (count == 0 || count > 125 || dst == nullptr) return {Status::BadArgument};

    const std::uint8_t pdu[5] = {fc, hi(addr), lo(addr), hi(count), lo(count)};
    std::uint8_t       resp[256];
    std::size_t        rn = 0;

    Result r = transact(pdu, sizeof(pdu), fc, resp, sizeof(resp), &rn);
    if (!r) return r;

    const std::size_t want = static_cast<std::size_t>(count) * 2;
    if (rn < 1 + want || resp[0] != want) return {Status::ProtocolError};

    for (std::uint16_t i = 0; i < count; ++i) {
        dst[i] = static_cast<std::uint16_t>((resp[1 + 2 * i] << 8) | resp[2 + 2 * i]);
    }
    return {};
}

Result ModbusTcpClient::read_holding_registers(std::uint16_t addr, std::uint16_t count, std::uint16_t* dst) {
    return read_registers(0x03, addr, count, dst);
}

Result ModbusTcpClient::read_input_registers(std::uint16_t addr, std::uint16_t count, std::uint16_t* dst) {
    return read_registers(0x04, addr, count, dst);
}

Result ModbusTcpClient::write_single_register(std::uint16_t addr, std::uint16_t value) {
    const std::uint8_t pdu[5] = {0x06, hi(addr), lo(addr), hi(value), lo(value)};
    std::uint8_t       resp[8];
    std::size_t        rn = 0;
    return transact(pdu, sizeof(pdu), 0x06, resp, sizeof(resp), &rn);
}

Result ModbusTcpClient::write_multiple_registers(std::uint16_t addr, std::uint16_t count,
                                                 const std::uint16_t* src) {
    if (count == 0 || count > 123 || src == nullptr) return {Status::BadArgument};

    std::uint8_t pdu[256];
    pdu[0] = 0x10;
    pdu[1] = hi(addr);
    pdu[2] = lo(addr);
    pdu[3] = hi(count);
    pdu[4] = lo(count);
    pdu[5] = static_cast<std::uint8_t>(count * 2);
    for (std::uint16_t i = 0; i < count; ++i) {
        pdu[6 + 2 * i] = hi(src[i]);
        pdu[7 + 2 * i] = lo(src[i]);
    }

    std::uint8_t resp[8];
    std::size_t  rn = 0;
    return transact(pdu, 6 + static_cast<std::size_t>(count) * 2, 0x10, resp, sizeof(resp), &rn);
}

Result ModbusTcpClient::read_write_multiple_registers(std::uint16_t read_addr, std::uint16_t read_count,
                                                      std::uint16_t* dst, std::uint16_t write_addr,
                                                      std::uint16_t write_count, const std::uint16_t* src) {
    if (read_count == 0 || read_count > 125 || dst == nullptr) return {Status::BadArgument};
    if (write_count == 0 || write_count > 121 || src == nullptr) return {Status::BadArgument};

    std::uint8_t pdu[256];
    pdu[0] = 0x17;
    pdu[1] = hi(read_addr);
    pdu[2] = lo(read_addr);
    pdu[3] = hi(read_count);
    pdu[4] = lo(read_count);
    pdu[5] = hi(write_addr);
    pdu[6] = lo(write_addr);
    pdu[7] = hi(write_count);
    pdu[8] = lo(write_count);
    pdu[9] = static_cast<std::uint8_t>(write_count * 2);
    for (std::uint16_t i = 0; i < write_count; ++i) {
        pdu[10 + 2 * i] = hi(src[i]);
        pdu[11 + 2 * i] = lo(src[i]);
    }

    std::uint8_t resp[256];
    std::size_t  rn = 0;
    Result       r  = transact(pdu, 10 + static_cast<std::size_t>(write_count) * 2, 0x17, resp,
                               sizeof(resp), &rn);
    if (!r) return r;

    const std::size_t want = static_cast<std::size_t>(read_count) * 2;
    if (rn < 1 + want || resp[0] != want) return {Status::ProtocolError};
    for (std::uint16_t i = 0; i < read_count; ++i) {
        dst[i] = static_cast<std::uint16_t>((resp[1 + 2 * i] << 8) | resp[2 + 2 * i]);
    }
    return {};
}

}  // namespace mgate