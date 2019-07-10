/*
	Projekt: 
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe 
	
	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		symtable.c
	Varianta:
		Tým 85, varianta 1
*/
#include "symtable.h"
#ifndef SymTable_c
#define SymTable_c





// Vytváøení tabulek, neboli alokace místa a volání inicializace
SymTable* SymTableCreate()
{
	SymTable *symTable = malloc(sizeof(SymTable));
	if (symTable == NULL)
		error_exit(INTERNAL_ERROR, "chyba v alokaci symtable");
	SymTableInit(symTable);
	return symTable;
}

VarTable* VarTableCreate()
{
	VarTable *varTable = malloc(sizeof(VarTable));
	if (varTable == NULL)
		error_exit(INTERNAL_ERROR, "chyba v alokaci varTable");
	VarTableInit(varTable);
	return varTable;

}

FuncTable* FuncTableCreate()
{
	FuncTable *funcTable = malloc(sizeof(FuncTable));
	if (funcTable == NULL)
		error_exit(INTERNAL_ERROR, "chyba v alokaci funcTable");
	FuncTableInit(funcTable);
	return funcTable;
}
// INICIALIZACE tabulek
void SymTableInit(SymTable *symTable)
{
	symTable->varTable = VarTableCreate();
	symTable->funcTable = FuncTableCreate();
}

void VarTableInit(VarTable *varTable) 
{
	varTable->MainRoot = NULL;
}
void FuncTableInit(FuncTable *funcTable)
{
	funcTable->MainRoot = NULL;
}

void VariableAdd(VarNodePtr *RootPtr, char* variable, int typ, bool isLocalFrame)
{
	while (true)
	{
		if (*RootPtr == NULL) //vytvoreni noveho ROOT
		{
			*RootPtr = malloc(sizeof(VarNode));
			(*RootPtr)->variableName = stringCreate(variable);
			(*RootPtr)->Type = typ;
			(*RootPtr)->IsLocalFrame = isLocalFrame;

			(*RootPtr)->lPtr = NULL;
			(*RootPtr)->rPtr = NULL;
			return;
		}
		if (strcmp((*RootPtr)->variableName, variable) == 0)
		{
			(*RootPtr)->Type = typ;
			(*RootPtr)->IsLocalFrame = isLocalFrame;
		}
		if (strcmp((*RootPtr)->variableName, variable) < 0)
			RootPtr = &(*RootPtr)->rPtr;
		else
			RootPtr = &(*RootPtr)->lPtr;
	}

}



void FunctionAdd(FuncNodePtr *RootPtr, char* funcName, bool IsDefined, unsigned paramCount)
{
	while (true)
	{
		if (*RootPtr == NULL) //vytvoreni noveho ROOT
		{
			*RootPtr = malloc(sizeof(VarNode));
			(*RootPtr)->functionName = stringCreate(funcName);

			(*RootPtr)->IsDef = IsDefined;
			(*RootPtr)->paramCount = paramCount;

			(*RootPtr)->lPtr = NULL;
			(*RootPtr)->rPtr = NULL;
			return;
		}
		if (strcmp((*RootPtr)->functionName, funcName) == 0)
			return;
		if (strcmp((*RootPtr)->functionName, funcName) < 0)
			RootPtr = &(*RootPtr)->rPtr;
		else
			RootPtr = &(*RootPtr)->lPtr;
	}


}

void DisposeTree(SymTable *symTable) 
{
	if (symTable == NULL)
		return;
	if (symTable->funcTable != NULL) 
	{
		DisposeFunc(symTable->funcTable->MainRoot);
		symTable->funcTable->MainRoot = NULL;
	}
	if (symTable->varTable != NULL)
	{
		DisposeVar(symTable->varTable->MainRoot);
		symTable->varTable->MainRoot = NULL;
	}

	free(symTable->funcTable);
	free(symTable->varTable);
	free(symTable);
}

void DisposeFunc(FuncNodePtr func) 
{
	if (func != NULL)
	{
		DisposeFunc(func->lPtr);
		DisposeFunc(func->rPtr);
		free(func->functionName);
		free(func);
	}
	
	
}
void DisposeVar(VarNodePtr var) 
{

		if (var != NULL)
		{
			DisposeVar(var->lPtr);
			DisposeVar(var->rPtr);
			free(var->variableName);
			free(var);
		}
}


VarNodePtr VarSearch(VarTable *varTable, char *key)
{
	if ((varTable == NULL) || (key == NULL))
	{
		return NULL;
	}

	VarNodePtr Current;
	int tmp;

	Current = varTable->MainRoot;

	while (Current != NULL)
	{
		tmp = strcmp(Current->variableName, key);
		if (tmp == 0)
			return Current;

		else if (tmp < 0)
			Current = Current->rPtr;

		else
			Current = Current->lPtr;

	}


	return NULL;
}


FuncNodePtr FuncSearch(FuncTable *funcTable, char *key)
{
	if ((funcTable == NULL) || (key == NULL))
	{
		return NULL;
	}

	FuncNodePtr Current;
	int tmp;

	Current = funcTable->MainRoot;

	while (Current != NULL)
	{
		tmp = strcmp(Current->functionName, key);
		if (tmp == 0)
			return Current;

		else if (tmp < 0)
			Current = Current->rPtr;

		else
			Current = Current->lPtr;

	}


	return NULL;
}

#endif // !SymTable_c