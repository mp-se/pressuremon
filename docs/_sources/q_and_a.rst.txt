Questions and answers
=====================

Why does the device return to setup mode?
-----------------------------------------

PressureMon starts its setup access point when it needs configuration, when
measurement sensors are disabled, or when the configured battery voltage is
outside the accepted range. Check the battery and sensor settings, then
reconnect to the device access point. The default access point password is
``password`` and the setup page is at ``192.168.4.1``.

Why is Bluetooth missing?
-------------------------

Bluetooth is available on the ESP32-S3 and ESP32-C3 builds. The ESP32-S2 Mini
does not provide BLE support in this firmware.

Can I connect two pressure sensors?
-----------------------------------

Some supported boards expose a second I2C bus for an optional second sensor.
Check the board table in :doc:`hardware`; availability and bus implementation
differ by target.

How do I restore the device after changing Wi-Fi settings?
----------------------------------------------------------

Use the device setup access point and configure Wi-Fi again. If necessary,
force configuration mode from the Home page or restart the device while its
configuration condition is present.

How do I update firmware?
-------------------------

Use the PressureMon flasher linked from the `GravityMon website
<https://gravitymon.com/>`_. Select the build for the exact board and follow
the instructions shown by the flasher. The web interface also supports OTA
updates when the device is already reachable on the network.
