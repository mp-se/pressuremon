.. _services:

Push targets and integrations
#############################

Configure data destinations under **Push targets**. PressureMon supports two
HTTP POST targets, one HTTP GET target, InfluxDB v2, MQTT, Bluetooth, and
Wi-Fi Direct delivery to GravityMon Gateway. Multiple network targets can be
active, although each connection adds radio-on time and reduces battery life.

Push settings
=============

Open **Push targets → Settings**.

* **Token 1** — available in templates as ``${token}``; the default iSpindel
  compatible HTTP format uses it.
* **Token 2** — available as ``${token2}``; the default HTTP GET template can
  use it for services that expect a query token.
* **Sleep interval** — time between measurement cycles in sleep mode, 10–3600
  seconds. A lower value produces more frequent readings and generally shortens
  battery life. The interface warns when set below 300 seconds. Use the
  battery estimate as a rough guide, not a guarantee.
* **Estimated battery life** — approximation based on the board, nominal
  battery capacity, and measured or assumed run time. Wi-Fi signal and
  connection time can change real battery life substantially.
* **Push timeout** — maximum time to wait for a service to accept a connection,
  10–60 seconds.

HTTP POST #1 and HTTP POST #2
=============================

Open **Push targets → HTTP Post** or **HTTP Post 2**. The two targets have
independent endpoints, headers, skip intervals, templates, and enable
switches.

* **HTTP URL** — complete ``http://`` or ``https://`` endpoint.
* **HTTP Header #1 / #2** — optional lines in ``Name: value`` format. The
  **Predefined headers** menu can insert common headers such as
  ``Content-Type: application/json``. Leave unused headers blank.
* **Skip interval** — 0 sends on every wake cycle; 1 skips one cycle, 2 skips
  two cycles, and so on. The UI range is 0–5.
* **Data format (pressure)** — editable payload template. Choose a predefined
  format, preview the rendered result, and enable pressure data for scheduled
  pushes. The current PressureMon firmware exposes pressure templates.
* **Run push pressure test** — sends a test request using that target's
  pressure template. Save the endpoint and credentials first, then check the
  receiver to verify it accepted the data.

An advanced build may expose **Use tcp** on HTTP POST #1. It replaces the URL
and header controls with a ``hostname:port`` server field. This option is not
enabled in the standard UI build.

HTTP GET
========

The HTTP GET target has its own URL, two optional headers, skip interval,
pressure template, enable switch, predefined format, preview, and test action.
The rendered template is appended to the configured URL; include query
parameter names and separators expected by the server. URL encode values when
required by the service.

InfluxDB v2
===========

Open **Push targets → Influxdb v2**.

* **Server** — full InfluxDB URL, including scheme and port if needed.
* **Organisation** — organization name or identifier.
* **Bucket** — destination bucket name.
* **Authentication token** — token with write access to that organization and
  bucket.
* **Skip interval** — 0–5 target cycles, with 0 sending on each eligible
  wake-up.
* **Data format (pressure)** — line protocol template with a separate enable
  switch, predefined formats, preview, and test button.

The template must be valid InfluxDB line protocol. Verify the resulting point
in the selected bucket after a successful test. Confirm that the configured
server address and transport are supported by the installed firmware.

MQTT
====

Open **Push targets → MQTT**.

* **Server** — broker host name or IP address without a URL scheme.
* **Port** — the UI accepts 0–65535. The firmware treats ports above 8000 as
  TLS connections; use 1883 for standard MQTT or the broker's configured TLS
  port (commonly 8883). A value of 0 uses the firmware default.
* **User name / Password** — optional credentials, up to 20 characters each.
* **Skip interval** — 0–5 target cycles.
* **Retain** — controls whether the broker retains the published message.
* **Data format (pressure)** — template with enable switch, predefined formats,
  preview, and test action. MQTT templates use ``topic:payload`` pairs
  separated by ``|``.

Check the preview for topic spelling and payload syntax, then confirm that a
subscriber receives the message.

Bluetooth
=========

Open **Push targets → Bluetooth** on a BLE-capable board and choose **Disabled**
or **Pressuremon iBeacon**. The iBeacon advertisement carries PressureMon
measurement data for a nearby BLE receiver. Eddystone is not an enabled UI
option. The ESP32-S2 target does not include BLE support.

Wi-Fi Direct
============

Open **Push targets → Wi-Fi Direct** to configure a GravityMon Gateway access
point. Enter the gateway's **Direct SSID** and **Direct Password**, then enable
**Use Wi-Fi Direct in gravity mode**. In measurement mode the device connects
to that access point and sends the gateway's predefined payload. This mode
bypasses regular service targets while active. Disable it to resume normal
endpoint pushes.

When Wi-Fi Direct is enabled, the HTTP, MQTT, and InfluxDB controls are disabled
in the UI because those targets are bypassed in measurement mode.

Format preview and tests
========================

Use **Predefined formats** to insert a starting template. **Preview format**
shows the substituted sample values; for HTTP POST and MQTT the UI also checks
JSON where applicable. Run a pressure test after saving. A successful request
shows that the service returned a response; verify the accepted measurement on
the receiving service. See :doc:`format-template` for variables and examples.

Common services
===============

* **Brewfather** — use its iSpindel-compatible endpoint or a custom endpoint
  and matching JSON format.
* **Home Assistant** — ingest measurements over MQTT or HTTP. See
  :doc:`format-template` for data available to the templates.
* **InfluxDB** — use InfluxDB v2 and a line protocol template.
* **GravityMon Gateway** — use Wi-Fi Direct for a direct local link or a normal
  network target when both devices share Wi-Fi.
