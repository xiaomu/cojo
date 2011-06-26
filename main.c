#include <stdio.h>
#include "code.h"

int main()
{
	char str[] = "hello world.";
	printf("before reverse: %s\n", str);
	ReverseStr(str);
	printf("After reverse: %s\n", str);

	return 0;
}
