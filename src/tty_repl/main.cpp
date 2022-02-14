#include "platform_print.h"
#include "repl.h"
#include "simple_gpio.h"
#include <DigiCDC.h>
#include <WProgram.h>

#define LED_BANK B
#define LED_PINA 0
#define LED_PINB 1

void setup()
{
    // assert off double LED
    gpio_dir(LED_BANK, LED_PINA, INPUT);
    gpio_out(LED_BANK, LED_PINA, LOW);
    gpio_dir(LED_BANK, LED_PINB, INPUT);
    gpio_out(LED_BANK, LED_PINB, LOW);
    // initialize the digital pin as an output.
    SerialUSB.begin();
}

void loop()
{
    if (SerialUSB.available()) {
        REPL_receiveHandler(SerialUSB.read());
    }
}

static int led_commandHandler(size_t argc, char* argv[])
{
    static char state = '0';
    if (1 < argc) {
        char newstate = argv[1][0];
        switch (newstate) {
        case '0': {
            gpio_dir(LED_BANK, LED_PINA, INPUT);
            gpio_out(LED_BANK, LED_PINA, LOW);
            gpio_dir(LED_BANK, LED_PINB, INPUT);
            gpio_out(LED_BANK, LED_PINB, LOW);
            state = newstate;
        } break;
        case 'A': {
            gpio_dir(LED_BANK, LED_PINA, OUTPUT);
            gpio_out(LED_BANK, LED_PINA, LOW);
            gpio_dir(LED_BANK, LED_PINB, INPUT);
            gpio_out(LED_BANK, LED_PINB, HIGH);
            state = newstate;
        } break;
        case 'B': {
            gpio_dir(LED_BANK, LED_PINA, INPUT);
            gpio_out(LED_BANK, LED_PINA, HIGH);
            gpio_dir(LED_BANK, LED_PINB, OUTPUT);
            gpio_out(LED_BANK, LED_PINB, LOW);
            state = newstate;
        } break;
        default: {

        } break;
        }
    } else {
        SerialUSB.write(state);
        SerialUSB.print(NEWLINE);
    }
    return 0;
}

const REPL_Command_t SystemCommands[] = {
    {
        .name = "led",
        .help = "set led",
        .handler = &led_commandHandler,
    },
};

const REPL_CommandList_t SystemCommandList = REPL_COMMAND_LIST(SystemCommands);
