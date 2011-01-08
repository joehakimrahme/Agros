#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agros.h"


int main(int argc, char** argv, char** envp){
    int pid = 0;
    int _pid = 0;
    command_t cmd;
    char commandline[MAX_LINE_LEN];

    while (1){
        print_prompt();
        read_input(commandline, MAX_LINE_LEN);
        parse_command(commandline, &cmd);

        if (!strcmp(cmd.name, "exit"))
            break;

        if (!strcmp(cmd.name, ""))
            continue;


        if (!strcmp(cmd.name, "cd")){
            change_directory(concat_spaces(cmd.argv));
            continue;
        }

        if (!strcmp(cmd.name, "?")){
            print_help();
            continue;
        }

        pid = fork();
        if (pid == 0){
            execvp(cmd.name, cmd.argv);
            fprintf(stdout, "%s: Could not execute command!\nType '?' for help.\n", cmd.name);
            break;
        }else if (pid < 0){
            fprintf(stdout, "Error!\n");
         }else {
            wait(0);
          }
    }

    return 0;
}

