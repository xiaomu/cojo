#ifndef COJO_USER_ONLINE
#define COJO_USER_ONLINE

#include <sys/un.h>	// include sockaddr_un
#include <netinet/in.h>	// include sockaddr_in

struct cojo_user_online
{
	cojo_user_t cojo_user_obj;
	struct sockaddr_in cojo_user_addr;
	struct cojo_user_online *next;
}
typedef struct cojo_user_online cojo_user_online_t;

//declaration of functions
cojo_sockaddr_t *cojo_get_addr_byId(
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
