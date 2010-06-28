/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#ifndef __collision_h__
#define __collision_h__

#include "common.h"
#include "heightmap.h"

typedef struct
{
  t_coordonnees position;
  float largeur;
  float longueur;
} AABB;

typedef struct
{
  t_coordonnees point[4];
} t_rectangle;

typedef struct
{
  t_coordonnees centre;
  double rayon;
} t_cercle;

void actualisation_englobant(t_rectangle *r, double x, double y, double angle);
int collision_segment_cercle(t_coordonnees A, t_coordonnees B, t_cercle cercle);
int collision_rectangle_cercle(t_rectangle *r, t_cercle cercle);
int collision_bambous(t_rectangle *r, t_cercle *cercles, int nb_cercles);
int collisionTerrain(AABB box, char pHeightMap[]);
int deplacement_camera_valide(t_coordonnees p);

#endif /* __collision_h__ */
