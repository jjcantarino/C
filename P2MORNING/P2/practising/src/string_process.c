//@author: Jonatan Cantarino
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_process.h"
#include "red-black-tree.h"
#include "linked-list.h"


//Meto de que itera pel fitxer aeroports.csv en busca d'IATAcodes d'aeroports
//i inserta aquests ISOCodes com a claus d'un arbre
int aeroports_process(char* filename,rb_tree * tree){
    //declaracio de variables
    FILE *fp;          
    int num_aeroports;  
    char str[100];       
    int iata_length; //guarda la longitud del iata_code
    node_data *n_data; //apunta a una lista indexada amb clau aeroport origen

    fp = fopen(filename , "r");
    if(fp == NULL) {
      perror("Error opening file");
      return(-1);
    }
    if( fgets (str, 100, fp)!=NULL ){
        num_aeroports = atoi(str);              
    
    }
    while( fgets (str, 100, fp)!=NULL ) {
        
        iata_length = strlen(str);                    
        //tractem l'ultim caracter de la fila
        if (iata_length > 0 && str[iata_length-1] == '\n') {  
            str[--iata_length] = '\0';
        }       
        n_data = find_node(tree, str);
        //comprovem que no existeixi el aeroport per tal d'evitar duplicats
        if(n_data==NULL){            
            if ((n_data = malloc(sizeof(*n_data)))==0)return report_error();           
            // This is the key by which the node is indexed in the tree 
            if ((n_data->key =(char *)malloc(iata_length*sizeof(char)))==0)return report_error();            
            strcpy(n_data->key, str);  
            if ((n_data->list = malloc(sizeof(*n_data->list)))==0)return report_error();
            insert_node(tree, n_data);
        }
    }
}

//rep el path al fitxer per parametre
//parseja el fitxer
//emmagatzema a vectors
//endressa amb qsort (amb el comparador)
//mostra per pantalla
int dades_process(char* filename, rb_tree * tree){
   /*We declare variables;*/
   FILE *fp;            
   char str[100];       
   char **vector;
   char * current_origin;
   char * current_dest;
   char * str_delay;
   int current_delay;
   int word_length;
   int line_length;
   node_data *n_data;
   list_data * linked_data;
   int j = 0;
   int first_idx = 0;
   int column = 0;
   int current_column = 0;
   int nombre_elements = 0, i = 0;
   // Obrim el fitxer amb filename rebut per parametre
   fp = fopen(filename , "r");

   if(fp == NULL) {
      perror("Error opening file");
      return(-1);
   }
   //vector =  (char **)malloc(nombre_elements*sizeof(char *));
  
   //Agafem el nombre d'elements a parsejar, ja que la primera linia es un int amb aquest nombre 
   //reservem memoria per un vector indicant el tipus de dada, el numero d'elements i l'espai que ocupa una unitat
  while( fgets (str, 100, fp)!=NULL ) {
      //convertim a integer
      line_length = strlen(str);                    
      first_idx = 0;
      word_length = 0;
      column = 0;
      current_delay = 0;
      current_origin = NULL;
      current_dest = NULL;
      n_data = NULL;
      linked_data = NULL;
      for( j = 0; j < line_length; j++){
          if( str[j] == ',' || str[j] == '\n' ){
              word_length = j - first_idx;
              if(word_length > 0){
                  if(str[j] == '\n'){
                      str[j] = '\0';
                      column = 0;
                  }
                  if(column==14){//delay         
                      if ((str_delay = (char *)malloc(word_length*sizeof(char)))==0)return report_error();
                      strncpy(str_delay, str+first_idx, word_length);          
                      if(str_delay[0]=='N'){
                          current_delay = 0;

                      }else{
                          current_delay = atoi(str_delay);
                      }
                   }
                  else if(column==16){                
                      if ((current_origin = (char *)malloc(word_length*sizeof(char)))==0)return report_error();
                      strncpy(current_origin, str+first_idx, word_length);          
                  }
                  else if(column==17){ //destination               
                      if ((current_dest = (char *)malloc(word_length*sizeof(char)))==0)return report_error();
                      strncpy(current_dest, str+first_idx, word_length);                  
                
                      if ((n_data = malloc(sizeof(node_data)))==0)return report_error();  
                      if( tree != NULL){

                          n_data = find_node(tree, current_origin);

                          if (n_data != NULL) {
                              //if list_data
                              linked_data = find_list(n_data->list, current_dest);
                              if(linked_data != NULL){
                                  linked_data->minutes = linked_data->minutes + current_delay;
                                  linked_data->num_flights++;
                              }
                              else{

                                  if ((linked_data = malloc(sizeof(list_data)))==0)return report_error();
                                
                                  linked_data->key = current_dest;
                                  linked_data->minutes = current_delay;
                                  linked_data->num_flights = 1;
                                  insert_list(n_data->list, linked_data);
                              }
                          } 
                           else {
                            printf("NOT FOUND: %s \n", current_origin);
    /*
                              // If the key is not in the tree, allocate memory for the data
                              //   and insert in the tree

                              if ((n_data = malloc(sizeof(node_data)))==0)return report_error();
                              
                              
                              // This is the key by which the node is indexed in the tree 
                              n_data->key = current_origin;

                              if ((n_data->list = malloc(sizeof(list)))==0)return report_error();
                              
                              // This is additional information that is stored in the tree 

                              if ((linked_data = malloc(sizeof(list_data)))==0){
                                  }
                              if ((linked_data->key = (char *)malloc(word_length*sizeof(char)))==0)return report_error();

                              linked_data->key = current_dest;
                              linked_data->minutes = current_delay;
                              linked_data->num_flights++;
                              insert_list(n_data->list, linked_data);
                              // We insert the node in the tree 
                              insert_node(tree, n_data);*/
                          }      
                      }
                  }
              }
              first_idx = j + 1;
              column++;
          }
      }




      /*
      TO DO: 
            FOR EACH ROW: FIND KEY (ORIGIN) IN TREE, INSERT DESINY IN ITS LINKED LIST

      */

      //assignem l'espai necessari per ficar la cadena a i-essima posicio del vector
      //i finalment guardem el valor de la cadena modificada al vector
      

  }
   /*
   //Bucle que itera per les seguents linies i coloca al vector la dada rebuda
   while (fgets (str, 100, fp)!=NULL ) {
      //per tal d'eliminar el valor \n hem de saber la longitud, i li fiquem un valor \0
      line_length = strlen(str);                    

      if (line_length > 0 && str[line_length-1] == '\n') {  
          str[--line_length] = '\0';
      	  line_length--;
      }

      //assignem l'espai necessari per ficar la cadena a i-essima posicio del vector
      //i finalment guardem el valor de la cadena modificada al vector
      vector[i] = (char *)malloc(line_length*sizeof(char));
      
      strcpy(vector[i], str);               
      i++;
      
   }
   //quicksort c metode
   qsort(vector, nombre_elements, sizeof(char *), compare_string_length);
   //per cada element del vector printem per pantalla*/
   
   //Important: tancar fitxer i liberar l'espai de cada vector
    fclose(fp);

    return(0);
}

int report_error(){
   printf("Not enough space, exiting \n");
   return (-1);
}