#ifndef LOOP_H
#define LOOP_H

#define MAX_LOOP_CMD 20
#define MAX_LOOP_LINE 16

#include <Arduino.h>

static int8_t loop_cmd = 0;
static char loop_arr[MAX_LOOP_CMD][MAX_LOOP_LINE];

void handleCmd(char *input);

void loopCmd(char **args, int argc)
{
  if(argc != 1)
  { Serial.println("[ERROR] Usage: loopCmd(times)"); return; }

  static int16_t loop = atoi(args[0]);

  if(loop < 1)
  { Serial.println("[ERROR] Invalid number"); return; }

  Serial.println("[LOOP MODE]\nInput exit() to exit.");
  
  char input[32];
  int8_t index = 0;

  while(true)
  {
    if(Serial.available() > 0)
    {
      char ch = Serial.read();

      if(ch == '\r') continue;

      if(ch == '\n')
      {
        input[index] = '\0';
        index = 0;

        if(strlen(input) == 0) continue;

        if(strcmp(input, "exit()") == 0)
        { Serial.println("[EXITING]"); return; }
        if(loop_cmd < MAX_LOOP_CMD)
        {
          strncpy(loop_arr[loop_cmd], input, MAX_LOOP_LINE);
          loop_arr[loop_cmd][MAX_LOOP_LINE - 1] = '\0';
          loop_cmd++;
          Serial.println("[SAVED]");
        }
        else
        { Serial.println("BUFFER FULL"); return; }
      }
      else
      {
        if(index < sizeof(input) - 1)
          input[index++] = ch;
      }
    }
  }
}

void runLoop(char **args, int argc)
{
  if(loop_cmd == 0)
  { Serial.println("[ERROR] No stored commands."); return; }

  Serial.println("[LOOP] Looping...");

  for(int8_t i = 0; i < loop; i++)
  {
    for(int8_t j = 0; j < MAX_LOOP_CMD; j++)
    {
      handleCmd(loop_arr[j]);
      delay(25);
    }
  }
}

#endif
