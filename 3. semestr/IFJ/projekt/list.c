/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		list.c

	Varianta:
		Tým 85, varianta 1
*/

#ifndef InList_C
#define InList_C

#include "list.h"

InListPtr InListCreate() 
{
	InListPtr InList = malloc(sizeof(InList));
	if (InList == NULL)
		error_exit(INTERNAL_ERROR, "InList malloc fail\n");

	InList->active = NULL;
	InList->first = NULL;
	InList->last = NULL;
	return InList;
}

void InListDestroy(InListPtr lp)
{
	InListPtr InList = lp;
	InstructionPtr toDestroy = NULL;

	while (InList->first != NULL)
	{
		toDestroy = InList->first;
		InList->first = toDestroy->next;
		//uvolnit misto stringu

		free(toDestroy);
	}
	free(InList);
	lp = NULL;
}

//------- Insert --------//
InstructionPtr InListInsertFirst(InListPtr lp, void *content)
{
	InstructionPtr toInsert = malloc(sizeof(Instruction));
	if (toInsert == NULL)
		error_exit(INTERNAL_ERROR, "toInsert malloc fail\n");

	toInsert->content = content;
	toInsert->prev = NULL; //vkladame na prvni misto
	toInsert->next = lp->first;
	if (lp->first == NULL) // prazdny InList
		lp->last = toInsert; //posledni je zaroven prvni
	else
		lp->first->prev = toInsert;
	lp->first = toInsert;
	
	return toInsert;
}

// string predavat bez alokace, naalokuje si ho samo
void InListInsertLast(InListPtr lp, char *content)
{

	InstructionPtr toInsert = NULL;
	toInsert = (InstructionPtr)malloc(sizeof(Instruction));
	if (toInsert == NULL)
		error_exit(INTERNAL_ERROR, "");

	toInsert->content = stringCreate(content);
	toInsert->prev = lp->last; //vkladame na posledni misto
	toInsert->next = NULL; 
	if (lp->last == NULL) // prazdny InList
		lp->first = toInsert; //posledni je zaroven prvni
	else
		lp->last->next = toInsert;
	lp->last = toInsert;

}


//------- Actualize --------//

void InListActFirst(InListPtr lp) 
{
	lp->active = lp->first;
}


void InListActLast(InListPtr lp)
{
	lp->active = lp->last;
}

void InListActNext(InListPtr lp)
{
	if (lp->active != NULL)
		lp->active = lp->active->next;
}


//------- Delete --------//

void InListDelFirst(InListPtr lp)
{
	InstructionPtr toDel = NULL;
	if (lp->first != NULL) //neprazdny seznam
	{
		toDel = lp->first;
		if (lp->first == lp->last) // jednoprvkovy seznam
			lp->last = NULL;
		if (lp->first == lp->active)//aktivni prvni Instruction
			lp->active = NULL;

		lp->first = toDel->next; // aktualizace 1. prvku
		if (lp->first != NULL)
			lp->first->prev = NULL;
	
		free(toDel);
	}
}

void InListDelAct(InListPtr lp)
{
	InstructionPtr toDel = NULL;
	if (lp->active != NULL) // aktivni seznam
	{
		toDel = lp->active;
		if (lp->active == lp->last) // aktivni posledni Instruction
			lp->last = lp->last->prev;
		if (lp->first == lp->active)//aktivni prvni Instruction
			lp->first = toDel->next;
		if (lp->active->next != NULL) //aktivni ma nasledujici Instruction
			lp->active->next->prev = toDel->prev; 
		if (lp->active->prev != NULL) //aktivni ma predchozi Instruction
			lp->active->prev->next = toDel->next;
		
		free(toDel);
		lp->active = NULL;
	}
}

void InListDelLast(InListPtr lp)
{
	InstructionPtr toDel = NULL;
	if (lp->last != NULL) //neprazdny seznam
	{
		toDel = lp->last;
		if (lp->first == lp->last) // jednoprvkovy seznam
		{
			lp->first = NULL;
			lp->last = NULL;
		}
		if (lp->last == lp->active)//aktivni posledni Instruction
			lp->active = NULL;

		lp->last = toDel->prev; // aktualizace posledniho prvku
		if (lp->last != NULL)
			lp->last->next = NULL;
	}
	free(toDel);
}
//POUZE PRO STRINGY
void PrintInList(InListPtr lp)
{
	if (lp == NULL)
		return;
	
	InListActFirst(lp); //aktualizuje 1. prvek
	while (lp->active != NULL)
	{
		
		//PRIKAZY na provedeni
		char *out = (char*)lp->active->content;
		printf ("%s\n", out);

		//PRIKAZY na provedeni
		
		InstructionPtr tmp = lp->active;
		free(tmp->content); //odstrani alokaci stringu
		InListActNext(lp); //akutalizuje dalsi
		
		if (lp->active == NULL)
			free(tmp);
		
		else
			free(lp->active->prev);
		
	}

}


#endif