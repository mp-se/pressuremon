# Pressure monitor for Beer fermentation 

Since I have started to ferment my beer under pressure I wanted to be able to measure and collect that data in like i do with gravity/temperature (using iSpindle). So this project was born.

```
CAUTION!!! This project uses 3D printed models and these could break under too high pressure. I take no responsibility for a failed 3D printed part or any injuries that this may cause. Make sure you test the part in a safe way before using it for measuring pressure. 
```

I have tested both analog and digital pressure senors and the key is to find a sensor that has a limited measurement range to have the best accuracy. Most of the analog sensors have a to wide measurement range which make them not accurate enough for this. The digital sensors work much better and can be adapted for the range that is of interrest.  

Since we are measuring beer fermentation, a sensor in the range of 0-4 bar (or 0-60 psi) is enough for me, but the honeywell range have sensors that can handle higher pressure. But I'm not so sure how the 3D printed PLA adapter would handle that. We will most likley not end up above 2.5 bar since most fermentation vessels have this as a upper limit. The Honeywell sensors I selected can be obtained in a number of variations so it can be adapted to other pressure ranges with minimal changes to the code.

The aim is to make this project as small as possible and make it easy to connect it to an gas post on a fermentation vessel with possibility to have a spunding valve to release excess pressure. 

The trickey part was to figure out how to connect the small sensor and I ended up with having a 3D printed adapter in 3/8" format that can be connected to a quick release John Guest adapter. This model can be adpated to other size adapters if needed. 

## Versions

* 0.1.0 First version based on Honeywell Digital pressure sensor.

## Future changes

* Make 3d case 
* Make pcb-board
* Create webinterface for interacting with device
* Create pressure adapter for other dimensions
* Add push support for more targets

## How it works

The device will measure the pressure and temperature (yes the honeywell sensors have a built in temperature sensor) and report these on an attached LED display or to one of the defined endpoints. Dont use display if you want long battery life. 

The following endpoints are currently implemented:
* Brewfather 
* Fermentrack (Using the iSpindel interface for reporting pressure)
* Thingsboard 

__TOOD__ Test brewfather endpoint

If the device is used with an battery it will go into sleep mode when there is a pressure higher than 0.5 psi and the voltage is less than 4.5V (indication of an attached battery). This is to prolong the lifespan of the device when on battery. If you run the device via a powersupply it will never to into sleep mode. 

__TOOD__ Testing how long the device will work on a full battery.

## 3D Print

In order to attach the pressure sensor to the fermentation vessel I created a 3D printed adapter that fits on a John Guest 3/8" adapter and the pressure sensor.

Use high infill rate and make sure you get good layer adhesion so it can handle 2.5 bar pressure. Make sure you test that it's fits to the JC adapter and there is no leaks before attaching it to the pressure sensor. I used hot glue to attach mine to the sensor but any good adhesive should work. Just make sure the small reference hole on the bottom of the sensor is kept open.

The stl file can be found in the __cad/__ subdirectory.

## Installation

You can use VisualStudio Code + Platform IO to handle the device flashing. 

An option could be to use this tool; https://github.com/marcelstoer/nodemcu-pyflasher

## Setup

The device will act as an WIFI accesspoint if it's not configured (you can also double tap on the reset button to force it into AP mode). The device will flash the power led while in this mode. Search for an AP called __PressureMon__ and use the password = 'password'. The device will have the IP adress 192.168.4.1 as default. 

In the interface you can set some of the parameters for the device but it's also possible to change these via web requests (later on there will be a fancy web interface for this).

Once the device is on the wifi network it will have a running webserver that can show the active configuration and also force the device into configuration model. The name of the device will be __pressuremonXXXXX.local__ (or just use the dynamic IP). Chip ID will be 6 characters and uniqe for that device (eg 7a84DC).

The device has the following endpoints that can be used to interact with it;

* __/__ will show the name, version and chip ID
* __/config__ will show the current configuration in json format
* __/reset__ will reboot the device.
* __/clearwifi__ will force the device into wifi configuration mode by erasing the wifi settings.
* __/calibrate__ will do a zero level calibration so the sensor ports will be in synk.
* __/api/config/get?param=__ will receive a configuration parameter.
* __/api/config/set?id=X&param=Y&value=Z__ will set a configuration parameter. The ID is used to validate that the commands are for the correct device (ID = ChipID). This can be found on the main page or via the /config page. 

Valid configuration parameters:

* __id__ Chip ID (Read Only)
* __mdns__ mDNS name of the device
* __otaurl__ url to directory where new firmware versions are located
* __fermentrackpush__ url to fermentrack endpoint (using ispindel for now)
* __brewfatherpush__ url to brewfather endpoint
* __httppush__ url to standard http endpoint
* __pushinterval__ seconds between push
* __tempformat__ temperature format (Valid: C or F)
* __pressurecorr__ temperature correction value (from calibration)

## Build Configuration

I prefer to use Visual Studio Code with the extension PlatformIO which makes it quite easy to make a build.

The following defintions can be used to enable/disable parts of the code

* ACTIVATE_PUSH       Include push target code in build (requires wifi)
* ACTIVATE_OTA        Include ota code in build (requires wifi)
* ACTIVATE_WIFI       Include wifi access in build 

Development related settings

* LOG_LEVEL=6       Configure Arduino Log (6=Debug, 5=Trace, 4=Notice, 3=Warning, 2=Error, 1=Fatal, 0=Silent)
* DISPLAY_SELFTEST  Runs some tests on the display at startup
* SIMULATE_SENSOR   Will simulate sensor values (currently only fixed values)
* SKIP_SLEEPMODE    WIll force the device to never go into sleep mode.

## OTA function

The software can do updates via OTA from a local web server over port 80. 

For this to work, place the following files (version.json + firmware.bin) at the location that you pointed out in the mysecrets.h file. If the version number in the json file is newer than in the code the update will procced.

Example; OTA URL (don't forget trailing dash) 
```
http://192.168.1.x/firmware/pressuremon/
```

Contents version.json
```
{ "project":"pressuremon", "version":"0.3.0" }
```

## Pushtarget

Work in progress...

## Materials

In order to build this project you will need the following;

Work in progress...

* ...

Feel free to use the code and modify your own build. 

Happy building. /Magnus 