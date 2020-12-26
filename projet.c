#include "projet.h"


int main() {

	AUTOMATEAFN afnVide,afnMotVide,afncaractere1,afncaractere2, afnUnion, afnConcatene;

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

	afnConcatene = concatenationDeDeuxAutomates(afncaractere1,afncaractere2);
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
	chaine = (char*)malloc(sizeof(char)); //alouer que la taille d'un char ?
	chaine[0] = '0';
	chaine[1] = '/';
	chaine[2] = caractere;
	chaine[3] = '/';
	chaine[4] = '1';
	chaine[5] = '\0';
	afn.D[0]=chaine;
	afn.tailleD = 1;

	return afn;
}

/**AUTOMATE FINI NON DETERMINISTES PLUS EVOLUES **/

AUTOMATEAFN unionDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2){
	int verifCaractere=0,etatInitialAccepteur=0;
	int nouvelleTailleQ,nouvelleTailleF, nouvelleTailleD;
	int etat;
	int i,j,k,y;
	char delim[] = "/";
	char ** transitionTmp;

	//Allocation de transitionTmp
	transitionTmp = (char**)malloc(sizeof(char*)*NOMBRE_ELEMENT_TRANSITION);
	for (i=0;i<NOMBRE_ELEMENT_TRANSITION;i++){
		transitionTmp[i] = (char*)malloc(sizeof(char)); //char ?
	}

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

	/* Creation de D*/
	//reallocation de D
	//taille de D = afn1.tailleD + afn2.tailleD
	nouvelleTailleD = afn1.tailleD + afn2.tailleD;
	afn1.D = (char**)realloc(afn1.D,sizeof(char*)*nouvelleTailleD);
	for(i=0;i<nouvelleTailleD;i++){
		afn1.D[i] = realloc(afn1.D[i],sizeof(char));
	}

	//On ajoute les etats de afn2 en les incrémentant sauf l'etat initial 0
	//printf("afn1:%d et afntot:%d\n",afn1.tailleD,nouvelleTailleD );
	for(i=afn1.tailleD;i<nouvelleTailleD;i++){

		//on sépare la chaine de transition en 3
		char *ptr = strtok(afn2.D[i-afn1.tailleD], delim);
		j=0; //index des caracteres dans afn1.D[i][j]


		for(k=0;k<NOMBRE_ELEMENT_TRANSITION;k++)
		{
			transitionTmp[k] = ptr;
			
			//traitement sur transitionTmp[k]
			if(k==1) //on ne modifie pas le caractere
			{
				afn1.D[i][j] = transitionTmp[k][0];
				j++;

			}else{ // traitement des etats
				//on ne change pas si c'est l'etat initial
				if(!strcmp(transitionTmp[k],"0")){ 
					afn1.D[i][j] = '0';
					j++;
				}else{ //traitement des autres etats
					//on passe le char en int
					sscanf(transitionTmp[k], "%d", &etat); 
					//on incremente avec le nombre d'etat de afn1
					etat = etat + afn1.tailleQ - 1;

					//on le remet en string (char*)
					sprintf(transitionTmp[k], "%d", etat);

					//on ajoute les char de l'etat dans la chaine de transition
					y=0; //index de la tranisition k
					while(transitionTmp[k][y]!='\0'){
						afn1.D[i][j] = transitionTmp[k][y];
						j++;
						y++;
					}
   					
				}

			}
			if(k!=2){
				//on ajoute l'element separateur si on est pas sur l'etat darrivee
				afn1.D[i][j] = '/';
				j++;
			}
			


			//on passe a la partie suivante de la transition
			ptr = strtok(NULL, delim);
		}
		afn1.D[i][j] = '\0';
			
	}


	//enregistre les nouvelle taille de Q, F et D
	afn1.tailleF = nouvelleTailleF;
	afn1.tailleQ = nouvelleTailleQ;
	afn1.tailleD = nouvelleTailleD;


	return afn1;
}


/** Fonctions annexe **/
//Les afn sont-ils identiques ?
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

//Extraction de mot
int extract(int from, int to, char *chaine, char *sousChaine)
{
  int i=0, j=0;
  //récupérer la longueur de la chaîne
  int length = strlen(chaine);

  if( from > length || from < 0 ){
    printf("L'index 'from' est invalide\n");
    return 1;
  }
  if( to > length ){
    printf("L'index 'to' est invalide\n");
    return 1;
  }
  for( i = from, j = 0; i <= to; i++, j++){
    sousChaine[j] = chaine[i];
  }
  return 0;
}

