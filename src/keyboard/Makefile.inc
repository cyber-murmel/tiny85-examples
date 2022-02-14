TARGET_SRC_CXX = \
    main.cpp \

SRC_S += \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkKeyboard/usbdrvasm.S \

SRC_C += \
    lib/DigistumpArduino/digistump-avr/cores/tiny/wiring.c \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkKeyboard/usbdrv.c \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkKeyboard/osccal.c \

SRC_CXX += \
    lib/DigistumpArduino/digistump-avr/cores/tiny/main.cpp \
    lib/DigistumpArduino/digistump-avr/cores/tiny/Print.cpp \

TARGET_DIRS = \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkKeyboard/ \
    lib/DigistumpArduino/digistump-avr/cores/tiny/ \
