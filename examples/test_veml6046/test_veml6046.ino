/*
 * Test sketch for VEML6046 RGBIR color sensor
 *
 * Written by Limor 'ladyada' Fried with assistance from Claude Code for
 * Adafruit Industries. MIT license, check license.txt for more information
 */

#include "Adafruit_VEML6046.h"

Adafruit_VEML6046 veml = Adafruit_VEML6046();
uint16_t sample_delay_ms = 30; // Will be calculated in setup

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println(F("VEML6046 test"));

  if (!veml.begin()) {
    Serial.println(F("Couldn't find VEML6046 chip"));
    while (1)
      delay(10);
  }
  Serial.println(F("Found VEML6046 chip"));

  // Print current status (set by begin())
  bool is_enabled = veml.isEnabled();
  Serial.print(F("Sensor enabled: "));
  Serial.println(is_enabled ? F("Yes") : F("No"));

  bool cal_enabled = veml.getRGBCalibration();
  Serial.print(F("RGB calibration enabled: "));
  Serial.println(cal_enabled ? F("Yes") : F("No"));

  // Test photodiode size
  Serial.println(F("Setting photodiode to full size"));
  if (veml.setPhotoDiodeHalfSize(false)) {
    Serial.println(F("Photodiode full size set successfully"));
  } else {
    Serial.println(F("Failed to set photodiode full size"));
  }

  // Read back photodiode size
  bool is_half_size = veml.getPhotoDiodeHalfSize();
  Serial.print(F("Photodiode half size: "));
  Serial.println(is_half_size ? F("Yes") : F("No"));

  // Test integration time setter/getter
  Serial.println(F("Setting integration time to 25ms"));
  if (veml.setIntegrationTime(VEML6046_IT_25MS)) {
    Serial.println(F("Integration time set successfully"));
  } else {
    Serial.println(F("Failed to set integration time"));
  }

  // Read back and display current integration time
  veml6046_integration_time_t current_it = veml.getIntegrationTime();
  Serial.print(F("Current integration time: "));
  switch (current_it) {
    case VEML6046_IT_3_125MS:
      Serial.println(F("3.125ms"));
      break;
    case VEML6046_IT_6_25MS:
      Serial.println(F("6.25ms"));
      break;
    case VEML6046_IT_12_5MS:
      Serial.println(F("12.5ms"));
      break;
    case VEML6046_IT_25MS:
      Serial.println(F("25ms"));
      break;
    case VEML6046_IT_50MS:
      Serial.println(F("50ms"));
      break;
    case VEML6046_IT_100MS:
      Serial.println(F("100ms"));
      break;
    case VEML6046_IT_200MS:
      Serial.println(F("200ms"));
      break;
    case VEML6046_IT_400MS:
      Serial.println(F("400ms"));
      break;
    default:
      Serial.println(F("Unknown"));
      break;
  }

  // Test RGB mode setter/getter
  Serial.println(F("Setting RGB mode to auto"));
  if (veml.setRGBModeForced(false)) {
    Serial.println(F("RGB mode set successfully"));
  } else {
    Serial.println(F("Failed to set RGB mode"));
  }

  // Read back and display current RGB mode
  bool is_forced = veml.getRGBModeForced();
  Serial.print(F("Current RGB mode: "));
  Serial.println(is_forced ? F("Forced") : F("Auto"));

  // Test green interrupt enable
  Serial.println(F("Enabling green interrupt"));
  if (veml.setGreenIntEnabled(true)) {
    Serial.println(F("Green interrupt enabled successfully"));
  } else {
    Serial.println(F("Failed to enable green interrupt"));
  }

  // Read back green interrupt status
  bool green_int_enabled = veml.getGreenIntEnabled();
  Serial.print(F("Green interrupt enabled: "));
  Serial.println(green_int_enabled ? F("Yes") : F("No"));

  // Test RGB gain setter/getter
  Serial.println(F("Setting RGB gain to 1x"));
  if (veml.setRGBGain(VEML6046_GAIN_1X)) {
    Serial.println(F("RGB gain set successfully"));
  } else {
    Serial.println(F("Failed to set RGB gain"));
  }

  // Read back and display current RGB gain
  veml6046_gain_t current_gain = veml.getRGBGain();
  Serial.print(F("Current RGB gain: "));
  switch (current_gain) {
    case VEML6046_GAIN_1X:
      Serial.println(F("1x"));
      break;
    case VEML6046_GAIN_2X:
      Serial.println(F("2x"));
      break;
    case VEML6046_GAIN_0_66X:
      Serial.println(F("0.66x"));
      break;
    case VEML6046_GAIN_0_5X:
      Serial.println(F("0.5x"));
      break;
    default:
      Serial.println(F("Unknown"));
      break;
  }

  // Test interrupt persistence setter/getter
  Serial.println(F("Setting interrupt persistence to 1"));
  if (veml.setIntPersistence(VEML6046_PERS_1)) {
    Serial.println(F("Interrupt persistence set successfully"));
  } else {
    Serial.println(F("Failed to set interrupt persistence"));
  }

  // Read back and display current persistence
  veml6046_persistence_t current_pers = veml.getIntPersistence();
  Serial.print(F("Current interrupt persistence: "));
  switch (current_pers) {
    case VEML6046_PERS_1:
      Serial.println(F("1 time"));
      break;
    case VEML6046_PERS_2:
      Serial.println(F("2 times"));
      break;
    case VEML6046_PERS_4:
      Serial.println(F("4 times"));
      break;
    case VEML6046_PERS_8:
      Serial.println(F("8 times"));
      break;
    default:
      Serial.println(F("Unknown"));
      break;
  }

  // Test green threshold setter/getter
  Serial.println(F("Setting green thresholds: low=1000, high=10000"));
  if (veml.setGreenThresholdLow(1000)) {
    Serial.println(F("Green low threshold set successfully"));
  } else {
    Serial.println(F("Failed to set green low threshold"));
  }

  if (veml.setGreenThresholdHigh(10000)) {
    Serial.println(F("Green high threshold set successfully"));
  } else {
    Serial.println(F("Failed to set green high threshold"));
  }

  // Read back and display current thresholds
  uint16_t low_thresh = veml.getGreenThresholdLow();
  uint16_t high_thresh = veml.getGreenThresholdHigh();
  Serial.print(F("Current green thresholds - Low: "));
  Serial.print(low_thresh);
  Serial.print(F(", High: "));
  Serial.println(high_thresh);

  // Calculate optimal delay based on current integration time + 5ms buffer
  current_it = veml.getIntegrationTime();
  sample_delay_ms = 5; // Base 5ms buffer
  switch (current_it) {
    case VEML6046_IT_3_125MS:
      sample_delay_ms += 4;
      break; // 3.125ms rounded up
    case VEML6046_IT_6_25MS:
      sample_delay_ms += 7;
      break; // 6.25ms rounded up
    case VEML6046_IT_12_5MS:
      sample_delay_ms += 13;
      break; // 12.5ms rounded up
    case VEML6046_IT_25MS:
      sample_delay_ms += 25;
      break;
    case VEML6046_IT_50MS:
      sample_delay_ms += 50;
      break;
    case VEML6046_IT_100MS:
      sample_delay_ms += 100;
      break;
    case VEML6046_IT_200MS:
      sample_delay_ms += 200;
      break;
    case VEML6046_IT_400MS:
      sample_delay_ms += 400;
      break;
    default:
      sample_delay_ms += 25;
      break; // Default fallback
  }
  Serial.print(F("Using sample delay: "));
  Serial.print(sample_delay_ms);
  Serial.println(F("ms"));
}

void loop() {
  // Read RGBIR data in continuous mode
  uint16_t r, g, b, ir;
  if (veml.getData(&r, &g, &b, &ir)) {
    float lux = veml.calculateLux(g);
    Serial.print(F("R: "));
    Serial.print(r);
    Serial.print(F(" G: "));
    Serial.print(g);
    Serial.print(F(" B: "));
    Serial.print(b);
    Serial.print(F(" IR: "));
    Serial.print(ir);
    Serial.print(F(" Lux: "));
    Serial.println(lux, 2);
  } else {
    Serial.println(F("Failed to read data"));
  }

  // If in forced mode, trigger next reading
  if (veml.getRGBModeForced()) {
    veml.RGBTrigger();
  }

  delay(sample_delay_ms);
}