#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "cojo_log.h"
#include "cojo_addr.h"

cojo_addr_item_t *
cojo_get_addr(void)
{
	struct ifaddrs *ifAddrStruct = NULL;
	struct ifaddrs *ifa = NULL;
	cojo_addr_item_t *cojo_addr = NULL;
	cojo_addr_item_t *cojo_addr_temp = NULL;
	cojo_addr_item_t *cojo_addr_ptr = NULL;

	cojo_addr_ptr = cojo_addr;

	getifaddrs(&ifAddrStruct);
	for(ifa = ifAddrStruct; ifa != NULL; ifa=ifa->ifa_next)
	{
		if(ifa->ifa_addr->sa_family == AF_INET)
		{
			cojo_addr_temp =(cojo_addr_item_t *)
				malloc(sizeof(cojo_addr_item_t));
			if(cojo_addr_temp == NULL)
			{
				cojo_log("malloc failed in cojo_addr.c cojo_get_addr.\n");
				return NULL;
			}
			cojo_addr_temp->next = NULL;
			memcpy(&(cojo_addr_temp->cojo_sin_addr), 
					&(((struct sockaddr_in *)ifa->ifa_addr)->sin_addr),
					sizeof(struct in_addr)
			      );
			
			if(cojo_addr_ptr == NULL)
			{
				cojo_addr_ptr = cojo_addr_temp;
			}
			else
			{
				cojo_addr_ptr->next = cojo_addr_temp;
				cojo_addr_ptr = cojo_addr_ptr->next;
			}

		}
	}
		if(ifAddrStruct != NULL)
		freeifaddrs(ifAddrStruct);

		return cojo_addr;
}
