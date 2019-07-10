/* Projekt c. 2 - Iteracni vypocty
 * Autor: David Valecky
 * Login: xvalec01
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// maximalni pocet iteraci dle zadani
#define MAX_ITERATION 13
// pri devate iteraci se uz se nemeni hodnota tangensu v porovnani s hodnotou z matematicke knihovny
#define EFFECTIVE_ITERATION 10
// standartni hodnota pro vysku, ve ktere se meric nachazi
#define STANDART_HEIGHT 1.5 
// nejvetsi mozna vyska ve ktere se nachazi pristroj
#define MAX_HEIGHT 100
// fuknce pro vypsani napovedy
void help()
{
	printf("\nProgram se spousti v nasledujici podobe:\n"
	"     ./proj2 --tan A N M\n"
	"Toto vypocita hodnotu tangens zadaneho uhlu v zadanem intervalu\n"
	"pomoci trech ruznych funkci:\n"
	"               1. funkce z matematicke knihovny jazyka C\n"
	"               2. tayloruv polynom\n"
	"               3. zretezeny zlomek\n"
	"A vypise odchylky od funkce z knihovny.\n"
	"  nebo\n"  
	"     ./proj2 [-c X] -m A [B]\n"
	"         kde [-c X] je nepovinne, standartne je zadane jako 1.5 m\n"
	"Argumenty programu:\n"
	"--tan     -porovna presnosti vypoctu tangens uhlu A (v radianech)\n"
	"           mezi volanim tan z matematicke knihovny,\n"
	"           a vypoctu tangens pomoci Taylorova polynomu a zretezeneho zlomku.\n"
	"      A   -uhel alfa (v radianech)\n"
	"      N   -prvni iterace\n"
	"      M   -posledni iterace\n"
	"-c        -argument pro nastaveni vysky, ve ktere se pristroj nachazi\n"
    "      X   -vyska uvedena v metrech (max. 100)\n"
	"-m        -argument pro vypocitani delky a vysky\n"
	"      A   -uhel (v radianech), ktery svira vodorovny smer od pristroje\n"
	"           se smerem od pristroje ke spodku objektu\n"
	"      B   -uhel (v radianech), ktery svira vodorovny smer od pristroje\n"
	"           se smerem od pristroje k vrsku objektu\n");
}

// funkce pro vypocet hodnoty pro tangens zadaneho uhlu pomoci taylerova polynomu
double taylor_tan(double x, unsigned int n)
{
    double denominator[MAX_ITERATION] = {1,3,15,315,2835,155925,6081075,638512875,
    		10854718875,1856156927625,194896477400625,
            49308808782358125,3698160658676859375};

    double nominator[MAX_ITERATION] = {1,1,2,17,62,1382,21844,929569,  
            6404582,443861162,18888466084,
            113927491862,58870668456604};
    double result = 0; 
    double x2 = x*x;

    for(unsigned int i = 0;i < n;)
    {
        result += x * nominator[i] / denominator[i];
        x *= x2;
        i++;
    }
    return result;
}
// funkce pro vypocet hodnoty pro tangens zadaneho uhlu pomoci zretezeneho zlomku
double cfrac_tan(double x, unsigned int n)
{
	double fraction = 0;
	double nominator = x*x;

	for (unsigned int i = n-1; i>=1; i--)
	{
		int number = (i + 1) * 2 - 1;
		fraction = nominator / (number - fraction);
	}

	return x / (1.0 - fraction);
}
// funkce pro vypocet vzdalenosti od objektu
double distance_from_object(int argc, char *argv[])
{
	double distance = 0;
	if (argc == 4 || argc == 3)
	{
		// vyska ve ktere se meric nachazi
		double c = STANDART_HEIGHT;
		double angle_a = atof(argv[2]);
		// vzdalenost je podilem vysky merice a tangens uhlu alfa (trigonometrie)
		distance = c / cfrac_tan(angle_a, EFFECTIVE_ITERATION); 
	}
	else
	{
		double c = atof(argv[2]);
		double angle_a = atof(argv[4]);
		distance = c / cfrac_tan(angle_a, EFFECTIVE_ITERATION);
	}

	return distance;
}
// funkce pro vypocet vysky objektu
double height_of_object(int argc, char *argv[])
{
	double height = 0;
	if (argc == 4) 
	{
		double c = STANDART_HEIGHT; 
		double angle_b = atof(argv[3]); 
		// vyska je soucin vzdalenosti od objektu a hodnoty tangens uhlu beta
		// + vyska v ktere se nachazi meric 
		height = (distance_from_object(argc,argv) * cfrac_tan(angle_b, EFFECTIVE_ITERATION)) + c;
	}
	else 
	{
		double c = atof(argv[2]);
		double angle_b = atof(argv[5]);
		height = (distance_from_object(argc,argv) * cfrac_tan(angle_b, EFFECTIVE_ITERATION)) + c;
	}

	return height;
}
// funkce pro kontrolu vstupu
int errors(int argc, char **argv)
{
	if (argc < 2)
		return -1;
	if (argc == 2 && strcmp(argv[1],"--help") == 0)
	{
		help();
		return 6;
	}
	else if ((strcmp(argv[1],"-c") == 0) && (strcmp(argv[3],"-m")) == 0 && argc == 6)
	{
		if ((atof(argv[4]) > 0 && atof(argv[4]) <= 1.4 
			&& atof(argv[5]) > 0 && atof(argv[5]) <= 1.4)
			&& atof(argv[2]) <= 100 && atof(argv[2]) > 0)
			return 1;
		else
			return -1;
	}
	else if ((strcmp(argv[1],"-c") == 0) && (strcmp(argv[3],"-m")) == 0 && argc == 5)
	{
		if (atof(argv[4]) > 0 && atof(argv[4]) <= 1.4 
			&& atof(argv[2]) <= 100 && atof(argv[2]) > 0)
			return 2;
		else
			return -1;
	}
	else if ((strcmp(argv[1],"-m")) == 0 && argc == 4)
	{
		if (atof(argv[2]) > 0 && atof(argv[2]) <= 1.4 
			&& atof(argv[3]) > 0 && atof(argv[3]) <= 1.4 )
			return 3;
		else
			return -1;
	}
	else if ((strcmp(argv[1],"-m")) == 0 && argc == 3)
	{
		if (atof(argv[2]) > 0 && atof(argv[2]) <= 1.4 )
			return 4;
		else
			return -1;
	}
	else if ((strcmp(argv[1],"--tan")) == 0 && argc == 5)
	{
		if (atoi(argv[3]) <= atoi(argv[4]) && atoi(argv[3]) > 0  
			&& atoi(argv[3]) <= MAX_ITERATION && atoi(argv[4]) > 0 && atoi(argv[4]) <= MAX_ITERATION)
			return 5;
		else
			return -1;
	}
	else
	{
		return -1;
	}
}

int main(int argc, char *argv[])
{ 	
	if (errors(argc, argv) == -1)
	{
		fprintf(stderr,"Nespravne zadane argumenty!\n");
		return 1;
	}
	if (errors(argc, argv) == 6)
		return 0;
	if (errors(argc, argv) == 1)
	{
		printf("%.10e\n", distance_from_object(argc,argv));
		printf("%.10e\n", height_of_object(argc,argv));
		return 0;
	}
	if (errors(argc, argv) == 2)
	{
		printf("%.10e\n", distance_from_object(argc,argv));
		return 0;
	}
	if (errors(argc, argv) == 3)
	{
		printf("%.10e\n", distance_from_object(argc,argv));
		printf("%.10e\n", height_of_object(argc,argv));
		return 0;
	}
	if (errors(argc, argv) == 4)
	{
		printf("%.10e\n", distance_from_object(argc,argv));
		return 0;
	}
	// jestlize je zadan vypis hodnot funkci pomoci parametru --tan
	if (errors(argc,argv) == 5)
	{
		double angle = atof(argv[2]);
		unsigned int interval1 = atoi(argv[3]);
		unsigned int interval2 = atoi(argv[4]);
		double tan_result = tan(angle);	
		// aby se vypisovaly jen urcite iterace dle zadani	
		while (interval1<=interval2)
		{
			double cfrac_tan_result = cfrac_tan(angle, interval1);
			double taylor_tan_result = taylor_tan(angle, interval1);
			printf("%d %e %e %e %e %e\n", interval1, tan_result, taylor_tan_result, 
					fabs(taylor_tan_result - tan_result), cfrac_tan_result,
					fabs(cfrac_tan_result - tan_result));
			interval1++;
		}
	}

	return 0;
}