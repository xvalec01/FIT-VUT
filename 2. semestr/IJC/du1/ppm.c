/*
 * Soubor:		ppm.c
 * Projekt:		IJC:DU1
 * Autor:		David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:	gcc 5.4.0 ubuntu
 * Datum:		20. 3. 2018
 * Popis:		
*/
#ifndef PPM_C
#define PPM_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "ppm.h"

struct ppm *ppm_read(const char *filename)
{
	FILE *fp;
	struct ppm *image = NULL;
	int read;
	unsigned xsize, ysize;

	fp = fopen(filename,"r");

	if(fp == NULL)
	{
		warning_msg("Soubor %s se nepodarilo otevrit.\n",filename);
		return NULL;
	}

	read = fscanf(fp,"P6 %u %u 255 ", &xsize, &ysize);

	if(read != 2)
	{
		fclose(fp);
		warning_msg("Soubor %s není validní\n",filename);
		return NULL;
	}

	if(VELIKOST(xsize,ysize) > MAX_VELIKOST)
	{
		warning_msg("Soubor %s je moc velký.\n",filename);
		return NULL;
	}

	image = malloc(VELIKOST(xsize,ysize)+sizeof(struct ppm));
	image->xsize = xsize;
	image->ysize = ysize;

	if(image == NULL)
	{
		fclose(fp);
		warning_msg("Nepodařilo se správně allocovat paměť.\n");
		return NULL;
	}

	read = fread(image->data,sizeof(char),VELIKOST(xsize,ysize),fp);

	if(read != VELIKOST(xsize,ysize) || ferror(fp) != 0 || fgetc(fp) != EOF)
	{
		fclose(fp);
		free(image);
		warning_msg("Obrázek není validní.\n");
		return NULL;
	}


	fclose(fp);
	return image;
}

int ppm_write(struct ppm *p, const char *filename)
{
	FILE *fp;
	int write;

	if(p == NULL)
	{
		warning_msg("Špatná vstupní data.\n");
		return -2;
	}

	fp = fopen(filename,"w");

	if(fp == NULL)
	{
		warning_msg("Nemohl se otevřít soubor %s.\n",filename);
		return -2;
	}

	write = fprintf(fp,"P6 %u %u 255 ", p->xsize, p->ysize );

	if (write < 1)
	{
		warning_msg("Nemohlo se zapsat do souboru %s.\n",filename);
		fclose(fp);
		return -2;
	}

	write = fwrite(p->data,sizeof(char),VELIKOST(p->xsize,p->ysize),fp);

	if(write != VELIKOST(p->xsize,p->ysize))
	{
		warning_msg("Není možné zapsat do souboru či vypsat celou zprávu.\n");
		fclose(fp);
		return -2;
	}

	fclose(fp);

	return 0;
}

#endif