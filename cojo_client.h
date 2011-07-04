#ifndef COJO_CLIENT_H
#define COJO_CLIENT_H

#include <netinet/in.h>

struct cojo_client_def
{
	sockaddr_in cojo_client_addr;
};
typedef struct cojo_client_def cojo_client;
