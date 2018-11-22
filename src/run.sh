clear
make clean
make
valgrind --leak-check=full --show-leak-kinds=all ./myprog #./dades/dades.csv ./aeroports/aeroports.csv LAS 
#./myprog #./dades/dades.csv ./aeroports/aeroports.csv LAS 