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
#include <readline/readline.h>
#include <readline/history.h>
#include "agros.h"

/*
 * A global array holding the associations between each built-in command
 * and their command_code. More explanations can be found in the declaration
 * of the "built_in_commands" structure.
 *
 * TODO: Replace array size (100) by a dynamic allocation.
 */

built_in_commands my_commands[100] = {
        {"exit" , EXIT_CMD  },
        {""     , EMPTY_CMD },
        {"cd"   , CD_CMD    },
        {"?"    , HELP_CMD  }
};


/*
 * This function parses a string and fills a command_t struct.
 * It uses the strtok() to split the string into tokens. Then it fills the argv
 * array with the tokens.
 *
 * After filling the array, it copies argv[0] as the cmd->name and cmd->argc
 * as the length of the array.
 *
 */

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


/*
 * Reads the input using fgets (don't use scanf!!!)
 * DEPRECATED.
 * DEPRECATED.
 * DEPRECATED.
 *
 */

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


/*
 * Modifiy this function to modify the prompt
 */

char* return_prompt(void){
    char prompt[10000] = "[AGROS]";

    strcat(prompt, getenv("USERNAME"));
    strcat(prompt, ":");
    strcat(prompt, getenv("PWD"));
    strcat(prompt, "$ ");

    return prompt;
}

/*
 * Prints the help message.
 * TODO: Store my string messages (help + error messages) in a separate file.
 */

void print_help(void){
    fprintf(stdout, "\nWelcome to AGROS, the newer limited shell.\nNote: At any time, you can type 'exit' to close the shell.\n\n\n");
}

/*
 * This command changes the current working directory used in the computation
 * of relative paths. using the chdir() function.
 * It then updates the $PWD environment variable with the new value of the current
 * directory
 */

void change_directory(char* PATH){
    if (chdir(PATH) == 0){
        getcwd(PATH, MAX_LINE_LEN);
        setenv("PWD", PATH, 1);
    }else{
        fprintf(stdout, "%s: Could not change to such directory\n", PATH);
    }
}

/*
 * This is used to send an argument to the CD command.
 * It concatenates the arguments in case of a path containing a space character.
 *
 * TODO: This function should be updated to affect only parts of a global string.
 */

char* concat_spaces (char** string_array){
    char* tmp = string_array[1];
    int count = 2;

    while (string_array[count]){
        strcat(tmp, " ");
        strcat(tmp, string_array[count]);
        count++;
    }

    return tmp;
}

/*
 * This function access the global array variable my_commands
 * and returns the command_code eauivalent to each command.
 *
 */

int get_cmd_code(char* cmd_name){
    int i = 0;
    for (i=0; i<100; i++){
        if (!strcmp(my_commands[i].command_name, cmd_name)){
            return my_commands[i].command_code;
        }
    }
    return OTHER_CMD;
}

/*
 * This is a wrapper for the GNU Readline function.
 * Readline rocks! Thank you GNU!
 *
 */

char* ag_readline(char* prompt){
    static char *line_read = (char *)NULL;

    if (line_read){
        free (line_read);
        line_read = (char *)NULL;
    }

    line_read = readline (prompt);

    if (line_read && *line_read)
      add_history (line_read);

    return line_read;
}
