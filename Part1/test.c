#include <stdio.h>
#include "acutest.h"
#include <stdlib.h>
#include <string.h>

#include "readX.h"
#include "ids_list.h"
#include "hashTable.h"
#include "readW.h"


struct FirstId* first_ids_list = NULL;
#define NUM_OF_ENTRIES 20
#define BUCKET_SIZE 100




void test_addinlist2(void){          //checking if it adds the new_nodes in the list
    struct FirstId* list = NULL;
    struct ListId* new_node, *new_node2;

    new_node = malloc(sizeof(struct ListId));
    new_node->id = NULL;
    new_node2 = malloc(sizeof(struct ListId));
    new_node2->id = NULL;

    addIdInList2(new_node, &list);
    TEST_ASSERT( list->first_id == new_node );

    addIdInList2(new_node2, &list);             //check if it takes the place in the beginning of the list
    TEST_ASSERT( list->first_id == new_node2 );

    
    free(new_node);
    free(new_node2);
    free(list->next_id);
    free(list);
}



void test_addinlist(void){
    struct ListId *list1, *list2, *list2temp, *list1temp;

    list1=malloc(sizeof(struct ListId));
    list2=malloc(sizeof(struct ListId));

    list1temp = list1;
    list2temp = list2;

    for (int i=0; i <= 3; i++){

        list1->next_list_id = malloc(sizeof(struct ListId));
        list1 = list1->next_list_id;

        list2->next_list_id = malloc(sizeof(struct ListId));
        list2 = list2->next_list_id;

    }

    addIdInList( list2temp, list1temp);
    TEST_ASSERT( list1->next_list_id == list2temp );

    //free
    list1 = list1temp;
    list2 = list2temp;

    for (int i=0; i <= 3; i++){
        list1 = list1temp->next_list_id;
        free(list1temp);
        list1temp = list1;

        list2 = list2temp->next_list_id;
        free(list2temp);
        list2temp = list2;
    }

    free(list1);
    free(list2);
}



void test_insertInHashTable(void){
    struct Entry* HashTable;
    HashTable = malloc( sizeof(struct Entry) * NUM_OF_ENTRIES);

    //FIRST ELEMENT
    struct Spec spec;

    spec.spec_id = malloc( strlen("www.nelly.com//1234") + 1 );
    strcpy(spec.spec_id,"www.nelly.com//1234" );
    int pos = hashFunction(NUM_OF_ENTRIES, spec.spec_id);

    initializeHashTable(HashTable, NUM_OF_ENTRIES, 1);  //bucketsize = 1
    insertInHashTable(&spec, NUM_OF_ENTRIES, HashTable, 1);     //inserting first element


    TEST_ASSERT(  strcmp(HashTable[pos].bucket->bucket_specs[0].spec->spec_id, spec.spec_id) == 0 );


    //SECOND ELEMENT ( create chain of buckets (overflow of a bucket) )
    struct Spec spec2;
    spec2.spec_id = malloc( strlen("www.nelly.com//1333") + 1 );
    strcpy(spec2.spec_id,"www.nelly.com//1333" );

    pos = hashFunction(NUM_OF_ENTRIES, spec2.spec_id);

    insertInHashTable(&spec2, NUM_OF_ENTRIES, HashTable, 1);     //inserting element

    TEST_ASSERT(strcmp(HashTable[pos].bucket->nextBucket->bucket_specs[0].spec->spec_id, spec2.spec_id) == 0 );


    //FREE MEMORY
    struct Bucket* currentBucket;
    struct Bucket* nextTempBucket;
    for(int i = 0;i < NUM_OF_ENTRIES;i++){                //for every entry in the hashtable
        currentBucket = HashTable[i].bucket;
        while(currentBucket != NULL){
        nextTempBucket = currentBucket->nextBucket;                               // remove data from bucket_specs
        
        for(int j = 0;j < currentBucket->isFull; j++){
            free(currentBucket->bucket_specs[j].spec->spec_id);
            free(currentBucket->bucket_specs[j].spec);
        }
        free(currentBucket->bucket_specs);
        free(currentBucket);
        currentBucket = nextTempBucket;
        }
        currentBucket = HashTable[i].bucket;
  }


    
    free(nextTempBucket);
    free(HashTable);
    free(spec.spec_id);
    free(spec2.spec_id);
    
}


