/*
 * Soubor:      tail.c
 * Projekt:     IJC:DU2
 * Autor:       David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:   gcc 5.4.0 ubuntu
 * Datum:       24. 4. 2018
 * Popis:       Vypisování zadaných řádků ze souboru/standartního vstupu v c
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define MAXLENGTH 1024
#define PRINT_ROWS 10
#define OK 0
#define FILE_ERROR 1
#define INVALID_INPUT 2
#define UNKNOWN_ERROR -1

int main(int argc, char const *argv[])
{
	if (argc == 1)
	{
        int i = 0;
        int j = -1;// začíná to na -1 kvůli tomu, aby v cyklu bylo j v prvním kroku 0
        char rows[PRINT_ROWS][MAXLENGTH] = { {'\0',}, {'\0',}, };
        char row[MAXLENGTH] = {'\0',};
        // řádek po řádku ze standartního vstupu v terminálu
        while (fgets(row,MAXLENGTH-1,stdin) != NULL)
        {   
            j++;
            i = j % PRINT_ROWS;
            strncpy(rows[i],row,MAXLENGTH); // při menší dělce row než MAXLENGTH se zbytek vyplní '̈́\0'
        }
        // vypisování žádaného počtu řádků
        for(int k = 0; k < PRINT_ROWS; k++)
        {
            j++;
            i = j % PRINT_ROWS;
            printf("%s",rows[i]);
        }
    }
	else if (argc == 2)
	{
		FILE *fp;
    	fp = fopen(argv[1],"r");
    	if (fp == NULL) 
    	{
        	fprintf (stderr,"Invalid file opening\n");
        	return FILE_ERROR;
    	}

    	int i = 0;
    	int j = -1; // začíná to na -1 kvůli tomu, aby v cyklu bylo j v prvním kroku 0
    	char rows[PRINT_ROWS][MAXLENGTH] = { {'\0',}, {'\0',}, };
    	char row[MAXLENGTH] = {'\0',};
        // řádek po řádku z načteného souboru
    	while (fgets(row,MAXLENGTH-1,fp))
    	{	
    		j++;
    		i = j % PRINT_ROWS;
    		strncpy(rows[i],row,MAXLENGTH);
    	}
        // vypisování požadovaného počtu řádků
    	for(int k = 0; k < PRINT_ROWS; k++)
    	{
    		j++;
    		i = j % PRINT_ROWS;
    		printf("%s",rows[i]);
            // nový řádek za poslední 
            if(k == PRINT_ROWS-1)
            {
                printf("\n");
            }
    	}
    	fclose(fp);
	}
    else if ((argc == 3) && (strcmp(argv[1],"-n") == 0 ))
	{
		char *endptr;
        // kontrola jestli je druhý argument číslo
        int print = strtol(argv[2], &endptr, 10);
        if(print == 0)
                return OK;
        if ((endptr == argv[2]) || (*endptr != '\0') || (print < 0))
        {
            fprintf (stderr,"'%s' is not valid argument\n", argv[2]);
            return INVALID_INPUT;
        } 
        else 
        {
            int i = 0;
            int j = -1;// začíná to na -1 kvůli tomu, aby v cyklu bylo j v prvním kroku 0
            char rows[print][MAXLENGTH];
            char row[MAXLENGTH] = {'\0',};
            while (fgets(row,MAXLENGTH-1,stdin) != NULL)
            {   
                j++;
                i = j % print;
                strncpy(rows[i],row,MAXLENGTH);
            }
            for(int k = 0; k < print; k++)
            {
                j++;
                i = j % print;
                printf("%s",rows[i]);
                if(k == print-1)
                {
                    printf("\n");
                }
            }
        }
        
	}
	else if ((argc == 4) && (strcmp(argv[1],"-n") == 0 ))
	{
        FILE *fp;
        fp = fopen(argv[3],"r");
        if (fp == NULL) 
        {
            fprintf (stderr,"Invalid file opening\n");
            return FILE_ERROR;
        }

		char *endptr;
        // kontrola jestli je druhý argument číslo
        int print = strtol(argv[2], &endptr, 10);
        if(print == 0)
                return OK;
        if ((endptr == argv[2]) || (*endptr != '\0') || (print < 0))
        {
            fprintf (stderr,"'%s' is not valid argument\n", argv[2]);
            fclose(fp);
            return INVALID_INPUT;
        }
        else
        {
            int i = 0;
            int j = -1; // začíná to na -1 kvůli tomu, aby v cyklu bylo j v prvním kroku 0
            char rows[print][MAXLENGTH];
            char row[MAXLENGTH] = {'\0',};
            // řádek po řádku
            while (fgets(row,MAXLENGTH-1,fp))
            {   
                j++;
                i = j % print;
                strncpy(rows[i],row,MAXLENGTH);
            }
            for(int k = 0; k < print; k++)
            {
                j++;
                i = j % print;
                printf("%s",rows[i]);
                if(k == print-1)
                {
                    printf("\n");
                }
            }
        }
        fclose(fp);
	}
    else if (argc > 4)
    {
        fprintf(stderr, "Too many arguments\n" );
        return INVALID_INPUT;
    }
    else
    {
        fprintf(stderr, "Something went wrong\n" );
        return UNKNOWN_ERROR;
    }

    return OK;
}