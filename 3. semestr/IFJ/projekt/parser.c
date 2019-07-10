#ifndef _PARSER_C
#define _PARSER_C

#include "parser.h"

sToken *toAssign;
SymTable *symTable;
unsigned LoopCount = 0;
unsigned IfCount = 0;

void gen_new_while(char** StartWhile, char** EndWhile, unsigned* Count) {

	sprintf(*StartWhile, "StartWhile%d", *Count);
	sprintf(*EndWhile, "EndWhile%d", *Count);
	(*Count)++;
}

void gen_new_if(char** EndIf, char** Else, unsigned* Count) {

	sprintf(*EndIf, "EndIf%d", *Count);
	sprintf(*Else, "Else%d", *Count);
	(*Count)++;
}

void GET_NEXT_TOKEN(sToken *token)
{
	if (token != NULL)
	{
		freeToken(token);
		token = newToken();
	}
	int err = get_token(token,stdin);
	if ( err == INTERNAL_ERROR )
	{
		freeToken(token);
		DisposeTree(symTable);
		error_exit(INTERNAL_ERROR,"INTERNAL ERROR in lexical phase\n");
	}
	else if ( err == LEXICAL_ERROR )
	{
		freeToken(token);
		DisposeTree(symTable);
		error_exit(LEXICAL_ERROR,"Lexical error\n");
	}
}

ListPtr save_all_tokens_till_the_expression_end(List *expression_list, sToken *save_token, sToken *token)
{
	if(save_token != NULL)
		ListInsertLast(expression_list,save_token);
	ListInsertLast(expression_list,token);
	do 
	{
		if (expression_list->first->next != NULL)
			if (expression_list->first->next->token->typ == EOL)
				break;
		GET_NEXT_TOKEN(token);
		if ((token->typ == KEY_WORD_DEF) || (token->typ == KEY_WORD_WHILE)
		 || (token->typ == KEY_WORD_IF) || (token->typ == KEY_WORD_ELSE))
		{
			freeToken(token);
			DisposeTree(symTable);
			ListDestroy(expression_list);
			error_exit(SYNTAX_ERROR,"SYNTAX ERROR \n");
		}
		ListInsertLast(expression_list,token);
	}
	while(!( (token->typ == EOL) || (token->typ == KEY_WORD_DO) || (token->typ == KEY_WORD_THEN) || (token->typ == END_OF_FILE)));
	return expression_list;
}

bool prog(sToken *token, InListPtr lp, SymTable* symTable)
{ 
	
	//SymbVarTableInit(var_table);
	//SymbFuncTableInit(func_table);
	if (token->typ != END_OF_FILE)
		GET_NEXT_TOKEN(token);
	if (token->typ == END_OF_FILE) // Ukončovací podmínka, když příjde end of file
		return true;
	bool is_ok = false; 
	if (token->typ == KEY_WORD_DEF) // Pravidlo č. 1: <prog> -> <fun_def> <prog> 
	{
		is_ok = function_definition(token,lp) && prog(token,lp,symTable);
	}
	// Pravidlo č. 2: <prog> -> <statement_list> <prog>
	else if((token->typ == KEY_WORD_WHILE) || (token->typ == KEY_WORD_IF) || (token->typ == IDENTIFIKATOR)
		 || (token->typ == OPERATOR_LEVA_ZAVORKA)) 
	{
		is_ok = (statement_list(token,lp)) && (prog(token,lp,symTable));
	}
	else if (token->typ == EOL) // Pravidlo č. 3: <prog> -> EOL <prog>
	{
		is_ok = (prog(token,lp,symTable));
	}
	// else if pro případ, kdy příjde expression hned jako první věc do programu
	return is_ok;
}

