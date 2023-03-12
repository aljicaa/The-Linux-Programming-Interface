/*
Copy.c rewritten without use of lib dependencies offered by LPI book
By Almir Aljic 2023
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	// Open the files.

	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		printf("Failed to open\n");
		exit(EXIT_FAILURE);
	}

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		printf("Failed to open\n");
		exit(EXIT_FAILURE);
	}

	// Copy contents from file1 to file2.
	


	//return EXIT_SUCCESS;
}