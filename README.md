# uno_fsr
Proof of concept FSR dance pad code for Arduino Uno (and compatible w/ 16u2 chip) boards, using the 16u2 for USB communication and the 328p for reading and handling the sensors. Communication with the chips is done either via simple 4 wire connection or over hardware serial (currently not recommended, due to unresolved ~50ms latency issue). This project was motivated by a desire to repurpose an existing Uno board, instead of acquiring a better suited one with [existing software solutions](https://github.com/teejusb/fsr), such as Pro Micro or Teensy.

## Hardware setup
By default, the sensors are connected via A0-A3 pins, using MCU's internal pullup to form a voltage divider, though it may be desirable to use external resistors to tune the range. Pins D3-D6 on the 328p are used to output the sensor activation status, which need to be connected to pins D1-D4 on the 16u2 (refer to https://github.com/NicoHood/HoodLoader2/wiki/Arduino-Uno-Mega-16u2-Pinout for details on the pinout). In case of hardware serial this can be skipped.

## Software setup
You will need to set up HoodLoader2 (https://github.com/NicoHood/HoodLoader2) on your board, refer to the instructions on the wiki. Upload the "fsr_io.ino" sketch to the 328p and the "fsr_usb.ino" to the 16u2. If you want to use serial communication, make sure to uncomment `#define SERIAL_COMM` in both .ino files before uploading. Sensor threshold values can be adjusted in the I/O sketch by editing the values in onThreshold and offThreshold arrays, and the reported keyboard keys can be changed in the USB sketch.

## Known issues
* Usage of hardware serial results in approx. 50ms delay, which is very much undesirable for rhythm games.
* There is currently no way to preview current sensor values for calibration purposes. A simple way to do it would be to print the values over serial in the I/O sketch (immediately after uploading the 328p sketch, HoodLoader2 will enter the bootloader and conveniently function like a standard Arduino with the 16u2 acting as a serial device).
