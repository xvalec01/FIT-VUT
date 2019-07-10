
/*
 * Soubor:		bit_array.h
 * Projekt:		IJC:DU1
 * Autor:		David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:	gcc 5.4.0 ubuntu
 * Datum:		20. 3. 2018
 * Popis:		Makra a inline funkce pro práci s bitovým polem
*/

/* Podmineny preklad BIT_ARRAY_H */
#ifndef BIT_ARRAY_H
#define BIT_ARRAY_H


#include <stdio.h>
#include <limits.h>
#include "error.h"

/* Definice datoveho typu pole bitu */
typedef unsigned long bit_array_t[];

/** Pomocná makra **/
/* Výpočet bitů unsigned longu */
#define UNSL_BIT (sizeof(unsigned long) * CHAR_BIT)

/* Výpočet počtu indexů pole */
#define UL_COUNT(velikost) ((((velikost) % (UNSL_BIT)) == 0) ? ((velikost) / UNSL_BIT) : ((velikost) / UNSL_BIT + 1))

/** Povinný makra **/
/* Inicializuje pole, 0. index je žádaný počet bitů v poli, zbytek nuluje */
#define bit_array_create(jmeno_pole, velikost) \
unsigned long jmeno_pole[UL_COUNT(velikost)+1] = {(unsigned long)(velikost), 0}

/* Vrací velikost pole z nultého indexu */
#ifdef USE_INLINE
	inline unsigned long bit_array_size(bit_array_t jmeno_pole)
	{
		return jmeno_pole[0];
	}
#else
#define bit_array_size(jmeno_pole) (jmeno_pole[0])
#endif

/* Funkce nastavující požadovaný bit v zadaném poli na určený výraz (0 nebo 1) */
#ifdef USE_INLINE
	inline void bit_array_setbit(bit_array_t jmeno_pole, unsigned long index, int vyraz)
	{
		if(((*jmeno_pole) <= index) || index < 0)
		{
			error_exit("Index %lu mimo rozsah 0..%lu", index, bit_array_size(jmeno_pole));
		}
		if(vyraz) 
		{ 
			jmeno_pole[(index / (UNSL_BIT)) + 1] |= ((unsigned long)1 << (index % (UNSL_BIT)));
		} 
		else
		{
    		jmeno_pole[(index / (UNSL_BIT)) + 1] &= ~((unsigned long)1 << (index % (UNSL_BIT)));
		}
	}
#else
#define bit_array_setbit(jmeno_pole,index,vyraz) \
	do { \
		if(((*jmeno_pole) <= index) || index < 0) \
			{ \
				error_exit("Index %lu mimo rozsah 0..%lu", (unsigned long)index, (unsigned long)bit_array_size(jmeno_pole)); \
			} \
		if(vyraz) \
			{ \
				jmeno_pole[(index / (UNSL_BIT)) + 1] |= ((unsigned long)1 << (index % (UNSL_BIT))); \
			} \
		else \
			{ \
	    		jmeno_pole[(index / (UNSL_BIT)) + 1] &= ~((unsigned long)1 << (index % (UNSL_BIT))); \
			} \
	} while(0)
#endif

/* Funkce získávající hodnotu požadovaného bitu v zadaném poli */
#ifdef USE_INLINE
	inline unsigned long bit_array_getbit(bit_array_t jmeno_pole, unsigned long index)
	{
		if(((*jmeno_pole) < index) || index < 0)
		{
			error_exit("Index %lu mimo rozsah 0..%lu", (unsigned long)index, bit_array_size(jmeno_pole));
		}
		return (jmeno_pole[(index / (UNSL_BIT)) + 1] & ((unsigned long)1 << (index % (UNSL_BIT))));

	}
#else
#define bit_array_getbit(jmeno_pole,index) \
	((*jmeno_pole) < index || ( index < 0)) ? \
	error_exit("Index %lu mimo rozsah 0..%lu", (unsigned long)index, (unsigned long)(jmeno_pole[0])), 0 : \
		(unsigned long)(jmeno_pole[(index / UNSL_BIT) + 1] & ((unsigned long)1 << (index % UNSL_BIT))) 
#endif

#endif