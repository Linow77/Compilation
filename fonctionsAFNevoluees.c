#include "fonctionsAFNevoluees.h"


/**AUTOMATE FINI NON DETERMINISTES PLUS EVOLUES **/
AUTOMATEAFN unionDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2){
	
	/**Creation des variables **/
	AUTOMATEAFN afn; //automate que l'on renvoie

	int verifCaractere=0,etatInitialAccepteur=0;
	int i,j;

	//copie de afn1 dans afn
	afn = copie(afn1);

	//Allocation de Ztemp au maximum (afn1.Z + afn2.Z)
	char* Ztemp = (char*)malloc(sizeof(char)*(strlen(afn1.Z)+strlen(afn2.Z)));
	strcpy(Ztemp,afn1.Z); //copie de l'alphabet de afn1 dans Ztemp

	//Verifier que les deux afn ne sont pas identique sinon directement renvoyer la copie d'un des deux
	if(afn_identique(afn1,afn2)){
		return afn;
	}

	// Verif si les etats initiaux de afn1 et afn2 sont accepteurs
	//Nous avons les 3 cas distincts qui nous serviront pour remplir F
	//cas 1
	//on vérifie tout les etats accepteurs car l'état 0 n'est pas forcément le premier accepteur
	for(i=0;i<afn1.tailleF;i++){ 
		if(afn1.F[i]==0){
			etatInitialAccepteur=1; // afn1 a son etat initial accepteur
		}
	}

	//cas 2
	//Verif si l'etat initial de afn1 n'est pas déja accepteur
	if(!etatInitialAccepteur){
		for(i=0;i<afn2.tailleF;i++){ 
			if(afn2.F[i]==0){
				etatInitialAccepteur=2;// afn2 a son etat initial accepteur
			}
		}
	}else{ //cas 3
		for(i=0;i<afn2.tailleF;i++){ 
			if(afn2.F[i]==0){
				etatInitialAccepteur=3;// les deux afn ont leurs etats initiaux accepteurs
			}
		}
	}


	/** Modification de afn1 pour y ajouter le langage reconnu par afn2 **/

	//Création de Q
	//tailleQ = tailleQ1 -1 + tailleQ2 -1 + 1 car on supprime les etats initiaux et on en creer un qui sera initial pour les deux automates
	afn.tailleQ = afn1.tailleQ + afn2.tailleQ - 1;

	//réallocation de Q
	afn.Q = (unsigned int *) realloc( afn.Q, afn.tailleQ * sizeof(unsigned int) );

	//ajout des états de afn2 dans afn1.Q
	for(i=0; i<afn2.tailleQ; i++){ //on ne récupère pas l'etat initial de afn2
		if(afn2.Q[i]!=0){
			afn.Q[afn1.tailleQ-1+i] = afn2.Q[i]+afn1.tailleQ-1; //on incremente les etats de afn2 avec les etats de afn1
		}	
	}

	//Creation de Z
	//On cherches les nouveaux caracteres
	for(i=0;i<strlen(afn2.Z);i++){		//Pour chaque caractere de afn2
		verifCaractere=0; //on a pas trouve le caractere
		for(j=0;j<strlen(afn1.Z);j++){	//Est il deja present dans afn1
			if(!verifCaractere){ //si on a trouve le caractere pas besoin de continuer de le chercher
				if(afn2.Z[i]==afn1.Z[j]){ 	//Si il est present on passe au prochain caractere de afn2
					verifCaractere=1;
				}
			}
		}
		if(!verifCaractere){ //si on a pas trouvé le caractere dans tout l'aphabet de afn1 alors on l'ajoute
			Ztemp[strlen(afn1.Z)] = afn2.Z[i];
		}
	}

	
	//reallocation de afn1.Z a la bonne taille
	afn.Z = (char*) realloc(afn.Z,strlen(Ztemp)*sizeof(char));
	//On modifie afn1.Z par Ztemp
	strcpy(afn.Z,Ztemp);

	//s n'est pas modifié il reste 0

	//Creation de F
	//on modifie la taille de F par afn1.tailleF + afn2.tailleF 
	afn.tailleF = afn1.tailleF + afn2.tailleF;
	//-1 si l'état initial de afn2 est accepteur et celui de afn1 aussi (cas 3)
	if(etatInitialAccepteur==3){
		afn.tailleF--;
	}
	//On realloue F a la bonne taille
	afn.F = (int*) realloc(afn.F,afn.tailleF*sizeof(int));

	//ajout des etats finaux de afn2 et son etat initial est final
	for(i=afn1.tailleF; i<afn.tailleF;i++){
		//on ajoute l'etat initial seulement si l'etat initial de afn2 est accepteur et pas celui de afn1
		if(etatInitialAccepteur==2 && afn2.F[i-afn1.tailleF]==0){
			afn.F[i]=afn2.F[i-afn1.tailleF];
		}else{//sinon on ajoute les etats finaux de afn2 en les incrementant avec les etats de afn1
			afn.F[i]=afn2.F[i-afn1.tailleF]+afn1.tailleQ-1;
		}
		
	}

	/* Creation de D*/
	//On ajoute toutes les transitions de afn2 a afn1
	afn.tailleD = afn1.tailleD + afn2.tailleD;
	//reallocation de D a la bonne taille
	afn.D = (TRANSITION*)realloc(afn.D,sizeof(TRANSITION)*afn.tailleD);

	//On ajoute les etats de afn2 en les incrémentant avec les etats de afn1 sauf l'etat initial 0
	for(i=afn1.tailleD;i<afn.tailleD;i++){
		//On ne change pas le caractere
		afn.D[i].caractere = afn2.D[i-afn1.tailleD].caractere;
		//Traitements des etats
		//On ne change pas si c'est l'etat initial 0
		if(afn2.D[i-afn1.tailleD].depart==0){
			afn.D[i].depart = afn2.D[i-afn1.tailleD].depart;
		}else{
			afn.D[i].depart = afn2.D[i-afn1.tailleD].depart + afn1.tailleQ-1;
		}
		afn.D[i].arrivee = afn2.D[i-afn1.tailleD].arrivee + afn1.tailleQ-1;			
	}

	return afn;
}

