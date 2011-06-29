/*
 * author: xiaomu
 * date: 2011/06/28
 */

// to read the configure information
#include <stdio.h>
#include <string.h>

int cojo_read_global(void)
{
	FILE *f_config = NULL;
	char buf[COJO_FILENAME_MAX_LENGTH] = {'\0'};
	char var_name[COJO_VARNAME_MAX_LENGTH] = {'\0'};

	int temp_i;
	
	f_config = fopen( cojo_config_filename, "r");
	if(NULL == f_config)
	{
		cojo_log("open configure file failed.\n");
		return 1;
	}

	while( fgets( buf, COJO_FILENAME_MAX_LENGTH, f_config) != NULL)
	{
		temp_i = 0;
		while(buf[temp_i] != '\t')
		{
			var_name[temp_i] = buf[temp_i];
			temp_i ++;
		}
		var_name[temp_i] = '\0';

		if(strcmp(var_name, cojo_default_var_userdb) == 0)
		{
			strncpy(cojo_userdb, buf[temp_i+2], COJO_FILENAME_MAX_LENGTH);
		}
	}
		
	fclose(f_config);

	return 0;
}
		

