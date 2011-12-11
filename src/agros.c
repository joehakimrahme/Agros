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
 e
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <syslog.h>
#include <pwd.h>
#include <sys/types.h>
#include <signal.h>

#include "smags.h"
#include "agros.h"


/*
 * A global array holding the associations between each built-in command
 * and their command_code. More explanations can be found in the declaration
 * of the "built_in_commands" structure.
 *
 */

built_in_commands my_commands[] = {
    {""         , EMPTY_CMD         , ""},
    {"cd"       , CD_CMD            , "change directory"},
    {"env"      , ENV_CMD           , "print environment"},
    {"exit"     , EXIT_CMD          , "exit from AGROS"},
    {"help"     , HELP_CMD          , "print help"},
    {"set"      , SETENV_CMD        , "modify the environment"},
    {"unset"    , UNSETENV_CMD      , "unset environment variable"},
    {"?"        , SHORTHELP_CMD     , "print short help"}
};

int const CMD_NBR = (sizeof(my_commands)/sizeof(my_commands[0]));

/*
 * A reference to the list and number of allowed commands.
 * This list is used when autocompleting commands.
 */
char **allowed_list = (char **)NULL;
int allowed_nbr = 0;

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
    char* word = NULL;

    word = strtok (cmdline, WHITESPACE);

    if (word == NULL) { word = ""; } // Fixes blank line bug

    if (cmd->name != NULL){
        free (cmd->name);
    }

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
 * Prints the prompt to the given string.
 * - prompt: The string that will contain the prompt
 * - length: The maximum length of the prompt
 * - username: The username to display
 */
void get_prompt (char *prompt, int length, user_t *user)
{
    char *cwd = (char *)malloc (sizeof (MAX_LINE_LEN));

    snprintf(prompt, length, "[%s]%s$ ", user->username, getcwd (cwd, MAX_LINE_LEN));
}

/*
 * Prints the help message.
 * TODO: Store my string messages (help + error messages) in a separate file.
 */

void print_help(config_t* config, char* helparg){
    int i =0;

    if (!strcmp (helparg, "-b")){

        fprintf(stdout, "\n");
        for (i=0; i<CMD_NBR; i++){

            if (my_commands[i].command_code != OTHER_CMD
                && my_commands[i].command_code != EMPTY_CMD)

                fprintf (stdout, "\t%s\t%s\n", my_commands[i].command_name,
                                              my_commands[i].command_desc);
        }

        fprintf(stdout, "\n");

    }else if (!strcmp (helparg, "-a")){

        fprintf (stdout, "\n\n");

        for (i=0; i<70; i++)
            fprintf (stdout, "*");

        fprintf (stdout, "\nWelcome to AGROS, the newer limited shell.\n");
        /* Show list of builtins */
        fprintf (stdout, "Note: At any time, you can type 'exit' to close the shell.\n\n");

        print_allowed (config->allowed_list);
        print_forbidden (config->forbidden_list);

        for (i=0; i<80; i++)
            fprintf (stdout, "*");

        fprintf (stdout, "\n\n");

    }else if (!strcmp (helparg, "-s")){

        print_allowed (config->allowed_list);
        print_forbidden (config->forbidden_list);

    }else if (!strcmp (helparg, "-w")){

        fprintf(stdout, "warnings left: %d\n", config->warnings);

    } else

        fprintf (stdout, "Unkown option %s\n", helparg);

}

/*
 * Uses chdir() to change current working directory. Then updates the environement,
 * with the new value of PWD.
 *
 * If path is NULL, the function sends the user to his home directory. It builds the path
 * in the temp string "home" then copies it into "path" before freeing "home".
 *
 */

