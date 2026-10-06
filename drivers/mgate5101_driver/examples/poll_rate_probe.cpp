// poll_rate_probe.cpp - find the shortest usable poll_interval_ms for the MGate 5101-PBM-MN.
//
// For each candidate interval this runs the real MGateDriver cycle (write outputs, read inputs,
// read status) for a while and reports how long a cycle takes, the period actually achieved,
// overruns, errors, and how often the input data changes. It then recommends the shortest
// interval with margin.
//
//   poll_rate_probe [--host 192.168.1.70] [--port 502] [--seconds 10]
//                   [--intervals 50,40,30,20,15,10,5,2]
//
// SAFETY: the output area is written as all zeros every cycle. That is control word 0 (drive
// off, parameter mode). Run it with the wavemaker controller stopped and the drive disabled,
// because it would switch off a drive another program has enabled.
//
// The layout matches wavemaker_bringup/config/wavemakers.yaml (ladertanken).

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "mgate/mgate_driver.h"

using namespace mgate;
using Clock = std::chrono::steady_clock;

namespace {

struct Options {
    std::string      host    = "192.168.1.70";
    int              port    = 502;
    int              seconds = 10;
    std::vector<int> intervals_ms{50, 40, 30, 20, 15, 10, 5, 2};
};

struct Result_ {
    int         interval_ms = 0;
    std::size_t cycles      = 0;
    std::size_t failed      = 0;
    std::size_t overruns    = 0;  // cycle took at least the interval
    std::size_t input_changes = 0;
    double      cycle_p50_us = 0, cycle_p99_us = 0, cycle_max_us = 0;
    double      period_mean_ms = 0, period_max_ms = 0;
    std::uint64_t read_errors = 0, write_errors = 0, reconnects = 0;
    bool        connected = false;
};

double percentile(std::vector<double> v, double p) {
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    const auto i = static_cast<std::size_t>(p * static_cast<double>(v.size() - 1) + 0.5);
    return v[std::min(i, v.size() - 1)];
}

GatewayConfig make_config(const Options& opt, int interval_ms) {
    GatewayConfig cfg;
    cfg.host              = opt.host;
    cfg.port              = static_cast<std::uint16_t>(opt.port);
    cfg.unit_id           = 1;
    cfg.input_base_reg    = 0x0000;
    cfg.input_word_count  = 7;
    cfg.output_base_reg   = 0x0000;
    cfg.output_word_count = 6;
    cfg.input_uses_fc4    = true;
    cfg.status_base_reg   = 0x0300;  // gateway status word + PROFIBUS live list
    cfg.status_word_count = 9;
    cfg.poll_interval_ms  = interval_ms;
    cfg.timeout_ms        = 1000;
    cfg.reconnect_ms      = 500;
    cfg.write_only_dirty  = false;  // as in production: the whole output area every cycle
    return cfg;
}

Result_ probe(const Options& opt, int interval_ms) {
    Result_ r;
    r.interval_ms = interval_ms;

    MGateDriver drv(make_config(opt, interval_ms));

    std::mutex                mtx;
    std::vector<double>       cycle_us;
    std::vector<double>       period_ms;
    Clock::time_point         last_cb{};
    std::vector<std::uint8_t> last_inputs;
    std::atomic<bool>         recording{false};

    drv.set_cycle_callback([&](bool ok, const Result&) {
        if (!recording.load()) return;
        const auto now = Clock::now();
        const auto s   = drv.stats();
        std::lock_guard<std::mutex> lk(mtx);
        ++r.cycles;
        if (!ok) {
            ++r.failed;
        } else {
            cycle_us.push_back(static_cast<double>(s.last_cycle_us));
            if (s.last_cycle_us >= static_cast<long long>(interval_ms) * 1000) ++r.overruns;
            auto inputs = drv.input_snapshot();
            if (!last_inputs.empty() && inputs != last_inputs) ++r.input_changes;
            last_inputs = std::move(inputs);
        }
        if (last_cb != Clock::time_point{}) {
            period_ms.push_back(std::chrono::duration<double, std::milli>(now - last_cb).count());
        }
        last_cb = now;
    });

    drv.start();
    r.connected = drv.wait_for_data(5000);
    if (!r.connected) {
        std::printf("  %3d ms: no data from %s (%s)\n", interval_ms, opt.host.c_str(),
                    drv.stats().last_error.c_str());
        drv.stop();
        return r;
    }

    const DriverStats before = drv.stats();
    recording.store(true);
    std::this_thread::sleep_for(std::chrono::seconds(opt.seconds));
    recording.store(false);
    const DriverStats after = drv.stats();
    drv.stop();

    r.read_errors  = after.read_errors - before.read_errors;
    r.write_errors = after.write_errors - before.write_errors;
    r.reconnects   = after.reconnects - before.reconnects;

    std::lock_guard<std::mutex> lk(mtx);
    r.cycle_p50_us = percentile(cycle_us, 0.50);
    r.cycle_p99_us = percentile(cycle_us, 0.99);
    r.cycle_max_us = cycle_us.empty() ? 0.0 : *std::max_element(cycle_us.begin(), cycle_us.end());
    if (!period_ms.empty()) {
        double sum = 0.0;
        for (double p : period_ms) sum += p;
        r.period_mean_ms = sum / static_cast<double>(period_ms.size());
        r.period_max_ms  = *std::max_element(period_ms.begin(), period_ms.end());
    }
    return r;
}

// Usable: no errors or reconnects, no overruns, and the slowest 1 % of cycles still leave
// 20 % of the interval free. IndraDriveActuator::start() refuses to activate if a cycle
// takes as long as the interval, so the margin keeps activation reliable.
bool usable(const Result_& r) {
    return r.connected && r.failed == 0 && r.read_errors == 0 && r.write_errors == 0 &&
           r.reconnects == 0 && r.overruns == 0 &&
           r.cycle_p99_us <= 0.8 * r.interval_ms * 1000.0;
}

bool parse(int argc, char** argv, Options& opt) {
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        const bool has_value = i + 1 < argc;
        if (a == "--host" && has_value) {
            opt.host = argv[++i];
        } else if (a == "--port" && has_value) {
            opt.port = std::atoi(argv[++i]);
        } else if (a == "--seconds" && has_value) {
            opt.seconds = std::max(1, std::atoi(argv[++i]));
        } else if (a == "--intervals" && has_value) {
            opt.intervals_ms.clear();
            std::stringstream ss(argv[++i]);
            std::string item;
            while (std::getline(ss, item, ',')) {
                const int v = std::atoi(item.c_str());
                if (v > 0) opt.intervals_ms.push_back(v);
            }
        } else {
            std::printf("usage: %s [--host IP] [--port N] [--seconds N] [--intervals 50,20,10]\n", argv[0]);
            return false;
        }
    }
    return !opt.intervals_ms.empty();
}

}  // namespace

