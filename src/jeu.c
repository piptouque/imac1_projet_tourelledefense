#include "../include/jeu.h"

Jeu* creerJeu(unsigned char niveau, Joueur *joueur, Carte *carte, Cite *cite, Vague *chaine)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = niveau;
	jeu->joueur = joueur;
	jeu->carte = carte;
	jeu->cite = cite;
	jeu->chaine = chaine;

	return jeu;
}

Jeu* allouerJeu(void)
{
	Jeu *jeu = malloc( sizeof(Jeu) );
	if( !jeu )
	{
		printf("Jeu -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	jeu->niveau = 0;
	jeu->joueur = allouerJoueur();
	jeu->carte = allouerCarte();
	jeu->cite = allouerCite();
	jeu->chaine = NULL;
	jeu->image = NULL;

	/** on initialise les ressources de l'affichage ailleurs **/
	jeu->scene = NULL;
	jeu->arrierePlan = NULL;
	return jeu;
}

Joueur* allouerJoueur(void)
{
	Joueur *joueur = malloc(sizeof(Joueur));
	if( !joueur )
	{
		printf("Joueur -- Échec d'allocation dynamique !\n");
		exit(EXIT_FAILURE);
	}
	joueur->pointage = 0;
	joueur->argent = 0;
	joueur->memTouche = '\0';
	return joueur;
}

void libererJoueur(Joueur *joueur)
{
	free(joueur);
}

bool preparerJeu(Jeu *jeu, char *nomDonnees)
{
	/* nom et chemin des fichiers de la carte*/
	char cheminDonnees[MAX_TAILLE_CHEMIN_FICHIER];
	char cheminImage[MAX_TAILLE_CHEMIN_FICHIER];

	int nombreModif;

	/*** Chemin du fichier et Ouverture ***/
	printf("Chargement de la carte...");
	sprintf(cheminDonnees,"%s/%s",REP_DONNEES_CARTE, nomDonnees);
	FILE *fichierCarte = fopen(cheminDonnees,"r");
	if( !fichierCarte )
	{
		printf("Échec d'ouverture des données au chemin : %s\n",cheminDonnees);
		return false;
	}
	printf(" Fait !\n");
	/*Lecture du fichier carte*/
	printf("Lecture des données carte...");
	if( !lireCarte(fichierCarte, jeu->carte) )
		return false;
	fclose(fichierCarte); /*On ferme le fichier lu, plus besoin.*/
	printf(" Fait !\n");
	/*****/

	/*Affichage de la carte (console)*/
	/*afficherCarte(carte);*/

	/*** Chargement de l'image associée ***/
	printf("Chargement de l'image...");
	sprintf(cheminImage, "%s/%s", REP_DONNEES_IMAGE, jeu->carte->nomImage);
	FILE *fichierImage = fopen(cheminImage, "r");
	if( !fichierImage )
	{
		printf("Échec d'ouverture de l'image au chemin : %s\n", cheminImage );
		return false;
	}
	printf(" Fait !\n");
	/*Lecture de l'image associée*/
	printf("Lecture des données image...");
	if( !PPM_lireImage(fichierImage, &(jeu->image) ) )
		return false;
	printf(" Fait ! \n");
	

	/** VÉRIFICATIONS **/
	printf("Vérification des chemins...");
	if( !validerChemins(jeu->carte, jeu->image, &nombreModif) )
	{
		printf("IMAGE INCORRECTE (modifications : %d)\n", nombreModif);
		printf("Ouverture de la modification...");
		sprintf(cheminImage, "%s/%s_mod", REP_DONNEES_IMAGE, jeu->carte->nomImage);
		FILE *fichierImageSortie = fopen(cheminImage, "w");
		if( !fichierImageSortie )
		{
			printf("Échec d'ouverture de l'image au chemin : %s\n", cheminImage );
			return false;
		}
		printf(" Fait !\n");
		/*Lecture de l'image associée*/
		printf("Écriture des données image...");
		if( !PPM_ecrireImage(fichierImageSortie, jeu->image ) )
			return false;
		fclose(fichierImageSortie);
		printf(" Fait !\n");
			return false;
	}
	printf(" Fait ! \n");
	/*** Chargement de l'image associée ***/
	return true;
}

void relancerJeu(Jeu *jeu)
{
	/** VAGUE DE MONSTRES **/
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);

	/** affichage **/
	////
}

void lancerJeu(Jeu *jeu)
{
	/** LANCER LE CONTEXTE OPENGL D'AFFICHAGE
	*** AVANT DE CHARGER LES RESSOURCES !!!
	***/
	/*** Pour une raison obscure, SDL_Init provoque une erreur de seg
	*** si on appelle lancerAffichage après le lancement de la vague
	*** depuis le passage à plusieurs sorties
	***/
	SDL_Surface *scene;
	lancerAffichage(&scene);
	jeu->scene = scene;
	/** VAGUE DE MONSTRES **/
	/* première itération */
	Vague *vague = creerVague(jeu->niveau, jeu->carte);
	jeu->chaine = vague;
	lancerVague(jeu->chaine, jeu->carte, jeu->cite);
	char *nomArrierePlan = jeu->carte->nomArrierePlan;
	bool possedeArrierePlan = jeu->carte->possedeArrierePlan;
	chargerRessourcesAffichage(jeu->arrierePlan, &(jeu->affichageArrierePlan),  &(jeu->textureArrierePlan), jeu->lutins, jeu->banqueAffichage, jeu->banqueTextures, jeu->listeDim, jeu->image->dim, possedeArrierePlan, nomArrierePlan);

}

void quitterJeu(Jeu *jeu)
{
	libererJeu(jeu);
}

void traitementJeu(Jeu* jeu, time_t deltaT)
{
	/* ici, on fait tout les calculs et changements,
	* la fonction est appelée à chaque itération du jeu
	*/
	/* les vagues de monstres */
	//printf("Who let the dogs out ?\n");
	//terminalMonstre(jeu->chaine->monstres[0]);
	traitementChaine(&(jeu->chaine), deltaT, jeu->carte, jeu->cite, &(jeu->niveau), &(jeu->joueur->pointage), &(jeu->joueur->argent));
	//printf("Woof ?\n");	
	/* la vague, les monstres ont été mis à jour.
	* On traite maintenant la cité.
	*/
	traitementCite(jeu->cite, deltaT, jeu->carte, jeu->chaine->monstres, jeu->chaine->nombreMonstres);
}

void boucleJeu(Jeu *jeu)
{
	bool boucle;
	clock_t deltaT, tempsDebut, tempsEcoule;
	Uint32 tempsDebut_SDL, tempsEcoule_SDL;

	/* temps au début de la boucle */
	tempsDebut = clock();
	tempsEcoule  = tempsDebut;
	/** BOUCLE **/
	boucle = true;
	while(boucle)
	{
		/*récup temps au début de la boucle*/
		tempsDebut_SDL = SDL_GetTicks();
		deltaT = clock() - tempsEcoule;
		tempsEcoule = clock();

		//printf("Delta T : %ld, temps écoulé : %lf \n", deltaT, deltaT/(double)CLOCKS_PER_SEC);
		/*** TRAITEMENT ***/
		traitementJeu(jeu, deltaT);
		
		//terminalVague(jeu->chaine);

		glClear(GL_COLOR_BUFFER_BIT);
		/** AFFICHAGE ***/

		afficherJeu(jeu);
		
		//afficherCarte();

		boucle = interfaceJeu(jeu);
		
		

		/*Échange du tampon arrière et avant -> mise à jour fenêtre*/
		SDL_GL_SwapBuffers();

		/** *ÉVÉNEMENTS -- INTERFACE  */
		// autre fonction

		/* Calcul du temps écoulé*/
		tempsEcoule_SDL = SDL_GetTicks() - tempsEcoule_SDL;
		/*Pause éventuell du programme si trop peu de temps écoulé*/
		if( tempsEcoule_SDL < FRAMERATE_MILLISECONDS)
		{
			SDL_Delay(FRAMERATE_MILLISECONDS - tempsEcoule_SDL);
		}
	}
	SDL_Quit();
}

bool interfaceJeu(Jeu *jeu)
{
	/* ÉVÉNEMENTS SDL */
	SDL_Event e;
	bool boucle = true;
	while(SDL_PollEvent(&e))
	{
		 /* L'utilisateur ferme la fenetre : */
		if(e.type == SDL_QUIT) 
		{
			boucle = false;
			break;
		}
	
		if(	e.type == SDL_KEYDOWN 
			&& (e.key.keysym.sym == SDLK_q || e.key.keysym.sym == SDLK_ESCAPE))
		{
			boucle = false; 
			break;
		}
		/* Quelques exemples de traitement d'evenements : */
        switch(e.type) 
        {
            /* Redimensionnement fenetre */
			case SDL_VIDEORESIZE:
                //reshape(&surface, e.resize.w, e.resize.h);
                //glMatrixMode(GL_MODELVIEW);
                break;

            /* Clic souris */
            case SDL_MOUSEBUTTONUP:
            	gestionClic(jeu, &e);
                //printf("clic en (%d, %d)\n", e.button.x, e.button.y);
                break;
            
            /* Touche clavier */
            case SDL_KEYDOWN:
                printf("touche pressée (code = %c)\n",e.key.keysym.sym);
            	 //terminalVague(jeu->chaine);
                //terminalListe(jeu->cite->listeTour);
            	gestionTouche(jeu, &e);
                break;
                
            default:
                break;
        }
	}
	return boucle;
}

void gestionTouche(Jeu *jeu, SDL_Event *e)
{
	/* on enregistre la touche pressée
	* dans la mémoire tampon du joueur
	*/
	jeu->joueur->memTouche = e->key.keysym.sym;
}

void gestionClic(Jeu *jeu, SDL_Event *e)
{
	int type;
	Point coordClique;
	calculerCoordonneesEchelle(&coordClique, e->button.x, e->button.y, jeu->image->dim);

	/* */
	type = toucheVersTypeTour(jeu->joueur->memTouche);
	if( type != -1)
	{
		if( construireTour(jeu->cite, jeu->carte->chemins, jeu->carte->nombreNoeuds, type, &coordClique) )
		{
			printf("Tour construite en ");
			afficherPoint(&coordClique);

		}
	}
}

void afficherJoueur(Joueur *joueur)
{
	afficherTouche(joueur->memTouche);
}

void afficherJeu(Jeu *jeu)
{
   /* l'arrière-plan s'il existe */
	if( jeu->carte->possedeArrierePlan )
	{
		 afficherCarte(jeu->affichageArrierePlan, jeu->image->dim);
	}
    //peut-être mettre en arguments le score et l'argent et changer le type d'argu que la fonction attend : afficher Carte(jeu->pointage, jeu->argent);
  	
  	/* on afficher la cité, la chaîne de monstres */
    afficherCite(jeu->cite, jeu->banqueAffichage, jeu->listeDim, jeu->image->dim);

    afficherChaine(jeu->chaine, jeu->banqueAffichage, jeu->listeDim, jeu->image->dim);

    /* le joueur, et les textes associés */
    afficherJoueur(jeu->joueur);
}

void libererJeu(Jeu *jeu)
{
	/* on libère tout */
	libererJoueur(jeu->joueur);
	libererCarte(jeu->carte);
	/* LIBÉRER LA CHAÎNE AVANT LA CITÉ
	* CAR LA CHAÎNE RÉINITIALISE LES CIBLES DES TOURS
	*/
	libererChaine(jeu->chaine, jeu->cite->listeTour);
	libererCite(jeu->cite);
	PPM_libererImage(jeu->image);
	/** l'affichage aussi **/
	libererRessourcesAffichage(jeu->arrierePlan, &(jeu->affichageArrierePlan), &(jeu->textureArrierePlan), jeu->lutins, jeu->banqueAffichage, jeu->banqueTextures);
	fermerAffichage(jeu->scene);
	free(jeu);
}


void debug(void)
{
	static int woof = 0;
	if( !(woof%6) )
		printf("\nWho let the dogs out ?\n");
	else
		printf("Woof\n");
	woof++;
}

int toucheVersTypeTour(char touche)
{
	int i;
	for( i=0; i<NB_TYPES_TOUR; i++ )
	{
		if( TOUCHES_TOUR[i] == touche )
			return (TypeTour) i;
	}
	return -1;
}