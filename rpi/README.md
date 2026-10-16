# Running a lab Pi as a service

`wavemaker@.service` runs `lab.launch.py` (controller, bridge, lifecycle manager, soft stop and
web page) under systemd. Unlike a launch in an SSH session, it keeps running when the connection
drops, logs to the journal, and is started and stopped the same way on every Pi.

It is a template unit: the part after `@` is the wavemaker, which is also the Pi's user
(`wavemaker@ladertanken` runs as user `ladertanken`; see *Lab computers* in the top-level README).

| File | Purpose |
|---|---|
| `wavemaker@.service` | The unit. Installed as `/etc/systemd/system/wavemaker@.service`. |
| `run.sh` | `ExecStart`: sources ROS and the workspace, then `exec`s `lab.launch.py`. |
| `stop.sh` | `ExecStop`: asks the lifecycle manager to shut the nodes down before the launch is stopped. |

> **The service never starts at boot.** The unit has no `[Install]` section, so it cannot be
> enabled: the drive is enabled as soon as `lab.launch.py` runs, and a power cut or reboot must
> not do that by itself.

## Install (once per Pi)

Build the workspace first (see *Build* in the top-level README). Then, as the wavemaker user:

```bash
sudo cp ~/ros_ws/src/ros2_wavemakers/rpi/wavemaker@.service /etc/systemd/system/
sudo systemctl daemon-reload
systemd-analyze verify wavemaker@ladertanken.service
```

Create `/etc/default/wavemaker` with `sudoedit /etc/default/wavemaker`:

```bash
ROS_DOMAIN_ID=1        # the lab's domain; required, and not 0
PORT=8080
WATER_DEPTH=           # m; empty uses wavemakers.yaml
```

systemd does not read `~/.bashrc`, so `ROS_DOMAIN_ID` must be set here. `run.sh` refuses to start
without it, because on domain 0 anyone on the network could command the wavemaker. The settings
are read at start; after a change, stop and start the service.

To start and stop without a password, add a sudoers rule with
`sudo visudo -f /etc/sudoers.d/wavemaker` (always through `visudo`: a syntax error breaks sudo):

```
ladertanken ALL=(root) NOPASSWD: /usr/bin/systemctl start wavemaker@ladertanken, /usr/bin/systemctl stop wavemaker@ladertanken
```

## Use

```bash
sudo systemctl start wavemaker@ladertanken     # enables the drive
sudo systemctl stop wavemaker@ladertanken      # stops the wave and disables the drive
systemctl status wavemaker@ladertanken
journalctl -u wavemaker@ladertanken -f         # follow the log; -b for this boot only
```

Type the commands as shown; the sudoers rule matches them exactly.

## How it stops

`systemctl stop`:

1. `stop.sh` calls the lifecycle manager's `manage_nodes` with `SHUTDOWN`, which deactivates,
   cleans up and shuts down the bridge and then the controller; the controller disables the drive.
   The call has a 15 s timeout.
2. systemd then sends SIGINT to `ros2 launch` (`KillMode=mixed`), which forwards it to the
   remaining processes. If step 1 failed, this alone still stops the drive: the controller stops
   it when it receives SIGINT.
3. `ros2 launch` escalates to SIGTERM after 20 s (`run.sh` sets `sigterm_timeout`), and systemd
   kills whatever is left after `TimeoutStopSec`.

**Idle auto-stop.** After 2 h without a running wave, the web server does step 1 itself and exits,
and the launch shuts down with it; the service then ends on its own (`inactive`). The page warns 10
minutes before, with a **Keep running** button. See *Web page* in the top-level README.

The service is never restarted automatically (`Restart=no`): a restart would re-enable the drive.
If a node dies, `ros2 launch` keeps running with the rest. Check the journal, then stop and start
the service.

## Rig test

1. Start the service, then run a wave from the web page.
2. `sudo systemctl stop wavemaker@ladertanken` mid-wave. The wave stops and the drive is disabled.
   The journal shows the lifecycle manager's `Deactivate, cleanup, and shutdown nodes`, then a
   clean exit of every process.
3. Make `stop.sh` fail (for example `exit 1` at the top), start, and stop again: the drive must
   still be disabled, through SIGINT.
4. Leave the service idle with a short timeout (temporarily `idle_timeout_s` in `web.launch.py`):
   the warning appears, Keep running restarts the count, and at the timeout the service ends.
5. Reboot the Pi. `systemctl status wavemaker@ladertanken` reports `inactive (dead)`.
