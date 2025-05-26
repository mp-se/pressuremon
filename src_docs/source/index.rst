.. GravityMon documentation master file, created by
   sphinx-quickstart on Wed Jan  5 22:46:42 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to PressureMon
######################

.. note::
  This documentation reflects **v0.6.0**. Last updated 2025-51-28


What is PressureMon?
--------------------

PressuremonMon is a IOT device that can measure and measure pressure and temperature during 
fermentation or storage of beer and transmit the data to a number of external services. Is uses a 
digital or analog pressure sensor to measure the pressure. 

The analog sensor version require a separate ADC chip to have the needed accuracy. This option is still work in progress.

The codebase for this project is shared with my project GravityMon so the code is stored in the same repository. 70% of the 
codebase is the same so it makes sense from a maintenance perspective.

The github repository can be found here; `PressureMon on Github <https://github.com/mp-se/pressuremon>`_, where the documentation 
and builds are stored. The sensor specific code is also stored here for easier testing and development. 

User Interface is stored in this repositiory; `PressureMon-UI on Github <https://github.com/mp-se/pressuremon-ui>`_, where the documentation 
and builds are stored. The sensor specific code is also stored here for easier testing and development. 

The github repository for the source code is; `GravityMon on Github <https://github.com/mp-se/gravitymon>`_

.. note::  
  I don't take responsibility for any errors or issues caused by the software or hardware. 
  The software is provided as-is. I will however try my best to fix issues that might occur. 


Credits to
----------
Ideas to some of these functions have been picked up from discussions in the iSpindle forums. This software uses 
the following libraries and without these this would have been much more difficult to achieve:

* https://github.com/thijse/Arduino-Log

  Logging library for handling different loglevels and configure what sent over the serial.

* https://github.com/bblanchon/ArduinoJson

  Json parser/creator used in configuration files and API's

* https://github.com/256dpi/arduino-mqtt

  Library for sending data to mqtt based on lightweight mqtt implementation. 

* https://getbootstrap.com/

  CSS templates for the web page.

* https://github.com/h2zero/NimBLE-Arduino

  Bluetooth library for ESP32 framework


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   functionality
   releases
   intro
   installation
   configuration
   format-template
   hardware
   services
   custom
   compiling
   contributing
   license
   q_and_a

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
