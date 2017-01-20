#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef struct stackNode_s
{
	struct stackNode_s *next;
	void *data;
} stackNode_t;

void stack_init(stackNode_t* stack);
void stack_push(stackNode_t* stack, void *element);
void stack_pop(stackNode_t* stack);
void stack_free(stackNode_t* stack);

#endif // STACK_H_INCLUDED
