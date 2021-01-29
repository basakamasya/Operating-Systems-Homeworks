//Basak Amasya
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main() //method 1 - c file uses memory mapping
{
	int fd = open("loremipsum.txt", O_RDONLY); //mapping and unmapping parts are taken from recitation 6 ppt
	struct stat s;
	size_t size;
	int status = fstat(fd,&s);
	size = s.st_size; //getting the size of the file
	char * ptr = mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0); //mapping the file
	if (ptr == MAP_FAILED) //checking if failed
	{
		printf("Mapping Failed\n");
		return 1;
	}
	close(fd); //closing the fd because file is mapped

	int count = 0;
	size_t i;
	for (i=0; i <= size; i++)
	{
		if (ptr[i] == 'a')
		{
			count += 1; //increment count by 1 if the char is 'a'
			//printf("%d ", count);
		}
	}
	printf("Number of a occurences in the file is %d.\n",count);
	
	status = munmap(ptr,size); //unmapping the file
	if (status != 0)
	{
		printf("Unmapping Failed\n");
		return 1;
	}
	
	return 0;
}