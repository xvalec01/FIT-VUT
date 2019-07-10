/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		symtable.h

	Varianta:
		Tým 85, varianta 1
*/
#include "error.h"
#include "str.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef SymTable_h
#define SymTable_h

typedef struct S_VarNode
	VarNode, *VarNodePtr;

struct S_VarNode
{
	char *variableName;	// jméno promìnné 
	int Type;
	bool IsLocalFrame;	// je v lokalnim ramci
	VarNodePtr lPtr;	// ukazatel na levý podstrom
	VarNodePtr rPtr;	// ukazatel na pravý podstrom
};


typedef struct S_FuncNode
	FuncNode, *FuncNodePtr;

struct S_FuncNode
{
	char *functionName;	// jméno funkce 
	bool IsDef;			// je fce definovana
	unsigned paramCount; 
	FuncNodePtr lPtr;	// ukazatel na levý podstrom
	FuncNodePtr rPtr;	//ukazatel na pravý podstrom
};

typedef struct S_FuncTable
{
	FuncNodePtr MainRoot; // ukazatel na prvni prvek vlozeny do stromu
}FuncTable;

typedef struct S_VarTable
{
	VarNodePtr MainRoot; // ukazatel na prvni prvek vlozeny do stromu
}VarTable;

typedef struct S_MainTable 
{
	FuncTable *funcTable;
	VarTable *varTable;
}SymTable;

///////////////////////////////////////////////
/*
 *@brief vztvoøí, naalokuje SymTable a inicializuje ji, vztvoøí i zbyle tabulky
 *@return ukazatel na novou tabulku	
*/
SymTable* SymTableCreate();
FuncTable* FuncTableCreate();
VarTable* VarTableCreate();
///////////////////////////////////////////////
void SymTableInit(SymTable *symTable);

/*
 *@brief Inicializuje tabulku promennych
 *@param ukazatel na Root
 */
void VarTableInit(VarTable *varTable);

/*
 *@brief Inicializuje tabulku funkci
 *@param ukazatel na Root
 */
void FuncTableInit(FuncTable *funcTable);

//////////////////////////////////////////////////
/*
 *@brief prida variable do stromu
 *@param ukazatel na na tabulku
 *@param variablename => nealokovany nazev 
 *@return Navratovy kod uspech/fail
 */
void VariableAdd(VarNodePtr *root, char *variableName, int typ, bool localFrame);

/*
 *@brief prida fci do stromu
 *@param ukazatel na tabulku 
 *@param funcname => nealokovany nazev fce
 *@return Navratovy kod uspech/fail
 */
void FunctionAdd(FuncNodePtr *root, char* funcName, bool IsDefined,unsigned ParamCount);


//////////////////////////////////////////////////////////////

void DisposeTree(SymTable *symTable);

/*
 *@brief znici strom  fci a vsechnu jeho alokaci
 *@param ukazatel na na tabulku
 *@return Navratovy kod uspech/fail
 */
void DisposeFunc(FuncNodePtr func);
/*
 *@brief znici strom variable a vsechnu jeho alokaci
 *@param varTabke ukazatel na tabulku 
 *@return Navratovy kod uspech/fail
 */
void DisposeVar(VarNodePtr var);
//////////////////////////////////////////////////////////////////

/*
 *@brief nalezeni prvku Bin Stromu podle jmena
 *@param varTable ukazatel na tabulku ve ktere hledame
 *@return ukazatel na najdutz prvek nebo NULL pri nenajduti
 */
VarNodePtr VarSearch(VarTable *varTable, char *key);

/*
 *@brief nalezeni prvku Bin Stromu podle jmena
 *@param FuncTable ukazatel na tabulku ve ktere hledame
 *@return ukazatel na najdutz prvek nebo NULL pri nenajduti
 */
FuncNodePtr FuncSearch(FuncTable *funcTable, char *key);

#endif // !SymTable_h
