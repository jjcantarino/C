//@author: Jonatan Cantarino
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "controller.h"

#define MAGIC_NUMBER 0x0133C8F9
#define IATA_SIZE 3


int load_tree(rb_tree * tree, char* treeFilename){

        FILE *fp;          
        int32_t int_input; 
        char *current_origin;
        char *current_dest;
        int num_dest;
        float current_delay;
        int current_num_flights;
        
        int32_t num_nodes;
        fp = fopen(treeFilename , "rb");
        if(fp == NULL) {
            perror("Error opening file");
            return(-1);
        }
        init_tree(tree);   
       
        fread(&int_input, sizeof(int32_t), 1, fp);
        if(int_input == MAGIC_NUMBER)
            printf("MAGIC_NUMBER %#08x succeeded, loading tree... \n", int_input);        
        
        fread(&num_nodes, sizeof(int32_t), 1, fp);
        //per cada node
        for (int32_t i = 0; i < num_nodes; i++){
         
            if ((current_origin = (char *)malloc(1+IATA_SIZE*sizeof(char)))==0)return report_error();
            current_origin[IATA_SIZE]= '\0'; 
             
            fread(current_origin, sizeof(char), IATA_SIZE, fp);          
            //current_origin[IATA_SIZE]= '\0'; 
            create_tree_node(current_origin, tree);
            
            fread(&num_dest, sizeof(int), 1, fp);          
            
            for (int j = 0; j < num_dest ; j++){

                if ((current_dest = (char *)malloc(1+IATA_SIZE*sizeof(char)))==0)return report_error();
                    
                fread(current_dest, sizeof(char), IATA_SIZE, fp);          
                current_dest[IATA_SIZE]= '\0'; 
                fread(&current_num_flights, sizeof(int), 1, fp);          
                fread(&current_delay, sizeof(float), 1, fp);
            
                update_list_entry(tree, current_origin, current_dest, current_delay, current_num_flights);

            }
            free(current_origin);
        }
        
        fclose(fp);
}


int save_list(list *l, FILE *fp){

    list_item *current;

    current = l->first;

    float delay = 0.0;

    while (current != NULL)
    {
        
        fwrite(current->data->key, sizeof(char), strlen(current->data->key), fp);
        fwrite(&current->data->num_flights, sizeof(int32_t), 1, fp);
        
        if(current->data->num_flights > 0 )
            delay = current->data->minutes;
        else
            delay = 0.0;

        fwrite(&delay, sizeof(float), 1, fp);
        
        current = current->next;
    } 
}

int save_tree_recursive(node *node, FILE *fp){
    if(node!=NIL){
        if(node->right != NIL)
            save_tree_recursive(node->right, fp);
        if(node->left != NIL)
            save_tree_recursive(node->left, fp);

        fwrite(node->data->key,sizeof(char),strlen(node->data->key),fp);
        fwrite(&node->data->list->num_items, sizeof(int), 1, fp);
        //printf("ORigin %s NUM DEST: %int",node->data->key, node->data->list->num_items);
        
        save_list(node->data->list, fp);
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
        printf("\n Writing current tree in %s file", treeFilename);
        fwrite(&int_input, sizeof(int32_t), 1, fp);
        fwrite(&tree->num_nodes, sizeof(int32_t), 1, fp);
        save_tree_recursive(tree->root, fp);
        fclose(fp);
}