#ifndef UNFORMATTED_DATA_H
#define UNFORMATTED_DATA_H
#include "red-black-tree.h"
#include "linked-list.h"
#include "controller.h"

int load_tree(rb_tree *, char*);
int save_list(list *, FILE *);
int save_tree_recursive(node *, FILE *);
int save_tree(rb_tree *, char*);

#endif
