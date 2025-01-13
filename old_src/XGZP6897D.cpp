/*
  XGZP6897D.cpp - Library for using a familly of pressure sensors from
  CFSensor.com

  I2C sensors

  Created by Francis Sourbier

  GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
  This library is free software;
  Released into the public domain
*/
#include <Wire.h>

#include <XGZP6897D.hpp>

//  Descriptor. K depends on the exact model of the sensor. See datasheet and
//  documentation
XGZP6897D::XGZP6897D(uint16_t k, TwoWire *wire) {
  _k = k;
  _wire = wire;
}

bool XGZP6897D::begin() {
  // _wire->begin(); // Removed since this is set in the main code where pins
  // can also be defined.
  _wire->beginTransmission(_i2cAdress);
  return _wire->endTransmission() == 0;
}

//  Return raw integer values for temperature and pressure.
//  The raw integer of temperature must be devided by 256 to convert in degree
//  Celsius The raw integer of pressure must be devided by the K factor to
//  convert in Pa
bool XGZP6897D::readRawSensor(int16_t &rawTemperature, int32_t &rawPressure) {
  int32_t pressure_adc;
  int16_t temperature_adc;
  uint8_t pressure_H, pressure_M, pressure_L, temperature_H, temperature_L;
  uint8_t CMD_reg;
  unsigned long endConversion;
  // start conversion
  _wire->beginTransmission(_i2cAdress);
  _wire->write(0x30);
  _wire->write(0x0A);  // start combined conversion pressure and temperature
  if (_wire->endTransmission() != 0) return false;
  // wait until the end of conversion (Sco bit in CMD_reg. bit 3)
  endConversion = millis() + 20;  //  conversion should take 20ms at most...
  do {
    if (millis() > endConversion) return false;
    _wire->beginTransmission(_i2cAdress);
    _wire->write(0x30);  // send 0x30 CMD register address
    if (_wire->endTransmission() != 0) return false;
    if (_wire->requestFrom(_i2cAdress, byte(1)) != 1) return false;
    ;
    CMD_reg = _wire->read();  // read 0x30 register value
  } while ((CMD_reg & 0x08) > 0);  // loop while bit 3 of 0x30 register copy is
                                   // 1
  // read temperature and pressure
  _wire->beginTransmission(_i2cAdress);
  _wire->write(0x06);  // send pressure high byte register address
  if (_wire->endTransmission() != 0) return false;
  if (_wire->requestFrom(_i2cAdress, byte(5)) != 5)
    return false;  // read 3 bytes for pressure and 2 for temperature
  pressure_H = _wire->read();
  pressure_M = _wire->read();
  pressure_L = _wire->read();
  temperature_H = _wire->read();
  temperature_L = _wire->read();
  pressure_adc = (static_cast<uint32_t>(pressure_H) << 8) +
                 static_cast<uint32_t>(pressure_M);
  pressure_adc = (pressure_adc << 8) + static_cast<uint32_t>(pressure_L);
  temperature_adc = (static_cast<uint16_t>(temperature_H) << 8) +
                    static_cast<uint16_t>(temperature_L);
  // pressure is a signed2 complement style value, on 24bits.
  // need to extend the bit sign on the full 32bits.
  rawPressure = ((pressure_adc << 8) >> 8);
  rawTemperature = temperature_adc;
  return true;
}

bool XGZP6897D::readSensor(float &temperature, float &pressure) {
  pressure = NAN;
  temperature = NAN;
  
  int32_t rawPressure;
  int16_t rawTemperature;
  if (!readRawSensor(rawTemperature, rawPressure)) return false; 
  pressure = rawPressure / _k;
  temperature = static_cast<float>(rawTemperature) / 256;
  return true;
}

// EOF
