#include "fonctionsAFN.h"
#include "fonctionsAFNevoluees.h"
#include "fonctionsAFD.h"

int main() {

	AUTOMATEAFN afnVide,afnMotVide,afncaractere1,afncaractere2, afnUnion, afnConcatene,afnKleene;
	AUTOMATEAFD afd;
	char* mot;
	unsigned int motReconnu;

	//Creer l'AFN qui reconnait le langageVide
	afnVide = langageVide();
	printf("/**** Langage Vide ****/\n");
	AfficherAutomateNonDeterministe(afnVide);

	//Creer l'AFN qui reconnait le langage compose du seul mot vide
	afnMotVide = langageMotVide();
	printf("\n/**** Langage Mot Vide ****/\n");
	AfficherAutomateNonDeterministe(afnMotVide);

	//Creer l'AFN qui reconnait le langage compose d'un mot d'un caractere passe en parametre (ici a)
	afncaractere1 = langagecaractere('a');
	printf("\n/**** Langage Caractere ****/\n");
	AfficherAutomateNonDeterministe(afncaractere1);

	//Creer l'AFN qui reconnait le langage compose d'un mot d'un caractere passe en parametre (ici b)
	afncaractere2 = langagecaractere('b');
	printf("\n/**** Langage Caractere ****/\n");
	AfficherAutomateNonDeterministe(afncaractere2);

	//Creer l'AFN qui reconnait la reunion des deux langages donnes en parametre (ici a et b)
	afnUnion = unionDeDeuxAutomates(afncaractere1,afncaractere2);
	printf("\n/**** Langage Union ****/\n");
	AfficherAutomateNonDeterministe(afnUnion);

	//Creer l'AFN qui reconnait la fermeture iterative de Kleene du langage donne en parametre (ici a)
	afnKleene = kleene(afncaractere1);
	printf("\n/**** Version Kleene sur langagecaractere1 ****/\n");
	AfficherAutomateNonDeterministe(afnKleene);

	//Creer l'AFN qui reconnait la concatenation des deux langages donnes en parametre (ici a et a)
	afnConcatene = concatenationDeDeuxAutomates(afncaractere1,afncaractere1);
	printf("\n/**** Langage Concatené ****/\n");
	AfficherAutomateNonDeterministe(afnConcatene);

	/** Exemple pour la minimisation **/
	afnKleene = kleene(afnConcatene);
	printf("\n/**** Version Kleene sur langagecaractere1 ****/\n");
	AfficherAutomateNonDeterministe(afnKleene);

	//Creer l'AFD qui reconnait le meme langage que l'AFN donne en parametre
	afd = determinisation(afnKleene);
	printf("\n/**** AFD ****/\n");
	AfficherAutomateDeterministe(afd);

	//Creer l'AFD minimal qui reconnait le meme langage que l'AFD donne en parametre
	afd = minimisation(afd);
	printf("\n/**** AFD Minimise****/\n");
	AfficherAutomateDeterministe(afd);
	
 	//Permet de tester si la chaine mot est reconnu par l'afd donne en parametre
	mot = "aaa";
	motReconnu = verifMot(afd,mot);
    if(motReconnu == 1)
    {
        printf("\n/**Le mot:%s est reconnu par l'automate**/\n",mot);
    }
    else
    {
        printf("\n/**Le mot:%s n'est pas reconnu par l'automate**/\n",mot);
    }

    

	/** FREE DES AUTOMATES **/
	free_afn(afnVide);
	free_afn(afnMotVide);
	free_afn(afncaractere1);
	free_afn(afncaractere2);
	free_afn(afnUnion);
	free_afn(afnConcatene);
	free_afn(afnKleene);
	free_afd(afd);

	return 1;

}
