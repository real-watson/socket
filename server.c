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

#define BUFF_SIZE_12K 1024*12
#define BUFF_TEXT_1K 1024
#define PATH "video.mp4"
#define PATH_MESG "."
//jpg 
//#define PATH "girl.jpg"
unsigned int img_index = 0;
unsigned int id_index = 200;


pthread_mutex_t mutex;//pthread lock 


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



/*print all links*/
static void print_links(LINKS *head,DLINKS *dhead)
{
	LINKS *move = NULL;
	DLINKS *dmove = NULL;

	move = head;
	dmove = dhead;

	//point to head
	if (move->next != NULL)
	{
		while(move != NULL)//check the first link whether is null
		{
			printf("The ipaddr: %s and the port is %d\n",move->ipaddr,move->port);
			move = move->next;//next link
		}
	}
	else
	{
		printf("The ipaddr: %s and the port is %d\n",move->ipaddr,move->port);
	}

	if (dmove->rnext != NULL)
	{
		while(dmove != NULL)
		{
			printf("The ipaddr: %s, and the port is %d, and the id index is %d\n",dmove->ipaddr,dmove->port,dmove->index);
			dmove = dmove->rnext;//right list
		}
	}
	else
	{
		printf("The ipaddr: %s, and the port is %d, and the id index is %d\n",dmove->ipaddr,dmove->port,dmove->index);
		
	}
}

/*add links for restoring ipaddr and port*/
static void create_links(LINKS **head, LINKS *input)
{
	LINKS *move = *head;
	if (*head == NULL)//first link(head)
	{
		printf("hellow\n");
		*head = input;
		input->next = NULL;
	}
	else//after first link
	{
		if(move->next != NULL)
		{
			move = move->next;
		}
		move->next = input;
		input->next = NULL;//the end of link
	}
}

static void double_create_links(DLINKS **head, DLINKS *input)
{
	DLINKS *move = *head;

	/*lnext--head--rnext*/
	if (*head == NULL)//first link(head)
	{
		*head = input;
		//left and right should be null
		input->lnext = NULL;
		input->rnext = NULL;
	}
	else//after first link
	{
		if(move->rnext != NULL)
		{
			move = move->rnext;
		}
		//move--input-rnext(NULL)
		move->rnext = input;
		input->lnext = move;
		input->rnext = NULL;//the end of link
	}
}


//init the dynamic memeory
static void get_memeory(char **ptr,int m)
{
    *ptr = (char*)malloc(sizeof(char)*m);//alloc the memory and give the memory to ptr(string)
}

static int set_memeory(unsigned char *ipaddr,int m)
{
    char *string = NULL; 

    if (ipaddr == NULL)
    	return -1;

    get_memeory(&string,m);//just get the address from string(ptr)
    strcpy(string,ipaddr);//string means *ptr
    printf("The string is %s,while the length of it is %d\n",string,(int)strlen(string));
    free (string);

    return 0;
}

//init random number
static int init_random_n()
{
    unsigned int n = 0;
    while(n = random()*100)
	break;
    return n;
}

//init the dynamic buff
static int init_dynamic_buff(unsigned int m, unsigned char *ipaddr)
{
    char *buff = NULL;

    if( m == 0 || ipaddr == NULL)
    	return -1;

    //char *buff = "";
    buff = (char*)malloc((sizeof(char)*m));
    memset(buff,0,m);

    strncpy(buff,ipaddr,m);
    printf("The buff is %s\n",buff); 
    return 0;
}

//create mesg queen for ipaddr storage
static int msg_queen_ipaddr(unsigned char *ipaddr, char *path, unsigned int id)
{
    typedef struct message
    {
        long msg_type;
	char msg_text[BUFF_TEXT_1K];
    }MESSAGE;
    int qid;
    key_t key;
    MESSAGE msg;

    if (path == NULL)
    	return -1;
    //different path and key
    if ((key = ftok(path,id)) == -1)
	return -1;
    //create mseg queen
    if ((qid = msgget(key,IPC_CREAT|0666)) == -1)
    	return -1;
    
    if (NULL == ipaddr)
    	return -1;
    
    strcpy(msg.msg_text,ipaddr);

    msg.msg_type = getpid();
    //add to mesg queen
    if ((msgsnd(qid,&msg,strlen(msg.msg_text),0)) < 0)
    	return -1;
    printf("The mesg is %s\n",msg.msg_text);
	
    return 0;
}

//init database
static int init_mysql_database(unsigned int id, unsigned char *ipaddr, unsigned int port, char *mesg)
{
    MYSQL mysql_conn;
    mysql_init(&mysql_conn);
    char database[] = "ipaddr";
    char query[256] = "";
   
    if (ipaddr == NULL || mesg == NULL)
    	return -1;

    //connected to mysql 
    if (!mysql_real_connect(&mysql_conn, "localhost", "root", NULL, database, 0, NULL, 0)) 
        return -1;	

    //insert data 
    sprintf(query,"INSERT INTO address(id, addr,port,mesg) VALUES(%d,'%s',%d,'%s')",id,ipaddr,port,mesg);
    if(mysql_query(&mysql_conn,query) != 0)
	return -1;

    mysql_close(&mysql_conn);
    return 0; 
}

static int init_lock(FILE *file, int type)
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
    if (fd=fileno(file) == -1)
    	return -1;

    //set lock
    if ((fcntl(fd,F_SETLKW,&lock)) < 0)
    	return -1;
    return 0;
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
        fwrite(buff,len,sizeof(char),img);		
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
    int sock_type = -1;
    socklen_t len = -1;
    
    unsigned short port = 23456; 
    struct sockaddr_in my_addr; 


    printf("TCP port at server is %d\n", port);
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd < 0)
    {
	perror("socket error");
	return -1;
    }
    //check the type of socket
    len = sizeof(sock_type);
    err_log = getsockopt(sockfd,SOL_SOCKET,SO_TYPE,&sock_type,&len);
    if (err_log == -1)
    {
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
		init_dynamic_buff(24,inet_ntoa(client_addr.sin_addr));
		msg_queen_ipaddr(inet_ntoa(client_addr.sin_addr),PATH_MESG,init_random_n());
		init_mysql_database(id_index,inet_ntoa(client_addr.sin_addr),client_addr.sin_port,"connected");

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
