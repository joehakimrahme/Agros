#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iniparser.h"

#define BASEGRP "default"

/*
 * A structure that holds the AGROS conf.
 */

typedef struct config_t config_t;
struct config_t{
    char** allowed_list;
    char** forbidden_list;
    char* _allowed_string;
    char* _forbidden_string;
    int allowed_nbr;
    int forbidden_nbr;
    char* welcome_message;
    int loglevel;
    int warnings;
};


char*    get_inipkey    (char* sec, char* key);
int      sec_haskey     (dictionary *dict, char* section, char* key);
char    *ag_get_agkey   (dictionary *dict, char* conf_group, char* conf_groupkey, char* conf_defgroup);
void     parse_config   (config_t* config, char* username);
