#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
 * Class Person
 */

struct Person_class {
  void*   super;
  void (* toString ) (void*, char*, int);
};

struct Person {
  struct Person_class* class;
  char* name;
};

void Person_toString (void* thisVP, char* buf, int bufSize) {
	struct Person* this = thisVP; 
	snprintf (buf, bufSize, "Name: %s", this->name); 
}

struct Person_class Person_class_obj = {NULL, Person_toString};

struct Person* new_Person(char* name) {
  struct Person* obj = malloc (sizeof (struct Person));
  obj->class = &Person_class_obj;
  obj->name = strdup(name);
  return obj;
}


/*
 * Class Student extends Person
 */

struct Student_class {
  struct Person_class* super;
  void         (* toString ) (void*, char*, int);
};

struct Student {
  struct Student_class* class;
  char* name;
  int sid;
};

void Student_toString (void* thisVP, char* buf, int bufSize) {
	struct Student* this = thisVP; 
	char Person_str[255];
	this->class->super->toString (this, Person_str, bufSize);
	snprintf (buf, bufSize, "%s, SID: %i", Person_str, this->sid); 
}

struct Student_class Student_class_obj = {&Person_class_obj, Student_toString};

struct Student* new_Student(char* name, int sid) {
  struct Student* obj = malloc (sizeof (struct Student));
  obj->class = &Student_class_obj;
  obj->name = strdup(name);
  obj->sid = sid;
  return obj;
}


/*
 * Main
 */

void print (void* personVP) {
  struct Person* person = (struct Person*) personVP;
  char buf[255];
  person->class->toString (person, buf, sizeof buf);
  printf("%s\n", buf);
}


int main (int argc, char** argv) {
    void* people[2] = {new_Person("Alex"), new_Student("Alice", 300)};
	for (int i = 0; i < 2; i++)
		print(people[i]);
}