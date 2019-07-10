/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		instruction.c

	Varianta:
		Tým 85, varianta 1
*/

#include "instruction.h"


#ifndef Instruction_c
#define Instruction_c

///*
//*@brief z promene 'ahoj' vezme hodnotu a yapise jako 'TYPE@NUMBER '
//*/
//char *NumToStr(VarNodePtr var) 
//{
//	char *variable = "";
//	char *type = GetType(var->Type);
//
//	variable = strConCat(variable, type); //int//
//	variable = strConCat(variable, "@"); //int@//
//	variable = strConCat(variable, var->value); //int@10//
//	variable = strConCat(variable, " "); //int@10 //
//
//
//	return variable;
//}



/*
*@brief z promene jmena 'ahoj' udela 'LF@ahoj '
*/
char* VarToStr(VarNodePtr var)  
{
	char *variable = "";
	if (var->IsLocalFrame)
		variable = strConCat(variable, "LF@");//LF@//
	else
		variable = strConCat(variable, "TF@");//TF@//

	variable = strConCat(variable, var->variableName);//TF@NAME//
	variable = strConCat(variable," ");//TF@NAME //

	return variable;
}


	/**
	 * Converts int to data type string, returns pointer to string
	 */
	char* GetType(int val)
	{
		char* type = "";

		if (val == 7)
			type = "int";

		else if (val == 8)
			type = "float";

		else if (val == 5)
			type = "string";

		else
			error_exit(INTERNAL_ERROR, "");
		

		return type;
	}





int I_Move(InListPtr l, VarNodePtr result, VarNodePtr op1) 
{
	char *var = "";
		var = VarToStr(op1);
	char *res = "";
	res = VarToStr(result);
	char *instr = "MOVE ";
	instr = strConCat(instr, var);
	instr = strConCat(instr, res);

	char *instruction = stringCreate(instr);

	InListInsertLast(l,instruction);


	return SUCCES;
}
int I_MoveS(InListPtr l, VarNodePtr result, char* str1)
{
	char *var = "";
	var = strAscii(str1);
	char *res = "";
	res = VarToStr(result);
	char *instr = "MOVE ";
	instr = strConCat(instr, var);
	instr = strConCat(instr, res);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	free(var);
	free(res);
	free(instr);
	free(instruction);


	return SUCCES;
}
int I_MoveSS(InListPtr l, char *result, char* str1)
{

		char *var = "";
		var = strAscii(str1);
		char *res = "";
		res = strAscii(result);
		char *instr = "MOVE ";

		instr = strConCat(instr, var);
		instr = strConCat(instr, res);

		char *instruction = stringCreate(instr);
		InListInsertLast(l, instruction);

		free(var);
		free(res);
		free(instr);
		free(instruction);


		return SUCCES;
	
}

int I_CreateFrame(InListPtr l)
{
	char *instr = "CREATEFRAME";

	InListInsertLast(l, instr);


	return SUCCES;
}
int I_PushFrame(InListPtr l)
{
	char *instr = "PUSHFRAME";

	InListInsertLast(l, instr);

	return SUCCES;
}
int I_PopFrame(InListPtr l)
{
	char *instr = "POPFRAME";
	InListInsertLast(l, instr);

	return SUCCES;
}
int I_DefVarNode(InListPtr l, VarNodePtr var)
{
	char *variable = "";
	variable = VarToStr(var); //misto v pameti, musi byt var

	char *instr = "DEFVAR "; // DEFVAR
	instr = strConCat(instr, variable); // DEFVAR LF@NAME

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	free(instruction);
	free(instr);
	free(variable);



	return SUCCES;
}
int I_DefVarString(InListPtr l, char *var) 
{
	char *instr = "DEFVAR "; // DEFVAR
	instr = strConCat(instr, var); // DEFVAR LF@NAME

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);
	free(instr);
	free(instruction);
	return SUCCES;
}

int I_Call(InListPtr l, FuncNodePtr function)
{
	char *instr = "CALL ";
	instr = strConCat(instr, instr);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);
	free(instr);
	free(instruction);



	return SUCCES;
}

int I_Return(InListPtr l)
{
	char *instr = "RETURN ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);
	free(instruction);

	return SUCCES;
}