void test_searchHashTable(void){
    struct Entry* HashTable;
    HashTable = malloc( sizeof(struct Entry) * NUM_OF_ENTRIES);
    initializeHashTable(HashTable, NUM_OF_ENTRIES, 1);  //bucketsize = 1

    //FIRST ELEMENT
    struct Spec spec;

    spec.spec_id = malloc( strlen("www.maria.com//1234") + 1 );
    strcpy(spec.spec_id,"www.maria.com//1234" );

    int pos = hashFunction(NUM_OF_ENTRIES, spec.spec_id);
    insertInHashTable(&spec, NUM_OF_ENTRIES, HashTable, 1);     //inserting first element

    //SECOND ELEMENT ( create chain of buckets (overflow of a bucket) )
    struct Spec spec2;
    spec2.spec_id = malloc( strlen("www.maria.com//1333") + 1 );
    strcpy(spec2.spec_id,"www.maria.com//1333" );

    int pos2 = hashFunction(NUM_OF_ENTRIES, spec2.spec_id);
    insertInHashTable(&spec2, NUM_OF_ENTRIES, HashTable, 1);     //inserting element
    searchHashTable( HashTable, NUM_OF_ENTRIES, 1, spec.spec_id, spec2.spec_id);

    TEST_ASSERT( HashTable[pos].bucket->bucket_specs[0].matching_ids == HashTable[pos2].bucket->nextBucket->bucket_specs[0].matching_ids ); //checking that they are in the same clique



    //FREE MEMORY
    struct Bucket* currentBucket;
    struct Bucket* nextTempBucket;
    for(int i = 0;i < NUM_OF_ENTRIES;i++){                //for every entry in the hashtable
        currentBucket = HashTable[i].bucket;
        while(currentBucket != NULL){
        nextTempBucket = currentBucket->nextBucket;                               // remove data from bucket_specs
        
        for(int j = 0;j < currentBucket->isFull; j++){
            free(currentBucket->bucket_specs[j].spec->spec_id);
            free(currentBucket->bucket_specs[j].spec);
        }
        free(currentBucket->bucket_specs);
        free(currentBucket);
        currentBucket = nextTempBucket;
        }
        currentBucket = HashTable[i].bucket;
    }


    free(HashTable);
    free(spec.spec_id);
    free(spec2.spec_id);
}




void test_extract_id(void){

    char dir[20] = "www.anna.com";
    char file[20] = "1234.json";

    char *spec_id = extract_id( dir, file);

    TEST_ASSERT( strcmp(spec_id, "www.anna.com//1234") == 0 );

    free(spec_id);

}




void test_deleteIdFromList2(void){
    struct FirstId* firstslist = NULL;
    struct ListId spec_node1;
    struct ListId spec_node2;
    struct ListId spec_node3;
    struct ListId spec_node4;

    spec_node1.id = malloc(sizeof(char) * (strlen("www.anna.com//1111")+1 ) );
    spec_node2.id = malloc(sizeof(char) * (strlen("www.maria.com//2222")+1 ) );
    spec_node3.id = malloc(sizeof(char) * (strlen("www.nelly.com//3333")+1 ) );
    spec_node4.id = malloc(sizeof(char) * (strlen("www.project.com//4444")+1 ) );

    strcpy(spec_node1.id, "www.anna.com//1111");
    strcpy(spec_node2.id, "www.maria.com//2222");
    strcpy(spec_node3.id, "www.nelly.com//3333");
    strcpy(spec_node4.id, "www.project.com//4444");

    addIdInList2( &spec_node1, &firstslist);
    addIdInList2( &spec_node2, &firstslist);
    addIdInList2( &spec_node3, &firstslist);
    addIdInList2( &spec_node4, &firstslist); //firstslist: spec_node4 -> spec_node3-> spec_node2 -> spec_node1

    deleteIdFromList2(&spec_node2, &firstslist);    //delete 2nd node
    TEST_ASSERT( firstslist->next_id->next_id->first_id ==  &spec_node1); //if now the second node is the spec_node3
                                                                             //firstslist: spec_node4->spec_node3->spec_node1

    deleteIdFromList2(&spec_node4, &firstslist);    //delete 1st node
    TEST_ASSERT( firstslist->first_id ==  &spec_node3);        //if now the first node is spec_node3
                                                                //firstslist: spec_node3 -> spec_node1

    deleteIdFromList2(&spec_node1, &firstslist);    //delete last node
    TEST_ASSERT( firstslist->next_id == NULL);        //if now there is only one node in the list
                                                                ////firstslist: spec_node3
    free(spec_node1.id);
    free(spec_node2.id);
    free(spec_node3.id);
    free(spec_node4.id);

    free(firstslist);
}



