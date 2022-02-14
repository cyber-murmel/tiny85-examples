TARGET_SRC_CXX = \
    main.cpp \
    repl.cpp \

TARGET_SRC_C = \

TARGET_FLAGS = -DBUILTINS=0

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

TARGET_DIRS = \
    lib/DigistumpArduino/digistump-avr/libraries/DigisparkCDC/ \
    lib/DigistumpArduino/digistump-avr/cores/tiny/ \
