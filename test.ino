/*
 * Raspberry Pi Pico W - DMX512 Output Example
 *
 * This example demonstrates basic DMX output using the Pico-DMX library.
 * It sends DMX data to control lighting fixtures.
 *
 * Hardware Requirements:
 * - Raspberry Pi Pico W
 * - RS485 transceiver module (e.g., MAX485)
 * - DMX lighting fixture(s)
 *
 * Wiring:
 * - Pico GPIO Pin → RS485 DI (Data Input)
 * - RS485 A → DMX Pin 3 (DMX+)
 * - RS485 B → DMX Pin 2 (DMX-)
 * - GND → DMX Pin 1 (GND)
 *
 * Library: https://github.com/jostlowe/Pico-DMX
 */

#include <DmxOutput.h>

// Configuration
const uint DMX_TX_PIN = 0;        // GPIO pin for DMX transmission
// Note: DMX_UNIVERSE_SIZE is defined in DmxOutput.h (512 channels)

// Create DMX output instance
DmxOutput dmxOutput;

// DMX data buffer (one full universe)
uint8_t dmxData[DMX_UNIVERSE_SIZE];

void setup() {
  // Initialize DMX output
  dmxOutput.begin(DMX_TX_PIN);

  // Initialize DMX data buffer to zero
  memset(dmxData, 0, DMX_UNIVERSE_SIZE);

  dmxOutput.write(dmxData, DMX_UNIVERSE_SIZE);
  while (dmxOutput.busy());
}

void loop() {

  /*
  dmxData[1] = 100;
  dmxData[2] = 20;
  dmxData[3] = 200;

  dmxData[5] = 100;
  dmxData[6] = 20;
  dmxData[7] = 200;

  dmxData[9] = 100;
  dmxData[10] = 20;
  dmxData[11] = 200;

  dmxData[13] = 100;
  dmxData[14] = 20;
  dmxData[15] = 200;

  dmxData[17] = 100;
  dmxData[18] = 20;
  dmxData[19] = 200;

  dmxData[21] = 100;
  dmxData[22] = 20;
  dmxData[23] = 200;
  */

  /*
  dmxData[25] = 20;
  dmxData[26] = 20;
  dmxData[27] = 20;
  dmxData[28] = 20;
  dmxData[29] = 20;
  dmxData[30] = 20;
  dmxData[31] = 20;
  dmxData[32] = 20;
  dmxData[33] = 20;
  dmxData[34] = 20;
  dmxData[35] = 20;
  dmxData[36] = 20;
  dmxData[37] = 20;
  */

  dmxOutput.write(dmxData, DMX_UNIVERSE_SIZE);
  while (dmxOutput.busy());
  delay(10);
}
