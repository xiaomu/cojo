/*
 * author: xiaomu
 * date: 2011/06/28
 */

// define operations on db
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cojo_userdb.h"
#include "cojo_log.h"

cojo_user_t *cojo_get_user_byId(const char *cojo_user_id)
{	 
	int fd;
	FILE *fp;
	struct flock read_lock;
	int ret;
	char buf[COJO_USER_TOTAL_LEN] = {'\0'};
	cojo_user_t *user = NULL;

	fd = cojo_open_userdb(O_RDONLY);	
	if( fd == -1)
	{
		cojo_log("cojo_open_userdb failed in cojo_userdb.c cojo_get_user_byId.\n");
		return NULL;
	}

	read_lock.l_type = F_RDLCK;
	read_lock.l_whence = SEEK_SET;
	read_lock.l_start = 10;
	read_lock.l_len = 0;

	ret = fcntl(fd, F_SETLK, &read_lock);
	if(ret == -1)
	{
		cojo_log("fcntl failed in cojo_userdb.c cojo_get_user_byId().\n");
		return NULL;
	}

	fp = fdopen(fd, "r");
	if(fp == NULL)
	{
		cojo_log("fdopen failed in cojo_userdb.c cojo_get_user_byId().\n");
		return NULL;
	}

	while(fgets( buf, COJO_USER_TOTAL_LEN, fp) != NULL)
	{
		if(strncmp(cojo_user_id, buf, COJO_USER_ID_LEN) == 0)
		{
			user = (cojo_user_t *)malloc(sizeof(cojo_user_t));
			if(user == NULL)
			{
				cojo_log("malloc failed in cojo_userdb.c fcojo_get_user_byId().\n");
				return NULL;
			}

			strncpy(user->cojo_user_id, cojo_user_id, COJO_USER_ID_LEN);	
			strncpy(user->cojo_user_name, &buf[COJO_USER_ID_LEN +1], COJO_USER_NAME_LEN);
			strncpy(user->cojo_user_pwd, &buf[COJO_USER_ID_LEN + COJO_USER_NAME_LEN +2], COJO_USER_PWD_LEN);
			break;
		}
	}

	fclose(fp);
	read_lock.l_type = F_UNLCK;
	ret = fcntl(fd, F_SETLK, &read_lock);
	if(ret == -1)
		cojo_log("fcntl failed in cojo_userdb.c cojo_get_user_byId().\n");
	
	return user;
} /* cojo_get_user_byId() */


static int cojo_open_userdb(const int flags)
{
	int fd;
	char *cojo_userdb_filename = NULL;
	
	cojo_userdb_filename = cojo_get_userdb_filename();
	if(cojo_userdb_filename == NULL)
	{
		cojo_log("cojo_get_userdb_filename failed in cojo_userdb.c cojo_open_userdb.\n");
		return -1;
	}

	fd = open(cojo_userdb_filename, flags, 0);

	return fd;
} /* cojo_open_userdb() */
