#ifndef COJO_SERVER_H
#define COJO_SERVER_H

#include <netinet/in.h>
#include "cojo_user_online.h"

struct cojo_server_def
{
	struct sockaddr_in cojo_server_addr;
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
};
typedef enum cojo_con_type_def cojo_con_type_t;

struct cojo_msg
{
	cojo_con_type_t cojo_con_type;
	char *content;
};
typedef struct cojo_msg cojo_msg_t;

#define COJO_MSG_LEN 100

void cojo_init_server(void);
int cojo_set_addr(void);
void cojo_server_work(void);
void *cojo_handle_con(void *arg);
int cojo_do_register(cojo_msg_t *cojo_msg_obj);
int cojo_do_login(cojo_msg_t *cojo_msg_obj);
int cojo_do_sltid(cojo_msg_t *cojo_msg_obj);
void cojo_comn(int cojo_sockfd, int cojo_con_sockfd);
int cojo_del_userol_byfd(int fd);
int cojo_rel_userol_byfd(int fd);

#endif
