#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "readX.h"

//type of bucket's content
struct BucketSpec{
  struct Spec* spec;  //pointer to a spec
  struct ListId* matching_ids;  //list of matching specs = clique of spec
};

//bucket
struct Bucket{
  struct BucketSpec* bucket_specs;  //content of bucket: bucketsize*bucket_specs
  struct Bucket* nextBucket;  //pointer to next bucket (in case of overflow)
  int isFull;   //current number of BucketSpecs inside bucket => place in bucket to add a new spec
};

// hash table entry
struct Entry{
  int key;
  struct Bucket* bucket;  // pointer to bucket
};

void initializeHashTable(struct Entry*, int, int);  //initialises hashtable
int hashFunction(int, char*); //function to hash spec_ids for placing spec to bucket
void insertInHashTable(struct Spec*, int, struct Entry*, int);  //insert a spec in hashtable
void searchHashTable(struct Entry*, int, int, char*, char*);   // search position of 2 spec_ids inside hash table in order to merge their matching lists (cliques)
void fixing_pointers(struct ListId* , struct ListId* );  // when merging 2 lists: make all pointers of origin list point to destination list
void freeHashTable(struct Entry* hashTable,int numOfEntries,int bucketSize);

#endif