bool function_definition(sToken *token, InListPtr lp)
{
	sToken *funcName;
	funcName = newToken();
	bool is_ok = false;
	int param_counter = 0;
	if (token->typ == KEY_WORD_DEF) // Pravidlo č. 4: <fun_def> -> DEF ID ( <param> ) EOL <statement_list> END <prog>
	{
		GET_NEXT_TOKEN(token);
		if (token->typ == IDENTIFIKATOR)
		{
			if((isfce(token->data) != 99) || (iskeyword(token->data) != 99))
			{
				DisposeTree(symTable);
				freeToken(funcName);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error, redefinice vestavěné funkce/klíčové slovo v názvu funkce\n");
			}
			if (strCopyString(funcName,token))
			{
				DisposeTree(symTable);
				freeToken(funcName);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(INTERNAL_ERROR, "Chyba alokace");
			}
			GET_NEXT_TOKEN(token);

			FuncNodePtr fcePtr = FuncSearch(symTable->funcTable, toAssign->data);
			if(fcePtr != NULL)
			{
				DisposeTree(symTable);
				freeToken(funcName);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error, re\n");
			}

			if (token->typ == OPERATOR_LEVA_ZAVORKA)
			{
				GET_NEXT_TOKEN(token);
				if (param(token,param_counter))
				{
					if (token->typ == OPERATOR_PRAVA_ZAVORKA)
					{
						
						FunctionAdd(&symTable->funcTable->MainRoot, funcName->data , true, param_counter);
						GET_NEXT_TOKEN(token);
						if (token->typ == EOL)
						{
							GET_NEXT_TOKEN(token);
							if (statement_list(token,lp))
							{
								// GET_NEXT_TOKEN(token); Nejspíš nemá být
								if (token->typ == KEY_WORD_END)
								{
									GET_NEXT_TOKEN(token);
									is_ok = prog(token,lp,symTable); 
								}
								else
								{
									DisposeTree(symTable);
									freeToken(funcName);
									freeToken(token);
									DisposeTree(symTable);
									error_exit(SYNTAX_ERROR,"SYNTAX error, END ve func def\n");
								}
							}
							else
							{
								DisposeTree(symTable);
								freeToken(funcName);
								freeToken(token);
								DisposeTree(symTable);
								error_exit(SYNTAX_ERROR,"SYNTAX error, statement_list ve func def\n");
							}
						}
						else
						{
							DisposeTree(symTable);
							freeToken(funcName);
							freeToken(token);
							DisposeTree(symTable);
							error_exit(SYNTAX_ERROR,"SYNTAX error, EOL ve func def\n");
						}
					}
					else
					{
						DisposeTree(symTable);
						freeToken(funcName);
						freeToken(token);
						DisposeTree(symTable);
						error_exit(SYNTAX_ERROR,"SYNTAX error, pravá závorka v func def\n");
					}
				} 
				else
				{
					DisposeTree(symTable);
					freeToken(funcName);
					freeToken(token);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR,"SYNTAX error, chyba v parametru func def\n");
				}
			}
			else
			{
				DisposeTree(symTable);
				freeToken(funcName);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error, levá závorka ve func_def\n");
			}
		}
		else
		{
			DisposeTree(symTable);
			freeToken(funcName);
			freeToken(token);
			DisposeTree(symTable);
			error_exit(SYNTAX_ERROR,"SYNTAX error, END ve func_def\n");
		}
	}
	else
	{
		DisposeTree(symTable);
		freeToken(funcName);
		freeToken(token);
		DisposeTree(symTable);
		error_exit(SYNTAX_ERROR,"SYNTAX error, očekáván DEF\n");
	}
	return is_ok;
}

bool param(sToken *token, int param_counter)
{
	bool is_ok = false;
	if (token->typ == IDENTIFIKATOR) // Pravidlo č. 5: <param> -> ID <parameters>
	{
		param_counter++;
		GET_NEXT_TOKEN(token);
		is_ok = parameters(token, param_counter);
	}
	else if ( token->typ == OPERATOR_PRAVA_ZAVORKA ) // Pravidlo č. 6: <param> -> eps
		is_ok = true;
	return is_ok;
}

bool parameters(sToken *token, int param_counter)
{
	bool is_ok = false;
	if (token->typ == OPERATOR_CARKA) // Pravidlo č. 7: <parameters> -> , ID <parameters>
	{
		GET_NEXT_TOKEN(token);
		if (token->typ == IDENTIFIKATOR)
		{
			param_counter++;
			GET_NEXT_TOKEN(token);
			is_ok = parameters(token, param_counter);
		}
	}
	else if ( token->typ == OPERATOR_PRAVA_ZAVORKA ) // Pravidlo č. 8: <parameters> -> eps
		is_ok = true;

	return is_ok;
}

