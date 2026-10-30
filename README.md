# Raspberry Pi MIDI Bridge for SAM2695

A MIDI bridge that allows a Raspberry Pi Zero 2 W to play sounds using the SAM2695 sound module via an FT232R USB-to-UART converter.

The program receives MIDI messages through the ALSA MIDI Sequencer and forwards them to the SAM2695 using a serial UART connection.

## Features

- Receives MIDI messages through the ALSA MIDI Sequencer.
- Sends MIDI messages to the SAM2695 via USB-UART.
- Uses the FT232R USB-to-UART converter.
- Supports MIDI standard baud rate of 31,250 bps.
- Supports MIDI keyboards and MIDI file playback.
- Allows MIDI port connections using `aconnect` and `pmidi`.

## Hardware Configuration

- Raspberry Pi Zero 2 W
- USB Hub
- FT232R USB-to-UART converter
- M5Stack Unit MIDI (SAM2695)

### Connection Diagram

```text
Raspberry Pi Zero 2 W
        |
        | USB
        v
     USB Hub
        |
        | USB
        v
      FT232R
   (USB-to-UART)
        |
        | TXD
        | 31,250 bps
        | 8N1
        v
   M5Stack Unit MIDI
      (SAM2695)
        |
        v
    Audio Output
```

Connect the FT232R TXD pin to the UART_RX pin of the SAM2695 module. Connect GND between the two devices.

## How It Works

1. The Raspberry Pi receives MIDI messages through the ALSA MIDI Sequencer.
2. `miditoserial` reads incoming MIDI events.
3. MIDI messages are forwarded to the FT232R through USB.
4. The FT232R converts USB data into UART serial signals.
5. The SAM2695 receives the MIDI messages and generates sound.

## Requirements

- Raspberry Pi OS
- Raspberry Pi Zero 2 W
- FT232R USB-to-UART converter
- M5Stack Unit MIDI (SAM2695)
- GCC compiler
- GNU Make
- ALSA development libraries

## Installation

Clone the repository:

```bash
git clone <repository-url>
cd <repository-directory>
```

Compile the program:

```bash
make
```

This creates the executable:

```text
miditoserial
```

## Usage

Connect the FT232R USB-to-UART converter to the Raspberry Pi.

Check the serial device:

```bash
ls /dev/ttyUSB*
```

Run the program:

```bash
./miditoserial
```

The program receives MIDI messages through the ALSA MIDI Sequencer and sends them to the SAM2695.

## MIDI Communication

The serial communication parameters are:

| Parameter | Value |
|---|---|
| Baud rate |
