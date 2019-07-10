/*
    Projekt:
        PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

    Autor:
        Martin Kladňák - xkladn00

    Soubor:
        scanner.h

    Varianta:
        Tým 85, varianta 1
*/
#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "str.h"
#include "error.h"
#define EOL 1
#define END_OF_FILE 2
#define OPERATOR_LEQ 20 //<=
#define OPERATOR_LTN 21//<
#define OPERATOR_BEQ 22//>=
#define OPERATOR_BTN 23//>
#define OPERATOR_EQ 24//==
#define OPERATOR_NEQ 25//!=
#define OPERATOR_PRIRAZOVACI 26//=
#define OPERATOR_PLUS 27//+
#define OPERATOR_MINUS 28
#define OPERATOR_DELENO 29
#define OPERATOR_KRAT 30
#define OPERATOR_LEVA_ZAVORKA 31
#define OPERATOR_PRAVA_ZAVORKA 32
#define OPERATOR_CARKA 33
#define IDENTIFIKATOR 3
#define NIL 4
#define STRING 5
#define KEY_WORD 6
#define INTEGER 7
#define DOUBLE 8
#define DOUBLE_EXPONENT 9
#define DOUBLE_DOTTED_EXPONENT 10
#define KEY_WORD_DEF    11
#define KEY_WORD_DO     12
#define KEY_WORD_ELSE   13
#define KEY_WORD_END    14
#define KEY_WORD_IF     15
#define KEY_WORD_NOT    16
#define KEY_WORD_NILL   17
#define KEY_WORD_THEN   18
#define KEY_WORD_WHILE  19
#define STRING_LENGHT 8 //pocet bytu
#define INPUTS 34
#define INPUTI 35
#define INPUTF 36
#define PRINT 37
#define LENGTH 38
#define SUBSTR 39
#define ORD 40
#define CHR 41
#define BOOL 42

typedef enum {
	S_START, 
    S_END,				
    S_CHYBA,					
    S_KOMENTAR,				
    S_INTEGER,					
    S_DOUBLE,					
    S_IDENTIFIKATOR,			
    S_PLUS,					
    S_MINUS,					
    S_KRAT,					
    S_DELENO,					
    S_MENSI,					
    S_MENSI_NEBO_ROVNO,
    S_VETSI,					
    S_VETSI_NEBO_ROVNO,
    S_ROVNO,					  
    S_RUZNE,
    S_EOL,										
    S_LEVA_ZAVORKA,			
    S_PRAVA_ZAVORKA,
    S_CARKA,
    S_END_OF_FILE,
    S_COMENT,
    S_BLOK_COMENT,
    S_STRING,
    S_X_STRING,
    S_LOM_STRING,
    S_EXPONENT,
    S_DESETINY,
    S_CEKEJ,
} tstav;
void ungettoken(struct token *tokenptr,FILE * file);
int get_token(sToken *tokenptr,FILE * file);
int isfce(char *slovo);
int iskeyword(char *slovo);
#endif
