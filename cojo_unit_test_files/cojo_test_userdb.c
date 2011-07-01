#include "cojo_userdb.h"
#include "cojo_log.h"
#include <stdio.h>
#include "cojo_test_userdb.h"
#include <stdlib.h>

int main()
{
	int choice;

	char *menu[] = {
		"1---get by id.\n",
		"2---del by id.\n",
		"3---add user.\n",
		"4---alter user.\n",
		"0---quit.\n",
		NULL
	};

	show_menu(menu);

	printf("please choice: ");
	scanf("%d", &choice);

	while(1)
	{
		switch(choice)
		{
			case 1:
				test_get();
				break;
			case 2:
				test_del();
				break;
			case 3:
				test_add();
				break;
			case 4:
				test_alter();
				break;
			case 0:
				exit(0);
			default:
				break;
		}
		printf("---------------------------------\n");
		printf("please choice: ");
		scanf("%d", &choice);
	}
	
	return 0;
}

void show_menu(char **menu)
{
	char **p = menu;

	while(*p != NULL)
	{
		printf("%s", *p);
		*p ++;
	}
}

void test_get(void)
{
	char cojo_user_id[COJO_USER_ID_LEN + 1];
	cojo_user_t *user_a;

	printf("input the user's id you want get.\n");
	scanf("%s", cojo_user_id);
	// get by id
	
	user_a = cojo_get_user_byId(cojo_user_id);
	if(user_a == NULL)
	{
		fprintf(stderr, "cojo_get_user_byId failed.\n");
		exit(1);
	}
	printf("get id: %s\t", user_a->cojo_user_id);
	printf("get pwd: %s\t", user_a->cojo_user_pwd);
	printf("get name: %s\n", user_a->cojo_user_name);

	
}

void test_del(void)
{
	char cojo_user_id[COJO_USER_ID_LEN + 1];
	int ret;

	printf("input the user's id you want del.\n");
	scanf("%s", cojo_user_id);
	ret = cojo_del_user_byId(cojo_user_id);
	if(ret == -1)
		printf("del failed.\n");
}

void test_add(void)
{
	cojo_user_t user;
	int ret;

	printf("add's id: ");
	scanf("%s", user.cojo_user_id);
	printf("add's pwd: ");
	scanf("%s", user.cojo_user_pwd);
	printf("add's name: ");
	scanf("%s", user.cojo_user_name);

	ret = cojo_add_user( &user);
	if(ret == -1)
		printf("add failed\n");
}


void test_alter(void)
{
	cojo_user_t user;
	int ret;
	printf("alter's id: ");
	scanf("%s", user.cojo_user_id);
	printf("alter's new pwd: ");
	scanf("%s", user.cojo_user_pwd);
	printf("alter's new name: ");
	scanf("%s", user.cojo_user_name);

	ret = cojo_alter_user( &user);
	if(ret == -1)
		printf("alter failed\n");

}

