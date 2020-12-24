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

	afnUnion = unionDeDeuxAutomates(afncaractere1, afnMotVide);
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
	afn.Z = ""; //Avec strlen le mot vide ne s'affiche pas dans le langage après une union

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
	afn.tailleQ = 3;
	afn.Q = (unsigned int*)malloc(sizeof(unsigned int)*afn.tailleQ);
	afn.Q[0] = 0;
	afn.Q[1] = 1;
	afn.Q[2] = 2;//temporaire pour les tests
	

	afn.Z = (char*)malloc(sizeof(char));	// allouer que la taille d'un char ?
	afn.Z[0] = caractere;
	afn.Z[1] = 'o'; //temporaire pour les tests
	afn.Z[2] = 'p'; //temporaire pour les tests
	afn.Z[3] = '\0';

	afn.s = 0;

	afn.tailleF = 2;
	afn.F = (int*)malloc(sizeof(int)*afn.tailleF);
	afn.F[0] = 1;
	afn.F[1] = 2;//temporaire pour les tests
	//afn.F[2] = 0;//temporaire pour les tests

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
	int verifCaractere=0,etatInitialAccepteur=0;
	int nouvelleTailleQ,nouvelleTailleF;
	int i,j;

	//Allocation de Ztemp au maximum (afn1.Z + afn2.Z)
	char* Ztemp = (char*)malloc(sizeof(char)*(strlen(afn1.Z)+strlen(afn2.Z)));
	strcpy(Ztemp,afn1.Z); //copie de afn1.Z dans Ztemp

	//Verifier que les deux afn ne sont pas identique sinon directement renvoyer l'un des deux
	if(afn_identique(afn1,afn2)){
		return afn1;
	}

	//Verif si l'etat initial de afn1 et afn2 sont accepteur
	//on vérfie tout F car l'état 0 n'est pas forcément le premier accepteur
	for(i=0;i<afn1.tailleF;i++){ 
		if(afn1.F[i]==0){
			etatInitialAccepteur=1; // afn1 a son etat initial accepteur
		}
	}
	//Verif si l'etat initialial de afn1 n'est pas déja accepteur
	//on vérfie tout F car l'état 0 n'est pas forcément le premier accepteur
	if(!etatInitialAccepteur){
		for(i=0;i<afn2.tailleF;i++){ 
			if(afn2.F[i]==0){
				etatInitialAccepteur=2;// afn2 a son etat initial accepteur
			}
		}
	}else{
		for(i=0;i<afn2.tailleF;i++){ 
			if(afn2.F[i]==0){
				etatInitialAccepteur=3;// les deux afn ont leurs etats initiaux accepteurs
			}
		}
	}
	//printf("etatInitialAccepteur:%d",etatInitialAccepteur);
	/*Modification de afn1 pour y ajouter le langage reconnu par afn2*/

	//Création de Q
	//tailleQ = tailleQ1 -1 + tailleQ2 -1 + 1
	nouvelleTailleQ = afn1.tailleQ + afn2.tailleQ - 1;

	//réallocation de Q
	afn1.Q = (unsigned int *) realloc( afn1.Q, nouvelleTailleQ * sizeof(unsigned int) );

	//ajout des états de afn2 dans afn1.Q
	for(i=0; i<afn2.tailleQ; i++){ //on ne récupère pas l'état initial de afn2
		if(afn2.Q[i]!=0){
			afn1.Q[afn1.tailleQ-1+i] = afn2.Q[i]+afn1.tailleQ-1; //on démarre le compte des etats de afn2 après ceux de afn1
		}
		
	}

	//Creation de Z
	//Determinisation des nouveaux caracteres
	for(i=0;i<strlen(afn2.Z);i++){		//Pour chaque caractere de afn2
		verifCaractere=0; //on a pas trouvé le caractère
		for(j=0;j<strlen(afn1.Z);j++){	//Est il deja present dans afn1
			if(!verifCaractere){ //si on a trouvé le caractère pas besoin de continuer de le chercher
				//printf("afn1:%d et afn2:%d\n",j,i );
				if(afn2.Z[i]==afn1.Z[j]){ 	//Si oui on passe au prochain caractere de afn2
					//printf("meme caractere :%c et %c\n",afn2.Z[i],afn1.Z[j] );
					verifCaractere=1;
					
					
				}else{ //Sinon on regarde s'il est présent dans un autre
					//printf("caractere different %c et %c\n",afn2.Z[i],afn1.Z[j] );
				}
			}
		}

		if(!verifCaractere){ //si on a pas trouvé le caractere dans tout l'aphabet de afn1 alors on l'ajoute
				//printf("ajout de %c\n", afn2.Z[i]);
				Ztemp[strlen(afn1.Z)] = afn2.Z[i];

		}
	}
	//printf("Z:%s\n",Ztemp );

	//On modifie afn1.Z par Ztemp
	//reallocation de afn1.Z
	afn1.Z = (char*) realloc(afn1.Z,strlen(Ztemp)*sizeof(char));
	strcpy(afn1.Z,Ztemp);

	//s n'est pas modifié il reste 0

	//Creation de F
	//on realloue la afn1.tailleF  + afn2.tailleF -1(si l'état initial de afn2 est accepteur et celui de afn1 aussi)
	nouvelleTailleF = afn1.tailleF + afn2.tailleF;
	if(etatInitialAccepteur==3){
		nouvelleTailleF--;
	}
	//printf("taille:%d\n",nouvelleTailleF );
	afn1.F = (int*) realloc(afn1.F,nouvelleTailleF*sizeof(int));

	//ajout des F de afn2 et l'etat initial si accepteur
	for(i=afn1.tailleF; i<nouvelleTailleF;i++){
		//on ajoute l'etat initial seulement si l'etat initial de afn2 est accepteur et pas celui de afn1
		if(etatInitialAccepteur==2 && afn2.F[i]==0){
			afn1.F[afn1.tailleF-1+i]=afn2.F[i];
		
		}else{//sinon on ajoute les etats de afn2 en démarrant le compte après ceux de afn1
			afn1.F[i]=afn2.F[i-afn1.tailleF]+afn1.tailleQ-1;
		}
		
	}

	//enregistre les nouvelle taille de Q et F
	afn1.tailleF=nouvelleTailleF;
	afn1.tailleQ = nouvelleTailleQ;

	return afn1;
}

int afn_identique(AUTOMATEAFN afn1, AUTOMATEAFN afn2){
	int i;

	//Verif de la taille de Q, Z, F,D et la valeur de s
	if(afn1.tailleQ != afn2.tailleQ || 
		strlen(afn1.Z)!=strlen(afn2.Z) ||
		afn1.s != afn2.s ||
		afn1.tailleF != afn2.tailleF ||
		afn1.tailleD != afn2.tailleD){
		return 0;
	}

	//Verif du contenu de Q
	for (i=0;i<afn1.tailleQ;i++){
		if (afn1.Q[i]!=afn2.Q[i]){
			return 0;
		}
	}

	//Verif du contenu de Z
	for (i=0;i<strlen(afn1.Z);i++){
		if(afn1.Z[i]!=afn2.Z[i]){
			return 0;
		}
	}

	//Verif du contenu de F
	for (i=0;i<afn1.tailleF;i++){
		if(afn1.Z[i]!=afn2.Z[i]){
			return 0;
		}
	}

	//Verif du contenu de D
	for (i=0;i<afn1.tailleD;i++){
		if(afn1.Z[i]!=afn2.Z[i]){
			return 0;
		}
	}
	
	return 1; //si tout est identique
	
}