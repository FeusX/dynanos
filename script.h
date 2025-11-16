#ifndef SCRIPT_H
#define SCRIPT_H

#define MAX_SCRIPT_LINES 20
#define MAX_LINE_LENGTH 16

static char script[MAX_SCRIPT_LINES][MAX_LINE_LENGTH];
static int8_t script_count = 0;

void handleCmd(char *input);

void scriptCmd(char **args, int argc)
{
  Serial.println("[SCRIPT MODE]\nInput exit() to exit.");
  script_count = 0;

  char input[64];
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
        {
          Serial.println("[EXITING]");
          return;
        }

        if(script_count < MAX_SCRIPT_LINES)
        {
          strncpy(script[script_count], input, MAX_LINE_LENGTH);
          script[script_count][MAX_LINE_LENGTH - 1] = '\0';
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
        if(index < sizeof(input) - 1)
          input[index++] = ch;
      }
    }
  }
}


void runScriptCmd(char **args, int argc)
{
  if(script_count == 0)
  {
    Serial.println("[SCRIPT] No stored commands!");
    return;
  }

  Serial.println("[SCRIPT] Running...");

  for(int8_t i = 0; i < script_count; i++)
  {
    handleCmd(script[i]);
    delay(25);
  }

  Serial.println("[SCRIPT] Done.");
}


#endif
