#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#include "type_variables.h"
#include "fonctions_generales.h"
#include "fonctions_huffman.h"

/*  Fonction de securite permettant de faire des verifications des le debut du programme et de le fermer 
    immediatement s'il y a un problème. Il y est par exemple verifie le nombre d'argument passe en parametre 
    et si l'option a bien ete passe en parameter  */

void securite(int argc, char **argv){

    if(argc !=2 && argc != 4){
        fprintf(stderr, "ERREUR NOMBRE ARGUMENT. Lisez le READ ME ou utilisez l'option -h.");
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1], "-c") && strcmp(argv[1], "-d") && strcmp(argv[1], "-h")){
        fprintf(stderr, "ERREUR ARGUMENT OPTION. Lisez le READ ME ou utilisez l'option -h.");
        exit(EXIT_FAILURE); 
    }
    if(!strcmp(argv[1], "-d") && strcmp(strrchr(argv[2], '.'), ".hfzip")){
        fprintf(stderr, "ERREUR ARGUMENT NOM DE FICHIER. Le fichier a decompresser doitetre un '.hfzip'");
        exit(EXIT_FAILURE);
    }
    if(!strcmp(argv[1], "-h") && argc !=2){
        fprintf(stderr, "ERREUR ARGUMENT");
        exit(EXIT_FAILURE);
    }
}


/*  Fonction creant l'histogramme dans un tableau de 256 cases, ainsi la cases contient l'occurrnece du caractere/octet
    et il est facile d'y acceder en utilisant le carctere/octet en index   */

unsigned int histogramme(unsigned int histogramme[], char nomFichierACompresser[]){
    //On recupere chaque caractere à auquel on associe son nombre d'occurence

    FILE* fichierACompresser = NULL;
    int caractere;
    unsigned int nb_caractere = 0;

    fichierACompresser = fopen(nomFichierACompresser, "r");
    if(fichierACompresser == NULL){
        perror("Erreur dans l'ouverture du fichier à compresser");
        exit(EXIT_FAILURE);
    }

    while((caractere = fgetc(fichierACompresser))!=EOF){
        histogramme[caractere]++;
        nb_caractere++;
    }
    fclose(fichierACompresser);
    return nb_caractere;
}


/*  Fonction permettant de passer du tableau d'occurences à une liste d'arbre de un seul noeud chacun 
    pour par la suite creer l'arbre de huffman    */

PlisteA tabToListeA(unsigned int histogramme[]){
    Element elmtListeA;
    PlisteA listeArbre = NULL;
    for(int i=0; i<= 255; i++){
        if(histogramme[i] > 0){
            elmtListeA.caractere = (unsigned char)i;
            elmtListeA.occurrence = histogramme[i];
            listeArbre = consAElement(elmtListeA, listeArbre);
        }
    }
    return listeArbre;
}

// Fonction triant la liste d'arbre pour creer l'arbre de huffman

void trierListe(PlisteA histogramme){
   PlisteA tmp, tmp1, tmp2;
   PArbre min;
   for(tmp=histogramme ; tmp!=NULL ; tmp=tmp->suivA){
     tmp2=tmp;
     min=tmp->valA;
     for(tmp1=tmp->suivA ; tmp1!=NULL ; tmp1=tmp1->suivA){
        if(min->somme_occurrence > tmp1->valA->somme_occurrence){
           tmp2=tmp1; 
           min=tmp2->valA;
        }
     }
     tmp2->valA = tmp->valA;
     tmp->valA = min;
   }
}


// Fonction de creation de l'arbre de Huffman

PArbre creationArbre(PlisteA histogramme){
    unsigned int somme = 0;
    PArbre noeudSomme = NULL;
    PArbre fg = NULL;
    PArbre fd = NULL;
    while(histogramme->suivA != NULL){
        trierListe(histogramme);
        somme = histogramme->valA->somme_occurrence + histogramme->suivA->valA->somme_occurrence;

        fg = histogramme->valA;
        histogramme = supprimerNoeudListA(histogramme);
        fd = histogramme->valA;
        histogramme = supprimerNoeudListA(histogramme);

        noeudSomme = creerArbreSomme(somme, fg, fd);

        histogramme = consASomme(noeudSomme, histogramme);
    }
    PArbre arbreHuffman = histogramme->valA;
    return arbreHuffman;
}

