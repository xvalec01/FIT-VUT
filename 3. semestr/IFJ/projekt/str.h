/*
    Projekt:
        PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

    Autor:
        Martin Kladňák - xkladn00

    Soubor:
        str.h

    Varianta:
        Tým 85, varianta 1
*/
#ifndef STR_H
#define STR_H
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include "error.h"

typedef struct token
{
  char *data;	
  int length;		
  int allocSize;	
  int typ;
} sToken,*sTokenPtr;

int strCopyString(sToken *s1, sToken *s2);
int strInit(sTokenPtr s);
int strAddChar(sTokenPtr s1, char c);
char *stringCreate(char *string);
sTokenPtr newToken ();
void freeToken(sToken *token);
char* strConCat(char* str1, char *str2);
#endif

