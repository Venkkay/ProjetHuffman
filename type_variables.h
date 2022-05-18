/*
    Fichier compose des structures et des constantes utiles a tout le programme
*/

#ifndef TYPE_VARIABLES_H
#define TYPE_VARIABLES_H

//Definition des constantes
#define MALLOC(x)((x * ) malloc(sizeof(x)))
#define TRUE 1
#define FALSE 0
#define ARBRENULL NULL
#define LISTENULL NULL

//Definition du type Element representant un caractere et son occurrence dans le fichier lu
typedef struct defElmt{
    unsigned char caractere;
    unsigned int occurrence;
}Element;

/*Definition du type tableHuffman compose du caractere/octet compresse, du caractere compresse/octet 
au format texte et du nombre de bit du caracter/octet compresse*/
typedef struct defTableHuffman{
    unsigned int caractere_code_binaire;
    char caractere_code_texte[32];
    unsigned int nb_bits;
}tableHuffman;

//Structure de l'arbre huffman
typedef struct arb{
    Element elmt;
    int somme_occurrence;
    struct arb* fg;
    struct arb* fd;
    unsigned int nb_bits;
} Arbre;
typedef Arbre* PArbre;

//Structure de la liste d'Arbre permettant de former l'arbre de Huffman
typedef struct lstA{
    PArbre valA;
    struct lstA * suivA;
} ListeA;
typedef ListeA* PlisteA;

#endif