// Fonction permettant d'enregistrer le niveau de l'arbre correspond au nombre de bit de l'octet compresse pour chaque feuille

void arbreNbBits(PArbre arbreHuffman, int niveau_arbre){
    if(estVideArb(arbreHuffman)){
        return ;
    }
    else if(estFeuille(arbreHuffman)){
        arbreHuffman->nb_bits = niveau_arbre;
        return ;
    }
    else{
        if(!estVideArb(arbreHuffman->fg)){
            arbreNbBits(arbreHuffman->fg, niveau_arbre+1);
        }
        if(!estVideArb(arbreHuffman->fd)){
            arbreNbBits(arbreHuffman->fd, niveau_arbre+1);
        }
    }
}


/*  Fonction recuperant la valeur de chaque caractere/octet a partir de l'arbre de huffman et 
    la stockant dans un tableau de type tableHufflman  */

void recupBin(PArbre arbreHuffman, unsigned int codeBin, int niveau_arbre, tableHuffman table[]){
    if(estVideArb(arbreHuffman)){
        return ;
    }
    else if(estFeuille(arbreHuffman)){
        table[arbreHuffman->elmt.caractere].caractere_code_binaire = codeBin;
        table[arbreHuffman->elmt.caractere].nb_bits = niveau_arbre;
        return ;
    }
    else{
        if(!estVideArb(arbreHuffman->fg)){
            recupBin(arbreHuffman->fg, (codeBin<<1), niveau_arbre+1, table);
        }
        if(!estVideArb(arbreHuffman->fd)){
            recupBin(arbreHuffman->fd, (codeBin<<1)+1, niveau_arbre+1, table);
        }
    }
}


/*  Fonction ecrivant le fichier compresse dans un fichier texte normal pour pouvoir voir le fichier 
    compresse avec les 0 et les 1*/

void compresserTexte(tableHuffman table[], char nomFichierACompresser[], char nomFichierCompresse[]){
    FILE* fichierACompresser = NULL;
    FILE* fichierCompresseTxt = NULL;
    int caractere = 0;

    fichierACompresser = fopen(nomFichierACompresser, "r");
    if(fichierACompresser == NULL){
        perror("Erreur dans l'ouverture du fichier à compresser");
        exit(EXIT_FAILURE);
    }
    fichierCompresseTxt = fopen(nomFichierCompresse, "w");
    if(fichierCompresseTxt == NULL){
        perror("Erreur dans l'ouverture du fichier compresse");
        exit(EXIT_FAILURE);
    }

    do{
        caractere = fgetc(fichierACompresser);
        fputs(table[caractere].caractere_code_texte, fichierCompresseTxt);
    }while(caractere != EOF);

    fclose(fichierACompresser);
    fclose(fichierCompresseTxt);
}


// Fonction creant le fichier compresse a partir de la table de Huffman

