#ifndef LOOP_H
#define LOOP_H

#include <Arduino.h>

void handleCmd(char *input);

void loopCmd(char **args, int argc)
{
  if(argc != 1)
  { Serial.println("[ERROR] Usage: loopCmd(times)"); return; }

  int16_t loop = atoi(args[0]);

  if(loop < 1)
  { Serial.println("[ERROR] Invalid number"); return; }

  Serial.println("[LOOP MODE]\nInput exit() to exit.");
  
  char input[32];
  int8_t index = 0;

  while(true)
  {
    if(Serial.available() > 0)
    {
      //brb
    }
  }
}

#endif
