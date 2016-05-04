#include <stdlib.h>
#include <stdio.h>

int arr[] = {0,0,0,0,0,0,0,0,0,0};
int* arrPtr = arr;

void foo(int a, int b) {
	arrPtr[b] += a;
}

int main() {
	int x = 1;
	int y = 2;
	foo(3, 4);
	foo(x, y);
	
	// printing the array
	for(int i = 0; i < 10; i++) 
		printf("%d\n", arrPtr[i]);
	
	return 0;
}