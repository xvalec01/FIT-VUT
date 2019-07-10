/* Projekt 1 - práce s textem 
 * Autor: David Valecký 
 * Login: xvalec01 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
// maximální index dle zadání
#define MAXINDEX 100

//Tato funkce slouží jako vymazání původních hodnot v poli
void eraseArray(char array[MAXINDEX])
{
	for (int i = 0; i < MAXINDEX; i++)
	{
		array[i] = '\0';
	}
}

// Tato funkce kontroluje zda-li se v poli znaků, které může uživatel zadat, znak už nachází.
int charEnable(char cityChar, char characters[MAXINDEX])
{
	int i = 0;
	int included = 0;
	// zajišťuje, aby se první nalezený znak zapsal do pole
	if (characters[0] == '\0')
	{
		characters[0] = cityChar;
	}	
	// zjišťuje, jestli se tam znak už nachází
	while (characters[i] != '\0' && i < MAXINDEX)
	{
		if (characters[i] == cityChar)
			included = 1;
		i++;
	}
    // jestliže tam znak není, úmístí ho na poslední místo v poli
	if (included == 0)
	{
		characters[(int)strlen(characters)] = cityChar;
	}
	return 1;
}
// seřadí písmena v poli podle abecedy
void sortArray(char characters[MAXINDEX])
{
	char temp;
	for(int i = 0; i < (int)strlen(characters); i++)
	{
	    for(int j = 0; j < ((int)strlen(characters) - i); j++)
	    {
	        if(characters[j] >= characters[j+1] && characters[j+1] != '\0')
	        {
	           	temp=characters[j];
	           	characters[j]=characters[j+1];
	           	characters[j+1]=temp;
	        }
	    }
	}
	printf("Enable: %s\n", characters);
}
// zmenšuje písmena aby program fungoval jako case insensitive
void makeCharactersUppercase(char *string)
{
	int i = 0;
	char c;
	while (string[i] != '\0' && i < MAXINDEX-1)
	{
		if (isalpha(string[i]))
		{
			c = string[i];
			string[i] = toupper(c);
		}
		i++;
	}
}


// Zkopíruje znak po znaku pole prvního parametru do pole druhého parametru
void copyArray(char cityName[MAXINDEX], char city[MAXINDEX])
{
	char c;
	for (int i = 0; i < (int)strlen(cityName) && i < MAXINDEX-1 && cityName[i] != '\0'; ++i)
	{
		c = cityName[i];
		city[i] = c;
	}
}
// funkce zajišťující správný výstup
void outputMatches(char characters[MAXINDEX],char city[MAXINDEX] ,int match)
{
	// provede se, když se nic nenašlo
	if (match == 0)
		printf("Not found\n");
	// jestliže už je nalezen více jak 1 znak, vypíše je pomocí řadící funkce sortArray
	// jestliže jen jeden, vypíše se rovnou to město
 	if (characters[0] != '\0' && match >= 2)
 		sortArray(characters);
 	else if (match == 1)
	{
		printf("Found: %s", city);
 	}
}
// Jestliže se v souboru adres nachází neplatný znak, vrací hodnotu 1 
int errors(char cityName[MAXINDEX])
{
	int i = 0;
	while(cityName[i] != '\0' && i < MAXINDEX-1 )
	{
		
		if (isalpha(cityName[i]) || isblank(cityName[i])
			|| isdigit(cityName[i]) || isspace(cityName[i]) 
			|| cityName[i] == '-' )
		{
			i++;
			continue;	
		}

		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{	
	if (errors(argv[1]))
	{
		fprintf(stderr,"Invalid argument\n");
			return -1;
	}
	char cityName[MAXINDEX];
	char chars[MAXINDEX];
	char cities[MAXINDEX];
	char city[MAXINDEX];
	int match = 0;

	eraseArray(chars);
	eraseArray(city);
	eraseArray(cityName);

	if (argc == 1)
	{
		while(fgets(cityName,MAXINDEX,stdin))
		{
			if (errors(cityName))
			{
				fprintf(stderr,"Invalid input data\n");
				return -1;
			}
			charEnable(cityName[0],chars);
		}
		sortArray(chars);
	}
	else if (argc == 2)
	{	
		while(fgets(cityName,MAXINDEX,stdin))
		{
			if (errors(cityName))
			{
				fprintf(stderr,"Invalid input data\n");
				return -1;
			}
			copyArray(cityName,cities);
			makeCharactersUppercase(cities);
			makeCharactersUppercase(argv[1]);

			if (strncmp(cities,argv[1],(int)strlen(argv[1])) == 0)
			{
				// ukládá název města, abych ho vypsal jen při nálezu jednoho možného znaku
				copyArray(cityName,city);
				if (strncmp(cities,argv[1],(int)strlen(argv[1])) == 0 && cities[(int)strlen(argv[1])] == '\n')
				{
					printf("Found: %s", cityName);
					match = 2;
					break;
				}
				/* jestliže jsem našel alespoň jednu shodu, proměná match se zvedne o 1
				 * aby se později nevypsalo "Not Found: i když jsem našel shodu
				 * a když bude match = 1, vypíše se: "Found: město"
				 */
				if (charEnable(cityName[(int)strlen(argv[1])],chars))
					match += 1;
			}
			eraseArray(cityName);
		}
		outputMatches(chars,city,match);
	}
	else 
	{
		fprintf(stderr,"Too many arguments!\n");
		return -1;
	}
	return 0;
}
