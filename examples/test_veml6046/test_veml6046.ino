/*
 * Test sketch for VEML6046 RGBIR color sensor
 * 
 * Written by Limor 'ladyada' Fried with assistance from Claude Code for Adafruit Industries.
 * MIT license, check license.txt for more information
 */

#include "Adafruit_VEML6046.h"

Adafruit_VEML6046 veml = Adafruit_VEML6046();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println(F("VEML6046 test"));
  
  if (!veml.begin()) {
    Serial.println(F("Couldn't find VEML6046 chip"));
    while (1) delay(10);
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
}

void loop() {
  // Read RGBIR data in continuous mode
  uint16_t r, g, b, ir;
  if (veml.getData(&r, &g, &b, &ir)) {
    Serial.print(F("R: ")); Serial.print(r);
    Serial.print(F(" G: ")); Serial.print(g);
    Serial.print(F(" B: ")); Serial.print(b);
    Serial.print(F(" IR: ")); Serial.println(ir);
  } else {
    Serial.println(F("Failed to read data"));
  }
  
  delay(1000);
}