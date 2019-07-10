#ifndef _PRECEDENCE_ANALYSIS_C
#define _PRECEDENCE_ANALYSIS_C

#include "precedence_analysis.h"

#define TABLE_SIZE 8

typedef enum
{
	P = 0,    // < PUSH
	E = 1,     // = EQUAL
	R = 2,    // > REDUCE
	N = 3    // # NOTDEFINED
} Prec_table_sign_enum;

typedef enum
{
	MUL_DIV = 0,			// 0 */
	PLUS_MINUS = 1,			// 1 +-
	REL_OPERATORS = 2,		// 2 R
	REL_OPERATORS_2 = 3,	// 3 R2
	LEFT_BRACKET = 4,		// 4 (
	RIGHT_BRACKET = 5,		// 5 )
	DATA = 6,				// 6 ID
	DOLLAR = 7				// 7 $
} Prec_table_index_enum;
stack s_stack;


// Precedence table
int prec_table[TABLE_SIZE][TABLE_SIZE] =
{
//	|+- | */| R | R2| ( | ) | ID | $ |
	{ R , P , R , R , P , R , P , R }, // +-
	{ R , R , R , R , P , R , P , R }, // */
	{ P , P , N , R , P , R , P , R }, // R (realtion operators) = <> <= < >= >
	{ P , P , P , N , P , R , P , R }, // R2
	{ P , P , P , P , P , E , P , N }, // (
	{ R , R , R , R , N , R , N , R }, // )
	{ R , R , R , R , N , R , N , R }, // ID (identifikators, values)
	{ P , P , P , P , P , N , P , N }  // $
};


Prec_table_index_enum get_prec_table_index(int typ)
{
	switch (typ)
	{
	case OPERATOR_PLUS:
	case OPERATOR_MINUS:
		return PLUS_MINUS;

	case OPERATOR_KRAT:
	case OPERATOR_DELENO:
		return MUL_DIV;

	case OPERATOR_LEQ:
	case OPERATOR_BEQ:
	case OPERATOR_BTN:
	case OPERATOR_LTN:
		return REL_OPERATORS;

	case OPERATOR_EQ:
	case OPERATOR_NEQ:
		return REL_OPERATORS_2;

	case OPERATOR_LEVA_ZAVORKA:
		return LEFT_BRACKET;

	case OPERATOR_PRAVA_ZAVORKA:
		return RIGHT_BRACKET;

	case IDENTIFIKATOR:
	case INTEGER:
	case DOUBLE:
	case DOUBLE_EXPONENT:
	case DOUBLE_DOTTED_EXPONENT:
	case STRING:
		return DATA;

	default:
		return DOLLAR;

	}
} 

bool gen_push(sToken* token, VarTable *varTable, InListPtr lp) {
	char* tmp;
	char* f_tmp;
	VarNodePtr symbol;
	switch (token->typ)
	{
	case IDENTIFIKATOR:
		symbol = VarSearch(varTable, token->data);
		I_PushSNode(lp, symbol);
		//ADD_CODE("LF@"); ADD_CODE(token.attribute.string->str);
		break;

	case INTEGER:
		tmp = "int@";
		f_tmp = strConCat(tmp, token->data);
		I_PushSString(lp, f_tmp);
		free(f_tmp);
		break;

	case DOUBLE_EXPONENT:
	case DOUBLE_DOTTED_EXPONENT:
	case DOUBLE:
		tmp = "double@";
		f_tmp = strConCat(tmp, token->data);
		I_PushSString(lp, f_tmp);
		free(f_tmp);
		break;

	case STRING:
		tmp = "string@";
		f_tmp = strConCat(tmp, token->data);
		I_PushSString(lp, f_tmp);
		free(f_tmp);
		break;

	default:
		return false;
	}

	
	return true;
}

