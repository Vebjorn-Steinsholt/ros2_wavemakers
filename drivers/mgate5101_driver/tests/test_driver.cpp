// End-to-end test of MGateDriver against the Python stub server.
#include <chrono>
#include <cmath>
#include <cstdio>
#include <thread>
#include "mgate/mgate_driver.h"

using namespace mgate;

int failures = 0;
#define CHECK(cond) do { if (!(cond)) { std::printf("FAIL %d: %s\n", __LINE__, #cond); ++failures; } } while (0)

int main() {
    GatewayConfig cfg;
    cfg.host = "127.0.0.1";
    cfg.port = 1502;
    cfg.unit_id = 1;
    cfg.input_base_reg = 0x0000;
    cfg.input_word_count = 7;
    cfg.output_base_reg = 0x0800;
    cfg.output_word_count = 7;
    cfg.poll_interval_ms = 20;
    cfg.timeout_ms = 1000;

    MGateDriver drv(cfg);
    drv.add_tags({
        {"S4.DI1", Area::Input, DataType::Bit, 0, 1},
        {"S4.DI3", Area::Input, DataType::Bit, 0, 3},
        {"S4.AI0", Area::Input, DataType::I16, 1, 0, 0, ByteOrder::BigEndian, 100.0 / 27648.0, 0.0, "%"},
        {"S4.AI1", Area::Input, DataType::I16, 3},
        {"VFD.StatusWord", Area::Input, DataType::U16, 6},
        {"VFD.ActualSpeed", Area::Input, DataType::I16, 8, 0, 0, ByteOrder::BigEndian, 0.01, 0.0, "Hz"},

        {"S4.DO0", Area::Output, DataType::Bit, 0, 0},
        {"S4.AO0", Area::Output, DataType::I16, 1, 0, 0, ByteOrder::BigEndian, 100.0 / 27648.0, 0.0, "%"},
        {"VFD.ControlWord", Area::Output, DataType::U16, 6},
        {"VFD.SpeedSetpoint", Area::Output, DataType::I16, 8, 0, 0, ByteOrder::BigEndian, 0.01, 0.0, "Hz"},
    });

    // Out-of-range tag must be rejected at registration.
    bool threw = false;
    try { drv.add_tag({"oops", Area::Input, DataType::U16, 13}); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
    // Wrong-area access must be rejected.
    threw = false;
    try { drv.write("S4.AI0", 1.0); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);

    drv.start();
    CHECK(drv.wait_for_data(5000));
    CHECK(drv.online());
    if (!drv.online()) { std::printf("  last_error: %s\n", drv.stats().last_error.c_str()); drv.stop(); return 1; }

    CHECK(drv.read_bool("S4.DI1") == true);
    CHECK(drv.read_bool("S4.DI3") == true);
    CHECK(std::fabs(drv.read("S4.AI0") - 50.0) < 1e-9);   // straddles reg0/reg1
    CHECK(drv.read_int("S4.AI1") == -1000);               // straddles reg1/reg2
    CHECK(drv.read_int("VFD.StatusWord") == 0x0F37);
    CHECK(std::fabs(drv.read("VFD.ActualSpeed") - 25.0) < 1e-9);

    // Stage writes; they should reach the stub on the next cycle.
    drv.write_bool("S4.DO0", true);
    drv.write("S4.AO0", 50.0);
    drv.write_int("VFD.ControlWord", 0x047F);
    drv.write("VFD.SpeedSetpoint", 25.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    CHECK(drv.read_output_bool("S4.DO0") == true);
    CHECK(drv.read_output_int("VFD.ControlWord") == 0x047F);

    const DriverStats s = drv.stats();
    std::printf("cycles=%llu read_err=%llu write_err=%llu reconnects=%llu\n",
                (unsigned long long)s.cycles, (unsigned long long)s.read_errors,
                (unsigned long long)s.write_errors, (unsigned long long)s.reconnects);
    CHECK(s.read_errors == 0);
    CHECK(s.write_errors == 0);
    CHECK(s.cycles > 5);

    drv.stop();
    std::printf(failures ? "%d FAILURE(S)\n" : "driver end-to-end passed\n", failures);
    return failures ? 1 : 0;
}
