/*
 * author: xiaomu
 * date: 2011/06/28
 */

// define operations on db
#include <unistd.h>
#include <fcntl.h>
#include "cojo_db_oper.h"

cojo_user_t *cojo_get_user_byId(const char *cojo_user_id)
{	 
	int fd;
	struct flock read_lock;
	int ret;

	fd = open(cojo_userdb, O_RDONLY, 0);
	

