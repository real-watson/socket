#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*print all links*/
void print_links(LINKS *head,DLINKS *dhead)
{
	LINKS *move = NULL;
	DLINKS *dmove = NULL;
	move = head;
	dmove = dhead;
	//point to head
	if (move->next != NULL)
		while(move != NULL)//check the first link whether is null
		{
			printf("The ipaddr: %s and the port is %d\n",move->ipaddr,move->port);
			move = move->next;//next link
		}
	else
		printf("The ipaddr: %s and the port is %d\n",move->ipaddr,move->port);

	if (dmove->rnext != NULL)
		while(dmove != NULL)
		{
			printf("The ipaddr: %s, and the port is %d, and the id index is %d\n",dmove->ipaddr,dmove->port,dmove->index);
			dmove = dmove->rnext;//right list
		}
	else
		printf("The ipaddr: %s, and the port is %d, and the id index is %d\n",dmove->ipaddr,dmove->port,dmove->index);
}

/*add links for restoring ipaddr and port*/
void create_links(LINKS **head, LINKS *input)
{
	LINKS *move = *head;
	if (*head == NULL)//first link(head)
	{
		*head = input;
		input->next = NULL;
	}
	else//after first link
	{
		if(move->next != NULL)
			move = move->next;
		move->next = input;
		input->next = NULL;//the end of link
	}
}

void double_create_links(DLINKS **dhead, DLINKS *dinput)
{
	DLINKS *dmove = *dhead;

	/*lnext--dhead--rnext*/
	if (*dhead == NULL)//first link(head)
	{
		*dhead = dinput;
		//left and right should be null
		dinput->lnext = NULL;
		dinput->rnext = NULL;
	}
	else//after first link
	{
		if(dmove->rnext != NULL)
			dmove = dmove->rnext;
		//move--input-rnext(NULL)
		dmove->rnext = dinput;
		dinput->lnext = dmove;
		dinput->rnext = NULL;//the end of link
	}
}



