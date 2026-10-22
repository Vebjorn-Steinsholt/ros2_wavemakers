#!/usr/bin/env python3
"""
Play MCL .dat wavemaker files (write2mcl.m format) as pregenerated move_wavemaker goals.

File format (write2mcl.m, "MCL format, first used for sloshing in 2003"):
  int32    time step in ms (rounded)
  int32    number of channels (always 6)
  float32  frames: channel 1..N of step 1, channel 1..N of step 2, ...
Little-endian (written on Windows with the native byte order). The format has no units:
generateRegular.m and generateIrregular.m write mm at the flap arm, relative to upright, in
channel 1, with the transfer function (Biesel, arm lever, mech) already applied. The values are
played as they are, without wavemaker_transfer_gain.

Regular and irregular files are handled alike: a goal is only positions at a fixed interval.
The file is split into runs where the paddle is at rest for at least --min-gap seconds; gaps may
differ in length, and shorter pauses stay inside the run. Runs are found from the file, not from
testinfo: generateRegular.m skips any test that exceeds maxFlap, so run N is not necessarily
test N. If a run table <file>.csv exists next to the .dat, it names the runs and gives their
boundaries.

Each run is one goal; the gaps are played as pauses. Ctrl-C cancels the current goal and stops
the programme. A rejected or aborted run stops it too. A goal can be long (an irregular run is
about 26 min) and has no heartbeat: Ctrl-C, the soft stop and the e-stop stop it.

The controller must run in pregenerated mode (terminal mode; the bridge only sends regular waves):
    ros2 launch wavemaker_bringup wavemakers.launch.py wavemaker:=ladertanken \
      mode_pregenerated:=true
Without hardware:
    ros2 run rig_controller fake_controller --pregenerated \
      --ros-args -r __ns:=/wavemakers/ladertanken

    ros2 run rig_controller play_dat 2002.dat --min -0.106 --max 0.106 \
      --max-velocity 0.54 --dry-run
    ros2 run rig_controller play_dat 2002.dat --min -0.106 --max 0.106 \
      --max-velocity 0.54 --runs 3-5 --ros-args -r __ns:=/wavemakers/ladertanken
"""

import argparse
from dataclasses import dataclass, field
import os
import sys

import numpy as np

HEADER_BYTES = 8           # int32 time step [ms], int32 channel count
MIN_GAP_S = 1.0            # rest at least this long separates two runs
REST_SPEED = 1e-3          # file units per s; slower than this (and near zero) counts as rest
SHORT_PAUSE_REPORT_S = 0.1  # rest shorter than MIN_GAP_S but longer than this is reported
UPRIGHT_TOLERANCE_M = 0.002  # a run should start and end this close to upright (blend tolerance)
CLASSIFY_SPREAD = 0.02     # zero-crossing spacing std / mean below this -> regular
UNIT_SCALE = {'mm': 1e-3, 'm': 1.0}
SERVER_TIMEOUT_S = 5.0


# ---------------------------------------------------------------- file handling (no ROS)

@dataclass
class McLFile:
    sample_interval: float   # s, from the header
    channels: int
    data: np.ndarray         # frames x channels, float32, as in the file


@dataclass
class Run:
    index: int               # 0-based, as in the run table
    start: int               # first frame in the file
    values: np.ndarray       # file units, relative to upright (for the table and checks)
    positions: np.ndarray    # absolute positions in m at the flap attachment point
    gap_before_s: float      # measured rest before this run, played as a pause
    kind: str = 'unknown'    # 'regular' | 'irregular', from the run table or classify_run
    info: dict = field(default_factory=dict)  # run table row (T/steep/Hw or Tp/Hs/gamma/seed)
    short_pauses: list = field(default_factory=list)  # (start_s, length_s) kept inside the run


def read_mcl(path) -> McLFile:
    """Read a write2mcl file; raise ValueError if it is not one."""
    file_size = os.path.getsize(path)
    if (file_size < HEADER_BYTES):
        raise ValueError(f'{path}: {file_size} bytes. File is too small to contain a valid header')
    interval_ms, channels = (int(v) for v in np.fromfile(path, dtype='<i4', count=2))
    frames, lefover = divmod(os.path.getsize(path) - HEADER_BYTES, channels * 4)
    if (interval_ms <= 0):
        raise ValueError(f'{path}: interval_ms must be positive, got {interval_ms}')
    if (channels <= 0):
        raise ValueError(f'{path}: channels must be positive, got {channels}')
    if (file_size - HEADER_BYTES) % (channels * 4) != 0:
        raise ValueError('File size does not match header information')

    # TODO 4: body as '<f4', reshape to (frames, channels).
    # TODO 5: reject non-finite values.
    raise NotImplementedError


