#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agros.h"



int main(int argc, char** argv, char** envp){
    int pid = 0;
    int _pid = 0;
    command_t cmd;
    char commandline[MAX_LINE_LEN];

    /* Main loop:
        - print prompt
        - read input and parse it
        - either a built-in command ("cd", "?" or "exit)
        - or a system command, in which case the program forks and executes it with execvp()
    */

    while (1){
        print_prompt();
        read_input(commandline, MAX_LINE_LEN);
        parse_command(commandline, &cmd);

        switch (get_cmd_code(cmd.name)){
            case EMPTY_CMD:
                break;
            case CD_CMD:
                change_directory(concat_spaces(cmd.argv));
                break;
            case HELP_CMD:
                print_help();
                break;
            case EXIT_CMD:
                return 0;
            case OTHER_CMD:
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
                break;
        }

    }
    return 0;
}

