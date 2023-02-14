#ifndef LIST_H
#define LIST_H
#include <stddef.h>
#include <stdlib.h>
struct list {
	struct listentry* head;
	struct listentry* tail;
};

struct listentry {
	void *data;
	struct listentry *prev;
	struct listentry *next;
};

void list_rm(struct list* l, struct listentry* e)  // Remove an item from the list
// free from memory and attach prev to the next item and vice-versa
{
	if (e->prev != NULL) e->prev->next = e->next;
	if (e->next != NULL) e->next->prev = e->prev;
	if (l->head == e) l->head = e->next;
	if (l->tail == e) l->tail = e->prev;
	free(e);}
struct listentry* list_add(struct list* l, void* data){  // Add an item to the list
	struct listentry* e = (struct listentry*)malloc(sizeof(struct listentry));
	e->data = data;
	e->prev = l->tail;
	e->next = NULL;
	if (l->tail != NULL) l->tail->next = e;
	l->tail = e;
	if (l->head == NULL) l->head = e;
	return e;
} 
struct list* list_new()  // Return an initialized list
{
	struct list* l = (struct list*)malloc(sizeof(struct list));
	l->head = NULL;
	l->tail = NULL;
	return l;

}
int is_empty(struct list* l){  // Check if list is empty or not
if (l->head == NULL ) return 1;
else return 0;
}
#endif
