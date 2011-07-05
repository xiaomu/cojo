#include <stdio.h>

#include "cojo_log.h"
#include "cojo_server.h"

int main(int argc, char *argv[])
{

	// init the server
	cojo_init_server();

	// set server's socket addr
	cojo_set_addr();
	
	// run the server
	cojo_server_work();

	//cojo_clear();

	return 0;
}

