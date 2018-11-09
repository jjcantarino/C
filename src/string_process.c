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
            str[iata_length-1] = '\0';
            iata_length--;
        }       
        create_tree_node(str, tree);

    }
    fclose(fp);
}

int create_tree_node(char* key, rb_tree * tree){
    
    node_data *n_data; //apunta a una lista indexada amb clau aeroport origen
    //comprovem que no existeixi el aeroport per tal d'evitar duplicats
    if ((n_data = malloc(sizeof(*n_data)))==0)return report_error();           
    // This is the key by which the node is indexed in the tree 
    if ((n_data->key =(char *)malloc(1+strlen(key)*sizeof(char)))==0)return report_error();            
    strcpy(n_data->key, key);  
    if ((n_data->list = malloc(sizeof(list_data)))==0)return report_error();
    init_list(n_data->list);
    insert_node(tree, n_data);
 }

int update_list_entry(rb_tree * tree, char* origin, char* dest, float delay, int param_flights){
    
    node_data *n_data;
    list_data * linked_data;

    if( tree != NULL){

        n_data = find_node(tree, origin);

        if (n_data != NULL) {
            //if list_data
            linked_data = find_list(n_data->list, dest);
            //si existeix llista, vol dir que ja s'ha inicialitzat la llista desti
            if(linked_data != NULL){
                //llavors, nomes afegim el delay i incrementem el numero de flights ORIGEN - DESTI
                linked_data->minutes = linked_data->minutes + delay;
                linked_data->num_flights = linked_data->num_flights + param_flights;
                free(dest);
            }
            else{
                
                //sino, caldra crear la llista amb clau current_dest     
                if ((linked_data = malloc(sizeof(*linked_data)))==0)return report_error();
                linked_data->key = dest;
                linked_data->minutes = delay;
                linked_data->num_flights = param_flights;

                insert_list(n_data->list, linked_data);
            }
        } 
    }

}

//processa l'arxiu dades.csv corresponent a vols
int dades_process(char* filename, rb_tree * tree){
     //declaracio de variables
    FILE *fp;            
    char str[100];      
    char * current_origin;
    char * current_dest;
    char * str_delay;
    int current_delay;
    int word_length;
    int line_length;
    int j = 0;
    int begin_word = 0;
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
        begin_word = 0; 
        column = 0;
        //per cada caracter de la linia
        for( j = 0 ; j < line_length; j++){
            //si es , contem la linia, si es \n contemplem el canvi de linia
            if( str[j] == ',' || str[j] == '\n' ){
                //el tamany d'aquesta cel la de la taula es igual a 
                //l'ultim index de ','' trobat menys l'actual index de ','
                word_length = j - begin_word;
                if(word_length > 0){
                    if(str[j] == '\n'){
                        str[j] = '\0';
                        column = 0;
                    }
                    //si l'index de columna == 14 correspon a una cel la de delay de vol
                    if( column == 14){//delay         
                        if ((str_delay = (char *)malloc(1+word_length*sizeof(char)))==0)return report_error();
                        // copiem a str_delay el valor corresponent al delay en format string
                        strncpy(str_delay, str+begin_word, word_length);         
                        str_delay[word_length]= '\0'; 
                        //si aquest string conté una N i no un nombre la cel la es 'NA' i el delay sera 0
                        if(str_delay[0]=='N'){
                            current_delay = 0;
                        }
                        else{
                          //otherwise we convertim el delay a integer
                            current_delay = atoi(str_delay);
                        }
                        free(str_delay);
                     }
                    //si la l'index de la taula es 16 correspon a un codi IATA d'aeroport origen
                     //guardem a current_origin, per mapejar a la seguent iteracio
                    else if(column==16){                
                        if ((current_origin = (char *)malloc(1+word_length*sizeof(char)))==0)return report_error();
                        strncpy(current_origin, str+begin_word, word_length);          
                        current_origin[word_length]= '\0'; 
                    }
                    //finalment, aquesta columna correspon a desti i ja podem afegir un node desti a la llista enllacada
                    //amb clau origen a l'arbre
                    else if(column==17){ //destination   
                        //guardem el codi IATA desti            
                        if ((current_dest = (char *)malloc(1+word_length*sizeof(char)))==0)return report_error();
                        strncpy(current_dest, str+begin_word, word_length);                  
                        current_dest[word_length]= '\0'; 

                        update_list_entry(tree, current_origin, current_dest, current_delay, 1);
                        free(current_origin);
                    }
                }
                begin_word = j + 1;
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