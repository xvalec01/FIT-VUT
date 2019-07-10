/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastn�ho p�eklada�e

	Autor:
		Petr Urb�nek - Xurban 70

	Soubor:
		instruction.h

	Varianta:
		T�m 85, varianta 1
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
 *@brief P�i�azen� hodnoty do prom�nn�.
 *
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
 */
int I_Move(InListPtr lp, VarNodePtr result, VarNodePtr op2);
/**
 *@brief p�i�azen� hodnoty ve stringu do rpom�nn�
 *
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
 */
int I_MoveS(InListPtr l, VarNodePtr result, char* str1);
/**
 *@brief P�i�azen� hodnoty ve stringu do prom�nn� ve stringu.
 *
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
 */
int I_MoveSS(InListPtr l, char *result, char* str1);
/*
 *@brief Vytvo�� nov� do�asn� r�mec
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_CreateFrame(InListPtr lp);
/*
 *@brief Pushne na stack do�asn� r�mec TF=NULL LF=top(stack)
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_PushFrame(InListPtr lp);
/*
 *@brief Vezme ze stacku a ulo�� jako TF
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_PopFrame(InListPtr lp);
/*
 *@brief Definuj novou prom�nnou v r�mci
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/

int I_DefVarString(InListPtr lp, char* string);
/*
 *@brief Definuj novou prom�nnou v r�mci
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_DefVarNode(InListPtr lp, VarNodePtr var);
/*
 *@brief Skok na n�v�t� s podporou n�vratu
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Call(InListPtr lp, FuncNodePtr function);
/*
 *@brief N�vrat na pozici ulo�enou instrukc� CALL
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Return(InListPtr lp);

/*
 *@brief Vlo�� hodnotu na vrchol datov�ho z�sobn�ku
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_PushSString(InListPtr lp, char* variable);
/*
 *@brief Vlo�� hodnotu z prom�nn� na vrchol datov�ho z�sobn�ku
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_PushSNode(InListPtr lp, VarNodePtr var);
/*
 *@brief Vyjmy hodnotu z vrcholu datov�ho z�sobn�ku a vlo� do prom�nn�
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_PopSString(InListPtr lp, char *variable);
int I_PopSNode(InListPtr lp, VarNodePtr var);
/*
 *@brief Vymaz�n� obsahu cel�ho datov�ho z�sobn�ku
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_ClearS(InListPtr lp);

/*
 *@brief Sou�et dvou ��seln�ch hodnot
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Add(InListPtr lp,VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Ode��t�n� dvou ��seln�ch hodnot .. op1-op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Sub(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief N�soben� dvou ��seln�ch hodnot
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Mul(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief D�len� dvou ��seln�ch hodnot .. op1/op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Div(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief d�len� se znam�nkem dvou ��seln�ch hodnot .. op1/op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_IDiv(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);


int I_AddS(InListPtr lp);
int I_SubS(InListPtr lp);
int I_MulS(InListPtr lp);
int I_DivS(InListPtr lp);
int I_IDivS(InListPtr lp);

/*
 *@brief vyhodnot� je-li men�� .. op1 < op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_LT(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief vyhodnot� je-li v�t�� .. op1 > op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_GT(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief vyhodnot� jsou-li rovno .. op1 == op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_EQ(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);

int I_LTS(InListPtr lp);
int I_GTS(InListPtr lp);
int I_EQS(InListPtr lp);

/*
 *@brief logick� a-z�rove� .. op1 & op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_And(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief logick� nebo .. op1 | op2
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Or(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief logick� negace .. !op1
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Not(InListPtr lp, VarNodePtr result, VarNodePtr op1);

int I_AndS(InListPtr lp);
int I_OrS(InListPtr lp);
int I_NotS(InListPtr lp);

/*
 *@brief P�evod celo��seln� hodnoty na desetinnou
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Int2Float(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief P�evod desetinn� hodnoty na celo��selnou (oseknut�)
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Float2Int(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief P�evod cel�ho ��sla na znak
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Int2Char(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief Ordin�ln� hodnota znaku na pozici
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Str2Int(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);

int I_Int2FloatS(InListPtr lp);
int I_Float2IntS(InListPtr lp);
int I_Int2CharS(InListPtr lp);
int I_Str2IntS(InListPtr lp);

/*
 *@brief Na�ten� hodnoty intu ze standardn�ho vstupu
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
void I_InputI(InListPtr lp);
/*
 *@brief Na�ten� hodnoty Floatu ze standardn�ho vstupu
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
void I_InputF(InListPtr lp);
/*
 *@brief Na�ten� hodnoty stringu ze standardn�ho vstupu
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
void I_InputS(InListPtr lp);

/*
 *@brief Na�ten� hodnoty ze standardn�ho vstupu
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Read(InListPtr lp, VarNodePtr result, char* Type);
/*
 *@brief V�pis hodnoty na standardn� v�stup
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Write(InListPtr lp, VarNodePtr toPrint);
//pro manualni nazev stringu, norm�ln� nap�e� co chce� aby vyb�hlo nav�stup, t�eba ahoj ja jsem peta
int I_WriteString(InListPtr l, char* str);


/*
 *@brief Konkatenace dvou �et�zc�
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
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
 *@brief Zjisti d�lku �et�zce
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_StrLen(InListPtr lp, VarNodePtr result, VarNodePtr op1);
/*
 *@brief Vra� znak �et�zce
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_GetChar(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Zm�� znak �et�zce
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_SetChar(InListPtr lp, VarNodePtr result, VarNodePtr op1, VarNodePtr op2);

/*
 *@brief Zjisti typ dan�ho symbolu
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Type(InListPtr lp, VarNodePtr result, VarNodePtr op1);

/*
 *@brief Definice n�v�t�
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Label(InListPtr lp, char* label);
/*
 *@brief Nepodm�n�n� skok na n�v�t�
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_Jump(InListPtr lp, char* label);
/*
 *@brief Podm�n�n� skok na n�v�t� p�i rovnosti
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_JumpIfEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);
//int I_JumpIfEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);
/*
 *@brief Podm�n�n� skok na n�v�t� p�i nerovnosti
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
int I_JumpIfNEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);

//int I_JumpIfNEq(InListPtr lp, char* label, VarNodePtr op1, VarNodePtr op2);
int I_JumpIfEqS(InListPtr lp, char* label);
int I_JumpIfNEqS(InListPtr lp, char* label);
/*
 *@brief Ukon�en� interpretace s n�vratov�m k�dem
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
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
 *@brief V�pis stavu interpretu na stderr
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
//int Break();
/*
 *@brief V�pis hodnoty na stderr
 *@return N�vratov� k�d popisuj�c� chybu/�sp�ch
*/
//int DPrint();



#endif // !Instruction_h