AUTOMATEAFN concatenationDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2){
	AUTOMATEAFN automateConcatene; 
	int i,j,verifCaractere;

	//Q
	automateConcatene.tailleQ = afn1.tailleQ + afn2.tailleQ - 1; //On retire l'etat initial de afn2
	//allocation de Q
	automateConcatene.Q = (unsigned int *) malloc(automateConcatene.tailleQ * sizeof(unsigned int));

	//ajout des états de afn2 dans afn1.Q
	for(i=0; i<=automateConcatene.tailleQ; i++){ //<= car quand on rencontre l'etat 0, on n'ajoute pas d'etat
		if(i<afn1.tailleQ) //on mets tous les etats de afn1
		{
			automateConcatene.Q[i] = afn1.Q[i];
		}else{
			if(afn2.Q[i-afn1.tailleQ]!=0){	//on ne récupère pas l'état initial de afn2
				automateConcatene.Q[i-1] = afn2.Q[i-afn1.tailleQ] +afn1.tailleQ-1; 
			}
		}
		
	}

	//Z
	automateConcatene.Z = (char*)malloc(sizeof(char));
	//Allocation de Ztemp au maximum (afn1.Z + afn2.Z)
	char* Ztemp = (char*)malloc(sizeof(char)*(strlen(afn1.Z)+strlen(afn2.Z)));
	strcpy(Ztemp,afn1.Z); //copie de afn1.Z dans Ztemp

	//Determinisation des nouveaux caracteres
	for(i=0;i<strlen(afn2.Z);i++){		//Pour chaque caractere de afn2
		verifCaractere=0; //on a pas trouvé le caractère
		for(j=0;j<strlen(afn1.Z);j++){	//Est il deja present dans afn1
			if(!verifCaractere){ //si on a trouvé le caractère pas besoin de continuer de le chercher
				if(afn2.Z[i]==afn1.Z[j]){ 	//Si oui on passe au prochain caractere de afn2
					verifCaractere=1;
				}
			}
		}

		if(!verifCaractere){ //si on a pas trouvé le caractere dans tout l'aphabet de afn1 alors on l'ajoute
				Ztemp[strlen(afn1.Z)] = afn2.Z[i];

		}
	}

	//On modifie automateConcatene.Z par Ztemp
	automateConcatene.Z = (char*)realloc(automateConcatene.Z,strlen(Ztemp)*sizeof(char));
	strcpy(automateConcatene.Z,Ztemp);


	//S
	//l'etat initial est le meme que celui du 1er automate
	automateConcatene.s = afn1.s;

	//F
	unsigned int k=0;
	unsigned int etat0Accept = 0;

	//On regarde si l'etat initial de afn2 est accepteur
	for( i=0; i<afn2.tailleF; i++)
	{
		if(afn2.F[i]==0)
			etat0Accept = 1;
	}

	if(etat0Accept == 1)
	{
		//dans ce cas là les etats finaux de afn1 le sont aussi
		automateConcatene.tailleF = afn1.tailleF + afn2.tailleF-1; //taille= etats finaux de afn1 + etats finaux de afn2 sauf l'etat 0
		automateConcatene.F = (int*) malloc(automateConcatene.tailleF *sizeof(int));
		//On remplit F
		for(i=0; i<automateConcatene.tailleF; i++)
		{
			if(i<afn1.tailleF)
			{
				automateConcatene.F[i] = afn1.F[i];//on remplit avec les etats finaux
			}
			else 
			{
				if(afn2.F[k]!= 0)// si ce n'est pas l'etat 0 de afn2 alors on met l'etat
				{
					automateConcatene.F[i] = afn2.F[k] + afn1.tailleQ -1;

				}else{
					i--;
				}
				k++;


			}
		}
	
	}else // sinon seuls les etats finaux de afn2 sont finaux
	{
		automateConcatene.tailleF = afn2.tailleF;
		automateConcatene.F = (int*) malloc(automateConcatene.tailleF *sizeof(int));
		//On remplit F
		for(i=0;i<automateConcatene.tailleF;i++)
		{
			automateConcatene.F[i] = afn2.F[i] + afn1.tailleQ -1;
		}
		
	}
	
	//D
	//Taille de D = D1 + etats finaux afn1 * etats qui ont une transitions depuis s2 + reste transition afn2

	unsigned int cmpS2 = 0;//nbre d'etat qui partent de s2
	unsigned int cmpnS2 = 0;//nbre d'etat qui ne partent pas de s2

	for(i=0; i< afn2.tailleD; i++)
	{	

		if(afn2.D[i].depart == 0){
			cmpS2++; //nbre etats qui sortent de s2
		}else{
			cmpnS2++; //nbre etats qui ne sortent pas de s2
		}

	}


	//Taille de D et allocation de D
	automateConcatene.tailleD = afn1.tailleD + (afn1.tailleF*cmpS2) + cmpnS2;
	automateConcatene.D =(TRANSITION *) malloc(sizeof(TRANSITION)*automateConcatene.tailleD); 

	//remplissage de D
	for(i=0;i<automateConcatene.tailleD;i+=0)
	{
		if(i<afn1.tailleD) //on met d'abord toutes les transitions de afn1
		{
			automateConcatene.D[i].depart = afn1.D[i].depart;
			automateConcatene.D[i].caractere = afn1.D[i].caractere;
			automateConcatene.D[i].arrivee = afn1.D[i].arrivee;
			i++;
		}
		else //on rajoute les nvelles transitions
		{
	
			for(k=0;k<afn2.tailleD;k++)  //on parcours les transitions de afn2
			{
				if (afn2.D[k].depart==0){
				
					for(j=0;j<afn1.tailleF;j++)  //pour tous les etats accepteurs de afn1
					{
						automateConcatene.D[i].depart = afn1.F[j];
						automateConcatene.D[i].caractere = afn2.D[k].caractere;
						automateConcatene.D[i].arrivee = afn2.D[k].arrivee + afn1.tailleQ -1;
						i++;
					}

				}else{
					automateConcatene.D[i].depart = afn2.D[k].depart + afn1.tailleQ -1;
					automateConcatene.D[i].caractere = afn2.D[k].caractere;
					automateConcatene.D[i].arrivee = afn2.D[k].arrivee + afn1.tailleQ -1;
					i++;
				}	
			}
		}
	}

	return automateConcatene;
}

