#include "sll.h"
#include <stdlib.h>
#include <stdbool.h>
	
slinked_list_t* 
init_slist_t()
{
	slinked_list_t *tmp = NULL;
	tmp = alloc_list_t();
	
	if( tmp )
	{
		tmp->head = NULL;
		tmp->tail = NULL;
		tmp->size = 0;
	}

	return tmp;
}



bool 
slinked_list_append(slinked_list_t * list, slinked_node_t * node)
{
	bool res = false;
	if( !list || !node) return res;

	/*
	 * Could be also checking for 
	 * head and tail being NULL
	 */
	if( list->size == 0 )
	{
		list->head = list->tail = node;
	} else 
	{
		list->tail->next = node;
		list->tail = node;
	}

	res = true;
	list->size++;
	
	return res;


}
bool 
slinked_list_prepend(slinked_list_t * list, slinked_node_t * node)
{
	bool res = false;
	if( !list || !node) return res;

	/*
	 * Could be also checking for 
	 * head and tail being NULL
	 */
	if( list->size == 0 )
	{
		list->head = list->tail = node;
	} else 
	{
		node->next = list->head;
		list->head = node;
	}

	res = true;
	list->size++;
	
	return res;
}


static slinked_list_t*
alloc_list_t()
{
	return (slinked_list_t *)(malloc(sizeof(slinked_list_t)));
}

