#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Element {
  char   name[200];
  struct Element *next;
};

struct Element *top = 0;

void push (char* aName) {
  struct Element* e = malloc (sizeof (*e));    // Not the bug: sizeof (*e) == sizeof(struct Element)
  strncpy (e->name, aName, sizeof (e->name));  // Not the bug: sizeof (e->name) == 200
  e->next  = top;
  top = e;
}

void pop(char* aName) { 
  struct Element* e = top;
  strncpy (aName, e->name, sizeof (e->name));
  top = e->next;
  free (e);
  e = NULL;
}

int main (int argc, char** argv) {
  char w[200];
  char x[200];
  char y[200];
  char z[200];
  push ("A");
  push ("B");
  pop(w);
  push ("C");
  push ("D");
  pop(x);
  pop(y);
  pop(z);
  printf ("%s %s %s %s\n", w, x, y, z);
}
