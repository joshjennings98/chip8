# Josh's CHIP-8 Emulator

A simple CHIP-8 emulator written in C++.

- [Josh's CHIP-8 Emulator](#joshs-chip-8-emulator)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Acknowledgements](#acknowledgements)

## Prerequisites

This emulator utilises SDL2 for graphics. If you are using Ubuntu this can be easily installed by running `sudo apt-get install libsdl2-dev` in the terminal.

For other operating systems see [this page from the libsdl wiki](https://wiki.libsdl.org/Installation).


## Installation

Compile with `g++ main.cpp -o chip8 -l SDL2 `

## Usage

Run with `./chip8 <ROM> <width> <height>`

Alternatively, run `./chip8 <ROM>` for a window with `width=1024`, and `height=512`.

**ROMS**

This repo includes a two player game of pong at `pong.ch8`. 

For some more roms to try out, see [this GitHub repo](https://github.com/loktar00/chip8/tree/master/roms).

**Controls**

The keys on the keypad have been mapped to the keyboard in the following mapping:

```
Keypad                   Keyboard
+-+-+-+-+                +-+-+-+-+
|1|2|3|C|                |1|2|3|4|
+-+-+-+-+                +-+-+-+-+
|4|5|6|D|                |Q|W|E|R|
+-+-+-+-+       =>       +-+-+-+-+
|7|8|9|E|                |A|S|D|F|
+-+-+-+-+                +-+-+-+-+
|A|0|B|F|                |Z|X|C|V|
+-+-+-+-+                +-+-+-+-+
```

## Acknowledgements

These resources were helpful when I programmed this:
* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
* http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
* https://en.wikipedia.org/wiki/CHIP-8
* https://mir3z.github.io/chip8-emu/
* https://github.com/JamesGriffin/CHIP-8-Emulator

