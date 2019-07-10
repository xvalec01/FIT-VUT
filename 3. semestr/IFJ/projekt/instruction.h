/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		instruction.h

	Varianta:
		Tım 85, varianta 1
*/
#include "list.h"
#include "error.h"
#include "symtable.h"
#include "str.h"
#include <stdio.h>
#include <string.h>
 
#ifndef Instruction_h
#define Instruction_h


typedef enum E_Datatypes
{
	INT = 111,
	FLOAT = 222,
	SSTRING = 333,
}dataTypes;


//typedef struct S_Instruction 
//{
//	char *content;
//	dataTypes dataType;
//	char *op1;
//	char *op2;
//	char *result;
//}Instr;


/////////////////////////// POMOCNE FUNKCE ///////////////////////////////////////////////////
char* VarToStr(VarNodePtr var);
char *NumToStr(VarNodePtr var);
char* FuncToStr(FuncNodePtr func);
char* GetType(int val);


//////////////////////////////////////////////////////////////////////////////////////////////

/**
 *@brief Pøiøazení hodnoty do promìnné.
 *
 *@return Návratovı kód popisující chybu/úspìch
 */
int I_Move(InListPtr lp, VarNodePtr result, VarNodePtr op2);
/**
 *@brief pøiøazení hodnoty ve stringu do rpomìnné
 *
 *@return Návratovı kód popisující chybu/úspìch
 */
int I_MoveS(InListPtr l, VarNodePtr result, char* str1);
/**
 *@brief Pøiøazení hodnoty ve stringu do promìnné ve stringu.
 *
 *@return Návratovı kód popisující chybu/úspìch
 */
int I_MoveSS(InListPtr l, char *result, char* str1);
/*
 *@brief Vytvoøí novı doèasnı rámec
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_CreateFrame(InListPtr lp);
/*
 *@brief Pushne na stack doèasnı rámec TF=NULL LF=top(stack)
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_PushFrame(InListPtr lp);
/*
 *@brief Vezme ze stacku a uloí jako TF
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_PopFrame(InListPtr lp);
/*
 *@brief Definuj novou promìnnou v rámci
 *@return Návratovı kód popisující chybu/úspìch
*/

int I_DefVarString(InListPtr lp, char* string);
/*
 *@brief Definuj novou promìnnou v rámci
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_DefVarNode(InListPtr lp, VarNodePtr var);
/*
 *@brief Skok na návìští s podporou návratu
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Call(InListPtr lp, FuncNodePtr function);
/*
 *@brief Návrat na pozici uloenou instrukcí CALL
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Return(InListPtr lp);

/*
 *@brief Vloí hodnotu na vrchol datového zásobníku
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_PushSString(InListPtr lp, char* variable);
/*
 *@brief Vloí hodnotu z promìnné na vrchol datového zásobníku
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_PushSNode(InListPtr lp, VarNodePtr var);
/*
 *@brief Vyjmy hodnotu z vrcholu datového zásobníku a vlo do promìnné
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_PopSString(InListPtr lp, char *variable);
int I_PopSNode(InListPtr lp, VarNodePtr var);
/*
 *@brief Vymazání obsahu celého datového zásobníku
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_ClearS(InListPtr lp);

/*
 *@brief Souèet dvou èíselnıch hodnot
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Add(InListPtr lp,VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Odeèítání dvou èíselnıch hodnot .. op1-op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Sub(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Násobení dvou èíselnıch hodnot
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Mul(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Dìlení dvou èíselnıch hodnot .. op1/op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Div(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief dìlení se znaménkem dvou èíselnıch hodnot .. op1/op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_IDiv(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);


int I_AddS(InListPtr lp);
int I_SubS(InListPtr lp);
int I_MulS(InListPtr lp);
int I_DivS(InListPtr lp);
int I_IDivS(InListPtr lp);

/*
 *@brief vyhodnotí je-li menší .. op1 < op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_LT(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief vyhodnotí je-li vìtší .. op1 > op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_GT(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief vyhodnotí jsou-li rovno .. op1 == op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_EQ(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);

int I_LTS(InListPtr lp);
int I_GTS(InListPtr lp);
int I_EQS(InListPtr lp);

/*
 *@brief logické a-zároveò .. op1 & op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_And(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief logické nebo .. op1 | op2
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Or(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief logická negace .. !op1
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Not(InListPtr lp, VarNodePtr result, VarNodePtr op1);

int I_AndS(InListPtr lp);
int I_OrS(InListPtr lp);
int I_NotS(InListPtr lp);

/*
 *@brief Pøevod celoèíselné hodnoty na desetinnou
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Int2Float(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief Pøevod desetinné hodnoty na celoèíselnou (oseknutí)
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Float2Int(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief Pøevod celého èísla na znak
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Int2Char(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief Ordinální hodnota znaku na pozici
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Str2Int(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);

int I_Int2FloatS(InListPtr lp);
int I_Float2IntS(InListPtr lp);
int I_Int2CharS(InListPtr lp);
int I_Str2IntS(InListPtr lp);

/*
 *@brief Naètení hodnoty intu ze standardního vstupu
 *@return Návratovı kód popisující chybu/úspìch
*/
void I_InputI(InListPtr lp);
/*
 *@brief Naètení hodnoty Floatu ze standardního vstupu
 *@return Návratovı kód popisující chybu/úspìch
*/
void I_InputF(InListPtr lp);
/*
 *@brief Naètení hodnoty stringu ze standardního vstupu
 *@return Návratovı kód popisující chybu/úspìch
*/
void I_InputS(InListPtr lp);

