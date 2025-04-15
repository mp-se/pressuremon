.. _hardware:

Hardware
########

To build the hardware for this project you will need the following. 

* A microcontroller ESP32c3 ESP32c3 pico. 
* A digital pressure sensor from the brand XIDIBEI (see aliexpress.com), select one that matches the range you want to measure.
* 2.2k resistors for the I2C, SDA and SCL lines pullup
* PCB for mounting the components (TODO: add link to pcbway)

Optional:
* A analog pressure sensor from the brand XIDIBEI (see aliexpress.com) and an external ADC ADS1115
* Lithium battery compatible with the ESP32c3 pico (check the battery matches the connector, +/- pins)
* DS18B20 temperature sensor (Needed for temperature data since the internal sensor is not accurate enough)
* 3.3k resistors for the temperature sensor data pullup

.. note:: 

  ESP32c3 mini, ESP32s2 mini or ESP32s3 mini will also work but will require additional hardware such as LiPo 
  charger and voltage divider for measuring battery. (The s2 does not support bluetooth). If you want to use 
  any of these boards the pinout is the same. 


Custom boards
=============

Besides the standard boards listed here (which are tested and verified by me) there is also a possibility 
to add custom boards which are included in the builds and made available through this gihub repository. 
A list of these boards can be found here :ref:`custom-boards`. I cannot take full responsibility for these 
builds since I dont have access to these boards for testing.


Schema for ESP32c3 pico
+++++++++++++++++++++++

.. image:: images/schema_esp32c3pico.png
  :width: 700
  :alt: Schema ESP32c3 pico

Building a device
+++++++++++++++++

Work in progress