//Mise a l'etoile
AUTOMATEAFN kleene(AUTOMATEAFN afn){
	AUTOMATEAFN automateKleene;
	unsigned int i,k,j,verif_s =0;

	//Q
	//allocation de Q
	automateKleene.Q = (unsigned int *) malloc(afn.tailleQ * sizeof(unsigned int));
	automateKleene.tailleQ = afn.tailleQ;

	for(i=0; i< automateKleene.tailleQ;i++)
	{
		automateKleene.Q[i] = afn.Q[i];
	}

	//Z
	automateKleene.Z = (char*)malloc(strlen(afn.Z)*sizeof(char));
	strcpy(automateKleene.Z,afn.Z);

	//S
	automateKleene.s = afn.s;

	//F
	//On vérifie si l'etat initial est déjà accepteur 
	for(i=0; i< afn.tailleF;i++)
	{
		if(afn.F[i]==0) //si il est deja accepteur
		{
			verif_s =1;
		}
	}
	if( verif_s == 1)//on ne doit pas changer tailleF ni F
		{
			automateKleene.tailleF = afn.tailleF;
			automateKleene.F = (int*) malloc(automateKleene.tailleF *sizeof(int));
			for(i=0; i< automateKleene.tailleF;i++)
			{
				automateKleene.F[i] = afn.F[i];
			}
		}
		else//sinon on augùente la taille de 1 pour qu'elle prenne l'etat initial
		{
			automateKleene.tailleF = afn.tailleF+1;
			automateKleene.F = (int*) malloc(automateKleene.tailleF *sizeof(int));

			//On remplit le tab F en rajoutant l'etat 0
			automateKleene.F[0] = 0;
			for(i=1; i< automateKleene.tailleF;i++)
			{
				automateKleene.F[i] = afn.F[i-1];
			}
		}

	//D

	//Calcul de la taille de D
	unsigned int cmpS2 = 0;//nbre d'etat qui partent de s2

	for(i=0; i< afn.tailleD; i++)
	{	
		//strcpy(tmp,afn.D[i]);
		if(afn.D[i].depart == 0) //si on trouve une transition qui part de s2
			{
				cmpS2++; //nbre etats qui sortent de s2
			}
		
	}

	if(verif_s==1)
	{
			automateKleene.tailleD = afn.tailleD + ((afn.tailleF-1)*cmpS2);
	}
	else{
			automateKleene.tailleD = afn.tailleD + (afn.tailleF*cmpS2);
	}

	//Allocation de D
	automateKleene.D =(TRANSITION *) malloc(sizeof(TRANSITION)*automateKleene.tailleD); //remplit tab d'etat que je peux avoir depuis s2

	//remplissage de D
	for(i=0;i<automateKleene.tailleD;i+=0)
	{
		if(i<afn.tailleD) //on met d'abord toutes les transitions de afn
		{
			automateKleene.D[i].depart = afn.D[i].depart;
			automateKleene.D[i].caractere = afn.D[i].caractere;
			automateKleene.D[i].arrivee = afn.D[i].arrivee;
			i++;
		}
		else //on rajoute les nvelles transitions
		{
	
			for(k=0;k<afn.tailleD;k++)  //on parcours les transitions de afn2
			{
				if(afn.D[k].depart == 0){

					for(j=0;j<afn.tailleF;j++)  //pour tous les etats accepteurs de afn
					{
						if(afn.F[j]!=0){
							automateKleene.D[i].depart = afn.F[j];
							automateKleene.D[i].caractere = afn.D[k].caractere;
							automateKleene.D[i].arrivee = afn.D[k].arrivee;
							i++;
						}
					}
				}	
				
			}
			
		}
		
	}

	return automateKleene;
}

