/*
 * author: xiaomu
 * date: 2011/06/28
 */

#ifndef COJO_GLOBAL
#define COJO_GLOBAL

#define COJO_FILENAME_MAX_LENGTH 256
#define COJO_VARNAME_MAX_LENGTH 50

char cojo_userdb[COJO_FILENAME_MAX_LENGTH] = {'\0'};
char cojo_config_filename[] = "cojo_config";
char cojo_default_var_userdb = "userdb";

int cojo_read_global(void);

#endif
