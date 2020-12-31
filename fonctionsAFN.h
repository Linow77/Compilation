#ifndef FONCTION_AFN_H
#define FONCTION_AFN_H

#include <stdio.h> //printf
#include <stdlib.h> //malloc
#include <string.h> //strcmp strlen

/* STRUCTURES */
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
    int tailleD;			//D : ensemble des transitions 1 tableau 1D de TRANSITION

} AUTOMATEAFN; 

/* Prototypes */
/**AUTOMATE FINI NON DETERMINISTES **/
AUTOMATEAFN langageVide(); //renvoie un automate standard reconnaissant le langage vide
AUTOMATEAFN langageMotVide(); //renvoie un automate standard reconnaissant le langage composant du seul mot vide
AUTOMATEAFN langagecaractere(char caractere); //renvoi un automate standard reconnaissant le langage du caractere donne en entree



#endif