.. _installation:

Software Installation 
---------------------

Webflasher
==========

The preferred option for flashing PressureMon is using my WebFlasher from gravitymon.com, its a tools that support all my brewing related firmwares. This works 
on both Windows and Mac. The flasher is available here `WebFlasher <https://gravitymon.com/flasher/install.html>`_. After flashing is complete you can also 
set the WiFi ssid and password through the tool which is easier than connecting to the access point.

Binaries
********

In the /bin directory you will find 3 different firmware builds;

* **firmware32c3.bin**

  This is the release build for an ESP32c3-mini or ESP32c3-pico. When flashing an ESP32 you also need the *partition32c3.bin* file that outlines the flash memory structure. Due to 
  the size of the firmware we are using a custom partition setup. 

* **firmware32s2.bin**

  This is the release build for an ESP32s2-mini variant. When flashing an ESP32 you also need the *partition32s2.bin* file that outlines the flash memory structure. Due to 
  the size of the firmware we are using a custom partition setup.

* **firmware32s3.bin**

  This is the release build for an ESP32s3-mini variant. When flashing an ESP32 you also need the *partition32s3.bin* file that outlines the flash memory structure. Due to 
  the size of the firmware we are using a custom partition setup.


Updating firmware 
=================

You can use the above options for updating the firmware as well. But if you want to do updates without connecting the USB cable these 
are your options.

Manual update
*************

When the device in is configuration mode you can manually update with a new firmware. Just open navigate to the UI and upload the correct the firmware for your device.


.. _serial_monitoring:

Serial Monitoring
=================

To check output from the device (logs) there are several tools out there. I found this simple tool in the Windows Store called ``Serial Port Monitoring``. 
Just select a baud rate of 115200, 8N1.

.. _setup_wifi:

WIFI compatibility
==================

The esp has limited wifi support and still relies on old standards so these are a few things to consider on the wifi setup.

* Don't use space in your wifi SSID or wifi password
* Only supports 2.4Ghz bands, channels must be 1-13, other channels are not supported (In 802.11 b/g/n modes)
* Channels between 20-25 Mhz bands
* SSID must be visible (hidden SSID dont work)

Source: https://www.espressif.com/sites/default/files/esp8266_wi-fi_channel_selection_guidelines.pdf

Configuring WIFI
================

When the device is flashed it will need to have WIFI configuration in order to work. If you have used other software on 
the device its possible that wifi settings already exist.

If you need to enter the WIFI setup mode after it has been configured, tap the reset button at least 3 times. Tap the reset 
button in 1-2 second intervals. Not to fast and not to slow. 

If this is not configured in the device it will create an wireless access point called `PressureMon`. The default password is `password`.

Connect to this AP and enter the SSID and password you want to use. If the web page don't open automatically you can enter the following address 
in the browser: **http://192.168.4.1**

WIFI settings can be found under Device - WIFI in the menu.

.. _setup_ip:

Finding the device adress
=========================

Once the wifi network settings have been added then the device will reboot and connect to your network. If the blue led is flashing then it's up and running and is ready to be configured. 

If your computer supports mDNS the address you saw before can be used in your web browser to connect to the device. Windows does not have the best support for mDNS so if you are having issues 
with finding the network name you can try the following:

* Check your wireless router for the IP address and use that to connect instead, for example; http://192.168.1.56
* Download an IP scanner / Port Scanner on your Windows computer or mobile device and use that to find what devices are listening on port 80.

Once you can access the user interface then proceed to the next step.
