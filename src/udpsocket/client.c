#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct sockaddr* saddrp;
typedef struct mesg
{
	int head;
	char text[64];
}MESG;

#define PORT 23456
#define IPADDR "127.0.0.1"
#define ACKLEN4 4
#define VERSION "v1.02"

void init_version()
{
	char cmd[64] = "";
	sprintf(cmd,"echo %s @ `date` >> version/log ",VERSION);
	system(cmd);
}

/*
* interface for udp send with ack
* when sending something to server 
* the interface should be set common one
* need to enhence this one
*/

int udpack_send(int sockfd,struct sockaddr_in addr,char *mesg)
{
	int i = 0;
	int ret = -1;
	int len = 0;
	char *buff = NULL;

	len = strlen(mesg);
	socklen_t addr_len = sizeof(struct sockaddr_in);

	buff = (char*)malloc(sizeof(char*)*(len+ACKLEN4));//_ack

	if (buff == NULL)
		return -1;

	if (mesg != NULL)
	{
		ret = sendto(sockfd,mesg,len,0,(saddrp)&addr,sizeof(addr));
		
		if (ret == 0)
			return -1;

		/*read server's ack*/
		ret = recvfrom(sockfd,buff,len+ACKLEN4,0,(saddrp)&addr,&addr_len);
		if (ret == 0)
			return -1;

		if (strstr(buff,"_ack") != NULL)
			printf("the recv buff is %s\n",buff);
		else
			return -1;

		free(buff);
	}

	buff = NULL;
	return 0;
}

int main(int argc, char const *argv[])
{
	int sockfd = -1;
	int ret = 0;
	char mesg[32] = "helloworld";
	int i;
	char pack[128] = "";
	init_version();

	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (0 > sockfd)
	{
		perror("socket");
		return -1;
	}
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IPADDR);
	
	MESG msg;

	for(i=0;i<100;i++)
	{
		msg.head = i;	
		strcpy(msg.text,mesg);
		/*
		*	head|data
		*	i   |mesg
		*/
		sprintf(pack,"%d_%s%d",msg.head,msg.text,msg.head);	
		ret = udpack_send(sockfd,addr,pack);
		memset(pack,0,128);
	}
	close(sockfd);
	return 0;
}
