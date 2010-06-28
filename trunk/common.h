/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#ifndef __common_h__
#define __common_h__

#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define true 1
#define false 0

#define PI 3.1415926535898

typedef struct
{
  double x;
  double y;
  double z;
} t_coordonnees;

int nb_aleatoire(int min, int max);

#endif /* __common_h__ */
