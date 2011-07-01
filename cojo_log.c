/*
 * author: xiaomu
 * date: 2011/06/30
 */

#include <stdio.h>
#include "cojo_log.h"

/*
void cojo_log(const char *format, ...)
{
	fprintf(stderr, format, ...);
} 
*/

void cojo_log(const char *str)
{
	fprintf(stderr, str);
}
