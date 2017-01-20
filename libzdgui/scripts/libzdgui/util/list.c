#include "libzdgui.h"

#include "util/list.h"

/**
 * @brief      Initializes the list (constructor)
 * @return     new list
 */
list_t* list_init()
{
	list_t *new_list = (list_t*)malloc(sizeof(list_t));
	new_list->count = 0;
    new_list->head = NULL;
    new_list->tail = NULL;
	return new_list;
}
/**
 * @brief          Removes list from memory
 * @param list     List to remove
 * @param clear    Should we clear elements
 */
void list_remove(list_t* list, bool clear)
{
	if (!list) {
		return;
	}
	
	listNode_t *it = list->head;
	listNode_t *it2;
	while (it) {
		if (clear) {
			free(it->data);
		}
		it2 = it->next;
		free(it);
		it = it2;
	}
	free(list);
	list = NULL;
}

unsigned int list_size(const list_t* list)
{
	if (!list) {
		return 0;
	}
	return list->count;
}

void list_push_back(list_t* list, void* data)
{
	listNode_t *elem = (listNode_t*)malloc(sizeof(listNode_t));
	elem->data = data;
	elem->next = NULL;
	elem->prev = list->tail;
	list->tail->next = elem;
	list->tail = elem;
}

void list_push_front(list_t* list, void* data)
{
	listNode_t *elem = (listNode_t*)malloc(sizeof(listNode_t));
	elem->data = data;
	elem->prev = NULL;
	elem->next = list->head;
	list->head->prev = elem;
	list->head = elem;
}

void list_pop_front(list_t* list)
{
	listNode_t *elem = list->head;
	list->head = list->head->next;
	list->head->prev = NULL;
	if (elem->data) {
		free(elem->data);
	}
	if (elem) {
		free(elem);
	}
}

void list_pop_back(list_t* list)
{
	listNode_t *elem = list->tail;
	list->tail = list->tail->prev;
	list->tail->next = NULL;
	if (elem->data) {
		free(elem->data);
	}
	if (elem) {
		free(elem);
	}
}

void list_insert(list_t* list, listNode_t* at, bool before, void* data)
{
	if (!list || !at || !data) {
		return;
	}
	listNode_t *it = list->head;
	listNode_t *head = list->head;
	listNode_t *tail = list->tail;
	
	while (it) {
		if (it == at) {
			listNode_t *new_elem = (listNode_t*)malloc(sizeof(listNode_t));
			new_elem->data = data;
			if (before) {
				if (it->prev) {
					listNode_t *elem = it->prev;
					elem->next = new_elem;
					new_elem->next = it;
					new_elem->prev = elem;
					it->prev = new_elem;
				} else {
					new_elem->prev = NULL;
					new_elem->next = it;
					it->prev = new_elem;
					// assert
					list->head = new_elem;
				}
			} else {
				if (it->next) {
					listNode_t *elem = it->next;
					elem->prev = new_elem;
					new_elem->prev = it;
					new_elem->next = elem;
					it->next = new_elem;
				} else {
					new_elem->next = NULL;
					new_elem->prev = it;
					it->next = new_elem;
					
					list->tail = new_elem;
				}
			}
		}
		it = it->next;
	}
}
