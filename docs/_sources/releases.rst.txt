.. _releases:

Releases 
########

v0.6.0
======

Known limitation:
* On the ESP32c3 and ESP32s2 the software only supports one I2C bus and one sensor since those boards 
only have one I2C bus. I need to do some refactoring to add support for multiple senors.


New features
++++++++++++
* Added support for 0.96" OLED display connected to first I2C bus or connector on ESP32C3-PICO.
* Enabled support for second pressure sensor on I2C bus #2
* Enabled analog sensor support using an external ADS1115 ADC chip. This should be conneected to the first I2C bus.

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

