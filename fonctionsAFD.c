#include "fonctionsAFD.h"

/**AUTOMATE FINI DETERMINISTES **/
AUTOMATEAFD determinisation(AUTOMATEAFN afn){

	/**Creation des variables **/
	AUTOMATEAFD afd; //automate que l'on retourne

	unsigned int i,j,k,z,y,m;
	unsigned int tailleEtat=1;
	
	unsigned int etatDejaVu = 0; //0 = jamais vu

	/** Creation des tableaux etats et transitions **/
	//allocation de transitions
	TRANSITIONDETERMINISTE** transitions = (TRANSITIONDETERMINISTE **)malloc(sizeof(TRANSITIONDETERMINISTE*)*tailleEtat);  //lignes

	for (i=0;i<tailleEtat;i++){
		transitions[i] = (TRANSITIONDETERMINISTE*)malloc(sizeof(TRANSITIONDETERMINISTE)*strlen(afn.Z)); //colonnes

		for(j=0;j<strlen(afn.Z);j++){
			transitions[i][j].tailleArrivee = 1;
			transitions[i][j].arrivee = (int*)malloc(sizeof(int)*transitions[i][j].tailleArrivee);
			//init a -1 la premiere case d'arrivee de chaque transitions
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
	etats[0].colonne[0]=0; //On commence avec l'etat de depart

	for(i=0;i<tailleEtat;i++){	//Pour toutes les cases de etats
		for(j=0;j<etats[i].tailleColonne;j++) //Pour tous les elements de chaque cases de etats
		{
			for(z=0;z<strlen(afn.Z);z++){		//Pour toutes les lettres de l'alphabet
				transitions[i][z].depart = etats[i].colonne[j]; //on prend le premier etat de etats
				transitions[i][z].caractere = afn.Z[z];			//on prend le premier caractere de Z
			
				for(k=0;k<afn.tailleD;k++){		//Pour toutes les transitions de l'afn
					if(afn.D[k].depart == transitions[i][z].depart &&	//Si la transition afn.D[k] part du meme etat que l'etat[i].colonne[j] que l'on evalue
						afn.D[k].caractere == transitions[i][z].caractere){	//Si la transition afn.D[k] a le meme caractere que celui evalue actuellement (afn.Z[z])

						//On ajoute l'etat d'arrivee dans le tableau transition s'il n'y est pas déja
						if(verif_ajout_non_present(transitions[i][z].arrivee, transitions[i][z].tailleArrivee, afn.D[k].arrivee)){
							// On realloue le bon nombre de case dans les transitions pour pouvoir en rajouter une
							if(transitions[i][z].tailleArrivee!=1){ //la 1ere case est alloué de base, pour les autres il faut les allouer avant d'ajouter l'etat
								//realloue arrivee transitions
								transitions[i][z].arrivee = (int*)realloc(transitions[i][z].arrivee,sizeof(int)*transitions[i][z].tailleArrivee);
								transitions[i][z].arrivee[transitions[i][z].tailleArrivee]=-1;
							}
							//ajout de l'etat
							transitions[i][z].arrivee[transitions[i][z].tailleArrivee-1] = afn.D[k].arrivee;
							//on incremente la taille du tableau arrivee pour pouvoir ajouter un nouvel element
							transitions[i][z].tailleArrivee++; //On prévoit le prochain ajout
						}
					}
				}
				//On a regardé toutes les transitions depuis l'etat qu'on evalue
				//On tri nos cases pour pouvoir verifier plus facilement si on l'a deja dans nos etats
				tri(transitions[i][z].arrivee,transitions[i][z].tailleArrivee-1);

				//On ajoute lorsque l'on a fini d'etudier chaque elements de la case etats[i] donc tous les j
				if(j==etats[i].tailleColonne-1){ 	
					/** traitement de l'ajout de la case transition[i][z] dans la case etats[i]**/
					if(transitions[i][z].arrivee[0]!=-1){ //Si le premier element de transition vaut -1, on a pas trouvé d'etat d'arrivee

						//On regarde si l'element que l'on veut ajouter n'est pas deja dans nos etats
						etatDejaVu = 0; //0 = jamais vu
						for(y=0;y<tailleEtat;y++){	//Pour tous les etats de etats
							//on compare la taille de la case etats[y] avec la taille de la case transitions qu'on veut ajouter
							//si la taille n'est pas la meme ils sont forcement differents
							if(etats[y].tailleColonne == (transitions[i][z].tailleArrivee-1)){ //Si taille egale 
								for(m=0;m<etats[y].tailleColonne;m++) //Pour tous les elements de chaque case de etats[y]
								{	
									if(etats[y].colonne[m]==transitions[i][z].arrivee[m]){ //si l'element m de etats[y].colonne[m] est le meme 
																						   //que l'element m de la transition que l'on veut ajouter
										if(m+1==transitions[i][z].tailleArrivee-1){
											etatDejaVu = 1;
										}
									}else{
										break; //termine la for sur m --> on passe a y+1
									}
								}
							}
						}
						//S'il la case n'est pas deja dans nos etats
						if(!etatDejaVu){
							//On ajoute la case de transition[i][z] a la prochaine case de etats[]
							tailleEtat++; 

							//On alloue la nouvelle case de etats
							etats = (TABETATS*)realloc(etats,sizeof(TABETATS)*tailleEtat);
							//on donne a la nouvelle case la taille de la case de transition que l'on va ajouter
							etats[tailleEtat-1].tailleColonne=transitions[i][z].tailleArrivee-1; //on a tailleArrivee qui a une case supplementaire (pour un prochain ajout)

							//On alloue sa colonne
							etats[tailleEtat-1].colonne=(unsigned int*)malloc(sizeof(unsigned int)*etats[tailleEtat-1].tailleColonne);

							//on alloue la nouvelle colonne de transitions corespondant a l'etude du nouvel etat
							transitions = (TRANSITIONDETERMINISTE **)realloc(transitions,sizeof(TRANSITIONDETERMINISTE*)*tailleEtat);  //lignes
							transitions[tailleEtat-1] = (TRANSITIONDETERMINISTE*)malloc(sizeof(TRANSITIONDETERMINISTE)*strlen(afn.Z)); //colonnes

							//allocations du tableau arrivee de la nouvelle case
							for(y=0;y<strlen(afn.Z);y++){
								transitions[tailleEtat-1][y].tailleArrivee = 1;
								transitions[tailleEtat-1][y].arrivee = (int*)malloc(sizeof(int)*transitions[tailleEtat-1][y].tailleArrivee);
								//init a -1 des transitions 
								transitions[tailleEtat-1][y].arrivee[0] = -1;
							}
									
							//on ajoute les transitions d'arrivees dans la nouvelle case
							for (y=0;y<etats[tailleEtat-1].tailleColonne;y++){
								//ajout des arrivee dans le nouvel etat
								etats[tailleEtat-1].colonne[y]=transitions[i][z].arrivee[y];
							}
						}
					}else{
						transitions[i][z].tailleArrivee++;
					}	
				}	
			}			
		}			
	}

	/**Permet d'afficher les etats obtenus via la determinisation**/
	//affiche_determinisation(etats, tailleEtat);

	/**Permet d'afficher les transitions obtenus via la determinisation**/
	//affiche_transitions(transitions,tailleEtat, strlen(afn.Z));


	/**Remplissage de l'AFD **/
	//Creation de Q
	afd.tailleQ = tailleEtat;
	//allocation de Q
	afd.Q = (unsigned int*)malloc(sizeof(unsigned int)*afd.tailleQ);
	//Remplissage de Q
	for(i=0;i<afd.tailleQ;i++){
		afd.Q[i]=i;
	}

	//Creation de Z, l'alphabet est inchangé
	afd.Z=(char*)malloc(sizeof(char)*strlen(afn.Z));
	strcpy(afd.Z,afn.Z);

	//Creation de s, l'etat initial est le même
	afd.s=afn.s; //=0

	//Creation de F
	//Un etat i de afd est final si l'un de ses etats dans afn l'etait
	int dejaPresent=0;

	//Allocation de F au maximum
	afd.F = (int*)malloc(sizeof(int)*afn.tailleQ);
	afd.tailleF=0;
	//Initialisation de F a -1 (au cas ou letat initial 0 est final)
	for (i = 0; i < afn.tailleQ; i++)
	{
		afd.F[i]=-1;
	}

	//On cherche a savoir si les nouveaux etats sont finaux
	for(i=0;i<tailleEtat;i++){//pour toutes les cases de etats
		
		for(j=0;j<etats[i].tailleColonne;j++){//pour tous les etats de chaque cases etats
			for(k=0;k<afn.tailleF;k++){ //pour tous les etats accepteurs de afn
				//si un etat de etats[i].colonne est final alors i est final dans afd
				if(etats[i].colonne[j]==afn.F[k]){
					//On regarde si l'etat i est deja final (appartient aux etats finaux de afd)
					for (m = 0; m < afd.tailleF; m++)
					{
						if(i==afd.F[m]){
							dejaPresent=1;
							break;
						}
					}
					//on ajoute l'etat s'il nest pas deja dans afd.F
					if (!dejaPresent){
						afd.F[afd.tailleF]=i;
						afd.tailleF++;
					}
				}
			}
		}
		dejaPresent=0;
	}

	//On realloue F a la bonne taille
	afd.F = (int*)realloc(afd.F,sizeof(int)*afd.tailleF-1);
			
	//Creation de Delta
	//On va lire toutes les transitions dans le tableau de transitions et changer les tableaux d'arrivé par l'etat qui correspond
	//On alloue D au maximum : strlen(afn.Z)*tailleEtat
	afd.Delta = (TRANSITION*)malloc(sizeof(TRANSITION)*(strlen(afn.Z)*tailleEtat));
	afd.tailleDelta = 0;

	for(i=0;i<tailleEtat;i++){  //pour tous les etats
		for(j=0;j<strlen(afn.Z);j++){	//pour tous les caracteres
			afd.Delta[afd.tailleDelta].depart = afd.Q[i]; 	//le depart est l'etat que l'on regarde dans nos nouveaux etats Q
			afd.Delta[afd.tailleDelta].caractere = transitions[i][j].caractere;

			//on cherche quel etat dans nos nouveaux etats, representent les arrivees  
			for(k=1;k<tailleEtat;k++){ //Dans toutes les cases de etats sauf la premiere 
				//on verifie que la premiere case ne contient pas -1
				//on compare la taille des cases
				if(transitions[i][j].tailleArrivee-1 == etats[k].tailleColonne){
					etatDejaVu = 0; //0 = jamais vu
					//On compare chaque elements entre eux
					for(y=0;y<etats[k].tailleColonne;y++){ //Dans tous les etats de chaque case
					
						if(transitions[i][j].arrivee[y]==etats[k].colonne[y]){
							if(y+1==transitions[i][j].tailleArrivee-1){
								etatDejaVu = 1;
								afd.Delta[afd.tailleDelta].arrivee = afd.Q[k];
								afd.tailleDelta++;
							}
						}else{
							break; //termine la for sur y --> on passe a k+1
						}
					}
				}
			}	
		}
	}

	//reallocation de Delta la bonne taille
	afd.Delta = (TRANSITION*)realloc(afd.Delta,sizeof(TRANSITION)*afd.tailleDelta);

	/**free des tableaux **/
	//free de transitions
	
	for (i=0;i<tailleEtat;i++){
		for(j=0;j<strlen(afn.Z);j++){
			free(transitions[i][j].arrivee);			
		}
		free(transitions[i]);			
	}
	free(transitions);
	
	//free des etats	
	for (i=0;i<tailleEtat;i++){
		free(etats[i].colonne);		
	}
	free(etats);

	return afd;
}

unsigned int verifMot(AUTOMATEAFD afd, char* mot){
    unsigned int etatinit = afd.s;
    unsigned int etatActuel = -1;
    unsigned i,j;

    for(i=0;i<strlen(mot);i++)
    {
        //On regarde si on a une transition qui commence par etatInit et qui a pour caractere mot[i]
        for(j=0;j<afd.tailleDelta;j++)
        {
            if((etatinit == afd.Delta[j].depart)&&(afd.Delta[j].caractere == mot[i]))
            {
                etatActuel = afd.Delta[j].arrivee;
            }
        }

        //On vérifie si on est sur le dernier caractere de mot et si on est sur un etat final
        if(i==strlen(mot)-1)
        {
            for(j=0;j<afd.tailleF;j++)
            {
                if(etatActuel == afd.F[j])
                {
                    return 1;
                }
            }
        }

        //Sinon on a trouvé une transition et on continue la traitement
        if(etatActuel != -1)
        {
            etatinit = etatActuel;
            etatActuel =-1;
        }
        else //sinon on renvoie 0, on ne peut pas lire le mot
        {
            return 0;
        }

    }

    //si mot vide
    return 0;
}

AUTOMATEAFD minimisation(AUTOMATEAFD afd){
	/**Creation des variables **/

	AUTOMATEAFD afdMin; //automate minimise renvoye

	int** transitionsTmp;
	int* bilan1;
	int* bilan2;
	int i,j,k,m;
	int bilanEgaux=0;
	int final;
	int transitionPossible = 0;
	int arriveeTransition = -1;
	int indexBilan=0;
	int compteurComparaison=0;
	int indexF=0;
	int* etatsTmp;
	int indexEtatsTmp;
	int present=0;
	int* suiviBilan;
	int dejaTraite;
	int indexDelta=0;

	//Allocation des variables temporaires
	transitionsTmp = (int**)malloc(sizeof(int*)*strlen(afd.Z));
	for(i=0;i<strlen(afd.Z);i++){
		transitionsTmp[i] = (int*)malloc(sizeof(int)*afd.tailleQ);
	}
	bilan1 = (int*)malloc(sizeof(int)*afd.tailleQ);
	bilan2 = (int*)malloc(sizeof(int)*afd.tailleQ);

	suiviBilan = (int*)malloc(sizeof(int)*afd.tailleQ);

	//Init du premier bilan
	for(i=0;i<afd.tailleQ;i++){
		//On regarde si l'element appartient aux etats finaux
		final=0;
		for(j=0;j<afd.tailleF;j++){
			if(i==afd.F[j]){
				final=1;
				break;
			}
		}

		if(final){//on met 1 dans bilan[i]
			bilan1[i]=1;
		}else{//on met 0
			bilan1[i]=0;
		}

	}

	//Tant que les bilans ne sont pas egaux on continue
	while(!bilanEgaux){
		//On rempli transitionsTmp
		for(i=0;i<strlen(afd.Z);i++){ //pour tous les caracteres 
			for(j=0;j<afd.tailleQ;j++){	//Pour tous les etats
				//on cherche la transition qui part de i avec le caractere j, si elle existe
				transitionPossible=0;
				for(k=0;k<afd.tailleDelta;k++){
					if(afd.Delta[k].depart==j && afd.Delta[k].caractere==afd.Z[i]){
						arriveeTransition = afd.Delta[k].arrivee;
						transitionPossible=1;
					}
				}
				//On verifie qu'il existe une transitions de i avec le caractere lu
				if(transitionPossible){
					//On ajoute dans transitionsTmp[i][j], le resultat du dernier bilan pour l'arrivee de l'etat i
					transitionsTmp[i][j]=bilan1[arriveeTransition];

				}else{
					transitionsTmp[i][j]=-1;
				}
			}
		}
		
		//Init de bilan 2 a -1
		for(i=0;i<afd.tailleQ;i++){//Pour toutes les cases de bilan et celle de transitionsTmp
			bilan2[i]=-1;
		}

		//On rempli bilan2
		indexBilan=0;
		//On cherche les cases qui ont leur etat dans bilan1 egaux, ainsi que leurs etats egaux pour chaque transitions
		for(i=0;i<afd.tailleQ;i++){//Pour toutes les cases de bilan2
			//si elle n'est pas encore rempli
			if(bilan2[i]==-1){
				//on met la premiere case a indexBilan
				bilan2[i]=indexBilan;

				//On cherche les autres cases qui auront le meme index
				for(j=i+1;j<afd.tailleQ;j++){	//Pour tous les etats
					//Si les etats sont les memes
					if(bilan1[i]==bilan1[j]){
						//On regarde si les cases de transitions sont identiques
						for(k=0;k<strlen(afd.Z);k++){
							//Si une transition n'est pas la meme
							if(transitionsTmp[k][i]!=transitionsTmp[k][j]){
								break; //on arrete la for sur k
							}

							if(k==strlen(afd.Z)-1){
								//On met a la case j le meme index que i
								bilan2[j]=indexBilan;
							}
						}
					}
				}
			indexBilan++;
			}
		}

		bilanEgaux=1;
		//Comparaison des bilans
		compteurComparaison++;
		for(i=0;i<afd.tailleQ;i++){ //Pour tous les etats
			if(bilan1[i]!=bilan2[i]){//un element ne correspond pas
				bilanEgaux=0;
				//On met bilan2 dans bilan1
				for(j=0;j<afd.tailleQ;j++){
					bilan1[j]=bilan2[j];
				}
				break;
			}
		}
		
	}
	
	/** Remplissage de afdMin **/ 
	if(compteurComparaison==1){ //Si l'afd en entrée est deja minimisé on renvoie sa copie (si les bilans sont egaux)
		afdMin=copie_afd(afd);
	}else{ //Sinon on va le recreer a partir du bilan

		//Creation de Q
		//Allocation de Q
		afdMin.tailleQ = indexBilan; //a chaque ajout d'un etat dans le bilan on incremente indexBilan
		afdMin.Q = (unsigned int*)malloc(sizeof(unsigned int)*afdMin.tailleQ);
		for(i=0;i<afdMin.tailleQ;i++){
			afdMin.Q[i]=i;
		}

		//Creation de Z
		//Allocation de Z
		afdMin.Z = (char*)malloc(sizeof(char)*strlen(afd.Z));
		//L'alphabet est inchange
		strcpy(afdMin.Z,afd.Z);

		//Creation de s (inchange)
		afdMin.s = afd.s;

		//Creation de F
		//Allocation de F (au maximum on a autant d'etats accepteurs qu'avant la minimisation)
		afdMin.tailleF = afd.tailleF;
		afdMin.F = (int*)malloc(sizeof(int)*afdMin.tailleF);

		//Ce tableau nous permettra de repertorier les anciens etats qui sont rassemble dans un unique etat 
		etatsTmp=(int*)malloc(sizeof(int)*afd.tailleQ);
		
		//init du tableau suiviBilan a 0
		//Ce tableau nous permettra de savoir quels etats nous avons traite dans le bilan (utile si plusieurs cases ont le meme etat)
		//0:non traitee et 1: traitee
		for(i=0;i<afd.tailleQ;i++){
			suiviBilan[i]=0;
		}

		//on recherche tous les etats du bilans qui sont egaux:
		for(i=0;i<afd.tailleQ;i++){
			if(suiviBilan[i]==0){ //si la case du bilan n'a pas ete traitee
				
				//init de etatsTmp a -1
				for(j=0;j<afd.tailleQ;j++){
					etatsTmp[j]=-1;
				}
				indexEtatsTmp=0;
				//on ajoute l'etat de la case bilan qu'on regarde
				etatsTmp[indexEtatsTmp]=afd.Q[i];
				indexEtatsTmp++;
				//On recherche ceux qui sont egaux a cet etat
				for(j=i+1;j<afd.tailleQ;j++){
					present=0;
					if(bilan2[i]==bilan2[j]){
						//On regarde si l'etat est deja dans etatsTmp
						for(k=0;k<afd.tailleQ;k++){
							if(afd.Q[j]==etatsTmp[k]){
								present=1;
							}
						}
						//on ajoute l'etat au tableau s'il n'y est pas
						if(!present){
							etatsTmp[indexEtatsTmp]=afd.Q[j];
							indexEtatsTmp++;
						}
					}	
				}

				//On regarde pour tous les anciens etats repertories dans etatsTmp, si l'un deux est final
				final=0;
				for(j=0;j<indexEtatsTmp;j++){
					//On note les etats qu'on traite
					suiviBilan[etatsTmp[j]]=1;

					for(m=0;m<afd.tailleF;m++){

						if(etatsTmp[j]==afd.F[m]){
							final=1;
							break;
						}
					}
				}

				//Si l'un des etats est final
				if(final){ //on met l'etat correspondant dans les etats finaux
					afdMin.F[indexF]=bilan2[i];
					indexF++;
				}
			}	
		}

		//On realloue afdMin.F a la bonne taille
		afdMin.tailleF = indexF;
		afdMin.F = (int*)realloc(afdMin.F,sizeof(int)*afdMin.tailleF);

		//Creation de Delta
		//On alloue delta au maximum
		afdMin.tailleDelta = afd.tailleDelta;
		afdMin.Delta = (TRANSITION*)malloc(sizeof(TRANSITION)*afdMin.tailleDelta);
		
		for(i=0;i<afd.tailleQ;i++){//Pour toutes les cases du bilan, on va ecrire les transitions de transitionsTmp 
			//si on change n'a pas encore traite cet etat (pour ne pas ecrire plusieurs fois les memes)
			dejaTraite=0;
			for(j=0;j<i;j++){
				if(bilan2[i]==bilan2[j]){ //si on trouve la meme valeur dans le bilan
					dejaTraite=1;
					break;
				}
			}
			if(!dejaTraite){
				//On regarde les transitions de cet etat
				for(j=0;j<strlen(afd.Z);j++){

					if(transitionsTmp[j][i] !=-1){//Si on a une transition
						//On ajoute cette transition
						afdMin.Delta[indexDelta].depart = bilan2[i];
						afdMin.Delta[indexDelta].caractere = afd.Z[j];
						afdMin.Delta[indexDelta].arrivee = transitionsTmp[j][i];
						indexDelta++;						
					}
				}
			}
		}

		//On realloue Delta a la bonne taille
		afdMin.tailleDelta = indexDelta;
		afdMin.Delta = (TRANSITION*)realloc(afdMin.Delta,sizeof(TRANSITION)*afdMin.tailleDelta);
	}

	/** free des tableaux **/
	//Liberation des variables temporaires
	for(i=0;i<strlen(afd.Z);i++){
		free(transitionsTmp[i]);
	}

	free(transitionsTmp);
	free(bilan1);
	free(bilan2);
	free(suiviBilan);
	free(etatsTmp);

	return afdMin;
}

/** Fonctions Annexes **/
//Afficher un automate standard deterministe
void AfficherAutomateDeterministe(AUTOMATEAFD afd){
	int i=0;
	int tailleZ;

	//Q
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
void tri(int* T, int ligne){
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
		printf("[");

		for(j=0;j<etats[i].tailleColonne;j++){

			printf(" %d ",etats[i].colonne[j]);
		}
		printf("]");
	}
	printf("]\n");
}

//affiche transitions
void affiche_transitions(TRANSITIONDETERMINISTE** transitions,int tailleEtat, int tailleZ){
	int i,j,y;
	printf("/*** Transitions ***/\n");
	for (i=0;i<tailleEtat;i++){
		for (j=0;j<tailleZ;j++){
			printf("%c: ",transitions[i][j].caractere );
			for(y=0;y<transitions[i][j].tailleArrivee-1;y++){
				printf("%2.d ",transitions[i][j].arrivee[y]);
			}
			
		}
		printf("\n");

	}
}

//verifie si la valeur ajout est presente dans le tableau arrivee
int verif_ajout_non_present(int* arrivee, int tailleArrivee, int ajout){
	int i;
	int verif=1;

	for (i=0;i<tailleArrivee;i++){
		if(arrivee[i] == ajout){
			verif=0;
			break;
		}
				
			
	}

	return verif;
}

//Copie d'un automate fini deterministe
AUTOMATEAFD copie_afd(AUTOMATEAFD afd){
	AUTOMATEAFD afd_copie;
	int i;

	//copie de Q
	afd_copie.tailleQ = afd.tailleQ;
	afd_copie.Q = (unsigned int*)malloc(sizeof(unsigned int)*afd_copie.tailleQ);
	
	for (i=0;i<afd_copie.tailleQ;i++){
		afd_copie.Q[i] = afd.Q[i];
	}

	//copie de Z
	afd_copie.Z = (char*)malloc(sizeof(char));
	strcpy(afd_copie.Z,afd.Z);

	//copie de s
	afd_copie.s = afd.s;

	//copie de F
	afd_copie.tailleF = afd.tailleF;
	afd_copie.F = (int*)malloc(sizeof(int)*afd_copie.tailleF);
	
	for (i=0;i<afd_copie.tailleF;i++){
		afd_copie.F[i] = afd.F[i];
	}
	//copie de Delta
	afd_copie.tailleDelta = afd.tailleDelta;
	afd_copie.Delta = (TRANSITION*)malloc(sizeof(TRANSITION)*afd.tailleDelta);

	for (i=0;i<afd_copie.tailleDelta;i++){
		afd_copie.Delta[i].depart = afd.Delta[i].depart;
		afd_copie.Delta[i].caractere = afd.Delta[i].caractere;
		afd_copie.Delta[i].arrivee = afd.Delta[i].arrivee;
	}

	return afd_copie;
}

//Libere le contenu d'un afd
void free_afd(AUTOMATEAFD afd){
	//Q
	free(afd.Q);

	//Z
	if(afd.Z!=NULL && strcmp(afd.Z,"")){ //on n'a pas alloue pour le langage vide et le mot vide
		free(afd.Z);
	}
	

	//F
	if(afd.F!=NULL){
		free(afd.F);
	}
	

	//Delta
	if(afd.Delta!=NULL){ //on n'a pas alloue pour le langage vide et le mot vide
		free(afd.Delta);
	}	
}