int get_data_type(sToken* token, VarTable *varTable)
{
	VarNodePtr symbol;
	
	switch (token->typ)
	{
	case IDENTIFIKATOR:
		symbol = VarSearch(varTable, token->data);
		if (symbol == NULL)
			error_exit(SEMANTIC_DEFINE_ERROR, "Nedefinovaná proměnná.\n");

		return symbol->Type;

	case INTEGER:
		return INTEGER;

	case DOUBLE_EXPONENT:
	case DOUBLE_DOTTED_EXPONENT:
	case DOUBLE:
		return DOUBLE;
	
	case NIL:
		return NIL;

	case STRING:
		return STRING;
	default:
		
		return token->typ;
	}
 
}
void gen_s_oper(precedence_rules rule, InListPtr lp) {
	switch (rule)
	{
	case RULE_PLUS:
		I_AddS(lp);
		break;

	case RULE_MINUS:
		I_SubS(  lp);
		break;

	case RULE_MUL:
		I_MulS( lp);
		break;

	case RULE_DIV:
		I_DivS( lp);
		break;

	
	case RULE_EQ:
		I_EQS( lp);
		break;

	case RULE_NEQ:
		I_EQS( lp);
		I_NotS( lp);
		break;

	case RULE_LEQ:
		I_PopSString( lp, "GF@%help1");
		I_PopSString( lp, "GF@%help2");
		I_PushSString( lp, "GF@%help2");
		I_PushSString( lp, "GF@%help1");
		I_LTS( lp);
		I_PushSString( lp, "GF@%help2");
		I_PushSString( lp, "GF@%help1");
		I_EQS( lp);
		I_OrS( lp);
		break;

	case RULE_LTN:
		I_LTS( lp);
		break;

	case RULE_BEQ:
		I_PopSString( lp, "GF@%help1");
		I_PopSString( lp, "GF@%help2");
		I_PushSString( lp, "GF@%help2");
		I_PushSString( lp, "GF@%help1");
		I_GTS( lp);
		I_PushSString( lp, "GF@%help2");
		I_PushSString( lp, "GF@%help1");
		I_EQS( lp);
		I_OrS( lp);
		break;

	case RULE_BTN:
		I_GTS( lp);
		break;

	default:
		break;
	}
	
}


int check_semantics(precedence_rules rule, stack_item* op1, stack_item* op2, stack_item* op3,int* final_type, InListPtr lp)
{
	bool retype_op1_to_double = false;
	bool retype_op3_to_double = false;
	bool retype_op1_to_integer = false;
	bool retype_op3_to_integer = false;


	if (rule != RULE_OPERAND && rule != RULE_BRACKETS)
	{
		if (op1->data_typ == BOOL || op3->data_typ == BOOL)
			error_exit(SEMANTIC_DATATYPE_ERROR, "operace nad boolean.\n");
	}
	switch (rule)
	{
	case RULE_OPERAND:
		*final_type = op1->data_typ;
		break;

	case RULE_BRACKETS:
		*final_type = op2->data_typ;
		break;

	case RULE_PLUS:
	case RULE_MINUS:
	case RULE_MUL:
		if (op1->data_typ == STRING && op3->data_typ == STRING && rule == RULE_PLUS)
		{
			*final_type = STRING;
			break;
		}

		if (op1->data_typ == INTEGER && op3->data_typ == INTEGER)
		{
			*final_type = INTEGER;
			break;
		}

		if (op1->data_typ == STRING || op3->data_typ == STRING)
			error_exit(SEMANTIC_DATATYPE_ERROR, "spatna typova kompabilita.\n");
		
		if (op1->data_typ == NIL || op3->data_typ == NIL)
			error_exit(SEMANTIC_DATATYPE_ERROR, "spatna typova kompabilita.\n");

		*final_type = DOUBLE;

		if (op1->data_typ == INTEGER)
			retype_op1_to_double = true;

		if (op3->data_typ == INTEGER)
			retype_op3_to_double = true;

		break;

	case RULE_DIV:
		*final_type = DOUBLE;

		if (op1->data_typ == STRING || op3->data_typ == STRING)
			error_exit(SEMANTIC_DATATYPE_ERROR, "spatna typova kompabilita.\n");

		if (op1->data_typ == NIL || op3->data_typ == NIL)
			error_exit(SEMANTIC_DATATYPE_ERROR, "spatna typova kompabilita.\n");
		
		if (op1->data_typ == INTEGER)
			retype_op1_to_double = true;

		if (op3->data_typ == INTEGER)
			retype_op3_to_double = true;

		break;


	case RULE_BTN:
	case RULE_LTN:
	case RULE_BEQ:
	case RULE_LEQ:
	case RULE_EQ:
	case RULE_NEQ:
		*final_type = BOOL;//spatne potreba bool

		if (op1->data_typ == INTEGER && op3->data_typ == DOUBLE)
			retype_op1_to_double = true;

		else if (op1->data_typ == DOUBLE && op3->data_typ == INTEGER)
			retype_op3_to_double = true;

		if (op1->data_typ == NIL || op3->data_typ == NIL)
			error_exit(SEMANTIC_DATATYPE_ERROR, "spatna typova kompabilita.\n");
		
		else if (op1->data_typ != op3->data_typ)
			error_exit(SEMANTIC_DATATYPE_ERROR, "spatna typova kompabilita.\n");

		break;

	default:
		break;
	}

	if (retype_op1_to_double)
	{
		I_PopSString(lp,"GF@%help1");
		I_Int2FloatS(lp);
		I_PushSString(lp, "GF@%help1");
	}

	if (retype_op3_to_double)
	{
		I_Int2FloatS(lp);
	}

	if (retype_op1_to_integer)
	{
		I_PopSString(lp, "GF@%help1");
		I_Float2IntS(lp);
		I_PushSString(lp, "GF@%help1");
	}

	if (retype_op3_to_integer)
	{
		I_Float2IntS(lp);
	}

	return SUCCES;
}

