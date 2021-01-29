//Basak Amasya
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main() //method 1 - cpp file uses fstream
{
	fstream file;
	string filename = "loremipsum.txt";
	file.open(filename.c_str()); //opening the file named loremipsum.txt into file fstream
	if (file.fail()) //if failed to open the file
	{
		cout << "Couldn't open the file." << endl;
		return 1;
	}
	char ch;
	int count = 0;

	while (!file.eof()) //until the end of file
	{
		ch = file.get(); //get each char
		if ((char)ch == 'a') 
		{
			count ++; //increment count by 1 if the char is 'a'
			//cout << count << endl;
		}
	}
	cout << "Number of a occurences in the file is " << count << "." << endl;
	
	file.close(); //closing the file
	return 0;
}