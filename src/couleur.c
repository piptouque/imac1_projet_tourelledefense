#include "../include/couleur.h"

bool comparerCouleurs(unsigned char couleur1[], unsigned char couleur2[])
{
	int c;
	for(c=0; c<NB_COULEURS; c++)
	{
		if( couleur1[c] != couleur2[c] )
			return false;
	}
	return true;
}