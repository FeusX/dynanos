#ifndef NTOP_H
#define NTOP_H

#include <Arduino.h>

static volatile unsigned long busy = 0; 

static int free_ram()
{
  extern int16_t __heap_start, *__brkval;
  int16_t v;

  return (int16_t)&v - (__brkval == 0 ? (int16_t)&__heap_start : (int16_t)__brkval);
}

static void ntop(char **args, int argc)
{
  unsigned long total = millis() * 1000;

  Serial.println(F("[NTOP]"));
  Serial.print(F("CPU: "));
  Serial.print(100.0 * busy / total, 2);
  Serial.println(F("%"));

  Serial.print(F("RAM: "));
  Serial.print(2048 - free_ram());
  Serial.println(F(" bytes"));

  Serial.println(F("[NTOP]"));
}

#endif
