/*
	Projekt:
		PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

	Autor:
		Petr Urbánek - Xurban 70

	Soubor:
		error.c

	Varianta:
		Tým 85, varianta 1
*/

#include "error.h"

void error_exit(ErrorCodes errorcode, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, "CHYBA: ");
	vfprintf(stderr, fmt, args);

	va_end(args);
	exit(errorcode);
}