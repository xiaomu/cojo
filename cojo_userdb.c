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
	read_lock.l_start = 0;
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
			memset(user, '\0', sizeof(user));

			int i, j;
			
			strncpy(user->cojo_user_id, cojo_user_id, COJO_USER_ID_LEN);	
			
			i = COJO_USER_ID_LEN;
			j = ++i;
			while((buf[j] != '\t')&&(j<COJO_USER_TOTAL_LEN)&&(buf[j] != '\n'))
				j++;
			strncpy(user->cojo_user_pwd, &buf[i+1], j-i-1);
			
			i = j;
			j = ++i;
			while((buf[j] != '\t')&&(j<COJO_USER_TOTAL_LEN)&&(buf[j] != '\n'))
				j++;
			strncpy(user->cojo_user_name, &buf[i +1], j-i-1);

			i = j;
			j = ++i; 
			while((buf[j] != '\t')&&(j<COJO_USER_TOTAL_LEN)&&(buf[j] != '\n'))
				j++;
			strncpy(user->cojo_user_crypt, &buf[i+1], j-i-1);

			break;
		}
	}

	read_lock.l_type = F_UNLCK;
	ret = fcntl(fd, F_SETLK, &read_lock);
	if(ret == -1)
		cojo_log("fcntl failed in cojo_userdb.c cojo_get_user_byId().\n");
	
	fclose(fp);
	return user;
} /* cojo_get_user_byId() */


// open the userdb file and return the file descripter
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


// add a new user
int cojo_add_user(const cojo_user_t *cojo_user_obj)
{
	int fd;
	FILE *fp = NULL;
	struct flock write_lock;
	int ret;

	// get the file descriptor
	fd = cojo_open_userdb(O_WRONLY);
	if(fd == -1)
	{
		cojo_log("cojo_pen_userdb failed in cojo_userdb.c cojo_add_user.\n");
		return -1;
	}

	// lock the file
	write_lock.l_type = F_WRLCK;
	write_lock.l_whence = SEEK_SET;
	write_lock.l_start = 0;
	write_lock.l_len =0;

	ret = fcntl(fd, F_SETLK, &write_lock);
	if(ret == -1)
	{
		cojo_log("fcntl failed in cojo_userdb.c cojo_add_user.\n");
		return -1;
	}

	// convert the file descriptor to stream
	fp = fdopen(fd, "a");
	if(fp == NULL)
	{
		cojo_log("fdopen failed in cojo_userdb.c cojo_add_user.\n");
		return -1;
	}

	fprintf(fp, "%s\t", cojo_user_obj->cojo_user_id);
	fprintf(fp, "%s\t", cojo_user_obj->cojo_user_pwd);
	fprintf(fp, "%s\t", cojo_user_obj->cojo_user_name);
	fprintf(fp, "%s\t", cojo_user_obj->cojo_user_crypt);

	// unlock the file
	write_lock.l_type = F_UNLCK;
	ret = fcntl(fd, F_SETLK, &write_lock);
	if(ret == -1)
		cojo_log("fcntl failed in cojo_userdb.c cojo_add_user.\n");
	fclose(fp);

	return 0;
}/* cojo_add_user() */

