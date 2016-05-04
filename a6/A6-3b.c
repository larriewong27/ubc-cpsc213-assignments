#include <stdlib.h>
#include <stdio.h>

int x[] = {1,2,3,-1,-2,0,184,340057058};
int y[] = {0,0,0,0,0,0,0,0};

int f(int a) {
	
	int ans = 0;
	
	while(a != 0) {
		if ((a & 0x80000000) != 0)
			ans++;
		a = a << 1;	
	}
	
	return ans;
}

int main() {
	
	int i = 8;
	
	while(i != 0) {
		i--;
		y[i] = f(x[i]);
	}
	
	//printing array x
	for(int j = 0; j < 8; j++)
		printf("%d\n", x[j]);
		
	//printing array y
	for(int j = 0; j < 8; j++)
		printf("%d\n", y[j]);
	
	return 0;
}