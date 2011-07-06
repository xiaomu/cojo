#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdio.h>

#include "cojo_client.h"
#include "cojo_log.c"

static void cojo_show_menu(void);

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
	int choice;

	fd_set readfds;
	struct timeval s_time;
	// init the cilent
	cojo_init_client();

	// 设置要连接的服务器端的 sockaddr_in
	cojo_cli_set_addr();

	// build a socket
	cli_addr_len = sizeof(cojo_client.cojo_client_addr);

	cli_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cli_sockfd == -1)
	{
		cojo_log("socket failed in cojo_client_main.c main.\n");
		exit(1);
	}

	ret = connect(cli_sockfd, (struct sockaddr *)&(cojo_client.cojo_client_addr), cli_addr_len);
	if(ret == -1)
	{
		cojo_log("connect failed in cojo_client_main.c main.\n");
		perror("client_main connect : ");
		exit(1);
	}

	
	fprintf(stdout, "connect succeed.\n");

	cojo_show_menu();
	fprintf(stdout, "input your choice: ");
	fscanf(stdin, "%d", &choice);

	while(1)
	{
		if(b_log == 0)
		{
			s_time.tv_sec = 2;
			s_time.tv_usec = 500000;
			FD_ZERO(&readfds);
			FD_SET(cli_sockfd, &readfds);
			ret = select(cli_sockfd + 1, &readfds, (fd_set *)0,
					(fd_set *)0, &s_time);
			if((ret != 0) && (ret != -1))
			{
				cojo_cli_comn(cli_sockfd);
			}
		}
				
		switch(choice)
		{
			case 1:
				b_reg =	cojo_cli_register(cli_sockfd);
				if(b_reg == -1)
				{
					fprintf(stdout, "register failed.\n");
				}
				else if(b_reg == 0)
				{
					fprintf(stdout, "register succeed.\n");
				}
				break;
			case 2:
				b_log = cojo_cli_login(cli_sockfd);
				if(b_log == -1)
				{
					fprintf(stdout, "login failed.\n");
				}
				else if(b_log == 0)
				{
					fprintf(stdout, "login succeed.\n");
				}
				break;
			case 3:
				if(b_log == 0)
				{
					cojo_cli_sltid(cli_sockfd);
				}
				else
				{
					fprintf(stdout, "you have not log.\n");
				}
				break;
			case 0:
				close(cli_sockfd);
				exit(0);
			default:
				fprintf(stdout, "choice not recognzied.\n");
		}
		
		cojo_show_menu();
		fprintf(stdout, "input your choice : ");
		fscanf(stdin, "%d", &choice);
	}
}


static void cojo_show_menu(void)
{
	char **p = menu;
	
	while(*p != NULL)
	{
		printf("%s", *p);
		p ++;
	}
}/* cojo_show_menu() */

