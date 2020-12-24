#include <stdio.h> //printf
#include <stdlib.h> //malloc
#include <string.h> //strcmp strlen

/* STRUCTURES */
// Automate Fini Non Deterministe
typedef struct
   {
   	unsigned int * Q;		//Q : ensemble fini d'etats
      unsigned int tailleQ;
   	char * Z;				//Z : Alphabet de l'automate
   	unsigned int s;			//s : etat initial
   	int * F;				//F : ensemble d'etats accepteurs
      int tailleF;
   	char ** D;	
      int tailleD;			//D : ensemble des transitions 1 tableau 1D, chaque case reprÃ©sente : etat1/lettre/etat2
   } AUTOMATEAFN;

// Automate Fini Deterministe a revoir
typedef struct
   {
   	unsigned int * Q;		//Q : ensemble fini d'etats
   	char * Z;				//Z : Alphabet de l'automate
   	unsigned int s;			//s : etat initial
   	int * F;				//F : ensemble d'etats accepteurs
   	char ** S;				//S : ensemble des transitions 1 tableau 1D, chaque case reprÃ©sente : etat1/lettre/etat2
   } AUTOMATEAFD;

/* Macro */
#define TAILLE_CHAINE_TRANSITION 4

/* Prototypes */
void AfficherAutomate(AUTOMATEAFN afn); //permet d'afficher le contenu d'un automate standard
AUTOMATEAFN langageVide(); //renvoie un automate standard reconnaissant le langage vide
AUTOMATEAFN langageMotVide(); //renvoie un automate standard reconnaissant le langage composÃ© du seul mot vide
AUTOMATEAFN langagecaractere(char caractere);
AUTOMATEAFN unionDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2);
int afn_identique(AUTOMATEAFN afn1, AUTOMATEAFN anf2);
