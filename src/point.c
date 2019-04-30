#include "../include/point.h"

Point* creerPoint(unsigned int x, unsigned int y)
{
	Point *point = malloc(sizeof(Point));
	if( !point )
	{
		printf("Échec d'allocation dynamique d'un point. (%u, %u) \n", x, y);
		exit(EXIT_FAILURE);
	}
	point->x = x;
	point->y = y;
	return point;
}

Point* copiePoint(Point *point)
{
	return ( creerPoint(point->x, point->y) ) ;
}

int calculerDistance(Point *p1, Point *p2)
{
	int distance;
	distance = (int)sqrt( (double)pow(((int)p2->x - (int)p1->x),2) + pow(((int)p2->y - (int)p1->y),2) );
	return distance;
}

Point* creerProjeteOrtho(Point *point, Point *segP1, Point *segP2)
{
	int deltaX = (int)segP2->x - (int)segP1->x;
	int deltaY = (int)segP2->y - (int)segP1->y;
	bool renverser = abs(deltaX) < abs(deltaY);
	double c = renverser ? deltaX/(double)deltaY : deltaY/(double)deltaX;
	double norme = sqrt( (double)pow(deltaY,2) + pow(deltaX,2) );
	double xProj = point->x*deltaX/norme;
	double yProj = point->y*deltaY/norme;
	if( renverser )
		xProj *= c;
	else
		yProj *= c;
	Point *projeteOrtho = creerPoint( (unsigned int)xProj, (unsigned int)yProj );
	return projeteOrtho;
}

int calculerDistanceSegment(Point *point, Point *segP1, Point *segP2)
{
	Point *projeteOrtho = creerProjeteOrtho(point, segP1, segP2);
	int distanceSegment = calculerDistance(point, projeteOrtho);
	libererPoint(projeteOrtho);
	return distanceSegment;
}

void libererPoint(Point *point)
{
	free(point);
}

ListePoint* allouerListePoint(int nombrePoints)
{
	int i;
	ListePoint *listePoint = malloc(nombrePoints * sizeof(Point*));
	if( !listePoint )
	{
		printf("Échec d'allocation dynamique d'une liste de points. (nombre de points : %d) \n", nombrePoints);
		exit(EXIT_FAILURE);
	}
	for(i=0; i<nombrePoints; i++)
	{
		listePoint[i] = creerPoint(0,0); /*par défaut*/
	}
	return listePoint;
}


void libererListePoint(int nombrePoints, ListePoint *listePoint, bool avecContenu )
{

	int i;
	if( avecContenu )
	{
			for(i=0; i<nombrePoints; i++)
		{
			free(listePoint[i]);
		}
	}
	free(listePoint);
}