/*
 *@brief Naètení hodnoty ze standardního vstupu
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Read(InListPtr lp, VarNodePtr result, char* Type);
/*
 *@brief Vıpis hodnoty na standardní vıstup
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Write(InListPtr lp, VarNodePtr toPrint);
//pro manualni nazev stringu, normálnì napíšeš co chceš aby vybìhlo navıstup, tøeba ahoj ja jsem peta
int I_WriteString(InListPtr l, char* str);


/*
 *@brief Konkatenace dvou øetìzcù
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Concat(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
//instrukce concat pro string a promenou
int I_ConCatNS(InListPtr l, VarNodePtr result, VarNodePtr op1, char* s);
//instrukce concat pro 2 stringy
int I_ConCatSS(InListPtr l, VarNodePtr result, char *s1, char* s2);
// pro  3 stringy
int I_ConCatSSS(InListPtr l, char* s0, char* s1, char* s2);
//interni fce pro zmenu charu ve stringu v zadanych hodnotach na format \xyz
char *strAscii(char * str);

/*
 *@brief Zjisti délku øetìzce
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_StrLen(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief Vra znak øetìzce
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_GetChar(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Zmìò znak øetìzce
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_SetChar(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);

/*
 *@brief Zjisti typ daného symbolu
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Type(InListPtr lp, VarNodePtr result, VarNodePtr op1);

/*
 *@brief Definice návìští
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Label(InListPtr lp, char* label);
/*
 *@brief Nepodmínìnı skok na návìští
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Jump(InListPtr lp, char* label);
/*
 *@brief Podmínìnı skok na návìští pøi rovnosti
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_JumpIfEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);
//int I_JumpIfEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Podmínìnı skok na návìští pøi nerovnosti
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_JumpIfNEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);

//int I_JumpIfNEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);
int I_JumpIfEqS(InListPtr lp, char* label);
int I_JumpIfNEqS(InListPtr lp, char* label);
/*
 *@brief Ukonèení interpretace s návratovım kódem
 *@return Návratovı kód popisující chybu/úspìch
*/
int I_Exit(InListPtr lp, VarNodePtr number);


void I_Length(InListPtr l);
/*
 *@brief napise hlavicku Instrukci
*/
void BaseGen(InListPtr l);

void BuildInGen(InListPtr l);
void MainGen(InListPtr l);



/*
 *@brief Vıpis stavu interpretu na stderr
 *@return Návratovı kód popisující chybu/úspìch
*/
//int Break();
/*
 *@brief Vıpis hodnoty na stderr
 *@return Návratovı kód popisující chybu/úspìch
*/
//int DPrint();



#endif // !Instruction_h