void change_directory (char* path, int loglevel, user_t user){

    /* If no arguments are given, go to $HOME directory */
    if (path == NULL)
        path = user.homedir;

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
 * and returns the command_code equivalent to each command.
 *
 */

int get_cmd_code (char* cmd_name){
    int i = 0;
    for (i=0; i<CMD_NBR; i++){
        if (!strcmp (my_commands[i].command_name, cmd_name))
            return my_commands[i].command_code;
    }
    return OTHER_CMD;
}

/*
 * This function access the global array variable my_commands
 * and returns the equivalent command_desc vaule.
 *
 */

char* get_cmd_desc (char* cmd_name){
    int i = 0;
    for (i=0; i<CMD_NBR; i++){
        if (!strcmp (my_commands[i].command_name, cmd_name))
            return my_commands[i].command_desc;
    }
    return "";
}

/*
 * This function checks for the validity of user input.
 * allowed_list is an array of strings, the only commands
 * allowed to use.
 *
 *
 */

int check_validity (command_t cmd, config_t config){
    int valid = AG_FALSE;
    int i = 0, j = 0;

    /* Checks if the command name is part of the allowed list */
    while (config.allowed_list[i]){
        if (!strcmp (config.allowed_list[i], cmd.name) || !strcmp (config.allowed_list[i], "*"))
            valid = AG_TRUE;
        i++;
    }

    /* Checks that the command line does not include any forbidden character */
    i = 0;

    while (config.forbidden_list[i]){
        for (j=0; j<cmd.argc; j++){
            if (strstr (cmd.argv[j], config.forbidden_list[i]) != NULL &&
                                    *(config.forbidden_list[i]) != 0)
                valid = AG_FALSE;
        }

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
 * Prints the list of forbidden commands. Used in the print_help() function
 *
 */

void print_forbidden (char** forbidden){
    int i=0;

    fprintf (stdout, "List of forbidden characters:\n\n");
    if (forbidden[0] != NULL){
        while (forbidden[i]){
            fprintf (stdout, " * %s\n", forbidden[i]);
            i++;
        }
    } else{
        fprintf (stdout, " * (none)\n");
    }

    fprintf (stdout, "\n");
}


/*
 * Prints the list of allowed commands. Used in the print_help() function
 *
 */

void print_allowed (char** allowed){
    int i=0;

    fprintf (stdout, "List of allowed actions:\n\n");
    if (strcmp (allowed[0],"*")){
        while (allowed[i]){
            fprintf (stdout, " * %s\n", allowed[i]);
            i++;
        }
        fprintf (stdout, "\n");
    } else
        fprintf (stdout, " * (all)\n\n");
}

/*
 * This function is called when a forbidden command is entered.
 */
void decrease_warnings (config_t* ag_config){
    if (ag_config->warnings > 1){

        ag_config->warnings--;
        fprintf (stdout, "Warnings remaining: %d\n", ag_config->warnings);

    }else if (ag_config->warnings == 1){

        fprintf (stderr, "Exiting AGROS. The incident will be reported. \n");
        if (ag_config->loglevel >= 1)    syslog (LOG_NOTICE, "User reached Max warnings. \n");
        kill (getppid(), SIGTERM);
        _exit (EXIT_FAILURE);
    }
}


/*
 * line arg should be 'foo=bar'
 */
inline int ag_setenv (char *line)
{
    char *_envline = (char *)malloc (sizeof (line));

    if (strchr (line, '=')){
        strcpy (_envline, line);

        printf ("%s", _envline);
        putenv (_envline);
        return EXIT_SUCCESS;
    }
    else{

        free (_envline);
        return EXIT_FAILURE;
    }
}

inline int ag_unsetenv (char *line)
{
    char*_envline = NULL;

    if (strlen (line) < MAX_LINE_LEN){

        _envline = (char *)malloc (sizeof (line) + 1);
        sprintf (_envline, "%s=", line);
        putenv (_envline);
    }

    free (_envline);
    return EXIT_SUCCESS;
}

inline void init_user (user_t *user)
{
    struct passwd *pwd = getpwuid (getuid());

    user->username = (char *)malloc (sizeof (strlen (pwd->pw_name)));
    user->homedir = (char *)malloc (sizeof (strlen (pwd->pw_dir)));

    user->username = pwd->pw_name;
    user->homedir = pwd->pw_dir;
}

char *read_input (int linelength){
    char* CRPosition = NULL;
    char *cmdline = (char *)malloc (sizeof (char) * linelength);


    if (fgets (cmdline, linelength, stdin)){
        CRPosition = strchr (cmdline, '\n');
        if (CRPosition)
            *CRPosition = '\0';

    }else
        return NULL;

    return cmdline;
}
