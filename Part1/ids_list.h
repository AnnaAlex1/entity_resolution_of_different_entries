#ifndef __IDSLIST__
#define __IDSLIST__

//node of a list representing a clique of matching specs <--- LIST
struct ListId{
  char* id; //spec_id
  struct ListId* next_list_id;  //pointer to next node
  struct BucketSpec* mat_spec;  //pointer to BucketSpec (spec + its matching clique)
};

//node of a list containing pointers to the start of every clique <--- LIST 2
struct FirstId{
  struct ListId* first_id;  //pointer to first element of clique
  struct FirstId* next_id;  //pointer to next node
};

void addIdInList(struct ListId*,struct ListId*);          //adds node in the clique matching spec_ids (LIST)
void addIdInList2(struct ListId*,struct FirstId**);       //adds node in the list of pointers to cliques' heads (LIST 2)
void deleteIdFromList2(struct ListId* ,struct FirstId**); //removes a pointer to clique's head from the list (LIST 2)
void release_listid(struct FirstId* first_ids_list);      //release memory of cliques (LIST)
void release_flistid(struct FirstId** first_ids_list);    //release memory of list of pointers to cliques (LIST2)


#endif
