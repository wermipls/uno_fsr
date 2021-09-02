// uno_fsr proof of concept
// usb (16u2) code

#include "Keyboard.h"

// uncomment line below to communicate over hardware serial (NOT recommended, due to unresolved issue with massive ~50ms latency)
//#define SERIAL_COMM

// pins used for digital input (when serial communication is not used)
// refer to https://github.com/NicoHood/HoodLoader2/wiki/Arduino-Uno-Mega-16u2-Pinout
const int pins[] = {1, 2, 3, 4};
// output keyboard keys (change to your liking)
const char keys[] = {'q', 'w', 'e', 'r'};


void setup() {
	Serial1.begin(115200);

	Keyboard.begin();

	Serial.begin(115200);

	#ifndef SERIAL_COMM
	init_pins();
	delay(1000); // to prevent any potential bogus inputs happening
	#endif
}

void loop() {
	#ifdef SERIAL_COMM
	serial_bridge();
	#else
	digitalin_bridge();
	#endif
}

inline void serial_bridge()
{
	// check for serial data from i/o mcu
	char c = Serial1.read();

	// if bits 4-7 = 0, its a report
	if ((c & 0b11110000) == 0) {
		for (int i = 0; i < 4; ++i) {
			if ((c >> i) & 0b00000001) { 
				Keyboard.press(keys[i]);
			} else {
				Keyboard.release(keys[i]);
			}
		}
	}
}

inline void digitalin_bridge()
{
	for (int i = 0; i < 4; ++i) {
		if (digitalRead(pins[i])) { 
			Keyboard.press(keys[i]);
		} else {
			Keyboard.release(keys[i]);
		}
	}
}

inline void init_pins()
{
	for (int i = 0; i < 4; ++i) {
		pinMode(pins[i], INPUT);
	}
}
