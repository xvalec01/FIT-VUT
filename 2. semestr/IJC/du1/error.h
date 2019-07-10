/*
 * Soubor:		error.h
 * Projekt:		IJC:DU1
 * Autor:		David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:	gcc 5.4.0 ubuntu
 * Datum:		20. 3. 2018
 * Popis:		rozhraní pro error.c
*/
#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void warning_msg(const char *fmt, ...);
void error_exit(const char *fmt, ...);

#endif