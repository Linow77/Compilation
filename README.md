# Compilation
Création d'un Analyseur en C

Structure 'etat' ou 'sommet'
unsigned int numéro_état
char* liste_de_voisins
char caractère_de_transition
BOOLEAN accepteur
char* action //que pour les états accepteurs

enum BOOLEAN{
	TRUE, FALSE
};
typedef enum BOOLEAN BOOLEAN;

/*
Un automate standard est un tableau de ETAT
Un automate qui reconnait le langage vide : reconnaitre le EOF (le code un dans fichier texte) ?
Un automate qui reconnait le langage composé du mot vide: reconnaitre rien : ''
Un automate qui reconnait un langagne composé d'un mot d'un caractère: 2 états: 1 entrée et l'autre accepteur en lisant le caractère

La taille des automates suite à l'union ou la concaténation est de T[1] + T[2] -1
Question:
- Pour l'état de d'entrée, peut-on choisir un char pour le differencier des autres, ou doit-ont utiliser un autre 
moyens afin de pouvoir reonnaitre tous les caractères présent dans le type char (par exemple: le mettre à NULL)?
- Pour reconnaitre le langage vide, revient à reconnaitre la fin du fichier dans lequel se trouve le code à compiler ?
- Pour reconnaitre le langage composé du seul mot vide, revient à reconnaitre ""?


Remarque:
utiliser des unsigned char au lieu de unsigned int si on a pas trop d'états ?
Langage reconnaisant le langage vide -> Aucun états accepteurs (F vide)
Langage reconnaissant le mot vide -> 1 état accepteur en lisant epsilon (F à un élémént) et aucunes transitions
*/
