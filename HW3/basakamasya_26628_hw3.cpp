//Basak Amasya

#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h> 
#include <queue> 
#include <semaphore.h>
using namespace std;

#define NUM_THREADS 10
#define MEMORY_SIZE 150

struct node
{
	int id;
	int size;
};


queue<node> myqueue; // shared que
pthread_mutex_t sharedLock = PTHREAD_MUTEX_INITIALIZER; // mutex
pthread_t server; // server thread handle
sem_t semlist[NUM_THREADS]; // thread semaphores

int thread_message[NUM_THREADS]; // thread memory information
char  memory[MEMORY_SIZE]; // memory size


int index = 0; //to keep the index of the next avaliable space
int remaining_threads = NUM_THREADS; //server will stop when there are no remaining threads left


void my_malloc(int thread_id, int size)
{
	//This function will add the struct to the queue		
	node newnode; //creating the node struct with corresponding id and size
	newnode.id = thread_id;
	newnode.size = size;
	pthread_mutex_lock(&sharedLock); //protecting the shared queue with mutex
	myqueue.push(newnode);
	pthread_mutex_unlock(&sharedLock);
}

void * server_function(void *)
{
	//This function should grant or decline a thread depending on memory size.
	while (remaining_threads != 0){  //until there is no thread left
		
		pthread_mutex_lock(&sharedLock); //entering critical region, protect with mutex
		while(!myqueue.empty()) //if there is an item to be popped out
		{
			node temp = myqueue.front();
			myqueue.pop();
			if (temp.size <= (MEMORY_SIZE - index)) //if there is available space
			{
				thread_message[temp.id] = index; //write the starting index for that thread
				index += temp.size; //update the index
			}
			else
			{
				thread_message[temp.id] = -1;
			}
			remaining_threads --; //decrease the number of remaining threads left
			sem_post(&semlist[temp.id]); //unblock the thread
		}
		pthread_mutex_unlock(&sharedLock); //unlock the shared queue
	}
}

void * thread_function(void * id) 
{
	//This function will create a random size, and call my_malloc
	int * id_pointer = (int*) id;
	int randsize = (rand() % ((MEMORY_SIZE / 6) - 1)) + 1; //generating random size between 1 and memory_size/6

	my_malloc(*id_pointer,randsize);
	
	//Block	
	sem_wait(&semlist[*id_pointer]); //waiting for the server to process the request
	
	//Then fill the memory with id's or give an error prompt		
	if (thread_message[*id_pointer] == -1)
	{
		printf("Thread %d: Not enough memory\n" ,*id_pointer);
	}
	else //access granted to the memory, since those indexes are reserved for this thread, no need to protect it with mutex
	{
		for (int i = 0; i < randsize; i++)
		{
			char idchar = '0' + *id_pointer; //converting it to char
			memory[thread_message[*id_pointer]+i] = idchar; //thread_message[*id_pointer] is the starting point
		}
	}
}

void init()	 
{
	pthread_mutex_lock(&sharedLock);	//lock
	for(int i = 0; i < NUM_THREADS; i++) //initialize semaphores
	{sem_init(&semlist[i],0,0);}
	for (int i = 0; i < MEMORY_SIZE; i++)	//initialize memory 
  	{char zero = '0'; memory[i] = zero;}
   	pthread_create(&server,NULL,server_function,NULL); //start server 
	pthread_mutex_unlock(&sharedLock); //unlock
}



void dump_memory() 
{
 // You need to print the whole memory array here.
	cout << "Memory:" << endl;
	for (int i = 0; i < MEMORY_SIZE; i++)
	{
		cout << memory[i]; //can use cout and no need for mutexes since all threads are finished
	}
}

int main (int argc, char *argv[])
 {
	srand(time(NULL)); //generate different seeds for better randomization

 	//You need to create a thread ID array here	
	int thread_ID_array[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
	{
		thread_ID_array[i] = i; //initializing thread ID's from 0, incremented by 1
	}
	
 	init();	// call init
	
 	//You need to create threads with using thread ID array, using pthread_create()
	pthread_t threads_array[NUM_THREADS];
	for (int j = 0; j < NUM_THREADS; j++)
	{
		pthread_create(&threads_array[j], NULL, thread_function, (void *) &thread_ID_array[j]); //making the threads run in thread_function
	}
	
 	//You need to join the threads
	for(int k = 0; k < NUM_THREADS; k++)
	{
         pthread_join(threads_array[k],NULL); //waiting for all threads to be finished
	}

 	dump_memory(); // this will print out the memory
 	
 	printf("\nMemory Indexes:\n" );
 	for (int i = 0; i < NUM_THREADS; i++)
 	{
 		printf("[%d]" ,thread_message[i]); // this will print out the memory indexes
 	}
 	printf("\nTerminating...\n");
 }