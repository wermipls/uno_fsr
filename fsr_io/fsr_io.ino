// uno_fsr proof of concept
// i/o (328p) code

// uncomment line below to communicate over hardware serial (NOT recommended, due to unresolved issue with massive ~50ms latency)
//#define SERIAL_COMM
// uncomment line below to enable reporting sensor values over serial, for e.g. calibration purposes
//#define PRINT_VALS

// default inputs: A0-A3 (left down up right)
const int pins[] = {A0, A1, A2, A3};
const int pinsOut[] = {3, 4, 5, 6};
// thresholds
const int offThreshold[] = {925, 945, 925, 955}; // must be higher than corresponding onThreshold
const int onThreshold[] = {920, 940, 920, 950};

int vals[4];
bool pressed[4];
char pressed_packed = 0;

void setup() {
	Serial.begin(115200);
	init_pins();
}

void loop() {
	// read vals and send em to usb mcu
	read_analog();
	check_thresholds();

	#ifdef SERIAL_COMM
	pressed_packed = pack_bits();
	Serial.write(pressed_packed);
	#else
	write_output();
	#endif

	#ifdef PRINT_VALS
	print_vals();
	#endif
}

inline void init_pins()
{
	for (int i = 0; i < 4; ++i) {
		pinMode(pins[i], INPUT_PULLUP);
		pinMode(pins[i], HIGH);
		// out pins
		pinMode(pinsOut[i], OUTPUT);
	}
}

inline void read_analog()
{
	for (int i = 0; i < 4; ++i) {
		vals[i] = analogRead(pins[i]);
	}
}

inline void check_thresholds()
{
	for (int i = 0; i < 4; ++i) {
		if (pressed[i]) {
			if (vals[i] > offThreshold[i])
				pressed[i] = false;
		} else {
			if (vals[i] < onThreshold[i])
				pressed[i] = true;
		}
	}
}

inline void write_output()
{
	for (int i = 0; i < 4; ++i) {
		digitalWrite(pinsOut[i], pressed[i]);
	}
}

inline char pack_bits()
{
	char packed = 0;
	for (int i = 0; i < 4; ++i) {
		bitWrite(packed, i, pressed[i]);
	}

	return packed;
}

inline void print_vals()
{
	char buffer[128];
	snprintf(buffer, 128, "vals: %d, %d, %d, %d\n", vals[0], vals[1], vals[2], vals[3]);
	Serial.print(buffer);
}
