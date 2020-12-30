#include "projet.h"

int main() {

	AUTOMATEAFN afnVide,afnMotVide,afncaractere1,afncaractere2, afnUnion, afnConcatene,afnKleene;
	AUTOMATEAFD afd;

	afnVide = langageVide();
	printf("/**** Langage Vide ****/\n");
	AfficherAutomateNonDeterministe(afnVide);

	afnMotVide = langageMotVide();
	printf("\n/**** Langage Mot Vide ****/\n");
	AfficherAutomateNonDeterministe(afnMotVide);

	afncaractere1 = langagecaractere('a');
	printf("\n/**** Langage Caractere ****/\n");
	AfficherAutomateNonDeterministe(afncaractere1);

	afncaractere2 = langagecaractere('b');
	printf("\n/**** Langage Caractere ****/\n");
	AfficherAutomateNonDeterministe(afncaractere2);

	afnKleene = kleene(afncaractere1);
	printf("\n/**** Version Kleene sur langagecaractere1 ****/\n");
	AfficherAutomateNonDeterministe(afnKleene);

	afnUnion = unionDeDeuxAutomates(afncaractere2,afnKleene);
	printf("\n/**** Langage Union ****/\n");
	AfficherAutomateNonDeterministe(afnUnion);

	afnConcatene = concatenationDeDeuxAutomates(afncaractere1,afncaractere1);
	printf("\n/**** Langage Concatené ****/\n");
	AfficherAutomateNonDeterministe(afnConcatene);

	//afnConcatene = concatenationDeDeuxAutomates(afnConcatene,afnConcatene);
	//printf("\n/**** Langage Concatené ****/\n");
	//AfficherAutomateNonDeterministe(afnConcatene);

	//afnKleene = kleene(afnConcatene);
	//printf("\n/**** Version Kleene sur langagecaractere1 ****/\n");
	//AfficherAutomateNonDeterministe(afnKleene);

	afd = determinisation(afnKleene);
	printf("\n/**** AFD Concatené ****/\n");
	//AfficherAutomateDeterministe(afd);
	
	//free des afn
	free_afn(afnVide);
	free_afn(afnMotVide);
	free_afn(afncaractere1);
	free_afn(afncaractere2);
	free_afn(afnUnion);
	free_afn(afnConcatene);
	free_afn(afnKleene);

	return 1;

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
	afn.Z = (char*)malloc(sizeof(char));
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
	afn.Q[2] = 2;
	

	afn.Z = (char*)malloc(sizeof(char));	// allouer que la taille d'un char ?
	afn.Z[0] = caractere;
	afn.Z[1] = '\0';

	afn.s = 0;

	afn.tailleF = 2;
	afn.F = (int*)malloc(sizeof(int)*afn.tailleF);
	afn.F[0] = 1;
	afn.F[1] = 2;//temporaire pour les tests

	afn.tailleD = 2;
	afn.D = (TRANSITION*)malloc(sizeof(TRANSITION)*afn.tailleD);
	afn.D[0].depart = 0;
	afn.D[0].caractere = caractere;
	afn.D[0].arrivee = 1;

	//test
	afn.D[1].depart = 1;
	afn.D[1].caractere = caractere;
	afn.D[1].arrivee = 2;

	return afn;
}

