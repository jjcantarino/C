#ifndef STRING_PROCESS_H
#define STRING_PROCESS_H
#include "red-black-tree.h"
#include "linked-list.h"


int update_list_entry(rb_tree *, char*, char*, float, int);
int create_tree_node(char* key, rb_tree * tree);
int dades_process(char*, rb_tree * tree);
int aeroports_process(char*, rb_tree * tree);
int report_error();

#endif
