#ifndef LOOP_H
#define LOOP_H

#define MAX_LOOP_CMD 12
#define MAX_LOOP_LINE 20

#include <Arduino.h>

static int16_t total_iterations = 0; 
static int8_t stored_cmd_count = 0; 
static char loop_arr[MAX_LOOP_CMD][MAX_LOOP_LINE];

void handleCmd(char *input);

void loopCmd(char **args, int argc)
{
  if(argc != 1)
  { Serial.println("[ERROR] Usage: loopCmd(times)"); return; }
  
  total_iterations = atoi(args[0]);

  if(total_iterations < 1)
  { Serial.println("[ERROR] Invalid number"); return; }

  Serial.println("[LOOP MODE]\nInput exit() to exit.");
  
  stored_cmd_count = 0;
  
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
        if(strcmp(input, "exit()") == 0) { Serial.println("[EXITING]"); return; }

        if(stored_cmd_count < MAX_LOOP_CMD)
        {
          strncpy(loop_arr[stored_cmd_count], input, MAX_LOOP_LINE);
          loop_arr[stored_cmd_count][MAX_LOOP_LINE - 1] = '\0';
          stored_cmd_count++;
          Serial.println("[SAVED]");
        }
        else
        { Serial.println("BUFFER FULL"); return; }
      }
      else
      {
        if(index < sizeof(input) - 1) input[index++] = ch;
      }
    }
  }
}

void runLoop(char **args, int argc)
{
  if(stored_cmd_count == 0)
  { Serial.println("[ERROR] No stored commands."); return; }

  Serial.print("[LOOP] Running ");
  Serial.print(total_iterations);
  Serial.println(" times.");

  for(int16_t i = 0; i < total_iterations; i++)
  {
    for(int8_t j = 0; j < stored_cmd_count; j++)
    {
      char temp_cmd[MAX_LOOP_LINE]; 
      strcpy(temp_cmd, loop_arr[j]);
      handleCmd(temp_cmd);
    }
  }
  Serial.println("[LOOP FINISHED]");
}

#endif
