#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ids_list.h"

void addIdInList(struct ListId* origin_matching_list,struct ListId* dest_matching_list){
  
  struct ListId* dest_pointer;    
  dest_pointer = dest_matching_list;

  //search last node - end of list
  while(dest_pointer->next_list_id != NULL){    
    dest_pointer = dest_pointer->next_list_id;
  }

  //add new node at the end of the list
  dest_pointer->next_list_id = origin_matching_list;

  return;
}

void addIdInList2(struct ListId* new_list_id,struct FirstId** first_ids_list){
  struct FirstId* firstId;

  firstId = malloc(sizeof(struct FirstId));

  if(*first_ids_list == NULL){    //list is empty - add first node
    firstId->first_id = new_list_id;
    firstId->next_id = NULL;
    *first_ids_list = firstId;
  }
  else{   //add node at the beginning of the list
    firstId->first_id = new_list_id;
    firstId->next_id = *first_ids_list;
    *first_ids_list = firstId;
  }
}


void deleteIdFromList2(struct ListId* removing_id,struct FirstId** firstId){


    struct FirstId* temp = *firstId;  //temporary pointer to the first node
    struct FirstId* prev = NULL;      //temporary pointer to the previous of current node

    if (temp != NULL && (strcmp( temp->first_id->id,removing_id->id) == 0) )  //remove first node of list
    {
        *firstId = temp->next_id;   //set 2nd node as first
        free(temp);   //free temp --> first node
        return;
    }


    while (temp != NULL && (strcmp( temp->first_id->id,removing_id->id) != 0))  //loop until finding node to delete
    {
        prev = temp;
        temp = temp->next_id;
    }

   
    if (temp == NULL)   //list is empty - cannot delete any node
        return;

    prev->next_id = temp->next_id;  //skip node that you will delete
    free(temp); //free that node

    return;

}


void release_listid(struct FirstId* first_ids_list){ //release memory of cliques

  struct ListId* temp, *to_free;

  while( first_ids_list != NULL){

    temp = first_ids_list->first_id;
    while( temp != NULL){
      free(temp->id);
      to_free = temp;
      temp = temp->next_list_id;
      free(to_free);

    }

    first_ids_list = first_ids_list->next_id;
  } 


}



void release_flistid(struct FirstId** first_ids_list){

  struct FirstId* temp, *to_free;
  temp = *first_ids_list;

  while( temp != NULL){

    to_free = temp;
    temp = temp->next_id;
    free(to_free);

  } 
}