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
* @brief generuje nov� n�v�t� pro While.
*
*/
void gen_new_while(char** StartWhile, char** EndWhile, unsigned* Count);
/**
* @brief generuje nov� n�v�t� pro If.
*
*/
void gen_new_if(char** EndIf, char** Else, unsigned* Count);
/**
* @brief Na�it� tokeny do listu dokud neskon�� v�raz.
*
* @return Vrac� ukazatel na list s tokeny.
*/
ListPtr save_all_tokens_till_the_expression_end(List *expression_list, sToken *save_token, sToken *token);
/**
* @brief Do "token" ulo�� na�te nov� token.
*
*/
void GET_NEXT_TOKEN(sToken *token);
/**
* @brief .
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool prog(sToken *token, InListPtr lp, SymTable* s);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool function_definition(sToken *token, InListPtr lp);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool statement(sToken *token, InListPtr lp);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool statement_list(sToken *token,InListPtr lp);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool param(sToken *token, int param_counter);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool parameters(sToken *token, int param_counter);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool next(sToken *token,sToken *save_token, InListPtr lp);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool assign_value(sToken *token,InListPtr lp);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool assign_value_2(sToken *token);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool call_function(sToken *token);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool term(sToken *token);
/**
* @brief 
*
* @return Vrac� true nebo false podle �sp�nosti proveden�.
*/
bool terms(sToken *token);

#endif
