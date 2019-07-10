/*
 * Soubor:      proj2.c
 * Projekt:     IOS:proj2
 * Autor:       David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:   gcc 5.4.0 ubuntu
 * Datum:       1. 5. 2018
 * Popis:       Souběžně běžící procesy, synchonizované pomocí semaforů
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>

#define SEMAPHORE1_NAME "/semaphore_1_xvalec01"
#define SEMAPHORE2_NAME "/semaphore_2_xvalec01"
#define SEMAPHORE3_NAME "/semaphore_3_xvalec01"
#define SEMAPHORE4_NAME "/semaphore_4_xvalec01"
#define MAX_WAIT_VALUE 1000 // 1000 milisekund, maximální hodnota doby čekání
#define FILE_ERROR -1
#define INVALID_INPUT 1
#define SUCCESS 0

sem_t *mutex = NULL; // protective semaphor
sem_t *busstop = NULL; // allow riders to go on busstop
sem_t *board = NULL; // allow riders to go to bus
sem_t *allow = NULL;

int *row = NULL; // counting every record written to file/stdout
int *waiting = NULL; // riders waiting at bus stop
int *riders = NULL; // number of riders whom were already generated
int *travelers = NULL;
// IDs for shared variables
int rowID = 0; 
int waitingID = 0;
int ridersID = 0;
int travelersID = 0;

void clean_resources()
{
	sem_close(mutex);
	sem_close(board);
	sem_close(busstop);
	sem_close(allow);

	// remove semaphores
	sem_unlink(SEMAPHORE1_NAME);
	sem_unlink(SEMAPHORE2_NAME);
	sem_unlink(SEMAPHORE3_NAME);
	sem_unlink(SEMAPHORE4_NAME);


	shmctl(waitingID, IPC_RMID, NULL);
	shmctl(rowID, IPC_RMID, NULL);
	shmctl(ridersID, IPC_RMID, NULL);
	shmctl(travelersID, IPC_RMID, NULL);

}

void cleanre_exit()
{
	fprintf(stderr, "Something went wrong in allocating resources.\n");
	clean_resources();
	exit(1);
}

void init_resources(FILE *fp)
{
	if ((rowID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1)
		cleanre_exit();
	if ((waitingID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1)
		cleanre_exit();
	if ((ridersID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1)
		cleanre_exit();
	if ((travelersID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1)
		cleanre_exit();

	if ((riders = shmat(ridersID, NULL, 0)) == NULL)
		cleanre_exit();
	if ((row = shmat(rowID, NULL, 0)) == NULL)
		cleanre_exit();
	if ((waiting = shmat(waitingID, NULL, 0)) == NULL)
		cleanre_exit();
	if ((travelers = shmat(travelersID, NULL, 0)) == NULL)
		cleanre_exit();

	// create semaphores
	if ((mutex = sem_open(SEMAPHORE1_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
		cleanre_exit();
	if ((board = sem_open(SEMAPHORE2_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
		cleanre_exit();
	if ((busstop = sem_open(SEMAPHORE3_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
		cleanre_exit();
	if ((allow = sem_open(SEMAPHORE4_NAME, O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
		cleanre_exit();

	*row = 1;
	*waiting = 0;
	*riders = 0;
	*travelers = 0;
	setbuf(stdout, NULL); // prevent buffering of the output
	setbuf(fp,NULL); // prevent buffering of the output
}

void bus_process(int capacity, int delay)
{
	if(capacity && delay) {};
	exit(SUCCESS);
}

void generate_riders(int count_of_processes, int wait_time, FILE *fp)
{	
	pid_t rider;
	for (int i = 0; i < count_of_processes; i++)
	{
		
		rider = fork();

		if (rider == 0) // child
	    {
	    	
	    	(*row)++;
	    	(*riders)++;
	    	fprintf(fp,"%d\t: RID %d\t: start\n", (*row), (*riders));
	    	sem_wait(busstop);
	    	(*row)++;
	    	(*waiting)++;
            fprintf(fp,"%d\t: RID %d\t: enter %d \n", (*row), (*riders), (*waiting));
            sem_wait(board);
            (*row)++;
            fprintf(fp,"%d\t: RID %d\t: boarding\n", (*row), (*riders));
	        sem_wait(allow);
	        (*row)++;
	        fprintf(fp,"%d\t: RID %d\t: finish\n", (*row), (*riders));
	    
	    }
	    else if (rider < 0) // error
	    {
	        cleanre_exit();
	        exit(INVALID_INPUT);
	    }

	    usleep(rand() % wait_time+1);
	}
    waitpid(rider, NULL, 0);
}

int main(int argc, char const *argv[])
{
	if (argc != 5)
	{
		fprintf(stderr, "Wrong count of arguments.\n");
		exit(INVALID_INPUT);
	}

	// ART = max waiting value for riders, ABT = max waiting value for bus
	int count_riders,bus_capacity,ART,ABT;

    // kontrola jestli je argument číslo
    for (int i = 1; i < argc; i++)
    {
    	char *endptr;
    	int argument = strtol(argv[i], &endptr, 10);
		if ((endptr == argv[i]) || (*endptr != '\0') || (argument < 0))
		{
		    fprintf (stderr,"'%s' is not valid argument\n", argv[i]);
		    exit(INVALID_INPUT);
		}
		if ((i == 3 || i == 4) && (argument > MAX_WAIT_VALUE))
		{
			fprintf (stderr,"'%s' is not valid argument\n", argv[i]);
		    exit(INVALID_INPUT);
		}
		switch(i) {

			case 1  :
				count_riders = argument;
				break;

			case 2  :
				bus_capacity = argument;
				break;

			case 3  :
				ART = argument;
				break;

			case 4  :
				ABT = argument;
				break;
		}
    }
    FILE *fp;
	fp = fopen("proj2.out","w");
	if (fp == NULL) 
	{
		fprintf (stderr,"Invalid file opening\n");
		return FILE_ERROR;
	}

    init_resources(fp);
    
    pid_t bus = fork();
    if (bus < 0)
    {
 		cleanre_exit(); // handle error
    }
    if (bus == 0)
    {
    	fprintf(fp,"%d\t: BUS\t: start\n", (*row)++);
    	while((count_riders - (*travelers)) > 0)
    	{
    		sem_wait(mutex);
    			sem_post(busstop);
		    	fprintf(fp,"%d\t: BUS\t: arrival\n", (*row)++);
		    	int nwait = *waiting;
		    	if (nwait != 0)
		    	{
					if (nwait > bus_capacity)
					{
						nwait = bus_capacity;
					}
			    	fprintf(fp,"%d\t: BUS\t: start boarding: %d\n", (*row)++, (*waiting));
			    	// nastupování lidí
			 		for (int i = 0; i < nwait; i++)
			 		{
			 			(*waiting)--;
			 			sem_post(board);
			 		}
			 		fprintf(fp,"%d\t: BUS\t: end boarding: %d\n", (*row)++, (*waiting));
		 		}
		 	sem_post(mutex);
	 		fprintf(fp,"%d\t: BUS\t: depart\n", (*row)++);

	 		if (ABT != 0)
	    		usleep(rand() % ABT);

	    	// vystupování
	    	if (nwait != 0)
	    	{
		    	for (int i = 0; i < nwait; i++)
		    	{
		    		sem_post(allow);
		    		(*travelers)++;
		    	}
	    	}
	    	fprintf(fp,"%d\t: BUS\t: end\n", (*row)++);

    	}
    	fprintf(fp,"%d\t: BUS\t: finish\n", (*row)++);

    }
    else
    {
    	generate_riders(count_riders,ART,fp);
    }
    
    waitpid(bus, NULL, 0);
    clean_resources();
    fclose(fp);

	return SUCCESS;
}