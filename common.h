#ifndef __INIT_H__
#include <stdio.h>
#include <pthread.h>
#define BUFF_SIZE_12K 1024*12
#define BUFF_TEXT_1K 1024
#define PATH "video.mp4"
#define PATH_MESG "."
#define VERSION "2.01"

pthread_mutex_t mutex;//pthread lock 

/*two c files include these variables*/
static unsigned int img_index = 0;
static unsigned int id_index = 200;

typedef struct ip
{
	char ipaddr[32];
	unsigned int port;
	struct ip *next;
}LINKS;

typedef struct dlink
{
	unsigned int index;	
	unsigned int port;
	char ipaddr[32];
	struct dlink *lnext;
	struct dlink *rnext;
}DLINKS;

extern int init_socket_server();

extern void print_links(LINKS *head,DLINKS *dhead);
extern void create_links(LINKS **head, LINKS *input);

extern void double_create_links(DLINKS **dhead, DLINKS *dinput);
extern void init_version();
extern int recv_video_from_client(unsigned int connfd);
extern int init_lock(FILE *file, int type);
extern void *client_process(void *arg);
extern int recv_image_from_client(int connfd);
#define __INIT_H__


#endif
