#include <string.h>

/* Author: xiaomu
 * date: 2011/6/25
 *
 * ReverseStr is used to reverse a string.
 * param *str is the source  string.
 */
void ReverseStr(char *str)
{
	char ch;
	int len = strlen(str);
	int start, end;

	for(start = 0, end = len-1; end > start; start++, end--)
	{
		ch = str[end];
		str[end] = str[start];
		str[start] = ch;
	}
}


