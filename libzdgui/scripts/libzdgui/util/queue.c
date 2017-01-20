#include "libzdgui.h"

#include "util/queue.h"

queue_t* queue_init()
{
	queue_t *new_queue = (queue_t*)malloc(sizeof(queue_t));
	new_queue->count = 0;
    new_queue->head = NULL;
    new_queue->tail = NULL;
	return new_queue;
}

void queue_remove(queue_t* queue, bool clear)
{
	//FIXME
}

unsigned int queue_size(const queue_t* queue)
{
	if (!queue) {
		return 0;
	}
	return queue->count;
}

void queue_push(queue_t* queue, void* data)
{
	queueNode_t *elem = (queueNode_t*)malloc(sizeof(queueNode_t));
	elem->data = data;
	elem->next = NULL;
	if (!queue->head) {
		queue->head = queue->tail = elem;
	} else {
		queue->tail->next = elem;
		queue->tail = elem;
	}
}

void queue_pop(queue_t* queue)
{
	queueNode_t *elem = queue->head;
	queue->head = queue->head->next;
	if (elem->data) {
		free(elem->data);
	}
	if (elem) {
		free(elem);
	}
}

queueNode_t* queue_back(queue_t* queue)
{
	return queue->tail;
}

queueNode_t* queue_front(queue_t* queue)
{
	return queue->head;
}
