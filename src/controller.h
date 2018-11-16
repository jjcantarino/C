#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "red-black-tree.h"
#include "linked-list.h"
#include "formatted_data.h"
#include "unformatted_data.h"


int update_list_entry(rb_tree *, char*, char*, float, int);
int create_tree_node(char*, rb_tree *);
int report_error();

int get_average_delay(rb_tree *, char *);
int get_max_inflight(rb_tree *);
int create_tree(rb_tree * , char*, char*);
int clean_memory(rb_tree * );
int get_max_inflight_recursive(node *, char*, int );

#endif
