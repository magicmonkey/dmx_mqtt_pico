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
  // Initialize serial communication
  Serial.begin(115200);
  delay(2000);
  Serial.println("Pico W DMX Output Example");
  Serial.println("========================");

  // Initialize DMX output
  dmxOutput.begin(DMX_TX_PIN);
  Serial.print("DMX output initialized on GPIO ");
  Serial.println(DMX_TX_PIN);

  // Initialize DMX data buffer to zero
  memset(dmxData, 0, DMX_UNIVERSE_SIZE);

  Serial.println("Starting DMX transmission...");
  Serial.println("Ready!");
}

void loop() {
  // Example 1: Fade a single channel (channel 1) up and down
  static uint8_t brightness = 0;
  static int8_t direction = 1;

  brightness += direction;

  if (brightness == 255) {
    direction = -1;
  } else if (brightness == 0) {
    direction = 1;
  }

  // Set channel 1 to the current brightness
  dmxData[0] = brightness;

  // Example 2: Set RGB channels (channels 2-4) to a color
  // Uncomment to use:
  // dmxData[1] = 255;  // Red
  // dmxData[2] = 0;    // Green
  // dmxData[3] = 128;  // Blue

  // Send DMX data
  dmxOutput.write(dmxData, DMX_UNIVERSE_SIZE);

  // Print status every 25 cycles (~1 second at 40ms delay)
  static uint8_t counter = 0;
  if (counter++ >= 25) {
    Serial.print("Channel 1: ");
    Serial.print(brightness);
    Serial.println(" (0-255)");
    counter = 0;
  }

  delay(40); // ~25 Hz update rate
}
