#include "readX.h"
#include "hashTable.h"
#include "result.h"
#include "readW.h"

#include <stdlib.h>
#include <errno.h>

#define NUM_OF_ENTRIES 20
#define BUCKET_SIZE 100

struct FirstId* first_ids_list = NULL;         //a list holding the start of each clique

int main(int argc, char** argv){

  struct Entry* hashTable;
  hashTable = malloc( sizeof(struct Entry) * NUM_OF_ENTRIES);

  initializeHashTable(hashTable, NUM_OF_ENTRIES, BUCKET_SIZE);


//algorithm:

  //Reading dataset X
  if ( read_datasetX(NUM_OF_ENTRIES, hashTable, BUCKET_SIZE) == 1 ) { return 1; }


  /*checkHashTable(hashTable, NUM_OF_ENTRIES, BUCKET_SIZE);*/

  printf("Specs added to the hashtable successfully!\n");


  //Reading dataset W

  FILE* datasetw = fopen("./sigmod_large_labelled_dataset.csv","r");
  char left_spec_id[50];
  char right_spec_id[50];
  int wstatus;

  while ((wstatus=readDatasetW(datasetw,left_spec_id,right_spec_id))!=EOF)
  {

      if (wstatus==ENOENT) return wstatus;

      //in merge of matching lists move spec with greater id value
      if (greaterSpecId (left_spec_id, right_spec_id) == RIGHT){
        searchHashTable(hashTable, NUM_OF_ENTRIES, BUCKET_SIZE, right_spec_id, left_spec_id);
      }
      else{
        searchHashTable(hashTable, NUM_OF_ENTRIES, BUCKET_SIZE, left_spec_id, right_spec_id);

      }

  }

  fclose(datasetw);
  printf("Matching specs is successfully completed!\n");


  //Results - create output file
  if ( print_results( first_ids_list ) == 1) {return 1;}

  release_listid(first_ids_list);             //release memory of cliques
  release_flistid(&first_ids_list);           //release memory of list with the beginnings of cliques

  freeHashTable(hashTable, NUM_OF_ENTRIES,BUCKET_SIZE);
  free(hashTable);

  return 0;

}
