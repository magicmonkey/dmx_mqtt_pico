/*
 * Raspberry Pi Pico W Arduino Project
 *
 * Board: Raspberry Pi Pico W
 *
 * To use this project:
 * 1. Install the Raspberry Pi Pico/RP2040 board support in Arduino IDE
 * 2. Select "Raspberry Pi Pico W" from Tools > Board
 * 3. Upload the sketch
 */

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect
  }

  Serial.println("Raspberry Pi Pico W - Starting...");

  // Initialize built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Blink the built-in LED
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON");
  delay(1000);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
