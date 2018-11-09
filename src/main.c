//@author: Jonatan Cantarino
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "string_process.h"
#include "red-black-tree.h"
#include "linked-list.h"

#define MAXLINE      200
#define MAGIC_NUMBER 0x0133C8F9
#define true 1
#define false 0
#define CREATE 1
#define SAVE 2
#define LOAD 3
#define INFO 4
#define EXIT 5

int get_max_inflight_recursive(node *node, char* airport, int max_times){
    if(node!=NIL){
        if(node->right != NIL)
            max_times=get_max_inflight_recursive(node->right, airport, max_times);
        if(node->left != NIL)
            max_times=get_max_inflight_recursive(node->left, airport, max_times);
        if(max_times < node->data->list->num_items){
            max_times = node->data->list->num_items;
            strcpy(airport, node->data->key);
        }
    }
    return max_times;    
}
int clean_memory(rb_tree * tree){
    if(tree != NULL){
      delete_tree(tree);
    }         
}

int create_tree(rb_tree * tree, char* aeroportsFilename, char* dadesFilename){
        //construccio dels nodes origen de l'arbre
        //reserva de memoria per l'arbre, (si no hi ha espai sortim)
        //inicialitzacio del tree
        init_tree(tree);   
        aeroports_process(aeroportsFilename, tree);
        dades_process(dadesFilename, tree);
}

int load_tree(rb_tree * tree, char* treeFilename){

        FILE *fp;          
        int32_t int_input;
        fp = fopen(treeFilename , "rb");
        if(fp == NULL) {
            perror("Error opening file");
            return(-1);
        }
        fread(&int_input, sizeof(int32_t), 1, fp);
        if(int_input == MAGIC_NUMBER)
            printf("Save and Load succeded %#08x \n", int_input);        
        
        fread(&int_input, sizeof(int32_t), 1, fp);
        if(int_input != 0)
            printf("Loaded num nodes %d \n", int_input);     
        while(fread)
        load_tree_recursive(tree->root, fp);
    
        fclose(fp);
}


int load_tree_recursive(node *node, FILE *fp){
    if(node!=NIL){
        if(node->right != NIL)
            load_tree_recursive(node->right, fp);
        if(node->left != NIL)
            load_tree_recursive(node->left, fp);

        fread(node->data->key,sizeof(char),strlen(node->data->key)-1,fp);
        fread(node->data->list->num_items, sizeof(int), 1, fp);
        //load_list_recursive(node->data->list->first, fp);
        //save_list
    }
}

int load_list_recursive(list_item *node, FILE *fp){
    if(node!=NIL){
        if(node->next != NIL)
            load_list_recursive(node->next, fp);
        
        fread(node->data->key, sizeof(char), strlen(node->data->key)-1, fp);
        fread(node->data->num_flights, sizeof(int32_t), 1, fp);
        fread(node->data->minutes, sizeof(float), 1, fp);
    }
}


int save_list_recursive(list_item *node, FILE *fp){
    if(node!=NIL){
        if(node->next != NIL)
            save_list_recursive(node->next, fp);
        
        fwrite(&node->data->key, sizeof(char), strlen(node->data->key)-1, fp);
        fwrite(&node->data->num_flights, sizeof(int32_t), 1, fp);
        fwrite(&node->data->minutes, sizeof(float), 1, fp);
    } 
}

int save_tree_recursive(node *node, FILE *fp){
    if(node!=NIL){
        if(node->right != NIL)
            save_tree_recursive(node->right, fp);
        if(node->left != NIL)
            save_tree_recursive(node->left, fp);

        fwrite(&node->data->key,sizeof(char),strlen(node->data->key)-1,fp);
        fwrite(&node->data->list->num_items, sizeof(int), 1, fp);
        //save_list_recursive(node->data->list->first, fp);
        //save_list
    }
}

int save_tree(rb_tree * tree, char* treeFilename){
        FILE *fp;          
        fp = fopen(treeFilename , "wb");
        int32_t int_input = MAGIC_NUMBER;
        if(fp == NULL) {
            perror("Error opening file");
            return(-1);
        }
        fwrite(&int_input, sizeof(int32_t), 1, fp);
        printf("TREE NUM NODES: %d", tree->num_nodes);
        fwrite(&tree->num_nodes, sizeof(int32_t), 1, fp);
        save_tree_recursive(tree->root, fp);
        fclose(fp);
}

