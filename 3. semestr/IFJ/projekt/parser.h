#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "error.h"
#include "str.h"
#include "scanner.h"
#include "precedence_analysis.h"
#include "expression_list.h"
#include "symtable.h"
#include "list.h"
/**
* @brief generuje nové návìští pro While.
*
*/
void gen_new_while(char** StartWhile, char** EndWhile, unsigned* Count);
/**
* @brief generuje nové návìští pro If.
*
*/
void gen_new_if(char** EndIf, char** Else, unsigned* Count);
/**
* @brief Naèitá tokeny do listu dokud neskonèí výraz.
*
* @return Vrací ukazatel na list s tokeny.
*/
ListPtr save_all_tokens_till_the_expression_end(List *expression_list, sToken *save_token, sToken *token);
/**
* @brief Do "token" uloží naète nový token.
*
*/
void GET_NEXT_TOKEN(sToken *token);
/**
* @brief .
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool prog(sToken *token, InListPtr lp, SymTable* s);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool function_definition(sToken *token, InListPtr lp);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool statement(sToken *token, InListPtr lp);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool statement_list(sToken *token,InListPtr lp);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool param(sToken *token, int param_counter);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool parameters(sToken *token, int param_counter);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool next(sToken *token,sToken *save_token, InListPtr lp);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool assign_value(sToken *token,InListPtr lp);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool assign_value_2(sToken *token);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool call_function(sToken *token);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool term(sToken *token);
/**
* @brief 
*
* @return Vrací true nebo false podle úspìšnosti provedení.
*/
bool terms(sToken *token);

#endif
