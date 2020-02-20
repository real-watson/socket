#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*save way for copying string from others' modification*/
int mystrncpy(const char *src, char *dst, unsigned int n)
{
	char c;
	if (!n)
		return -1;
	/*
	*	--n ->strlen(src) + 1
	*	n-- ->strlen(src)
	*	check the last c is '\0' of src
	*/
	while(n--)
	{
		//if ((*dst++ = *src++) == '\0')
		c = *src++;
		if(!c)
			break;
		*dst = c;
	}
	*dst = 0;

	return 0;
}

/*copy way from memset*/
void mymemnset(char *string, unsigned int n)
{
	if (!n)
		return;
	while(n--)
	{
		*string++ = '\0';
	}
}
