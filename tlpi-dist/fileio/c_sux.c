#include <stdio.h>
#include <stdlib.h>

int main() {
    int *a = malloc(sizeof(int));
    *a = 42;
	//printf("a=%d\n", *a);
    free(a);
    printf("a=%d\n", *a);
}