int compresserBinaire(tableHuffman table[], unsigned int histogramme[], unsigned int nb_octet, char nomFichierACompresser[], char nomFichierCompresse[]){
    FILE* fichierACompresser;
    FILE* fichierCompresse;
    unsigned int char_tmp = 0;
    unsigned int caractere = 0;
    int nb_bits_total = 0;
    unsigned char caractere_compresse;
    int nb_octet_fichier_compresse = 0;

    //Ouverture et test d'erreur des fichiers
    fichierACompresser = fopen(nomFichierACompresser, "r");
    if(fichierACompresser == NULL){
        perror("Erreur dans l'ouverture du fichier à compresser");
        exit(EXIT_FAILURE);
    }
    fichierCompresse = fopen(nomFichierCompresse, "wb");
    if(fichierCompresse == NULL){
        perror("Erreur dans l'ouverture du fichier compresse");
        exit(EXIT_FAILURE);
    }

    // Ecriture de l'histogramme dans le fichier compresse
    fwrite(&nb_octet, sizeof(unsigned int), 1, fichierCompresse);
    nb_octet_fichier_compresse += sizeof(unsigned int);
    for(int i=0; i<=255; i++){
        if(histogramme[i] > 0){
            fwrite(&i, sizeof(unsigned char), 1, fichierCompresse);
            fwrite(&histogramme[i], sizeof(unsigned int), 1, fichierCompresse);
            nb_octet_fichier_compresse += sizeof(unsigned char) + sizeof(unsigned int);
        } 
    }

    // Ecriture du fichier en lui meme dans le fichier compresse
    while((caractere = fgetc(fichierACompresser)) != EOF){
        char_tmp = char_tmp << table[caractere].nb_bits;
        nb_bits_total = nb_bits_total + table[caractere].nb_bits;
        char_tmp = char_tmp | table[caractere].caractere_code_binaire;

        while(nb_bits_total >= 8){
            nb_bits_total = nb_bits_total - 8;
            caractere_compresse = char_tmp >> nb_bits_total;
            fputc(caractere_compresse, fichierCompresse);
            
            nb_octet_fichier_compresse += sizeof(unsigned char);
        }
    }
    if(nb_bits_total != 0){
        caractere_compresse = char_tmp << (8-nb_bits_total);
        fputc(caractere_compresse, fichierCompresse);
        nb_octet_fichier_compresse += sizeof(unsigned char);
    }

    fclose(fichierACompresser);
    fclose(fichierCompresse);
    return nb_octet_fichier_compresse;
}


/*  Fonction permettant de convertir les valeurs compressees des caracteres et de les convertir en texte qui 
    est stocké dans le tableau de Huffman pour pouvoir ensuite les afficher avec la fonction "compresserTexte"  */

void conversionBitText(tableHuffman table[]){
    unsigned mask = 1;
	unsigned bit = 0 ;
    char codeTxt[32];
    for(int i=0; i<=255; i++){
        if(table[i].nb_bits != 0){
            mask = 1;
            bit = 0;
            codeTxt[0]= '\0';
            mask = mask << (table[i].nb_bits-1);
            for (int j = 0 ; j < table[i].nb_bits ; j++)
            {
                bit = (table[i].caractere_code_binaire & mask) >> (table[i].nb_bits-j-1) ;
                if(bit == 0){
                    strcat(codeTxt, "0");
                }
                else{
                    strcat(codeTxt, "1");
                }
                mask >>= 1 ;
            }
            strcpy(table[i].caractere_code_texte, codeTxt);
        }
    }
}


// Fonction de compression qui appelle les autres fonctions

