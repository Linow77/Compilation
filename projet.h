#include <stdio.h> //printf
#include <stdlib.h> //malloc
#include <string.h> //strcmp strlen

/* STRUCTURES */

typedef struct 
{
	unsigned int * colonne;
	unsigned int tailleColonne;

} TABETATS; 

typedef struct 
{
	unsigned int depart;
	char caractere;
	unsigned int tailleArrivee;
	int* arrivee;

} TRANSITIONDETERMINISTE; 


//Structure d'une transition
typedef struct 
{
	unsigned int depart;
	char caractere;
	unsigned int arrivee;

} TRANSITION; 

// Automate Fini Non Deterministe
typedef struct
{
   	unsigned int * Q;		//Q : ensemble fini d'etats
    unsigned int tailleQ;
   	char * Z;				//Z : Alphabet de l'automate
   	unsigned int s;			//s : etat initial
   	int * F;				//F : ensemble d'etats accepteurs
    int tailleF;
    TRANSITION * D;
   	//char ** D;	
    int tailleD;			//D : ensemble des transitions 1 tableau 1D, chaque case represente : etat1/lettre/etat2
    //remplacer int par un struct avec 2 int et 1 char
} AUTOMATEAFN; 

// Automate Fini Deterministe a revoir
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

/* Macro */
#define NOMBRE_ELEMENT_TRANSITION 3

/* Prototypes */
/**AUTOMATE FINI NON DETERMINISTES **/
AUTOMATEAFN langageVide(); //renvoie un automate standard reconnaissant le langage vide
AUTOMATEAFN langageMotVide(); //renvoie un automate standard reconnaissant le langage composant du seul mot vide
AUTOMATEAFN langagecaractere(char caractere); //renvoi un automate standard reconnaissant le langage du caractere donne en entree

/**AUTOMATE FINI NON DETERMINISTES PLUS EVOLUES **/
AUTOMATEAFN unionDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2);//renvoie un automate standard faisant l'union de deux automates
AUTOMATEAFN concatenationDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2);//renvoie un automate standard faisant la concatenation de deux automates
AUTOMATEAFN kleene(AUTOMATEAFN afn);//renvoie la version de kleene de l'automate passé en paramètre

/**AUTOMATE FINI DETERMINISTES **/
AUTOMATEAFD determinisation(AUTOMATEAFN afn); //prend un automate fini non déterministe et renvoi un automate fini déterministe composé de ce dernier

/** Fonctions annexe **/
void AfficherAutomateNonDeterministe(AUTOMATEAFN afn); //permet d'afficher le contenu d'un automate standard non deterministe
int afn_identique(AUTOMATEAFN afn1, AUTOMATEAFN anf2); //permet de verifier si deux automates sont identiques
void free_afn(AUTOMATEAFN afn); //Libère les tableaux d'un AUTOMATEAFN
AUTOMATEAFN copie(AUTOMATEAFN afn); // renvoie la copie d'un automate fini non deterministe
void AfficherAutomateDeterministe(AUTOMATEAFD afn); //permet d'afficher le contenu d'un automate standard deterministe
void tri(int* T, int ligne); //Fonction de tri pas rapide
void affiche_determinisation(TABETATS * etats, int tailleEtat); //affiche le tableau des etats suite a la determinisation
void affiche_transitions(TRANSITIONDETERMINISTE** transitions,int tailleEtat, int tailleZ);
int verif_ajout_non_present(int* arrivee, int tailleArrivee, int ajout);


