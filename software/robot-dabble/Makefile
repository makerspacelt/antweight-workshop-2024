NAME=Anty
PORT=/dev/ttyUSB0

deps:
	arduino-cli lib install ESP32Servo

build:
	sed -i 's/^#define ROBOT_NAME.*/#define ROBOT_NAME "${NAME}"/g' robot-dabble.ino
	arduino-cli compile --clean -b esp32:esp32:esp32 -v

upload:
	arduino-cli upload -b esp32:esp32:esp32 -p ${PORT}

bu: build upload

serial:
	picocom -b 115200 ${PORT}
