.. _format-template:

Format Templates
################

To reduce the need for adding custom endpoints for various services there is an built in format editor that allows the user to customize the format being sent to the push target. 


You enter the format data in the text field and the preview button will show an example on what the 
output would look like. If the data cannot be formatted in json it will just be displayed as a text.

You can also select a template from the list and copy that to the current endpoint. 

.. tip::

   If you save a blank string the default template will be loaded.

These are the format keys available for use in the format.

.. list-table:: Format template keywords
   :widths: 30 50 20
   :header-rows: 1

   * - key 
     - description
     - example
   * - ${mdns}
     - Name of the device
     - gravmon2
   * - ${token}
     - Token
     - any value
   * - ${token2}
     - Token 2
     - any value
   * - ${id}
     - Unique id of the device
     - e422a3
   * - ${sleep-interval}
     - Seconds between data is pushed
     - 900
   * - ${temp}
     - Temperature in format configured on device, two decimals
     - 21.23
   * - ${temp-c}
     - Temperature in C, two decimals
     - 21.23
   * - ${temp-f}
     - Temperature in F, two decimals
     - 58.02
   * - ${temp-unit}
     - Temperature format `C` or `F`
     - C
   * - ${battery}
     - Battery voltage, two decimals
     - 3.89
   * - ${battery-percent}
     - Battery percentage
     - 20
   * - ${rssi}
     - Wifi signal strength
     - -75
   * - ${run-time}
     - How long the last measurement took, two decimals
     - 3.87
   * - ${pressure}
     - Pressure in PSI, tree decimals
     - 28.673
   * - ${pressure1}
     - Pressure in PSI from second sensor, tree decimals
     - 28.673
   * - ${pressure-psi}
     - Pressure in PSI, tree decimals
     - 28.673
   * - ${pressure1-psi}
     - Pressure in PSI from second sensor, tree decimals
     - 28.673
   * - ${pressure-bar}
     - Pressure in Bar, tree decimals
     - 28.673
   * - ${pressure1-bar}
     - Pressure in BAR from second sensor, tree decimals
     - 28.673
   * - ${pressure-kpa}
     - Pressure in kPa, tree decimals
     - 28.673
   * - ${pressure1-kpa}
     - Pressure in kPA from second sensor, tree decimals
     - 28.673
   * - ${pressure-unit}
     - Pressure format, `PSI`, `Bar` or `kPa`
     - PSI
   * - ${app-ver}
     - Software version
     - 1.3.0
   * - ${app-build}
     - Software revision (git hash)
     - ..e456743
