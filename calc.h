#ifndef CALC_H
#define CALC_H

#include <Arduino.h>
#include <stdlib.h>

bool is_numeric(const char *str);

void handleCmd(char *input);

void calc(char **args, int argc)
{
  Serial.println(F("[CALCULATOR]\nInput exit() to exit."));
  
  while(true)
  {
    // brb
  }
}

uint32_t fac(uint8_t num)
{
  uint32_t sum = 1;
  
  for(uint8_t i = 1; i <= num; i++)
  {
    sum *= i;
  }

  return sum; 
}

#endif
