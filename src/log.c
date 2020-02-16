#include <stdio.h>
#include <stdlib.h>

#define LOG_STORE(string,ret) (log_store_to_file(string,__FILE__,__func__,__LINE__))

int log_store_to_file(char *string, const char *filepath, const char *function, const int line)
{
	char tmplog[256] = "";
	char *ptr = NULL;
	ptr = (char*)malloc(sizeof(char)*256);
	ptr = tmplog;
	if (ptr == NULL)
		return -1;
	else
		sprintf(ptr,"echo string:[%s] filepath:[%s] function:[%s] line:[%d] >> ../logfile/store.log",string,filepath,function,line);
	if (-1 == system(ptr))
		return -1;
	return 0;
}


