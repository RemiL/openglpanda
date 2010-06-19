#ifndef __bambou_h__
#define __bambou_h__

#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void faire_composantes_bambou();
void faire_bambou(GLuint liste, int hauteur, int nb_branches);
int faire_foret_bambous(int nb_bambous, int nb_varietes);

#endif /* __bambou_h__ */
