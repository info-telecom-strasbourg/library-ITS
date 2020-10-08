#include "stack.h"
#include <stdlib.h>
#include <string.h>

int
create_stack(stack_t *stack, size_t node_memsize)
{
    if (node_memsize == 0)
        return -1;
    stack->node_memsize = node_memsize;
    stack->tail = NULL;
    return 0;
}

int
push_stack(stack_t *stack, const void *data)
{
    stack_node_t *new_node = (stack_node_t *)malloc(sizeof(stack_node_t));

    if (new_node == NULL)
        return -1;

    new_node->data = malloc(stack->node_memsize);

    if (new_node->data == NULL)
    {
        free(new_node);
        return -1;
    }

    memcpy(new_node->data, data, stack->node_memsize);

    if (stack->tail == NULL)
    {
        new_node->prev = NULL;
        stack->tail = new_node;
    }
    else
    {
        new_node->prev = stack->tail;
        stack->tail = new_node;
    }

    return 0;
}

int
pop_stack(stack_t *stack, void *data)
{
    if (stack->tail == NULL)
        return -1;

    stack_node_t *temp = stack->tail;
    memcpy(data, temp->data, stack->node_memsize);

    stack->tail = stack->tail->prev;

    free(temp->data);
    free(temp);
    return 0;
}

int
stack_peek(const stack_t *stack, void *data)
{
    if (stack->tail == NULL)
        return -1;

    stack_node_t *temp = stack->tail;
    memcpy(data, temp->data, stack->node_memsize);
    return 0;
}

void
delete_stack(stack_t *stack)
{
    stack_node_t *temp;

    while (stack->tail != NULL)
    {
        temp = stack->tail;
        stack->tail = stack->tail->prev;
        free(temp->data);
        free(temp);
    }
}

int
stack_is_empty(const stack_t *stack)
{
    return stack->tail == NULL;
}