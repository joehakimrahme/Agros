#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iniparser.h"

#define CONF_FILE "agros.conf"
#define BASEGRP "General"
#define LEN 100

char *smags_get_key (dictionary *dict, char *group, char* key);
char *smags_get_iniparserkey (char* group, char* key);

int main()
{
    dictionary *dict = (dictionary *)malloc (sizeof (dictionary));
    char *key = NULL;

    dict = iniparser_load (CONF_FILE);

    key = smags_get_key (dict, "root", "allowed");
    printf ("%s\n", key);
    key = smags_get_key (dict, "root", "forbidden");
    printf ("%s\n", key);

    return EXIT_SUCCESS;
}

char *smags_get_iniparserkey (char* group, char* key)
{
    char *smags_fullkey = NULL;;
    int fullkey_len = 0;
    char *sep = ":";

    fullkey_len = strlen(group) + strlen (key) + strlen (sep);
    smags_fullkey = (char *) malloc (sizeof (char) * fullkey_len);
    
    strcpy (smags_fullkey, group);
    strcat (smags_fullkey, sep);
    strcat (smags_fullkey, key);

    return smags_fullkey;
}



char *smags_get_key (dictionary *dict, char *group, char* key)
{
    char *smags_fullkey = NULL;

    smags_fullkey = smags_get_iniparserkey (group, key);

    if (iniparser_find_entry (dict, smags_fullkey)){
        return smags_fullkey;
    }
    else
    {
        smags_fullkey = smags_get_iniparserkey (BASEGRP, key);
        if (iniparser_find_entry (dict, smags_fullkey))
            return smags_fullkey;
    }
        
    return NULL;
}
