default: myprog

# To create the executable file myprog we need the object files

#creating ejectuables


myprog: main.o string_process.o linked-list.o red-black-tree.o
	gcc main.o string_process.o linked-list.o red-black-tree.o -o myprog -lm


main.o: main.c
	gcc -c main.c -o main.o

string_process.o: string_process.c  
	gcc -c string_process.c -o string_process.o

linked-list.o: linked-list.c
	gcc -c linked-list.c -o linked-list.o	

red-black-tree.o:  red-black-tree.c
	gcc -c red-black-tree.c -o red-black-tree.o



# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) myprog *.o *~ 


