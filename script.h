#ifndef SCRIPT_H
#define SCRIPT_H

#define MAX_SCRIPT_LINES 20
#define MAX_LINE_LENGTH 32

#include "cmd.h"

static char script[MAX_SCRIPT_LINES][MAX_LINE_LENGTH];
static int8_t script_count = 0;

void handleCmd(char *input);

void scriptCmd(char **args, int argc)
{
  Serial.println("[SCRIPT MODE]\nInput exit() to exit.");
  script_count = 0;

  char input[64];
  int index = 0;

  while (true)
  {
    while (Serial.available())
    {
      char ch = Serial.read();

      if (ch == '\r') continue;  // ignore carriage return

      if (ch == '\n')  // end of line
      {
        input[index] = '\0';  // null terminate
        index = 0;

        // skip empty lines
        if (strlen(input) == 0) continue;

        // check for exit
        if (strcmp(input, "exit()") == 0)
        {
          Serial.println("[EXITING]");
          return;
        }

        if (script_count < MAX_SCRIPT_LINES)
        {
          strncpy(script[script_count], input, MAX_LINE_LENGTH);
          script[script_count][MAX_LINE_LENGTH-1] = '\0';
          script_count++;
          Serial.println("[SAVED]");
        }
        else
        {
          Serial.println("BUFFER FULL");
          return;
        }
      }
      else
      {
        // accumulate character
        if (index < sizeof(input) - 1)
          input[index++] = ch;
      }
    }
  }
}



void runScriptCmd(char **args, int argc)
{
  if (script_count == 0)
  {
    Serial.println("[SCRIPT] No stored commands!");
    return;
  }

  Serial.println("[SCRIPT] Running...");

  for (int i = 0; i < script_count; i++)
  {
    Serial.print("> ");
    Serial.println(script[i]);
    handleCmd(script[i]);
    delay(50);
  }

  Serial.println("[SCRIPT] Done.");
}



#endif
