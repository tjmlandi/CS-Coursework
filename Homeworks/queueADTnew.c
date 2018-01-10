#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void free(void *ptr);
void *malloc(size_t size);
typedef struct node node;

struct node
{
	void* data;
	node* next;
} ;


#ifndef _QUEUE_IMPL_
typedef struct
{
	int (*cmp)(const void*a, const void*b);
	node* root;
} *QueueADT;
#endif



QueueADT que_create( int (*cmp)(const void*a, const void*b))
{
	printf("create called\n");
	QueueADT newQueue = malloc(sizeof(QueueADT));
	newQueue->cmp = cmp;
	newQueue->root = NULL;
	printf("queue created");
	return newQueue;
}

void que_destroy( QueueADT queue )
{
	printf("destroy called\n");
	node* current;
	node* next;
	current = queue->root;
	next = queue->root;
	while (current != NULL)
	{
		next = current->next;
		free(current->data);
		current->data = NULL;
		free(current);
		current = next;
	}
	free(queue->cmp);
	queue->cmp = NULL;
	free(queue);
	queue = NULL;
}

void que_clear( QueueADT queue )
{
	printf("clear called\n");
	node* next;
	node* current;
	next = queue->root;
	current = queue->root;
	while (current != NULL)
	{
		next = current->next;
		free(current->data);
		current->data = NULL;
		free(current);
		current = next;
	}
}

void que_insert( QueueADT queue, void *data )
{
	printf("going to insert\n");
	node* previous = queue->root;
	node* current = queue->root;
	if (queue->cmp == NULL)
	{
		while (current != NULL)
		{
			current = current->next;
			printf("no comparison function increment\n");
		}
		current = malloc(sizeof(node));
		current->next = NULL;
		current->data = data;
		printf("no comparison node added\n");
	}
	else
	{
		if (queue->root == NULL)
		{
			queue->root = malloc(sizeof(node));
			queue->root->data = data;
			queue->root->next = NULL;
			printf("first node added\n");
		}
		else 
		{
			while (current != NULL)
			{
				while ((queue->cmp(data, current->data)) > 0)
				{
					previous = current;
					current = current->next;
					printf("comparison function increment\n");
				}
			}
			if (current == NULL)
			{
				node* toInsert = malloc(sizeof(node));
				previous->next = toInsert;
				toInsert->data = data;
				toInsert->next = NULL;
				printf("last node added\n");
			}
			else
			{
				node* toInsert = malloc(sizeof(node));
				previous->next = toInsert;
				toInsert->next = current;
				toInsert->data = data;
				printf("middle node added\n");
			}
		}	
	}
}

void *que_remove( QueueADT queue )
{
	printf("removal called\n");
	if (queue->root != NULL)
	{
		void* temp = queue->root->data;
		node* newRoot = queue->root->next;
		free(queue->root);
		queue->root = newRoot;
		return temp;
	}
	else
	{
		fputs("ERROR", stderr);
		exit(EXIT_FAILURE);
	}
	
}

bool que_empty( QueueADT queue )
{
	printf("empty called\n");
	if (queue->root == NULL)
	{
		printf("queue is empty\n");
		return true;
	}
	else 
	{
		printf("queue is not empty\n");
		return false;
	}
}
