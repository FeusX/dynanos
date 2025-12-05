#ifndef MACRO_H
#define MACRO_H

#include <Arduino.h>
#include <string.h>

struct Macro {
  uint8_t pin;
  char command[MAX_LOOP_LINE];
  uint8_t last_state;
};

static Macro macros[10];
static int8_t macro_count = 0;

void processMacros();
void macro(char **args, int argc);

void macro(char **args, int argc)
{
  if(argc != 2)
  { Serial.println(F("[ERROR] Usage: macro(pin, function, 0/1)")); return; }

  if(macro_count >= 10)
  { Serial.println(F("[MACRO BUFFER FULL]")); return; }

  uint8_t pin = atoi(args[0]);
  const char* cmd = args[1];

  pinMode(pin, INPUT_PULLUP);
}

#endif