int reduce(stack* s_stack,InListPtr lp)
{
	stack_item* op1 = NULL;
	stack_item* op2 = NULL;
	stack_item* op3 = NULL;
	int final_type;
	precedence_rules rule_generate;
	bool found = false;
	int count = count_of_symbols_to_reduce(&found, s_stack);

	if (found && count == 3)
	{
		op1 = s_stack->top->next->next;
		op2 = s_stack->top->next;
		op3 = s_stack->top;
		rule_generate = rule(count, op1, op2, op3);
	}
	else if (found && count == 1)
	{
		op1 = s_stack->top;
		rule_generate = rule(count, op1, NULL, NULL);
	}
	else
		return SYNTAX_ERROR;

	if (rule_generate == NO_RULE)
	{
		return SYNTAX_ERROR;
	}
	else
	{
		check_semantics(rule_generate, op1, op2, op3, &final_type,lp);
			

		if (rule_generate == RULE_PLUS && final_type == STRING)
		{
			I_PopSString(lp,"GF@%help3");
			I_PopSString(lp,"GF@%help2");
			I_ConCatSSS(lp, "GF@%help1", "GF@%help2", "GF@%help3");
			I_PushSString(lp, "GF@%help1");
		}
		else 
			gen_s_oper(rule_generate,lp);
		
		stack_pop_count(s_stack, count + 1);
		stack_push(s_stack, NO_TERMINAL, final_type);
	}
	return SUCCES;
}

precedence_rules rule(int count, stack_item* op1, stack_item* op2, stack_item* op3)
{
	switch (count)
	{
	case 1:
		// pravidlo E -> ID
		if ( (op1->typ == IDENTIFIKATOR) || (op1->typ == INTEGER) || (op1->typ == DOUBLE) 
			|| (op1->typ == DOUBLE_EXPONENT) || (op1->typ == STRING) || (op1->typ == DOUBLE_DOTTED_EXPONENT) )
			return RULE_OPERAND;

		return NO_RULE;

	case 3:
		// rule E -> (E)
		if (op1->typ == OPERATOR_LEVA_ZAVORKA && op2->typ == NO_TERMINAL && op3->typ == OPERATOR_PRAVA_ZAVORKA)
			return RULE_BRACKETS;

		if (op1->typ == NO_TERMINAL && op3->typ == NO_TERMINAL)
		{
			switch (op2->typ)
			{
			case OPERATOR_PLUS:	// pravidlo E -> E + E
				return RULE_PLUS;			
			case OPERATOR_MINUS:	// pravidlo E -> E - E
				return RULE_MINUS;			
			case OPERATOR_KRAT:	// pravidlo E -> E * E
				return RULE_MUL;			
			case OPERATOR_DELENO:	// pravidlo E -> E / E
				return RULE_DIV;			
			case OPERATOR_BTN:	// pravidlo E -> E > E
				return RULE_BTN;			
			case OPERATOR_LTN:	// pravidlo E -> E < E
				return RULE_LTN;			
			case OPERATOR_BEQ:	// pravidlo E -> E >= E
				return RULE_BEQ;			
			case OPERATOR_LEQ:	// pravidlo E -> E <= E
				return RULE_LEQ;			
			case OPERATOR_EQ:	// pravidlo E -> E == E
				return RULE_EQ;			
			case OPERATOR_NEQ:	// pravidlo E -> E != E
				return RULE_NEQ;
			default:	// něco je špatně
				return NO_RULE;
			}
		}
		return NO_RULE;
	}
	return NO_RULE;
}

