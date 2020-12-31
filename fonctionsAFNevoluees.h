#ifndef FONCTION_AFNEVOLUUES_H
#define FONCTION_AFNEVOLUUES_H

#include "fonctionsAFN.h"

/**AUTOMATE FINI NON DETERMINISTES PLUS EVOLUES **/
AUTOMATEAFN unionDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2);//renvoie un automate standard reconnaissant la reunion de deux automates passes en parametre
AUTOMATEAFN concatenationDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2);//renvoie un automate standard reconnaissant la concatenation de deux automates passes en parametre
AUTOMATEAFN kleene(AUTOMATEAFN afn);//renvoie un automate standard reconnaissant la fermeture iterative de kleene de l'automate passé en paramètre


/** Fonctions annexe **/
void AfficherAutomateNonDeterministe(AUTOMATEAFN afn); //permet d'afficher le contenu d'un automate standard non deterministe
int afn_identique(AUTOMATEAFN afn1, AUTOMATEAFN anf2); //permet de verifier si deux automates standard non deterministes sont identiques
void free_afn(AUTOMATEAFN afn); //Libère le contenu d'un automate standard non deterministe
AUTOMATEAFN copie(AUTOMATEAFN afn); //renvoie la copie d'un automate fini non deterministe donne en parametre
#endif