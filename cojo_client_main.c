#include "cojo_client.h"
#include "cojo_log.c"

static char *menu[] =
{
	"*******************************\n",
	"1 --- register\n",
	"2 --- login\n",
	"3 --- select a ID to communicate.\n",
	"0 --- quit.\n",
	"*******************************\n",
	NULL
};

int main(int argc, char *argv[])
{
	int cli_sockfd;
	int cli_addr_len;

	int b_reg = -1;
	int b_log = -1;

	int ret;

	// init the cilent
	cojo_init_client(void);

	// set the address of the client
	cojo_cli_set_addr(void);

	// build a socket
	cli_addr_len = sizeof(cojo_client.cojo_client_addr);

	cli_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cli_sockfd == -1)
	{
		cojo_log("socket failed in cojo_client_main.c main.\n");
		exit(1);
	}

	ret = connect(cli_sockfd, &(cojo_client.cojo_client_addr), cli_addr_len);
	if(ret == -1)
	{
		cojo_log("connect failed in cojo_client_main.c main.\n");
		exit(1);
	}

	
	fprintf(stdout, "connect succeed.\n");

	show_menu();
	fprintf(stdout, "input your choice: ");
	fscanf(stdin, "%d", &choice);

	while(1)
	{
		switch(choice)
		{
			case 1:
				b_reg =	cojo_cli_register(cli_sockfd);
				break;
			case 2:
				b_log = cojo_cli_login(cli_sockfd);
				break;
			case 3:
				if(b_log == 0)
				{
					cojo_cli_sltid(cli_sockfd);
				}
				else
				{
					fprintf("you have not log.\n");
				}
				break;
			case 0:
				close(cli_sockfd);
				exit(0);
			default:
				fprintf(stdout, "choice not recognzied.\n");
		}
		
		show_menu();
		fprintf(stdout, "input your choice.\n");
		fscanf(stdin, "%d", &choice);
	}

	
	
}


static void cojo_show_menu()
{
	char **p = menu;
	
	while(*p != NULL)
	{
		printf("%s", *p);
		*p ++;
	}
}/* cojo_show_menu() */

