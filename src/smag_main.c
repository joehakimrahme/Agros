#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iniparser.h"
#include "smags.h"

#define CONF_FILE "agros.conf"
#define BASEGRP "General"



char* get_inipkey (char* sec, char* key)
{
    size_t length = strlen (sec) + strlen (key) + 1;
    char* inipkey = (char *)malloc (length);

    sprintf (inipkey, "%s:%s", sec, key);

    return inipkey;
}

int sec_haskey (dictionary *dict, char* section, char* key){
    return iniparser_find_entry (dict, get_inipkey (section, key));
}


char *ag_get_agkey (dictionary *dict, char* conf_group, char* conf_groupkey, char* conf_defgroup){
    /* Checks in group section
     */
    if (sec_haskey (dict, conf_group, conf_groupkey))
        return get_inipkey (conf_group, conf_groupkey);

        /* Checks in default section
         */
     else if (sec_haskey (dict, conf_defgroup, conf_groupkey))
        return get_inipkey (conf_defgroup, conf_groupkey);

        /* Key is not found
         *
         * - Should I log something?
         * - Is NULL a good return value?
         */
         else
            return NULL;

}

void parse_config (config_t* config, char* username){
    dictionary *conf_dict = (dictionary *)malloc (sizeof (dictionary));
    conf_dict = iniparser_load (CONF_FILE);

    config->loglevel = iniparser_getint (conf_dict, ag_get_agkey (conf_dict, username, "loglevel", BASEGRP), 0);
    config->welcome_message = iniparser_getstring (conf_dict, ag_get_agkey (conf_dict, username, "welcome", BASEGRP), "");

    config->_allowed_string = iniparser_getstring (conf_dict, ag_get_agkey (conf_dict, username, "allowed", BASEGRP), "");

    config->allowed_list = (char **)malloc (sizeof (char *));


    config->allowed_list = &config->_allowed_string;


    config->_forbidden_string = iniparser_getstring (conf_dict, ag_get_agkey (conf_dict, username, "forbidden", BASEGRP), "");

    config->forbidden_list = (char **)malloc (sizeof (char *));
    config->forbidden_list = &config->_forbidden_string;

    config->warnings = iniparser_getint (conf_dict, ag_get_agkey (conf_dict, username, "warnings", BASEGRP), 0);

    iniparser_freedict(conf_dict);

}
