#ifndef _SIMPLE_GPIO_H
#define _SIMPLE_GPIO_H

#include "DigistumpArduino/digistump-avr/cores/tiny/wiring.h"

typedef struct {
    union {
        unsigned reg : 8;
        struct {
            unsigned _0 : 1;
            unsigned _1 : 1;
            unsigned _2 : 1;
            unsigned _3 : 1;
            unsigned _4 : 1;
            unsigned _5 : 1;
            unsigned _6 : 1;
            unsigned _7 : 1;
        } bit;
    } in, dir, out;
} __attribute__((__packed__)) gpio_reg_t;

#define GPIOB (*((gpio_reg_t*)&PINB))

#define _gpio_dir(BANK, NUM, DIR) GPIO##BANK.dir.bit._##NUM = DIR
#define gpio_dir(BANK, NUM, DIR) _gpio_dir(BANK, NUM, DIR)

#define _gpio_out(BANK, NUM, VAL) GPIO##BANK.out.bit._##NUM = VAL
#define gpio_out(BANK, NUM, VAL) _gpio_out(BANK, NUM, VAL)
#define _gpio_in(BANK, NUM) GPIO##BANK.in.bit._##NUM
#define gpio_in(BANK, NUM) _gpio_in(BANK, NUM)

#include "bootloaderconfig.h"
static inline void usb_disconnect()
{
    gpio_out(USB_CFG_IOPORTNAME, USB_CFG_DMINUS_BIT, LOW);
    gpio_out(USB_CFG_IOPORTNAME, USB_CFG_DPLUS_BIT, LOW);
    gpio_dir(USB_CFG_IOPORTNAME, USB_CFG_DMINUS_BIT, OUTPUT);
    gpio_dir(USB_CFG_IOPORTNAME, USB_CFG_DPLUS_BIT, INPUT);
}

#endif
