# Raspberry Pi Pico W Arduino Project

This is an Arduino project for the Raspberry Pi Pico W board.

## Hardware

- **Board**: Raspberry Pi Pico W
- **Microcontroller**: RP2040
- **WiFi**: 2.4GHz wireless (802.11n)
- **Built-in LED**: GPIO 25

## Libraries

This project includes the following Arduino libraries:

### Pico-DMX
- **Repository**: https://github.com/jostlowe/Pico-DMX
- **Purpose**: DMX512-A lighting control protocol support
- **Features**:
  - Send and receive DMX signals
  - Support for up to 8 parallel universes using PIO
  - Hardware-accelerated DMA transfers
  - Professional lighting control

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
- `make install-libs` - Install required libraries only
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

### test.ino - DMX512 Output Example

This sketch demonstrates basic DMX output using the Pico-DMX library:
- DMX universe transmission on GPIO 0
- Channel brightness control with smooth fade effect
- RGB color control example (commented out, ready to use)
- Serial status monitoring at 115200 baud

**Hardware Requirements**:
- RS485 transceiver module (e.g., MAX485, SN75176)
- DMX lighting fixtures
- Proper DMX cabling (3-pin or 5-pin XLR)

**Wiring**:
```
Pico GPIO 0  →  RS485 DI (Data Input)
Pico GND     →  RS485 GND
RS485 A      →  DMX Pin 3 (DMX+)
RS485 B      →  DMX Pin 2 (DMX-)
GND          →  DMX Pin 1 (GND)
```

**Usage**:
1. Connect your DMX hardware as shown above
2. Upload the sketch: `make upload`
3. Monitor serial output: `make monitor`
4. Watch channel 1 fade up and down on your DMX fixture

## Next Steps

- Configure DMX hardware and test lighting control
- Add WiFi connectivity for wireless DMX protocols (Art-Net, sACN)
- Implement DMX input for fixture monitoring
- Create custom lighting effects and sequences
