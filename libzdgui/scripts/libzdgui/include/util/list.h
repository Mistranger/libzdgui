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

list_t* list_init();
void list_remove(list_t *list, bool clear);
unsigned int list_size(const list_t *list);
void list_push_back(list_t *list, void *data);
void list_push_front(list_t *list, void *data);
void list_pop_back(list_t *list);
void list_pop_front(list_t *list);
void list_insert(list_t *list, listNode_t *at, bool before, void *data);
void list_erase(list_t *list, const listNode_t *what);
void list_swap(list_t *list, listNode_t *elem1, listNode_t *elem2);


#endif // LIST_H_INCLUDED