#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static int send_recv_mesg(int sockfd);

int main(int argc, char *argv[])
{
    int sockfd = 0; 
    int connfd = 0;
    int err_log = 0;
    int ret;
    struct sockaddr_in my_addr; 
    unsigned short port = 23456; 

    printf("TCP Server Started at port %d!\n", port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd < 0)
    {
	perror("socket error");
	exit(-1);
    }
    bzero(&my_addr, sizeof(my_addr)); 
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Binding server to port %d\n", port);
    //connect to server

    if (connect(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr)) == -1)
    	perror("Connected\n");

    //send mesg to server and recv from server
    ret = send_recv_mesg(sockfd);
    if (-1 == ret)
    {
	printf("Send Recv Error\n");
	return -1;
    }

    close(sockfd);
    return 0;
}

/*send mesg and recv mesg*/
static int send_recv_mesg(int sockfd)
{
    int ret;
    char send_mesg[128] = "I am sending mesg to you";
    char recv_mesg[128] = "";

    ret = write(sockfd,send_mesg,strlen(send_mesg));
    if(-1 == ret)
    	return -1;
    printf("The sent mesg is %s\n",send_mesg);

    ret = read(sockfd,recv_mesg,128);
    if(-1 == ret)
    	return -1;
    printf("The recv mesg is %s\n",recv_mesg);

    return 0;
}




