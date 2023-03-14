#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_READ 20

int main() {
	char buffer[MAX_READ];
	ssize_t numRead;

	numRead = read(STDIN_FILENO, buffer, MAX_READ);
	if (numRead == -1)
		exit(EXIT_FAILURE);
	
	buffer[numRead] = '\0';
	printf("%s\n", buffer);

	/*
	if ((read(STDIN_FILENO, buffer, MAX_READ) == -1)) {
		exit(EXIT_FAILURE);
	}
	printf("%s\n", buffer);*
	*/
}