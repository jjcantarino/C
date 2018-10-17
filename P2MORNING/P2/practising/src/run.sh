gcc -c main.c red-black-tree.c linked-list.c string_process.c
gcc main.o red-black-tree.o linked-list.o string_process.o -o myprog -lm

./myprog ./dades/dades2.csv ./aeroports/aeroports.csv  
