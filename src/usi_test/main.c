// #include <avr/io.h>
// F_CPU frequency to be defined at command line
#include "simple_gpio.h"
#include "simple_usi.h"
#include <util/delay.h>

#define LED_BANK B
#define DELAY_MS 250

#define USI_BANK B
#define PIN_USI_SDA 0
#define PIN_USI_SCL 2

#define PORT_USI PORTB
#define DDR_USI DDRB

#define T2_TWI 5 // >4,7us
#define T4_TWI 4 // >4,0us

void usi_init() {
    PORT_USI |=
      (1
       << PIN_USI_SDA); // Enable pullup on SDA, to set high as released state.
    PORT_USI |=
        (1
         << PIN_USI_SCL); // Enable pullup on SCL, to set high as released state.
  
    DDR_USI |= (1 << PIN_USI_SCL); // Enable SCL as output.
    DDR_USI |= (1 << PIN_USI_SDA); // Enable SDA as output.
  
    USIDR = 0xFF; // Preload dataregister with "released level" data.
    USICR = (0 << USISIE) | (0 << USIOIE) | // Disable Interrupts.
            (1 << USIWM1) | (0 << USIWM0) | // Set USI in Two-wire mode.
            (1 << USICS1) | (0 << USICS0) |
            (1 << USICLK) | // Software stobe as counter clock source
            (0 << USITC);
    USISR = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) |
            (1 << USIDC) |    // Clear flags,
            (0x0 << USICNT0); // and reset counter.



    // gpio_out(USI_BANK, PIN_USI_SDA, HIGH);
    // gpio_out(USI_BANK, PIN_USI_SCL, HIGH);

    // gpio_dir(USI_BANK, PIN_USI_SDA, OUTPUT);
    // gpio_dir(USI_BANK, PIN_USI_SCL, OUTPUT);

    // USI.usidr = 0xFF;

    // USI.usicr = (usicr_t){
    //     .bit = {
    //         .usisie = false,
    //         .usioie = false,
    //         .usiwm = 0b10,
    //         .usics = 0b10,
    //         .usiclk = 1,
    //         .usitc = 0,
    //     },
    // };

    // USI.usisr = (usisr_t){
    //     .bit = {
    //         .usisif = true,
    //         .usioif = true,
    //         .usipf = true,
    //         .usidc = true,
    //         .usicnt = 0,
    //     },
    // };
}

void usi_send() {

    unsigned char const usisr = (1 << USISIF) | (1 << USIOIF) | (1 << USIPF) |
      (1 << USIDC) |    // Prepare register value to: Clear flags, and
      (0x0 << USICNT0); // set USI to shift 8 bits i.e. count 16 clock edges.

    unsigned char const usicr = (0 << USISIE) | (0 << USIOIE) | // Interrupts disabled
         (1 << USIWM1) | (0 << USIWM0) | // Set USI in Two-wire mode.
         (1 << USICS1) | (0 << USICS0) |
         (1 << USICLK) | // Software clock strobe as source.
         (1 << USITC);   // Toggle Clock Port.
    // usisr_t usisr = {
    //     .bit = {
    //         .usisif = true,
    //         .usioif = true,
    //         .usipf = true,
    //         .usidc = true,
    //         .usicnt = 0,
    //     },
    // };

    // usicr_t usicr = {
    //     .bit = {
    //         .usisie = false,
    //         .usioie = false,
    //         .usiwm = 0b10,
    //         .usics = 0b10,
    //         .usiclk = 1,
    //         .usitc = 1,
    //     },
    // };

    USISR = usisr;

    USICR = usicr;
    _delay_ms(1);
    USICR = usicr;
    _delay_ms(1);
    USICR = usicr;
    _delay_ms(1);
    USICR = usicr;
    _delay_ms(1);

    // USI.usisr = usisr;


    // USI.usicr = usicr;
    // _delay_ms(1);
    // USI.usicr = usicr;
    // _delay_ms(1);
    // USI.usicr = usicr;
    // _delay_ms(1);
    // USI.usicr = usicr;
    // _delay_ms(1);
    // USI.usicr = usicr;


    // do {
    //     USI.usicr = usicr;
    //     while(!gpio_in(USI_BANK, PIN_USI_SCL));
    //     USI.usicr = usicr;
    // } while (!(USI.usisr.bit.usioif));

    // for (int i = 0; i<8; i++) {
    //     USI.usicr = usicr;
    //     while(!gpio_in(USI_BANK, PIN_USI_SCL));
    //     USI.usicr = usicr;
    // }
}

void usi_start(void) {
    /* Release SCL to ensure that (repeated) Start can be performed */
    PORT_USI |= (1 << PIN_USI_SCL); // Release SCL.
    while (!(PORT_USI & (1 << PIN_USI_SCL)))
        ; // Verify that SCL becomes high.
      _delay_ms(T2_TWI);

    /* Generate Start Condition */
    PORT_USI &= ~(1 << PIN_USI_SDA); // Force SDA LOW.
    _delay_ms(T4_TWI);
    PORT_USI &= ~(1 << PIN_USI_SCL); // Pull SCL LOW.
    PORT_USI |= (1 << PIN_USI_SDA);  // Release SDA.

// #ifdef SIGNAL_VERIFY
//   if (!(USISR & (1 << USISIF))) {
//     USI_TWI_state.errorState = USI_TWI_MISSING_START_CON;
//     return (FALSE);
//   }
// #endif
//   return (TRUE);
}

int main()
{
    usb_disconnect();
    gpio_out(LED_BANK, LED_PIN, HIGH);
    usi_init();

    // _delay_ms(10);

    usi_start();

    usi_send();

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