bool expression(ListPtr expression_list, InListPtr lp, VarTable *varTable)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// FUNGUJE, ALE NEDOSTAVA SE DO EPRESSION LISTU UKONCOVACI THEN/EOL/DO, TUDIZ TO PADA
	// PROTOŽE SE TO DOSTANE NA NULL POINTER A SKOCI TO DO ERRORU, TO JE DŮVOD,
	// PROČ TO HÁŽE SIGSEGV UŽ V NAČÍTÁNÍ TOKENŮ DO LISTU, NEJSPÍŠ SE ŠPATNĚ POSOUVÁ
	// ACTIVE V UKLÁDÁNÍ TOKENŮ DO LISTU
	stack_create(&s_stack);
	if (!stack_push(&s_stack, DOLAR, DOLAR))
	{
		ListDestroy(expression_list);
		stack_free(&s_stack);
		error_exit(INTERNAL_ERROR, "Nepodařilo se alokovat paměť pro stack.");
	}
	bool is_ok = false;
	stack_item* terminal_on_stack_top;
	sToken *tmpToken;
	tmpToken = newToken();
	int actual_typ;
	ListActFirst(expression_list);
	do
	{
		actual_typ = expression_list->active->token->typ;

		strCopyString(tmpToken,expression_list->active->token);
		tmpToken->typ = expression_list->active->token->typ;

		terminal_on_stack_top = stack_top_terminal(&s_stack);

			printf("actual typ: %d\n",actual_typ );
			printf("Terminal top :%d\n",terminal_on_stack_top->typ );
			if (terminal_on_stack_top->next != NULL)
			printf("Terminal top next :%d\n",terminal_on_stack_top->next->typ );

		Prec_table_sign_enum prec_table_cell = prec_table[get_prec_table_index(terminal_on_stack_top->typ)][get_prec_table_index(actual_typ)];

		if (!((prec_table_cell == R) || (prec_table_cell == N)))
				ListActNext(expression_list);

		if (expression_list->active == NULL)
		{
			ListDestroy(expression_list);
			stack_free(&s_stack);
			error_exit(SYNTAX_ERROR, "Nepodařilo se alokovaasaast paměť pro stack.");
		}
		int err;

		switch(prec_table_cell)
		{
			case E:
				if(!stack_push(&s_stack, actual_typ, get_data_type(tmpToken,varTable)))
				{
					ListDestroy(expression_list);
					stack_free(&s_stack);
					error_exit(INTERNAL_ERROR,"Nepodařila se allokace itemu ve stacku.\n");
				}
			break;

			case P:
				if (!stack_insert_after_top_terminal(&s_stack, STOP, STOP)) // kontrola jestli je na stacku terminál
				{
					ListDestroy(expression_list);
					stack_free(&s_stack);
					error_exit(INTERNAL_ERROR,"Nepodařila se allokace itemu ve stacku.\n");
				}
				if(!stack_push(&s_stack, actual_typ, get_data_type(tmpToken,varTable)))
				{
					ListDestroy(expression_list);
					stack_free(&s_stack);
					error_exit(INTERNAL_ERROR,"Nepodařila se allokace itemu ve stacku.\n");
				}
				if (actual_typ == IDENTIFIKATOR || actual_typ == INTEGER || actual_typ == DOUBLE || actual_typ == DOUBLE_EXPONENT
					|| actual_typ == DOUBLE_DOTTED_EXPONENT || actual_typ == STRING)
				{
					gen_push(tmpToken, varTable, lp);
				}
				break;
			
			case R:
				err = reduce(&s_stack,lp);
				if(err == SYNTAX_ERROR)
				{
					ListDestroy(expression_list);
					stack_free(&s_stack);
					error_exit(SYNTAX_ERROR,"Chyba v pravidlech PSA.\n");
				}

				else if(err == INTERNAL_ERROR)
				{
					ListDestroy(expression_list);
					stack_free(&s_stack);
					error_exit(INTERNAL_ERROR,"Chyba v pravidlech PSA.\n");
				}


				break;

			case N:
				if( ((actual_typ == EOL) || (actual_typ == KEY_WORD_THEN) || (actual_typ == KEY_WORD_DO)) 
					&& (terminal_on_stack_top->typ == DOLAR))
				{
					is_ok = true;
				}
				else
				{
					ListDestroy(expression_list);
					stack_free(&s_stack);
					error_exit(SYNTAX_ERROR,"Ve stacku došlo k porovnání nedefinovaného průniku v tabulce.\n");
				}
				break;	
		}
	}
	while(!is_ok);

	stack_item *Nterm_on_top = stack_top(&s_stack);
	if (Nterm_on_top->typ != NO_TERMINAL)
	{
		ListDestroy(expression_list);
		stack_free(&s_stack);
		error_exit(SYNTAX_ERROR,"Na konci PSA na vrchu zásobníku neplatný neterminál.\n");
	}
	if (Nterm_on_top == NULL)
	{
		ListDestroy(expression_list);
		stack_free(&s_stack);
		error_exit(INTERNAL_ERROR,"Chyba po PSA.\n");
	}

	stack_free(&s_stack);
	freeToken(tmpToken);
	return is_ok;
}


#endif