def select_channel(mcl: McLFile, channel: int) -> np.ndarray:
    """One channel as float64, 1-based as in MATLAB (Data(:,1) is channel 1)."""
    # TODO: check 1 <= channel <= mcl.channels.
    # TODO: raise if the channel is all zeros (wrong channel?).
    raise NotImplementedError


def split_runs(values: np.ndarray, sample_interval: float, scale: float, upright: float,
               gap_threshold: float = 0.0, min_gap_s: float = MIN_GAP_S) -> list[Run]:
    """
    Split only where the paddle is at rest for at least min_gap_s; gaps may differ in length.

    At rest: |value| <= gap_threshold (file units) and |velocity| <= REST_SPEED, so a slow pass
    through zero inside an irregular run is never taken for a gap. When in doubt, keep it in the
    run: playing a pause inside a goal is harmless, cutting a goal while moving is not.
    """
    # TODO: rest = (abs(values) <= gap_threshold) & (abs(gradient(values)) / dt <= REST_SPEED).
    # TODO: rest stretches >= min_gap_s / dt samples are gaps; shorter ones stay in the run,
    #       and those longer than SHORT_PAUSE_REPORT_S go in run.short_pauses.
    # TODO: keep one rest sample at each end, so every run starts and ends at rest.
    # TODO: positions = upright + run_values * scale.
    # TODO: gap_before_s = measured length of the rest before each run (the first one includes
    #       the leading zeros).
    raise NotImplementedError


def whole_file_run(values: np.ndarray, scale: float, upright: float) -> list[Run]:
    """--no-split: the whole file as one run (fallback for files whose gaps can't be detected)."""
    # TODO
    raise NotImplementedError


def read_run_table(dat_path) -> list[dict] | None:
    """Rows of <file>.csv: required run, start_s, end_s; optional kind and any test parameters."""
    # TODO: csv.DictReader; None if the file does not exist.
    # TODO: check the required columns; raise ValueError on a malformed table.
    raise NotImplementedError


def apply_run_table(runs: list[Run], table: list[dict], values: np.ndarray,
                    sample_interval: float, gap_threshold: float) -> list[Run]:
    """Use the table's boundaries and names; refuse if they do not fit the file."""
    # TODO: every listed boundary must fall where the paddle is at rest (same rule as
    #       split_runs); otherwise raise: the table belongs to another file or version.
    # TODO: compare with the detected runs; report differences in the dry run.
    # TODO: fill kind and info from the table rows.
    raise NotImplementedError


def classify_run(run: Run, sample_interval: float) -> str:
    """'regular' if the zero-crossing spacing is constant, else 'irregular'."""
    # TODO: upward zero crossings of the middle part (skip the ramps, e.g. the first and last
    #       15 % of the run); std(spacing) / mean(spacing) < CLASSIFY_SPREAD -> regular.
    raise NotImplementedError


def describe_run(run: Run, sample_interval: float) -> dict:
    """Numbers for the table: the common ones plus the ones that make sense for the kind."""
    # TODO common: start [s], duration [s], gap before [s], max |value| (file units),
    #       peak speed [m/s], number of short pauses.
    # TODO regular: period from the zero-crossing spacing, amplitude.
    # TODO irregular: Tp from the FFT peak of the middle part; 4 * std as the significant flap
    #       excursion (not the wave Hs: that is in the run table, or comes from the probes).
    # TODO: table values win; mark each number 'table' or 'measured'.
    raise NotImplementedError


def check_run(run: Run, sample_interval: float, minimum: float, maximum: float,
              max_velocity: float) -> list[str]:
    """Reasons the controller would reject this run, or it should not be played; empty if fine."""
    # TODO: positions.min() / max() against minimum / maximum.
    # TODO: peak speed = max |diff(positions)| / sample_interval against max_velocity.
    # TODO: first and last position within UPRIGHT_TOLERANCE_M of upright (otherwise the run
    #       blends in at the start or leaves the paddle off upright between runs).
    raise NotImplementedError


