#include <stdio.h>
int tcp_socket_init(unsigned short port, const char *ip);

int main(void)
{
	unsigned short port = 1234;
	char ip[32] = "120.0.0.1";
	printf("The res is %d\n",tcp_socket_init(port,ip));
	return 0;
}
