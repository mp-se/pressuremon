.. _releases:

Releases 
########

v0.6.0
======

New features
++++++++++++
* Added format templates for second pressure sensor
* Added support for 0.96" OLED display connected to first I2C bus or connector on ESP32C3-PICO.
* Enabled support for second pressure sensor on I2C bus #2
* Enabled analog sensor support using an external ADS1115 ADC chip. This should be conneected to the first I2C bus.
* Fixed LED enable on c3 pico board

Bugs fixed
++++++++++
* Fixed conversion to kPa on device.

v0.5.0
======

Version based on the new hardware design for testing and evaulation. 

New features
++++++++++++
* Support for XIDIBEI digital I2C sensors and external temperature sensor DS18B20.
* New userinterface based on VueJS.
* Shared codebase with Gravitymon for easier maintenance.
* Format editor will now validate json payloads for http post and mqtt when selecting preview. 

Bugs fixed
++++++++++
* None

