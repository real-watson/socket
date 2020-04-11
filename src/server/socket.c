#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

/*init the socket for server*/
int init_socket_server()
{
    int err_log = 0;
    int sockfd;
    int sock_type = -1;
    socklen_t len = -1;
    
    unsigned short port = 23456; 
    struct sockaddr_in my_addr; 

    printf("TCP port at server is %d\n", port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd < 0){
		perror("socket error");
		return -1;
    }
    //check the type of socket
    len = sizeof(sock_type);
    err_log = getsockopt(sockfd,SOL_SOCKET,SO_TYPE,&sock_type,&len);
    if (err_log == -1){
		close(sockfd);
		return -1;
    }
    printf("The sock type is %d\n",sock_type);
    /*Init the addr struct.*/
    bzero(&my_addr, sizeof(my_addr)); 
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Binding server to port %d\n", port);
   
    err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0){
    	perror("bind");
		close(sockfd);//it needs closing
		return -1;
    }
    err_log = listen(sockfd, 10);
    if( err_log != 0){
		perror("listen");
		close(sockfd);
		return -1;
    }
    printf("Waiting client...\n");
	return sockfd;
}
