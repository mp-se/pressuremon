.. _installation:

Installation and Wi-Fi setup
############################

Choose firmware for the exact board. The standard builds are for the ESP32-C3
Pico, ESP32-S2 Mini, and ESP32-S3 Mini. Board support and optional features
differ; check :doc:`hardware` before flashing.

Initial flashing
================

Use the `GravityMon Web Flasher <https://gravitymon.com/flasher/index.html>`_
to install PressureMon. Select the PressureMon product and the matching board
variant, then follow the flasher's instructions. Full flashing erases the
device. The flasher can also help enter Wi-Fi credentials after installation.

If flashing manually, use the matching firmware and partition files from the
project's ``bin`` directory. The ESP32-C3, S2, and S3 builds have different
images; never flash a binary solely because it has the same filename prefix.

First Wi-Fi setup
=================

When no Wi-Fi configuration exists, the device starts an access point named
``PressureMon`` with password ``password``. Connect to it and open
``http://192.168.4.1``. Choose a 2.4 GHz Wi-Fi network under **Device → Wifi**,
enter its password, save, and restart. The device then joins the configured
network.

If Wi-Fi was configured previously but you need to enter setup again, tap the
reset button twice in quick succession to trigger the double-reset setup
sequence. Timing can vary with the board. A hidden SSID can be entered from the
manual network entry link on the Wi-Fi page.

Find the device
===============

After connecting to Wi-Fi, open the configured mDNS address
``<name>.local`` or the device IP address shown on Home. If mDNS does not work
on your computer, use the router's client list or connect a serial monitor at
115200 baud to read startup logs and the assigned address.

First configuration
===================

1. Open **Device → Settings** and set a unique mDNS name and display units.
2. Under **Device → Hardware**, confirm battery type, voltage readings, and
   configuration voltage threshold.
3. Under **Device → Sensors**, select the pressure sensor range and temperature
   resolution. Set up a second pressure sensor if the firmware exposes it.
4. Leave the pressure sensor unpressurized and run **Calibrate pressure**.
5. Under **Push targets → Settings**, set an appropriate sleep interval and
   configure destinations under **Push targets**.
6. Review the Home page self-checks, battery reading, pressure, temperature,
   and push status.

To keep the web interface awake, turn on **Force config mode** on Home or
connect external power. See :doc:`functionality` for power behavior.

Firmware updates
================

For a routine update while the device is awake, open **Other → Firmware
update**, choose the firmware file matching the detected board, and start the
upload. Keep power connected until the device restarts. Use the web flasher for
initial flashing, recovery, or a full reinstall.
