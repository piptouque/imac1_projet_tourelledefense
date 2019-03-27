#ifndef __FONCTIONS_FICHIER_CARTE

#define __FONCTIONS_FICHIER_CARTE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>

#include "point.h"
#include "graphe.h"
#include "carte.h"

#include "commun.h"


/*Étapes de lecture*/
bool lireVersion(FILE *fichierCarte, int *version); /*Fonctionne pour l'instant uniquement pour des cartes de version 1*/
bool lireParametres(FILE *fichierCarte, int version, unsigned int *energie, char nomImage[],  unsigned char couleurClef[][NB_COULEURS]);
bool lireChemins(FILE *fichierCarte, int *nombreNoeuds, Graphe **chemins);
/**/

/*Lecture entière*/
bool lireCarte(FILE *fichierCarte, Carte *carte); /*avec structure Carte*/
/**/

MotClef correspondanceMotClef(char motClef[], int version);

/*Fonction de formatage*/
void sautLigne(FILE *fichierCarte);
/*Lecture de fichier avec vérif de formatage*/
int lireEntier(FILE *fichierCarte, char position[],int ligne, int *e);
int lireChaine(FILE *fichierCarte, char position[],int ligne, char chaine[]);

#endif