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
   	char ** D;	
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
   	int tailleDelta;			
    TRANSITION* Delta;		//S : Fonction de transition		
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


/** Fonctions annexe **/
void AfficherAutomate(AUTOMATEAFN afn); //permet d'afficher le contenu d'un automate standard
int afn_identique(AUTOMATEAFN afn1, AUTOMATEAFN anf2); //permet de verifier si deux automates sont identiques
void free_afn(AUTOMATEAFN afn); //Libère les tableaux d'un AUTOMATEAFN

