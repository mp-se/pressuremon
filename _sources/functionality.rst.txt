.. _functionality:

How PressureMon works
#####################

PressureMon measures pressure and temperature during fermentation or storage.
Depending on the board and sensor setup, it can read one or two digital I2C
pressure sensors, an analog sensor through an ADS1115 converter, and a DS18B20
temperature probe. It forwards measurements to external services over Wi-Fi,
advertises data over BLE on supported boards, and sleeps between readings to
conserve battery.

Operating modes
===============

* **Configuration mode** keeps Wi-Fi and the web interface available so you
  can view readings and change settings. It uses more power than sleeping
  measurement mode.
* **Measurement mode** wakes for a measurement and configured data pushes,
  then enters deep sleep for the configured interval.
* **Wi-Fi setup mode** starts the ``PressureMon`` access point so network
  credentials can be entered.

The device stays awake for configuration when the Home page's force-config
switch is on, when no pressure sensor is configured, or when battery voltage
is above the configured charging threshold or below 2 V. On the supported
boards, connecting the designated TX and RX pins at startup also forces
configuration mode. The Wi-Fi access point is started when saved network
credentials are missing or the setup portal is triggered. See
:doc:`configuration` for the web interface controls and :doc:`hardware` for
board pin assignments.

Pressure and temperature
========================

The XIDIBEI XDB401 digital sensor communicates over I2C and reports pressure.
Choose its rated range in **Device → Sensors**; the configured range must
match the physical sensor. Temperature comes from the separate DS18B20 probe.
Pressure is represented internally in PSI and converted for display and
templates to PSI, Bar, or kPa.

An optional second pressure sensor is available on firmware builds that enable
it. The board may use a second hardware I2C bus or a software I2C bus. The
second reading is exposed separately in the Home page and templates.

The analog option uses an ADS1115 external ADC. The device maps configured
minimum and maximum voltage values to minimum and maximum pressure values. It
does not use the XIDIBEI digital I2C driver. The firmware configures a ±2.048 V
ADC range; check sensor output voltage against the ADC and board input limits
before connecting it.

DS18B20 temperature resolution trades conversion time and battery use for
precision. The supported settings are 9, 10, 11, and 12 bits. The temperature
adjustment is applied as an offset. See :doc:`configuration` for the settings
and :doc:`hardware` for sensor wiring context.

Battery and sleep
=================

The configured sleep interval is 10–3600 seconds. A shorter interval gives
more frequent readings but uses more battery, especially when Wi-Fi endpoints
are configured. The Home page's battery life estimate is approximate and uses
the platform, a nominal 2200 mAh battery, and a short assumed active runtime.
Actual life depends heavily on Wi-Fi connection time, signal strength, sensor
hardware, battery condition, and push destinations.

When battery saving is enabled and voltage is between 2.0 V and 3.73 V, the
effective sleep interval becomes one hour. The configured interval is used
again when voltage rises above that threshold. This helps preserve monitoring
continuity as the battery runs low.

Data delivery
=============

PressureMon has two HTTP POST targets, one HTTP GET target, InfluxDB v2, MQTT,
Bluetooth, and Wi-Fi Direct delivery to GravityMon Gateway. Wi-Fi targets can
be enabled together; each connection adds radio-on time. Wi-Fi Direct, when
enabled, is used in measurement mode instead of the normal network push
targets. BLE is configured as disabled or PressureMon iBeacon on supported
boards. See :doc:`services` and :doc:`format-template`.

Security note
=============

The firmware supports HTTPS for compatible HTTP targets. Verify certificate
handling and transport behavior against the current firmware and target
service before relying on server identity verification.
