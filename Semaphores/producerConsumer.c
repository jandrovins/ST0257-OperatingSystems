#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#define BUFFER_SIZE 10

void *producer (void *) ;
void *consumer(void *) ;
//Semaphores empty:emptyspots full:fullspots
sem_t empty, full;
//Define the buffer
int buffer[BUFFER_SIZE];
int ID[BUFFER_SIZE] ;
int in = 0 ; int out = 0 ;
int nextProduced = 0 ;
main() {
    int i ;
    pthread_t TID[10] ;
    //Initialize the emptyspots
    sem_init(&empty, 0, BUFFER_SIZE) ;
    //Initialize the fullspots
    sem_init(&full, 0, 0) ;
    
    //Initialize the buffer
    for(i = 0; i < 10; i++) {
        ID[i] = i ;
        buffer[i] = -1 ;
    }
	//Create the producer
        pthread_create(&TID[0], NULL, producer, (void *) &ID[0]) ;
	printf("Producer ID = %d created!\n", 0) ;
	//Create the consumer        
	pthread_create(&TID[1], NULL, consumer, (void *) &ID[1]) ;
	printf("Consumer ID = %d created!\n", 1) ;

    //Start each thread
    for(i = 0; i < 2 ; i++) {
        pthread_join(TID[i], NULL) ;
    }
}

/*
	Producer task

*/
void *producer(void *Boo) {
    int *ptr;
    int ID;
    ptr = (int *) Boo;
    ID = *ptr;
    while (1) {
        nextProduced++; //Producing Integers
        /* Check to see if Overwriting unread slot */
	
		//Wait for emptyspots        
		sem_wait(&empty);
        

        if (buffer[in] != -1) {
            printf("Synchronization Error: Producer %d Just overwrote %d from Slot %d\n", ID, buffer[in], in);
            exit(0);
        }

        /* Looks like we are OK */
        buffer[in] = nextProduced;
        printf("Producer %d. Put %d in slot %d\n", ID, nextProduced, in);
        in = (in + 1) % BUFFER_SIZE;
        printf("incremented in!\n");

        //Release fullspots
        sem_post(&full);
		//Sleep for a random time
		if (rand()%100 < 30)
			ssleep(rand()%3);

    }
 }

/*
    Consumer task
 
*/

void *consumer (void *Boo) {
    static int nextConsumed = 0 ;
    int *ptr ;
    int ID ;
    ptr = (int *) Boo ;
    ID = *ptr ;
    while (1) {
	//Wait for fullspots        
	sem_wait(&full);
        

        nextConsumed = buffer[out];
        /*Check to make sure we did not read from an empty slot*/
        if (nextConsumed == -1) {
            printf("Synch Error: Consumer %d Just Read from empty slot %d\n", ID, out) ;
            exit(0) ;
        }
        /* We must be OK */
        printf("Consumer %d Just consumed item %d from slot %d\n", ID, nextConsumed, out) ;
        buffer[out] = -1 ;
        out = (out + 1) % BUFFER_SIZE;


        //Release the emptyspots
        sem_post(&empty);
	//Sleep for a random time
	if (rand()%100 < 30)
	  sleep(rand()%3);
 

   }
}
