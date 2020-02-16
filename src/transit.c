#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include "common.h"
#include <sys/types.h>

unsigned int img_index = 0;

int init_lock(FILE *file, int type)
{
    struct flock lock;
    int fd = 0;

    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_type = type;
    lock.l_pid = -1;

    if (file == NULL)
    	return -1;

    //FILE *file to fd
    fd=fileno(file);
    if (-1 == -1)
    	return -1;

    //set lock
    if ((fcntl(fd,F_SETLKW,&lock)) < 0)
    	return -1;
    return 0;
}

int recv_video_from_client(unsigned int connfd)
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
        fwrite(buff,len,sizeof(char),video);		
    
    //unlock file
    if (-1 == init_lock(video,F_UNLCK))
    	return -1;

    if (len < 0)
    	return -1;

    fclose(video);
    close(connfd);
    return 0;
}

int recv_image_from_client(int connfd)
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
        fwrite(buff,len,sizeof(char),img);		
    }
    fclose(img);
    close(connfd);
    return 0;
}

void *client_process(void *arg)
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

