/*!
 * @file Adafruit_VEML6046.cpp
 *
 * @mainpage Adafruit VEML6046 RGBIR color sensor library
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for Adafruit's VEML6046 driver for the
 * Arduino platform.  It is designed specifically to work with the
 * Adafruit VEML6046 sensor: https://www.adafruit.com/product/xxxx
 *
 * These sensors use I2C to communicate, 2 pins are required to
 * interface.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor 'ladyada' Fried with assistance from Claude Code for
 * Adafruit Industries.
 *
 * @section license License
 *
 * MIT license, all text here must be included in any redistribution.
 */

#include "Adafruit_VEML6046.h"

/*!
 *  @brief  Instantiates a new VEML6046 class
 */
Adafruit_VEML6046::Adafruit_VEML6046() {
  i2c_dev = nullptr;
}

/*!
 *  @brief  Cleans up the VEML6046
 */
Adafruit_VEML6046::~Adafruit_VEML6046() {
  if (i2c_dev) {
    delete i2c_dev;
  }
}

/*!
 *  @brief  Sets up the hardware and initializes I2C
 *  @param  i2c_addr
 *          The I2C address to be used.
 *  @param  wire
 *          The Wire object to be used for I2C connections.
 *  @return True if initialization was successful, otherwise false.
 */
bool Adafruit_VEML6046::begin(uint8_t i2c_addr, TwoWire* wire) {
  if (i2c_dev) {
    delete i2c_dev;
  }

  i2c_dev = new Adafruit_I2CDevice(i2c_addr, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  // Check device ID
  Adafruit_BusIO_Register id_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_ID_L, 2, LSBFIRST);
  uint16_t chip_id = id_reg.read();

  if (chip_id != 0x0001) {
    return false;
  }

  // Enable the sensor
  if (!enable()) {
    return false;
  }

  // Enable RGB calibration
  if (!setRGBCalibration(true)) {
    return false;
  }

  return true;
}

/*!
 *  @brief  Sets the integration time for RGB measurements
 *  @param  it Integration time setting
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setIntegrationTime(veml6046_integration_time_t it) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits rgb_it_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 3, 4);

  return rgb_it_bits.write(it);
}

/*!
 *  @brief  Gets the current integration time setting
 *  @return Current integration time setting
 */
veml6046_integration_time_t Adafruit_VEML6046::getIntegrationTime(void) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits rgb_it_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 3, 4);

  return (veml6046_integration_time_t)rgb_it_bits.read();
}

/*!
 *  @brief  Sets the RGB measurement mode (auto vs forced)
 *  @param  forced True for active force mode, false for auto mode
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setRGBModeForced(bool forced) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits rgb_mode_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 1, 3);

  return rgb_mode_bits.write(forced ? 1 : 0);
}

/*!
 *  @brief  Gets the current RGB measurement mode
 *  @return True if in active force mode, false if in auto mode
 */
bool Adafruit_VEML6046::getRGBModeForced(void) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits rgb_mode_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 1, 3);

  return rgb_mode_bits.read() == 1;
}

/*!
 *  @brief  Triggers a measurement in active force mode
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::RGBTrigger(void) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits rgb_trig_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 1, 2);

  return rgb_trig_bits.write(1);
}

/*!
 *  @brief  Checks if measurement is still in progress (trigger bit set)
 *  @return True if measurement in progress, false if complete
 */
bool Adafruit_VEML6046::isTriggered(void) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits rgb_trig_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 1, 2);

  return rgb_trig_bits.read() == 1;
}

/*!
 *  @brief  Enables or disables green channel interrupt
 *  @param  enabled True to enable interrupt, false to disable
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setGreenIntEnabled(bool enabled) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits g_int_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 1, 1);

  return g_int_bits.write(enabled ? 1 : 0);
}

/*!
 *  @brief  Gets the green channel interrupt enable status
 *  @return True if interrupt enabled, false if disabled
 */
bool Adafruit_VEML6046::getGreenIntEnabled(void) {
  Adafruit_BusIO_Register rgb_conf0_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 1);
  Adafruit_BusIO_RegisterBits g_int_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf0_reg, 1, 1);

  return g_int_bits.read() == 1;
}

/*!
 *  @brief  Enables the VEML6046 sensor by clearing both RGB_ON bits
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::enable(void) {
  Adafruit_BusIO_Register rgb_conf_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits rgb_on_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf_reg, 2, 7);

  return rgb_on_bits.write(0x00);
}

/*!
 *  @brief  Checks if the VEML6046 sensor is enabled
 *  @return True if enabled, false if in shutdown
 */
bool Adafruit_VEML6046::isEnabled(void) {
  Adafruit_BusIO_Register rgb_conf_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_0, 2, MSBFIRST);
  Adafruit_BusIO_RegisterBits rgb_on_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf_reg, 2, 7);

  return rgb_on_bits.read() == 0x00;
}

