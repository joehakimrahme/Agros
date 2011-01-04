#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agros.h"

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
    fprintf(stdout, "Enter Command$ ");
}

void print_help(void){
    fprintf(stdout, "Welcome to Agros, the newer limited shell.\n\tYou can type 'exit' to close the shell.\n\n\n");
}