bool statement(sToken *token, InListPtr lp)
{
	bool is_ok = false;
	if (token->typ == KEY_WORD_WHILE) // Pravidlo č. 9: <statement> -> WHILE expression DO EOL <statement_list> END
	{
		//generate newlabelstartwhile(StartWhile)
		char* StartWhile = malloc(sizeof(char) * 16);
		char* EndWhile = malloc(sizeof(char) * 16);

		gen_new_while(&StartWhile, &EndWhile, &LoopCount);
		//generate newlabelendwhile(EndWhile)
		I_Label(lp, StartWhile);
		//I_DefVar(lp,vyraz);
		sToken *save_token = NULL; // pomocný token na uložení předešlého tokenu 
		ListPtr expression_list = NULL;
		GET_NEXT_TOKEN(token);
		expression_list = ListCreate(expression_list);
		save_all_tokens_till_the_expression_end(expression_list, save_token, token);
		if (expression(expression_list, lp, symTable->varTable))
		{
			if (token->typ == KEY_WORD_DO)
			{
				//I_PopS(lp,vyraz);
				//JumpIfEq(lp,EndWhile,vyraz,False);
				GET_NEXT_TOKEN(token);
				if (token->typ == EOL)
				{
					GET_NEXT_TOKEN(token);
					if (statement_list(token, lp))
					{
						// GET_NEXT_TOKEN(token); nejspíš nemá být
						if (token->typ == KEY_WORD_END)
						{
							I_Jump(lp, StartWhile);
							I_Label(lp, EndWhile);
							is_ok = true;
						}
						else
						{
							ListDestroy(expression_list);
							freeToken(token);
							DisposeTree(symTable);
							error_exit(SYNTAX_ERROR, "SYNTAX error, END ve WHILE.\n");
						}
					}
					else
					{
						ListDestroy(expression_list);
						freeToken(token);
						DisposeTree(symTable);
						error_exit(SYNTAX_ERROR, "SYNTAX error, statement_list ve WHILE.\n");
					}
				}
				else
				{
					ListDestroy(expression_list);
					freeToken(token);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR, "SYNTAX error, EOL ve WHILE.\n");
				}
			}
			else
			{
				ListDestroy(expression_list);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR, "SYNTAX error, DO ve WHILE.\n");
			}
		}
		else
		{
			ListDestroy(expression_list);
			freeToken(token);
			DisposeTree(symTable);
			error_exit(SYNTAX_ERROR, "SYNTAX error v prediktivní analýze ve WHILE.\n");
		}
		free(StartWhile);
		free(EndWhile);
	}
	else if (token->typ == KEY_WORD_IF) // pravidlo č. 10: <statement> -> IF expression THEN EOL <statement_list> <else_statement>
	{
		char* EndIf = malloc(sizeof(char) * 16);
		char* Else = malloc(sizeof(char) * 16);

		gen_new_if(&EndIf, &Else, &IfCount);
		//generate newlabelElse(Else)
		//generate newlabelendEndIf(EndIf)
		//I_DefVar(lp,vyraz);
		GET_NEXT_TOKEN(token);
		sToken *save_token = NULL; // pomocný token na uložení předešlého tokenu 
		ListPtr expression_list = NULL;
		expression_list = ListCreate(expression_list);
		save_all_tokens_till_the_expression_end(expression_list, save_token, token);
		if (expression(expression_list, lp, symTable->varTable))
		{
			if (token->typ == KEY_WORD_THEN)
			{
				//I_PopS(lp,vyraz);

				//JumpIfEq(lp,Else,vyraz,False);
				GET_NEXT_TOKEN(token);
				if (token->typ == EOL)
				{
					GET_NEXT_TOKEN(token);
					if (statement_list(token, lp))
					{
						// GET_NEXT_TOKEN(token); Nejspíš by nemělo být
						if (token->typ == KEY_WORD_ELSE)
						{
							I_Jump(lp, EndIf);
							I_Label(lp, Else);
							GET_NEXT_TOKEN(token);
							if (token->typ == EOL)
							{
								GET_NEXT_TOKEN(token);
								if (statement_list(token, lp))
								{
									// GET_NEXT_TOKEN(token); TODO: nejspíš tu nemá být!
									if (token->typ == KEY_WORD_END)
									{
										I_Label(lp, EndIf);
										is_ok = true;
									}
									else
									{
										ListDestroy(expression_list);
										freeToken(token);
										DisposeTree(symTable);
										error_exit(SYNTAX_ERROR, "SYNTAX error, END v IFu\n");
									}
								}
								else
								{
									ListDestroy(expression_list);
									freeToken(token);
									DisposeTree(symTable);
									error_exit(SYNTAX_ERROR, "SYNTAX error, 2. statement list v IFu\n");
								}
							}
							else
							{
								ListDestroy(expression_list);
								freeToken(token);
								DisposeTree(symTable);
								error_exit(SYNTAX_ERROR, "SYNTAX error, ELSE -> EOL \n");
							}
						}
						else
						{
							ListDestroy(expression_list);
							freeToken(token);
							DisposeTree(symTable);
							error_exit(SYNTAX_ERROR, "SYNTAX error, ELSE v IFu \n");
						}
					}
					else
					{
						ListDestroy(expression_list);
						freeToken(token);
						DisposeTree(symTable);
						error_exit(SYNTAX_ERROR, "SYNTAX error, statement list v IFu\n");
					}
				}
				else
				{
					ListDestroy(expression_list);
					freeToken(token);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR, "SYNTAX error, THEN -> EOL\n");
				}
			}
			else
			{
				ListDestroy(expression_list);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR, "SYNTAX error, IF -> THEN\n");
			}
		}
		else
		{
			ListDestroy(expression_list);
			freeToken(token);
			DisposeTree(symTable);
			error_exit(SYNTAX_ERROR, "SYNTAX error v prediktivní analýze za IFem.\n");
		}
		free(EndIf);
		free(Else);
	}
	else if(token->typ == IDENTIFIKATOR) // Pravidlo č.11: <statement> -> ID <next>
	{
		sToken *save_token = NULL; // pomocný token na uložení předešlého tokenu 
		toAssign = newToken();
		toAssign->typ = token->typ;
		strCopyString(toAssign,token);
		save_token = newToken(); // allokuju ho jen v případě, kdy potřebuji nejdřív načíst následující token pro zjištění, jestli jde o expression
		save_token->typ = token->typ;
		strCopyString(save_token,token);
		GET_NEXT_TOKEN(token);
		if ( (token->typ == OPERATOR_PLUS) || (token->typ == OPERATOR_MINUS) || (token->typ == OPERATOR_DELENO) || (token->typ == OPERATOR_KRAT) 
			|| (token->typ == OPERATOR_LEQ) || (token->typ == OPERATOR_BEQ) || (token->typ == OPERATOR_LTN) || (token->typ == OPERATOR_BTN) 
			|| (token->typ == OPERATOR_NEQ) || (token->typ == OPERATOR_EQ) )
		{
			
			ListPtr expression_list = NULL;;
			expression_list = ListCreate(expression_list);
			save_all_tokens_till_the_expression_end(expression_list, save_token, token);
			if (expression(expression_list,lp,symTable->varTable))
			{
				if (token->typ == EOL)
				{
					GET_NEXT_TOKEN(token);
					is_ok = true;
				}
				else
				{
					freeToken(save_token);
					freeToken(token);
					freeToken(toAssign);
					DisposeTree(symTable);
					ListDestroy(expression_list);
					error_exit(SYNTAX_ERROR,"SYNTAX error, EOL po expression.\n");
				}
			}
			ListDestroy(expression_list);
		}
		else  // Pravidlo č.12: <statement> -> <expression> EOL
		{
			// zavolani na radku jen funkce nebo jen promenne bez parametru ci prirazeni
			if (token->typ == EOL)
			{
				VarNodePtr varPtr = VarSearch(symTable->varTable, toAssign->data);
				FuncNodePtr fcePtr = FuncSearch(symTable->funcTable, toAssign->data);
				// kdyz neni v tabulce promennych
				if ( (varPtr == NULL) && (fcePtr == NULL))
				{
					bool isLocalFrame = true;
					VariableAdd(&symTable->varTable->MainRoot, toAssign->data, NIL, isLocalFrame);
					VarNodePtr VarNPtr;
					VarNPtr = VarSearch(symTable->varTable, toAssign->data);
					I_DefVarNode(lp, VarNPtr);
					I_MoveS(lp, VarNPtr, "nil@nil");
				}
				// kdyz je to promenna, neni funkce, a je definovana a je zavolana sama bez vseho na radku  
				else if ( (varPtr != NULL) && (fcePtr == NULL))
				{
					// vratit hodnotu promenne jako return v tom framu
					
				}
				else if ( (varPtr == NULL) && (fcePtr != NULL))
				{
					// zavolani funkce bez parametru
				}
				else if ( (varPtr != NULL) && (fcePtr != NULL))
				{
					freeToken(save_token);
					freeToken(token);
					freeToken(toAssign);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR,"SYNTAX error v next.\n");
				}
			}
			if(next(token,save_token,lp))
			{
				GET_NEXT_TOKEN(token);
				is_ok = true;
			}
			else
			{
				freeToken(save_token);
				freeToken(token);
				freeToken(toAssign);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error v next.\n");
			}
		}
		freeToken(save_token);
	}
	else if(token->typ == OPERATOR_LEVA_ZAVORKA) // možnost, kdy začíná expression levou závorkou
	{
		sToken *save_token = NULL; // pomocný token na uložení předešlého tokenu 
		ListPtr expression_list = NULL;;
		expression_list = ListCreate(expression_list);
		save_all_tokens_till_the_expression_end(expression_list, save_token, token);
		if(expression(expression_list,lp,symTable->varTable))
		{
			if (token->typ == EOL)
			{
				GET_NEXT_TOKEN(token);
				is_ok = true;
			}
			else
			{
				freeToken(save_token);
				freeToken(token);
				DisposeTree(symTable);
				ListDestroy(expression_list);
				error_exit(SYNTAX_ERROR,"SYNTAX error, EOL po expression.\n");
			}
		}
		freeToken(save_token);
		ListDestroy(expression_list);
	}
	return is_ok;
}


