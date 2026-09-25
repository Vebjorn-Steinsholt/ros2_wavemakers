// example_main.cpp - Rexroth IndraDrive (HCS03 / CSB01.1C-PB, MPB-04) behind
// an MGate 5101-PBM-MN at 192.168.1.70.
//
//   mgate_example            monitor only: prints drive status, never commands it
//   mgate_example --move     clears errors, enables, moves +5 deg and back at
//                            10 rpm, disables. Ctrl+C halts and disables.
//
// Before --move: set a fault timeout (200-500 ms, fault value 00) on the
// MGate's "Output 6 Words" module, and keep the E-stop within reach.
//
// Modbus (MGate manual p.37, register = byte offset / 2): reads at 0x0000 hit
// the input area, writes at 0x0000 hit the output area. Never write register
// 0x0300 - that is the gateway's PROFIBUS master control word.

#include <atomic>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <thread>

#include "mgate/indradrive.h"
#include "mgate/mgate_driver.h"

using namespace mgate;

namespace {

std::atomic<IndraDrive*> g_drive{nullptr};

void on_sigint(int) {
    if (IndraDrive* d = g_drive.load()) d->request_abort();
}

void sleep_ms(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

int monitor(MGateDriver& drv, IndraDrive& drive) {
    for (int i = 0; i < 50; ++i) {
        if (drv.online())
            std::printf("%s\n", drive.describe().c_str());
        else
            std::printf("offline: %s\n", drv.stats().last_error.c_str());
        sleep_ms(200);
    }
    return 0;
}

// Hardware test checklist for --move:
// - Verify the gateway address map, unit ID, input/output sizes, and byte order.
// - Confirm the drive is mechanically clear, the E-stop is reachable, and the
//   MGate output fault timeout is configured before enabling the axis.
// - Run monitor mode first and confirm the status word, position, velocity,
//   diagnostic number, and PROFIBUS live bit are sensible.
// - Start with the configured low speed and a small step; confirm the axis
//   stops at the target, returns to its starting position, and disables cleanly.
// - Interrupt the test or disconnect the gateway and verify the control word
//   goes to zero and the drive does not restart without enable().
int test_move(IndraDrive& drive) {
    constexpr double kStepDeg = 5.0;
    constexpr double kRpm     = 10.0;

    std::printf("before:  %s\n", drive.describe().c_str());

    if (drive.status_word() & indra_sw::kClass1Error) {
        std::printf("drive has an error, clearing...\n");
        if (!drive.clear_errors()) return 1;
        std::printf("cleared: %s\n", drive.describe().c_str());
    }

    std::printf("enabling...\n");
    if (!drive.enable()) return 1;
    std::printf("enabled: %s\n", drive.describe().c_str());

    const double start = drive.position_deg();
    const double moves[] = {start + kStepDeg, start};
    for (double target : moves) {
        std::printf("move to %.4f deg at %.1f rpm\n", target, kRpm);
        if (!drive.move_to(target, kRpm) || !drive.wait_in_position(15000)) return 1;
        std::printf("reached: %s\n", drive.describe().c_str());
        sleep_ms(500);
    }
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    const bool do_move = argc > 1 && std::strcmp(argv[1], "--move") == 0;

    GatewayConfig cfg;
    cfg.host    = "192.168.1.70";
    cfg.port    = 502;
    cfg.unit_id = 1;

    cfg.input_base_reg    = 0x0000;
    cfg.input_word_count  = 7;
    cfg.output_base_reg   = 0x0000;  // same address as inputs: FC16 goes to the output area
    cfg.output_word_count = 6;
    cfg.input_uses_fc4    = true;

    // Gateway status word + PROFIBUS live list (used to detect a lost slave).
    cfg.status_base_reg   = 0x0300;
    cfg.status_word_count = 9;

    cfg.poll_interval_ms = 50;
    cfg.timeout_ms       = 1000;
    // Write the whole output area every cycle. The MGate's fault timeout only
    // counts Modbus accesses to the output block; writing only on change would
    // let it expire while the PC is healthy but idle.
    cfg.write_only_dirty = false;

    MGateDriver drv(cfg);

    IndraDriveConfig dcfg;
    dcfg.max_velocity_rpm = 50.0;  // commissioning cap; move_to() rejects anything faster
    IndraDrive drive(drv, dcfg);

    g_drive.store(&drive);
    std::signal(SIGINT, on_sigint);

    drv.start();
    if (!drv.wait_for_data(5000)) {
        std::printf("no data from gateway: %s\n", drv.stats().last_error.c_str());
        drv.stop();
        return 1;
    }

    int rc = 0;
    if (do_move) {
        rc = test_move(drive);
        if (rc != 0) std::printf("test move failed: %s\n", drive.last_error().c_str());
        std::printf("disabling...\n");
        drive.disable();
        std::printf("after:   %s\n", drive.describe().c_str());
    } else {
        rc = monitor(drv, drive);
    }

    // Control word 0 (drive off, parameter mode) and give it a few cycles to go out.
    drive.shutdown();
    sleep_ms(300);
    g_drive.store(nullptr);

    const DriverStats s = drv.stats();
    std::printf("cycles=%llu read_err=%llu write_err=%llu reconnects=%llu last_cycle=%lld us\n",
                static_cast<unsigned long long>(s.cycles),
                static_cast<unsigned long long>(s.read_errors),
                static_cast<unsigned long long>(s.write_errors),
                static_cast<unsigned long long>(s.reconnects), s.last_cycle_us);

    drv.stop();
    return rc;
}
