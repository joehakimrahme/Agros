/*
 *    AGROS - The new Limited Shell
 *
 *    Author: Joe "rahmu" Hakim Rahme <joe.hakim.rahme@gmail.com>
 *
 *
 *    This file is part of AGROS.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agros.h"



int main(int argc, char** argv, char** envp){
    int pid = 0;
    command_t cmd;
    char commandline[MAX_LINE_LEN];

    /*
     *   Main loop:
     *   - print prompt
     *   - read input and parse it
     *   - either a built-in command ("cd", "?" or "exit)
     *   - or a system command, in which case the program forks and executes it with execvp()
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
