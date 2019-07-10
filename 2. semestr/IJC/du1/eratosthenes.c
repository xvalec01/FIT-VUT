/*
 * Soubor:      eratosthenes.c
 * Projekt:     IJC:DU1
 * Autor:       David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:   gcc 5.4.0 ubuntu
 * Datum:       20. 3. 2018
 * Popis:       Eratosthenesovo síto
*/
#ifndef ERATOSTHENES_C
#define ERATOSTHENES_C

#include "eratosthenes.h"

void eratosthenes(bit_array_t jmeno_pole)
{

	unsigned long x = bit_array_size(jmeno_pole);
	unsigned long limit = sqrt(x);

    for (unsigned long i = 2; i < limit; i++)
    {
        if (bit_array_getbit(jmeno_pole, i) == 0)
        {
            for (unsigned long j = 2; j * i < x; j++)
            {
                bit_array_setbit(jmeno_pole, j*i, 1);
            }
        }
	}
}

#endif