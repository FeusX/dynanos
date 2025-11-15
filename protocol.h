#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h>

#define I2C_BUFFER 64

static volatile uint8_t buf[I2C_BUFFER];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

static volatile uint8_t bit_count = 0;
static volatile uint8_t current_byte = 0;
static volatile bool in_frame = false;
static volatile uint8_t last_clock = 1;

ISR(PCINT1_vect)
{
  uint8_t pin = PINC;
  uint8_t scl = (pin & (1 << PC5)) ? 1 : 0;
  uint8_t sda = (pin & (1 << PC4)) ? 1 : 0;

  if(scl == 1 && sda == 0 && !in_frame)
  {
    in_frame = true;
    bit_count = 0;
    current_byte = 0;

    uint8_t next = (head + 1) & (I2C_BUFFER - 1);

    if(next != tail)
    {
      buf[head] = 0xF0;
      head = next;
    }
  }

  if(scl == 1 && sda == 1 && in_frame)
  {
    in_frame = false;
    uint8_t next = (head + 1) & (I2C_BUFFER - 1);
    if(next != tail)
    {
      buf[head] = 0xF1;
      head = next;
    }
  }

  if(last_clock == 0 && scl == 1 && in_frame)
  {
    current_byte <<= 1;
    if(sda) current_byte |= 1;
    bit_count++;

    if(bit_count == 8)
    {
      uint8_t next = (i2c_head + 1) & (I2C_BUF_SIZE - 1);
      if(next != tail)
      {
        buf[head] = current_byte;
        head = next;
      }
      bit_count = 0;
      current_byte = 0;
    }
  }

  last_clock = scl;
}

#endif
