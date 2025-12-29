/*
 * Raspberry Pi Pico W - DMX512 MQTT Controller
 *
 * This example receives DMX channel data via MQTT and outputs to DMX fixtures.
 * It connects to WiFi and subscribes to an MQTT topic for real-time control.
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
#include <WiFi.h>
#include <PubSubClient.h>

// Configuration
const uint DMX_TX_PIN = 0;        // GPIO pin for DMX transmission
// Note: DMX_UNIVERSE_SIZE is defined in DmxOutput.h (512 channels)

// WiFi Configuration
const char* ssid = "cyan_nomap";
const char* password = "";        // Add password if needed

// MQTT Configuration
const char* mqtt_server = "10.1.0.1";
const int mqtt_port = 1883;
const char* mqtt_topic = "/ledbar/0";

// Create DMX output instance
DmxOutput dmxOutput;

// DMX data buffer (one full universe)
uint8_t dmxData[DMX_UNIVERSE_SIZE];

// WiFi and MQTT clients
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// MQTT callback function to handle incoming messages
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Create a null-terminated string from the payload
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  // Parse comma-separated values
  char* token = strtok(message, ",");
  int channelIndex = 1;  // DMX channels start at index 0 (channel 1)

  while (token != NULL && channelIndex < DMX_UNIVERSE_SIZE) {
    // Trim whitespace
    while (*token == ' ') token++;

    // Check if the token is not blank/empty
    if (*token != '\0') {
      // Parse the value and update the DMX channel
      int value = atoi(token);
      if (value >= 0 && value <= 255) {
        dmxData[channelIndex] = (uint8_t)value;
        Serial.printf("Channel %d = %d\r\n", channelIndex, value);
      }
    }
    // If blank, we skip updating this channel (preserves existing value)
    //

    channelIndex++;
    token = strtok(NULL, ",");
  }

  Serial.println("---");

  // Send updated DMX data
  dmxOutput.write(dmxData, DMX_UNIVERSE_SIZE);
  while (dmxOutput.busy());
}

// Connect to WiFi
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

// Connect to MQTT broker
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT broker...");

    // Generate a unique client ID
    String clientId = "PicoW-DMX-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");

      // Subscribe to the topic
      mqttClient.subscribe(mqtt_topic);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  delay(1000);
  Serial.println("DMX MQTT Controller Starting...");

  // Initialize DMX output
  dmxOutput.begin(DMX_TX_PIN);

  // Initialize DMX data buffer to zero
  memset(dmxData, 0, DMX_UNIVERSE_SIZE);

  dmxOutput.write(dmxData, DMX_UNIVERSE_SIZE);
  while (dmxOutput.busy());

  // Connect to WiFi
  connectWiFi();

  // Setup MQTT client
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(mqttCallback);

  // Connect to MQTT broker
  connectMQTT();
}

void loop() {
  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  // Process incoming MQTT messages
  mqttClient.loop();

  // Small delay to prevent overwhelming the system
  delay(10);
}
