#ifndef _PRECEDENCE_ANALYSIS_H
#define _PRECEDENCE_ANALYSIS_H

#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"
#include "error.h"
#include "stack.h"
#include "symtable.h"
//#include "parser.h"
#include "list.h"
#include "instruction.h"
#include "expression_list.h"

/**
 * @enum Pravidla pro precedenční analýzu.
 */
typedef enum
{
	RULE_PLUS,		// E -> E + E
	RULE_MINUS,		// E -> E - E
	RULE_MUL,		// E -> E * E
	RULE_DIV,		// E -> E / E
	RULE_BTN,		// E -> E > E
	RULE_LTN,		// E -> E < E
	RULE_BEQ,		// E -> E >= E
	RULE_LEQ,		// E -> E <= E
	RULE_EQ,		// E -> E == E
	RULE_NEQ,		// E -> E != E
	RULE_BRACKETS,	// E -> (E)
	RULE_OPERAND,	// E -> ID
	NO_RULE		// pravidlo neexistuje	
} precedence_rules;
/*
*@brief Zavolá generaci kódu, který pushne token na vrchol zásobníku. 
*@return Návratový kód popisující chybu/úspěch.
*/
bool gen_push(sToken* token, VarTable *varTable, InListPtr lp);
/*
*@brief Pokud je tokenem idetifikátor, nalezne ho v tabulce proměnných a vrati jeho typ.Jinak vraci hodnotu tokenu.
*@return Vrací typ tokenu.
*/
int get_data_type(sToken* token, VarTable *varTable);
/*
*@brief Zavolá generaci kódu pro reduce operace.
*@return Vrací typ tokenu.
*/
void gen_s_oper(precedence_rules rule, InListPtr lp);
/*
*@brief Sémantická kontrola typů operandů a následné genrování přetypování.
* @param rule Pointer to table.
* @param op1 Symbol 1.
* @param op2 Symbol 2.
* @param op3 Symbol 3.
* @param final_type Sets data type which will be after executing rule.
*@return Vrací SUCCES při úspěchu.
*/
int check_semantics(precedence_rules rule, stack_item* op1, stack_item* op2, stack_item* op3, int* final_type, InListPtr lp);
/**
 * @brief Implemetovaná precedenční analýza pro výrazy.
 *
 * @return Vrací true nebo false podle úspěšnosti provedení.
 */
bool expression(ListPtr expression_list,InListPtr lp, VarTable *varTable);
/**
* @brief Na základě počtu operandů a typu operátoru vrací pravidlo.
*
* @return Vrací Pravidlo na základě kterého se provádí redukce.
*/
precedence_rules rule(int count, stack_item* op1, stack_item* op2, stack_item* op3);
/**
* @brief Provádí redukci nad prvky na stacku. Volá i sémantickou kontrolu.
*
* @return Vrací SUCCES nebo ERROR podle úspěšnosti provedení.
*/
int reduce(stack* s_stack,InListPtr lp);


#endif //_EXPRESSION_H
