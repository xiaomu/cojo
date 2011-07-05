#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "cojo_log.h"
#include "cojo_client.h"
#include "cojo_addr.h"
#include "cojo_server.h"


static int b_comn = 0;
int
cojo_cli_set_addr(void)
{

	cojo_addr_item_t *cojo_addrs = NULL;
	cojo_addr_item_t *ptr = NULL;
	int cojo_port;

	int i, j;
	char addr_buf[INET_ADDRSTRLEN + 1];
	int choice;
	
	cojo_addrs = cojo_get_addr();
	ptr = cojo_addrs;

	fprintf(stdout, "select an address from below: \n");
	i = 0;
	while( ptr != NULL)
	{
		i ++;
		inet_ntop(AF_INET, &(ptr->cojo_sin_addr), addr_buf, INET_ADDRSTRLEN);
		addr_buf[INET_ADDRSTRLEN] = '\0';

		fprintf(stdout, "%d--- %s\n", i, addr_buf);

		ptr = ptr->next;
	}
	fprintf(stdout, "input your choice: ");
	fscanf(stdin, "%d", &choice);

	while((choice < 1) || (choice > i))
	{
		fprintf(stdout, "invalid choice, input again: ");
		fscanf(stdin, "%d", &choice);
	}

	j =0;
	ptr = cojo_addrs;
	while( ptr != NULL)
	{
		j ++;
		if(j == choice)
		{
			break;
		}
		ptr = ptr->next;
	}
	
	fprintf(stdout, "\ninput the port number (default: 9798) :");
	fscanf(stdin, "%d", &cojo_port);

	cojo_client.cojo_client_addr.sin_addr = ptr->cojo_sin_addr;
	cojo_client.cojo_client_addr.sin_port = cojo_port;

	return 0;
}/* cojo_set_addr() */ 


// init the client
void
cojo_init_client(void)
{
	
	cojo_client.cojo_client_addr.sin_family = AF_INET;
	cojo_client.cojo_client_addr.sin_port = 9798;
	cojo_client.cojo_client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
}/* cojo_init_client() */


// register from client
int
cojo_cli_register(int cli_sockfd)
{
	cojo_msg_t msg_obj;
	char buf[COJO_MSG_LEN] = {'\0'};
	char tmp[COJO_MSG_LEN] = {'\0'};
	int i;
	int ret;

	fd_set readfds;

	fprintf(stdout, "input id(length = %d) : ", COJO_USER_ID_LEN);
	fscanf(stdin, "%s", tmp);
	printf("\n");

	strncpy(buf, tmp, COJO_USER_ID_LEN);
	i = 0;
	while(buf[i] != '\0')
	{
		i ++;
	}
	buf[i] = '\t';
	i++;
	buf[i] = '\0';

	fprintf(stdout, "input pwd(length <= %d) : ", COJO_USER_PWD_LEN);
	fscanf(stdin, "%s", tmp);
	strncat(buf, tmp, COJO_USER_PWD_LEN);
	while(buf[i] != '\0')
	{
		i ++;
	}
	buf[i] = '\t';
	i ++;
	buf[i] = '\0';

	fprintf(stdout, "input name(length <= %d) : ", COJO_USER_NAME_LEN);
	fscanf(stdin, "%s", tmp);
	strncat(buf, tmp, COJO_USER_NAME_LEN);
	buf[i] = '\t';
	i ++;
	buf[i] = '\0';

	fprintf(stdout, "input crypt(length <= %d) : ", COJO_USER_CRYPT_LEN);
	fscanf(stdin, "%s", tmp);
	strncat(buf, tmp, COJO_USER_CRYPT_LEN);
	buf[i] = '\t';
	i ++;
	buf[i] = '\0';


	msg_obj.cojo_con_type = REGISTER;
	msg_obj.content = buf;

	write(cli_sockfd, buf, sizeof(msg_obj));

	FD_ZERO(&readfds);
	FD_SET(cli_sockfd, &readfds);

	ret = select(FD_SETSIZE, &readfds, (fd_set *)0,
			(fd_set *)0, (struct timeval *)0);

	if(ret == -1)
	{
		cojo_log("select failed in cojo_client.c cojo_cli_register.\n");
		return -1;
	}

	read(cli_sockfd, &msg_obj, COJO_MSG_LEN);
	if(buf[0] == 'y')
	{
		return 0;
	}
	else
	{
		return -1;
	}
}/* cojo_cli_register */


