#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>          /* See NOTES */
#include <netinet/tcp.h> 

/*Add long connection for client to server*/
void long_connection_set(int sockfd)
{
	int ret;
	int keepalive = 1;
	int keepvalue = 60;
	int keepdetec = 5;
	int keepcount = 5;

	ret = setsockopt(sockfd,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepalive,sizeof(keepalive));
	if (-1 == ret)
		return;
	ret = setsockopt(sockfd,SOL_TCP,TCP_KEEPIDLE,(void*)&keepvalue,sizeof(keepvalue));
	if (-1 == ret)
		return;

	ret = setsockopt(sockfd,SOL_TCP,TCP_KEEPINTVL,(void*)&keepdetec,sizeof(keepdetec));
	if (-1 == ret)
		return;

	ret = setsockopt(sockfd,SOL_TCP,TCP_KEEPCNT,(void*)&keepcount,sizeof(keepcount));
	if (-1 == ret)
		return;
}
