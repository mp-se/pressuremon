.. _getting_started:

First readings
##############

After flashing and connecting the device to Wi-Fi, open its mDNS name or IP
address in a browser. The Home page shows sensor readings and a self-check for
configured and connected sensors, temperature probe status, push target setup,
and device state.

Pressure calibration
====================

Remove pressure from each connected sensor so it is exposed to normal
atmospheric pressure. Open **Device → Sensors**, select the correct sensor
type, then choose **Calibrate pressure**. The firmware averages several
readings per active sensor and stores a pressure offset (five for digital
XIDIBEI sensors and ten for analog sensors). Keep both sensors at
atmospheric pressure until calibration completes. Recalibrate after changing
a sensor or if its zero reading drifts.

Check the Home page for a pressure reading near zero after calibration. A
pressure sensor's specified full-scale range and wiring must match the
selected sensor type. PressureMon reports gauge pressure relative to the
calibrated atmosphere.

Check battery and sleep settings
================================

Open **Push targets → Settings** and choose a sleep interval appropriate for
the desired update frequency and battery life. Confirm that the Home page
voltage is plausible for the installed battery. If it is clearly wrong, check
the board's voltage divider and adjust **Voltage factor** under **Device →
Hardware**.

Connect a destination
=====================

Configure one target under **Push targets**, save it, and use its test button
where available. The Home page reports validation and recent delivery status.
See :doc:`services` for target-specific settings and :doc:`format-template`
for payload variables.

When configuration is complete, turn off the Home page force-config switch.
Then restart the device to resume scheduled measurements and sleep.
