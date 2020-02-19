#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int restart_server()
{
	if(-1 == (system("killall server")))
		return -1;
	if(-1 == (system("bash server &")))
		return -1;
	return 0;
}

void check_internet_error()
{
	char *url = "www.baidu.com";
	struct hostent *internet;
	internet = gethostbyname(url);
	if (!internet)
	{
		if(!restart_server());
			printf("error\n");
	}
}
