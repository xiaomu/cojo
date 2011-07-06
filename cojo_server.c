/*
 * author: xiaomu
 * date: 2011/07/03
 */

#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "cojo_server.h"
#include "cojo_addr.h"
#include "cojo_log.h"

void
cojo_init_server(void)
{
	// init server's sock address
	cojo_server.cojo_server_addr.sin_family = AF_INET;
	cojo_server.cojo_server_addr.sin_port = 9798;
	cojo_server.cojo_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// init server's online user 
	cojo_server.cojo_user_online_num = 0;
	cojo_server.cojo_user_online_list = NULL;

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

	cojo_server.cojo_server_addr.sin_addr = ptr->cojo_sin_addr;
	cojo_server.cojo_server_addr.sin_port = cojo_port;

	return 0;
}/* cojo_set_addr() */ 

void
cojo_server_work(void)
{
	int cojo_server_sockfd;
	int cojo_client_sockfd;
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

		if(cojo_server.cojo_user_online_num <= 9)
		{

			cojo_client_len = sizeof(cojo_client_addr);
			cojo_client_sockfd = accept(cojo_server_sockfd, 
					(struct sockaddr *)&cojo_client_addr,
					(socklen_t *)&cojo_client_len);

			fprintf(stdout, "create cli_sockfd in server: %d\n",
				       	cojo_client_sockfd);

			ret = pthread_create(&a_thread, NULL, cojo_handle_con ,
					(void *)&cojo_client_sockfd);
			if(ret != 0)
			{
				cojo_log("pthread_create failed in cojo_server.c \
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

	fd_set readfds;
	
	int ret, ret2;
	cojo_msg_t cojo_msg_obj;
	cojo_msg_t cojo_msg_back;

	int bool_login = 0; 
	cojo_user_t *user_obj;
	cojo_user_online_t user_online_obj;

	char *buf = cojo_msg_obj.content;

#if 0
	cojo_msg_obj.content =(char *)malloc(COJO_MSG_LEN *sizeof(char));
	if(cojo_msg_obj.content == NULL)
	{
		cojo_log("malloc failed in cojo_server.c cojo_handle_con.\n");
		exit(1);
	}
#endif

	FD_ZERO(&readfds);
	FD_SET(cojo_sockfd, &readfds);
	while(1)
	{
#if 0
		while(1)
		{
			FD_ZERO(&readfds);
			FD_SET(cojo_sockfd, &readfds);

			s_time.tv_sec = 3;
			s_time.tv_usec = 500000;
	
			ret = select(cojo_sockfd + 1, &readfds, (fd_set *)0,
				(fd_set *)0, &s_time);

			if(ret == -1)
			{
				cojo_log("select failed in cojo_server.c cojo_handle_con.\n");
				exit(1);
			}
			else if(ret != 0)
			{
				break;
			}
			fprintf(stdout, "select cli_sockfd: %d\n", cojo_sockfd);
		}
#endif

		//read(cojo_sockfd, &cojo_msg_obj, COJO_MSG_LEN + 4); // '4' for connect type
		read(cojo_sockfd, &cojo_msg_obj, 4 + COJO_MSG_LEN);
		fprintf(stdout, "recv: %s\n", cojo_msg_obj.content);

		if(cojo_msg_obj.cojo_con_type == REGISTER)
		{
			fprintf(stdout, "here register.\n");
			ret = cojo_do_register(&cojo_msg_obj);
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
					4 + strlen(cojo_msg_back.content));
			if(ret2 == -1)
			{
				cojo_log("write failed in cojo_server.c\
						cojo_handle_con.\n");
				exit(1);
			}
		}
		else if(cojo_msg_obj.cojo_con_type == LOGIN)
		{
			ret = cojo_do_login(&cojo_msg_obj);
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
					4 + strlen(cojo_msg_back.content));
			if(ret2 == -1)
			{
				cojo_log("write failed in cojo_server.c\
						cojo_handle_con.\n");
				exit(1);
			}

			bool_login = 1;

			user_obj = cojo_get_user_from_lnspa(cojo_msg_obj.content);
			memcpy(&(user_online_obj.cojo_user_obj), &user_obj, sizeof(user_obj));
			user_online_obj.cojo_user_sockfd = cojo_sockfd;
			user_online_obj.cojo_bool_comn = 0;

			ret2 = cojo_add_online_user(cojo_server.cojo_user_online_list,
					&user_online_obj);

			if(ret == -1)
			{
				cojo_log("cojo_add_online_user failed in cojo_server.c\
						cojo_handle_con().\n");
				exit(1);
			}
				

			
		}
		else if(cojo_msg_obj.cojo_con_type == SLTID)
		{
			cojo_con_sockfd = cojo_do_sltid(&cojo_msg_obj);
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
					4 + strlen(cojo_msg_back.content));
			if(ret2 == -1)
			{
				cojo_log("write failed in cojo_server.c\
						cojo_handle_con.\n");
				exit(1);
			}

			if(cojo_con_sockfd != -1)
			{
				cojo_comn(cojo_sockfd, cojo_con_sockfd);
			}
		}
		else if(cojo_msg_obj.cojo_con_type == QUIT)
		{
			close(cojo_sockfd);
			cojo_del_userol_byfd(cojo_sockfd);
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
		cojo_log("cojo_get_user_from_lnspa failed in cojo_server.c\
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

	printf("in login: %s\n", cojo_msg_obj->content);
	user_obj = cojo_get_user_from_lnspa(cojo_msg_obj->content);
	if(user_obj == NULL)
	{
		cojo_log("cojo_get_user_lnspa failed in cojo_server.c\
				cojo_do_login.\n");
		return -1;
	}

	user_out = cojo_get_user_byId(user_obj->cojo_user_id);
	if(user_out == NULL)
	{
		cojo_log("cojo_get_user_byId failed in cojo_server.c\
				cojo_do_login.\n");
		return -1;
	}

	printf("out: %s\n", user_out->cojo_user_pwd);
	if(strcmp(user_out->cojo_user_pwd, user_obj->cojo_user_pwd) != 0)
	{
		cojo_log("pwd wrong in cojo_do_login\n");
		return -1;
	}

	return 0;
}/* cojo_do_login() */


// get the sockfd of an online user
int
cojo_do_sltid(cojo_msg_t *cojo_msg_obj)
{
	int i, j;
	char cojo_user_id[COJO_USER_ID_LEN];
	int sockfd;

	i = 0;
	j = i;
	while((cojo_msg_obj->content[j] != '\t')&&(cojo_msg_obj->content[j] != '\n')
			&&(cojo_msg_obj->content[j] != '\0')
		       	&&(j < COJO_USER_TOTAL_LEN))
	{
		j++;
	}

	strncpy(cojo_user_id, &(cojo_msg_obj->content[i]), j-i);

	sockfd = cojo_get_sockfd_byId(cojo_server.cojo_user_online_list,
			cojo_user_id);

	return sockfd;
}/* cojo_do_sltid() */
			

// commucation between two sock fds
void
cojo_comn(int cojo_sockfd, int cojo_con_sockfd)
{
	char msg[COJO_MSG_LEN] = {'\0'};
	fd_set readfds, testfds;
	int ret;
	int fd;
	int nread;
		
	FD_ZERO(&readfds);
	FD_SET(cojo_sockfd, &readfds);
	FD_SET(cojo_con_sockfd, &readfds);

	while(1)
	{
		testfds = readfds;

		ret = select(FD_SETSIZE, &testfds, (fd_set *)0,
				(fd_set *)0, (struct timeval *)0);
		if(ret == -1)
		{
			cojo_log("selct failed in cojo_server.c cojo_comn.\n");
			exit(1);
		}

		for(fd = 0; fd < FD_SETSIZE; fd++)
		{
			if (FD_ISSET(fd, &testfds))
			{
				if(fd == cojo_sockfd)
				{
					ioctl(fd, FIONREAD, &nread);
					if(nread == 0)
					{
						close(fd);
						FD_CLR(fd, &readfds);
						FD_CLR(cojo_con_sockfd, &readfds);
						cojo_del_userol_byfd(fd);
						cojo_rel_userol_byfd(cojo_con_sockfd);
					}
					else
					{
						read(fd, msg, COJO_MSG_LEN);
						write(cojo_con_sockfd, msg, strlen(msg));
					}
				}
				else
				{
					ioctl(fd, FIONREAD, &nread);
					if(nread == 0)
					{
						close(fd);
						FD_CLR(fd, &readfds);
						FD_CLR(cojo_sockfd, &readfds);
						cojo_del_userol_byfd(fd);
						cojo_rel_userol_byfd(cojo_sockfd);
					}
					else
					{
						read(fd, msg, COJO_MSG_LEN);
						write(cojo_sockfd, msg, strlen(msg));
					}
				}
			}
		}

	}
} /* cojo_comn() */

int
cojo_del_userol_byfd(int fd)
{
	cojo_user_online_t *ptr = NULL;
	cojo_user_online_t *pos = NULL;

	ptr = cojo_server.cojo_user_online_list;
	if(ptr == NULL)
	{
		cojo_log("user ol list in null, error occured.\n");
		return -1;
	}

	pos = ptr;

	if(ptr->cojo_user_sockfd == fd)
	{
		cojo_server.cojo_user_online_list = ptr->next;
		free(ptr);
	}
	else
	{
		while(ptr != NULL)
		{
			pos = ptr;
			ptr = ptr->next;

			if(ptr->cojo_user_sockfd == fd)
			{
				pos->next = ptr->next;
				free(ptr);
			 }
		}
	}

	return 0;
} /* cojo_del_userol_byfd() */


// release a user by fd
int
cojo_rel_userol_byfd(int fd)
{
	cojo_user_online_t *ptr;

	ptr = cojo_server.cojo_user_online_list;
	if(ptr == NULL)
	{
		cojo_log("user ol list in null, error occured.\n");
		return -1;
	}

	while(ptr != NULL)
	{
		if(ptr->cojo_user_sockfd == fd)
		{
			ptr->cojo_bool_comn = 0;
			break;
		}
		ptr = ptr->next;
	}

	return 0;
}/* cojo_rel_user_byfd() */


