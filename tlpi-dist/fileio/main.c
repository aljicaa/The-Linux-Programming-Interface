#define _GNU_SOURCE
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>

//int child_process_func();
int child_process_func();

int child_process_func(void *args) {
	printf("Hello from child\n");
	return EXIT_SUCCESS;
}

int main() {
	printf("Hello from parent\n");

	const int STACKSIZE = 65536;
	char *stack;
	char *stackTop;

	stack = malloc(STACKSIZE);
	if (stack == NULL)
		return EXIT_FAILURE;
	stackTop = stack + STACKSIZE;

	ssize_t result = clone(child_process_func, stackTop, SIGCHLD, 0);
	if (result == -1)
		return EXIT_FAILURE;
	
	wait(NULL);
	
	return EXIT_SUCCESS;
}