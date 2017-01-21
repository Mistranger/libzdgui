#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef struct stackNode_s
{
	struct stackNode_s *next;
	void *data;
} stackNode_t;

typedef struct stack_s
{
	unsigned int count;
	stackNode_t *head;
} stack_t;

stack_t* stack_init();
void stack_push(stack_t* stack, void *element);
void stack_pop(stack_t* stack);
void stack_free(stack_t* stack);
unsigned int  stack_size(stack_t* stack);

#endif // STACK_H_INCLUDED
