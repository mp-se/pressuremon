import os, sys

args = len(sys.argv)
if args != 3:
  print("Usage: flash.py <target> <port>")
  exit(0)

if sys.argv[1] != "esp32s3":
  print("Supported targets are: esp32s3")
  exit(0)

flash = ""
port = ' --port "' + sys.argv[2] + '"'

if sys.argv[1] == "esp32s3":
  flash = " --chip esp32s3 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x0000 bin/bootloader32s3.bin 0x8000 bin/partitions32s3.bin 0xe000 bin/boot32s3.bin 0x10000 bin/firmware32s3.bin"

cmd = "python3 -m esptool" + port + flash

print(cmd)
os.system(cmd)
