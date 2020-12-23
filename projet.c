#include "projet.h"


int main() {

	AUTOMATEAFN afnVide,afnMotVide,afncaractere, afnUnion;

	afnVide = langageVide();
	printf("/**** Langage Vide ****/\n");
	AfficherAutomate(afnVide);

	afnMotVide = langageMotVide();
	printf("\n/**** Langage Mot Vide ****/\n");
	AfficherAutomate(afnMotVide);

	afncaractere = langagecaractere('a');
	printf("\n/**** Langage Caractere ****/\n");
	AfficherAutomate(afncaractere);

	afnUnion = unionDeDeuxAutomates(afnMotVide, afncaractere);
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
	AUTOMATEAFN afn;
	//Check si l'un des états initials de afn1/afn2 est accepteur
	//Création d'un état initial commun qui va vers les arrivées des états initiaux de afn1 et afn2

	//tailleQ = tailleQ1 -1 + tailleQ2 -1 + 1
	afn.tailleQ = afn1.tailleQ + afn2.tailleQ - 1;
	printf("tailleQ1:%d tailleQ2:%d tailleQ:%d\n",afn1.tailleQ, afn2.tailleQ, afn.tailleQ );
	
	afn1.Q = (unsigned int *) realloc( afn1.Q, afn.tailleQ * sizeof(unsigned int) );


	return afn;
}
