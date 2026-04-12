# Pharmacy Cross Display
<p align="left">
    <a href="https://github.com/vphilippoz/pharmacy/releases/tag/v1.1" alt="Latest stable version">
        <img src="https://img.shields.io/badge/version-1.1-blue" /></a>
</p>

A miniature pharmacy cross display made of LED matrices.

<p align="center">
    <img src="https://raw.githubusercontent.com/vphilippoz/pharmacy/master/media/cross_animation.gif"
        height="300">
</p>

This is a fun project to build and decorate you home, or bring at a rave party! The software runs on a ESP32 and controls 5 square LED matrices.

# Features

In its current version, the project has:

- 8 animations
- Button to switch between animations
- Potentiometer to tune brightness and animation speed
- Python script to easily create animation bitmap from a .gif file
- Excel file to easily create animation bitmap from a drawing (in excel file)

Available animations are:

1. Always ON
1. Blink
1. Random (White noise)
1. Cross shrinking
1. Girl dance
1. Fortnite dance
1. Spinning fish
1. Spiral

# Usage

Plug the power connector. The device starts automatically. A short press on the button changes the animation. Turning the knob allows to tune the animation speed. One cycle is between 80 BPM and 250 BPM. When holding the button, the knob controlls the display brightness.

# BOM

This list is not intended to be a shopping list. It enumerates everything needed to make the Pharmacy Cross Display, but use what you have and adapt the design as needed.

## Electronics

<p align="center">
    <img src="https://raw.githubusercontent.com/vphilippoz/pharmacy/master/media/elec_diagram.png"
        height="200">
</p>

- 5V power supply
- Custom-made PCBs (one of each)
- ESP32-C6-DevKit
- 3 HT16K33 modules (Adafruit 1427)
- 5 green LED matrices (Adafruit 1623)
- 1 10kOhm rotary potentiometer
- 1 THT 10kOhm resistor
- 1 push button
- 1 barrel power connector
- Cables
- Headers for ESP32, HT16k33 and peripherals

## Casing

- 4-parts 3D printed custom casing. I printed mine in black PETG.
- 2 M2.5x4 socket head cap screws
- 10 M2.5x8 socket head cap screws
- Optional: 6 M2.5 washers

## Tools

- 3D printer
- 2mm allen key
- Soldering iron
- Solder
- Set of pliers
