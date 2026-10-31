Development and compiling
=========================

Firmware
--------

Install `PlatformIO <https://platformio.org/>`_ and clone the repository with
its submodules. Open the project in VS Code or use the PlatformIO CLI. The
available firmware targets are declared in ``platformio.ini``:

* ``pressure-32s3_mini`` — ESP32-S3 Mini
* ``pressure-32s2_mini`` — ESP32-S2 Mini
* ``pressure-32c3_pico`` — ESP32-C3 Pico

Build or upload the environment matching the hardware. The firmware uses
ESP-IDF/Arduino libraries managed by PlatformIO; let PlatformIO install the
pinned dependencies before building.

Web interface simulator
-----------------------

The UI is under ``ui/``. Install dependencies with ``npm install``. Start the
mock API first in one terminal with ``npm run mock``, then start the Vite UI
with ``npm run dev`` in another terminal. The development UI is served at
``http://localhost:5173`` and uses the mock API on port 3000.

Documentation
-------------

The documentation dependencies require Python 3.10 or newer (Sphinx 8.2.3).
On macOS, create and activate a virtual environment with a compatible Python
before installing them. For example, with Python 3.13:

.. code-block:: console

   python3.13 -m venv .venv
   source .venv/bin/activate
   python -m pip install -r src_docs/requirements.txt
   make -C src_docs html

The HTML output is placed in ``src_docs/build/html``. If ``python3`` points to
Python 3.9 or older, use an explicit ``python3.13`` command when creating the
environment; upgrading pip alone does not change the Python version.
