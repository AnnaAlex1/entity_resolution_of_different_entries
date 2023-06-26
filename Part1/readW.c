#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "readW.h"

int readDatasetW (FILE* datasetW, char* left, char* right)
{
    int label = 0;
    char c = 0;
    char buffer[BSIZE];

    if (!datasetW)
    {
        perror("Opening dataset W failed");
        return ENOENT;
    }
    
    
    while ((c=fgetc(datasetW))!=EOF)
    {
        ungetc(c,datasetW);

        fgets(buffer,BSIZE,datasetW);

        strcpy(left,strtok(buffer,","));
        strcpy(right,strtok(NULL,","));
        label = atoi(strtok(NULL,","));

        if (label == 1) break;  // return this pair of specs
                                // else ignore unmatching pairs labeled by 0
    }

    return c;
}

void init_specid_parts(char** s)
{
  for (int i=0; i<NUM_OF_SPLITS; ++i) s[i]=NULL;

  return;

}

void free_specid_parts(char** s)
{
  for (int i=0; i<NUM_OF_SPLITS; ++i) free(s[i]);

  return;

}

void split_specid(char** specid_parts, char* specid)
{
  //create a local copy of specid
  char cp[strlen(specid) + 1];
  strcpy(cp,specid);

  //split copy of specid (cp) into: <link> - <code>                         
  init_specid_parts(specid_parts);                          //set all parts to NULL
  int i=0;
  char* split = strtok(cp,"//");
  
  while(split != NULL)
  {  
    specid_parts[i] = malloc(strlen(split) + 1);  
    strcpy(specid_parts[i++],split);     
    split = strtok(NULL,"//");      
  }
}

int greaterSpecId (char* left_spec_id, char* right_spec_id)
{
    //split spec_ids into: <link> - <code>
    char* lefts[NUM_OF_SPLITS]; 
    char* rights[NUM_OF_SPLITS];                                
    split_specid(lefts,left_spec_id);                 
    split_specid(rights,right_spec_id);   

    /* Comparison of spec_ids:
        - firstly check if <link> parts are similar
    */
    int cmp = strcmp(lefts[0],rights[0]);
    if (!cmp) {  //left_link == right_link -> compare values of codes
       
      int codel = atoi(lefts[1]);
      int coder = atoi(rights[1]);
      free_specid_parts(lefts);
      free_specid_parts(rights);
      return (codel > coder) ? LEFT : RIGHT;

    } else {  //compare links (alphabetically)

      free_specid_parts(lefts);
      free_specid_parts(rights);
      return (cmp>0) ? LEFT : RIGHT;
    }

}


int readOutputCsv (FILE* out, char* left, char* right)
{
    char c = 0;
    char buffer[BSIZE];

    if (!out)
    {
        perror("Opening output csv failed");
        return ENOENT;
    }
    
    
    while ((c=fgetc(out))!=EOF)
    {
        ungetc(c,out);

        fgets(buffer,BSIZE,out);

        strcpy(left,strtok(buffer,","));
        strcpy(right,strtok(NULL,","));

    }

    return c;
}