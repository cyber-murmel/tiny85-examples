TARGET_SRC_CXX = \
    main.cpp

SRC_C += \
    lib/DigistumpArduino/digistump-avr/cores/tiny/wiring.c \
    lib/DigistumpArduino/digistump-avr/cores/tiny/wiring_digital.c \
    lib/DigistumpArduino/digistump-avr/cores/tiny/pins_arduino.c \

SRC_CXX += \
    lib/DigistumpArduino/digistump-avr/cores/tiny/main.cpp \
    lib/DigistumpArduino/digistump-avr/cores/tiny/Tone.cpp \

TARGET_DIRS = \
    lib/DigistumpArduino/digistump-avr/cores/tiny/ \