#include <stdio.h>
int tcp_socket_init(unsigned short port);

int main(void)
{
	unsigned short port = 1234;
	printf("The res is %d\n",tcp_socket_init(port));
	return 0;
}
