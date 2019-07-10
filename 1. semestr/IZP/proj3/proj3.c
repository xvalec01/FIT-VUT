/**
 * Kostra programu pro 3. projekt IZP 2017/18
 * David Valecký xvalec01
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

/**
 * Struktura pro objekt se souradnicemi.
 */
struct obj_t {
    int id; /// identifikator
    float x; /// souradnice x
    float y; /// souradnice y
};

/**
 * Struktura pro shluk objektu.
 */
struct cluster_t {
    int size; /// pocet objektu ve shluku
    int capacity; /// kapacita shluku, maximalni počet objektu, ktere se muzou v shluku nachazet
    struct obj_t *obj; /// pole objektu v danemu shluku
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/**
 * Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 * Ukazatel NULL u pole objektu znamena kapacitu 0.
 *
 * @param c Shluk pro inicializaci
 * @param cap Zadana kapacita shluku
 */
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    c->capacity = cap;
    c->obj = malloc(cap * sizeof(struct obj_t));
    if(c->obj == NULL)
    {
      c->capacity = 0;
      fprintf(stderr,"Nepodarilo se alokovat pamet.\n");
      exit(-1);
    }

}

/**
 * Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 *
 * @param c Shluk pro uvolneni z pameti
 */
void clear_cluster(struct cluster_t *c)
{
	free(c->obj);
	if (c->obj != NULL)
		c->obj = NULL;
    
    c->size = 0;
    c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/**
 * Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 *
 * @param c Shluk pro zmenu velikosti
 * @param new_cap Nova zadana kapacita shluku
 * @return Shluk s novou kapacitou, jestlize nic nereallokuje
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

// Jestliže je kapacita už před požadavkem změny větší/rovna nové kapacitě, vratí původní strukturu
    if (c->capacity >= new_cap) 
        return c;
// novou velikost definujeme jako velikost, kterou zabírá objekt, vynásobením novou kapacitou 
    size_t size = sizeof(struct obj_t) * new_cap;
// *arr je "prázdný" ukazatel, do kterého se přiřadí objekty z clustru a k nim jejich nová velikost
    void *arr = realloc(c->obj, size);
    if (arr == NULL) // jestliže se do arr nic nového nereallokovalo, funkce vrátí NULL;
        return NULL;
// do objektu se přiřadí objekt z prázdného ukazatele, který do teď na nic neukazoval
    c->obj = (struct obj_t*) arr;
    c->capacity = new_cap;
    return c;
}

/**
 * Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt nevejde.
 * 
 * @param c Shluk pro pridani objektu
 * @param obj Objekt, ktery pridam na konec shluku
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if(c->size >= c->capacity)
    {
    	int new_cap = c->capacity + CLUSTER_CHUNK;
    	if(resize_cluster(c, new_cap) == NULL)
    	{
        	fprintf(stderr,"Nepodarilo se alokovat pamet.\n");
    	}
    }

    c->obj[c->size] = obj;
    c->size += 1;
}

/**
 * Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/**
 * Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 * Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 * Shluk 'c2' bude nezmenen.
 *
 * @param c1 Shluk do ktereho pridavam z c2
 * @param c2 Shluk objektu z ktereho kopiruji
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i < c2->size; i++) 
    {
        append_cluster(c1, c2->obj[i]);
    }

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/**
 * Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 * (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 * pocet shluku v poli.
 *
 * @param carr Pole shluku
 * @param narr Pocet shluku v poli
 * @param idx Index shluku v poli
 * @return Vrati pocet shluku o jeden mensi
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(&carr[idx]);
    for(int i = idx; i < narr - 1; i++)
    {
    	carr[i] = carr[i + 1];
    }
    return (narr - 1);
}

/**
 * Pocita Euklidovskou vzdalenost mezi dvema objekty.
 *
 * @param o1 Objekt o1 kde resime vzdalenost ku objektu o2
 * @param o2 Objekt o2
 * @return Vzdalenost dvou objektu vypocitana pomoci Pythagorovy věty
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float distance = ((o1->x - o2->x)*(o1->x - o2->x)) + ((o1->y - o2->y)*(o1->y - o2->y));

    return sqrtf(distance);
}

/**
 * Pocita vzdalenost dvou shluku.
 *
 * @param c1 Shluk kde resime vzdalenost ku shluku c2
 * @param c2 Shluk 
 * @return Vraci vzdalenost vypocitanou pomoci prumeru vzdalenosti jednoho objektu vuci ostatnim objektum v druhem shluku 
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float distance, sum = 0;
    int counter = 0;

    for (int j = 0; j < c1->size; j++) 
    {
        for (int i = 0; i < c2->size; i++) 
        {
        	//sčítání vzdálenosti jednoho objektu ku všem ostatním v druhém shluku
            sum += obj_distance(&c1->obj[j], &c2->obj[i]); 
            counter++;
        }
    }

    distance = sum / counter;

    return distance;
}

/**
 * Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 * hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 * 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 * adresu 'c1' resp. 'c2'.
 *
 * @param carr Pole shluků
 * @param narr Pocet prvnku v poli s hluku
 * @param c1 Index v poli na zapamatovani
 * @param c2 Index v poli na zapamatovani
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float distancePrev = INFINITY;
    float distance;

    for (int j = 0; j < narr; j++) 
    {
        for (int i = j + 1; i < narr; i++) 
        {
            distance = cluster_distance(&carr[j], &carr[i]);
            if (distance < distancePrev) 
            {
                distancePrev = distance;
                *c1 = j;
                *c2 = i;
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    FILE *file;
    int num_of_obj = 0;
    struct obj_t object;

    if((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr,"Nepodarilo se otevrit soubor %s.\n",filename);
        return 0;
    }

    int count = fscanf(file, "count=%d", &num_of_obj);

    //number returned by sscanf must be 1 and number of objects must be greater than 0
    if (num_of_obj <= 0 || count != 1) 
    {
        fprintf(stderr, "Pocet objektu je ve spatnym formatu\n.");
        if (fclose(file) == EOF)
        {
            fprintf(stderr, "Nepodarilo se zavrit soubor %s.\n", filename);
        }
        return 0;
    }

    *arr = malloc(sizeof (struct cluster_t) * num_of_obj);

    if (arr == NULL) 
    {
        fprintf(stderr, "Nepodarilo se alokovat pamet pro objekty.\n");
        if (fclose(file) == EOF)
        {
            fprintf(stderr, "Nepodarilo se zavrit soubor %s.\n", filename);
        }
        return 0;
    }

    for (int i = 0; i < num_of_obj; i++) 
    {
        int obj_variables = fscanf(file, "%d %f %f", &object.id, &object.x, &object.y);

        if (obj_variables != 3) 
        {
            fprintf(stderr, "Usporadani objektu je ve spatnem formatu. Spravny format: [id] [x] [y]\n");
            if (fclose(file) == EOF)
            {
                fprintf(stderr, "Nepodarilo se zavrit soubor %s.\n", filename);
            }
            for (int j = 0; j < i; j++)
                clear_cluster(&(*arr)[i]);

            return 0;
        }
        // Podle zadání nesmí být souřadnice větší jak 1000 a nižší než nula
        if ((object.x < 0 || object.x > 1000) || (object.y < 0 || object.y > 1000)) 
        {
            fprintf(stderr, "Souradnice objektu nejsou v rozsahu <0,1000>.\n");
            if (fclose(file) == EOF)
            {
                fprintf(stderr, "Nepodarilo se zavrit soubor %s.\n", filename);
            }
        	for (int j = 0; j < i; j++)
            	clear_cluster(&(*arr)[i]);

        	return 0;
        }
        init_cluster(&(*arr)[i], 1);
        append_cluster(&(*arr)[i], object);
    }

    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Nepodarilo se zavrit soubor %s.\n", filename);
    }

    return num_of_obj;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;

    if (argc > 3) 
    {
        fprintf(stderr, "Prilis mnoho argumentu.\n");
        return 1;
    }

    if (argc == 1) 
    {
        fprintf(stderr, "Chybí soubor, z ktereho se ma cist.\n");
        return 1;
    }

    char filename[100];
    char *ptr;
    long num_of_clusters = 1;
    int num_of_obj, c1, c2;
    strcpy(filename, argv[1]);

    if (argc == 3) 
    {
        num_of_clusters = strtol(argv[2], &ptr, 10);
        if (*ptr != '\0' || num_of_clusters < 0) 
        {
            fprintf(stderr, "Spatne zadany pocet shluku.\n");
            return 1;
        }
    }

    num_of_obj = load_clusters(filename, &clusters);

    if (num_of_obj == 0) 
    {
        fprintf(stderr, "Nenacetly se zadne objekty.\n");
        return 1;
    }

    if (atoi(argv[2]) > num_of_obj)
    {
        fprintf(stderr, "Pocet zadanych clusteru je vetsi nez pocet objektu v souboru.\n");
        return 1;
    }

    while (num_of_obj > num_of_clusters) 
    {
        find_neighbours(clusters, num_of_obj, &c1, &c2);
        merge_clusters(&clusters[c1], &clusters[c2]);
        num_of_obj = remove_cluster(clusters, num_of_obj, c2);
    }

    print_clusters(clusters, num_of_obj);

    for (int i = 0; i < num_of_obj; i++) 
        clear_cluster(&clusters[i]);

    free(clusters);

    return 0;
}
