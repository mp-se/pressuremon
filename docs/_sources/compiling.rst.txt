.. _compiling-the-software:

Compiling the software
######################

.. note::

  The codebase for this project is shared with my other IOT devices and is stored 
  in the gravitymon repositiory. This repository only contain a copy of the code for 
  interacting with the pressure sensors and the scripts for building and publishing 
  new versions. If you want to improve the pressure sensor code its fine to create 
  a PR towards this repository.


Tools
=====
I use the following tools in order to build and manage the software:

* Visual Studio Code
* PlatformIO
* Git for Windows
* Python3 (for building docs)


Code Formatting
===============
I use pre-commit and their cpp style checks to validate the code. Plugin defintions are found in **.pre-commit-config.yaml**

`Pre-Commit <https://www.pre-commit.com>`_

.. note::

  There is not yet any automatic checks since this does not work on Windows. It works if running under WSL2 
  with Ubuntu or on MacOS.


Targets 
=======
In the platformio config there are 3 targets defined

* pressure32c3-release: Version for ESP32 C3 mini and pico.
* pressure32s2-release: Version for ESP32 S2 mini.
* pressure32s3-release: Version for ESP32 S3 mini.

.. warning::
  The debug target can be unstable and crash the device under certain circumstanses. Excessive logging to the 
  serial port can cause corruption and crashes. So only enable enough debugging to troubleshoot your changes.

Serial debugging on battery
===========================

The serial output can be written to UART0 which is connected to the RX/TX pins on the chip. This way the serial output can be viewed 
without a connection to the USB port, convinient when running the device on battery power. In order to get this to work you need to compile the sofware 
with the option **ESPFWK_USE_SERIAL_PINS** and attach as USB to TTL cable to the correct pins. 

You connect the USB to TTL cable that you connect the TX, RX and GND pins. **Dont connect the power pin** if you are powering the device from USB or Battery.

.. image:: images/usb-ttl.jpg
  :width: 300
  :alt: USB to TTL cable


Source structure 
================
.. list-table:: Directory structure
   :widths: 40 60
   :header-rows: 1

   * - path
     - content
   * - /.github
     - Automated github action workflows
   * - /bin
     - Contains compiled binaries
   * - /data
     - Directory for flashing device filesystem
   * - /doc
     - Various external documents used as input
   * - /html
     - Copy of User Interface build
   * - /lib
     - External libraries used when compiling
   * - /script
     - Scripts used in build process
   * - /src
     - Source code for software
   * - /src_docs
     - Source code for documentation
   * - /test
     - Unit tests


Options 
=======
This is a list of C++ defines that is used to enable/disable functions in the code.

.. list-table:: Defines
   :widths: 40 60
   :header-rows: 1

   * - define
     - description
   * - USER_SSID
     - If defined the device will always use this SSID
   * - USER_SSID_PWD
     - Password to the SSID
   * - LOG_LEVEL
     - Defines the logging level used 4=INFO, 5=DEBUG, 6=VERBOSE
   * - CFG_APPVER
     - Defines the version of the compiled software
   * - CFG_GITREV
     - Contains the last 6 digits of the git revision
   * - ESP32S2, ESP32S3, ESP32C3
     - These are set to configure the target platform both in pressuremon but also the underlying espframework.  
     