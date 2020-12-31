#ifndef FONCTION_AFD_H
#define FONCTION_AFD_H
#include "fonctionsAFN.h"

/* STRUCTURES */
// Automate Fini Deterministe 
typedef struct
{
   	unsigned int * Q;		//Q : ensemble fini d'etats
    unsigned int tailleQ;
   	char * Z;				//Z : Alphabet de l'automate
   	unsigned int s;			//s : etat initial
   	int * F;				//F : ensemble d'etats accepteurs
    int tailleF;			
    TRANSITION* Delta;		//S : Fonction de transition	
    int tailleDelta;	
} AUTOMATEAFD;

typedef struct 
{
	unsigned int depart;
	char caractere;
	unsigned int tailleArrivee;
	int* arrivee;

} TRANSITIONDETERMINISTE; 

typedef struct 
{
	unsigned int * colonne;
	unsigned int tailleColonne;

} TABETATS; 

/**AUTOMATE FINI DETERMINISTES **/
AUTOMATEAFD determinisation(AUTOMATEAFN afn); //prend un automate fini non déterministe et renvoi un automate fini déterministe composé de ce dernier
unsigned int verifMot(AUTOMATEAFD afd, char* mot); //Renvoie 1 si l'afd peut lire le mot, 0 sinon
AUTOMATEAFD minimisation(AUTOMATEAFD afd); //prend un automate fini determinisite et renvoi un automate fini deterministe minimisé

/** Fonctions Annexes **/
void AfficherAutomateDeterministe(AUTOMATEAFD afn); //permet d'afficher le contenu d'un automate standard deterministe
void tri(int* T, int ligne); //Fonction de tri rapide
void affiche_determinisation(TABETATS * etats, int tailleEtat); //affiche le tableau des etats suite a la determinisation
void affiche_transitions(TRANSITIONDETERMINISTE** transitions,int tailleEtat, int tailleZ); //afficher le tableau transitions
int verif_ajout_non_present(int* arrivee, int tailleArrivee, int ajout); //verifie si la valeur ajout est presente dans le tableau arrivee
AUTOMATEAFD copie_afd(AUTOMATEAFD afd); //Copie d'un automate fini deterministe
void free_afd(AUTOMATEAFD afd); //Libere le contenu d'un afd

#endif