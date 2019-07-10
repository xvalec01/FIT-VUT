/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastn�ho p�eklada�e

	Autor:
		Petr Urb�nek - Xurban 70

	Soubor:
		error.h

	Varianta:
		T�m 85, varianta 1
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
	SUCCES					= 0,	// p�eklad prob�hl �sp�n�
	LEXICAL_ERROR			= 1,	// chyba v programu v r�mci lexik�ln� anal�zy (chybn� struktura aktu�ln�ho lex�mu)
	SYNTAX_ERROR			= 2,	// chyba v programu v r�mci syntaktick� anal�zy (chybn� syntaxe programu)
	SEMANTIC_DEFINE_ERROR	= 3,	// s�mantick� chyba v programu � nedefinovan� funkce/prom�nn�, pokus o redefinicifunkce / prom�nn�, atp.
	SEMANTIC_DATATYPE_ERROR = 4,    // s�mantick�/b�hov� chyba typov� kompatibility v aritmetick�ch, �et�zcov�ch arela�n�ch v�razech
	SEMANTIC_ARGUMENT_ERROR = 5,	// s�mantick� chyba v programu � �patn� po�et parametr� u vol�n� funkce
	SEMANTIC_OTHER_ERROR	= 6,	// ostatn� s�mantick� chyby
	RUN_ZERODIV_ERROR		= 9,	// b�hov� chyba d�len� nulou.
	INTERNAL_ERROR			= 99,	// intern� chyba p�eklada�e tj. neovlivn�n� vstupn�m programem (nap�. chyba alokacepam�ti, atd.).
} ErrorCodes;						// Chybov� k�dy


void error_exit(ErrorCodes errorcode, const char *fmt, ...);

#endif //!ERROR

