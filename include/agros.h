#define MAX_LINE_LEN 256
#define MAX_ARGS (MAX_LINE_LEN/2)
#define WHITESPACE " \t\n"

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
