NAME=Anty
PORT=/dev/ttyUSB0

deps:
	arduino-cli core install esp32:esp32
	arduino-cli lib install ESP32Servo

build:
	sed -i 's/^#define ROBOT_NAME.*/#define ROBOT_NAME "${NAME}"/g' robot.ino
	arduino-cli compile -b esp32:esp32:esp32 -v

upload:
	arduino-cli upload -b esp32:esp32:esp32 -p ${PORT}

bu: build upload

serial:
	picocom -b 115200 ${PORT}
