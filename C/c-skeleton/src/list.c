#include "list.h"


List *
List_create()
{
	return calloc(1, sizeof(List));
}


void
List_destroy(List *list)
{
	LIST_FOREACH(list, first, next, cur){
		if(cur->prev) {
			free(cur->prev);
			cur->prev=NULL;
		}
	}

	if(list->last){
		free(list->last);
		list->last = NULL;
	}

	free(list);
}


void
List_clear(List *list)
{
	LIST_FOREACH(list, first, next, cur){
		if(cur->value){
			free(cur->value);
		}
	}
}


void
List_clear_destroy(List *list)
{
	List_clear(list);
	List_destroy(list);
}


void
List_push(List *list, void *value)
{
	ListNode *node = calloc(1,sizeof(ListNode));
	node->value = value;

	if(list->last == NULL){
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;

	error:
		return;
}


void *
List_pop(List *list)
{
	ListNode *node = list->last;
	
	if(node == NULL){
		return NULL;
	}

	return List_remove(list, node);
}


void *
List_remove(List *list, ListNode *node)
{
	void *result = NULL;
		
	if(node == list->first && node == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else if(node == list->first) {
		list->first = node->next;
		node->next->prev = NULL;
	} else if(node == list->last) {
		list->last = node->prev;
		node->prev->next = NULL;
	} else {
		ListNode *before = node->prev;
		ListNode *after = node->next;
		
		before->next = after;
		after->prev = before;
	}

	list->count--;
	result = node->value;
	free(node);
	node = NULL;
	
	return result;
}

