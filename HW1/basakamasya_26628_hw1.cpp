//Basak Amasya
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib> //for random number generation
#include <pthread.h> //POSIX thread library
using namespace std;

vector < vector < int > > M(2, vector<int>(50)); //global matrix
int remaining = 100; //to keep the remaining empty seats
int turn = 0; //global variable for busy waiting

void * TravelAgency( void * id)
{
	srand(time(NULL)); //tip from the homework document, generate different seeds at each execution
	int * idpointer = (int *) id;
	int finished = 0;
	while (remaining > 0) //until there is no empty seat left in the plane
	{
		int seatno = rand() % 100; //generating random seat number, outside critical region
		int y = seatno/50; //found the x-y coordinates of that seat number
		int x = seatno%50 ;
		if (* idpointer == 1) //for the first thread
		{
			while (turn != 0) {} //busy wait
			cout << "Agency 1 Entered Critical Region" << endl;
			if (remaining == 0) finished = 1;
			if (M[y][x] == 0) //check if that seat is empty
			{
				M[y][x] = 1; //reserve the seat if empty
				remaining --;
				cout << "Seat Number " << (seatno+1) << " is reserved by Agency 1" << endl;
			}
			cout << "Agency 1 Exit Critical Region" << endl;
			if (finished) cout << "No Seats Left" << endl;
			cout << endl;
			turn = 1; //give the turn to the second thread
		}
		else if (* idpointer == 2) //for the second thread, same task
		{
			if (remaining == 0) finished = 1;
			while (turn != 1) {} //busy wait
			cout << "Agency 2 Entered Critical Region" << endl;
			if (remaining == 0) finished = 1;
			if (M[y][x] == 0)
			{
				M[y][x] = 2; 
				remaining --;
				cout << "Seat Number " << (seatno+1) << " is reserved by Agency 2" << endl;
			}
			cout << "Agency 2 Exit Critical Region" << endl;
			if (finished) cout << "No Seats Left" << endl;
			cout << endl;
			turn = 0;
		}
		
	}
}
int main()
{
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 50; y++)
		{
			M[x][y] = 0; //marking all the seats as 0 initially, plane is empty
		}
	}
	pthread_t agency1, agency2;
	int id1 = 1;
	int id2 = 2;
	pthread_create(&agency1, NULL, TravelAgency, (void*) &id1); //making both of the threads executable, sending to function with corresponding ids
	pthread_create(&agency2, NULL, TravelAgency, (void*) &id2);

	pthread_join(agency1, NULL); //waiting for the threads to finish their jobs
	pthread_join(agency2, NULL); //the child threads will terminate when plane becomes full

	cout << "Plane is full:" << endl;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 50; y++)
		{
			cout << M[x][y] << " "; //printing the final seat plan of the plane
		}
		cout << endl;
	}
	return 0;
}