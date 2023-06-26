#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>

#include "readX.h"
#include "hashTable.h"


#define MAXBUF 51000


int read_datasetX(int HashtableNumOfEntries, struct Entry* HashTable, int bucketSize){

  FILE *spec_file;
  

  DIR *maindr, *subdr;
  struct dirent *dsub_dir, *dfiles;


  struct Spec *spec;
  spec = malloc( sizeof(struct Spec));

  //open directory "2013_camera_specs"
  if ( (maindr = opendir("./camera_specs/2013_camera_specs")) == NULL ) { perror("Directory cannot open!\n"); return 1; }


  //for every subdirectory
  while ( (dsub_dir = readdir(maindr)) != NULL ){

                                    /*printf("%s\n", dsub_dir->d_name);*/

    if ( (strcmp(dsub_dir->d_name,".") != 0) && (strcmp(dsub_dir->d_name,"..") != 0) ){


                                    printf("Opening Directory: %s\n", dsub_dir->d_name);

        char *path;
        path = malloc(sizeof(char) * ( strlen(dsub_dir->d_name) + strlen("./camera_specs/2013_camera_specs/") + 1 ) );
        strcpy(path, "./camera_specs/2013_camera_specs/");
        strcat(path, dsub_dir->d_name);

                                    /*printf("diretory to open: %s\n", path);*/

        //open sub-directory
        if ( (subdr = opendir(path)) == NULL ) { perror("Sub-Directory cannot open!"); return 1; }


            //for every file in the sub-directory
            while ( (dfiles = readdir(subdr)) != NULL ){

                if ( (strcmp(dfiles->d_name,".") != 0) && (strcmp(dfiles->d_name,"..") != 0) ){

                                    /*printf("File: %s\n", dfiles->d_name);*/

                    char *filepath;
                    filepath = malloc(sizeof(char) * ( strlen(dfiles->d_name) + strlen(path) + 2 ) );         //+1 for the character '/'
                    strcpy(filepath, path);
                    strcat(filepath, "/");
                    strcat(filepath, dfiles->d_name);

                                    /*printf("file to open: %s\n", filepath);*/

                    //open file for spec
                    spec_file = fopen(filepath, "r");
                    if (spec_file == NULL){
                      perror("File cannot open!\n");
                      return 1;
                    }

                                                                            
                    set_spec(spec, spec_file, dsub_dir->d_name, dfiles->d_name); //passing file pointer, name of directory, file name
                    //print_spec(*spec);

                    //give spec to create nodes
                    insertInHashTable( spec, HashtableNumOfEntries, HashTable, bucketSize);

                    //close file
                    fclose(spec_file);

                    free(filepath);

                }


            }



        closedir(subdr);

        free(path);

    }



  }

  
  free(spec);
  closedir(maindr);

  return 0;
}









void set_spec(struct Spec *spec, FILE *spec_file, char* dir_name, char* file_name){            //returns a struct of spec from the file given

  
  
  char *line = malloc(sizeof(char) * 700);

  char first_part[200];
  char second_part[MAXBUF];


  //count number of fields for the Spec
  spec->num_of_fields = 0;

  int charsinline;                                                                                                  
  while ( fgets(line, 500, spec_file) != NULL){
    charsinline = strlen(line);
    for (int i=0; i< charsinline; i++ ){
      //printf("%c    ", line[i]);
      if ( (line[i-1] == '\"' ) && (line[i] == ':') ){
          spec->num_of_fields++; 
          break;
      }
    }
    
    
  }


  spec->spec_id = extract_id( dir_name, file_name );


  //create a table for the spec fields
  spec->fields = malloc(sizeof(struct Pair)* (spec->num_of_fields));


  //fill table
  fseek(spec_file, 0, SEEK_SET);                            //setting pointer back to the start
  
  int i;
  int fields_counter = 0;
  char prev_ch, ch;
  char help = '\\';

  
  while ( (ch = fgetc(spec_file)) != EOF && fields_counter < spec->num_of_fields){                    
      prev_ch = ch;

      while( ( (ch = fgetc(spec_file)) != EOF) && ( (ch != '"') || ( (ch == '"') && (prev_ch == help) ) ) ){ prev_ch = ch; }

      i = 0;

      
      ch = prev_ch;                       //change value '"' in order to enter the loop 
      while( ( (ch = fgetc(spec_file)) != EOF) && ( (ch != '"') || ( (ch == '"') && (prev_ch == help) ) )  ){  //reading first part (category name)
          
          prev_ch = ch;
          first_part[i] = ch;
          i++;
         
        
      }
                                                                                                           
      first_part[i] = '\0';
      
      
      if ( (ch = fgetc(spec_file)) != ':') {
        
        printf("Error: no ':' after!\n" );
        
        return;
      }

      while( ( (ch = fgetc(spec_file)) != EOF) && ( (ch != '"') || ( (ch == '"') && (prev_ch == help) ) ) && (ch != '[')    ){ prev_ch = ch;}


      i = 0;

      if ( ch == '['){                          //reading second part (details)

          
          while(  ( (ch = fgetc(spec_file)) != EOF) &&  (( ch != ',')  ||  prev_ch != ']')){ 
            
            prev_ch = ch;
            second_part[i] = ch;
            i++;
            
            }

      } else if (ch == '"'){

          ch = prev_ch;                       //change value '"' in order to enter the loop 
          while(  ( (ch = fgetc(spec_file)) != EOF) && ( (ch != '"') || ( (ch == '"') && (prev_ch == help) ) )  ){ 
            
            prev_ch = ch;
            second_part[i] = ch;
            i++;
            
            }

      }

      second_part[i] = '\0';


      

      set_pair(spec, fields_counter, first_part, second_part);      //put category and details to the spec
      fields_counter++;  

      
    
  }
  
  free(line);


}


char * extract_id(char * dir_name, char *file_name){

  char *spec_id = malloc( sizeof(char) * ( strlen(dir_name) + strlen(file_name) - 2 ) );      // +2 (for '//')  -5 (for '.json') + 1 (for '\0') =  -2

  int size_of_temp = sizeof(char) * (strlen(file_name) - 4) ;
  char temp[size_of_temp];
  temp[size_of_temp-1] = '\0';


  memcpy(temp, file_name, size_of_temp-1); //copy only the name (without .json or \0)

  strcpy(spec_id, dir_name);  //first the directory d_name
  strcat(spec_id, "//");      //then the characters '//'
  strcat(spec_id, temp);      //lastly, the filename without '.json'


  return spec_id;
}





void set_pair(struct Spec *spec,int pos, char *first_part, char *second_part){       //set pair in the position 'pos'
  spec->fields[pos].category = malloc( (strlen(first_part) + 1)*sizeof(char)); //of the spec's table of fields
  spec->fields[pos].details = malloc( (strlen(second_part) + 1)*sizeof(char));

  strcpy(spec->fields[pos].category, first_part);
  strcpy(spec->fields[pos].details, second_part);
}




void print_pair(struct Pair pair){
  printf("  %s -> %s\n", pair.category, pair.details);
}




void print_spec(struct Spec spec){

  printf("\nSpec_id: %s\n", spec.spec_id);
  printf("Num of fields: %d\n", spec.num_of_fields);

  for (int i=0; i<spec.num_of_fields; i++){
    print_pair(spec.fields[i]);
  }


}

void free_spec(struct Spec* spec){
  free(spec->spec_id);
  for (int i=0; i<spec->num_of_fields; i++){
    free(spec->fields[i].category);
    free(spec->fields[i].details);
  }
  free(spec->fields);

}