tiny85 Examples
===============

This repository contains examples for the ATtiny85.

# Hardware
This projects supports two hardware setups - either with an [ISP](#isp-setup) or via [USB](#usb-setup).
I recommend the USB setup due to its ease of use.

## ISP Setup
For this you only need an AVR **I**n **S**system **P**rogrammer like the [USBasp](https://www.fischl.de/usbasp/).
This setup is only necessary if you currently can't assemble an [USB setup](#usb-setup) or because the ATtiny doesn't have the [bootloader](#bootloader) yet.

The schematic are simple connections between the programmer and the micro controller.
```
    ┌─────────────────────┐
    │                     │
    │                    ┌┘
    │                    │
    │     ┌──────────┐ ┌─┼──────────────────────────┐
┌───┼─────┤MOSI   VCC├─┘ │                          │
│   │     │          │   └┐RST 1┌────────┐8 VCC     │
│   │    ─┤GND    TXD├─   └─────┤PB5  VCC├──────────┘
│   │     │          │         2│        │7 SCK
│   └─────┤RESET  RXD├─       ──┤PB3  PB2├──────────────┐
│         │          │         3│        │6 MISO        │
│   ┌─────┤SCK    GND├─       ──┤PB4  PB1├───────────┐  │
│   │     │          │     GND 4│        │5 MOSI     │  │
│   │   ┌─┤MISO   GND├──────────┤GND  PB0├───────┐   │  │
│   │   │ └──────────┘          └────────┘       │   │  │
│   │   │ USBasp                ATtiny85         │   │  │
│ ┌─┼───┼─┐                                      │   │  │
└─┘ │   │ └──────────────────────────────────────┘   │  │
    │   │                                            │  │
    │   └────────────────────────────────────────────┘  │
    │                                                   │
    └───────────────────────────────────────────────────┘
```

## USB Setup
This is based on the [Digispark](https://digistump.com/wiki/digispark/tutorials/digispark) schematic and requires the [bootloader](#bootloader) to be flashed to the ATtiny85.
If necessary, flash the bootloader with the [ISP setup](#isp-setup).

For the USB setup you need the following components:
- an USB connector
- one 1.5kΩ resistor
- two 68Ω resistors
- two 3V Zener diodes

Connect the components according to the schematic below.
```
┌─────┐
│ VBUS├──┬──────────────────────────────────────┐
│     │  │                                      │
│     │  │ ┌───┐                                │
│     │  └─┤1k5├─┐        ┌───┐                 │
│   D-├─┐  └───┘ ├────┬───┤68R├───┐             │
│     │ │        │    │   └───┘   │ 1┌────────┐8│
│     │ └────────┘    │           │ ─┤PB5  VCC├─┘
│     │             ┌─┼─┐ ┌───┐   │ 2│        │7
│   D+├───────┬─────┘ │ └─┤68R├─┐ └──┤PB3  PB2├─
│     │       │       │   └───┘ │   3│        │6
│     │ ┌─|>l─┘ ┌─|>l─┘         └────┤PB4  PB1├─
│     │ │ 3V    │ 3V                4│        │5
│  GND├─┴───────┴────────────────────┤GND  PB0├─
└─────┘                              └────────┘
USB                                  ATtiny85
```

# Software
In the following I want to describe the step necessary to build flash the examples.

## Cloning
Clone this repository recursively.
```bash
git clone --recurse-submodules https://github.com/cyber-murmel/tiny85-examples.git
```

## Prerequisites
To build this project you need to install
- [avr-gcc](https://gcc.gnu.org/wiki/avr-gcc)
- [avrdude](https://www.nongnu.org/avrdude/)
- [micronucleus cli](https://github.com/micronucleus/micronucleus/tree/master/commandline)
- [pySerial](https://pyserial.readthedocs.io/en/latest/pyserial.html)

For automatic code formatting you can also install [clang](https://clang.llvm.org/).

### Nix
If you are using Nix(OS) you can simply run `nix-shell` to enter a development enviroment.

## Bootloader
To build and flash the bootloader, run
```bash
make -C micronucleus/firmware/ CONFIG=t85_default
```
hook up hook up the [ISP setup](#isp-setup) and run the following - depending on your permission with `sudo`
```bash
make -C micronucleus/firmware/ CONFIG=t85_default flash
```

## Firmware
To build and upload an actual firmware example, first select a target from the [`src/`](src/) directory, then build it with
```bash
make TARGET=<target name>
# e.g.
make TARGET=blink
```
To then upload the binary run
```bash
make TARGET=<target name> upload
```
This also (re-)builds the firmware if necessary.

### Cleanup
If you build another target or something seems wrong, you are well acvised to clean up the old build files first.
Do this by running `make clean`.

# Credits
Bootloader:
- [micronucleus](https://github.com/micronucleus/micronucleus#credits)

USB:
- [digistump](https://github.com/digistump/DigistumpArduino)
