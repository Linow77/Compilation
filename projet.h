#include <stdio.h>

/* STRUCTURES */
// Automate Fini Non Deterministe
typedef struct
   {
   	unsigned int * Q		//Q : ensemble fini d'états
   	char * Z				//Z : Alphabet de l'automate
   	unsigned int s			//s : etat initial
   	int * F					//F : ensemble d'états accepteurs
   	int* Delta				//D : ensemble des transitions 1 tableau 1D, chaque case représente : etat1/lettre/etat2
   } AUTOMATEAFN;

// Automate Fini Deterministe a revoir
typedef struct
   {
   	unsigned int * Q		//Q : ensemble fini d'états
   	char * Z				//Z : Alphabet de l'automate
   	unsigned int s			//s : etat initial
   	int * F					//F : ensemble d'états accepteurs
   	int* S					//S : ensemble des transitions 1 tableau 1D, chaque case représente : etat1/lettre/etat2
   } AUTOMATEAFD;