int main(int argc, char** argv) {
    Options opt;
    if (!parse(argc, argv, opt)) return 2;

    std::printf("MGate poll-rate probe: %s, %d s per interval\n", opt.host.c_str(), opt.seconds);
    std::printf("Outputs are written as zeros (control word 0): run with the controller stopped.\n\n");

    std::vector<Result_> results;
    for (int interval : opt.intervals_ms) {
        std::printf("probing %d ms...\n", interval);
        std::fflush(stdout);
        results.push_back(probe(opt, interval));
    }

    std::printf("\n%8s %7s %6s %8s %10s %10s %10s %10s %10s %8s %6s %6s %s\n", "interval",
                "cycles", "failed", "overrun", "cycle p50", "cycle p99", "cycle max", "period",
                "period max", "in chg/s", "r/w err", "reconn", "usable");
    for (const auto& r : results) {
        if (!r.connected) {
            std::printf("%6d ms  (no connection)\n", r.interval_ms);
            continue;
        }
        std::printf("%6d ms %7zu %6zu %8zu %8.0f us %8.0f us %8.0f us %7.1f ms %7.1f ms %8.1f %3llu/%-3llu %6llu %s\n",
                    r.interval_ms, r.cycles, r.failed, r.overruns, r.cycle_p50_us, r.cycle_p99_us,
                    r.cycle_max_us, r.period_mean_ms, r.period_max_ms,
                    static_cast<double>(r.input_changes) / opt.seconds,
                    static_cast<unsigned long long>(r.read_errors),
                    static_cast<unsigned long long>(r.write_errors),
                    static_cast<unsigned long long>(r.reconnects), usable(r) ? "yes" : "no");
    }

    const Result_* best = nullptr;
    for (const auto& r : results) {
        if (usable(r) && (!best || r.interval_ms < best->interval_ms)) best = &r;
    }
    std::printf("\n");
    if (best) {
        std::printf("Shortest usable interval: %d ms (cycle p99 %.0f us). Set poll_interval_ms to\n"
                    "this or the next longer usable value, and keep the MGate output fault timeout\n"
                    "at several times it.\n",
                    best->interval_ms, best->cycle_p99_us);
    } else {
        std::printf("No interval was usable; check the connection and the errors above.\n");
    }
    std::printf("'in chg/s' near the cycle rate means the gateway refreshes inputs at least that\n"
                "fast; far below it means the inputs (or the drive's values) change more slowly.\n");
    return best ? 0 : 1;
}