bool statement_list(sToken *token,InListPtr lp)
{
	bool is_ok = false;
	// Pravidlo č. 13: <statement_list> -> <statement> <statement_list>
	if((token->typ == KEY_WORD_WHILE) || (token->typ == KEY_WORD_IF) || (token->typ == IDENTIFIKATOR) || (token->typ == OPERATOR_LEVA_ZAVORKA)) 
	{
		is_ok = ((statement(token,lp)) && (statement_list(token,lp)));
	}
	else if((token->typ == EOL) || (token->typ == KEY_WORD_END) || (token->typ == KEY_WORD_DO) || (token->typ == END_OF_FILE) || (token->typ == KEY_WORD_THEN) )
	{
		is_ok = true;
	}
	return is_ok;
}

bool next(sToken *token, sToken *save_token, InListPtr lp)
{
	bool is_ok = false;
	if (token->typ == OPERATOR_PRIRAZOVACI) // Pravidlo č. 14: <next> -> = <assign_value> EOL
	{
		
		GET_NEXT_TOKEN(token);
		if (assign_value(token,lp))
		{
			if (token->typ == EOL)
				is_ok = true;
		}
	}
	else if ((token->typ == EOL) || (token->typ == OPERATOR_LEVA_ZAVORKA) || (token->typ == IDENTIFIKATOR)) 
	{
		if(call_function(token)) // Pravidlo č. 15: <next> -> <call_func>
			is_ok = true;
	}
	return is_ok;
}

