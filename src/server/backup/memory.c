//keep these functions
//init the dynamic memeory
void get_memeory(char **ptr,int m)
{
    *ptr = (char*)malloc(sizeof(char)*m);//alloc the memory and give the memory to ptr(string)
}

int set_memeory(unsigned char *ipaddr,int m)
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
int init_random_n()
{
    unsigned int n = 0;
    while(n = random()*100)
		break;
    return n;
}

//init the dynamic buff
int init_dynamic_buff(unsigned int m, unsigned char *ipaddr)
{
    char *buff = NULL;

    if( m == 0 || ipaddr == NULL)
    	return -1;

    //char *buff = "";
    buff = (char*)malloc((sizeof(char)*m));
    //memset(buff,0,m);
    mymemnset(buff,m);

    strncpy(buff,ipaddr,m);
    printf("The buff is %s\n",buff); 
    return 0;
}
//create mesg queen for ipaddr storage
int msg_queen_ipaddr(unsigned char *ipaddr, char *path, unsigned int id)
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
int init_mysql_database(unsigned int id, unsigned char *ipaddr, unsigned int port, char *mesg)
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


