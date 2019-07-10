/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		list.h

	Varianta:
		Tým 85, varianta 1
*/
#ifndef InLIST_H
#define InLIST_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "str.h"
#include "scanner.h"





///--------------------------------------------/////
/// -----  DEKLARACE A DEFINICE STRUKTUR  ---/////
///-------------------------------------------/////

/// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
/// upesnit co budeme puvat Instructionuction nebo Instruction poitenr

typedef struct S_Instruction
Instruction,*InstructionPtr;

struct S_Instruction
{
	char *content;				// ukazatel an misto s obsahem prvku
	InstructionPtr  prev;       // Ukazatel na pedchozi prvek
	InstructionPtr  next;       // Ukazatel na nsledujci prvek
};	// Struktura pro ukldn dat 



typedef struct S_InList 
{
	InstructionPtr	first;				// Ukazatel na prvni Instructionukci v seznamu
	InstructionPtr  last;				// Ukazatel na posledn Instructionukci v seznamu
	InstructionPtr  active;			// Ukazatel na aktivn Instructionukci v seznamu
}InList,*InListPtr;	// Struktura pro ukldn Instructionukc pro vpis


///--------------------------------------------/////
/// -----  DEKLARACE FUNKC LISTU ---------------/////
///-------------------------------------------/////

/*
 *@brief Vytvo nov seznam pro ukldn textu, nap. isntrukc
 *
 *@return Nov vytvoen, alokovan a inicializovan ukazatel na strukturu List
*/
InListPtr InListCreate();

/*
 *@brief Zru a dealokuje strukturu List, vetn jejich prvk
 *
 *@param InListPtr lp		Ukazatel na Seznam 
*/
void InListDestroy(InListPtr lp);

/// --------- Insert --------- ////
/*
 *@brief Vlo novou Instructionukci na zatek InstructionuctionList
 *
 *@param InListPtr lp	Ukazatel na seznam 
 *@param char *content			Text Instructionukce
 *
 *@return ukazatel na seznam s nov pidanm itemem na zatku seznamu
*/
InstructionPtr InListInsertFirst(InListPtr lp, void *content);

/*
 *@brief Vlo novou Instructionukci na konec InstructionuctionList
 * string predavat bez alokace, naalokuje si ho samo
 *@param InListPtr lp	Ukazatel na seznam 
 *@param char *content			Text 
 *
*/
void InListInsertLast(InListPtr lp, char *content);


/// ------- Aktivace -----------////

/*
 *@brief Nastav prvn item na aktivn
 *
 *@param InListPtr lp		Ukazatel na Seznam 
*/
void InListActFirst(InListPtr lp);

/*
 *@brief Nastav posledn item na aktivn
 *
 *@param InListPtr lp		Ukazatel na Seznam 
*/
void InListActLast(InListPtr lp);

/*
 *@brief Nastav nsledujc item na aktivn
 *
 *@param InListPtr lp		Ukazatel na Seznam 
*/
void InListActNext(InListPtr lp);

/// ------- DELETE -----------////
/*
 *@brief Odstran prvn item 
 *
 *@param InListPtr lp		Ukazatel na Seznam 
*/
void InListDelFirst(InListPtr lp);

/*
 *@brief Odstran aktivn item
 *
 *@param InListPtr lp		Ukazatel na Seznam 
*/
void InListDelAct(InListPtr lp);

/*
 *@brief Odstran posledn item
 *
 *@param InListPtr lp		Ukazatel na Seznam 
*/
void InListDelLast(InListPtr lp);

/*
 *@brief Vypise obsah lsitu na stdout
 *
 *@param InListPtr lp		Ukazatel na Seznam
*/
void PrintInList(InListPtr lp);



#endif 