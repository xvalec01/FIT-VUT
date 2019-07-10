
/*
 * List.c obsahuje definice funkcí pro práci se seznamem využitém v generátoru výsledného kódu ifjcode18
 * 
 * @author Petr Urbánek - XURBAN 70
 * @date 14.11.2018
 */

#ifndef _EXPRESSION_LIST_C
#define _EXPRESSION_LIST_C

#include "expression_list.h"

ListPtr ListCreate() 
{
	ListPtr list = malloc(sizeof(List));
	if (list == NULL)
		//DEBUG
		error_exit(INTERNAL_ERROR, "");

	list->active = NULL;
	list->first = NULL;
	list->last = NULL;
	return list;
}

void ListDestroy(ListPtr lp)
{
	ListPtr list = lp;
	token_listPtr toDestroy = NULL;

	while (list->first != NULL)
	{
		toDestroy = list->first;
		list->first = toDestroy->next;
		//uvolnit misto stringu

		free(toDestroy->token->data);
		free(toDestroy->token);
		free(toDestroy);
	}
	free(list);
	lp = NULL;
}

//------- Insert --------//
token_listPtr ListInsertFirst(ListPtr lp, sToken *token)
{
	token_listPtr toInsert = malloc(sizeof(token_list));
	if (toInsert == NULL)
		//debug 
		error_exit(INTERNAL_ERROR, "");

	toInsert->token = newToken();
	toInsert->token->typ = token->typ;
	if (strCopyString(toInsert->token,token))
		error_exit(INTERNAL_ERROR, "");
	toInsert->prev = NULL; //vkladame na prvni misto
	toInsert->next = lp->first;
	if (lp->first == NULL) // prazdny list
		lp->last = toInsert; //posledni je zaroven prvni
	else
		lp->first->prev = toInsert;
	lp->first = toInsert;
	
	return toInsert;
}

token_listPtr ListInsertLast(ListPtr lp, sToken *token)
{

	token_listPtr toInsert = malloc(sizeof(token_list));
	if (toInsert == NULL)
		//debug 
		error_exit(INTERNAL_ERROR, "");
	
	toInsert->token = newToken();
	toInsert->token->typ = token->typ;
	if (strCopyString(toInsert->token,token))
		error_exit(INTERNAL_ERROR, "");
	toInsert->prev = lp->last; //vkladame na posledni misto
	toInsert->next = NULL; 
	if (lp->last == NULL) // prazdny list
		lp->first = toInsert; //posledni je zaroven prvni
	else
		lp->last->next = toInsert;
	lp->last = toInsert;

	return toInsert;
}

void PrintList(ListPtr lp)
{
	if (lp == NULL)
		return;
	
	ListActFirst(lp); //aktualizuje 1. prvek
	while (lp->active != NULL)
	{
		//PRIKAZY na provedeni
		fprintf(stdout, "%s", lp->active->token->data); 
		//PRIKAZY na provedeni

		ListActNext(lp); //akutalizuje dalsi
	}
}

//------- Actualize --------//
void ListActFirst(ListPtr lp) 
{
	lp->active = lp->first;
}


void ListActLast(ListPtr lp)
{
	lp->active = lp->last;
}

void ListActNext(ListPtr lp)
{
	if (lp->active != NULL)
		lp->active = lp->active->next;
}


//------- Delete --------//
void ListDelFirst(ListPtr lp)
{
	token_listPtr toDel = NULL;
	if (lp->first != NULL) //neprazdny seznam
	{
		toDel = lp->first;
		if (lp->first == lp->last) // jednoprvkovy seznam
			lp->last = NULL;
		if (lp->first == lp->active)//aktivni prvni instr
			lp->active = NULL;

		lp->first = toDel->next; // aktualizace 1. prvku
		if (lp->first != NULL)
			lp->first->prev = NULL;
	
		free(toDel->token->data);
		free(toDel->token);
		free(toDel);
	}
}

void ListDelAct(ListPtr lp)
{
	token_listPtr toDel = NULL;
	if (lp->active != NULL) // aktivni seznam
	{
		toDel = lp->active;
		if (lp->active == lp->last) // aktivni posledni sToken
			lp->last = lp->last->prev;
		if (lp->first == lp->active)//aktivni prvni sToken
			lp->first = toDel->next;
		if (lp->active->next != NULL) //aktivni ma nasledujici sToken
			lp->active->next->prev = toDel->prev; 
		if (lp->active->prev != NULL) //aktivni ma predchozi sToken
			lp->active->prev->next = toDel->next;
		
		free(toDel->token->data);
		free(toDel->token);
		free(toDel);
		lp->active = NULL;
	}
}

void ListDelLast(ListPtr lp)
{
	token_listPtr toDel = NULL;
	if (lp->last != NULL) //neprazdny seznam
	{
		toDel = lp->last;
		if (lp->first == lp->last) // jednoprvkovy seznam
		{
			lp->first = NULL;
			lp->last = NULL;
		}
		if (lp->last == lp->active)//aktivni posledni instr
			lp->active = NULL;

		lp->last = toDel->prev; // aktualizace posledniho prvku
		if (lp->last != NULL)
			lp->last->next = NULL;
	}
	free(toDel->token->data);
	free(toDel->token);
	free(toDel);
}

#endif