#include "system.h"

#include "util/queue.h"

queue_t* queue_init()
{
	queue_t *new_queue = new(queue_t);
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
	queueNode_t *elem = new(queueNode_t);
	elem->data = data;
	elem->next = NULL;
	++queue->count;
	if (!queue->head) {
		queue->head = queue->tail = elem;
	} else {
		queue->tail->next = elem;
		queue->tail = elem;
	}
}

void queue_pop(queue_t* queue)
{
	if (queue->count > 0) {
		queueNode_t *elem = queue->head;
		queue->head = queue->head->next;
		--queue->count;
		if (elem->data != NULL) {
			free(elem->data);
		}
		if (elem != NULL) {
			free(elem);
		}
	}
}

void* queue_back(queue_t* queue)
{
	return queue->tail->data;
}

void* queue_front(queue_t* queue)
{
	return queue->head->data;
}
