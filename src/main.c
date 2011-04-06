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
#include <unistd.h>
#include <syslog.h>
#include <sys/wait.h>
#include "agros.h"

int main (int argc, char** argv, char** envp){
    int pid = 0;
    command_t cmd = {NULL, 0, {NULL}};
    char commandline[MAX_LINE_LEN];
    char* username = NULL;
    config_t ag_config;

    /* Opens the syslog file */
    openlog ("[AGROS]", LOG_PID, LOG_USER);

    /* Sets the username */
    set_username (&username);

    /* Parses the config files for data */
    parse_config (&ag_config, username);


    /*
     *   Main loop:
     *   - print prompt
     *   - read input and parse it
     *   - either a built-in command ("cd", "?" or "exit)
     *   - or a system command, in which case the program forks and executes it with execvp()
     */

    if (ag_config.welcome_message!=NULL) {
        fprintf (stdout, "\n%s\n\n", ag_config.welcome_message);
    }

    while (1){
        print_prompt(username);
        read_input (commandline, MAX_LINE_LEN);
        parse_command (commandline, &cmd);

        switch (get_cmd_code (cmd.name)){
            case EMPTY_CMD:
   	            break;

            case CD_CMD:
                change_directory (cmd.argv[1], ag_config.loglevel);
   	            break;

            case HELP_CMD:
                print_help(ag_config.allowed_list);
   	            break;

            case ENV_CMD:
   	            print_env (cmd.argv[1]);
   	            break;

            case EXIT_CMD:
                closelog ();
   	            return 0;

            case OTHER_CMD:
   	            pid = fork();
   	            if (pid == 0){
                if (!check_validity (&cmd, ag_config.allowed_list)){
                    if (ag_config.loglevel == 3)  syslog (LOG_NOTICE, "Using command: %s.", cmd.name);
   	        	    execvp (cmd.name, cmd.argv);
   	        	    fprintf (stderr, "%s: Could not execute command!\nType '?' for help.\n", cmd.name);
                    if (ag_config.loglevel >= 2)  syslog (LOG_NOTICE, "Could not execute: %s.", cmd.name);
                }else {
   	        	    fprintf (stdout, "Not allowed! \n");
                    if (ag_config.loglevel >= 1)  syslog (LOG_ERR, "Trying to use forbidden command: %s.", cmd.name);
                }

   	        	kill(getpid(), SIGTERM);
   	        	break;
   	            }else if (pid < 0){
                    fprintf (stderr, "Error! ... Negative PID. God knows what that means ...\n");
                    if (ag_config.loglevel >= 1) syslog (LOG_ERR, "Negative PID. Using command: %s.", cmd.name);
   	            }else {
                    wait (0);
   	            }
   	            break;
        }
    }

    closelog();
    return 0;
}