/** Fonctions annexe **/
//Afficher un automate standard non deterministe
void AfficherAutomateNonDeterministe(AUTOMATEAFN afn){
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
			printf(" %d, ",afn.F[i]);
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

			printf("%d%c%d, ",afn.D[i].depart,afn.D[i].caractere,afn.D[i].arrivee);
		}
		printf("\n");	
	}else{
		printf("Aucune transition\n");
	}
}

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

void free_afn(AUTOMATEAFN afn){
	//Q
	free(afn.Q);

	//Z
	if(afn.Z!=NULL && strcmp(afn.Z,"")){ //on n'a pas alloue pour le langage vide et le mot vide
		free(afn.Z);
	}
	

	//F
	if(afn.F!=NULL){
		free(afn.F);
	}
	

	//D
	if(afn.D!=NULL){ //on n'a pas alloue pour le langage vide et le mot vide
		free(afn.D);
	}	
}

//Copie d'un automate fini non deterministe
AUTOMATEAFN copie(AUTOMATEAFN afn){
	AUTOMATEAFN afn_copie;
	int i;

	//copie de Q
	afn_copie.tailleQ = afn.tailleQ;
	afn_copie.Q = (unsigned int*)malloc(sizeof(unsigned int)*afn_copie.tailleQ);
	
	for (i=0;i<afn_copie.tailleQ;i++){
		afn_copie.Q[i] = afn.Q[i];
	}

	//copie de Z
	afn_copie.Z = (char*)malloc(sizeof(char));
	strcpy(afn_copie.Z,afn.Z);

	//copie de s
	afn_copie.s = afn.s;

	//copie de F
	afn_copie.tailleF = afn.tailleF;
	afn_copie.F = (int*)malloc(sizeof(int)*afn_copie.tailleF);
	
	for (i=0;i<afn_copie.tailleF;i++){
		afn_copie.F[i] = afn.F[i];
	}
	//copie de D
	afn_copie.tailleD = afn.tailleD;
	afn_copie.D = (TRANSITION*)malloc(sizeof(TRANSITION)*afn_copie.tailleD);

	for (i=0;i<afn_copie.tailleD;i++){
		afn_copie.D[i].depart = afn.D[i].depart;
		afn_copie.D[i].caractere = afn.D[i].caractere;
		afn_copie.D[i].arrivee = afn.D[i].arrivee;
	}

	return afn_copie;
}