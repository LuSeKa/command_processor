#include "Command_processor.h"

int Command_processor::get_num_params(char key) {
  for (uint8_t i = 0; i < num_commands; i++) {
    if (commands[i].key == key) {
      command_index = i;
      return commands[i].num_params;
    }
  }
  return -1; // invalid key
}

bool Command_processor::parse_params(int num_params) {
  for (uint8_t i = 0; i < num_params; i++) {
    if (Serial.available() > 1) {
      params[i] = Serial.parseInt();
    }
    else {
      return false;
    }
  }
  return true;
}

void Command_processor::flush_serial_buffer() {
  while (Serial.available()) {
    Serial.read();
  }
}

void Command_processor::cmd_info(String info) {
  if (verbose) {
    Serial.print("Command processor: ");
    Serial.println(info);
  }
}

Command_processor::Command_processor() {
  commands[0].key = 'h';
  commands[0].num_params = 0;
  commands[0].help = "print help";
  commands[0].in_use = true;
  num_commands_in_use = 1;
}

Command_processor::Command_processor(bool verbosity) {
  commands[0].key = 'h';
  commands[0].num_params = 0;
  commands[0].help = "print help";
  commands[0].in_use = true;
  num_commands_in_use = 1;
  set_verbose(verbosity);
}

bool Command_processor::add_command(char key, void (*command_handler)(int, int), uint8_t num_params, String help) {
  for (uint8_t i = 0; i < num_commands; i++) {
    if (commands[i].key == key) {
      cmd_info("Command key " + String(key) + " is already in use");
      return false;
    }
    if (commands[i].in_use == false) {
      commands[i].key = key;
      commands[i].command_handler = command_handler;
      commands[i].num_params = num_params;
      commands[i].help = help;
      commands[i].in_use = true;
      num_commands_in_use ++;
      return true;
    }
  }
  cmd_info("Could not add command");
  return false;
}

bool Command_processor::parse_command() {
  switch (parser_state) {
    case IDLE:
      if (Serial.available()) {
        num_received_params = get_num_params(Serial.read());
        parser_state = WAITING_FOR_PARAMS;
        parser_wait_start_time = millis();
        break;
      }
      else {
        return false;
      }
    case WAITING_FOR_PARAMS:
      if (millis() > (parser_wait_start_time + parser_wait_time_ms)) {
        parser_state = PROCESS_PARAMS;
      }
      else {
        break;
      }
    case PROCESS_PARAMS:
      parser_state = IDLE;
      if (num_received_params == -1) {
        cmd_info("Invalid command key");
        flush_serial_buffer();
        return false;
      }
      bool got_params = parse_params(num_received_params);
      flush_serial_buffer();
      if (!got_params) {
        cmd_info("Command " + String(commands[command_index].key) + " (" + commands[command_index].help + ")" + " requires " + String(commands[command_index].num_params) + " parameters");
        return false;
      }
      if (command_index == 0) {
        print_help();
      }
      else {
        commands[command_index].command_handler(params[0], params[1]);
      }
      return true;
    default:
      return false;
  }
}

void Command_processor::print_help() {
  cmd_info("commands in use:");
  for (uint8_t i = 0; i < num_commands; i++) {
    if (commands[i].in_use) {
      Serial.print(commands[i].key);
      Serial.print(" - ");
      Serial.println(commands[i].help);
    }
  }
}

void Command_processor::set_verbose(bool verbose) {
  this->verbose = verbose;
}
