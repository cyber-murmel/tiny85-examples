#include <avr/io.h>
// F_CPU frequency to be defined at command line
#include "simple_gpio.h"
#include <util/delay.h>

#define LED_BANK B
#define LED_PINA 0
#define LED_PINB 1
#define DELAY_MS 500

int main()
{
    usb_disconnect();
    while (1) {
        // color 1
        gpio_dir(LED_BANK, LED_PINA, OUTPUT);
        gpio_out(LED_BANK, LED_PINA, LOW);
        gpio_dir(LED_BANK, LED_PINB, INPUT);
        gpio_out(LED_BANK, LED_PINB, HIGH);
        _delay_ms(DELAY_MS);

        // off
        gpio_dir(LED_BANK, LED_PINA, INPUT);
        _delay_ms(DELAY_MS);

        // color 2
        gpio_dir(LED_BANK, LED_PINB, OUTPUT);
        gpio_out(LED_BANK, LED_PINB, LOW);
        gpio_dir(LED_BANK, LED_PINA, INPUT);
        gpio_out(LED_BANK, LED_PINA, HIGH);
        _delay_ms(DELAY_MS);

        // off
        gpio_dir(LED_BANK, LED_PINB, INPUT);
        _delay_ms(DELAY_MS);
    }
    return 0;
}
