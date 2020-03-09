#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct sockaddr* saddrp;
#define PORT 23456

void udpack_recv(int sockfd, struct sockaddr_in addr,int i)
{ 
	struct sockaddr_in src_addr;
	char buff[256] = "";
	int ret;
	char *token = NULL;
	char check[8] = "";

	socklen_t addr_len = sizeof(struct sockaddr_in);

	ret = recvfrom(sockfd,buff,sizeof(buff),0,(saddrp)&src_addr,&addr_len);

	token = strtok(buff,"_");
	sprintf(check,"%d",i);

	if (ret && token)
	{
		if (strcmp(check,token) == 0) /*check the order of mesg*/
		{
			/*send ack to client for response*/
			strcat(buff,"_ack");
			sendto(sockfd,buff,strlen(buff)+1,0,(saddrp)&src_addr,sizeof(addr));
			printf("send is %s\n",buff);
		}
		else
		{
			printf("Find the data loss\n");
			return;
		}
	}
}

int main(int argc, char const *argv[])
{
	int sockfd;
	int ret;
	int i;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (0 > sockfd)
	{
		perror("sockfd");
		return -1;
	}

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(sockfd,(saddrp)&addr,sizeof(addr));
	if (0 > ret)
	{
		perror("bind");
		return -1;
	}
	//while(1)//just read from client
	for (i=0; i<100; i++)	
		udpack_recv(sockfd,addr,i);

	close(sockfd);
	return 0;
}
