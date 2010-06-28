/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#include "common.h"

int nb_aleatoire(int min, int max)
{
  int n = max - min;
  int partSize   = 1 + (n == RAND_MAX ? 0 : (RAND_MAX - n) / (n + 1));
  int maxUsefull = partSize * n + (partSize - 1);
  int draw;
  
  do {
      draw = rand();
  } while (draw > maxUsefull);
  
  return ((draw / partSize) + min);
}