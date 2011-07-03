/*
 * author: xiaomu
 * date: 2011/07/03
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>

#include "cojo_server.h"
#include "cojo_addr.h"

void
cojo_init_server(void)
{
	// init server's sock address
	cojo_server->cojo_server_addr.sin_family = AF_INET;
	cojo_server->cojo_server_addr.sin_port = 9798;
	cojo_server->cojo_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// init server's online user 
	cojo_server->cojo_user_online_num = 0;
	cojo_server->cojo_user_online_list = NULL;

}/* cojo_init_server() */

int
cojo_set_addr(void)
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
		inet_ntop(AF_INET, ptr->cojo_server_addr, addr_buf, INET_ADDRSTRLEN);
		addr_buf[INET_ADDRSTRLEN] = '\0';

		fprintf(stdout, "%d--- %s\n", i, addr_buf);

		prt = ptr->next;
	}
	fprintf("input your choice: ");
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
	fscanf(stdin, "%d", cojo_port);

	cojo_server->cojo_server_addr.sin_addr = ptr->cojo_sin_addr;
	cojo_server->cojo_server_addr.sin_port = cojo_port;

	return 0;
}/* cojo_set_addr() */ 

void
cojo_server_work(void)
{
	int cojo_server_sockfd;
	int *cojo_client_sockfd;
	struct sockaddr_in cojo_server_addr;
	struct sockaddr_in cojo_client_addr;
	int cojo_server_len, cojo_client_len;
	fd_set readfds, testfds;
	int ret;
	pthread_t a_thread;

	cojo_server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	cojo_server_addr.sin_family = cojo_server.cojo_server_addr.sin_family;
	cojo_server_addr.sin_addr = cojo_server.cojo_server_addr.sin_addr;
	cojo_server_addr.sin_port =htons( cojo_server.cojo_server_addr.sin_port );
	cojo_server_len = sizeof(cojo_server_addr);

	bind(cojo_server_sockfd, (struct sockaddr *)&cojo_server_addr, cojo_server_len);
	listen(cojo_server_sockfd, 10);

	FD_ZERO(&readfds);
	FD_SET(cojo_server_sockfd, &readfds);

	while(1)
	{

		testfds = readfds;
		ret = select(FD_SETSIZE, &testfds, (fd_set *)0,
				(fd_set *)0, (struct timeval *)0);

		if(ret < 1)
		{
			cojo_log("select failed.\n");
			exit(1);
		}

		if(cojo_server.cojo_user_online_number <= 9)
		{

			cojo_client_len = sizeof(cojo_client_addr);
			cojo_client_sockfd = accept(cojo_server_sockfd, 
					(struct sockaddr *)&cojo_client_addr,
					&cojo_client_len);

			cojo_server.cojo_user_online_num ++;

			ret = pthread_create(&a_thread, NULL, cojo_handle_con ,
					(void *)cojo_client_sockfd);
			if(ret != 0)
			{
				cojo_log("pthread_create failed in cojo_server.c 
						cojo_server_word.\n");
			}
		}
	}
}/* cojo_server_work() */

void *
cojo_handle_con(void *arg)
{
	int cojo_sockfd = *(int *)arg;
	int cojo_con_sockfd;

	fd_set readfds, testfds;
	
	int ret, ret2;
	char buf[COJO_MSG_LEN];
	cojo_msg_t cojo_msg_obj;
	cojo_msg_t cojo_msg_back;

	int bool_login; 

	cojo_msg_ojb.content =(char *)malloc(COJO_MSG_LEN *sizeof(char));
	if(cojo_msg_obj.content == NULL)
	{
		cojo_log("malloc failed in cojo_server.c cojo_handle_con.\n");
		exit(1);
	}

	FD_ZERO(&readfds);
	FD_SET(cojo_sockfd, &readfds);
	while(1)
	{
		testfds = readfds;
		ret = select(FD_SETSIZE, &testfds, (fd_set *)0,
				(fd_set *)0, (struct timeval *)0);

		if(ret == -1)
		{
			cojo_log("select failed in cojo_server.c cojo_handle_con.\n");
			exit(1);
		}

		read(cojo_sockfd, &cojo_msg_obj, COJO_MSG_LEN + 4); // '4' for connect type

		if(cojo_msg_obj.cojo_con_type == REGISTER)
		{
			ret = cojo_do_register(&cojo_msg_obj);
			cojo_msg_back.content = (char *)malloc(2*sizeof(char));
			if(ret == 0)
			{
				strncpy(cojo_msg_back.content, "y", 2);
			}
			else
			{
				strncpy(cojo_msg_back.content, "n", 2);
			}
			cojo_msg_back.cojo_con_type = REGISTER;
			ret2 = write(cojo_sockfd, &cojo_msg_back,
					sizeof(cojo_msg_back));
			if(ret2 == -1)
			{
				cojo_log("write failed in cojo_server.c
						cojo_handle_con.\n");
				exit(1);
			}
		}
		else if(cojo_msg_obj.cojo_con_type == LOGIN)
		{
			ret = cojo_do_login(&cojo_msg_obj);
			cojo_msg_back.content = (char *)malloc(2*sizeof(char));
			if(ret == 0)
			{
				strncpy(cojo_msg_back.content, "y", 2);
			}
			else
			{
				strncpy(cojo_msg_back.content, "n", 2);
			}
			cojo_msg_back.cojo_con_type = LOGIN;
			ret2 = write(cojo_sockfd, &cojo_msg_back,
					sizeof(cojo_msg_back));
			if(ret2 == -1)
			{
				cojo_log("write failed in cojo_server.c
						cojo_handle_con.\n");
				exit(1);
			}
		}
		else if(cojo_msg_obj.cojo_con_type == SLTID)
		{
			cojo_con_sockfd = cojo_do_sltid(&cojo_msg_obj);
			cojo_msg_back.content = (char *)malloc(2*sizeof(char));
			if(cojo_con_sockfd != -1)				
			{
				strncpy(cojo_msg_back.content, "y", 2);
			}
			else
			{
				strncpy(cojo_msg_back.content, "n", 2);
			}
			cojo_msg_back.cojo_con_type = SLTID;
			ret2 = write(cojo_sockfd, &cojo_msg_back,
					sizeof(cojo_msg_back));
			if(ret2 == -1)
			{
				cojo_log("write failed in cojo_server.c
						cojo_handle_con.\n");
				exit(1);
			}

			if(cojo_con_sockfd != -1)
			{
				cojo_comu(cojo_sockfd, cojo_con_sockfd);
			}
		}
		else if(cojo_msg_obj.cojo_con_type == QUIT)
		{
			close(cojo_sockfd);
		}
	}

	pthread_exit(NULL);
}/* cojo_handle_con() */


// if the msg is for register, register the user.
// 0 is returned when succeed, else -1 is returned.
int
cojo_do_register(cojo_msg_t *cojo_msg_obj)
{
	cojo_user_t *user_obj;
	int ret;

	user_obj = cojo_get_user_from_lnspa(cojo_msg_obj->content);
	if(user_obj == NULL)
	{
		cojo_log("cojo_get_user_from_lnspa failed in cojo_server.c
				cojo_do_register() .\n");
		return -1;
	}

	ret = cojo_add_user(user_obj);

	if(ret == -1)
	{
		cojo_log("cojo_add_user failed in cojo_server.c cojo_do_register.\n");
	}

	return ret;
}/* cojo_do_register() */


// login a user.
// 0 is returned when succed, else -1 is returned.
int
cojo_do_login(cojo_msg_t *cojo_msg_obj)
{
	cojo_user_t *user_obj = NULL;
	cojo_user_t *user_out = NULL;
	int ret;

	user_obj = cojo_get_user_from_lnspa(cojo_msg_obj->content);
	if(user_obj == NULL)
	{
		cojo_log("cojo_get_user_lnspa failed in cojo_server.c
				cojo_do_login.\n");
		return -1;
	}

	user_out = cojo_get_user_byId(user_obj->cojo_user_id);
	if(user_out == NULL)
	{
		cojo_log("cojo_get_user_byId failed in cojo_server.c
				cojo_do_login.\n");
		return -1;
	}

	if(user_out->cojo_user_pwd != user_obj->cojo_user_pwd)
	{
		cojo_log("pwd wrong in cojo_do_login\n");
		return -1;
	}

	return 0;
}/* cojo_do_login() */


	