void compression(char nomFichierACompresser[], char nomFichierCompresse[]){
    struct timeval start, end;
    gettimeofday(&start, NULL); //Fonction permettant de compter le temps de compression

    PlisteA histogrammeArb;
    PArbre arbreHuffman;
    unsigned int histogrammeTab[256]={0};
    tableHuffman table[256];
    unsigned int nb_octet_non_compresse = 0;
    unsigned int nb_octet_fichier_compresse = 0;
    float rapport_compression = 0;
    char *nomFichierCompresseTexte;
    char *nomFichierCompresseBinaire;    


    if((nomFichierCompresseTexte = ((char *)malloc(sizeof(char)* (6+strlen(nomFichierCompresse))))) == NULL){
        perror("Erreur dans le malloc de la fonction compression ");
        exit(EXIT_FAILURE);
    }
    strcpy(nomFichierCompresseTexte, nomFichierCompresse);
    strcat(nomFichierCompresseTexte, ".hftxt");

    if((nomFichierCompresseBinaire= ((char *)malloc(sizeof(char)* (6+strlen(nomFichierCompresse))))) == NULL){
        perror("Erreur dans le malloc de la fonction compression ");
        exit(EXIT_FAILURE);
    }
    strcpy(nomFichierCompresseBinaire, nomFichierCompresse);
    strcat(nomFichierCompresseBinaire, ".hfzip");


    nb_octet_non_compresse = histogramme(histogrammeTab, nomFichierACompresser);
    histogrammeArb = tabToListeA(histogrammeTab);
    arbreHuffman = creationArbre(histogrammeArb);
    for(int i=0; i<= 255; i++){
        table[i].caractere_code_binaire = 0;
        table[i].caractere_code_texte[0] = '\0';
        table[i].nb_bits = 0;
    }
    recupBin(arbreHuffman, 0, 0, table);
    conversionBitText(table);
    supprimerArbre(arbreHuffman);
    compresserTexte(table, nomFichierACompresser, nomFichierCompresseTexte);
    nb_octet_fichier_compresse = compresserBinaire(table, histogrammeTab, nb_octet_non_compresse, nomFichierACompresser, nomFichierCompresseBinaire);


    gettimeofday(&end, NULL);
    double temps_compression = (double)((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec)/1000000;
    printf("\nDone (%.3f sec)\n", temps_compression);

    //Permet d'afficher le nombre d'octet selon l'unite la plus adapte
    if(nb_octet_non_compresse < pow(10,3)){
        rapport_compression = ((float)nb_octet_fichier_compresse / (float)nb_octet_non_compresse)*100;
        printf("%d octets compresse en %d octets (%.1f%%)\n",nb_octet_non_compresse, nb_octet_fichier_compresse, rapport_compression);
    }
    else if(nb_octet_non_compresse >= pow(10,3) && nb_octet_non_compresse < 10*pow(10,6)){
        rapport_compression = ((float)nb_octet_fichier_compresse / (float)nb_octet_non_compresse)*100;
        printf("%.1f ko compresse en %.1f ko (%.1f%%)\n",((float)nb_octet_non_compresse/pow(10,3)),((float)nb_octet_fichier_compresse/pow(10,3)), rapport_compression);
    }
    else if(nb_octet_non_compresse >= pow(10,6) && nb_octet_non_compresse < pow(10,9)){
        rapport_compression = ((float)nb_octet_fichier_compresse / (float)nb_octet_non_compresse)*100;
        printf("%.2f Mo compresse en %.2f Mo (%.1f%%)\n",((float)nb_octet_non_compresse/pow(10,6)),((float)nb_octet_fichier_compresse/pow(10,6)), rapport_compression);
    }
    else if(nb_octet_non_compresse >= pow(10,9) && nb_octet_non_compresse < pow(10,12)){
        rapport_compression = ((float)nb_octet_fichier_compresse / (float)nb_octet_non_compresse)*100;
        printf("%.3f Go compresse en %.3f Go (%.1f%%)\n",((float)nb_octet_non_compresse/pow(10,9)),((float)nb_octet_fichier_compresse/pow(10,9)), rapport_compression);
    }
    else if(nb_octet_non_compresse >= pow(10,12)){
        rapport_compression = ((float)nb_octet_fichier_compresse / (float)nb_octet_non_compresse)*100;
        printf("%.3f To compresse en %.3f To (%.1f%%)\n",((float)nb_octet_non_compresse/pow(10,12)),((float)nb_octet_fichier_compresse/pow(10,12)), rapport_compression);
    }

}


// Fonction permettant de lire l'en tete du fichier compresse et ainsi de reconstituer l'histogramme

void lireEnTete(unsigned int histogramme[], char nomFichierADecompresse[], unsigned int *nb_octet, unsigned int *taille_en_tete){
    FILE* fichierADecompresse;
    unsigned char octet = 0;
    unsigned int somme_occurrence = 0;

    fichierADecompresse = fopen(nomFichierADecompresse, "rb");
    if(fichierADecompresse == NULL){
        perror("Erreur dans l'ouverture du fichier à compresser");
        exit(EXIT_FAILURE);
    }

    fread(nb_octet, sizeof(unsigned int), 1, fichierADecompresse);
    *taille_en_tete =  *taille_en_tete + sizeof(unsigned int);

    do{  
        fread(&octet, sizeof(unsigned char), 1, fichierADecompresse);
        fread(&histogramme[octet], sizeof(unsigned int), 1, fichierADecompresse);
        somme_occurrence += histogramme[octet];
        *taille_en_tete = *taille_en_tete + sizeof(unsigned char) + sizeof(unsigned int);
    }while(somme_occurrence != *nb_octet);

    fclose(fichierADecompresse);
}


/*  Fonction permettant de rechercher un octet recuperer dans le fichier comp resse et de retrouver le ou les carcatere(s) 
    correspondnat grace à l'arbre de huffman */

PArbre rechercherOctet(PArbre arbreHuffman, int niveau_arbre, unsigned int carac_code, int nb_bits_total){
    if(niveau_arbre > nb_bits_total){
        return NULL;
    }
    if(estVideArb(arbreHuffman->fg) && estVideArb(arbreHuffman->fd)){
        return arbreHuffman;
    }
    if(!estVideArb(arbreHuffman->fg) && !((carac_code >> (nb_bits_total - niveau_arbre-1)) & 1)){
        return rechercherOctet(arbreHuffman->fg, niveau_arbre+1, carac_code, nb_bits_total);
    }
    if(!estVideArb(arbreHuffman->fd) && ((carac_code >> (nb_bits_total - niveau_arbre-1)) & 1)){
        return rechercherOctet(arbreHuffman->fd, niveau_arbre+1, carac_code, nb_bits_total);
    }

    return NULL;
}


// Fonction creant le fichier decompresse grace à l'arbre de huffman

void decompresser(PArbre arbreHuffman, unsigned int taille_en_tete, unsigned int nb_octet, char nomFichierADecompresser[], char nomFichierDecompresse[]){
    FILE* fichierADecompresser;
    FILE* fichierDecompresse;
    PArbre octetArbre;
    unsigned char octet = 0;
    unsigned int char_tmp = 0;
    int nb_bits_total = 0;

    fichierADecompresser = fopen(nomFichierADecompresser, "r");
    if(fichierADecompresser == NULL){
        perror("Erreur dans l'ouverture du fichier à decompresser");
        exit(EXIT_FAILURE);
    }
    fichierDecompresse = fopen(nomFichierDecompresse, "wb");
    if(fichierDecompresse == NULL){
        perror("Erreur dans l'ouverture du fichier decompresse");
        exit(EXIT_FAILURE);
    }
    fseek(fichierADecompresser, taille_en_tete, SEEK_SET);

    // Boucle récuperant chaque octet du fichier compresse pour rechercher dans l'arbre leur correspondance
    while((octet = fgetc(fichierADecompresser)) != EOF && nb_octet != 0){
        char_tmp = char_tmp << 8;
        char_tmp = char_tmp | octet;
        nb_bits_total += 8;


        while((octetArbre = rechercherOctet(arbreHuffman, 0, char_tmp, nb_bits_total)) && nb_octet != 0){
            nb_octet--;
            fputc(octetArbre->elmt.caractere, fichierDecompresse);
            nb_bits_total -= octetArbre->nb_bits;
        }
    }
    fclose(fichierADecompresser);
    fclose(fichierDecompresse);
}


// Fonction de decompression qui appelle les autres fonctions

void decompression(char nomFichierADecompresser[], char nomFichierDecompresse[]){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    PlisteA histogrammeA;
    PArbre arbreHuffman;
    unsigned int histogrammeTab[256] = {0};
    unsigned int nb_octet = 0;
    unsigned int taille_en_tete = 0;


    lireEnTete(histogrammeTab, nomFichierADecompresser, &nb_octet, &taille_en_tete);

    histogrammeA = tabToListeA(histogrammeTab);
    arbreHuffman = creationArbre(histogrammeA);
    arbreNbBits(arbreHuffman, 0);

    decompresser(arbreHuffman, taille_en_tete, nb_octet, nomFichierADecompresser, nomFichierDecompresse);

    gettimeofday(&end, NULL);
    double temps_compression = (double)((end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec)/1000000; 
    printf("\nDone (%.3f sec)\n", temps_compression);
}


// Fonction d'aide qui affiche une aide apres avoir utilise l'option -h

void help(){
    printf(" Huffman permet de compresser ou de decompresser un fichier.\n");
    printf(" LA commande : ./[chemin]/huffman [option] [fichierA] [fichierB].\n");
    printf(" Chemin         Repertoire de depart.\n");
    printf(" Options        -c, -d ou -h\n");
    printf(" -c             Demarrer l'application pour effectuer une compression du fichier A vers le fichier B.\n");
    printf(" -d             Demarrer l'application pour effectuer une decompression du fichier A vers le fichier B.\n");
    printf(" fichierA       Choix du fichier à lire.\n");
    printf(" fichierB       Choix du fichier ou ecrire.\n");
}