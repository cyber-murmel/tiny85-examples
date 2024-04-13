TARGET_SRC_CXX = \
    main.cpp \

SRC_S += \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkCDC/usbdrvasm.S \

SRC_C += \
    lib/DigistumpArduino/digistump-avr/cores/tiny/wiring.c \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkCDC/usbdrv.c \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkCDC/osccal.c \

SRC_CXX += \
    lib/DigistumpArduino/digistump-avr/cores/tiny/main.cpp \
    lib/DigistumpArduino/digistump-avr/cores/tiny/Print.cpp \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkCDC/DigiCDC.cpp \
    lib/DigistumpArduino/digistump-avr/libraries/Wire/Wire.cpp \
    lib/DigistumpArduino/digistump-avr/libraries/Wire/USI_TWI_Master.cpp \

TARGET_DIRS = \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkCDC/ \
    lib/DigistumpArduino/digistump-avr/libraries/Wire/ \
    lib/DigistumpArduino/digistump-avr/cores/tiny/ \
