#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int tcp_socket_init(unsigned short port)
{
	int sockfd = -1; 
	struct sockaddr_in my_addr; 

	printf("TCP Server Started at port %d!\n", port);
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 

	if(sockfd < 0){
		perror("socket error");
		exit(-1);
	}

	bzero(&my_addr, sizeof(my_addr)); 
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//connect to server
	if (connect(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr)) == -1){
		perror("Connected\n");
		return -1;
	}

	return sockfd;
}

