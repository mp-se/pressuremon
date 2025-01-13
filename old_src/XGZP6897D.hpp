/*
  XGZP6897D.h - Library for using a familly of pressure sensors from
  CFSensor.com

  I2C sensors

  Created by Francis Sourbier

  GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
  This library is free software;
  Released into the public domain
*/
#ifndef SRC_XGZP6897D_HPP_
#define SRC_XGZP6897D_HPP_

#include <Arduino.h>
#include <Wire.h>

constexpr auto XGZP6897D_I2C_ADDRESS = 0x6D;

class XGZP6897D {
 public:
  explicit XGZP6897D(uint16_t K, TwoWire *theWire = &Wire);
  // K depends on sensor. See datasheet.
  // theWire is a pointer the used interface. If NULL, the default is Wire is
  // used

  bool begin();  // true: device responding.  false:device not responding

  // readSensor:
  //  Read temperature (degree Celsius), and pressure (PA)
  //  Return float values
  // Return "true" if the read is a success , "false" if an error occurs
  // (timeout ??)
  bool readSensor(float &temperature, float &pressure);

  //  readRawSensor:
  //  Return raw integer values for temperature and pressure.
  //  The raw integer value of temperature must be devided by 256 to convert in
  //  degree Celsius. The raw integer value of pressure must be devided by the K
  //  factor to convert in Pa.
  // Return "true" if the read is a success , "false" if an error occurs
  // (timeout ??)
  bool readRawSensor(int16_t &rawTemperature, int32_t &rawPressure);

 private:
  float _k;
  uint8_t _i2cAdress = XGZP6897D_I2C_ADDRESS;
  TwoWire *_wire;
};

#endif  // SRC_XGZP6897D_HPP_
