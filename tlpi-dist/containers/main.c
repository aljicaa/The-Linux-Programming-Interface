#define _GNU_SOURCE
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mount.h>
#include "main.h"

void setupVariables() {
	clearenv();
	setenv("TERM", "xterm-256color", 0);
	setenv("PATH", "/bin/:/sbin/:usr/bin:/usr/sbin", 0);
}

void setupRoot(char *folder) {
	chroot(folder);
	chdir("/");
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

int runBash() {
	char *name_args[] = {
		"/bin/sh",
		(char *) 0
	};
	execvp("/bin/sh", name_args);
	return EXIT_SUCCESS;
}

int createChild(void (*childFunc), int flags) {
	pid_t childPID;
	//printf("%d\n", CLONE_NEWPID);
	childPID = clone(childFunc, allocStackMem(), flags, 0); // ssize_t (signed) so we can store negative values (read -1).
	if (childPID == -1) {
		printf("Cloning failed");
		exit(EXIT_FAILURE);
	}
	printf("Child/Clone returned %jd\n", (intmax_t) childPID);

	int status;
	ssize_t return_child_pid = waitpid(childPID, &status, 0);
	printf("Finished waiting on child process, return childPID is: %zu\n", return_child_pid);
	if (return_child_pid == -1) {
		printf("Return child PID was -1, exiting\n");
		exit(EXIT_FAILURE); // Instead of exit(), we can return EXIT_FAILURE as a status code and handle the code in main().
	}
	else
		printf("Finished waiting for child with status result %d\n", status);
	return EXIT_SUCCESS;
}

int child_process_func(void *args) {
	printf("Child PID: %d\n", getpid());
	
	sethostname("container", 10);
	setupVariables();
	setupRoot("./root");
	
	mount("proc", "/proc", "proc", 0, 0);
	createChild(runBash, SIGCHLD); // run /bin/sh in a new child process, within the container.
	umount("/proc");

	//run("/bin/sh");
	return EXIT_SUCCESS;
}

int main() {
	printf("Parent PID %d\n", getpid());

	createChild(child_process_func, CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD);

	printf("Child has terminated\n");
	
	return EXIT_SUCCESS;
}