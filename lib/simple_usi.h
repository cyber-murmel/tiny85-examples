#ifndef _SIMPLE_USI_H
#define _SIMPLE_USI_H

#include "DigistumpArduino/digistump-avr/cores/tiny/wiring.h"

typedef union {
    unsigned reg : 8;
    struct {
        unsigned usisif: 1; // Start Condition Interrupt Flag
        unsigned usioif: 1; // Counter Overflow Interrupt Flag
        unsigned usipf: 1; // Stop Condition Flag
        unsigned usidc: 1; // Data Output Collision
        unsigned usicnt: 4; // Counter Value
    } __attribute__((__packed__)) bit;
} __attribute__((__packed__)) usisr_t; // Status Register

typedef union {
    unsigned reg : 8;
    struct {
        unsigned usisie: 1; // Start Condition Interrupt Enable
        unsigned usioie: 1; // Counter Overflow Interrupt Enable
        unsigned usiwm: 2; // Wire Mode
        unsigned usics: 2; // Clock Source Select
        unsigned usiclk: 1; // Clock Strobe
        unsigned usitc: 1; // Toggle Clock Port Pin
    } __attribute__((__packed__)) bit;
} __attribute__((__packed__)) usicr_t; // Control Register

typedef struct {
    uint8_t usidr; // Data Register
    uint8_t usibr; // Buffer Register
    usisr_t usisr; // Status Register
    usicr_t usicr; // Control Register
} __attribute__((__packed__)) usi_reg_t;

#define USI (*((usi_reg_t*)&USIDR))

#endif
