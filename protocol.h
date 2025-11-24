#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Wire.h>
#include <Arduino.h>

uint8_t addr;

void readI2C(char **args, int argc);
void writeI2C(char **args, int argc);
void scanI2C(char **args, int argc);
void initI2C(char **args, int argc);
void clearI2C(char **args, int argc);

void readI2C(char **args, int argc)
{
  if(argc != 2)
  { Serial.println(F("[ERROR] Usage: readI2C(addr, reg)")); return; }

  addr = (int8_t)strtol(args[0], NULL, 16);
  int8_t reg = (int8_t)strtol(args[1], NULL, 16);

  Wire.beginTransmission(addr);
  Wire.write(reg);

  if(Wire.endTransmission() != 0)
  { Serial.println(F("[ERROR] No ACK.")); return; }

  Wire.requestFrom(addr, (uint8_t)1);
  if(Wire.available()) Serial.println(Wire.read());
  else Serial.println(F("[ERROR] No data recieved."));
}

void writeI2C(char **args, int argc)
{
  if(argc < 2)
  { Serial.println(F("[ERROR] Usage: writeI2C(addr, byte1, byte2, ...)")); return; }

  addr = (int8_t)strtol(args[0], NULL, 16);

  Wire.beginTransmission(addr);

  for(int16_t i = 1; i < argc; i++)
  {
    int8_t val = (int8_t)strtol(args[i], NULL, 16);
    Wire.write(val);

    if(val == 0x8D)
    { delay(10); }
  }

  if(Wire.endTransmission() != 0)
  { Serial.println(F("[ERROR] Failed to write.")); }
  else Serial.println(F("Written."));
}

void scanI2C(char **args, int argc)
{
  Serial.println(F("Scanning..."));
  for(uint8_t i = 1; i < 127; i++)
  {
    Wire.beginTransmission(i);
    if(Wire.endTransmission() == 0)
    {
      Serial.print(F("Device at 0x"));
      Serial.println(i, HEX);
    }
  }

Serial.println(F("Scan complete."));
}

void initI2C(char **args, int argc)
{
  Wire.beginTransmission(0x3C);
  Wire.write(0x00);

  static const uint8_t init_seq[] = {
    0xAE,       // display off
    0xD5, 0x80, // set clock div
    0xA8, 0x3F, // multiplex ratio (128x64)
    0xD3, 0x00, // display offset
    0x40,       // start line
    0x8D, 0x14, // charge pump
    0x20, 0x00, // memory mode (horizontal)
    0xA1,       // segment remap
    0xC8,       // com scan direction
    0xDA, 0x12, // com hardware config
    0x81, 0xCF, // contrast control
    0xD9, 0xF1, // pre-charge period
    0xDB, 0x40, // vcomh deselect level
    0xA4,       // output follows ram content
    0xA6,       // normal display (not inverted)
    0xAF        // display on
  };

  addr = 0x3C;

  Wire.write(init_seq, sizeof(init_seq));

  if(Wire.endTransmission() == 0) Serial.println(F("Done"));
  else Serial.println(F("Failed"));
}

void clearI2C(char **args, int argc)
{
  for(uint8_t i = 0; i < 8; i++)
  {
    Wire.beginTransmission(0x3C);
    Wire.write(0x00);
    Wire.write(0xB0 | i);
    Wire.write(0x00);
    Wire.write(0x10);
    Wire.endTransmission();

    for(uint8_t j = 0; j < 128; j += 16)
    {
      Wire.beginTransmission(0x3C);
      Wire.write(0x40);

      for(uint8_t k = 0; k < 16; k++)
      {
        Wire.write(0x00);
      }

      Wire.endTransmission();
    }
  }

  Serial.println(F("Screen cleared."));
}

#endif
