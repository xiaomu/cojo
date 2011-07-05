#ifndef COJO_CLIENT_H
#define COJO_CLIENT_H

#include <netinet/in.h>

struct cojo_client_def
{
	struct sockaddr_in cojo_client_addr;
};
typedef struct cojo_client_def cojo_client_t;
cojo_client_t cojo_client;

int cojo_cli_set_addr(void);
void cojo_init_client(void);
int cojo_cli_register(int cli_sockfd);
int cojo_cli_login(int cli_sockfd);
void cojo_cli_sltid(int cli_sockfd);
void *cojo_cli_sltid_write(void *arg);


#endif
