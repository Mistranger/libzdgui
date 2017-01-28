#include "system.h"

#include "util/list.h"

/**
 * @brief      Initializes the list (constructor)
 * @return     new list
 */
list_t *list_new()
{
	list_t *new_list = new (list_t);
	new_list->count = 0;
	new_list->head = NULL;
	new_list->tail = NULL;
	return new_list;
}

void list_delete(list_t *list)
{
	list_clear(list);
	free(list);
}

void list_remove(list_t *list, const void *data)
{
	listNode_t *it;
	for (it = list->head; it; it = it->next) {
		if (it->data == data) {
			list_erase(list, it);
		}
	}
}

unsigned int list_size(const list_t *list)
{
	if (!list) {
		return 0;
	}
	return list->count;
}

void list_push_back(list_t *list, void *data)
{
	listNode_t *elem = new (listNode_t);
	elem->data = data;
	elem->next = NULL;
	if (!list->tail) { // list is empty
		elem->prev = NULL;
		list->tail = list->head = elem;
	} else {
		elem->prev = list->tail;
		list->tail->next = elem;
		list->tail = elem;
	}
	++list->count;
}

void list_push_front(list_t *list, void *data)
{
	listNode_t *elem = new (listNode_t);
	elem->data = data;
	elem->prev = NULL;
	if (!list->head) { // list is empty
		elem->next = NULL;
		list->tail = list->head = elem;
	} else {
		elem->next = list->head;
		list->head->prev = elem;
		list->head = elem;
	}
	++list->count;
}

void list_pop_front(list_t *list)
{
	listNode_t *elem = list->head;
	list->head = list->head->next;
	list->head->prev = NULL;
	--list->count;
	if (!list->count) {
		list->head = list->tail = NULL;
	}/*
	if (elem->data) {
		free(elem->data);
	}*/
	free(elem);
}

void list_pop_back(list_t *list)
{
	listNode_t *elem = list->tail;
	list->tail = list->tail->prev;
	list->tail->next = NULL;
	--list->count;
	if (!list->count) {
		list->head = list->tail = NULL;
	}/*
	if (elem->data) {
		free(elem->data);
	}*/
	free(elem);
}

void list_insert(list_t *list, listNode_t *at, bool before, void *data)
{
	if (!list || !at || !data) {
		return;
	}
	listNode_t *it = list->head;
	++list->count;
	while (it) {
		if (it == at) {
			listNode_t *new_elem = new (listNode_t);
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

void *list_get(list_t *list, size_t index)
{
	listNode_t *node;
	for (node = list_front(list); index-- && node; node = node->next) {
	}
	return node;
}

listNode_t *list_find(list_t *list, void *data)
{
	for (listNode_t *node = list->head; node; node = node->next) {
		if (node->data == data) {
			return node;
		}
	}
	return NULL;
}

void list_erase(list_t *list, const listNode_t *what)
{
	if (!list->count) {
		return;
	}
	for (listNode_t *node = list->head; node; node = node->next) {
		if (node == what) {
			if (node == list->head) {
				list->head = list->head->next;
				list->head->prev = NULL;

			} else if (node == list->tail) {
				list->tail = list->tail->prev;
				list->tail->next = NULL;

			} else {
				if (node->prev) {
					node->prev->next = node->next;
				}
				if (node->next) {
					node->next->prev = node->prev;
				}
			}

			free(node);
			--list->count;
			if (!list->count) {
				list->head = list->tail = NULL;
			}
			return;
		}
	}
}

void list_swap(list_t *list, listNode_t *elem1, listNode_t *elem2)
{
	void *temp = elem1->data;
	elem1->data = elem2->data;
	elem2->data = temp;
}

void list_clear(list_t *list)
{
	listNode_t *node = list->head;
	listNode_t *delete;
	while (node) {
		delete = node;
		node = node->next;/*
		if (delete->data) {
			free(delete->data);
		}*/
		free(delete);
	}
	list->count = 0;
}
