# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

ARDUINO_DIR = ~/arduino-1.8.9
#ARDMK_DIR = ~/...
AVR_TOOLS_DIR = /usr/include

BOARD_TAG    = uno
#ARDUINO_PORT = /dev/ttyUSB0
#MONITOR_PORT = /dev/ttyUSB0
ARDUINO_LIBS = EEPROM MIDI Mozzi
include $(ARDMK_DIR)/Arduino.mk
