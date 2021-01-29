//Basak Amasya
#include <stdio.h>
#include <stdlib.h>

int main() //method 2 - c file uses fopen
{
	char filename[20] = "loremipsum.txt";
	FILE *fp;
	
	fp = fopen(filename, "r"); //opening the file named "loremipsum.txt", pointed by fp
	if (fp == NULL)
	{
		printf("Couldn't open the file.\n"); //failed to open the file
		return;
	}
	int c = getc(fp); //get the first char
	int count = 0;
	while (c != EOF) //while character is not equal to end of file
	{
		if ((char)c == 'a')
		{
			count += 1; //increment count by 1 if the char is 'a'
			//printf("%d ", count);
		}
		c = getc(fp); //get the next char
	}
	printf("Number of a occurences in the file is %d.\n",count);
    		
	fclose(fp); //closing the file
	return 0;
}