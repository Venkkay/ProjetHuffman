/*------------------------------------------------
            Algorithme de Huffman
Par : Gautier Jordan, Etrillard Yann, Velay Lucas
-------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "type_variables.h"
#include "fonctions_generales.h"
#include "fonctions_huffman.h"


int main(int argc, char **argv){

    securite(argc, argv);

    if(strcmp(argv[1], "-c") == 0){
        printf("Compression de %s", argv[2]);
        compression(argv[2], argv[3]);
    }
    else if(strcmp(argv[1], "-d") == 0){
        printf("Decompression...");
        decompression(argv[2], argv[3]);
    }
    else if(strcmp(argv[1], "-h") == 0){
        help();
    }
    return 0;
}