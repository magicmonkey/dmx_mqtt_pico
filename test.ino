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
#include "pico/stdlib.h"
#include "pico/mutex.h"
#include "wifi_password.h"

// Configuration
// Note: DMX_UNIVERSE_SIZE is defined in DmxOutput.h (512 channels)

// WiFi Configuration
const char* ssid = "cyan_nomap";
const char* password = WIFI_PASSWORD; // from wifi_password.h

// MQTT Configuration
const char* mqtt_server = "10.1.0.1";
const int mqtt_port = 1883;
const char* mqtt_topic_dmx = "/ledbar/0";
const char* mqtt_topic_fadetime = "/ledbar/0/fadetime";

// Create DMX output instance
DmxOutput dmxOutput;

// DMX data buffer (one full universe)
uint8_t dmxData[DMX_UNIVERSE_SIZE];

// Fade variables
uint8_t initialDmxData[DMX_UNIVERSE_SIZE];
uint8_t targetDmxData[DMX_UNIVERSE_SIZE];
uint32_t fadeTime = 0;          // Fade duration in milliseconds
uint32_t fadeStartTime = 0;     // Time when fade started
uint32_t fadeTargetTime = 0;    // Time when fade should complete

// Mutex for protecting dmxData access between cores
mutex_t dmxMutex;

// WiFi and MQTT clients
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Function declaration from dmx_core1.cpp
void setDmxTarget(const uint8_t* newTargetData);

// MQTT callback function to handle incoming messages
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // Create a null-terminated string from the payload
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  // Check which topic this message is for
  if (strcmp(topic, mqtt_topic_fadetime) == 0) {
    // Handle fade time update
    int newFadeTime = atoi(message);
    if (newFadeTime >= 0) {
      mutex_enter_blocking(&dmxMutex);
      fadeTime = (uint32_t)newFadeTime;
      mutex_exit(&dmxMutex);
      Serial.print("Fade time set to: ");
      Serial.print(fadeTime);
      Serial.println(" ms");
    }
  } else if (strcmp(topic, mqtt_topic_dmx) == 0) {
    // Handle DMX data update with fade
    // Create temporary buffer for new target values
    uint8_t newTarget[DMX_UNIVERSE_SIZE];

    // Initialize with current DMX values (in case message doesn't include all channels)
    mutex_enter_blocking(&dmxMutex);
    memcpy(newTarget, dmxData, DMX_UNIVERSE_SIZE);
    mutex_exit(&dmxMutex);

    // Parse comma-separated values into temporary buffer
    char* token = strtok(message, ",");
    int channelIndex = 1;  // DMX channels start at 1

    while (token != NULL && channelIndex < DMX_UNIVERSE_SIZE) {
      // Trim whitespace
      while (*token == ' ') token++;

      // Check if the token is not blank/empty
      if (*token != '\0') {
        // Parse the value and update the temporary target buffer
        int value = atoi(token);
        if (value >= 0 && value <= 255) {
          newTarget[channelIndex] = (uint8_t)value;
        }
      }

      channelIndex++;
      token = strtok(NULL, ",");
    }

    // Set new target and initiate fade
    setDmxTarget(newTarget);
  }
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

      // Subscribe to both topics
      mqttClient.subscribe(mqtt_topic_dmx);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic_dmx);

      mqttClient.subscribe(mqtt_topic_fadetime);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic_fadetime);
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
  delay(2000);
  Serial.println("DMX MQTT Controller Starting...");

  // Initialize mutex for dmxData protection
  mutex_init(&dmxMutex);

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
}

