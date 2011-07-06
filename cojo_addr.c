#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "cojo_log.h"
#include "cojo_addr.h"

// 获得本机可用的IP地址
cojo_addr_item_t *
cojo_get_addr(void)
{
	struct ifaddrs *ifAddrStruct = NULL;
	struct ifaddrs *ifa = NULL;
	cojo_addr_item_t *cojo_addr = NULL;	// 跟踪链表头部
	cojo_addr_item_t *cojo_addr_temp = NULL;	// 指向新分配节点
	cojo_addr_item_t *cojo_addr_ptr = NULL;		// 跟踪链表尾部

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
				cojo_addr = cojo_addr_ptr;
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
}/* cojo_get_addr() */
