/*
 * author: xiaomu
 * date: 2011/06/30
 */
#include <stdio.h>
#include "cojo_log.h"
#include "cojo_test_rduser.h"
#include <stdlib.h>

int main()
{
	cojo_user_t *user;
	char *user_id = "0123456789";
	
	user = cojo_get_user_byId(user_id);
	if(user == NULL)
	{
		cojo_log("failed in cojo_test_rduser.c main.\n");
		exit(1);
	}

	printf("%s\n", user->cojo_user_id);
	printf("%s\n", user->cojo_user_name);
	printf("%s\n", user->cojo_user_pwd);

	return 0;
}
