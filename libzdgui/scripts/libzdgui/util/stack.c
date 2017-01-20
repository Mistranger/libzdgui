#include "libzdgui.h"

#include "util/stack.h"

void stack_init(stackNode_t* stack)
{
	stack->next = stack->data = NULL;
}

void stack_push(stackNode_t* stack, void* element)
{
	stackNode_t *temp;
	temp = (stackNode_t*)malloc(sizeof(stackNode_t));
	temp->data = element;
	temp->next = stack;
	stack = temp;
}

void stack_pop(stackNode_t* stack)
{
	stackNode_t *temp;
	if (stack == NULL) {
		
	} else {
		temp = stack;
		stack = stack->next;
		free(temp);
	}
}

void stack_free(stackNode_t* stack)
{
	stackNode_t *temp;
	while (stack->next) {
		temp = stack->next;
		free(stack);
		stack = temp;
	}
	free(stack);
	stack = NULL;
}
