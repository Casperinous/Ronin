#ifndef SLINKEDLIST_H_
#define SLINKEDLIST_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct slinked_node_t
{
	int identifier;
	void *data;
	struct slinked_node_t * next;
} slinked_node_t;


typedef struct slinked_list_t
{
	slinked_node_t * head;
	slinked_node_t * tail;
	size_t size;
	bool use_alloc;

} slinked_list_t;





slinked_list_t* 
init_slist_t();

bool 
slinked_list_append(slinked_list_t * list, slinked_node_t * node);


bool 
slinked_list_prepend(slinked_list_t * list, slinked_node_t * node);


static slinked_list_t*
alloc_list_t();

#endif





