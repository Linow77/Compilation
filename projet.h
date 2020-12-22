#include <stdio.h> //printf
#include <stdlib.h> //malloc
#include <string.h> //strcmp strlen

/* STRUCTURES */
// Automate Fini Non Deterministe
typedef struct
   {
   	unsigned int * Q;		//Q : ensemble fini d'Ã©tats
   	char * Z;				//Z : Alphabet de l'automate
   	unsigned int s;			//s : etat initial
   	int * F;				//F : ensemble d'Ã©tats accepteurs
   	char ** D;				//D : ensemble des transitions 1 tableau 1D, chaque case reprÃ©sente : etat1/lettre/etat2
   } AUTOMATEAFN;

// Automate Fini Deterministe a revoir
typedef struct
   {
   	unsigned int * Q;		//Q : ensemble fini d'Ã©tats
   	char * Z;				//Z : Alphabet de l'automate
   	unsigned int s;			//s : etat initial
   	int * F;				//F : ensemble d'Ã©tats accepteurs
   	char ** S;				//S : ensemble des transitions 1 tableau 1D, chaque case reprÃ©sente : etat1/lettre/etat2
   } AUTOMATEAFD;

/* Prototypes */
void AfficherAutomate(AUTOMATEAFN afn); //permet d'afficher le contenu d'un automate standard
AUTOMATEAFN langageVide(); //renvoie un automate standard reconnaissant le langage vide
AUTOMATEAFN langageMotVide(); //renvoie un automate standard reconnaissant le langage composÃ© du seul mot vide
AUTOMATEAFN langagecaractere(char caractere);
