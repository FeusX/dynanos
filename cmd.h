#ifndef CMD_H
#define CMD_H

#include "script.h"
#include "ntop.h"

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

typedef void (*CmdFunc) (char **args, int argc);

typedef struct {
  const char *name;
  CmdFunc func;
} Command;

void writePin(char **args, int argc);
void help(char **args, int argc);
void readPin(char **args, int argc);
void echoCmd(char **args, int argc);
void sleepCmd(char **args, int argc);
void btd(char **args, int argc);

const Command commands[] = {
  {"writePin", writePin},
  {"echoCmd", echoCmd},
  {"HELP", help},
  {"readPin", readPin},
  {"SCRIPT", scriptCmd},
  {"sleepCmd", sleepCmd},
  {"RUN", runScriptCmd},
  {"ntop", ntop},
  {"btd", btd}
};

const int command_num = sizeof(commands) / sizeof(commands[0]);

void writePin(char **args, int argc)
{
  delay(10);
  if(argc != 2)
  {
    Serial.println("[ERROR] Usage: writePin(pin, value)"); 
    return;
  }

  int8_t pin = atoi(args[0]);
  int8_t val = atoi(args[1]);

  pinMode(pin, OUTPUT);

  if(val > 255)
  {
    Serial.println("[ERROR] Value can't be greater than 255");
    return;
  }

  if(val > 1) analogWrite(pin, val);
  else digitalWrite(pin, val ? HIGH : LOW);

  Serial.println("pinwritten");
}

void echoCmd(char **args, int argc)
{
  if(argc != 1)
  { Serial.println("[ERROR] Usage: echoCmd(msg)"); return; }
  Serial.println(args[0]);
}

void sleepCmd(char **args, int argc)
{
  if(argc != 1)
  { Serial.println("[ERROR] Usage: sleepCmd(var)"); return; }

  static bool active = false;
  static unsigned long end = 0;

  unsigned long now = millis();
  unsigned long dur = atoi(args[0]);

  if(!active)
  { end = now + dur; active = true; return; }

  if(now >= end)
  { active = false; return; }
}

void help(char **args, int argc)
{
  for(int8_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
  {
    Serial.println(commands[i].name);
  }
}

void readPin(char **args, int argc)
{
  if(argc != 1)
  { Serial.println("[ERROR] Usage: readPin(pin)"); return; }

  int8_t pin = atoi(args[0]);

  pinMode(pin, INPUT);

  if(pin >= A0 && pin < A7)
  {
    Serial.print("analog: ");
    Serial.println(analogRead(pin));
  }
  else
  {
    Serial.print("digital: ");
    Serial.println(digitalRead(pin));
  }
}

void btd(char **args, int argc)
{
  if(argc != 1)
  { Serial.println("[ERROR] Usage: btd(binary)"); return; }

  const char* bin = args[0];
  uint16_t dec = 0;

  for(int i = 0; bin[i] != '\0'; i++)
  {
    dec <<= 1;
    if(bin[i] == '1') dec |= 1;
    else if(bin[i] != '0')
    {
      Serial.println("Invalid binary");
      return;
    }
  }

  Serial.println(dec);
}

#endif
