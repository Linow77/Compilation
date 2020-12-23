#include "projet.h"


int main() {

	AUTOMATEAFN afnVide,afnMotVide,afncaractere1,afncaractere2, afnUnion;

	afnVide = langageVide();
	printf("/**** Langage Vide ****/\n");
	AfficherAutomate(afnVide);

	afnMotVide = langageMotVide();
	printf("\n/**** Langage Mot Vide ****/\n");
	AfficherAutomate(afnMotVide);

	afncaractere1 = langagecaractere('a');
	printf("\n/**** Langage Caractere ****/\n");
	AfficherAutomate(afncaractere1);

	afncaractere2 = langagecaractere('b');
	printf("\n/**** Langage Caractere ****/\n");
	AfficherAutomate(afncaractere2);

	afnUnion = unionDeDeuxAutomates(afncaractere1, afncaractere2);
	printf("\n/**** Langage Union ****/\n");
	AfficherAutomate(afnUnion);
	return 1;

}

void AfficherAutomate(AUTOMATEAFN afn){
	int i=0;
	int tailleZ;

	//Q
	//printf("taille de Q: %d\n",tailleQ);
	if(afn.Q!=NULL){
		printf("Ensemble d'etats (Q): ");
		for(i=0;i<afn.tailleQ;i++){
			printf("%u, ",afn.Q[i]);
		}	
		printf("\n");
	}

	//Z

	if(afn.Z == NULL){
		printf("Alphabet de l'automate (Z): vide\n");
	} else if(!strcmp(afn.Z,"")){
		printf("Alphabet de l'automate (Z): mot vide\n");
	}else{
		tailleZ = strlen(afn.Z);
		printf("Alphabet de l'automate (Z): ");
		for(i=0;i<tailleZ;i++){
			printf("%c, ",afn.Z[i]);
		}
		printf("\n");
	}
	

	//s
	printf("Etat initial (s): %u\n",afn.s);

	//F
	//printf("taille de F: %d\n",tailleF);
	if(afn.F!=NULL){
		printf("Ensemble d'etats accepteurs (F): ");
		for(i=0;i<afn.tailleF;i++){
			printf("%d, ",afn.F[i]);
		}
		printf("\n");
	}else{
		printf("Aucun etat accepteur\n");
	}

	//D
	//printf("taille de D: %d\n",tailleD);
	if(afn.D!=NULL){
		printf("Ensemble des etats transitions (D): ");
		for(i=0;i<afn.tailleD;i++){
			printf("%s, ",afn.D[i]);
		}
		printf("\n");	
	}else{
		printf("Aucune transition\n");
	}
}

/**AUTOMATE FINI NON DETERMINISTES **/
AUTOMATEAFN langageVide(){
	AUTOMATEAFN afn;

	afn.Q = (unsigned int*)malloc(sizeof(unsigned int));
	afn.Q[0] = 0;
	afn.tailleQ = 1;

	afn.Z = NULL;

	afn.s = 0;

	afn.F = NULL;
	afn.tailleF = 0;

	afn.D = NULL;
	afn.tailleD = 0;

	return afn;
}

AUTOMATEAFN langageMotVide(){
	AUTOMATEAFN afn;
	afn.Q = (unsigned int*)malloc(sizeof(unsigned int));
	afn.Q[0] = 0;
	afn.tailleQ = 1;
	afn.Z = "";

	afn.s = 0;

	afn.F = (int*)malloc(sizeof(int));
	afn.F[0] = 0;
	afn.tailleF = 1;

	afn.D = NULL;
	afn.tailleD = 0;

	return afn;
}

AUTOMATEAFN langagecaractere(char caractere){
	AUTOMATEAFN afn;
	afn.tailleQ = 2;
	afn.Q = (unsigned int*)malloc(sizeof(unsigned int)*afn.tailleQ);
	afn.Q[0] = 0;
	afn.Q[1] = 1;
	

	afn.Z = (char*)malloc(sizeof(char));	// allouer que la taille d'un char ?
	afn.Z[0] = caractere;
	afn.Z[1] = '\0';

	afn.s = 0;

	afn.F = (int*)malloc(sizeof(int));
	afn.F[0] = 1;
	afn.tailleF = 1;

	afn.D = (char**)malloc(sizeof(char*));
	afn.D[0] = (char*)malloc(sizeof(char)); // allouer que la taille d'un char ?
	//creation de la chaine "etat1-caractere-etat2"
	char * chaine =NULL;
	chaine = (char*)malloc(sizeof(char)*TAILLE_CHAINE_TRANSITION);
	chaine[0] = '0';
	chaine[1] = caractere;
	chaine[2] = '1';
	chaine[3] = '\0';
	afn.D[0]=chaine;
	afn.tailleD = 1;

	return afn;
}

/**AUTOMATE FINI NON DETERMINISTES PLUS EVOLUES **/

AUTOMATEAFN unionDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2){
	//Verifier que les deux afn ne sont pas identique sinon directement renvoyer l'un des deux
	
	//Check si l'un des états initials de afn1/afn2 est accepteur
	//Création d'un état initial commun qui va vers les arrivées des états initiaux de afn1 et afn2

	//tailleQ = tailleQ1 -1 + tailleQ2 -1 + 1
	int nouvelleTailleQ = afn1.tailleQ + afn2.tailleQ - 1;

	//réallocation de Q
	afn1.Q = (unsigned int *) realloc( afn1.Q, nouvelleTailleQ * sizeof(unsigned int) );

	//ajout des états de afn2 dans afn1.Q
	for(int i=1; i<afn2.tailleQ; i++){ //on ne récupère pas l'état initial de afn2, on commence donc a 1
		printf("afn1:%d=%d et afn2:%d=%d\n", afn1.tailleQ-1+i,afn1.Q[afn1.tailleQ-1+i],i,afn2.Q[i]);
		afn1.Q[afn1.tailleQ-1+i] = afn2.Q[i]+afn1.tailleQ-1; //on démarre le compte des etats de afn2 après ceux de afn1
	}

	//On enregistre la nouvelle taille de afn1
	afn1.tailleQ = nouvelleTailleQ;

	return afn1;
}
