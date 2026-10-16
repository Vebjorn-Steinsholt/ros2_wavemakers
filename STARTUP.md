# Wavemaker startup checklist

Every lab session, before enabling the drive. It assumes commissioning is done
([COMMISSIONING.md](COMMISSIONING.md)); if anything below fails, stop and check the item named in
brackets there.

## Before launch

- [ ] Emergency stop is reachable and released. It is the only hardware stop; stops from the
      software (web page, bridge, Ctrl-C) do not remove drive power.
- [ ] Travel area is clear: nothing and nobody in reach of the flap, actuator or belt.
- [ ] Water depth measured. If the level has changed since the last session, reset the spring
      tension (item 34) and use the new depth at launch.
- [ ] Blue ropes: no new fraying or broken strands (item 34).
- [ ] Belt: tension and condition look normal (item 1).

## Launch

- [ ] If the water depth changed, set `WATER_DEPTH` with `sudoedit /etc/default/wavemaker` (see
      *Water depth* in the README).
- [ ] Start the service: `sudo systemctl start wavemaker@ladertanken` ([rpi/README.md](rpi/README.md)).
      Follow the log with `journalctl -u wavemaker@ladertanken -f`.
- [ ] Activation causes no motion.
- [ ] With the drive holding upright, motor torque or current in IndraWorks is close to zero
      (spring balance, item 34).
- [ ] Run `return_to_upright` and check the flap is visually upright (a jumped belt tooth shows as
      an offset, item 1).

## End of session

- [ ] Run `return_to_upright` and check the flap is visually upright.
- [ ] Stop the service (`sudo systemctl stop wavemaker@ladertanken`), so the controller disables
      the drive. If forgotten, it stops by itself after 2 h without a wave.
- [ ] Draining the tank? Release the spring tension first (item 34).
