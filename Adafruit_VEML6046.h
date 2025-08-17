/*!
 * @file Adafruit_VEML6046.h
 *
 * This is part of Adafruit's VEML6046 driver for the Arduino platform. It is
 * designed specifically to work with the Adafruit VEML6046 sensor:
 * https://www.adafruit.com/product/xxxx
 *
 * These sensors use I2C to communicate, 2 pins are required to interface.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor 'ladyada' Fried with assistance from Claude Code for
 * Adafruit Industries.
 *
 * MIT license, all text here must be included in any redistribution.
 */

#ifndef _ADAFRUIT_VEML6046_H
#define _ADAFRUIT_VEML6046_H

#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>

#include "Arduino.h"

/*! Default I2C address for VEML6046 */
#define VEML6046_DEFAULT_ADDR 0x29

/*! VEML6046 register addresses */
#define VEML6046_REG_RGB_CONF_0 0x00 ///< RGB configuration register 0
#define VEML6046_REG_RGB_CONF_1 0x01 ///< RGB configuration register 1
#define VEML6046_REG_G_THDH_L 0x04   ///< Green high threshold low byte
#define VEML6046_REG_G_THDH_H 0x05   ///< Green high threshold high byte
#define VEML6046_REG_G_THDL_L 0x06   ///< Green low threshold low byte
#define VEML6046_REG_G_THDL_H 0x07   ///< Green low threshold high byte
#define VEML6046_REG_R_DATA_L 0x10   ///< Red data low byte
#define VEML6046_REG_R_DATA_H 0x11   ///< Red data high byte
#define VEML6046_REG_G_DATA_L 0x12   ///< Green data low byte
#define VEML6046_REG_G_DATA_H 0x13   ///< Green data high byte
#define VEML6046_REG_B_DATA_L 0x14   ///< Blue data low byte
#define VEML6046_REG_B_DATA_H 0x15   ///< Blue data high byte
#define VEML6046_REG_IR_DATA_L 0x16  ///< IR data low byte
#define VEML6046_REG_IR_DATA_H 0x17  ///< IR data high byte
#define VEML6046_REG_ID_L 0x18       ///< Device ID low byte
#define VEML6046_REG_ID_H 0x19       ///< Device ID high byte
#define VEML6046_REG_INT_FLAG 0x1A   ///< Interrupt flag register

/*!
 * @brief Integration time settings for VEML6046
 */
typedef enum {
  VEML6046_IT_3_125MS = 0x00, ///< 3.125 ms integration time (default)
  VEML6046_IT_6_25MS = 0x01,  ///< 6.25 ms integration time
  VEML6046_IT_12_5MS = 0x02,  ///< 12.5 ms integration time
  VEML6046_IT_25MS = 0x03,    ///< 25 ms integration time
  VEML6046_IT_50MS = 0x04,    ///< 50 ms integration time
  VEML6046_IT_100MS = 0x05,   ///< 100 ms integration time
  VEML6046_IT_200MS = 0x06,   ///< 200 ms integration time
  VEML6046_IT_400MS = 0x07    ///< 400 ms integration time
} veml6046_integration_time_t;

/*!
 * @brief RGB gain settings for VEML6046
 */
typedef enum {
  VEML6046_GAIN_1X = 0x00,    ///< Gain x1 (default)
  VEML6046_GAIN_2X = 0x01,    ///< Gain x2
  VEML6046_GAIN_0_66X = 0x02, ///< Gain x0.66
  VEML6046_GAIN_0_5X = 0x03   ///< Gain x0.5
} veml6046_gain_t;

/*!
 * @brief Interrupt persistence settings for VEML6046
 */
typedef enum {
  VEML6046_PERS_1 = 0x00, ///< 1 time (default)
  VEML6046_PERS_2 = 0x01, ///< 2 times
  VEML6046_PERS_4 = 0x02, ///< 4 times
  VEML6046_PERS_8 = 0x03  ///< 8 times
} veml6046_persistence_t;

/*!
 * @brief Main VEML6046 class for RGBIR color sensor
 */
class Adafruit_VEML6046 {
 public:
  Adafruit_VEML6046();
  ~Adafruit_VEML6046();
  bool begin(uint8_t i2c_addr = VEML6046_DEFAULT_ADDR, TwoWire* wire = &Wire);

  bool setIntegrationTime(veml6046_integration_time_t it);
  veml6046_integration_time_t getIntegrationTime(void);

  bool setRGBModeForced(bool forced);
  bool getRGBModeForced(void);

  bool RGBTrigger(void);
  bool isTriggered(void);

  bool setGreenIntEnabled(bool enabled);
  bool getGreenIntEnabled(void);

  bool enable(void);
  bool isEnabled(void);

  bool setPhotoDiodeHalfSize(bool half_size);
  bool getPhotoDiodeHalfSize(void);

  bool setRGBGain(veml6046_gain_t gain);
  veml6046_gain_t getRGBGain(void);

  bool setIntPersistence(veml6046_persistence_t pers);
  veml6046_persistence_t getIntPersistence(void);

  bool setRGBCalibration(bool enabled);
  bool getRGBCalibration(void);

  bool setGreenThresholdHigh(uint16_t threshold);
  uint16_t getGreenThresholdHigh(void);

  bool setGreenThresholdLow(uint16_t threshold);
  uint16_t getGreenThresholdLow(void);

  bool getData(uint16_t* r, uint16_t* g, uint16_t* b, uint16_t* ir);

 private:
  Adafruit_I2CDevice* i2c_dev; ///< Pointer to I2C bus interface
};

#endif