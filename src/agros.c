#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "agros.h"

//TODO: Replace size by dynamic allocation.
built_in_commands my_commands[100] = {
        {"exit" , EXIT_CMD  },
        {""     , EMPTY_CMD },
        {"cd"   , CD_CMD    },
        {"?"    , HELP_CMD  }
};


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
    fprintf(stdout, "%s@%s$ ", getenv("USERNAME"), getenv("PWD"));
}

void print_help(void){
    fprintf(stdout, "\nWelcome to AGROS, the newer limited shell.\nNote: At any time, you can type 'exit' to close the shell.\n\n\n");
}

void change_directory(char* PATH){
    if (chdir(PATH) == 0){
        getcwd(PATH, MAX_LINE_LEN);
        setenv("PWD", PATH, 1);
    }else{
        fprintf(stdout, "%s: Could not change to such directory\n", PATH);
    }
}

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

int get_cmd_code(char* cmd_name){
    int i = 0;
    for (i=0; i<100; i++){
        if (!strcmp(my_commands[i].command_name, cmd_name)){
            return my_commands[i].command_code;
        }
    }
    return OTHER_CMD;
}
