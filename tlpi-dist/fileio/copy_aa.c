/*
Copy.c rewritten without use of lib dependencies offered by LPI book
By Almir Aljic 2023
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	ssize_t numWritten;
	char buf[BUF_SIZE];

	// Open the files.

	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		printf("Failed to open input fd\n");
		exit(EXIT_FAILURE);
	}

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		printf("Failed to open output fd\n");
		exit(EXIT_FAILURE);
	}

	// Copy contents from file1 to file2.
	/*
	1. Read bytes from inputFd.
		Save number of bytes read.
	2. Write bytes to outputFd.
		Save number of bytes written.
	3. Compare number of bytes read/written.
		If not the same, or -1, then it's an error.
	4. EXIT_SUCCESS.
	*/
	while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
		numWritten = write(outputFd, buf, numRead);
		if (numRead != numWritten) {
			printf("Mismatch in number of bytes read/written, exiting.");
			exit(EXIT_FAILURE);
		}
		//printf("BUF_SIZE bytes read/written\n");
	}
	if (numRead == -1) {
		printf("Reading returned error");
		exit(EXIT_FAILURE);
	}
	if (close(inputFd) == -1) {
		printf("Error closing inputFd");
		exit(EXIT_FAILURE);
	}
	if (close(outputFd) == -1) {
		printf("Error closing outputFd");
		exit(EXIT_FAILURE);
	}

	//printf("Exit Success!");
	exit(EXIT_SUCCESS);

}