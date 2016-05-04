#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "list.h"
#include <string.h>

void print_string (element_t ev) {
  char* e = ev;
  printf ("%s\n", e);
}

void print_num (element_t ev) {
  intptr_t e = (intptr_t) ev;
  printf ("%ld\n", e);
}

void str_toNum(element_t* outputv, element_t inputv) {
	char* input = (char*) inputv;
	intptr_t* output = (intptr_t*) outputv;
	char** input_ptr = &input;
	*output = strtol(input, input_ptr, 10);
	if (*input_ptr != NULL && *output == 0)
		*output = -1;
}

void toNULL(element_t* rv, element_t av, element_t bv) {
	char* a = av;
	intptr_t b = (intptr_t) bv;
	char** r = (char**) rv;
	if (b < 0)
		*r = a;
	else
		*r = 0;
}

int isPositive(element_t av) {
	intptr_t a = (intptr_t) av;
	if (a != -1)
		return 1;
	else 
		return 0;
}

int notNULL (element_t av) {
	char* a = av;
	if (a != NULL)
		return 1;
	else
		return 0;
}

void truncString(element_t* rv, element_t av, element_t bv) {
	char* a = av;
	intptr_t b = (intptr_t) bv;
	char** r = (char**) rv;
	*r = strdup(a);
	if (strlen(*r) > b) {
		for (int i = 0; i < strlen(*r); i++) {
			if (i == b) {
				(*r)[i] = '\0';
				return;
			}
		}
	}
}

int maxValue(struct list *l) {
	intptr_t max = (intptr_t)list_get(l, 0);
	for (int i = 0; i < list_len(l); i++) {
		intptr_t a = (intptr_t)list_get(l, i);
		if (a > max) {
			max = a;
		}
	}
	return max;
}

int main(int argc, char** argv) {
	
	struct list* l0 = list_create();
	for (int i = 1; i < argc; i++)
		list_append(l0, argv[i]);
	
	struct list* l1 = list_create();
	list_map1(str_toNum, l1, l0);
	
	struct list* l2 = list_create();
	list_map2(toNULL, l2, l0, l1);
	
	struct list* l3 = list_create();
	list_filter(isPositive, l3, l1);

	struct list* l4 = list_create();
	list_filter(notNULL, l4, l2);
	
	struct list* l5 = list_create();
	list_map2(truncString, l5, l4, l3);
	list_foreach(print_string, l5);
	list_foreach(free, l5);
	
	int max = maxValue(l1);
	printf("%ld\n", max);
	
	list_destroy(l0);
	list_destroy(l1);
	list_destroy(l2);
	list_destroy(l3);
	list_destroy(l4);
	list_destroy(l5);
}