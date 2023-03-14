#define _GNU_SOURCE
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>

int child_process_func(void *args) {
	printf("Hello from child\n");
	return EXIT_SUCCESS;
}

char* allocStackMem() {
	/*
	Initialize variables used to allocate stack memory for the child process.
	stackTop points to the end of the stack, because it grows upwards and a pointer to the end must be sent to the clone() syscall.
	*/
	const int STACKSIZE = 65536;
	char *stack;
	char *stackTop;

	/*
	Allocate memory for the child process.
	*/
	stack = malloc(STACKSIZE);
	if (stack == NULL) {
		printf("malloc failed\n");
		exit(EXIT_FAILURE);
	}

	stackTop = stack + STACKSIZE; // Pointer to the end of the stack.
	return stackTop;
}

int main() {
	printf("Hello from parent\n");

	char *stackTop = allocStackMem();

	ssize_t result = clone(child_process_func, stackTop, SIGCHLD, 0); // ssize_t (signed) so we can store negative values (read -1).
	if (result == -1)
		return EXIT_FAILURE;
	
	ssize_t wait_result = waitpid(-1, NULL, 0);
	if (wait_result == -1)
		return EXIT_FAILURE;
	printf("Child has terminated\n");
	
	return EXIT_SUCCESS;
}