/*
    Busqueda del aeroport origen amb mes destinacions

*/
int get_max_inflight(rb_tree * tree){
    int max_times = 0;
    char * airport; 

    if(tree != NULL)
    {
        if(tree->root != NULL)
        {
            if((airport = (char *)malloc(1+strlen(tree->root->data->key)*sizeof(char)))==0) return report_error();   
            strcpy(airport, tree->root->data->key);  
            max_times = get_max_inflight_recursive(tree->root, airport, max_times);
            printf("L'aeroport amb mes destinacions es : %s \n amb  %d  destinacions \n", airport, max_times);
            free(airport);
        }
    }

}

/*
    Busqueda dels retards promitjos per un aeroport origen
*/
int get_average_delay(rb_tree * tree, char * origin){
    float retard = 0.;
    node_data *n_data;
    list_item *current_item;

    n_data = find_node(tree, origin);
    if (n_data == NULL){ 
        printf("Aeroport %s no trobat al fitxer csv \n", origin);
        return (-1);
    }
    //si la dimensio del 3er parametre es difernet de 3 no trobara cap aeroport origin
    if(strlen(origin) == 3){
        if (n_data != NULL) {
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
    } 
}

/**
 * 
 *  Menu
 * 
 */

int menu() 
{
    char str[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(str, 5, stdin);
    opcio = atoi(str); 

    return opcio;
}

/**
 * 
 *  Main procedure
 *
 */

int main(int argc, char **argv)
{    
    //guardem els arguments rebuts per parametre
    //declaracio de variables
    rb_tree * tree;

    char str1[MAXLINE], str2[MAXLINE];
    int opcio;
    int arbre_creat = false;

    if ((tree = (rb_tree *) malloc(sizeof(rb_tree)))==0) return report_error();

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case CREATE:
                if(arbre_creat==true)
                    clean_memory(tree);

                printf("Introdueix fitxer que conte llistat d'aeroports: \n Prem enter per buscarlo a ./aeroports/aeroports.csv");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;
                
                if(strlen(str1)<3)
                    strcpy(str1, "./aeroports/aeroports.csv");                    

                printf("Introdueix fitxer de dades: \n Prem enter per buscarlo a ./dades/dades.csv");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;

                if(strlen(str2)<3)
                strcpy(str2,"./dades/dades.csv");
                
                create_tree(tree, str1, str2);
                arbre_creat = true;

                break;

            case SAVE:
                
                if(arbre_creat==true){
                    printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
                    fgets(str1, MAXLINE, stdin);
                    str1[strlen(str1)-1]=0;

                    if(strlen(str1)<3)
                        strcpy(str1, "./tree");    

                    save_tree(tree, str1);
                    clean_memory(tree);
                    arbre_creat = false;

                    /* Falta codi */
                }
                else
                    printf("No existeix cap arbre creat!");    

                break;

            case LOAD:
                if(arbre_creat==true){
                    printf("Ja tens un arbre creat, es perdrà, vols continuar? Y/N");
                    fgets(str1, MAXLINE, stdin);
                    str1[strlen(str1)-1]=0;

                    if(strcmp(str1,"Y")==0){
                        clean_memory(tree);
                        arbre_creat = false;
                    }
                }
                if(arbre_creat==false){
                    printf("Introdueix nom del fitxer que conte l'arbre: ");
                    fgets(str1, MAXLINE, stdin);
                    str1[strlen(str1)-1]=0;       

                    if(strlen(str1)<3)
                        strcpy(str1, "./tree");    

                    load_tree(tree, str1);
                    //arbre_creat = true;                

                }

                /* Falta codi */

                break;

            case INFO:
                //WE ASK FOR INPUT
                printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destins: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                //IF NO INPUT GIVEN THEN SEARCH MAX INFLIGHT
                //ELSE GET AVERAGE DELAY FOR GIVEN AIRPORT
                if (strlen(str1)<3)
                    get_max_inflight(tree);
                else
                    get_average_delay(tree, str1);

                /* Falta codi */

                break;
            case EXIT:

                //IF TREE CREATED WE MUST CLEAN MEMORY ASSOCIATED

                if(arbre_creat==true)clean_memory(tree);
                arbre_creat=false;

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);
    free(tree);
    return 0;
}
