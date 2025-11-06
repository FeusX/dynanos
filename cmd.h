#ifndef CMD_H
#define CMD_H

typedef void (*CmdFunc) (char *args);

typedef struct {
  const char *name;
  CmdFunc func;
} Command;

const Command commands[] = {
  { "WRITE LED", writeLed },
  { "WRITE CMD", writeCmd},
  { "HELP", help},
  { "INPUT", getInput}
};

const int command_num = sizeof(commands) / sizeof(commands[0]);

int writeLed(char **args, int argc) {
  if(argc < 2) Serial.println("[ERROR] Usage: writeLed(pin, value)"); return;

  int8_t pin = atoi(args[0]);
  int8_t val = atoi(args[1]);

  pinMode(pin, OUTPUT);

  if(val > 255) Serial.println("[ERROR] value can't be greater than 255");

  if(val > 1) analogWrite(pin, val);
  else digitalWrite(pin, val ? HIGH : LOW);

  return;
}

void writeCmd(char **args, int argc) {
  if(argc < 1) Serial.println(" ");
  // echo
}

#endif
