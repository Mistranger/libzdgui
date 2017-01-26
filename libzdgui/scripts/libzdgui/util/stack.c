#include "system.h"

#include "util/stack.h"

stack_t* stack_init()
{
	stack_t *new_stack = new(stack_t);
	new_stack->count = 0;
    new_stack->head = NULL;
	return new_stack;
}

void stack_push(stack_t* stack, void* element)
{
	if (!stack) {
		return;
	}
	stackNode_t *temp = new(stackNode_t);
	temp->data = element;
	if (!stack->head) {
		temp->next = NULL;
		stack->head = temp;
	} else {
		temp->next = stack->head;
		stack->head = temp;
	}
	++stack->count;
}

void stack_pop(stack_t* stack)
{
	if (!stack || !stack_size(stack)) {
		return;
	}
	stackNode_t *temp;
	temp = stack->head;
	stack->head = stack->head->next;
	--stack->count;
	free(temp);
}

void stack_free(stackNode_t* stack)
{
	// FIXME rewrite
	stackNode_t *temp;
	while (stack->next) {
		temp = stack->next;
		free(stack);
		stack = temp;
	}
	free(stack);
}

unsigned int stack_size(stack_t* stack)
{
	if (!stack) {
		return 0;
	}
	return stack->count;
}
