#ifndef _MAIN_C
#define _MAIN_C

#include <stdio.h>
#include <stdbool.h>
#include "error.h"
#include "str.h"
#include "scanner.h"
#include "parser.h"
#include "precedence_analysis.h"
#include "expression_list.h"
#include "list.h"
#include "symtable.h"
#include "instruction.h"

int main()
{
	InListPtr list = InListCreate();
	SymTable *symTable = SymTableCreate();
	//SymTable *symTable = SymTableCreate();
	//printf("vpoho pro3lo");
	/*
	VariableAdd(&symTable->varTable->MainRoot, "test1", "0", 3, true);
	VariableAdd(&symTable->varTable->MainRoot, "test0", "2", 5, true);
	VariableAdd(&symTable->varTable->MainRoot, "test4", "1", 4, false);
	VariableAdd(&symTable->varTable->MainRoot, "test3", "2", 5, true);

	FunctionAdd(&symTable->funcTable->MainRoot, "ahoj1", true, true);
	FunctionAdd(&symTable->funcTable->MainRoot, "ahoj0", true, true);
	FunctionAdd(&symTable->funcTable->MainRoot, "ahoj3", true, true);
	FunctionAdd(&symTable->funcTable->MainRoot, "ahoj4", true, true);
	VarNodePtr op = VarSearch(symTable->varTable,"test1");
	VarNodePtr opp = VarSearch(symTable->varTable, "test0");
	I_Move(l, op, opp);
	
	DisposeTree(symTable);
	*/
	/*
	BaseGen(l); // vygenerovani hlavicky

	BuildInGen(l); // vzgenerovani definic built-in

	MainGen(l); //vygenerovani zacatku programu

	//free(symTable);
	return SUCCES;
	*/
	//I_DefVarString(list,"LF@%ahoj1");
	//InListInsertLast(list,"DEFVAR LF@ahoj2");
	//PrintInList(list);

	//I_WriteString(list, "ahoj jmenuju se peta");


	sToken *token = newToken();
	if (prog(token,list,symTable) == false)
	{
		printf("ssss\n");
		return SYNTAX_ERROR;
	}
	freeToken(token);
	DisposeTree(symTable);
	free(list);
	return SUCCES;


}

#endif

