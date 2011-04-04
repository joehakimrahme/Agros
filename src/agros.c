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
#include <assert.h>
#include <syslog.h>
#include <glib.h>
#include "agros.h"

#ifndef CONFIG_FILE
#define CONFIG_FILE "agros.conf"
#endif

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
    {"env"  , ENV_CMD   },
    {"?"    , HELP_CMD  }
};



/* This variable contains the environment. I use it in my "env" built-in
   function */
extern char** environ;

/*
 * This function parses a string and fills a command_t struct.
 * It uses the strtok() to split the string into tokens. Then it fills the argv
 * array with the tokens.
 *
 * After filling the array, it copies argv[0] as the cmd->name and cmd->argc
 * as the length of the array.
 *
 */

void parse_command (char *cmdline, command_t *cmd){
    int count = 0;
    char* word;

    word = strtok (cmdline, WHITESPACE);

    if (word == NULL) { word = ""; } // Fixes blank line bug

    while (word) {
        cmd->argv[count] = word;
        word = strtok (NULL, WHITESPACE);
        count++;
    }
    cmd->argv[count] = NULL;

    cmd->argc = count;
    cmd->name = (char *) malloc (strlen (cmd->argv[0])+1);
    strcpy (cmd->name, cmd->argv[0]);
}


/*
 * Reads the input using fgets (don't use scanf!!!)
 */

int read_input (char* string, int num){
    char* CRPosition = NULL;

    if (fgets (string, num, stdin)){
	    CRPosition = strchr (string, '\n');

	    if (CRPosition)
	        *CRPosition = '\0';

	    return 1;

    }else
	    return 0;
}


/*
 * Modifiy this function to modify the prompt. Ultimately, I want to define the prompt
 * inside the conf file à la bash.
 *
 * Some systems use USERNAME, others use USER. Hence the ugly if.
 *
 */

void print_prompt (void){
    char* username = NULL;

    if (getenv ("USERNAME"))
	username = getenv ("USERNAME");
    else
	username = getenv ("USER");

    assert (username != NULL);

    fprintf (stdout, "[AGROS]%s:%s$ ", username, getenv ("PWD"));
}

/*
 * Prints the help message.
 * TODO: Store my string messages (help + error messages) in a separate file.
 */

void print_help(char** allowed){
    fprintf (stdout, "\nWelcome to AGROS, the newer limited shell.\n");
    fprintf (stdout, "Note: At any time, you can type 'exit' to close the shell.\nList of allowed actions:\n");
    print_allowed(allowed);
}

/*
 * Uses chdir() to change current working directory. Then updates the environement,
 * with the new value of PWD.
 *
 * If path is NULL, the function sends the user to his home directory. It builds the path
 * in the temp string "home" then copies it into "path" before freeing "home".
 *
 */

void change_directory (char* path, int loglevel){

    char* home;

    if (path == NULL){

        /* Creates the "home" variable containing path to home directory */
        if ((home = malloc (strlen ("/home/") + strlen (getenv ("USERNAME")) + 1)) == NULL){
            fprintf (stderr, "Memory allocation error!");
            exit(EXIT_FAILURE);
        }

        strcpy (home, "");
        strcat (home, "/home/");
        strcat (home, getenv ("USERNAME"));

        /* Copies the "home" temp variable into "path" in order to allow a free() */
        if ((path = malloc (strlen(home))) == NULL){
            fprintf (stderr, "Memory allocation error!");
            exit(EXIT_FAILURE);
        }

        strcpy (path, home);
        free (home);
    }

    if (chdir (path) == 0){
        if (loglevel >= 3) syslog (LOG_NOTICE, "Changing to directory: %s.", path);
        getcwd (path, MAX_LINE_LEN);
        setenv ("PWD", path, 1);
    } else {
        fprintf (stderr, "%s: Could not change to such directory\n", path);
        if (loglevel >= 2) syslog (LOG_NOTICE, "Could not change to directory: %s.", path);
    }

}

/*
 * This function access the global array variable my_commands
 * and returns the command_code eauivalent to each command.
 *
 */

int get_cmd_code (char* cmd_name){
    int i = 0;
    for (i=0; i<100; i++){
	    if (!strcmp (my_commands[i].command_name, cmd_name))
	        return my_commands[i].command_code;
    }
    return OTHER_CMD;
}

/*
 * This function checks for the validity of user input.
 * allowed_list is an array of strings, the only commands
 * allowed to use.
 *
 */

int check_validity (command_t* cmd, char** allowed){
    int valid = 1;
    int i = 0;

    while (allowed[i]){
	    if (!strcmp (allowed[i], cmd->name) || !strcmp (allowed[i], "*"))
	        valid = 0;
	    i++;
    }
    return valid;
}

/* 
 * Built-in function that displays the environment. As simple as that. 
 * 
 */

void print_env (char* env_variable){
    char* env_value = NULL;
    char** var = NULL;

    if (env_variable != NULL){
	    env_value = getenv(env_variable);
	    if (env_value)
	        fprintf (stdout, "%s:\t%s\n", env_variable, getenv(env_variable));
	    else
	        fprintf (stdout, "Environment variable %s does not exist.\n", env_variable);
    }else {
	    for (var = environ; *var != NULL; ++var)
	        fprintf (stdout, "%s\n", *var);
    }
}

/*
 * Prints the list of allowed commands. Used in the print_help() function
 *
 */

void print_allowed (char** allowed){
    int i=0;
    while (allowed[i]){
	    fprintf (stdout, " * %s\n", allowed[i]);
	    i++;
    }
}

/*
 * EFFECTS: parses CONFIG_FILE.
 * MODIFIES: allowed_list, allowed_nbr, welcome_message, loglevel
 */

void parse_config (char*** allowed_list, int* allowed_nbr, char** welcome_message, int* loglevel){
    GKeyFile* gkf;
    gsize     gallowed_nbr;
    gkf = g_key_file_new ();

    if (!g_key_file_load_from_file (gkf, CONFIG_FILE, G_KEY_FILE_NONE, NULL)){
	    fprintf (stderr, "Could not read config file %s\nTry using another shell or contact an administrator.\n", CONFIG_FILE);
        syslog (LOG_ERR, "Could not read config file: %s.", CONFIG_FILE);
        closelog ();
	    exit (EXIT_FAILURE);
    }

    /* If the file exists and is loaded, we proceed to parsing it */
    if (g_key_file_has_key (gkf, "General", "loglevel", NULL)){
	    *loglevel = g_key_file_get_integer (gkf, "General", "loglevel", NULL);
        syslog (LOG_NOTICE, "Setting log level to: %d.", *loglevel);
    }
    else
	    *loglevel = 0;

    if (g_key_file_has_key (gkf, "General", "welcome", NULL)){
	    *welcome_message = g_key_file_get_string (gkf, "General", "welcome", NULL);
        if (*loglevel >=3) syslog (LOG_NOTICE, "Setting welcome message to: %s.", *welcome_message);
    }

    if (g_key_file_has_key (gkf, "General", "allowed", NULL)){
	    *allowed_list = g_key_file_get_string_list (gkf, "General", "allowed", &gallowed_nbr, NULL);
	    *allowed_nbr = gallowed_nbr;
    }else {
        if (*loglevel >=1) syslog (LOG_NOTICE, "No allowed list. Setting allowed to NULL");
	    *allowed_list = NULL;
	    *allowed_nbr=0;
    }

     g_key_file_free (gkf);
}
