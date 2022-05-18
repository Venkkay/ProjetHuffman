#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type_variables.h"
#include "fonctions_generales.h"


// Test si un arbre est vide
int estVideArb(PArbre a) {
    return (a == ARBRENULL);
}


// Test si une liste d'arbre est vide
int estVideLstA(PlisteA lst){
    return (lst == NULL);
}


// Test si l'arbre entre est une feuille de l'arbre
int estFeuille(PArbre a) {
    return !(a->fg || a->fd);
}


// Permet de creer un noeud d'arbre pour un element/caractere
PArbre creerArbreElement(Element elmt){
    PArbre arbre;
    if ((arbre = MALLOC(Arbre)) == NULL) {
        fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
        exit(1);
    }
    arbre->elmt = elmt;
    arbre->somme_occurrence = elmt.occurrence;
    arbre->fg = NULL;
    arbre->fd = NULL;
    arbre->nb_bits = 0;
    return arbre;
}


// Permet d'ajouter a la liste d'arbre un noeud pour un element/carcatere
PlisteA consAElement(Element elmt, PlisteA list){
	PlisteA noeud = NULL;
	if ((noeud = MALLOC(ListeA)) == NULL) {
		fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
		exit(1);
	}
	noeud->valA = creerArbreElement(elmt);
	noeud->suivA = list;
	return noeud;
}


// Permet de creer un noeud d'arbre pour une somme des occurences des ses descendants
PArbre creerArbreSomme(int somme, PArbre fg, PArbre fd){
    PArbre arbre;
    if ((arbre = MALLOC(Arbre)) == NULL) {
        fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
        exit(1);
    }
    arbre->elmt.caractere = 0;
    arbre->elmt.occurrence = 0;
    arbre->somme_occurrence = somme;
    arbre->fg = fg;
    arbre->fd = fd;
    arbre->nb_bits = 0;
    return arbre;
}


// Permet d'ajouter a la liste d'arbre un noeud pour une somme
PlisteA consASomme(PArbre NoeudSomme, PlisteA list){
	PlisteA noeud = NULL;
	if ((noeud = MALLOC(ListeA)) == NULL) {
		fprintf(stderr, "ERREUR ALLOCATION MEMOIRE");
		exit(1);
	}
	noeud->valA = NoeudSomme;
	noeud->suivA = list;
	return noeud;
}


// Permet de supprimer un arbre
PArbre supprimerArbre(PArbre arbre){
    if(!estVideArb(arbre)){
        supprimerArbre(arbre->fg);
        supprimerArbre(arbre->fd);
        free(arbre);
    }
    else{
        return ARBRENULL;
    }
    return 0;
}


// Permet de suppimer le premier noeud d'une liste d'arbres
PlisteA supprimerNoeudListA(PlisteA list){
    PlisteA tmp = list;
    list = list->suivA;
    //supprimerArbre(tmp->valA);
    free(tmp);
    return list;
}
