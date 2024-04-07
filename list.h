/*---------------------------------------------------------------- 
..Project: dict2 and dict 3
  list.h :  
          = the interface of module linkedList of the project.
------------------------------------------------------------------*/


#ifndef _LIST_H_
#define _LIST_H_
#include "data.h"

/*------------ Data definitions ----------------------------------*/
typedef struct node node_t;
struct node {
	place_t *data;          // Points to the data element of the node
	node_t *next;           // Points to the next node in the list
};

struct list {
	node_t *head;          // Points to the first node of the list
	node_t *tail;          // Points to the last node of the list 
	size_t n;              // Number of elements in the list
};
typedef struct list list_t;

/*------------ Function definitions -------------------------------*/
/**
* Creates and returns an empty linked list.
* The following function was adapted from W3.8 of Workshop Week 3.
*/
list_t *listCreate();

/**
* Inserts a new node with place "data" to the end of "list". 
* The following function was adapted from W3.8 of Workshop Week 3.
*/
void listAppend(list_t *list, place_t *data);

/**
* Free the memory allocated to list elements and the list itself. 
*/
void listFree(list_t *list);

/**
* Free only the memory allocated to list structure (nodes) 
* and the list itself. 
*/
void listPartialFree(list_t *list);

#endif
