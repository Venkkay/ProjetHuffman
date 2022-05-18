#ifndef FONCTIONS_HUFFMAN_H
#define FONCTIONS_HUFFMAN_H


// Prototypes des fonctions du fichier "fonctions_huffman.c"

void securite(int argc, char **argv);
unsigned int histogramme(unsigned int histogramme[], char nomFichierACompresser[]);
PlisteA tabToListeA(unsigned int histogramme[]);
void trierListe(PlisteA histogramme);
PArbre creationArbre(PlisteA histogramme);
void arbreNbBits(PArbre arbreHuffman, int niveau_arbre);
void recupBin(PArbre arbreHuffman, unsigned int codeBin, int niveau_arbre, tableHuffman table[]);
void compresserTexte(tableHuffman table[], char nomFichierACompresser[], char nomFichierCompresse[]);
int compresserBinaire(tableHuffman table[], unsigned int histogramme[], unsigned int nb_octet, char nomFichierACompresser[], char nomFichierCompresse[]);
void conversionBitText(tableHuffman table[]);
void compression(char nomFichierACompresser[], char nomFichierCompresse[]);
void lireEnTete(unsigned int histogramme[], char nomFichierADecompresse[], unsigned int *nb_octet, unsigned int *taille_en_tete);
PArbre rechercherOctet(PArbre arbreHuffman, int niveau_arbre, unsigned int carac_code, int nb_bits_total);
void decompresser(PArbre arbreHuffman, unsigned int taille_en_tete, unsigned int nb_octet, char nomFichierADecompresser[], char nomFichierDecompresse[]);
void decompression(char nomFichierADecompresser[], char nomFichierDecompresse[]);
void help();

#endif