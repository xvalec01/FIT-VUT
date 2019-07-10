/*
 * List.h obsahuje deklarace funkc pro prci se seznamem vzuitm nap. v genertoru vslednho kdu ifjcode18
 *
 *@author Petr Urbnek - XURBAN 70
 *@date 14.11.2018
*/

#ifndef _EXPRESSION_LIST_H
#define _EXPRESSION_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "str.h"
#include "scanner.h"

///--------------------------------------------/////
/// -----  DEKLARACE A DEFINICE STRUKTUR  ---/////
///-------------------------------------------/////

typedef struct token_list
token_list,*token_listPtr;

struct token_list 
{
	sToken *token;
	token_listPtr next;
	token_listPtr prev;
};

typedef struct S_List 
{
	token_listPtr  first;			// Ukazatel na prvni token v seznamu
	token_listPtr  last;			// Ukazatel na posledn token v seznamu
	token_listPtr  active;			// Ukazatel na aktivn token v seznamu
}List,*ListPtr;	// Struktura pro ukldn instrukc pro vpis


///--------------------------------------------/////
/// -----  DEKLARACE FUNKC LISTU ---------------/////
///-------------------------------------------/////

/*
 *@brief Vytvo nov seznam pro ukldn textu, nap. isntrukc
 *
 *@return Nov vytvoen, alokovan a inicializovan ukazatel na strukturu List
*/
ListPtr ListCreate();

/*
 *@brief Zru a dealokuje strukturu List, vetn jejich prvk
 *
 *@param ListPtr lp		Ukazatel na Seznam 
*/
void ListDestroy(ListPtr lp);

/// --------- Insert --------- ////
/*
 *@brief Vlo novou token na zatek InstructionList
 *
 *@param ListPtr lp	Ukazatel na seznam 
 *@param sToken *token			Text instrukce
 *
 *@return ukazatel na seznam s nov pidanm itemem na zatku seznamu
*/
token_listPtr ListInsertFirst(ListPtr lp, sToken *token);

/*
 *@brief Vlo novou token na konec InstructionList
 *
 *@param ListPtr lp	Ukazatel na seznam 
 *@param sToken *token			Text 
 *
 *@return ukazatel na seznam  s nov pidanm itemem na konci seznamu
*/

token_listPtr ListInsertLast(ListPtr lp, sToken *token);

/// ------- Aktivace -----------////

/*
 *@brief Nastav prvn item na aktivn
 *
 *@param ListPtr lp		Ukazatel na Seznam 
*/
void ListActFirst(ListPtr lp);

/*
 *@brief Nastav posledn item na aktivn
 *
 *@param ListPtr lp		Ukazatel na Seznam 
*/
void ListActLast(ListPtr lp);

/*
 *@brief Nastav nsledujc item na aktivn
 *
 *@param ListPtr lp		Ukazatel na Seznam 
*/
void ListActNext(ListPtr lp);

/// ------- DELETE -----------////
/*
 *@brief Odstran prvn item 
 *
 *@param ListPtr lp		Ukazatel na Seznam 
*/
void ListDelFirst(ListPtr lp);

/*
 *@brief Odstran aktivn item
 *
 *@param ListPtr lp		Ukazatel na Seznam 
*/
void ListDelAct(ListPtr lp);

/*
 *@brief Odstran posledn item
 *
 *@param ListPtr lp		Ukazatel na Seznam 
*/
void ListDelLast(ListPtr lp);

#endif 