def print_table(runs: list[Run], sample_interval: float, units: str,
                problems: dict[int, list[str]]):
    """
    One line per run, with OK or the problems; short pauses listed under their run.

    Showing the amplitude in file units next to metres makes a unit mixup obvious
    (±47.9 is mm, ±0.0479 would be m).
    """
    # TODO
    raise NotImplementedError


# ---------------------------------------------------------------- ROS (goals)

def play(runs: list[Run], sample_interval: float, ros_args: list[str]) -> int:
    """Send each run as a move_wavemaker goal, in order; return an exit code."""
    # TODO 1: rclpy.init(args=ros_args); node 'play_dat';
    #         ActionClient(node, MoveRig, f'move_{rig}') (namespace via --ros-args;
    #         add --rig wavemaker|oscillator, default wavemaker, as in fake_controller).
    # TODO 2: wait_for_server(SERVER_TIMEOUT_S); fail clearly if the controller is not up.
    # TODO 3: for each run:
    #           - pause gap_before_s (interruptible by Ctrl-C)
    #           - log "run N (kind): duration, amplitude, peak speed"
    #           - goal.positions = run.positions.tolist(); goal.sample_interval = sample_interval
    #           - send_goal_async, spin until accepted; a rejection stops the programme with
    #             "goal rejected, see the controller log" (the reason is not sent to clients:
    #             wrong mode, outside limits, too fast, busy, not active)
    #           - get_result_async, spin until done; stop unless status SUCCEEDED
    # TODO 4: KeyboardInterrupt: cancel_goal_async on the current goal, wait briefly, exit 130.
    # TODO 5: finally: destroy the node, rclpy.try_shutdown().
    raise NotImplementedError


# ---------------------------------------------------------------- command line

def parse_runs(text: str, count: int) -> list[int]:
    """'3', '3-5', '3,7,9' -> run indices (0-based, as in the table); '' -> all runs."""
    # TODO: check every index < count.
    raise NotImplementedError


def main(argv=None):
    from rclpy.utilities import remove_ros_args
    argv = sys.argv if argv is None else argv
    parser = argparse.ArgumentParser(description='Play MCL .dat files as pregenerated waves.')
    parser.add_argument('file')
    parser.add_argument('--channel', type=int, default=1, help='1-based, default 1 (X)')
    parser.add_argument('--units', choices=UNIT_SCALE, default='mm',
                        help='units of the values in the file (default mm)')
    parser.add_argument('--upright', type=float, default=0.0,
                        help='upright_position_m of the target wavemaker [m]')
    parser.add_argument('--runs', default='', help="e.g. '3', '3-5', '3,7'; default all")
    parser.add_argument('--gap-threshold', type=float, default=0.0,
                        help='|value| at or below this (file units) counts as rest; '
                             'default exact zeros')
    parser.add_argument('--min-gap', type=float, default=MIN_GAP_S,
                        help='shortest rest [s] that separates two runs')
    parser.add_argument('--no-split', action='store_true',
                        help='send the whole file as one goal')
    parser.add_argument('--min', type=float, required=True, help='lowest position [m]')
    parser.add_argument('--max', type=float, required=True, help='highest position [m]')
    parser.add_argument('--max-velocity', type=float, required=True, help='[m/s]')
    parser.add_argument('--dry-run', action='store_true', help='print the run table only')
    args = parser.parse_args(remove_ros_args(argv)[1:])  # noqa: F841, used by the TODOs below

    # TODO: mcl = read_mcl(args.file)                       (ValueError -> message, exit 2)
    # TODO: values = select_channel(mcl, args.channel); scale = UNIT_SCALE[args.units]
    # TODO: runs = whole_file_run(...) if args.no_split else split_runs(...)
    # TODO: table = read_run_table(args.file)
    #       if table: runs = apply_run_table(runs, table, values, ...)
    #       else: print "no run table; kinds and periods are measured"
    # TODO: for runs without a kind from the table: run.kind = classify_run(run, dt)
    # TODO: selected = [runs[i] for i in parse_runs(args.runs, len(runs))]
    # TODO: problems = {r.index: check_run(r, ...) for r in selected}
    # TODO: print_table(selected, mcl.sample_interval, args.units, problems)
    # TODO: if args.dry_run: return 0;  if any(problems.values()): return 1 (nothing moved)
    # TODO: return play(selected, mcl.sample_interval, argv)
    return 0


if __name__ == '__main__':
    sys.exit(main())
