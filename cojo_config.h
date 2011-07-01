/*
 * author: xiaomu
 * date: 2011/06/28
 */

#ifndef COJO_GLOBAL
#define COJO_GLOBAL

#define COJO_FILENAME_MAX_LEN 256
#define COJO_VARNAME_MAX_LEN 50


static char cojo_config_filename[] = "cojo_config";
static char cojo_default_var_userdb[] = "userdb";

static int cojo_read_config(char *cojo_var_name, char *cojo_config_val);
char *cojo_get_userdb_filename(void);
#endif
