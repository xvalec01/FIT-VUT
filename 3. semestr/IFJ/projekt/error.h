/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		error.h

	Varianta:
		Tým 85, varianta 1
*/

#ifndef Error_h
#define Error_h

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


/*
 *
*/
typedef enum
{
	SUCCES					= 0,	// pøeklad probìhl úspìšnì
	LEXICAL_ERROR			= 1,	// chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému)
	SYNTAX_ERROR			= 2,	// chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu)
	SEMANTIC_DEFINE_ERROR	= 3,	// sémantická chyba v programu – nedefinovaná funkce/promìnná, pokus o redefinicifunkce / promìnné, atp.
	SEMANTIC_DATATYPE_ERROR = 4,    // sémantická/bìhová chyba typové kompatibility v aritmetických, øetìzcových arelaèních výrazech
	SEMANTIC_ARGUMENT_ERROR = 5,	// sémantická chyba v programu – špatný poèet parametrù u volání funkce
	SEMANTIC_OTHER_ERROR	= 6,	// ostatní sémantické chyby
	RUN_ZERODIV_ERROR		= 9,	// bìhová chyba dìlení nulou.
	INTERNAL_ERROR			= 99,	// interní chyba pøekladaèe tj. neovlivnìná vstupním programem (napø. chyba alokacepamìti, atd.).
} ErrorCodes;						// Chybové kódy


void error_exit(ErrorCodes errorcode, const char *fmt, ...);

#endif //!ERROR

