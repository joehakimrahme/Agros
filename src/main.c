#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agros.h"


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

        if (!strcmp(cmd.name, ""))
            continue;

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
