/*
 * Soubor:		error.c
 * Projekt:		IJC:DU1
 * Autor:		David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:	gcc 5.4.0 ubuntu
 * Datum:		20. 3. 2018
 * Popis:		vypisování hlášek při chybách na stderr, pomocí vlastních funkcí 
*/
#ifndef ERROR_C
#define ERROR_C

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "error.h"


void warning_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, ap);
}

void error_exit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "CHYBA: ");
    vfprintf(stderr, fmt, ap);
    exit(1);
}

#endif