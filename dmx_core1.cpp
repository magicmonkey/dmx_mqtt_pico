/*
 * Core 1 Functions - DMX Output Handler
 *
 * This file contains the setup1() and loop1() functions that run on Core 1
 * of the Raspberry Pi Pico W. Core 1 handles DMX output with fade interpolation.
 */

#include <DmxOutput.h>
#include "pico/stdlib.h"
#include "pico/mutex.h"

// External references to global variables defined in test.ino
extern DmxOutput dmxOutput;
extern uint8_t dmxData[DMX_UNIVERSE_SIZE];
extern uint8_t initialDmxData[DMX_UNIVERSE_SIZE];
extern uint8_t targetDmxData[DMX_UNIVERSE_SIZE];
extern uint32_t fadeStartTime;
extern uint32_t fadeTargetTime;
extern mutex_t dmxMutex;

const uint DMX_TX_PIN = 0;        // GPIO pin for DMX transmission

void setup1() {
  // Initialize DMX output
  dmxOutput.begin(DMX_TX_PIN);

  // Initialize DMX data buffers to zero
  memset(dmxData, 0, DMX_UNIVERSE_SIZE);
  memset(initialDmxData, 0, DMX_UNIVERSE_SIZE);
  memset(targetDmxData, 0, DMX_UNIVERSE_SIZE);

  dmxOutput.write(dmxData, DMX_UNIVERSE_SIZE);
  while (dmxOutput.busy());

}

void loop1() {
  // Local buffers for fade calculation
  uint8_t localInitial[DMX_UNIVERSE_SIZE];
  uint8_t localTarget[DMX_UNIVERSE_SIZE];
  uint8_t localDmxData[DMX_UNIVERSE_SIZE];
  uint32_t localStartTime, localTargetTime;

  // Get current time
  uint32_t currentTime = millis();

  // Lock mutex to read fade parameters
  mutex_enter_blocking(&dmxMutex);

  // Copy fade parameters to local variables
  memcpy(localInitial, initialDmxData, DMX_UNIVERSE_SIZE);
  memcpy(localTarget, targetDmxData, DMX_UNIVERSE_SIZE);
  localStartTime = fadeStartTime;
  localTargetTime = fadeTargetTime;

  // Unlock mutex after copying
  mutex_exit(&dmxMutex);

  // Calculate interpolation factor
  float factor;
  if (currentTime >= localTargetTime) {
    // Fade complete, use target values
    factor = 1.0;
  } else if (currentTime <= localStartTime) {
    // Fade not started, use initial values
    factor = 0.0;
  } else {
    // In the middle of fade, interpolate
    uint32_t elapsed = currentTime - localStartTime;
    uint32_t duration = localTargetTime - localStartTime;
    factor = (float)elapsed / (float)duration;
  }

  // Calculate interpolated DMX values
  for (int i = 0; i < DMX_UNIVERSE_SIZE; i++) {
    int interpolated = localInitial[i] + (int)(factor * (localTarget[i] - localInitial[i]));
    localDmxData[i] = (uint8_t)interpolated;
  }

  // Lock mutex to update dmxData with current interpolated values
  mutex_enter_blocking(&dmxMutex);
  memcpy(dmxData, localDmxData, DMX_UNIVERSE_SIZE);
  mutex_exit(&dmxMutex);

  // Send DMX universe (using local copy)
  dmxOutput.write(localDmxData, DMX_UNIVERSE_SIZE);
  while (dmxOutput.busy());

  // Small delay to prevent overwhelming the system
  delay(10);
}
