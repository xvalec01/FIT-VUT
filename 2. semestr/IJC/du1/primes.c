/*
 * Soubor:		primes.c
 * Projekt:		IJC:DU1
 * Autor:		David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:	gcc 5.4.0 ubuntu
 * Datum:		20. 3. 2018
 * Popis:		soubor pro spouštění programu pro výpis prvočísel získaných z eratosthenesova síta
*/
#ifndef PRIMES
#define PRIMES

#include <stdio.h>
#include <stdlib.h>
#include "bit_array.h"
#include "eratosthenes.h"

#define LIMIT 222000000
#define PRINT 10

int main()
{
	bit_array_create(pole,LIMIT+1);
	eratosthenes(pole);
	bit_array_setbit(pole,0,1);
	bit_array_setbit(pole,1,1);

 	int completed = 0;
 
	for(unsigned long i = LIMIT ; i > 2; i--)
	{
		if(bit_array_getbit(pole,i) == 0)
		{
			completed++;
			if(completed == PRINT)
			{
				for(unsigned long x = i; x <= LIMIT; x++)
				{
					if(bit_array_getbit(pole,x) == 0)
					{
						printf("%lu\n", x);
					}
				}
			}
		}
	}
 	
return 0;
}	

#endif