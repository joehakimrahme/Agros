#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256
#define MAX_ARGS (MAX_LINE_LEN/2)
#define WHITESPACE " \t\n"

typedef struct {		/* Struct to contain a parsed command */

    char* name;
    int argc;
    char* argv[MAX_ARGS+1];

} command_t;



void parse_command(char *cmdline, command_t *cmd){
    int argc = 0;
    char* word;

    word = strtok(cmdline, WHITESPACE);
    if (word == NULL) { word = ""; } // Fixes blank line bug
    while (word) {
        cmd->argv[argc] = (char *) malloc(strlen(word)+1);
        strcpy(cmd->argv[argc], word);
        word = strtok(NULL, WHITESPACE);
        argc++;
    }
    cmd->argv[argc] = NULL;

    cmd->argc = argc;
    cmd->name = (char *) malloc(strlen(cmd->argv[0])+1);
    strcpy(cmd->name, cmd->argv[0]);
}

int read_input(char* string, int num){
    char* CRPosition = NULL;
    if (fgets(string, num, stdin)){
        CRPosition = strchr(string, '\n');
        if (CRPosition){
            *CRPosition = '\0';
        }
        return 1;
    }else {
        return 0;
     }
}

void print_prompt(void){
    fprintf(stdout, "Enter Command$ ");
}

void print_help(void){
    fprintf(stdout, "Welcome to Agros, the newer limited shell.\n\tYou can type 'exit' to close the shell.\n\n\n");
}







int main(int argc, char** argv){
    int pid = 0;
    int _pid = 0;
    command_t cmd;
    char* commandline;

    while (1){
        print_prompt();
        read_input(commandline, MAX_LINE_LEN);
        parse_command(commandline, &cmd);

        if (!strcmp(cmd.name, "exit"))
            break;

        if (!strcmp(cmd.name, "?")){
            print_help();
            continue;
        }

        pid = fork();
        if (pid == 0){
            execv(cmd.name, cmd.argv);
            printf("%s: Could not execute command!\n", cmd.name);
        }else if (pid < 0){
            printf("Error!\n");
         }else {
            wait(0);
          }
    }

    return 0;
}
