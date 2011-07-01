/*
 * author: xiaomu
 * date: 2011/06/28
 */

// to read the configure information
#include <stdio.h>
#include <string.h>
#include "cojo_config.h"
#include <stdlib.h>
#include "cojo_log.h"

char *cojo_get_userdb_filename(void)
{
	char *cojo_userdb_filename = NULL;

	cojo_userdb_filename = (char *)malloc(COJO_FILENAME_MAX_LEN * sizeof(char));
	if(NULL == cojo_userdb_filename)
	{
		cojo_log("malloc failed in cojo_config.c --- cojo_get_userdb_filename.\n");
		return NULL;
	}
	memset(cojo_userdb_filename, '\0', COJO_FILENAME_MAX_LEN);
	cojo_read_config(cojo_default_var_userdb, cojo_userdb_filename);
	return cojo_userdb_filename;
}

static int cojo_read_config(char *cojo_var_name, char *cojo_config_val)
{
	FILE *f_config = NULL;
	char buf[COJO_FILENAME_MAX_LEN] = {'\0'};
	char var_name[COJO_VARNAME_MAX_LEN] = {'\0'};

	int temp_i;
	
	f_config = fopen( cojo_config_filename, "r");
	if(NULL == f_config)
	{
		cojo_log("open configure file failed.\n");
		return 1;
	}

	while( fgets( buf, COJO_FILENAME_MAX_LEN, f_config) != NULL)
	{
		temp_i = 0;
		while(buf[temp_i] != '\t')
		{
			var_name[temp_i] = buf[temp_i];
			temp_i ++;
		}
		var_name[temp_i] = '\0';

		if(strcmp(var_name, cojo_var_name) == 0)
		{
			int len;
			strncpy(cojo_config_val, &buf[temp_i+3], COJO_FILENAME_MAX_LEN-temp_i-2);
			len = strlen(cojo_config_val);
			if(cojo_config_val[len-1] == '\n')
				cojo_config_val[len-1]='\0';
		}
	}
		
	fclose(f_config);

	return 0;
}
		

