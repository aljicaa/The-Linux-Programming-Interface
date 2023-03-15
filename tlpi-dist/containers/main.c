#define _GNU_SOURCE
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "main.h"

int child_process_func(void *args) {
	printf("Hello from child\n");
	clearenv(); // Clear the environment, so that we may re-build it.
	run("/bin/sh");
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

int run(char *cmd) {
	char *name_args[] = {
		"/bin/sh",
		(char *) 0
	};
	execvp(cmd, name_args);
	return EXIT_SUCCESS;
}

int main() {
	printf("Hello from parent\n");

	char *stackTop = allocStackMem();

	int status;
	pid_t childPID;
	childPID = clone(child_process_func, stackTop, SIGCHLD, 0); // ssize_t (signed) so we can store negative values (read -1).
	if (childPID == -1)
		return EXIT_FAILURE;
	printf("Clone returned %jd\n", (intmax_t) childPID);
	
	ssize_t wait_result = waitpid(childPID, &status, 0);
	printf("Wait result printed %zu\n", wait_result);
	if (wait_result == -1)
		return EXIT_FAILURE;
	else
		printf("Finished waiting for child with status result %d\n", status);
	printf("Child has terminated\n");
	
	return EXIT_SUCCESS;
}