#ifndef COJO_SERVER_H
#define COJO_SERVER_H

#include <netinet/in.h>

struct cojo_server_def
{
	sockaddr_in cojo_server_addr;
	int cojo_user_online_num;
	cojo_user_online_t *cojo_user_online_list;
};
typedef struct cojo_server_def cojo_server_t;

cojo_server_t cojo_server;

enum cojo_con_type_def
{
	REGISTER,
	LOGIN,
	SLTID,
	QUIT,
	MSG
};
typedef cojo_con_type_def cojo_con_type_t;

struct cojo_msg
{
	cojo_con_type_t cojo_con_type;
	char *content;
}
typedef struct cojo_msg cojo_msg_t;

#define COJO_MSG_LEN 100


#endif
