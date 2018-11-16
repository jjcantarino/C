//@author: Jonatan Cantarino
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include "controller.h"
#include "red-black-tree.h"
#include <pthread.h>

#define MAXLINE 200
#define true 1
#define false 0
#define CREATE 1
#define SAVE 2
#define LOAD 3
#define INFO 4
#define EXIT 5


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
    thread_args * args;
    char str1[MAXLINE], str2[MAXLINE];
    int opcio;
    int arbre_creat = false;
    pthread_t ntid;

    if ((args = (thread_args *) malloc(sizeof(thread_args)))==0) return report_error();
    if ((tree = (rb_tree *) malloc(sizeof(rb_tree)))==0) return report_error();
    //https://linux.die.net/man/3/pthread_create
    args->tree = tree;
    args->str1 = str1;
    args->str2 = str2;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");
    
    printids("fil principal:");

    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case CREATE:
            
                if(arbre_creat==true)
                    clean_memory(args);

                printf("Introdueix fitxer que conte llistat d'aeroports: \n Prem enter per buscarlo a ./aeroports/aeroports.csv\n");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;
                
                if(strlen(str1)<3)
                    strcpy(str1, "./aeroports/aeroports.csv");                    

                printf("Introdueix fitxer de dades: \n Prem enter per buscarlo a ./dades/dades.csv\n");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;

                if(strlen(str2)<3)
                strcpy(str2,"./dades/dades.csv");

                if(pthread_create(&ntid, NULL, create_tree, args)!=0) return report_error();
            
                arbre_creat = true;
                sleep(1);

                break;

            case SAVE:
                
                if(arbre_creat==true){
                    printf("Introdueix el nom de fitxer en el qual es desara l'arbre: \nO polsa enter per agafar el per defecte (tree)\n");
                    fgets(str1, MAXLINE, stdin);
                    str1[strlen(str1)-1]=0;

                    if(strlen(str1)<3)
                        strcpy(str1, "./tree");    

                    save_tree(tree, str1);
                }
                else
                    printf("No existeix cap arbre creat!\n");    

                break;

            case LOAD:
                if(arbre_creat==true){
                    printf("Ja tens un arbre creat, es perdrà, vols continuar? (Y/N)\n");
                    fgets(str1, MAXLINE, stdin);
                    str1[strlen(str1)-1]=0;

                    if(strcmp(str1,"Y")==0){
                        clean_memory(args);
                        arbre_creat = false;
                    }
                }
                if(arbre_creat==false){
                    printf("Introdueix nom del fitxer que conte l'arbre: \nO polsa enter per agafar el per defecte (tree)\n");
                    fgets(str1, MAXLINE, stdin);
                    str1[strlen(str1)-1]=0;       

                    if(strlen(str1)<3)
                        strcpy(str1, "./tree");

                    if(load_tree(tree, str1)!=-1)
                        arbre_creat = true;      
                }

                /* Falta codi */

                break;

            case INFO:
                //WE ASK FOR INPUT
                if(arbre_creat==true){
                    printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destins: \n");
                    fgets(str1, MAXLINE, stdin);
                    str1[strlen(str1)-1]=0;

                    //IF NO INPUT GIVEN THEN SEARCH MAX INFLIGHT
                    //ELSE GET AVERAGE DELAY FOR GIVEN AIRPORT
                    if (strlen(str1)<3)
                        get_max_inflight(tree);
                    else
                        get_average_delay(tree, str1);
                }
                else
                    printf("No existeix cap arbre creat!\n");


                break;
            case EXIT:

                //IF TREE CREATED WE MUST CLEAN MEMORY ASSOCIATED

                if(arbre_creat==true)clean_memory(args);
                arbre_creat=false;

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);
    free(tree);
    free(args);
    return 0;
}
