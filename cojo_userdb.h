/*
 * author: xiaomu
 * date: 2011/06/28
 */

#ifndef COJO_DB_H
#define COJO_DB_H


#define COJO_USER_ID_LEN 10
#define COJO_USER_NAME_LEN 16
#define COJO_USER_PWD_LEN 10
#define COJO_USER_CRYPT_LEN 10
#define COJO_USER_TOTAL_LEN 50

struct cojo_user
{
	char cojo_user_id[COJO_USER_ID_LEN + 1];
	char cojo_user_pwd[COJO_USER_PWD_LEN + 1];
	char cojo_user_name[COJO_USER_NAME_LEN + 1];
	char cojo_user_crypt[COJO_USER_CRYPT_LEN +1];
};

typedef struct cojo_user cojo_user_t;

extern char *cojo_get_userdb_filename(void);

cojo_user_t *cojo_get_user_byId(const char *cojo_user_id);
int cojo_alter_user(const cojo_user_t *cojo_user_obj);
int cojo_add_user(const cojo_user_t *cojo_user_obj);
int cojo_del_user_byId(const char *cojo_user_id);
static int cojo_open_userdb(const int flags);
cojo_user_t *cojo_get_user_from_lnspa(const char *cojo_lnspa);
#endif
