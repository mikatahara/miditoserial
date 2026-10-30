# Raspberry Pi MIDI Bridge for SAM2695

A MIDI bridge that allows a Raspberry Pi Zero 2 W to play sounds using the SAM2695 sound module via an FT232R USB-to-UART converter.

The program receives MIDI messages through the ALSA MIDI Sequencer and forwards them to the SAM2695 using a serial UART connection.

## Features

- Receives MIDI messages through the ALSA MIDI Sequencer.
- Sends MIDI messages to the SAM2695 via USB-UART.
- Uses the FT232R USB-to-UART converter.
- Supports the standard MIDI baud rate of 31,250 bps.
- Supports MIDI keyboards and MIDI file playback.
- Allows MIDI port connections using `aconnect` and `pmidi`.

## Hardware Configuration

The system consists of the following components:

- Raspberry Pi Zero 2 W
- USB OTG adapter / USB Hub
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

The FT232R TXD pin is connected to the UART_RX pin of the SAM2695 module. The GND connections must also be shared.

## How It Works

1. The Raspberry Pi receives MIDI messages through the ALSA MIDI Sequencer.
2. The program reads incoming MIDI events.
3. MIDI messages are forwarded to the FT232R through USB.
4. The FT232R converts USB data into UART serial signals.
5. The SAM2695 receives the MIDI messages and generates sound.

## Requirements

- Raspberry Pi OS (64-bit)
- Raspberry Pi Zero 2 W
- FT232R USB-to-UART converter
- M5Stack Unit MIDI (SAM2695)
- GCC compiler
- ALSA development libraries

## MIDI Communication

The SAM2695 is controlled using standard MIDI serial communication.

| Parameter | Value |
|---|---|
| Baud rate | 31,250 bps |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Format | 8N1 |

Since 31,250 bps is a non-standard baud rate in Linux, this program uses the Linux-specific `termios2` interface and `BOTHER` to configure the serial port.

## MIDI Connections

The program uses the ALSA MIDI Sequencer, so MIDI input sources can be connected using `aconnect`.

List available MIDI ports:

```bash
aconnect -l
```

Connect a USB MIDI keyboard to the program:

```bash
aconnect <keyboard_port> <program_port>
```

MIDI files can also be played using `pmidi`:

```bash
pmidi -p <program_port> example.mid
```

## USB Serial Device

The FT232R is normally detected as:

```text
/dev/ttyUSB0
```

The actual device name may vary depending on the system.

## License

Specify your preferred open-source license here.
