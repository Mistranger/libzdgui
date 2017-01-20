#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

typedef struct queueNode_s
{
	struct queueNode_s *next;
	void *data;
} queueNode_t;

typedef struct queue_s
{
	unsigned int count;
	queueNode_t *head, *tail;
} queue_t;

queue_t* queue_init();
void queue_remove(queue_t *queue, bool clear);
unsigned int queue_size(const queue_t *queue);
void queue_push(queue_t *queue, void *data);
void queue_pop(queue_t *queue);
queueNode_t* queue_front(queue_t *queue);
queueNode_t* queue_back(queue_t *queue);


#endif // QUEUE_H_INCLUDED
