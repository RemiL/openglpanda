/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#ifndef __bambou_h__
#define __bambou_h__

#include "common.h"
#include "heightmap.h"

void faire_composantes_bambou();
void faire_bambou(GLuint liste, int hauteur, int nb_branches);
int faire_foret_bambous(int nb_bambous, int nb_varietes, char* pHeightMap);

#endif /* __bambou_h__ */
