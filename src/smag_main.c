#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "iniparser.h"
#include "smags.h"



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
    int i = 0;
    char* _aword = NULL;
    char* _bword = NULL;

    dictionary *conf_dict = (dictionary *)malloc (sizeof (dictionary));
    conf_dict = iniparser_load (CONF_FILE);
    

    config->loglevel = iniparser_getint (conf_dict, ag_get_agkey (conf_dict, username, "loglevel", BASEGRP), 0);
    config->welcome_message = iniparser_getstring (conf_dict, ag_get_agkey (conf_dict, username, "welcome", BASEGRP), "");


    config->_allowed_string = iniparser_getstring (conf_dict, ag_get_agkey (conf_dict, username, "allowed", BASEGRP), "");
    if (strlen(config->_allowed_string) == 0){
        fprintf (stderr, "Missing allowed config\n");
        if (config->loglevel >= 1) syslog (LOG_ERR, "Missing allowed config\n");
        _exit(EXIT_FAILURE);
    }
    config->allowed_list = (char **)malloc (sizeof (char *) * 256);

    _aword = strtok (config->_allowed_string, ",");

    if (!_aword) { _aword = ""; } // Fixes blank line bug

    while (_aword)
    {
        config->allowed_list[i] = _aword;
        _aword = strtok (NULL, ",");
        i++;
    }


    config->_forbidden_string = iniparser_getstring (conf_dict, ag_get_agkey (conf_dict, username, "forbidden", BASEGRP), "");
    config->forbidden_list = (char **)malloc (sizeof (char *) * 256);

    _bword = strtok (config->_forbidden_string, ",");

    if (!_bword) { _bword = ""; } // Fixes blank line bug

    i=0;
    while (_bword)
    {
        config->forbidden_list[i] = _bword;
        _bword = strtok (NULL, ",");
        i++;
    }

    config->warnings = iniparser_getint (conf_dict, ag_get_agkey (conf_dict, username, "warnings", BASEGRP), -1);
    if (config->warnings < -1){
        fprintf(stderr, "Error: Bad 'warnings' config input.\n");
        if (config->loglevel >= 1) syslog (LOG_ERR, "Error: Bad 'warnings' config input.\n");
        _exit(EXIT_FAILURE);
    }

    if (config->warnings == -1){
        fprintf (stderr, "Missing warnings config\n");
        if (config->loglevel >= 1) syslog (LOG_ERR, "Missing warnings config\n");
        _exit(EXIT_FAILURE);
    }
}
