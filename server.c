#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#define BUFF_SIZE_12K 1024*12
#define PATH "video.mp4"
unsigned int img_index = 0;

pthread_mutex_t mutex;//pthread lock 

static int init_lock(FILE *file, int type)
{
    struct flock lock;
    int fd = 0;

    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    //lock.l_type = type;
    lock.l_pid = -1;

    //to fd
    fd = fileno(file);

    //get lock status
    fcntl(fd,F_GETLK,&lock);
    if (lock.l_type == F_RDLCK || lock.l_type == F_WRLCK)
    	return -1;
    
    //set lock
    lock.l_type = type;
    if ((fcntl(fd,F_SETLKW,&lock)) < 0)
    	return -1;
}

static int recv_video_from_client(unsigned int connfd)
{
    FILE *video = NULL;
    unsigned char buff[BUFF_SIZE_12K] = {0};
    unsigned int len = 0;

    video = fopen(PATH,"wb");
    if (NULL == video)
	return -1;
    //lock file avoid others writting file
    if (-1 == init_lock(video,F_WRLCK))
    	return -1;
    //read video from server
    while((len = recv(connfd,buff,BUFF_SIZE_12K,0)) > 0)
        fwrite(buff,len,1,video);		
    
    if (len < 0)
    	return -1;

    fclose(video);
    close(connfd);
    return 0;
}

static int recv_image_from_client(int connfd)
{
    img_index++;
    FILE *img = NULL;
    int len = 0;
    char buff[BUFF_SIZE_12K] = {0};
    char index_jpg[12] = "";

    sprintf(index_jpg,"img/no_%d.jpg",img_index);
    img = fopen(index_jpg,"wb");
    if (NULL == img)
    {
    	printf("Error in fopen\n");
    	return -1;
    }
    //read from client and write to img
    while((len = recv(connfd,buff,sizeof(buff),0)) > 0)
    {
    	printf("The len is %d\n",len);
        fwrite(buff,len,1,img);		
    }
    fclose(img);
    close(connfd);
    return 0;
}

static void *client_process(void *arg)
{
    int connfd = *(int *)arg; 
    /*Unlock the pthread.*/
    pthread_mutex_unlock(&mutex);
    printf("receive image from client: ip:port:mesg:flag:id\n");
    //recv_image_from_client(connfd);
    recv_video_from_client(connfd);

    close(connfd); 
    return NULL;
}

static int init_socket_server()
{
    int err_log = 0;
    int sockfd;
    unsigned short port = 23456; 
    struct sockaddr_in my_addr; 


    printf("TCP port at server is %d\n", port);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd < 0)
    {
	perror("socket error");
	return -1;
    }
    /*Init the addr struct.*/
    bzero(&my_addr, sizeof(my_addr)); 
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Binding server to port %d\n", port);
   
    err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if(err_log != 0)
    {
    	perror("bind");
	close(sockfd);//it needs closing
	return -1;
    }
    err_log = listen(sockfd, 10);
    if( err_log != 0)
    {
	perror("listen");
	close(sockfd);
	return -1;
    }
    printf("Waiting client...\n");
    return sockfd;
}
//===============================================================
int main(int argc, char **argv)
{
    int sockfd = 0;
    int connfd = 0;
    pthread_t thread_id;
    /*Init the pthread.*/
    pthread_mutex_init(&mutex, NULL); 

    sockfd = init_socket_server();

    while(1)
    {
	char cli_ip[INET_ADDRSTRLEN] = "";
 	struct sockaddr_in client_addr; 
	socklen_t cliaddr_len = sizeof(client_addr); 
	//set lock for each accept-> connfd_pthread
	pthread_mutex_lock(&mutex);
	connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
	if(connfd < 0)
	{
	    perror("accept this time");
	    continue;
	}

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
