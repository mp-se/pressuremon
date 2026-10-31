Custom firmware
===============

PressureMon uses PlatformIO and the Arduino framework. Most users should use
one of the supported environments in ``platformio.ini`` and edit settings
through the web interface. Custom builds are useful when changing hardware,
sensor ranges, or integrations.

Configuration values
--------------------

Build-time defaults and board-specific pin assignments are defined in the
firmware source and ``platformio.ini``. Review the target definitions before
changing pins: the ESP32-S2, ESP32-S3, and ESP32-C3 boards use different
assignments, and only the S3 and C3 targets support Bluetooth Low Energy.

For normal device settings, use the Device and Push configuration pages. See
:doc:`configuration` for descriptions of each field and supported options.

Adding hardware
---------------

When adding a sensor or changing an existing sensor, check its voltage,
interface, I2C address, and pressure range against the board and sensor data
sheet. The second pressure sensor is optional and is available on the targets
that expose its bus pins. The analog sensor input uses an ADS1115; keep input
voltage within the converter's specified range.

After changing hardware support, update the board documentation and pin table
in :doc:`hardware` as well as the code.

Building and testing
--------------------

Install PlatformIO, clone the repository with its submodules, and build the
target matching the board. The supported target names are listed in
``platformio.ini``. See :doc:`compiling` for the development setup and commands.

For UI work, start the mock server before the Vite development server. This
provides the API responses used by the simulator.
