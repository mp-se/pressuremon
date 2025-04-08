.. _getting_started:

Getting started
===============

First you need a completed hardware build.

Step 1 - Flash the device
-------------------------

The first step is to flash the firmware, I recommend using Brewflasher as the easy option. Detailed 
instructions can be found here :ref:`installation`

Step 2 - Setup WIFI & Configuration
-----------------------------------

When the device starts up the first time it will first start an WIFI access point so that the WIFI Settings 
can be configured. The wifi settings can be found under the Device - WIFI menu.
  
The user interface does no longer require internet access for most functions, the exception is the graphs which 
require access to chartjs in order to render properly. This is due to lack of memory on the device.

The device will go into `configuration mode` and start a web server for doing the initial configuration. 
In order to access the device you will need to find its name or ip address.

It will broadcast a name like pressureymonXXXXXX.local over mDNS. Where the XXXXXX is the unique device id. You can 
find the name via an mDNS browser, check your router or connect the device to a serial monitor. On windows mDNS 
might not work so then use the IP address instead. Once connected you will meet a web interface that looks like this.

.. image:: images/ui-home.png
  :width: 800
  :alt: Index page

In order for the software to work properly there are a few steps that needs to be configured. When you open up the
index page a selftest will be run and common issues will be shown for your action.

Configuration - Device Settings - Device Name
+++++++++++++++++++++++++++++++++++++++++++++

Give your device a good name.

Configuration - Device Settings - Sensor Calibration
++++++++++++++++++++++++++++++++++++++++++++++++++++

You need to have the sensors without any pressure attached and press the calibrate button. It will take a few seconds 
for this to complete. Calibration will secure that the sensor is showing values compared to the normal atmospheric pressure.

Configuration - Push Settings
+++++++++++++++++++++++++++++
Add the endpoints where you want data to be transmitted. All URLs that contain a valid endpoint will receive the data.

Check battery voltage
+++++++++++++++++++++

Check the battery voltage so the value is correctly measured. If the wrong resistor values in the
voltage are then the calculation is off. This could block the device from going to sleep properly or never go into configuration mode.   

Step 4 - Completed
------------------

You are now done and can enjoy the GravityMon software. Check out the :ref:`setting-up-device` section for other configuration options.

If you want to enter the configuration mode place the device flat on a surface and do a reset (or wait until it wakes up). 
Its recommended to attach the device to power when you have it in `configuration mode` so the battery is not drained. 

**If you have suggestions for more awesome features, head over to the github repository and make a request.**
