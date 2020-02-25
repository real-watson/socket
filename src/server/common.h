#ifndef __INIT_H__

#include <stdio.h>
#include <pthread.h>
#define BUFF_SIZE_12K 1024*12
#define BUFF_TEXT_1K 1024
#define PATH "video.mp4"
#define PATH_MESG "."
#define VERSION "v4.01"
#define IP_ADDR_SIZE 64
#define LOG_STORE(string) (log_store_to_file(string,__FILE__,__func__,__LINE__))
pthread_mutex_t mutex;//pthread lock 


//single link
typedef struct link
{
	char ipaddr[IP_ADDR_SIZE];
	unsigned int port;
	struct link *next;
}LINKS;

//double link
typedef struct dlink
{
	unsigned int index;	
	unsigned int port;
	char ipaddr[IP_ADDR_SIZE];
	struct dlink *lnext;
	struct dlink *rnext;
}DLINKS;

extern int init_socket_server();
extern void print_links(LINKS *head,DLINKS *dhead);
extern void create_links(LINKS **head, LINKS *input, char *ipaddr, unsigned int port);
extern void double_create_links(DLINKS **dhead, DLINKS *dinput, char *ipaddr, unsigned int port, unsigned int index);
extern void init_version();
extern int recv_video_from_client(unsigned int connfd);
extern int init_lock(FILE *file, int type);
extern void *client_process(void *arg);
extern int recv_image_from_client(int connfd);
extern int log_store_to_file(char *string, char *filepath, const char *function, const int line);
extern void mystrncpy(const char *src, char *dst, unsigned int n);
extern void mymemnset(const char *src, unsigned int n);
extern void long_connection_set(int sockfd);
extern int mystrlen(const char *str);

#define __INIT_H__
#endif
