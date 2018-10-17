gcc -c -g main.c red-black-tree.c linked-list.c string_process.c
gcc -g main.o red-black-tree.o linked-list.o string_process.o -o myprog -lm

./myprog ./dades/dades.csv ./aeroports/aeroports.csv IAD 
