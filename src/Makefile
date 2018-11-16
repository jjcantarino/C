default: myprog 

# To create the executable file myprog we need the object files

#creating ejectuables


myprog: main.o linked-list.o red-black-tree.o controller.o unformatted_data.o formatted_data.o
	gcc -g main.o linked-list.o red-black-tree.o controller.o formatted_data.o unformatted_data.o -o myprog -lm -lpthread


main.o: main.c
	gcc -g -c main.c -o main.o

formatted_data.o: formatted_data.c  
	gcc -g -c formatted_data.c -o formatted_data.o

unformatted_data.o: unformatted_data.c  
	gcc -g -c unformatted_data.c -o unformatted_data.o

controller.o: controller.c  
	gcc -g -c controller.c -o controller.o

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