bool assign_value(sToken *token, InListPtr lp)
{
	bool is_ok = false;
	if(token->typ == IDENTIFIKATOR || (token->typ == INTEGER) || (token->typ == DOUBLE) || (token->typ == DOUBLE_EXPONENT) || (token->typ == DOUBLE_DOTTED_EXPONENT) || (token->typ == STRING) ) // Pravidlo č.17: <assign_value> -> ID <assign_value2>
	{
		sToken *save_token = NULL; // pomocný token na uložení předešlého tokenu 
		save_token = newToken(); // allokuju ho jen v případě, kdy potřebuji nejdřív načíst následující token pro zjištění, jestli jde o expression
		save_token->typ = token->typ;
		strCopyString(save_token,token);
		GET_NEXT_TOKEN(token);
		if ( (token->typ == OPERATOR_PLUS) || (token->typ == OPERATOR_MINUS) || (token->typ == OPERATOR_DELENO) || (token->typ == OPERATOR_KRAT) 
			|| (token->typ == OPERATOR_LEQ) || (token->typ == OPERATOR_BEQ) || (token->typ == OPERATOR_LTN) || (token->typ == OPERATOR_BTN) 
			|| (token->typ == OPERATOR_NEQ) || (token->typ == OPERATOR_EQ) ) // Pravidlo č.16: <assign_value> -> <expression>
		{
				ListPtr expression_list = NULL;
			expression_list = ListCreate(expression_list);
			save_all_tokens_till_the_expression_end(expression_list, save_token, token);
			if (expression(expression_list,lp,symTable->varTable))
			{
				if (token->typ == EOL)
					is_ok = true;
				else
				{
					freeToken(save_token);
					freeToken(token);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR,"SYNTAX error, EOL po expression.\n");
				}
				// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				// Hledani promenne jestli je v tabulce symbolu
				VarNodePtr varPtr = VarSearch(symTable->varTable, toAssign->data);
				FuncNodePtr fcePtr = FuncSearch(symTable->funcTable, toAssign->data);
				// kdyz neni v tabulce promennych
				if ( (varPtr == NULL) && (fcePtr == NULL))
				{
					bool isLocalFrame = true;
					VariableAdd(&symTable->varTable->MainRoot, toAssign->data, NIL, isLocalFrame);
					VarNodePtr VarNPtr;
					VarNPtr = VarSearch(symTable->varTable, toAssign->data);
					I_DefVarNode(lp, VarNPtr);
					//I_MoveS(lp, VarPtr, "nil@nil");
					//I_PopSNode(lp, VarNPtr var);
				}
				// kdyz je to promenna, neni funkce, a je definovana a je zavolana sama bez vseho na radku  
				else if ( (varPtr != NULL) && (fcePtr == NULL))
				{
					// prirazovani hodnoty do promenne ktera uz byla definovana
					bool isLocalFrame = true;
					VariableAdd(&symTable->varTable->MainRoot, toAssign->data, varPtr->Type , isLocalFrame);
				}
				else if ( (varPtr == NULL) && (fcePtr != NULL))
				{
					freeToken(save_token);
					freeToken(token);
					freeToken(toAssign);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR,"SYNTAX error nelze prirazovat do funkce.\n");
				}
				else if ( (varPtr != NULL) && (fcePtr != NULL))
				{
					freeToken(save_token);
					freeToken(token);
					freeToken(toAssign);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR,"SYNTAX error nalezeno i v tabulce promennych i v tabulce funkci \n");
				}
			}
			ListDestroy(expression_list);		
		}
		else
		{
		if( ((save_token->typ == STRING) || (save_token->typ == DOUBLE) || (save_token->typ == NIL) 
				|| (save_token->typ == DOUBLE_EXPONENT) || (save_token->typ == DOUBLE_DOTTED_EXPONENT))
			&& (token->typ == EOL) )
		{ 
			ListPtr expression_list = NULL;
			expression_list = ListCreate(expression_list);
			save_all_tokens_till_the_expression_end(expression_list, save_token, token);
			if (!expression(expression_list,lp,symTable->varTable))
			{
				freeToken(save_token);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error v assign_value.\n");
			}
		}
		ungettoken(token,stdin);
		token->typ = save_token->typ;
		strCopyString(token,save_token);
		if(assign_value_2(token))
				is_ok = true;
			else
			{
				freeToken(save_token);
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error v assign_value.\n");
			}
		}
		freeToken(save_token);
	}
	else if(token->typ == OPERATOR_LEVA_ZAVORKA) // možnost, kdy začíná expression levou závorkou
	{
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!§§
		// JE POTŘEBA NĚJAK ZMĚNIT, PROTOŽE MŮŽE PŘIJÍT LEVÁ ZÁVORKA TAK, ŽE SE NEJEDNÁ
		// O EXPRESSION
		sToken *save_token = NULL; // pomocný token na uložení předešlého tokenu  
		ListPtr expression_list = NULL;
		expression_list = ListCreate(expression_list);
		save_all_tokens_till_the_expression_end(expression_list, save_token, token);
		if(expression(expression_list,lp,symTable->varTable))
		{
			if (token->typ == EOL)
				is_ok = true;
			else
			{
				freeToken(save_token);
				freeToken(token);
				DisposeTree(symTable);
				ListDestroy(expression_list);
				error_exit(SYNTAX_ERROR,"SYNTAX error, EOL po expression.\n");
			}
		}
		ListDestroy(expression_list);
	}
	return is_ok;
}

