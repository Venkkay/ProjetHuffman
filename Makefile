CC = gcc

all : huffman

huffman : main.o fonctions_generales.o fonctions_huffman.o
	$(CC) -o huffman main.o fonctions_generales.o fonctions_huffman.o

main.o : main.c	fonctions_huffman.h fonctions_generales.h type_variables.h
	$(CC) -c -Wall main.c

fonctions_generales.o: fonctions_generales.c type_variables.h
	$(CC) -c -Wall fonctions_generales.c

fonctions_huffman.o : fonctions_huffman.c type_variables.h
	$(CC) -c -Wall fonctions_huffman.c

clean : 
	rm -rf *.o
