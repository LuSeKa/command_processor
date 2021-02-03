 #include "Arduino.h"

class Command_processor {
    
  private:

    typedef enum Parser_state {
      IDLE = 0,
      WAITING_FOR_PARAMS,
      PROCESS_PARAMS
    } Parser_state;

    typedef struct command_struct {
      void (*command_handler)(int, int) = NULL; // this is how all command handlers must look like
      char key = ' ';
      uint8_t num_params = 0;
      String help = "Unused";
      bool in_use = false;
    } command_struct;

    static const uint8_t num_commands = 10;
    static const uint8_t max_num_params = 2;
    static const uint8_t max_help_length = 30;
    uint8_t parser_wait_time_ms = 10;
    uint8_t command_index = 0;
    uint8_t params[max_num_params];
    uint8_t num_commands_in_use = 0;
    int num_received_params = 0;
    bool verbose = true;
    Parser_state parser_state = IDLE;
    long parser_wait_start_time;
    command_struct commands[num_commands];
    int get_num_params(char key);
    bool parse_params(int num_params);
    void flush_serial_buffer();
    void cmd_info(String info);


  public:

    Command_processor();
    Command_processor(bool verbosity);
    bool add_command(char key, bool (*command_handler)(int, int), uint8_t num_params, String help);
    bool parse_command();
    void print_help();
    void set_verbose(bool verbose);
};
