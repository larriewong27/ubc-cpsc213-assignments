#include <stdlib.h>
#include <stdio.h>

struct S* s;

struct S {
	struct D* d1;
	struct D* d2;
};

struct D {
	int x;
	int y;
};

int main() {
	s = malloc(sizeof(struct S));
	s->d1 = malloc(sizeof(struct D));
	s->d2 = malloc(sizeof(struct D));
	s->d1->x = 1;
	s->d1->y = 2;
	s->d2->x = 3;
	s->d2->y = 4;
	s->d2->x = s->d1->y;
	s->d1->x = s->d2->y;
	printf("%d\n%d\n%d\n%d\n", s->d1->x, s->d1->y, s->d2->x, s->d2->y);
	return 0;
}
	
	

