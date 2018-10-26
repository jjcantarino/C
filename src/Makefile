default: myprog 

# To create the executable file myprog we need the object files

#creating ejectuables


myprog: main.o string_process.o linked-list.o red-black-tree.o
	gcc -g main.o string_process.o linked-list.o red-black-tree.o -o myprog -lm


main.o: main.c
	gcc -g -c main.c -o main.o

string_process.o: string_process.c  
	gcc -g -c string_process.c -o string_process.o

linked-list.o: linked-list.c
	gcc -g -c linked-list.c -o linked-list.o	

red-black-tree.o:  red-black-tree.c
	gcc -g -c red-black-tree.c -o red-black-tree.o



# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) myprog *.o *~ 


