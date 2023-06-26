#ifndef __READX__
#define __READX__

#include "hashTable.h"
#include <stdio.h>

struct Pair {
  char* category;
  char* details;
};


struct Spec {
  char *spec_id;
  int num_of_fields;
  struct Pair* fields;
};



void set_pair(struct Spec *spec,int pos, char *first_part, char *second_part);
void set_spec(struct Spec *spec, FILE *spec_file, char* dir_name, char* file_name);
void print_pair(struct Pair pair);
void print_spec(struct Spec spec);
char *extract_id(char * dir_name, char *file_name);
int read_datasetX(int HashtableNumOfEntries, struct Entry* HashTable, int bucketSize);
void free_spec(struct Spec *spec);

#endif
