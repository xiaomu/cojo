/*
 * author: xiaomu
 * date: 2011/06/28
 */

#ifndef COJO_DB_H
#define COJO_DB_H

struct cojo_user
{
	char *cojo_user_id;
	char *cojo_user_name;
	char *cojo_user_pwd;
};

typedef struct cojo_user cojo_user_t;

extern char cojo_userdb[];

cojo_user_t *cojo_get_user_byId(char *cojo_user_id);
int cojo_update_user(cojo_user_t *cojo_user_info);
int cojo_add_user(cojo_user_t *cojo_user_info);
int cojo_del_user_byId(char *cojo_user_id);

#endif
