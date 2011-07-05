#ifndef COJO_ADDR_H
#define COJO_ADDR_H

#include <netinet/in.h>

struct cojo_addr_item
{
	struct in_addr cojo_sin_addr;
	struct cojo_addr_item *next;
};
typedef struct cojo_addr_item cojo_addr_item_t;

cojo_addr_item_t *cojo_get_addr(void);

#endif