/**AUTOMATE FINI NON DETERMINISTES PLUS EVOLUES **/
AUTOMATEAFN unionDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2){
	AUTOMATEAFN afn; //automate que l'on renvoie

	int verifCaractere=0,etatInitialAccepteur=0;
	int i,j;

	//copie de afn1 dans afn
	afn = copie(afn1);

	//Allocation de Ztemp au maximum (afn1.Z + afn2.Z)
	char* Ztemp = (char*)malloc(sizeof(char)*(strlen(afn1.Z)+strlen(afn2.Z)));
	strcpy(Ztemp,afn1.Z); //copie de afn1.Z dans Ztemp

	//Verifier que les deux afn ne sont pas identique sinon directement renvoyer l'un des deux
	if(afn_identique(afn1,afn2)){
		return afn;
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
	afn.tailleQ = afn1.tailleQ + afn2.tailleQ - 1;

	//réallocation de Q
	afn.Q = (unsigned int *) realloc( afn.Q, afn.tailleQ * sizeof(unsigned int) );

	//ajout des états de afn2 dans afn1.Q
	for(i=0; i<afn2.tailleQ; i++){ //on ne récupère pas l'état initial de afn2
		if(afn2.Q[i]!=0){
			afn.Q[afn1.tailleQ-1+i] = afn2.Q[i]+afn1.tailleQ-1; //on démarre le compte des etats de afn2 après ceux de afn1
		}
		
	}

	//Creation de Z
	//Determinisation des nouveaux caracteres
	for(i=0;i<strlen(afn2.Z);i++){		//Pour chaque caractere de afn2
		verifCaractere=0; //on a pas trouvé le caractère
		for(j=0;j<strlen(afn1.Z);j++){	//Est il deja present dans afn1
			if(!verifCaractere){ //si on a trouvé le caractère pas besoin de continuer de le chercher
				if(afn2.Z[i]==afn1.Z[j]){ 	//Si oui on passe au prochain caractere de afn2
					verifCaractere=1;
					
					
				}else{ //Sinon on regarde s'il est présent dans un autre
				}
			}
		}

		if(!verifCaractere){ //si on a pas trouvé le caractere dans tout l'aphabet de afn1 alors on l'ajoute
				Ztemp[strlen(afn1.Z)] = afn2.Z[i];
		}
	}

	//On modifie afn1.Z par Ztemp
	//reallocation de afn1.Z
	afn.Z = (char*) realloc(afn.Z,strlen(Ztemp)*sizeof(char));
	strcpy(afn.Z,Ztemp);

	//s n'est pas modifié il reste 0

	//Creation de F
	//on realloue la afn1.tailleF  + afn2.tailleF -1(si l'état initial de afn2 est accepteur et celui de afn1 aussi)
	afn.tailleF = afn1.tailleF + afn2.tailleF;
	if(etatInitialAccepteur==3){
		afn.tailleF--;
	}
	afn.F = (int*) realloc(afn.F,afn.tailleF*sizeof(int));

	//ajout des F de afn2 et l'etat initial si accepteur
	for(i=afn1.tailleF; i<afn.tailleF;i++){
		//on ajoute l'etat initial seulement si l'etat initial de afn2 est accepteur et pas celui de afn1
		if(etatInitialAccepteur==2 && afn2.F[i-afn1.tailleF]==0){

			afn.F[i]=afn2.F[i-afn1.tailleF];
		
		}else{//sinon on ajoute les etats de afn2 en démarrant le compte après ceux de afn1
			afn.F[i]=afn2.F[i-afn1.tailleF]+afn1.tailleQ-1;
		}
		
	}

	/* Creation de D*/
	//reallocation de D
	afn.tailleD = afn1.tailleD + afn2.tailleD;
	afn.D = (TRANSITION*)realloc(afn.D,sizeof(TRANSITION)*afn.tailleD);

	//On ajoute les etats de afn2 en les incrémentant sauf l'etat initial 0
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

AUTOMATEAFN concatenationDeDeuxAutomates(AUTOMATEAFN afn1, AUTOMATEAFN afn2)
{
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

AUTOMATEAFN kleene(AUTOMATEAFN afn)//Mise a l'etoile
{
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

/**AUTOMATE FINI DETERMINISTES **/
AUTOMATEAFD determinisation(AUTOMATEAFN afn){
	AUTOMATEAFD afd; //automate que l'on retourne
	unsigned int i,j,k,z,y,m;
	unsigned int tailleEtat=1;
	
	unsigned int etatDejaVu = 0; //0 = jamais vu
	unsigned int transitionNonValide = 0;
	unsigned int cpt=0;

	//allocation de transitions
	TRANSITIONDETERMINISTE** transitions = (TRANSITIONDETERMINISTE **)malloc(sizeof(TRANSITIONDETERMINISTE*)*tailleEtat);  //lignes

	for (i=0;i<tailleEtat;i++){
		transitions[i] = (TRANSITIONDETERMINISTE*)malloc(sizeof(TRANSITIONDETERMINISTE)*strlen(afn.Z)); //colonnes

		for(j=0;j<strlen(afn.Z);j++){
			transitions[i][j].tailleArrivee = 1;
			transitions[i][j].arrivee = (unsigned int*)malloc(sizeof(unsigned int)*transitions[i][j].tailleArrivee);
			//init a -1 des transitions 
			transitions[i][j].arrivee[0] = -1;
		}
				
	}
	
	//allocation de etats
	TABETATS * etats = (TABETATS*)malloc(sizeof(TABETATS)*tailleEtat);
	
	for (i=0;i<tailleEtat;i++){
		etats[i].tailleColonne=1;
		etats[i].colonne=(unsigned int*)malloc(sizeof(unsigned int)*etats[i].tailleColonne);

		
	}
	//init du premier etat avec 0
	etats[0].colonne[0]=0; //1er etat 


	for(i=0;i<tailleEtat;i++){	//Pour toutes les cases de etats
		//printf("i:%d\n",i );
		for(j=0;j<etats[i].tailleColonne;j++) //Pour tous les elements de chaque cases de etats
		{
			printf("\nLa colonne etudié est de taille:%d",etats[i].tailleColonne);

			//for(k=0;k<afn.tailleD;k++){
			for(z=0;z<strlen(afn.Z);z++){		//Pour toutes les lettres de l'alphabet
				//for(z=0;z<strlen(afn.Z);z++){

				transitions[i][z].depart = etats[i].colonne[j]; //on prend le premier etat de etats
				transitions[i][z].caractere = afn.Z[z];			//on prend le premier caractere de Z
				

				for(k=0;k<afn.tailleD;k++){				//Pour toutes les transitions de l'afn
					printf("\ni:%d et j:%d et k:%d et z:%d -- etat :%d\n", i,j,k,z,etats[i].colonne[j]);
					printf("Depuis l'etat:%d avec le caractere :%c \n",transitions[i][z].depart,transitions[i][z].caractere);
					printf("Evaluation de la transition afn.D[%d]=%d%c%d\n",k,afn.D[k].depart,afn.D[k].caractere,afn.D[k].arrivee);
					printf("transition avant modif:%d\n",transitions[i][z].arrivee[transitions[i][z].tailleArrivee-cpt-1]);
					printf("transitions.tailleArrivee:%d\n",transitions[i][z].tailleArrivee);
					

					if(afn.D[k].depart == transitions[i][z].depart &&	//Si la transition afn.D[k] part du meme etat que l'etat[i].colonne[j] que l'on evalue
						afn.D[k].caractere == transitions[i][z].caractere){	//Si la transition afn.D[k] a le meme caractere que celui evalue actuellement (afn.Z[z])

						//On ajoute l'etat d'arrivee dans le tableau transition s'il n'y est pas déja
						if(verif_ajout_non_present(transitions[i][z].arrivee, transitions[i][z].tailleArrivee, afn.D[k].arrivee)){
							// On realloue une nouvelle case dans les transitions pour la suite
							if(transitions[i][z].tailleArrivee!=1){ //la 1ere case est alloué de base, pour les autres il faut les allouer avant d'ajouter l'etat
								printf("allocation d'une nouvelle case\n");
								//realloue arrivee transitions
								transitions[i][z].arrivee = (unsigned int*)realloc(transitions[i][z].arrivee,sizeof(unsigned int)*transitions[i][z].tailleArrivee);
								transitions[i][z].arrivee[transitions[i][z].tailleArrivee]=-1;
							}

							//ajout de l'etat
							transitions[i][z].arrivee[transitions[i][z].tailleArrivee-1] = afn.D[k].arrivee;

							printf("Ajout de l'etat %d dans transitions[%d][%d].arrivee[%d]\n",afn.D[k].arrivee,i,z,transitions[i][z].tailleArrivee-1)	;	
							
							printf("transition apres:%d\n",transitions[i][z].arrivee[transitions[i][z].tailleArrivee-1]);

							//on incremente la taille du tableau arrivee pour pouvoir ajouter un nouvel element
							transitions[i][z].tailleArrivee++; //On prévoit le prochain ajout
							cpt++;
						}else{
							printf("l'etat est deja dans la case transition\n");
						}


						
						
						
						
					}else{

						printf("transition apres:%d\n",transitions[i][z].arrivee[transitions[i][z].tailleArrivee-cpt-1]);
					}

					

					
					

				}
				//On a regardé toutes les transitions depuis 
				printf("on a repertorié toutes les arrivee des transitions qui parte de etat:%d avec le caractere:%c\n",transitions[i][z].depart,transitions[i][z].caractere);
				//affiche_transitions(transitions,tailleEtat, strlen(afn.Z));
				tri(transitions[i][z].arrivee,transitions[i][z].tailleArrivee-1);
				affiche_transitions(transitions,tailleEtat, strlen(afn.Z));

				//On ajoute lorsque l'on a fini d'etudier chaque elements de la case etats[i] donc tous les j
				if(j==etats[i].tailleColonne-1){
					/** traitement de l'ajout de la case transition[i][z] dans la case etats[i]**/
					printf("premier etat:%d\n",transitions[i][z].arrivee[0]);
					if(transitions[i][z].arrivee[0]!=-1){ //Si le premier element de transition vaut -1, on a pas trouvé d'etat d'arrivee
						etatDejaVu = 0; //0 = jamais vu
						for(y=0;y<tailleEtat;y++){	//Pour tous les etats de etats
							//on compare la taille de la case etats[y] avec la taille de la case transitions qu'on veut ajouter
							printf("on regarde si tailleEtat:%d == tailleArrivee:%d\n",etats[y].tailleColonne,transitions[i][z].tailleArrivee-1);
							if(etats[y].tailleColonne == (transitions[i][z].tailleArrivee-1)){ //Si taille egale 
								printf("taille egale\n");
								for(m=0;m<etats[y].tailleColonne;m++) //Pour tous les elements de chaque case de etats[y]
								{	
									printf("iteration m:%d  on va comparer :%d et %d\n",m,etats[y].colonne[m],transitions[i][z].arrivee[m]);

									if(etats[y].colonne[m]==transitions[i][z].arrivee[m]){ //si l'element m de etats[y].colonne[m] est le meme 
																						   //que l'element m de la transition que l'on veut ajouter
										printf("element m:%d correspond et transitions[i][z].tailleArrivee-1:%d\n",m, transitions[i][z].tailleArrivee-1);

										if(m+1==transitions[i][z].tailleArrivee-1){
											etatDejaVu = 1;
											printf("tous les elements de la case etats[%d].colonne correspondent a la case transitions[%d)[%d].arrivee\n",y,i,z);
										}

									}else{
										printf("la case m:%d ne correspond pas on va a la case etats[y+1] si y+1<tailleEtat\n",m);
										break; //termine la for sur m --> on passe a y+1
									}
								}
							}
							
						}

						printf("/**AJOUT DE LA CASE si dejavu:%d == 0**/\n", etatDejaVu);
						if(!etatDejaVu){
							//On ajoute la case de transition[i][z] a la prochaine case de etats[]
							tailleEtat++; 
							printf("tailleEtat:%d\n",tailleEtat);
							//On alloue la nouvelle case de etats
							etats = (TABETATS*)realloc(etats,sizeof(TABETATS)*tailleEtat);
							//on donne a la nouvelle case la taille de la case de transition que l'on va ajouter
							etats[tailleEtat-1].tailleColonne=transitions[i][z].tailleArrivee-1; //on a tailleArrivee qui a une case supplementaire (pour un prochain ajout)

							printf("Taille de la case a ajouter:%d\n",etats[tailleEtat-1].tailleColonne);
							//On alloue sa colonne
							etats[tailleEtat-1].colonne=(unsigned int*)malloc(sizeof(unsigned int)*etats[tailleEtat-1].tailleColonne);

							//on alloue la nouvelle colonne de transitions corespondant a l'etude du nouvel etat
							printf("j'alloue une nouvelle colonne de transitions\n");
							transitions = (TRANSITIONDETERMINISTE **)realloc(transitions,sizeof(TRANSITIONDETERMINISTE*)*tailleEtat);  //lignes
							transitions[tailleEtat-1] = (TRANSITIONDETERMINISTE*)malloc(sizeof(TRANSITIONDETERMINISTE)*strlen(afn.Z)); //colonnes

							//allocations du tableau arrivee de la nouvelle case
							for(y=0;y<strlen(afn.Z);y++){
								transitions[tailleEtat-1][y].tailleArrivee = 1;
								transitions[tailleEtat-1][y].arrivee = (unsigned int*)malloc(sizeof(unsigned int)*transitions[tailleEtat-1][y].tailleArrivee);
								//init a -1 des transitions 
								transitions[tailleEtat-1][y].arrivee[0] = -1;
							}
									

							//on ajoute les transitions d'arrivees dans la nouvelle case
							//avant on trie les transitions pour ne pas créer des doublons tel que (4,1) et (1,4) 
							tri(transitions[i][z].arrivee,transitions[i][z].tailleArrivee-1);

							for (y=0;y<etats[tailleEtat-1].tailleColonne;y++){
								/** avant d'ajouter verifier si pas deja present --> effectuer grace au tri*/
								printf("y:%d cas arret:%d\n",y,etats[tailleEtat-1].tailleColonne);
								//ajout des arrivee dans le nouvel etat
								etats[tailleEtat-1].colonne[y]=transitions[i][z].arrivee[y];
								printf("ajout de %d sur etats[%d].colonne[%d]=%d\n", transitions[i][z].arrivee[y],tailleEtat-1,y,etats[tailleEtat-1].colonne[y]);
								
							}
							
						}
					}

					
					
				}
					
				
			}
			
		}
		//affiche_determinisation(etats, tailleEtat);
		cpt=0;
			
	}
	affiche_determinisation(etats, tailleEtat);

	//free des tableaux
	//free de transitions
	for (i=0;i<tailleEtat;i++){
		free(transitions[i]);
		for(j=0;j<strlen(afn.Z);j++){

			free(transitions[i][j].arrivee);
		}
				
	}
	free(transitions); 
	

	//free des etats	
	for (i=0;i<tailleEtat;i++){
		free(etats[i].colonne);		
	}
	free(etats);

		return afd;
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

void free_afn(AUTOMATEAFN afn)
{
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

//Copie d'un automate
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

//Afficher un automate standard deterministe
void AfficherAutomateDeterministe(AUTOMATEAFD afd){
	int i=0;
	int tailleZ;

	//Q
	//printf("taille de Q: %d\n",tailleQ);
	if(afd.Q!=NULL){
		printf("Ensemble d'etats (Q): ");
		for(i=0;i<afd.tailleQ;i++){
			printf("%u, ",afd.Q[i]);
		}	
		printf("\n");
	}

	//Z

	if(afd.Z == NULL){
		printf("Alphabet de l'automate (Z): vide\n");
	} else if(!strcmp(afd.Z,"")){
		printf("Alphabet de l'automate (Z): mot vide\n");
	}else{
		tailleZ = strlen(afd.Z);
		printf("Alphabet de l'automate (Z): ");
		for(i=0;i<tailleZ;i++){
			printf("%c, ",afd.Z[i]);
		}
		printf("\n");
	}
	

	//s
	printf("Etat initial (s): %u\n",afd.s);

	//F
	//printf("taille de F: %d\n",tailleF);
	if(afd.F!=NULL){
		printf("Ensemble d'etats accepteurs (F): ");
		for(i=0;i<afd.tailleF;i++){
			printf(" %d, ",afd.F[i]);
		}
		printf("\n");
	}else{
		printf("Aucun etat accepteur\n");
	}

	//D
	//printf("taille de D: %d\n",tailleD);
	if(afd.Delta!=NULL){
		printf("Ensemble des etats transitions (D): ");

		for(i=0;i<afd.tailleDelta;i++){

			printf("%d%c%d ,",afd.Delta[i].depart,afd.Delta[i].caractere,afd.Delta[i].arrivee);
		}
		printf("\n");	
	}else{
		printf("Aucune transition\n");
	}
}

//fonction de tri rapide
void tri(unsigned int* T, int ligne){
	int i,j,c;

	if(ligne>1){
		for(i=0;i<ligne-1;i++){

		    for(j=i+1;j<ligne;j++){


		        if ( T[i] > T[j] ) {
		            c = T[i];
		            T[i] = T[j];
		            T[j] = c;
		        }
		    }
		}
	}
}

//afficher etat
void affiche_determinisation(TABETATS * etats, int tailleEtat){
	int i,j;
	printf("/*** Etats ***/\n");
	printf("[");
	for(i=0;i<tailleEtat;i++){
		//printf("tailleColonne:%d\n",etats[i].tailleColonne);
		printf("[");

		for(j=0;j<etats[i].tailleColonne;j++){

			printf(" %d ",etats[i].colonne[j]);
		}
		printf("]");
	}
	printf("]");
}

//affiche transitions
void affiche_transitions(TRANSITIONDETERMINISTE** transitions,int tailleEtat, int tailleZ){
	int i,j,y;

	for (i=0;i<tailleEtat;i++){
		for (j=0;j<tailleZ;j++){
			printf("transitions[i][j].arrivee:");
			for(y=0;y<transitions[i][j].tailleArrivee-1;y++){
				printf("%d ",transitions[i][j].arrivee[y]);
			}
			printf("\n");
		}
	}
}

int verif_ajout_non_present(unsigned int* arrivee, int tailleArrivee, int ajout){
	int i;
	int verif=1;

	for (i=0;i<tailleArrivee;i++){
		
		printf("on regarde la case arrivee[%d]\n",i);
		if(arrivee[i] == ajout){
			verif=0;
			break;
		}
				
			
	}

	return verif;
}