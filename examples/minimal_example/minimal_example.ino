#include "Command_processor.h"

// my function
int square(int root) {
  return root * root;
}

// wrapper to make my function digestable for the command processor
void square_handler(int root, int dummy) {
  int result = square(root);
  Serial.println("The square of " + String(root) + " is " + String(result));
  return true;
}

// instantiate command processor
Command_processor cmd;

void setup() {
  Serial.begin(115200);
  // arguments: key, function pointer, number of arguments, help string
  cmd.add_command('s', &square_handler, 1, "compute square");
}

void loop() {
  // call parse_command() with high frequency
  cmd.parse_command();
}