int I_PushSString(InListPtr l, char *s)
{
	char *instr = "PUSHS ";
	char *var = strAscii(s);
	instr = strConCat(instr, var);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);
	free(instr);
	free(instruction);
	free(var);

	return SUCCES;
}
int I_PushSNode(InListPtr l, VarNodePtr var)
{
	char *instr = "PUSHS ";
	char* vari = VarToStr(var);
	instr = strConCat(instr, vari);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);
	free(instr);
	free(instruction);

	return SUCCES;
}

int I_PopSString(InListPtr l, char *var)
{
	char *instr = "POPS ";
	instr = strConCat(instr, var);

	char *instruction = stringCreate(instr);
	free(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_PopSNode(InListPtr l, VarNodePtr var)
{
	char *instr = "POPS ";
	char* vari = VarToStr(var);
	instr = strConCat(instr, vari);

	char *instruction = stringCreate(instr);
	free(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}

int I_ClearS(InListPtr l)
{
	char *instr = "CLEARS ";
	char *instruction = stringCreate(instr);


	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_Add(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// ADD RESULT A B => result=a+b

	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);


	char *res = VarToStr(result);

	char *instr = "ADD ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_Sub(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// SUB RESULT A B => result=a-b

	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);
	char *res = VarToStr(result);

	char *instr = "SUB ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_Mul(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// MUL RESULT A B => result=a*b

	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);


	char *res = VarToStr(result);

	char *instr = "MUL ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_Div(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// DIV RESULT A B => result=a/b

	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);

	char *res = VarToStr(result);

	char *instr = "DIV ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_IDiv(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// IDIV RESULT A B => result=a/b

	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);


	char *res = VarToStr(result);

	char *instr = "IDIV ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}


