#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char *s = "Hello";
	printf("%p\n", s);
	printf("%s\n", s);

	char *x = "Hello";
	printf("%s\n", x);
	printf("x length: %d\n", strlen(x));

	int l = 500;
	char pid_as_str[100];
	//itoa(l, pid_as_str, 10);
	//printf("%s", pid_as_str);
	sprintf(pid_as_str, "%d", l);
	printf("%s\n", pid_as_str);
}