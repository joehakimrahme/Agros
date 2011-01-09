#define MAX_LINE_LEN 256
#define MAX_ARGS (MAX_LINE_LEN/2)
#define WHITESPACE " \t\n"

#define OTHER_CMD   0
#define EMPTY_CMD   1
#define CD_CMD      2
#define HELP_CMD    3
#define EXIT_CMD    4

typedef struct built_in_commands built_in_commands;
struct built_in_commands{
    char command_name[MAX_LINE_LEN];
    int command_code;
};

typedef struct command_t command_t;
struct command_t{		
    char* name;
    int argc;
    char* argv[MAX_ARGS+1];
};

void parse_command(char *cmdline, command_t *cmd);
int read_input(char* string, int num);
void print_prompt(void);
void print_help(void);
void change_directory(char* PATH);
char* concat_spaces(char** string_array);
int get_cmd_code(char* cmd_name);
