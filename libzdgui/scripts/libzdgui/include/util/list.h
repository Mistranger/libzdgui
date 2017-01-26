#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#if DEBUGLEVEL > DEBUG_NONE
#define LIST_CHECK_ITERATOR
#endif

typedef struct listNode_s
{
	struct listNode_s *next, *prev;
	void *data;
} listNode_t;

typedef struct list_s
{
	unsigned int count;
	listNode_t *head, *tail;
} list_t;

list_t* list_new();
void list_delete(list_t *list);
unsigned int list_size(const list_t *list);
void list_push_back(list_t *list, void *data);
void list_push_front(list_t *list, void *data);
void list_pop_back(list_t *list);
void list_pop_front(list_t *list);
#define list_front(list) (list->head)
#define list_back(list) (list->tail)
void list_insert(list_t *list, listNode_t *at, bool before, void *data);
void* list_get(list_t *list, size_t index);
listNode_t* list_find(list_t *list, void *data);
void list_erase(list_t *list, const listNode_t *what);
void list_remove(list_t *list, const void *data);
void list_swap(list_t *list, listNode_t *elem1, listNode_t *elem2);
void list_clear(list_t *list);


#endif // LIST_H_INCLUDED
