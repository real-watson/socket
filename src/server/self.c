#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*save way for copying string from others' modification*/
/*If the src could not avoid null, so it should be check*/
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
	while(n--){
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
		*string++ = '\0';
}

/*create a func about strlen*/
int mystrlen(const char *str)
{
	int n = 0;
	while(*str++ != '\0')
		n++;
	return n;
}
