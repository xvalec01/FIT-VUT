
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    
	L->First = NULL;
	L->Last = NULL;
	if (L->First != NULL || L->Last != NULL)
		DLError();
	L->Act = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	
	tDLElemPtr hlp;

	while(L->First) // dokud nebude ukazatel na první prvek NULL
	{
		hlp = L->First; // hlp ukazatel na prvni prvek
		L->First = hlp->rptr; // ukazatel na prvni prvek bude na dalsi prvek
		free(hlp);			
	}
	L->Act=NULL;	
	L->Last=NULL; 
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

	tDLElemPtr hlp = malloc(sizeof(struct tDLElem));

	if(hlp == NULL)
		DLError();

	else
	{
		hlp->data = val;
		hlp->lptr = NULL;
		hlp->rptr = L->First;

		if(L->Last == NULL) // první vložení
			L->Last = hlp; // poslední je vložený prvek

		else // jiné než první vložení
			L->First->lptr = hlp;

		L->First = hlp;
	}
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	
	tDLElemPtr hlp = malloc(sizeof(struct tDLElem));

	if (hlp == NULL)
		DLError();
	else
	{
		hlp->data = val;
		hlp->rptr = NULL;
		hlp->lptr = L->Last;

		if(L->Last == NULL) 
			L->First = hlp; 

		else // jiný než první 
			L->Last->rptr = hlp;

		L->Last = hlp;
	}
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->First;

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if (L->First != NULL){
		tDLElemPtr hlp;
		hlp = L->First; // do hlp dám ukazatel na prvni prvek
		*val = hlp->data;
	}
	else
		DLError();
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last != NULL){
		tDLElemPtr hlp = L->Last; // do hlp dám ukazatel na poslední prvek
		*val = hlp->data;
	}
	else
		DLError();
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if (L->First != NULL){
		if (L->First == L->Act) // zruší aktivitu
			L->Act = NULL;

		tDLElemPtr hlp = L->First;

		if (L->First == L->Last)
		{
			L->Last = NULL;
			L->First = NULL; // jestliže je v seznamu jen jeden prvek dám oba ukazatele na NULL
		}
		else
		{
			L->First = L->First->rptr; // v jiném případě nastavim ukazatel na prvni prvek na druhy prvek prvek
			L->First->lptr = NULL;
		}
		free(hlp);
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	if (L->Last != NULL){
		if (L->Last == L->Act) // zruší aktivitu
			L->Act = NULL;

		tDLElemPtr hlp = L->Last;

		if (L->First == L->Last)
		{
			L->Last = NULL;
			L->First = NULL; // jestliže je v seznamu jen jeden prvek dám oba ukazatele na NULL
		}
		else
		{
			L->Last = L->Last->lptr; // v jiném případě nastavim ukazatel na prvni prvek na druhy prvek prvek
			L->Last->rptr = NULL;
		}
		free(hlp);
	}
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if (L->Act != NULL && L->Act != L->Last){

		tDLElemPtr hlp = L->Act->rptr;

		if (hlp->rptr != NULL) // za zrušeným je další prvek
		{
			L->Act->rptr = hlp->rptr;
			hlp->rptr->lptr = L->Act;
		}

		else // za zrušeným není další prvek
		{
			L->Last = L->Act;
			L->Act->rptr = NULL;
		}

		free(hlp);
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
			
	if (L->Act != NULL && L->Act != L->First){

		tDLElemPtr hlp = L->Act->lptr;

		if (hlp->lptr != NULL) // před zrušeným prvkem je další prvek
		{
			L->Act->lptr = hlp->lptr;
			hlp->lptr->rptr = L->Act;
		}

		else // před zrušeným prvkem není další prvek
		{
			L->First = L->Act;
			L->Act->lptr = NULL;
		}

		free(hlp);
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	
	if(L->Act != NULL)
	{	
		tDLElemPtr hlp = malloc(sizeof(struct tDLElem));

		if(hlp == NULL) 
			DLError();

		else // podařilo se alokovat, přidáme prvek
		{
			hlp->data = val;
			hlp->lptr = L->Act;
			hlp->rptr = L->Act->rptr;
			L->Act->rptr = hlp;

			if(L->Act != L->Last) // jestliže aktivní prvek nebyl poslední
				hlp->rptr->lptr = hlp;

			else // jestliže aktivní prvek byl poslední
				L->Last = hlp;
		}
	} 
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act != NULL)
	{	
		tDLElemPtr hlp = malloc(sizeof(struct tDLElem));

		if(hlp == NULL) 
			DLError();
		
		else // podařilo se alokovat, přidáme prvek
		{
			hlp->data = val;
			hlp->rptr = L->Act;
			hlp->lptr = L->Act->lptr;
			L->Act->lptr = hlp;

			if(L->Act != L->Last) // jestliže aktivní prvek nebyl první
				hlp->lptr->rptr = hlp;

			else // jestliže aktivní prvek byl první
				L->First = hlp;
		}
	} 
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
		
	if (L->Act == NULL)
		DLError();
	else
		*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

	if (L->Act != NULL)
		L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
	if(L->Act != NULL) // seznam je aktivní
	{
		if(L->Act == L->Last) // aktivní poslední, aktivitu ztrácí
			L->Act = NULL;

		else // aktivita se přesune na prvek vpravo
			L->Act = L->Act->rptr;
} 
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	
	if(L->Act != NULL) // seznam je aktivní
	{
		if(L->Act == L->First) // aktivní první, aktivitu ztrácí
			L->Act = NULL;
		
		else // aktivita se přesune na prvek vlevo
			L->Act = L->Act->lptr;
	} 
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	
	return(L->Act != NULL);
}

/* Konec c206.c*/
