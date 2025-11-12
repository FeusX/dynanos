#include "cmd.h"

void handleCmd(char *input)
{
  int len = strlen(input);
  while(len > 0 && (input[len - 1] == '\r' || input[len - 1] == '\n'))
    input[--len] = '\0';

  while(*input == ' ') input++;

  char *name = strtok(input, "(");
  char *arg_str = strtok(NULL, ")");
  char *args[8];
  int argc = 0;

  if(arg_str != NULL)
  {
    char clean_args[64];
    int j = 0;
    for(int i = 0; arg_str[i] != '\0'; i++)
      if(arg_str[i] != ' ' && arg_str[i] != '\r' && arg_str[i] != '\n')
        clean_args[j++] = arg_str[i];
    clean_args[j] = '\0';

    char *token = strtok(clean_args, ",");
    while(token != NULL && argc < 8)
    {
      args[argc++] = token;
      token = strtok(NULL, ",");
    }
  }  

  // debugger
  /*Serial.print("Parsed command name: ");
  Serial.println(name);
  Serial.print("Arg count: ");
  Serial.println(argc);
  for(int j = 0; j < argc; j++) {
    Serial.print("Arg[");
    Serial.print(j);
    Serial.print("]: ");
    Serial.println(args[j]);
  }*/

  for(int i = 0; i < command_num; i++)
  {
    if(strcasecmp(name, commands[i].name) == 0)
    {
      unsigned long t0 = micros();
      commands[i].func(args, argc);
      busy += (micros() - t0);
      return;
    }
  }

  Serial.print("[ERROR] Unknown command: ");
  Serial.println(name);
}


void setup()
{
  Serial.begin(57600);
  Serial.println("ready");
  delay(200);
}

void loop()
{
  static char input_buffer[64];
  static int index = 0;

  while(Serial.available())
  {
    char ch = Serial.read();

    if(ch == '\n' || ch == '\r')
    {
      input_buffer[index] = '\0';
      if(index > 0) handleCmd(input_buffer);
      index = 0;
    }

    else if(index < sizeof(input_buffer) - 1) {input_buffer[index++] = ch;}
  }
}

// test
