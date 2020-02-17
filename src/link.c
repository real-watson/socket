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
	while(move != NULL)//check the first link whether is null
	{
		printf("ONE_LINKSThe ipaddr: %s and the port is %d\n",move->ipaddr,move->port);
		if (move->next == NULL)
		{
			break;
		}
		move = move->next;//next link
	}
	//right list for links
	while(dmove != NULL)
	{
		printf("RIGHT_DLINKS The ipaddr: %s, and the port is %d, and the id index is %d\n",dmove->ipaddr,dmove->port,dmove->index);
		if (dmove->rnext == NULL)
		{
			break;
		}
		dmove = dmove->rnext;//right list
	}
	//left list for links
	while(dmove != NULL)
	{
		printf("LEFT_DLINKS The ipaddr: %s, and the port is %d, and the id index is %d\n",dmove->ipaddr,dmove->port,dmove->index);
		if (dmove->lnext == NULL)
		{
			break;
		}
		dmove = dmove->lnext;//left list
	}
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
		printf("create the next link\n");
		/*#1 BUG-Used if it returns the only one pointer,while using while, it runs in a list of pointer.*/
		while(move->next != NULL)
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
		/*#1 BUG-Used if it returns the only one pointer,while using while, it runs in a list of pointer.*/
		while(dmove->rnext != NULL)
			dmove = dmove->rnext;
		//move--input-rnext(NULL)
		dmove->rnext = dinput;
		dinput->lnext = dmove;
		dinput->rnext = NULL;//the end of link
	}
}