AUTOMATEAFN concatenationDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2)
{
	AUTOMATEAFN automateConcatene; 
	int i,j,verifCaractere;

	//Q
	printf("Q\n" );
		automateConcatene.tailleQ = afn1.tailleQ + afn2.tailleQ - 1; //On retire l'etat initial de afn2

	//allocation de Q
	automateConcatene.Q = (unsigned int *) malloc(automateConcatene.tailleQ * sizeof(unsigned int));

	//ajout des états de afn2 dans afn1.Q
	for(i=0; i<afn2.tailleQ; i++){ //on ne récupère pas l'état initial de afn2
		if(afn2.Q[i]!=0){
			automateConcatene.Q[afn1.tailleQ-1+i] = afn2.Q[i];//+afn1.tailleQ-1; //on démarre le compte des etats de afn2 après ceux de afn1
		}
		
	}

	//Z
	printf("Z\n" );
	//Allocation de Ztemp au maximum (afn1.Z + afn2.Z)
	char* Ztemp = (char*)malloc(sizeof(char)*(strlen(afn1.Z)+strlen(afn2.Z)));
	strcpy(Ztemp,afn1.Z); //copie de afn1.Z dans Ztemp

	//Determinisation des nouveaux caracteres
	for(i=0;i<strlen(afn2.Z);i++){		//Pour chaque caractere de afn2
		verifCaractere=0; //on a pas trouvé le caractère
		for(j=0;j<strlen(afn1.Z);j++){	//Est il deja present dans afn1
			if(!verifCaractere){ //si on a trouvé le caractère pas besoin de continuer de le chercher
				//printf("afn1:%d et afn2:%d\n",j,i );
				if(afn2.Z[i]==afn1.Z[j]){ 	//Si oui on passe au prochain caractere de afn2
					//printf("meme caractere :%c et %c\n",afn2.Z[i],afn1.Z[j] );
					verifCaractere=1;
				}
			}
		}

		if(!verifCaractere){ //si on a pas trouvé le caractere dans tout l'aphabet de afn1 alors on l'ajoute
				//printf("ajout de %c\n", afn2.Z[i]);
				Ztemp[strlen(afn1.Z)] = afn2.Z[i];

		}
	}

	//On modifie automateConcatene.Z par Ztemp
	automateConcatene.Z = (char*)malloc(strlen(Ztemp)*sizeof(char));
	strcpy(automateConcatene.Z,Ztemp);


	//S
	printf("S\n" );
	//l'etat initial est le meme que celui du 1er automate
	automateConcatene.s = afn1.s;

	//F
	printf("F\n" );
	unsigned int k=1;
	//On regarde si l'etat initial de afn2 est accepteur
	if(afn2.F[0]==0)
	{
		//dans ce cas là les etats finaux de afn1 le sont aussi
		automateConcatene.tailleF = afn1.tailleF + afn2.tailleF-1;
		//On remplit F
		for(i=0; i<automateConcatene.tailleF; i++)
		{
			if(i<afn1.tailleF)
			{
				automateConcatene.F[i] = afn1.F[i];//on remplit avec les etats finaux
			}
			else 
			{
				automateConcatene.F[i] = afn2.F[k];
				k++;
			}
		}
	
	}else // sinon seuls les etats finaux de afn2 sont finaux
	{
		automateConcatene.tailleF = afn2.tailleF;
		//On remplit F
		automateConcatene.F = afn2.F;
	}
	
	
	//D
	/*printf("D\n" );
	char* Dtemp = (char*)malloc(sizeof(char)*(strlen(afn1.D)+strlen(afn2.D)));
	unsigned int indiceD= afn1.tailleD;

	//Variables pour la gestion des expressions régulières
	int err;
	regex_t preg;
	const char *str_regex = "0[a-z0-9][0-9]+";

	//Variable pour stocker une sous-chaine
	char * etatDeDepart;
	char * carac;
	char * etatArrive;

	//On mets toutes les transitions de afn1 dans Dtemp
	strcpy(Dtemp,afn1.D);

	//Ajouter les transitions des états finaux de afn1 vers des etats que l'on peux obtenir à partir de s2
	for(i=0; i< afn1.tailleF; i++)
	{
		for(j=0; j< afn2.tailleD; j++)
			{
				err = regcomp (&preg, str_regex, REG_NOSUB | REG_EXTENDED);
				if (err == 0)
				{
					int match;

					//on teste si afn2.D[i] correspond à l'espression régulière 
					match = regexec (&preg, afn2.D[j], 0, NULL, 0);
					//on free preg
					regfree (&preg);

					//si l'espression regulière match afn2.D[i]
					if (match == 0) //si j'ai une transistion de s2 vers un autre etat
					{
						//On récupère les information de la nouvelle transition
						extract(2, 2, afn1.D[i], etatDeDepart);
						extract(1, 1, afn2.D[j] ,carac);
						extract(2, 2, afn2.D[j] ,etatArrive);
						Dtemp[indiceD]=etatDeDepart+carac+etatArrive;
						indiceD+=1;
					}
			}	}
	}
	//Supprimer les transitions partant de s2 dans afn2

	//On modifie automateConcatene.Z par Ztemp
	automateConcatene.D = (char*)malloc(strlen(Dtemp)*sizeof(char));
	strcpy(automateConcatene.D,Dtemp);*/
	return automateConcatene;
}