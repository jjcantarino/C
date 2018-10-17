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

//processa l'arxiu dades.csv corresponent a vols
int dades_process(char* filename, rb_tree * tree){
     //declaracio de variables
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
        //per cada caracter de la linia
        for( j = 0 ; j < line_length; j++){
            //si es , contem la linia, si es \n contemplem el canvi de linia
            if( str[j] == ',' || str[j] == '\n' ){
                //el tamany d'aquesta cel la de la taula es igual a 
                //l'ultim index de ','' trobat menys l'actual index de ','
                word_length = j - first_idx;
                if(word_length > 0){
                    if(str[j] == '\n'){
                        str[j] = '\0';
                        column = 0;
                    }
                    //si l'index de columna == 14 correspon a una cel la de delay de vol
                    if( column == 14){//delay         
                        if ((str_delay = (char *)malloc(word_length*sizeof(char)))==0)return report_error();
                        // copiem a str_delay el valor corresponent al delay en format string
                        strncpy(str_delay, str+first_idx, word_length);          
                        //si aquest string conté una N i no un nombre la cel la es 'NA' i el delay sera 0
                        if(str_delay[0]=='N'){
                            current_delay = 0;

                        }
                        else{
                          //otherwise we convertim el delay a integer
                            current_delay = atoi(str_delay);
                        }
                     }
                    //si la l'index de la taula es 16 correspon a un codi IATA d'aeroport origen
                     //guardem a current_origin, per mapejar a la seguent iteracio
                    else if(column==16){                
                        if ((current_origin = (char *)malloc(word_length*sizeof(char)))==0)return report_error();
                        strncpy(current_origin, str+first_idx, word_length);          
                    }
                    //finalment, aquesta columna correspon a desti i ja podem afegir un node desti a la llista enllacada
                    //amb clau origen a l'arbre
                    else if(column==17){ //destination   
                        //guardem el codi IATA desti            
                        if ((current_dest = (char *)malloc(word_length*sizeof(char)))==0)return report_error();
                        strncpy(current_dest, str+first_idx, word_length);                  
                  
                        if ((n_data = malloc(sizeof(node_data)))==0)return report_error();  
                        if( tree != NULL){

                            n_data = find_node(tree, current_origin);

                            if (n_data != NULL) {
                                //if list_data
                                linked_data = find_list(n_data->list, current_dest);
                                //si existeix llista, vol dir que ja s'ha inicialitzat la llista desti
                                if(linked_data != NULL){
                                    //llavors, nomes afegim el delay i incrementem el numero de flights ORIGEN - DESTI
                                    linked_data->minutes = linked_data->minutes + current_delay;
                                    linked_data->num_flights++;
                                }
                                else{
                                  //sino, caldra crear la llista amb clau current_dest     

                                    if ((linked_data = malloc(sizeof(list_data)))==0)return report_error();
                                  
                                    linked_data->key = current_dest;
                                    linked_data->minutes = current_delay;
                                    linked_data->num_flights = 1;
                                    insert_list(n_data->list, linked_data);
                                }
                            } 
                        }
                    }
                }
                first_idx = j + 1;
                column++;
            }
        }
    }
    fclose(fp);

    return(0);
}
//funcio utilitzada continuament per retornar error de malloc
int report_error(){
   printf("Not enough space, exiting \n");
   return (-1);
}