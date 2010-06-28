#ifndef __panda_h__
#define __panda_h__

#include "common.h"

typedef struct
{
  t_coordonnees position;
  float largeur;
  float longeur;
} AABB;

typedef struct
{
  t_coordonnees position;
  t_coordonnees direction;
  t_coordonnees direction_normal;
  t_coordonnees direction_initial;
  t_coordonnees direction_normal_initial;
  t_coordonnees vecteur_def_vertical;
  double angle;
  AABB box;
} t_panda;

GLuint faire_tete_panda();
GLuint faire_corps_panda();
GLuint faire_cuisse_panda();
GLuint faire_mollet_panda();
void init_panda();
void panda_actualiser_position();

#endif /* __panda_h__ */
