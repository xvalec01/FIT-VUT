#ifndef STACK_H
#define STACK_H
#include <stdbool.h>
#include <stdlib.h>

#define OPERAND 42
#define STOP 44
#define DOLAR 43
#define NO_TERMINAL 45

/**
 * @struct Stack item represetation.
 */
typedef struct stack_item
{
	int typ; // Symbol of stack item.
	int data_typ; // Data type used for semantic analysis.
	struct stack_item *next; // Pointer to next stack item.
} stack_item;

/**
 * @struct Stack representation.
 */
typedef struct stack
{
	stack_item *top; // Pointer to stack item on top of stack.
} stack;


/**
 * Function initializes stack.
 *
 * @param stack Pointer to stack.
 */
void stack_create(stack* stack);

/**
 * Function pushes symbol to stack and sets its data type.
 *
 * @param stack Pointer to stack.
 * @param symbol Symbol to be pushed.
 * @param type Data type to be set.
 * @return True if successfull else false.
 */
bool stack_push(stack* stack, int symbol, int data_typ);


int count_of_symbols_to_reduce(bool* founded, stack* s_stack);

/**
 * Function pops top symbol from stack.
 *
 * @param stack Pointer to stack.
 * @return True if successfull else false.
 */
bool stack_pop(stack* stack);

/**
 * Function pops stack more times.
 *
 * @param stack Pointer to stack.
 * @param count How many times stack will be popped.
 */
void stack_pop_count(stack* stack, int count);

/**
 * Function returns top termial.
 *
 * @param stack Pointer to stack.
 * @return Returns pointer to top terminal.
 */
stack_item* stack_top_terminal(stack* stack);

/**
 * Function inserts symbol after top terminal.
 *
 * @param stack Pointer to stack.
 * @param symbol Symbol to be pushed.
 * @param type Data type to be set.
 * @return True if successfull else false.
 */
bool stack_insert_after_top_terminal(stack* stack, int symbol, int data_typ);

/**
 * Function returns top symbol.
 *
 * @param stack Pointer to stack.
 * @return Pointer to symbol on top of stack.
 */
stack_item* stack_top(stack* stack);

/**
 * Function frees resources used for stack.
 *
 * @param stack Pointer to stack.
 */
void stack_free(stack* stack);
#endif
