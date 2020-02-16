#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <mysql.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "common.h"



//===============================================================
int main(int argc, char **argv)
{
	init_version();

    int sockfd = 0;
    int connfd = 0;
	unsigned int id_index = 0;

    /*Init the pthread.*/
    pthread_t thread_id;
    pthread_mutex_init(&mutex, NULL); 

    sockfd = init_socket_server();

	//init the links
	LINKS *head = NULL;
	LINKS *input = NULL;
	DLINKS *dhead = NULL;
	DLINKS *dinput = NULL;

    while(1)
    {
		char cli_ip[INET_ADDRSTRLEN] = "";
		struct sockaddr_in client_addr; 
		socklen_t cliaddr_len = sizeof(client_addr); 
		//set lock for each accept-> connfd_pthread
		pthread_mutex_lock(&mutex);
		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
		id_index++;
	
		//init link
		input = (LINKS *)malloc(sizeof(LINKS));
		dinput = (DLINKS *)malloc(sizeof(DLINKS));

		strcpy(input->ipaddr,inet_ntoa(client_addr.sin_addr));
		input->port = client_addr.sin_port;

		strcpy(dinput->ipaddr,inet_ntoa(client_addr.sin_addr));
		dinput->port = client_addr.sin_port;
		dinput->index = id_index;

		create_links(&head,input);
		double_create_links(&dhead,dinput);
		print_links(head,dhead);//print all links

		if(connfd < 0)
		{
			perror("accept this time");
			continue;
		}
		//store the id,address,port,mesg  in database(mysql)
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
		printf("client ip=%s,port=%d\n", cli_ip,ntohs(client_addr.sin_port));
		//cope with each clients from 
		if(connfd > 0)
		{
			pthread_create(&thread_id, NULL, (void *)client_process, (void *)&connfd);
			pthread_detach(thread_id); 
		}
    }

    close(sockfd);
    return 0;
}
