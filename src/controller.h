#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "red-black-tree.h"
#include "linked-list.h"
#include "formatted_data.h"
#include "unformatted_data.h"

#define MAX_ROW_READ 10

typedef struct thread_args
 {
    rb_tree * tree;
    char * str1;
    char * str2;
    FILE * fp;
    pthread_mutex_t mutex_fp;
}thread_args;

int create_tree(thread_args *);
//fitxer dades process
void * fill_tree(void *);
//fitxer aeroports process
void *generate_tree(void * );

//P2
int update_list_entry(rb_tree *, char*, char*, float, int);
int create_tree_node(char*, rb_tree *);

int clean_memory(thread_args *);

void printids(const char *);

//P1
int get_average_delay(rb_tree *, char *);
int get_max_inflight(rb_tree *);
int get_max_inflight_recursive(node *, char*, int );
int report_error();

#endif
