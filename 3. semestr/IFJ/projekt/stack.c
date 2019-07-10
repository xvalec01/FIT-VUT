
#ifndef STACK_C
#define STACK_C

#include "stack.h"

void stack_create(stack* s_stack)
{
    s_stack->top = NULL;
}


bool stack_push(stack* s_stack, int typ, int data_typ)
{
    stack_item* new_item = (stack_item*)malloc(sizeof(stack_item));
    if (new_item == NULL)
        return false;

    new_item->typ = typ;
    new_item->data_typ = data_typ; // slouží pro kontrolu typů
    new_item->next = s_stack->top;
    s_stack->top = new_item;
    return true;
}


bool stack_pop(stack* s_stack)
{
    if (s_stack->top != NULL)
    {
        stack_item* tmp = s_stack->top;
        s_stack->top = tmp->next;
        free(tmp);
        return true;
    }
    return false;
}


void stack_pop_count(stack* s_stack, int count)
{
    for (int i = 0; i < count; i++)
    {
        stack_pop(s_stack);
    }
}


stack_item* stack_top_terminal(stack* s_stack)
{
    for (stack_item* tmp = s_stack->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->typ < STOP)
            return tmp;
    }
    return NULL;
}

int count_of_symbols_to_reduce(bool* founded, stack* s_stack)
{
    stack_item* tmp = stack_top(s_stack);
    int count = 0;

    while (tmp != NULL)
    {
        if (tmp->typ != STOP)
        {
            *founded = false;
            count++;
        }
        else
        {
            *founded = true;
            break;
        }

        tmp = tmp->next;
    }

    return count;
}

bool stack_insert_after_top_terminal(stack* s_stack, int typ, int data_typ)
{
    stack_item* prev = NULL;

    for (stack_item* tmp = s_stack->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->typ < STOP)
        {
            stack_item* new_item = (stack_item*)malloc(sizeof(stack_item));
            if (new_item == NULL)
                return false;

            new_item->typ = typ;
            new_item->data_typ = data_typ;

            if (prev == NULL)
            {
                new_item->next = s_stack->top;
                s_stack->top = new_item;
            }
            else
            {
                new_item->next = prev->next;
                prev->next = new_item;
            }
            return true;
        }
        prev = tmp;
    }
    return false;
}


stack_item* stack_top(stack* s_stack)
{
    return s_stack->top;
}


void stack_free(stack* s_stack)
{
    while (stack_pop(s_stack));
}

/*int main() 
{ 
    struct s_stack* s_stack = createStack(10); 
  
    push(s_stack, 10); 
    push(s_stack, 20); 
    push(s_stack, 30);
    printf("%d Hodnota\n", pop(s_stack)); 
  
    return 0; 
} */
#endif