void test_hashFunction(void){

    char spec_id[20] = "www.nelly.com//1234";
    
    int hash =  hashFunction(NUM_OF_ENTRIES, spec_id);

    TEST_ASSERT( hash == ((1+2+3+4)%NUM_OF_ENTRIES) );

}


void test_greaterSpecId(void){


    //checking with different directories, same filename 
    char spec_id[20] = "www.nelly.com//1234";
    char spec_id2[20] = "www.maria.com//1234";

    TEST_ASSERT( greaterSpecId ( spec_id, spec_id2) == LEFT);


    //checking with same directories, different filename 
    char spec_id3[20] = "www.nelly.com//234";
    char spec_id4[20] = "www.nelly.com//1235";

    TEST_ASSERT( greaterSpecId ( spec_id3, spec_id4) == RIGHT);



}




void test_set_spec(void){

    struct Spec spec;

    //manually inserted the data expected to be retrieved from file
    spec.spec_id = malloc(sizeof(char) * (strlen("www.anna.com//1111")+1 ) );
    strcpy( spec.spec_id, "www.anna.com//1111");
    spec.num_of_fields = 2;
    char category1[20] = "<category1>";
    char category2[20] = "category2";
    char details1[20] = "details1";
    char details2[20] = "details2";

    spec.fields = malloc(sizeof(struct Pair)* 2 );
    spec.fields[0].category = malloc(sizeof(char) * (strlen(category1)+1 ) );
    strcpy(spec.fields[0].category, category1);

    spec.fields[1].category = malloc(sizeof(char) * (strlen(category2)+1 ) );
    strcpy(spec.fields[1].category, category2);

    spec.fields[0].details = malloc(sizeof(char) * (strlen(details1)+1 ) );
    strcpy(spec.fields[0].details, details1);
    
    spec.fields[1].details = malloc(sizeof(char) * (strlen(details2)+1 ) );
    strcpy(spec.fields[1].details, details2);






    FILE *file;
    file = fopen("./1111.json", "r");
    char dir_name[20] = "www.anna.com";
    char file_name[20] = "1111.json";


    struct Spec spec2;

   
    set_spec( &spec2, file, dir_name, file_name);


    TEST_ASSERT( strcmp(spec.spec_id, spec2.spec_id) == 0 );
    TEST_ASSERT( spec.num_of_fields == spec2.num_of_fields );
    TEST_ASSERT( strcmp( spec.fields[0].category, spec2.fields[0].category ) == 0 );
    TEST_ASSERT( strcmp( spec.fields[1].category, spec2.fields[1].category ) == 0 );
    TEST_ASSERT( strcmp( spec.fields[0].details, spec2.fields[0].details ) == 0 );
    TEST_ASSERT( strcmp( spec.fields[1].details, spec2.fields[1].details ) == 0 );


    
    free(spec.spec_id);
    
    free(spec.fields[0].category);
    free(spec.fields[1].category);
    free(spec.fields[0].details);
    free(spec.fields[1].details);
    free(spec.fields);

    free(spec2.fields[0].category);
    free(spec2.fields[1].category);
    free(spec2.fields[0].details);
    free(spec2.fields[1].details);
    free(spec2.fields);

    free(spec2.spec_id);

    fclose(file);


}



TEST_LIST = {

    { "addinlist2", test_addinlist2 },
    { "addinlist", test_addinlist },
    { "insertInHashTable", test_insertInHashTable },
    { "searchHashTable", test_searchHashTable },
    { "extract_id", test_extract_id },
    { "deleteIdFromList2", test_deleteIdFromList2 },
    { "hashFunction", test_hashFunction },
    { "greaterSpecId", test_greaterSpecId },
    { "set_spec", test_set_spec },
    {NULL,NULL}

};
