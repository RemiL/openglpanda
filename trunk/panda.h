#ifndef __panda_h__
#define __panda_h__

#include "common.h"

typedef struct
{
  t_coordonnees position;
  t_coordonnees direction;
} t_panda;

GLuint faire_tete_panda();
GLuint faire_corps_panda();
GLuint faire_cuisse_panda();
GLuint faire_mollet_panda();

#endif /* __panda_h__ */
