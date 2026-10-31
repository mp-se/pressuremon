.. _format-template:

Format templates
################

Each HTTP, InfluxDB, and MQTT target has a pressure data template. Variables
use the ``${variable}`` form and are replaced with current measurement values
before sending. Use **Predefined formats** to start from a service example,
edit the template, then select **Preview format** to inspect the result.

An empty template restores that target's default format. For HTTP POST and
MQTT, JSON previews are parsed and errors are shown. The MQTT preview also
shows the topic and payload structure. A preview checks formatting, not whether
the receiving service will accept the field names, token, or data types.

Template variables
==================

.. list-table:: PressureMon template variables
   :header-rows: 1

   * - Variable
     - Meaning
     - Example
   * - ``${mdns}``
     - Configured device name.
     - ``pressmon``
   * - ``${id}``
     - Unique device identifier.
     - ``e422a3``
   * - ``${token}``, ``${token2}``
     - Values configured under Push targets → Settings.
     - configured text
   * - ``${sleep-interval}``
     - Configured interval between measurement cycles, in seconds.
     - ``900``
   * - ``${temp}``
     - Temperature in the selected unit.
     - ``21.23``
   * - ``${temp-c}``, ``${temp-f}``
     - Temperature converted to Celsius or Fahrenheit.
     - ``21.23``
   * - ``${temp-unit}``
     - Temperature unit label, ``C`` or ``F``.
     - ``C``
   * - ``${pressure}``, ``${pressure-psi}``
     - Primary pressure in the selected unit or PSI.
     - ``28.673``
   * - ``${pressure-bar}``, ``${pressure-kpa}``
     - Primary pressure converted to Bar or kPa.
     - ``1.977``
   * - ``${pressure1}``, ``${pressure1-psi}``
     - Second pressure in the selected unit or PSI, when available.
     - ``28.673``
   * - ``${pressure1-bar}``, ``${pressure1-kpa}``
     - Second pressure converted to Bar or kPa, when available.
     - ``1.977``
   * - ``${pressure-unit}``
     - Selected unit label: PSI, Bar, or kPa.
     - ``PSI``
   * - ``${battery}``
     - Battery voltage.
     - ``3.89``
   * - ``${battery-percent}``
     - Estimated battery percentage.
     - ``70``
   * - ``${rssi}``
     - Wi-Fi signal strength in dBm.
     - ``-75``
   * - ``${run-time}``
     - Duration of the last measurement cycle in seconds.
     - ``3.87``
   * - ``${app-ver}``, ``${app-build}``
     - Firmware version and build revision.
     - ``0.8.0``

Availability of the second pressure value depends on the board and selected
sensor. If a sensor reading is unavailable, the firmware substitutes zero
while building a push payload. Check sensor status before interpreting zero as
a real reading.

Payload tips
============

For JSON, check quotes, commas, and whether the service expects pressure or
temperature as a number or string. For HTTP GET, include query separators and
parameter names in the template. For InfluxDB, use a measurement name, tags,
and fields with consistent types. For MQTT, provide one or more
``topic:payload`` pairs separated by ``|``. After previewing, run a target test
and confirm the data at the receiving service.
