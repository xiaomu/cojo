/*
 * author: xiaodeng
 * date: 2011/07/03
 */

#ifndef COJO_USER_ONLINE
#define COJO_USER_ONLINE

struct cojo_user_online
{
	cojo_user_t cojo_user_obj;
	int cojo_user_sockfd;
	int cojo_bool_comn;
	struct cojo_user_online *next;
}
typedef struct cojo_user_online cojo_user_online_t;

//declaration of functions
int *cojo_get_sockfd_byId(
		cojo_user_online_t *cojo_user_online_list_head,
	       	const char *cojo_user_id
		);

int cojo_add_online_user(
		cojo_user_online_t *cojo_user_online_list_head,
		cojo_user_online_t *cojo_user_online_obj
		);

int cojo_del_online_user(
		cojo_user_online_t *cojo_user_online_list_head,
		const char *cojo_user_id
		);

char *cojo_get_user_id_online(
		cojo_user_online_t *cojo_user_online_list_head
		);

#endif
