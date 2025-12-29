# Raspberry Pi Pico W Arduino Project

This is an Arduino project for the Raspberry Pi Pico W board.

## Hardware

- **Board**: Raspberry Pi Pico W
- **Microcontroller**: RP2040
- **WiFi**: 2.4GHz wireless (802.11n)
- **Built-in LED**: GPIO 25

## Setup

### Arduino CLI (Recommended)

Using the provided Makefile with Arduino CLI:

1. **Install board support** (one-time setup):
   ```bash
   make install
   ```

2. **Compile the sketch**:
   ```bash
   make compile
   ```

3. **Upload to board**:
   ```bash
   make upload
   ```

4. **Monitor serial output**:
   ```bash
   make monitor
   ```

5. **Compile and upload in one step**:
   ```bash
   make all
   ```

**Other useful commands**:
- `make help` - Show all available commands
- `make boards` - List connected boards
- `make clean` - Clean build files

### Arduino IDE Configuration

1. **Install Board Support**:
   - Open Arduino IDE
   - Go to `File > Preferences`
   - Add this URL to "Additional Board Manager URLs":
     ```
     https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
     ```
   - Go to `Tools > Board > Boards Manager`
   - Search for "pico" and install "Raspberry Pi Pico/RP2040"

2. **Select Board**:
   - Go to `Tools > Board > Raspberry Pi RP2040 Boards`
   - Select "Raspberry Pi Pico W"

3. **Configure Port**:
   - Connect your Pico W via USB
   - Go to `Tools > Port` and select the appropriate port

## Building and Uploading

1. Open `test.ino` in Arduino IDE
2. Click the Upload button or press `Ctrl+U` (Windows/Linux) / `Cmd+U` (Mac)
3. The sketch will compile and upload to your Pico W

## Current Project

The current sketch is a simple LED blink program that:
- Initializes serial communication at 115200 baud
- Blinks the built-in LED every second
- Prints LED status to the serial monitor

## Next Steps

- Add WiFi connectivity using the WiFi library
- Implement your custom functionality
- Add additional sensors or components as needed
