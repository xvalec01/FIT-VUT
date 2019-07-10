/*
    Projekt:
        PROJEKT IFJ18 - Tvorba vlastního pøekladaèe

    Autor:
        Martin Kladňák - xkladn00

    Soubor:
        str.c

    Varianta:
        Tým 85, varianta 1
*/
#ifndef STR_C
#define STR_C
#include <string.h>
#include "str.h"
#define STRING_LENGHT 8

sToken *newToken() 
{
    sToken *tokenptr = malloc (sizeof (struct token));
    if (tokenptr == NULL)
      error_exit(INTERNAL_ERROR,"Chyba alokace");
    if ((tokenptr->data = (char*)malloc(STRING_LENGHT*sizeof(char))) == NULL) 
      {
      free(tokenptr);
      error_exit(INTERNAL_ERROR,"Chyba alokace");
      }
   tokenptr->data[0] = '\0';
   tokenptr->length = 0;
   tokenptr->allocSize = STRING_LENGHT;
   tokenptr->typ = 0;
   return tokenptr;
}

void freeToken(sToken *token) 
{
   free(token->data);
   free(token);
}


int strAddChar(struct token *s1, char c)
{
   if (s1->length + 1 >= s1->allocSize)
   {
      if ((s1->data = (char*) realloc(s1->data, s1->length + STRING_LENGHT)) == NULL)
         return 1;
      s1->allocSize = s1->length + STRING_LENGHT;
   }
   s1->data[s1->length] = c;
   s1->length++;
   s1->data[s1->length] = '\0';
   return 0;
}

char *stringCreate(char* token)
{
	unsigned len = strlen(token);
	char *s;
	s = calloc(len+1, sizeof(char));
	if (s == NULL)
		error_exit(INTERNAL_ERROR, "");
	strcpy(s,token);
	return s;
}

//NEODSTRANI ALOKACI STRINGU VE VSTUPU¨, neni na to moc cas
//Alokuje novy string ze dvou vstupu
char* strConCat(char* str1, char *str2)
{

	unsigned s1 = strlen(str1);
	unsigned s2 = strlen(str2);
	unsigned size = s1 + s2;
	if (size > 200)
		error_exit(INTERNAL_ERROR, "moc dlouhe dva stringy vice jak 200 charu");

	char *result = (char*)calloc(size,sizeof(char));
	if (result == NULL)
		error_exit(INTERNAL_ERROR, "");
	strcat(result, str1);
	strcat(result, str2);

	return result;

}



int strCopyString(sToken *s1, sToken *s2)
// prekopiruje retezec s2 do s1
{
   int newLength = s2->length;
   if (newLength >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->data = (char*) realloc(s1->data, newLength + 1)) == NULL)
          return -1;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->data, s2->data);
   s1->length = newLength;
   return 0;
}
#endif
