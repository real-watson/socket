#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
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
	log_store_to_file("hello","loglog",__func__,__LINE__);
    /*Init the pthread.*/
    pthread_t thread_id;
    pthread_mutex_init(&mutex, NULL); 

    sockfd = init_socket_server();
	//init the links
	LINKS *head = NULL;
	LINKS *input = NULL;
	DLINKS *dhead = NULL;
	DLINKS *dinput = NULL;

    while(1){
		char cli_ip[INET_ADDRSTRLEN] = "";
		struct sockaddr_in client_addr; 
		socklen_t cliaddr_len = sizeof(client_addr); 
		//set lock for each accept-> connfd_pthread

		pthread_mutex_lock(&mutex);

		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
		if(connfd < 0){
			perror("accept this time");
			continue;
		}
		/*
		*	create links for one direction and the double create links for double directions
		*	storage about ipaddr,ip port and id index of client
		*	print all of the links from left to right
		*/
		create_links(&head,input,inet_ntoa(client_addr.sin_addr),client_addr.sin_port);
		double_create_links(&dhead,dinput,inet_ntoa(client_addr.sin_addr),client_addr.sin_port,++id_index);
		print_links(head,dhead);

		//store the id,address,port,mesg  in database(mysql)
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);

		//cope with each clients from 
		if(connfd > 0){
			pthread_create(&thread_id, NULL, (void *)client_process, (void *)&connfd);
			pthread_detach(thread_id);/*release the resourse of pthread*/
		}
    }

    close(sockfd);
    return 0;
}