/*!
 *  @brief  Sets the photodiode size (1/2 PD or 2/2 PD)
 *  @param  half_size True for 1/2 PD, false for 2/2 PD
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setPhotoDiodeHalfSize(bool half_size) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits pddiv_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 1, 6);

  return pddiv_bits.write(half_size ? 1 : 0);
}

/*!
 *  @brief  Gets the current photodiode size setting
 *  @return True if 1/2 PD, false if 2/2 PD
 */
bool Adafruit_VEML6046::getPhotoDiodeHalfSize(void) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits pddiv_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 1, 6);

  return pddiv_bits.read() == 1;
}

/*!
 *  @brief  Sets the RGB gain
 *  @param  gain RGB gain setting
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setRGBGain(veml6046_gain_t gain) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits gain_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 2, 3);

  return gain_bits.write(gain);
}

/*!
 *  @brief  Gets the current RGB gain setting
 *  @return Current RGB gain setting
 */
veml6046_gain_t Adafruit_VEML6046::getRGBGain(void) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits gain_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 2, 3);

  return (veml6046_gain_t)gain_bits.read();
}

/*!
 *  @brief  Sets the interrupt persistence
 *  @param  pers Persistence setting
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setIntPersistence(veml6046_persistence_t pers) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits pers_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 2, 1);

  return pers_bits.write(pers);
}

/*!
 *  @brief  Gets the current interrupt persistence setting
 *  @return Current persistence setting
 */
veml6046_persistence_t Adafruit_VEML6046::getIntPersistence(void) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits pers_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 2, 1);

  return (veml6046_persistence_t)pers_bits.read();
}

/*!
 *  @brief  Sets the RGB calibration enable
 *  @param  enabled True to enable calibration, false to disable
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setRGBCalibration(bool enabled) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits cal_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 1, 0);

  return cal_bits.write(enabled ? 1 : 0);
}

/*!
 *  @brief  Gets the RGB calibration enable status
 *  @return True if calibration enabled, false if disabled
 */
bool Adafruit_VEML6046::getRGBCalibration(void) {
  Adafruit_BusIO_Register rgb_conf1_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_RGB_CONF_1, 1);
  Adafruit_BusIO_RegisterBits cal_bits =
      Adafruit_BusIO_RegisterBits(&rgb_conf1_reg, 1, 0);

  return cal_bits.read() == 1;
}

/*!
 *  @brief  Sets the green channel high threshold
 *  @param  threshold 16-bit threshold value
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setGreenThresholdHigh(uint16_t threshold) {
  Adafruit_BusIO_Register thdh_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_G_THDH_L, 2, LSBFIRST);

  return thdh_reg.write(threshold);
}

/*!
 *  @brief  Gets the green channel high threshold
 *  @return Current high threshold value
 */
uint16_t Adafruit_VEML6046::getGreenThresholdHigh(void) {
  Adafruit_BusIO_Register thdh_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_G_THDH_L, 2, LSBFIRST);

  return thdh_reg.read();
}

/*!
 *  @brief  Sets the green channel low threshold
 *  @param  threshold 16-bit threshold value
 *  @return True if successful, false if I2C write failed
 */
bool Adafruit_VEML6046::setGreenThresholdLow(uint16_t threshold) {
  Adafruit_BusIO_Register thdl_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_G_THDL_L, 2, LSBFIRST);

  return thdl_reg.write(threshold);
}

/*!
 *  @brief  Gets the green channel low threshold
 *  @return Current low threshold value
 */
uint16_t Adafruit_VEML6046::getGreenThresholdLow(void) {
  Adafruit_BusIO_Register thdl_reg =
      Adafruit_BusIO_Register(i2c_dev, VEML6046_REG_G_THDL_L, 2, LSBFIRST);

  return thdl_reg.read();
}

/*!
 *  @brief  Reads all RGBIR channel data in one operation
 *  @param  r Pointer to store red channel data
 *  @param  g Pointer to store green channel data
 *  @param  b Pointer to store blue channel data
 *  @param  ir Pointer to store IR channel data
 *  @return True if successful, false if I2C read failed
 */
bool Adafruit_VEML6046::getData(uint16_t* r, uint16_t* g, uint16_t* b,
                                uint16_t* ir) {
  if (!r || !g || !b || !ir) {
    return false;
  }

  uint8_t buffer[8];
  uint8_t reg_addr = VEML6046_REG_R_DATA_L;
  if (!i2c_dev->write_then_read(&reg_addr, 1, buffer, 8)) {
    return false;
  }

  *r = buffer[0] | (buffer[1] << 8);
  *g = buffer[2] | (buffer[3] << 8);
  *b = buffer[4] | (buffer[5] << 8);
  *ir = buffer[6] | (buffer[7] << 8);

  return true;
}