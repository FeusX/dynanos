#ifndef CMD_H
#define CMD_H

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

typedef void (*CmdFunc) (char **args, int argc);

typedef struct {
  const char *name;
  CmdFunc func;
} Command;

void writeLed(char **args, int argc);
//void help(char **args, int argc);
//void getInput(char **args, int argc);
//void scriptCmd(char **args, int argc);
void echoCmd(char **args, int argc);
void sleepCmd(char **args, int argc);

const Command commands[] = {
  { "writeLed", writeLed },
  { "echoCmd", echoCmd},
  /*{ "HELP", help},
  { "INPUT", getInput},
  { "SCRIPT", scriptCmd},*/
  { "sleepCmd", sleepCmd}
};

const int command_num = sizeof(commands) / sizeof(commands[0]);

void writeLed(char **args, int argc)
{
  delay(10);
  if(argc < 2)
  {
    Serial.println("[ERROR] Usage: writeLed(pin, value)"); 
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

  Serial.println("ledwritten");
}

void echoCmd(char **args, int argc)
{
  if(argc < 1)
  {Serial.println("[ERROR] Usage: echoCmd(msg)"); return;}
  Serial.println(args[0]);
}

void sleepCmd(char **args, int argc)
{
  if(argc < 1) Serial.println("nope"); return;

  delay(atoi(args[0]));
}

#endif
