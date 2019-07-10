/**
 * @mainpage Projekt 3
 * @file proj3.h
 * @version 1.0
 * @date December 2017
 * @author David Valecký <xvalec01@stud.fit.vutbr.cz>
 * @brief Simple cluster analysis
 */

/**
 * @brief      Struction of object with coordinates.
 */
struct obj_t {
	/** identifier */
    int id;
    /** coordinate x in cartesian coordinate system */
    float x;
    /** coordinate y in cartesian coordinate system */
    float y;
};

/**
 * @brief      Struction of clusters with pointer on struction of objects. 
 */
struct cluster_t {
	/** number of contained objects */
    int size;
    /** maximal object quantity in cluster*/
    int capacity;
    /** pointer (address) on object */
    struct obj_t *obj;
};

/**
 * Chunk of cluster objects. Value recommended for reallocation.
 */
extern const int CLUSTER_CHUNK;

/**
 * @defgroup clusters Editing of objects.
 * @{
 */

/**
 * @brief      Initializes a cluster.
 *
 * @param      c     Cluster for inicialization.
 * @param      cap   The capacity of cluster. How many objects can be in clusters.
 *
 * @post cluster 'c' will have allocated memmory if there was not error in allocation.
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief      Clears a cluster.
 *
 * @post       All allocated memmory in all cluster objects will be freed
 *
 * @param      c     Required cluster.
 */
void clear_cluster(struct cluster_t *c);

/**
 * @brief      Merges a cluster.
 *
 * @pre        Number of objects in cluster has to be greater than zero.
 *
 * @post       Objects from 'c2' will be appends to the cluster 'c1'. Ojects in cluster 'c' will be sorted ascending according to the ID.
 *
 * @param      c1    Function adds cluster (c2) to this cluster (c1).
 * @param      c2    Cluster, which we want to merge to cluster (c1)
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief      Resizes a cluster.
 *
 * @pre        Number of objects in cluster has to be greater than zero.
 *
 * @post       Capacity will be changed to (new_cap) if there wasn't error before.
 *
 * @param      c        Cluster, where we want to change a capacity.
 * @param      new_cap  The new capacity.
 *
 * @return     Cluster with new capacity.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @}
 */

/**
 * @defgroup array_of_clusters Woking with arrays of clusters.
 * @{
 */

/**
 * @brief      Appends object to the end of the cluster.
 *
 * @pre        Number of objects in cluster has to be greater than zero.
 *
 * @post       At the last position of cluster will be added the object if there wasn't error.
 *
 * @param      c     The pointer on required cluster.
 * @param      obj   The object to add on the end of cluster.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);


/**
 * @brief      Removes a cluster.
 *
 * @param      carr  The carr is array of clusters.
 * @param      narr  The narr is number of clusters in array (carr)
 * @param      idx   Index of cluster in array of clusters.
 *
 * @return     Array of cluster with one cluster less.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief      Counts a distance between objects with coordinates using Pythagorean theorem.
 *
 * @param      o1    The object.
 * @param      o2    The another object.
 *
 * @return     Distance between two objects.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief      Counts an averege of distances between all objects in two clusters (Unweighted pair-group average). 
 *
 * @pre        Number of objects in 'o1' and 'o2' will be more than 0. 
 *
 * @param      c1    The cluster
 * @param      c2    The another cluster
 *
 * @return     Distance bewteen two clusters.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief      Findes the nearest cluster to required cluster. 
 *
 * @post       Index of founded clusters are stored in 'c1' and 'c2'
 *
 * @param      carr  The carr is array of clusters.
 * @param      narr  The narr is number of clusters in array (carr)
 * @param      c1    Index of one of the two nearest clusters.
 * @param      c2    Index of the second nearest cluster.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief      Sorts objects in cluster 'c' ascending according to the identification number.
 *
 * @param      c     Pointer on clusters
 *
 * @post       Objects in cluster 'c' will be sorted ascending according to the ID.
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief      Printes clusters.
 *
 * @param      c     The cluster
 *
 * @post       It will print cluster 'c' to stdout.
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief      Loads objects from file.
 *
 * @pre        If File 'filename' exists. The file will be formatted correctly.
 *
 * @post       For every object in file will be created cluster. All clusters will be stored in array of clusters 'arr'. It will have allocated memmory correctly.
 *
 * @param      filename  Name of file that we want to load
 * @param      arr       Array of clusters
 *
 * @return     Number of loaded rows from file.
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief      Prints clusters to standart output.
 *
 * @post       All objects of all clusters in array of clusters 'carr' will be printed on stdout.
 *
 * @param      The carr is array of clusters.
 * @param      narr  The narr is number of clusters in array (carr)
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */