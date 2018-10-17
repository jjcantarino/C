//@author: Jonatan Cantarino
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "string_process.h"
#include "red-black-tree.h"
#include "linked-list.h"

int main(int argc, char *argv[])
{  
    //guardem els arguments rebuts per parametre
    char* dadesFilename = argv[1];
    char* aeroportsFilename = argv[2];
    char* origin = argv[3];
    //declaracio de variables
    rb_tree * tree;
    node_data *n_data;
    list_item *current_item;
    //reserva de memoria per l'arbre, (si no hi ha espai sortim)
    //inicialitzacio del tree
    if((tree = (rb_tree *) malloc(sizeof(rb_tree)))==0) return report_error();
    init_tree(tree);   

    float retard = 0.;
    int max_times = 0;
    char * airport;
    if((airport = (char *)malloc(3*sizeof(char)))==0) return report_error();   

    //construccio dels nodes origen de l'arbre
    if(strstr(aeroportsFilename,"aeroport")!=NULL){
        aeroports_process(aeroportsFilename, tree);
    }   
    //parsejat del csv i construccio de les llistes indexades
    if(strstr(dadesFilename,"dades")!=NULL){
        dades_process(dadesFilename, tree);
    }
    //si trobem el node origen

    //Busqueda dels retards promitjos per un aeroport origen
    if ((n_data = malloc(sizeof(node_data)))==0)return report_error();  
    n_data = find_node(tree, origin);
    if (n_data == NULL){ 
        printf("Aeroport %s no trobat al fitxer csv \n", origin);
        return (-1);
    }
    //si la dimensio del 3er parametre es difernet de 3 no trobara cap aeroport origin
    if(strlen(origin) == 3){
        if ((n_data = malloc(sizeof(node_data)))==0)return report_error();  
        n_data = find_node(tree, origin);
        if (n_data != NULL) {
            if ((current_item = malloc(sizeof(list_item)))==0)return report_error();  
            current_item = n_data->list->first;
            while(current_item != NULL){
                //print retard for each
                if(current_item->data->num_flights > 0)
                    retard = (float)(current_item->data->minutes)/(current_item->data->num_flights);
                printf("Aeroport Origen: %s Aeroport Desti: %s Retard mig: %2.3f\n", origin, current_item->data->key, retard);
                current_item = current_item->next;
                retard= 0.0;
            }
        }
            
    //Busqueda del aeroport origen amb mes destinacions

    }    
    if(tree!=NULL){
        if(tree->root!=NULL){
            printf("Entra \n");
            airport = tree->root->data->key;
            max_times = get_max_inflight(tree->root, airport, max_times);
        }
    }
    printf("L'aeroport amb mes destinacions es : %s \n amb  %d  destinacions \n", airport, max_times);
  
    if(current_item!=NULL)free(current_item);
    if(airport!=NULL)free(current_item);
    if(n_data!=NULL)free(n_data);
    if(tree!=NULL){
      delete_tree(tree);
      free(tree);
    }   
    return(0);
}
int get_max_inflight(node *node, char* airport, int max_times){
    if(node!=NIL){
        if(node->right != NIL)
            max_times=get_max_inflight(node->right, airport, max_times);
        if(node->left != NIL)
            max_times=get_max_inflight(node->left, airport, max_times);
        if(max_times < node->data->list->num_items){
            max_times = node->data->list->num_items;
            strcpy(airport, node->data->key);
        }
    }
    return max_times;    
}