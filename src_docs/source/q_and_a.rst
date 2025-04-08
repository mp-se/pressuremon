Q & A
#####

HTTP error codes
----------------

All these errors are standard http error codes. This are the common ones;

*  400 - Bad request. Probably an issue with the post format. Check format in the format editor.
*  401 - Unauthorized. The service needs an token or other means to authenticate the device. 
*  403 - Forbidden. Could be an issue with token or URL. 
*  404 - Not found. Probably a wrong URL.

MQTT error codes
----------------

MQTT connection errors:

* -1 - Connection refused
* -2 - Send header failed
* -3 - Send payload failed
* -4 - Not connected
* -5 - Connection lost
* -6 - No stream
* -7 - No HTTP server
* -8 - Too little RAM available
* -9 - Error encoding
* -10 - Error writing to stream
* -11 - Read timeout
* -100 - Endpoint skipped since its SSL and the device is in gravity mode

MQTT push on topic errors:

* -1 - Buffer to short
* -2 - Overflow
* -3 - Network failed connected
* -4 - Network timeout
* -5 - Network read failed
* -6 - Network write failed
* -10 - Connection denied
* -11 - Failed subscription

WIFI error codes
----------------

* 1 - No SSID found.
* 4 - Connection failed. 
* 5 - Connection lot.
* 6 - Wrong password.
* 7 - Disconnected by AP.