int I_AddS(InListPtr l)
{
	char *instr = "ADDS ";
	char *instruction = stringCreate(instr);


	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_SubS(InListPtr l)
{
	char *instr = "SUBS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_MulS(InListPtr l)
{
	char *instr = "MULS ";
	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_DivS(InListPtr l)
{
	char *instr = "DIVS ";
	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_IDivS(InListPtr l)
{
	char *instr = "IDIVS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}

int I_LT(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2) 
{
	// LT RESULT A B => result = a<b
	char* symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);

	char *res = VarToStr(result);

	char *instr = "LT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_GT(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// LT RESULT A B => result = a>b
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";

		symb2 = VarToStr(op2);

	char *res = VarToStr(result);

	char *instr = "GT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_EQ(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// LT RESULT A B => result = a==b
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);

	char *res = VarToStr(result);

	char *instr = "EQ ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}

int I_LTS(InListPtr l)
{
	char *instr = "LTS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_GTS(InListPtr l)
{
	char *instr = "GTS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_EQS(InListPtr l)
{
	char *instr = "EQS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}



int I_And(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// AND RESULT A B => result = a&&b
	char *symb1 = VarToStr(op1);
	char *symb2 = VarToStr(op2);
	char *res = VarToStr(result);

	char *instr = "AND ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_Or(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// OR RESULT A B => result = a||b
	char *symb1 = VarToStr(op1);
	char *symb2 = VarToStr(op2);
	char *res = VarToStr(result);

	char *instr = "OR ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);
	return SUCCES;
}
int I_Not(InListPtr l, VarNodePtr result, VarNodePtr op1)
{
	// NOT RESULT A => result = !a
	char *symb1 = VarToStr(op1);
	char *res = VarToStr(result);

	char *instr = "NOT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_AndS(InListPtr l)
{
	char *instr = "ANDS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_OrS(InListPtr l)
{
	char *instr = "ORS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_NotS(InListPtr l)
{
	char *instr = "NOTS ";
	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}

int I_Int2Float(InListPtr l, VarNodePtr result, VarNodePtr op1)
{
	// INT2FLOAT RESULT A => result = int2float

	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *res = VarToStr(result);

	char *instr = "INT2FLOAT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_Float2Int(InListPtr l, VarNodePtr result, VarNodePtr op1)
{
	// FLOAT2INT RESULT A => result = float2int

	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *res = VarToStr(result);

	char *instr = "FLOAT2INT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);		

	return SUCCES;
}
int I_Int2Char(InListPtr l, VarNodePtr result, VarNodePtr op1)
{
	// INT2CHAR RESULT A => result = int2Char

	char *symb1 = "";
		symb1 = VarToStr(op1);
	
	char *res = VarToStr(result);

	char *instr = "INT2CHAR ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_Str2Int(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	// STR2INT RESULT A B => STR2INT

	char *symb1 = ""; //retezec
		symb1 = VarToStr(op1);

	char *symb2 = ""; //pozice
		symb2 = VarToStr(op2);

	char *res = VarToStr(result);

	char *instr = "STRI2INT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}

int I_Int2FloatS(InListPtr l)
{
	char *instr = "INT2FLOATS ";

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_Float2IntS(InListPtr l)
{
	char *instr = "FLOAT2INTS ";

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_Int2CharS(InListPtr l)
{
	char *instr = "INT2CHARS ";

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}
int I_Str2IntS(InListPtr l)
{
	char *instr = "STRI2INTS ";

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}


int I_Read(InListPtr l, VarNodePtr result,char* type)
{
	char *res = VarToStr(result);
	char *instr = "READ ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, type);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_Write(InListPtr l, VarNodePtr op1)
{
	char *symb = "";
		symb = VarToStr(op1);


	char *instr = "WRITE ";
	instr = strConCat(instr, symb); //WRITE GF@ahoj

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);
	free(instr);
	free(instruction);

	return SUCCES;
}
/*
 *@brief zmena mezer a podobnych znaky ve stringu na format \xyz
 *pouzita jen lokalne
 *@return alokovany string 
*/
char *strAscii(char * str) 
{
	int len = strlen(str);
	char *s = calloc(4 * len, sizeof(char)); //nemuze presahnout 4x tak velkou alokaci
	char *tmpS = calloc(2, sizeof(char));

	for (unsigned i = 0, j = 0; i < len; i++, j++)
	{
		if (((str[i] >= 0) && (str[i] <= 32)) || str[i] == 35 || str[i] == 92)
		{
			sprintf(tmpS, "%d", str[i]);
			s[j] = 92; // backslash '\' .. visual studio made problems pro hnidopichy bycg to zapsal '\\'
			s[++j] = '0';

			for (int p = 0; p < 2; p++)
			{
				j++;
				s[j] = tmpS[p];
			}

		}
		else
			s[j] = str[i]; // normalni pismena
	}
	free(tmpS);
	return s;
}

int I_WriteString(InListPtr l, char* str)
{

	char *s = strAscii(str);
	char *instr = "WRITE string@";
	instr = strConCat(instr, s); //WRITE GF@ahoj

	InListInsertLast(l, instr);
	free(s);
	free(instr);

	return SUCCES;
}

int I_Concat(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);

	char *res = VarToStr(result);

	char *instr = "CONCAT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);
	free(instruction);
	free(instr);
	free(symb1);
	free(symb2);
	free(res);


	return SUCCES;
}
int I_ConCatNS(InListPtr l, VarNodePtr result, VarNodePtr op1, char* str)
{
	char *res = VarToStr(result);

	char *symb1 = "";
	symb1 = VarToStr(op1);

	char *s = strAscii(str);
	char *instr = "CONCAT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, s);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	free(instruction);
	free(instr);
	free(symb1);
	free(res);
	free(s);


	return SUCCES;
}
int I_ConCatSS(InListPtr l, VarNodePtr result, char* s1, char* s2)
{

	char *res = VarToStr(result);
	char *str1 = strAscii(s1);
	char *str2 = strAscii(s2);

	char *instr = "CONCAT ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, str1);
	instr = strConCat(instr, str2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	free(str1);
	free(str2);
	free(instr);
	free(instruction);
	free(res);


	return SUCCES;
}

int I_ConCatSSS(InListPtr l, char* s0, char* s1, char* s2)
{
	

	char *instr = "CONCAT ";

	char *str1 = strAscii(s1);
	char *str2 = strAscii(s2);

	instr = strConCat(instr, s0);
	instr = strConCat(instr, str1);
	instr = strConCat(instr, str2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	free(str1);
	free(str2);
	free(instr);
	free(instruction);


	return SUCCES;
}

int I_StrLen(InListPtr l, VarNodePtr result, VarNodePtr op1)
{
	// STRLEN RESULT A => result = delka
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *res = VarToStr(result);

	char *instr = "STRLEN ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_GetChar(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	char *symb1 = ""; //string
		symb1 = VarToStr(op1);

	char *symb2 = ""; //pozice
		symb2 = VarToStr(op2);

	char *res = VarToStr(result);

	char *instr = "GETCHAR ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_SetChar(InListPtr l, VarNodePtr result, VarNodePtr op1, VarNodePtr op2)
{
	char *symb1 = ""; //string
		symb1 = VarToStr(op1);

	char *symb2 = ""; //pozice
		symb2 = VarToStr(op2);
	char *res = VarToStr(result);

	char *instr = "GETCHAR ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}
int I_Type(InListPtr l, VarNodePtr result, VarNodePtr op1)
{
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *res = VarToStr(result);

	char *instr = "TYPE ";
	instr = strConCat(instr, res);
	instr = strConCat(instr, symb1);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}

int I_Label(InListPtr l, char *label)
{
	char *instr = "LABEL ";
	instr = strConCat(instr, label);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);
	free(instr);
	free(instruction);

	return SUCCES;
}
int I_Jump(InListPtr l, char *label)
{
	char *instr = "JUMP ";
	instr = strConCat(instr, label);
	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);
	free(instr);
	free(instruction);


	return SUCCES;
}
int I_JumpIfEq(InListPtr l, char *label, VarNodePtr op1, VarNodePtr op2)
{
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);

	char *instr = "JUMPIFEQ ";
	instr = strConCat(instr, label);
	instr = strConCat(instr, " ");
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);
	return SUCCES;
}

int I_JumpIfNEq(InListPtr l, char *label, VarNodePtr op1, VarNodePtr op2)
{
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *symb2 = "";
		symb2 = VarToStr(op2);

	char *instr = "JUMPIFNEQ ";
	instr = strConCat(instr, label);
	instr = strConCat(instr, " ");
	instr = strConCat(instr, symb1);
	instr = strConCat(instr, symb2);

	char *instruction = stringCreate(instr);

	InListInsertLast(l, instruction);


	return SUCCES;
}

int I_JumpIfEqS(InListPtr l, char *label)
{
	char *instr = "JUMPIFEQS ";
	instr = strConCat(instr, label);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}

int I_JumpIfNEqS(InListPtr l, char *label)
{
	char *instr = "JUMPIFNEQS ";
	instr = strConCat(instr, label);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);

	return SUCCES;
}

int I_Exit(InListPtr l, VarNodePtr op1) 
{
	char *symb1 = "";
		symb1 = VarToStr(op1);

	char *instr = "EXIT ";
	instr = strConCat(instr, symb1);

	char *instruction = stringCreate(instr);
	InListInsertLast(l, instruction);


	return SUCCES;
}

/*
 *@brief 
 *@return vzhodi hodnotu promenne na stack
*/
void I_InputS(InListPtr l)
{
	I_Label(l, "$%inputs");
	I_PushFrame(l);
	InListInsertLast(l, "DEFVAR LF@%ret");
	InListInsertLast(l, "READ LF@%ret string");
	I_PushSString(l, "LF@%ret");
	I_PopFrame(l);
	I_Return(l);
}

void I_InputF(InListPtr l)
{
	I_Label(l, "$%inputf");
	I_PushFrame(l);
	InListInsertLast(l, "DEFVAR LF@%ret");
	InListInsertLast(l, "READ LF@%ret float");
	I_PushSString(l, "LF@%ret");
	I_PopFrame(l);
	I_Return(l);
}
void I_InputI(InListPtr l)
{
	I_Label(l, "$%inputi");
	I_PushFrame(l);
	InListInsertLast(l, "DEFVAR LF@%ret");
	InListInsertLast(l, "READ LF@%ret int");
	I_PushSString(l, "LF@%ret");
	I_PopFrame(l);
	I_Return(l);
}
void I_Print(InListPtr l)
{
	I_Label(l, "$%print");
	I_PushFrame(l);
	InListInsertLast(l, "WRITE LF@%0");
	I_PopFrame(l);
	I_Return(l);

}

/*
 *@brief nejdrive se pusnou parametry fce
 * nadefinuje se navrat hodnota a a popne se do help3
 * ulozi delku na stack
*/
void I_Length(InListPtr l)
{
	InListInsertLast(l, "# LENGTH");												
	I_Label(l,"$%length");
	I_PushFrame(l);
	InListInsertLast(l, "DEFVAR LF@%ret");
	I_PopSString(l, "GF@%help1"); //do help 3 si popnu pushlz parametr
	InListInsertLast(l, "STRLEN LF@%ret GF@%help1");

	I_PushSString(l, "LF@%ret"); //uloz delku stringu na stack
	I_PopFrame(l);
	I_Return(l);
}

/*
 *@brief nejdrive se pusnou parametry fce odzadu. Substr(s,i,n)
 * nejdrive push 'n', pak 'i' a pak 's'
 * narvatovka na stacku
*/
void I_SubStr(InListPtr l)
{
	InListInsertLast(l, "# SUBSTR");
	//pred volanim pushnout parametry v poradi N I S
	I_Label(l, "$%substr");
	InListInsertLast(l, "DEFVAR GF@%par1");
	InListInsertLast(l, "DEFVAR GF@%par2");
	InListInsertLast(l, "DEFVAR GF@%par3");
	I_PopSString(l, "GF@%par3"); //N
	I_PopSString(l, "GF@%par2"); //I
	I_PopSString(l, "GF@%par1"); //S

	I_PushFrame(l);
	InListInsertLast(l, "DEFVAR LF@%ret"); //navratova hodnota
	InListInsertLast(l, "MOVE LF@%ret nil@nil"); // init na nil
	InListInsertLast(l, "DEFVAR LF@%range"); //pro podminku rozsahu
	I_CreateFrame(l);
	I_PushSString(l, "GF@%par1"); //pushnu string pro fci len
	InListInsertLast(l, "Call $%length");
	I_PopSString(l, "LF@%range"); //vysledek len v range

	InListInsertLast(l, "DEFVAR LF@cond");
	InListInsertLast(l, "LT LF@%cond LF@%range int@0"); 
	InListInsertLast(l, "JUMPIFEQ $%SubstrNil LF@%range bool@true"); //range <0 jump na substrnil
	InListInsertLast(l, "EQ LF@%cond LF@%range int@0");
	InListInsertLast(l, "JUMPIFEQ $%SubstrNil LF@%range bool@true"); //range == 0 jump na substrnill

	InListInsertLast(l, "LT LF@%cond GF@%par2 int@0"); //2. param substru I
	InListInsertLast(l, "JUMPIFEQ $%SubstrNil LF@%cond bool@true"); //pokud je index <1
	InListInsertLast(l, "GT LF@%cond GF@%par2 LF@%range");
	InListInsertLast(l, "JUMPIFEQ $%SubstrNil LF@%cond bool@true");

	InListInsertLast(l, "LT LF@%cond GF@%par3 int@0"); // N<0
	InListInsertLast(l, "JUMPIFEQ $%SubstrNil LF@%cond bool@true");

	InListInsertLast(l, "DEFVAR LF@%max");
	InListInsertLast(l, "MOVE LF@%max LF@%range");
	InListInsertLast(l, "SUB LF@%max LF@%max GF@%par1"); // len - pos 
	InListInsertLast(l, "GT LF@%cond LF@%2 LF@%max"); // n>len-pos 
	InListInsertLast(l, "JUMPIFEQ $%SubstrEdit LF@%cond bool@true"); //edit
	InListInsertLast(l, "JUMP $%SubstrContinue"); //continue
	I_Label(l, "$%SubstrEdit");
	InListInsertLast(l, "MOVE GF@%par3 LF@%max"); // do delky dej len-pos
	I_Label(l, "$%SubstrContinue");
	InListInsertLast(l, "DEFVAR LF@index");
	InListInsertLast(l, "MOVE LF@index GF@%par2"); // do prom uloz pos
	I_DefVarString(l, "LF@%char"); //pokus I_DEFVarString() :D
	InListInsertLast(l, "DEFVAR LF@%loopCond");

	I_Label(l, "$%SubstrLoop");
	InListInsertLast(l, "GETCHAR LF@%char GF@%par1 LF@%index"); //ve stringu na pozici ziskej char
	InListInsertLast(l, "CONCAT LF@%ret LF@%ret LF@char"); // KONAKETENACE VYSTUPU
	InListInsertLast(l, "ADD LF@%index LF@%index int@1");//inkrementace pos
	InListInsertLast(l, "SUB GF@%par2 GF@%par2 int@1"); // dekrementace n (delky)
	InListInsertLast(l, "GT LF@%loopCond GF@%par2 int@0"); // kontrola konce loop
	InListInsertLast(l, "JUMPIFEQ $%SubstrLoop LF@%loopCond bool@true");
	InListInsertLast(l, "JUMP $%SubstrEnd");

	I_Label(l, "$%SubstrNil");
	I_PushSString(l, "nil@nil");
	I_Label(l,"$%SubstrEnd");
	I_PushSString(l, "LF@%ret"); //pushnut na stack return value
	I_PopFrame(l);
	I_Return(l);





}
/*
 *@brief nejdrive se pusnou parametry fce odzadu. ORD(s,i)
 * nejdrive push 'i', pak 's'
 * nadefinuje se navrat hodnota
*/void I_Ord(InListPtr l)
{
	InListInsertLast(l, "# ORD(s,i)");
	I_Label(l, "$%Ord");
	I_PushFrame(l);
	I_PopSString(l, "GF@%help2"); //do help2 dam index
	I_PopSString(l, "GF@%help1"); //do help1 dam string
	InListInsertLast(l, "DEFVAR LF@%ret"); //pro navratovou
	InListInsertLast(l, "MOVE LF@%ret int@0");
	InListInsertLast(l, "DEFVAR LF@%range");
	InListInsertLast(l, "LT LF@%range GF@%help2 int@0"); // index je druhy param co se pushne i < 0
	InListInsertLast(l, "JUMPIFEQ $%%OrdRet LF@%range bool@true");
	InListInsertLast(l, "DEFVAR LF@%len"); //delka stringu										
	I_CreateFrame(l);
	I_PushSString(l, "GF@%help1"); // pushnu string pro len
	InListInsertLast(l, "CALL $%length");
	I_PopSString(l, "LF@%length"); //popnu si vysledek do lf len
	InListInsertLast(l, "SUB LF@%Length	LF@%Length int@1");
	InListInsertLast(l, "GT LF@%range GF@%help2 LF@%Length"); //index vetsi nez delka retezce
	InListInsertLast(l, "JUMPIFEQ $%%OrdRet LF@%range bool@true");
	InListInsertLast(l, "STRI2INT LF@%ret GF@%help1 GF@%help2"); // ret pozici string

	I_Label(l, "$%%OrdRet"); //konec
	I_PushSString(l, "LF@ret"); //push vzsledku na stack
	I_PopFrame(l);
	I_Return(l);

}
/*
 *@brief nejdrive se pusnou parametry fce odzadu. CHR(i)
 * nejdrive push 's'
 * nadefinuje se navrat hodnota
 */
void I_Chr(InListPtr l)
{
	InListInsertLast(l, "# CHR");												
	I_Label(l, "$%Chr");
	I_PushFrame(l);
	I_PopSString(l, "GF@%help1"); //popnu si do pomocne promene
	InListInsertLast(l, "DEFVAR LF@%ret");
	InListInsertLast(l, "MOVE LF@%ret string@");
	InListInsertLast(l, "DEFVAR LF@%range");
	InListInsertLast(l, "LT LF@%range GF@%help1 int@0");
	InListInsertLast(l, "JUMPIFEQ $ChrRet LF@%range bool@true");
	InListInsertLast(l, "GT LF@%range GF@%help1 int@255");
	InListInsertLast(l, "JUMPIFEQ $ChrRet LF@%range bool@true");
	InListInsertLast(l, "INT2CHAR LF@%ret LF@%0");

	I_Label(l, "$ChrRet");
	I_PushSString(l, "LF@%ret");
	I_PopFrame(l);
	I_Return(l);
}

//TODO DODELAT BUILD_IN FUNKCE

void BaseGen(InListPtr l)
{
	InListInsertLast(l, ".IFJcode18");
	I_DefVarString(l, "GF@%help1");
	I_DefVarString(l, "GF@%help2");
	I_DefVarString(l, "GF@%help3");
	I_Jump(l, "%%main");
	I_Label(l, "%%zerodiv");
	InListInsertLast(l, "EXIT int@9");

}
void BuildInGen(InListPtr l )
{
	InListInsertLast(l, "# Build-In Funkce");
	I_InputS(l);
	I_InputF(l);
	I_InputI(l);
	I_Print(l);
	I_Length(l);
	I_Chr(l);
	I_SubStr(l);
	I_Ord(l);

}
void MainGen(InListPtr l)
{
	InListInsertLast(l, "# Main - po definicich build in fci");
	I_Label(l, "%%main");
	I_CreateFrame(l);
	I_PushFrame(l);
}



//int Break();
//int DPrint();


#endif // !Instruction_c
