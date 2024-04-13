#include <avr/io.h>
// F_CPU frequency to be defined at command line
#include "simple_gpio.h"
#include <util/delay.h>

#define LED_BANK B
#define LED_PIN 0
#define DELAY_MS 250

#define XSTR(x) STR(x)
#define STR(x) #x
#pragma message "PORTB: " XSTR(PORTB)

int main()
{
    usb_disconnect();
    gpio_dir(LED_BANK, LED_PIN, OUTPUT);
    while (1) {
        // on
        gpio_out(LED_BANK, LED_PIN, HIGH);
        _delay_ms(DELAY_MS);

        // off
        gpio_out(LED_BANK, LED_PIN, LOW);
        _delay_ms(DELAY_MS);
    }
    return 0;
}
