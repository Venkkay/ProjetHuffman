#ifndef FONCTIONS_GENERALES_H
#define FONCTIONS_GENERALES_H


// Prototypes des fonctions du fichier "fonctions_generales.c"

int estVideArb(PArbre a);
int estVideLstA(PlisteA lst);
int estFeuille(PArbre a);
PArbre creerArbreElement(Element elmt);
PlisteA consAElement(Element elmt, PlisteA list);
PArbre creerArbreSomme(int somme, PArbre fg, PArbre fd);
PlisteA consASomme(PArbre NoeudSomme, PlisteA list);
PArbre supprimerArbre(PArbre arbre);
PlisteA supprimerNoeudListA(PlisteA list);


#endif