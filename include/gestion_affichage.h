#ifndef __FONCTIONS_GESTION_AFFICHAGE
#define __FONCTIONS_GESTION_AFFICHAGE

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "couleur.h"
#include "point.h"
#include "monstre.h"
#include "tour.h"

/* Dimensions initiales et titre de la fenetre */
static const unsigned int LARGEUR_FENETRE = 800;
static const unsigned int HAUTEUR_FENETRE = 600;
static const char TITRE_FENETRE[] = "TOURELLE DÉFENSE !!";


/* Espace fenêtre virtuelle */
static const float GL_VUE_LARGEUR = 1.;
static const float GL_VUE_HAUTEUR = 1.;


/* Nombre de bits par pixel de la fenetre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;


#define NB_LUTINS_MONSTRE 2
#define NB_LUTINS_TOUR 4
#define NB_LUTINS NB_LUTINS_MONSTRE + NB_LUTINS_TOUR

#define BASE_TAILLE_TOUR 50 /* EN PIXELS */
#define BASE_TAILLE_MONSTRE 40 /* IDEM */

static const unsigned char COULEUR_PARDEFAUT[NB_COULEURS] = {MAX_VAL_COULEUR, MAX_VAL_COULEUR, MAX_VAL_COULEUR};

static const unsigned char COULEUR_MONSTRE[NB_TYPES_MONSTRE][NB_COULEURS] = { {COULEUR_PARDEFAUT[0],COULEUR_PARDEFAUT[1],COULEUR_PARDEFAUT[2]}, {0, 169, 122} };


typedef struct {
	enum { LUT_tour, LUT_monstre } nature;
	union
	{
		TypeTour typeTour;
		TypeMonstre typeMonstre;
	};
} TypeLutin;

static const char *REP_LUTIN = "images/lutin/";
static const char *CHEMIN_IMAGE_TOUR = "tour/";
static const char *CHEMIN_IMAGE_MONSTRE = "monstre/";

/** ATTENTION devra correspondre à l'ordre des TypeTour **/
static const char *NOM_IMAGE_TOUR[] = { "tourR", "tourV", "tourB", "tourJ"};

static const char *NOM_IMAGE_MONSTRE[] = {"virus"};

static const char EXTENTION[] = ".png";



SDL_Surface* lancerAffichage(void);
void fermerAffichage(SDL_Surface *scene);

void calculerCoordonneesVirtuelles(Point *coord, double *posX, double *posY, Dimensions *dimImage);
void calculerCoordonneesEchelle(Point *cood, int x, int y, Dimensions *dimImage);

/*** RESSOURCES D'AFFICHAGE ***/
/* remplir la liste d'affichage à partir des textures des lutins*/

void chargerRessourcesAffichage(SDL_Surface *lutins[], GLuint banqueAffichage[], GLuint banqueTextures[], Dimensions *dimImage);
void libererRessourcesAffichage(SDL_Surface *lutins[],  GLuint banqueAffichage[], GLuint banqueTextures[]);

void remplirListeType(TypeLutin listeType[]);
void remplirListeDimensions(Dimensions listeDim[], TypeLutin listeType[]);
void remplirBanqueAffichage(GLuint banqueAffichage[], GLuint banqueTextures[], TypeLutin listeType[], Dimensions listeDim[],  Dimensions *dimImage);

/* on chargera  toutes les textures deau début du programme */
void chargerToutesTexturesLutins(SDL_Surface *lutins[], GLuint banqueTextures[]);
SDL_Surface* chargerTextureLutin(GLuint idTexture, TypeLutin *type);

void libererToutesTexturesLutins(GLuint banqueTextures[]);
void libererToutesImagesLutins(SDL_Surface *lutins[]);
/*** ***/

/* le dessin même à partir des textures */
void  dessinerLutinEchelle(GLuint idTexture, TypeLutin *type, Dimensions *dimLutin);
void dessinerLutin(GLuint idTexture, TypeLutin *type);
/* */

/** correspondances **/
/* deux fonctions, liées par une relation de bijection réciproque */
TypeLutin correspondanceTypeLutin(int indice);
int correspondanceIndiceLutin(TypeLutin *type);
void correspondanceCheminLutin(char *cheminLutin, TypeLutin *type);

void redimensionner(SDL_Surface** surface, unsigned int largeur, unsigned int hauteur);


#endif