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
    
  //Guardem a la variable filename el argv
  //proporcionat per terminal   
  rb_tree * tree;
  char* dadesFilename = argv[1];
  char* aeroportsFilename = argv[2];
  if((tree = (rb_tree *) malloc(sizeof(rb_tree)))==0) return report_error();
  init_tree(tree);   


  //Depenent del contingut d'aquest filename
  //cridem dinamicament a una funció o una altra

  if(strstr(aeroportsFilename,"aeroport")!=NULL){
      aeroports_process(aeroportsFilename, tree);
  }
  if(strstr(dadesFilename,"dades")!=NULL){
      dades_process(dadesFilename, tree);
  }

 return(0);
}

