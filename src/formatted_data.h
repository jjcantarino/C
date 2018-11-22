#ifndef FORMATTED_DATA_H
#define FORMATTED_DATA_H

#include "red-black-tree.h"
#include "linked-list.h"
#include "controller.h"


int dades_process(FILE*, rb_tree *, pthread_mutex_t);
int aeroports_process(char*, rb_tree * tree);
int report_error();

#endif