bool assign_value_2(sToken *token)
{
	bool is_ok = false;
	if (token->typ == OPERATOR_LEVA_ZAVORKA) // Pravidlo č. 18: <assign_value2> -> ( <term> )
	{
		GET_NEXT_TOKEN(token);
		is_ok = term(token);
	}
	else if (term(token)) // Pravidlo č. 19: <assign_value2> -> <term>
		is_ok = true;
	return is_ok; 
}

bool call_function(sToken *token)
{
	bool is_ok = false;
	if (token->typ == OPERATOR_LEVA_ZAVORKA) // Pravidlo č. 20: <call_func> -> ( <term> )
	{
		GET_NEXT_TOKEN(token);
		if(term(token))
		{
			is_ok = true;
		}
	}
	else if(term(token))  // Pravidlo č. 21: <call_func> -> <term>
		is_ok = true;
	return is_ok; 
}

bool term(sToken *token)
{
	bool is_ok = false;
	if ((token->typ == IDENTIFIKATOR) || (token->typ == STRING) || (token->typ == DOUBLE) || (token->typ == NIL) 
				|| (token->typ == DOUBLE_EXPONENT) || (token->typ == DOUBLE_DOTTED_EXPONENT) || (token->typ == INTEGER)) // Pravidlo č. 22: <term> -> ID <terms>
	{
		if (token->typ == IDENTIFIKATOR)
		{
			VarNodePtr varPtr = VarSearch(symTable->varTable, token->data);
			FuncNodePtr fcePtr = FuncSearch(symTable->funcTable, token->data);
			// kdyz neni v tabulce promennych
			if ((fcePtr != NULL))
			{
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error, term jako jmeno funkce.\n");
			}
			else if ( (varPtr == NULL) )
			{
				freeToken(token);
				DisposeTree(symTable);
				error_exit(SYNTAX_ERROR,"SYNTAX error, nedefinovaná proměnná.\n");
			}
		}
		GET_NEXT_TOKEN(token);
		is_ok = terms(token);
	}
	else if ((token->typ == OPERATOR_PRAVA_ZAVORKA)) // Pravidlo č. 25: <terms> -> eps
	{	
		GET_NEXT_TOKEN(token);
		if (token->typ == EOL)
		{
			is_ok = true;	
		}
	}
	else if ((token->typ == EOL))
		is_ok = true;	
	return is_ok;
}

