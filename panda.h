/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

R�mi LACROIX et Nicolas POIRIER
******************************************************************************/
#ifndef __panda_h__
#define __panda_h__

#include "common.h"
#include "collisions.h"
#include "camera.h"

typedef struct
{
  t_coordonnees position;
  t_coordonnees direction;
  t_coordonnees direction_normal;
  t_coordonnees direction_initial;
  t_coordonnees direction_normal_initial;
  t_coordonnees vecteur_def_vertical;
  double angle;
  t_rectangle englobant;
} t_panda;

GLuint faire_tete_panda();
GLuint faire_corps_panda();
GLuint faire_cuisse_panda();
GLuint faire_mollet_panda();
void init_panda();
void panda_actualiser_position();

#endif /* __panda_h__ */