// login
int
cojo_cli_login(int cli_sockfd)
{
	cojo_msg_t msg_obj;
	char buf[COJO_MSG_LEN] = {'\0'};
	char tmp[COJO_MSG_LEN] = {'\0'};
	int i;
	int ret;

	fd_set readfds;

	fprintf(stdout, "input id(length = %d) : ", COJO_USER_ID_LEN);
	fscanf(stdin, "%s", tmp);
	printf("\n");

	strncpy(buf, tmp, COJO_USER_ID_LEN);
	i = 0;
	while(buf[i] != '\0')
	{
		i ++;
	}
	buf[i] = '\t';
	i++;
	buf[i] = '\0';

	fprintf(stdout, "input pwd(length <= %d) : ", COJO_USER_PWD_LEN);
	fscanf(stdin, "%s", tmp);
	strncat(buf, tmp, COJO_USER_PWD_LEN);
	while(buf[i] != '\0')
	{
		i ++;
	}
	buf[i] = '\t';
	i ++;
	buf[i] = '#';   // take the space of name
	i ++;
	buf[i] = '\t';
	i ++;
	buf[i] = '#';  // take the space of crypt
	i ++;
	buf[i] = '\0';

	

	msg_obj.cojo_con_type = LOGIN;
	msg_obj.content = buf;

	write(cli_sockfd, buf, sizeof(msg_obj));

	FD_ZERO(&readfds);
	FD_SET(cli_sockfd, &readfds);

	ret = select(FD_SETSIZE, &readfds, (fd_set *)0,
			(fd_set *)0, (struct timeval *)0);

	if(ret == -1)
	{
		cojo_log("select failed in cojo_client.c cojo_cli_register.\n");
		return -1;
	}

	read(cli_sockfd, &msg_obj, COJO_MSG_LEN);
	if(buf[0] == 'y')
	{
		return 0;
	}
	else
	{
		return -1;
	}
}/* cojo_cli_login() */


// 选择要通信的ID
void
cojo_cli_sltid(int cli_sockfd)
{	
	cojo_msg_t msg_obj;
	char buf[COJO_MSG_LEN] = {'\0'};
	char tmp[COJO_MSG_LEN] = {'\0'};
	int i;
	int ret;
	

	fd_set readfds, testfds;
	pthread_t a_thread;

	fprintf(stdout, "input id(length = %d) : ", COJO_USER_ID_LEN);
	fscanf(stdin, "%s", tmp);
	printf("\n");

	strncpy(buf, tmp, COJO_USER_ID_LEN);
	i = 0;
	while(buf[i] != '\0')
	{
		i ++;
	}
	buf[i] = '\t';
	i ++;
	buf[i] = '#';	// take the space of pwd
	buf[i] = '\t';
	i ++;
	buf[i] = '#';   // take the space of name
	i ++;
	buf[i] = '\t';
	i ++;
	buf[i] = '#';  // take the space of crypt
	i ++;
	buf[i] = '\0';

	

	msg_obj.cojo_con_type = SLTID;
	msg_obj.content = buf;

	write(cli_sockfd, buf, sizeof(msg_obj));

	FD_ZERO(&readfds);
	FD_SET(cli_sockfd, &readfds);

	ret = select(FD_SETSIZE, &readfds, (fd_set *)0,
			(fd_set *)0, (struct timeval *)0);

	if(ret == -1)
	{
		cojo_log("select failed in cojo_client.c cojo_cli_register.\n");
		exit(1);
	}

	read(cli_sockfd, &msg_obj, COJO_MSG_LEN);
	if(buf[0] == 'y')
	{
		pthread_create(&a_thread, NULL, cojo_cli_sltid_write, &cli_sockfd);
		fprintf(stdout, "connect to id %s succeed.\n", tmp);
		fprintf(stdout, "go > ");

		FD_ZERO(&readfds);
		FD_SET(cli_sockfd, &readfds);
		
		b_comn = 1;
		while(b_comn)
		{
			int nread;
			testfds = readfds;

			ret = select(FD_SETSIZE, &testfds, (fd_set *)0,
					(fd_set *)0, (struct timeval *)0);

			if(ret == -1)
			{
				cojo_log("select failed in cojo_client.c cojo_cli_sltid.\n");
				exit(1);
			}

			ioctl(cli_sockfd, FIONREAD, &nread);
			
			if(nread == 0)
			{
				close(cli_sockfd);
				FD_CLR(cli_sockfd, &readfds);
				b_comn = 0;
				exit(0);
			}
			else
			{
				read(cli_sockfd, buf, COJO_MSG_LEN);
				printf("come> %s\n", buf);
			}
		}
	}

} /* cojo_cli_sltid() */


// 在用户和另一用户通信时， 专门用来获取用户输入信息的一线程。
void *
cojo_cli_sltid_write(void *arg)
{
	int sockfd = *(int *)arg;

	char w_buf[COJO_MSG_LEN] = {'\0'};
	
	while(b_comn)
	{

		fprintf(stdout, "go>");
		fscanf(stdin,"%s", w_buf);

		write(sockfd, w_buf, COJO_MSG_LEN);
		memset(w_buf, '\0', COJO_MSG_LEN);
	}
	pthread_exit(NULL);
}/* cojo_cli_sltid_write */

