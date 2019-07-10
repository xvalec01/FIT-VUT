/*
 * Soubor:		ppm.h
 * Projekt:		IJC:DU1
 * Autor:		David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:	gcc 5.4.0 ubuntu
 * Datum:		20. 3. 2018
 * Popis:		rozhraní se strukturou na ukládání rozměru obrázku a výpis pro zakódovaný text
*/
#ifndef PPM_H
#define PPM_H

#define MAX_VELIKOST 3000000
#define RGB 3
#define VELIKOST(xsize,ysize) (xsize * ysize * RGB * sizeof(char))

struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[];	// RGB bajty, celkem 3*xsize*ysize
};

struct ppm *ppm_read(const char * filename);
int ppm_write(struct ppm *p, const char * filename);

#endif