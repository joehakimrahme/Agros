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
#define MAX_LINE_LEN 256
#define MAX_ARGS (MAX_LINE_LEN/2)
#define WHITESPACE " \t\n"




#define OTHER_CMD          -1
#define EMPTY_CMD           0

#define CD_CMD              1
#define ENV_CMD             2
#define EXIT_CMD            3
#define HELP_CMD            4
#define SHORTHELP_CMD       5
#define SETENV_CMD          6
#define UNSETENV_CMD        7




#define AG_FALSE 0
#define AG_TRUE  1



/*
 * This structure allows me to handle built-in commands with a switch statement
 * instead of multiple ifs. Since I cannot switch on a string, I associate a
 * command_code to each built-in command. The codes are define as preprocessor
 * instructions.
 */

typedef struct built_in_commands built_in_commands;
struct built_in_commands{
    char command_name[MAX_LINE_LEN];
    int command_code;
    char command_desc[MAX_LINE_LEN];

};

/*
 * This structure holds user input. 3 fields:
 *   - argv: an array of strings. Each word of the input is a case of the array.
 *   - name: the name of the executable called. By default it's argv[0]
 *   - argc: the number of words given in input. It's equivalent to the length
 *           of argv.
 *
 * Note: Some filenames have a space character (" ") in their names. I will have
 * to deal with that properly ... someday.
 */

typedef struct command_t command_t;
struct command_t{		
    char* name;
    int argc;
    char* argv[MAX_ARGS+1];
};

typedef struct user_t user_t;
struct user_t{
    char *username;
    char *homedir;
};



/* AGROS functions */
int     get_cmd_code        (char* cmd_name);
char*   get_cmd_desc        (char* cmd_name);
void    decrease_warnings   (config_t* ag_config);
int     check_validity      (command_t cmd, config_t config);
void    print_env           (char* env_variable);
void    print_allowed       (char** allowed);
void    print_forbidden     (char** forbidden);
void    print_help          (config_t* config, char* helparg);

/* Unix shell functions */
void	get_prompt          (char *prompt, int length, user_t *user);
char*	read_input          (char *prompt);
void    parse_command       (char *cmdline, command_t *cmd);
void    change_directory    (char* path, int loglevel, user_t user);
void	initialize_readline (config_t *config);
char*	make_completion	    (char *string);
char**	cmd_completion	    (const char *text, int start, int end);
char*	cmd_generator	    (const char *text, int state);
int     ag_setenv           (char* line);
int     ag_unsetenv         (char* line);
void    init_user           (user_t *user);