// del a user by id
int cojo_del_user_byId(const char *cojo_user_id)
{
	int fd;
	FILE *fp = NULL;
	FILE *tmp_fp = NULL;
	struct flock wr_lock;
	int ret;
	char buf[COJO_USER_TOTAL_LEN] = {'\0'};

	fd = cojo_open_userdb(O_RDWR);
	if(fd == -1)
	{
		cojo_log("cojo_open_userdb failed in cojo_userdb.c cojo_del_user_byId.\n");
		return -1;
	}

	wr_lock.l_type = F_WRLCK;
	wr_lock.l_whence = SEEK_SET;
	wr_lock.l_start = 0;
	wr_lock.l_len = 0;

	ret = fcntl(fd, F_SETLK, &wr_lock);
	if(ret == -1)
	{
		cojo_log("fcntl failed in cojo_userdb.c cojo_del_user_byId.\n");
		return -1;
	}

	fp = fdopen(fd, "r+");
	if(fp == NULL)
	{
		cojo_log("fcntl failed in cojo_userdb.c cojo_del_user_byId.\n");
		return -1;
	}

	tmp_fp = tmpfile();
	if(tmp_fp == NULL)
	{
		cojo_log("tmpfile failed in cojo_userdb.c cojo_del_user_byId.\n");
		return -1;
	}

	while(fgets( buf, COJO_USER_TOTAL_LEN, fp) != NULL)
	{
		if(strncmp(cojo_user_id, buf, COJO_USER_ID_LEN) != 0)
		{
			fputs(buf, tmp_fp);
		}
	}

	ftruncate(fd, 0);
	rewind(fp);
	rewind(tmp_fp);
	while(fgets( buf, COJO_USER_TOTAL_LEN, tmp_fp) != NULL)
	{
		fputs(buf, fp);
	}

	
	wr_lock.l_type = F_UNLCK;
	ret = fcntl(fd, F_SETLK, &wr_lock);
	if(ret == -1)
		cojo_log("fcntl failed in cojo_userdb.c cojo_del_user_byId().\n");
	fclose(tmp_fp);
	fclose(fp);
	return 0;
} /* cojo_del_user_byId() */
			
// alter a user's information
// first, del his origin infor
// then, add new infor
int cojo_alter_user(const cojo_user_t *cojo_user_obj)
{
	char *cojo_user_id = NULL;
	int ret;

	cojo_user_id = cojo_user_obj->cojo_user_id;
	ret = cojo_del_user_byId( cojo_user_id);
	if(ret == -1)
	{
		cojo_log("cojo_del_user_byId failed in cojo_userdb.c cojo_alter_user.\n");
		return -1;
	}
	ret = cojo_add_user(cojo_user_obj);
	if(ret == -1)
	{
		cojo_log("cojo_add_user failed in cojo_userdb.c cojo_alter_user.\n");
		return -1;
	}

	return 0;
}/* cojo_add_user() */


// get a cojo_user_t object from line space.
// return null if failed.
cojo_user_t *
cojo_get_user_from_lnspa(const char *cojo_lnspa)
{
	cojo_user_t *user_obj = NULL;
	int i, j;
	const char *buf = cojo_lnspa;

	user_obj = (cojo_user_t *)malloc(sizeof(cojo_user_t));
	if(user_obj == NULL)
	{
		cojo_log("malloc failed in cojo_userdb.c cojo_get_user_from_lnspa().\n");
		return NULL;
	}
	memset(user_obj, '\0', sizeof(user_obj));
			
	// get the user's id
	i = 0;
	j = i;
	while((ptr[j] != '\t')&&(ptr[j] !='\0')&&(ptr[j] != '\n')&&(j<COJO_USER_TOTAL_LEN))
	{
		j++;
	}
	strncpy(user_obj.cojo_user_id, &ptr[i], j-i);

	// get the user's pwd
	i = ++j;
	while((ptr[j] != '\t')&&(ptr[j] !='\0')&&(ptr[j] != '\n')&&(j<COJO_USER_TOTAL_LEN))
	{
		j++;
	}
	strncpy(user_obj.cojo_user_pwd, &ptr[i], j-i);

	// get the user's name
	i = ++j;
	while((ptr[j] != '\t')&&(ptr[j] !='\0')&&(ptr[j] != '\n')&&(j<COJO_USER_TOTAL_LEN))
	{
		j++;
	}
	strncpy(user_obj.cojo_user_name, &ptr[i], j-i);

	// get the user's crypt
	i = ++j;
	while((ptr[j] != '\t')&&(ptr[j] !='\0')&&(ptr[j] != '\n')&&(j<COJO_USER_TOTAL_LEN))
	{
		j++;
	}

	return user_obj;
}/* cojo_get_user_from_lnspa() */
