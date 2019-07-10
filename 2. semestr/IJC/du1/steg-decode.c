/*
 * Soubor:		steg-decode.c
 * Projekt:		IJC:DU1
 * Autor:		David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:	gcc 5.4.0 ubuntu
 * Datum:		20. 3. 2018
 * Popis:		rozšifrování textu v obrázku pomocí eratosthenova síta
*/


#ifndef ST_DE
#define ST_DE

#include <stdio.h>
#include <ctype.h>
#include "ppm.h"
#include "eratosthenes.h"
#include "bit_array.h"

void decode(struct ppm *img)
{
	unsigned long size = 3 * img->xsize * img->ysize + 1;
	unsigned long *message = malloc(size);	

	if (message == NULL)
	{
		error_exit("Není možné alokovat paměť.\n");
	}

	char bit[2] = {CHAR_BIT, 0};
	message[0] = size - 1;
	eratosthenes(message);

	int offset = 0;
	for(long i = 11; i < size; i++)
	{
		if (bit_array_getbit(message, i) == 0)
		{
			bit_array_setbit(bit, offset, (img->data[i] & 1));
			offset++;
			if(offset == CHAR_BIT)
			{
				if(bit[1] == '\0')
				{
					printf("\n");
					break;
				}
				printf("%c",bit[1]);
				offset = 0;
				bit[1] = 0;
			}
		}
	}
	free(message);
}


int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		error_exit("Špatné argumenty.\n");
	}

	struct ppm *img = ppm_read(argv[1]);

	if(img == NULL)
	{
		error_exit("Nevalidni soubor.\n");
	}

	decode(img);
	free(img);

	return 0;
}


#endif
