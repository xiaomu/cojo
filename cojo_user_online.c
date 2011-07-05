/*
 * author: xiaomu
 * date: 2011/07/03
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cojo_user_online.h"
#include "cojo_log.h"

// add an online user
int
cojo_add_online_user(
		cojo_user_online_t *cojo_user_online_list_head,
		cojo_user_online_t *cojo_user_online_obj
		)
{
	cojo_user_online_t *ptr = NULL;
	ptr = cojo_user_online_list_head;

	if( ptr == NULL)
	{
		cojo_user_online_list_head = cojo_user_online_obj;
		cojo_user_online_list_head -> next = NULL;
	}
	else
	{
		while(ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = cojo_user_online_obj;
		cojo_user_online_obj ->next = NULL;
	}

	return 0;
}/* cojo_add_online_user() */

int
cojo_get_sockfd_byId(
		cojo_user_online_t *cojo_user_online_list_head,
		const char *cojo_user_id
		)
{
	cojo_user_online_t *ptr = NULL;
	int cojo_sockfd = -1;

	ptr = cojo_user_online_list_head;
	while(ptr != NULL)
	{
		if(strncmp(ptr->cojo_user_obj.cojo_user_id, cojo_user_id, 
						COJO_USER_ID_LEN) == 0)
		{
			cojo_sockfd = ptr->cojo_user_sockfd;
			break;
		}
	}

	return cojo_sockfd;
}/* cojo_get_addr_byId() */

// delete an online user
int
cojo_del_online_user(
		cojo_user_online_t *cojo_user_online_list_head,
		const char *cojo_user_id
		)
{
	cojo_user_online_t *ptr = NULL;
	cojo_user_online_t *pre = NULL;
	
	ptr = cojo_user_online_list_head;
	pre = ptr;
	if(ptr == NULL)
		return 0;
	else
	{
		if(strncmp(ptr->cojo_user_obj.cojo_user_id,
					cojo_user_id,
					COJO_USER_ID_LEN
			     ) == 0)
		{
			cojo_user_online_list_head = NULL;
			return 0;
		}

		ptr = ptr->next;
		while(ptr != NULL)
		{
			if(strncmp(ptr->cojo_user_obj.cojo_user_id,
					cojo_user_id,
					COJO_USER_ID_LEN
			     ) == 0)
			{
				pre->next = ptr->next;
				free(ptr);
				return 0;
			}
			ptr = ptr->next;
			pre = pre->next;
		}

	}
	return 0;
}/* cojo_del_online_user() */
				
char *
cojo_get_user_id_online(
		cojo_user_online_t *cojo_user_online_list_head
		)
{
	cojo_user_online_t *ptr;
	int count;
	int i;
	char *id_array = NULL;

	ptr = cojo_user_online_list_head;
	count = 0;
	while(ptr != NULL)
	{
		ptr = ptr->next;
		count ++;
	}

	if(count == 0)
		return NULL;

	id_array = (char *)malloc((COJO_USER_ID_LEN + 1) * sizeof(char));
	if(NULL == id_array)
	{
		cojo_log("malloc failed in cojo_user_online.c cojo_get_user_id_online.\n");
		return NULL;
	}

	memset(id_array, '\0', (COJO_USER_ID_LEN +1) *sizeof(char));

	ptr = cojo_user_online_list_head;
	for(i =0; i<count; i++)	
	{
		strncpy(&id_array[i*(COJO_USER_ID_LEN + 1)], ptr->cojo_user_obj.cojo_user_id,
					COJO_USER_ID_LEN);
		ptr = ptr->next;
	}
	
	return id_array;
}
