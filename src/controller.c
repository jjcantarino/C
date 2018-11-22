//@author: Jonatan Cantarino
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "controller.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

void *thr_fn(void *arg)
{
  printids("nou fil: ");
  return((void *)0);
}

void printids(const char *s)
{
  pid_t pid;
  pthread_t tid;

  pid = getpid();
  tid = pthread_self();
  printf("%s pid %u tid %u (0x%x)\n", s, (unsigned int)pid,
      (unsigned int)tid, (unsigned int)tid);
}


/*
    CREATES A NODE TO BE INSERTED IN THE TREE
    REPRESENTS AIRPORTS ORIGIN
*/
int create_tree_node(char* key, rb_tree * tree){
    
    node_data *n_data; //apunta a una lista indexada amb clau aeroport origen
    //comprovem que no existeixi el aeroport per tal d'evitar duplicats
    if ((n_data = malloc(sizeof(*n_data)))==0)return report_error();           
    // This is the key by which the node is indexed in the tree 
    if ((n_data->key =(char *)malloc(1+strlen(key)*sizeof(char)))==0)return report_error();            
    strcpy(n_data->key, key);  
    if ((n_data->list = malloc(sizeof(list_data)))==0)return report_error();
    n_data->mutex = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
    init_list(n_data->list);
    insert_node(tree, n_data);
 }

/*
    UPSERT A NODE IN DESTINATIONS LINKED LIST
    GIVEN AN ORIGIN, DESTINY INSERT OR UPDATE ITS CONTENT
*/
int update_list_entry(rb_tree * tree, char* origin, char* dest, float delay, int param_flights){
    
    node_data *n_data;
    list_data * linked_data;

    if( tree != NULL){

        n_data = find_node(tree, origin);

        if (n_data != NULL) {
            //if list_data

            pthread_mutex_lock(&n_data->mutex);
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
            pthread_mutex_unlock(&n_data->mutex);
        } 
    }

}

//PRINT ERROR AND RETURN -1

int report_error(){
   printf("Not enough space, exiting \n");
   return (-1);
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

/*
    REMOVE TREE CONTENT

*/

int clean_memory(thread_args *args){
    if(args->tree != NULL)
        delete_tree(args->tree);
}

/*
    CREATE TREE (FORMATTED DATA, OTHERWISE WOULD BE LOAD)

int create_tree(rb_tree * , char*, char*);
*/

int create_tree(thread_args *args){
    
    FILE *fp;
    pthread_t ntid_f1, ntid_f2;
    void *ret1;
    void* ret2;
    int i = 0;
    char str[100];      
    printf("TID: %i \n", pthread_self());

    //INIT TREE
    init_tree(args->tree);   

    //TREE CREATION
    aeroports_process(args->str1, args->tree);

    printf("Filename: %s \n", args->str2);

    args->mutex_fp = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

    fp = fopen(args->str2 , "r");
    args->fp = fp;
    printf("Filename: %s  FP: %d \n", args->str2, args->fp);

    if(args->fp == NULL) {
        perror("Error opening file");
        return(-1);
    }
    fgets (str, 100, args->fp);//ignorem capçalera

    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);

    while (ret1 != 1 && ret2 != 1){
        if(pthread_create(&ntid_f1, NULL, fill_tree, args)!=0) return report_error();
        if(pthread_create(&ntid_f2, NULL, fill_tree, args)!=0) return report_error();
        pthread_join(ntid_f1, &ret1);
        pthread_join(ntid_f2, &ret2);
    }

    /* Tiempo cronologico */
    gettimeofday(&tv2, NULL);

    /* Tiempo para la creacion del arbol */
    printf("Tiempo para crear el arbol: %f segundos\n",
            (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec));

    fclose(fp);
   // if(pthread_create(&ntid_f2, NULL, fill_tree, args)!=0) return report_error();
   

  return((void *)0);
}

void *fill_tree(void * arg){


    struct thread_args *args = arg;
    //printids("Fil secundari : ");
    dades_process(args->fp, args->tree, args->mutex_fp);

}
/*
    GIVEN AN ORIGIN SEARCHS FOR ITS AVERAGE DELAY
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