bool terms(sToken *token)
{
	bool is_ok = false;
	if (token->typ == OPERATOR_CARKA)
	{
		GET_NEXT_TOKEN(token);
		if ((token->typ == IDENTIFIKATOR) || (token->typ == STRING) || (token->typ == DOUBLE) || (token->typ == NIL) 
			|| (token->typ == DOUBLE_EXPONENT) || (token->typ == DOUBLE_DOTTED_EXPONENT) || (token->typ == INTEGER)) // Pravidlo č. 24: <terms> -> , ID <terms>
		{
			if (token->typ == IDENTIFIKATOR)
			{
				VarNodePtr varPtr = VarSearch(symTable->varTable, token->data);
				FuncNodePtr fcePtr = FuncSearch(symTable->funcTable, token->data);
				// kdyz neni v tabulce promennych
				if ((fcePtr != NULL))
				{
					freeToken(token);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR,"SYNTAX error, term jako jmeno funkce.\n");
				}
				else if ( (varPtr == NULL) )
				{
					freeToken(token);
					DisposeTree(symTable);
					error_exit(SYNTAX_ERROR,"SYNTAX error, nedefinovaná proměnná.\n");
				}
			}

			GET_NEXT_TOKEN(token);
			is_ok = terms(token);
		}
	}
	else if ((token->typ == OPERATOR_PRAVA_ZAVORKA)) // Pravidlo č. 25: <terms> -> eps
	{	
		GET_NEXT_TOKEN(token);
		if (token->typ == EOL)
		{
			is_ok = true;	
		}
	}
	else if ((token->typ == EOL))
		is_ok = true;	
	return is_ok;
}

#endif
