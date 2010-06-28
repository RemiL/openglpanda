/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#include "panda.h"

extern t_panda panda;

GLuint faire_tete_panda()
{
  GLuint Ma_Tete = glGenLists(7);
  GLuint Ma_Tache = Ma_Tete + 1;
  GLuint Mon_Oeil = Ma_Tache + 1;
  GLuint Ma_Pupille = Mon_Oeil + 1;
  GLuint Mon_Oreille = Ma_Pupille + 1;
  GLuint Mon_Museau = Mon_Oreille + 1;
  GLuint Mon_Nez = Mon_Museau + 1;
  
  glNewList(Ma_Tache, GL_COMPILE);
    glColor3f(0.0, 0.0, 0.0);
    glScalef(0.3, 1.2, 1.6);
    glutSolidSphere(0.8, 50, 50);
  glEndList();
  
  glNewList(Mon_Oeil, GL_COMPILE);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(0.3, 1.3, 1.3);
    glutSolidSphere(0.4, 50, 50);
  glEndList();
  
  glNewList(Ma_Pupille, GL_COMPILE);
    glColor3f(0.0, 0.0, 0.0);
    glScalef(0.3, 1.2, 1.2);
    glutSolidSphere(0.2, 50, 50);
  glEndList();
  
  glNewList(Mon_Oreille, GL_COMPILE);
    glColor3f(0.0, 0.0, 0.0);
    glScalef(0.2, 1, 1);
    glutSolidSphere(1, 50, 50);
  glEndList();
  
  glNewList(Mon_Museau, GL_COMPILE);
    glColor3f(1.0, 1.0, 1.0);
    glScalef(1.5, 1.8, 1.5);
    glutSolidSphere(1, 50, 50);
  glEndList();
  
  glNewList(Mon_Nez, GL_COMPILE);
    glColor3f(0.0, 0.0, 0.0);
    glScalef(0.5, 1, 0.8);
    glutSolidSphere(0.2, 50, 50);
  glEndList();
  
  glNewList(Ma_Tete, GL_COMPILE);
    // La tête en elle même
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere(3.5, 50, 50);
    
    // Yeux
    glPushMatrix();
      glTranslatef(3.1, 0.0, 0.3);
      glPushMatrix();
        glTranslatef(0.0, 1.15, 0.0);
        glRotatef(30, 1, 0, 0);
        glRotatef(22, 0, 0, 1);
        glRotatef(8, 0, 1, 0);
        glCallList(Ma_Tache);
        glTranslatef(0.7, 0.0, 0.0);
        glCallList(Mon_Oeil);
        glTranslatef(0.5, 0.0, 0.0);
        glCallList(Ma_Pupille);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(0.0, -1.15, 0.0);
        glRotatef(-30, 1, 0, 0);
        glRotatef(-22, 0, 0, 1);
        glRotatef(8, 0, 1, 0);
        glCallList(Ma_Tache);
        glTranslatef(0.7, 0.0, 0.0);
        glCallList(Mon_Oeil);
        glTranslatef(0.5, 0.0, 0.0);
        glCallList(Ma_Pupille);
      glPopMatrix();
    glPopMatrix();
    
    // Oreilles
    glPushMatrix();
      glTranslatef(0, 0.0, 2.7);
      glPushMatrix();
        glTranslatef(0.0, 2.5, 0.0);
        glCallList(Mon_Oreille);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(0.0, -2.5, 0.0);
        glCallList(Mon_Oreille);
      glPopMatrix();
    glPopMatrix();
    
    // Museau
    glPushMatrix();
      glTranslatef(2.4, 0.0, -1.3);
      glCallList(Mon_Museau);
      glTranslatef(1.0, 0.0, 0.0);
      glCallList(Mon_Nez);
    glPopMatrix();
  glEndList();
  
  return Ma_Tete;
}

GLuint faire_corps_panda()
{
  GLuint Mon_Corps = glGenLists(1);
  
  glNewList(Mon_Corps, GL_COMPILE);
    glPushMatrix();
      glColor3f(1.0, 1.0, 1.0);
      glScalef(1, 0.75, 0.75);
      glutSolidSphere(5, 50, 50);
    glPopMatrix();
    glPushMatrix();
      glColor3f(0.0, 0.0, 0.0);
      glTranslatef(1.6, 0, 0);
      glScalef(0.6, 0.75, 0.75);
      glutSolidSphere(4.8, 50, 50);
    glPopMatrix();
    glPushMatrix();
      glColor3f(0.0, 0.0, 0.0);
      glTranslatef(-4.8, 0, 1.8);
      glScalef(0.7, 0.7, 1);
      glutSolidSphere(1, 50, 50);
    glPopMatrix();
  glEndList();
  
  return Mon_Corps;
}

GLuint faire_cuisse_panda()
{
  GLuint Ma_Cuisse = glGenLists(1);
  
  glNewList(Ma_Cuisse, GL_COMPILE);
    glPushMatrix();
      glColor3f(0.0, 0.0, 0.0);
      glScalef(0.5, 0.5, 1);
      glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
  glEndList();
  
  return Ma_Cuisse;
}

GLuint faire_mollet_panda()
{
  GLuint Mon_Mollet = glGenLists(1);
  
  glNewList(Mon_Mollet, GL_COMPILE);
    glPushMatrix();
      glColor3f(0.0, 0.0, 0.0);
      glScalef(0.5, 0.5, 1);
      glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
  glEndList();
  
  return Mon_Mollet;
}

void init_panda()
{
  panda.position.x = -60;
  panda.position.y = 0;
  panda.position.z = 0;
  panda.direction_initial.x = panda.direction.x = 1;
  panda.direction_initial.y = panda.direction.y = 0;
  panda.direction_initial.z = panda.direction.z = 0;
  panda.direction_normal_initial.x = panda.direction_normal.x = 0;
  panda.direction_normal_initial.y = panda.direction_normal.y = -1;
  panda.direction_normal_initial.z = panda.direction_normal.z = 0;
  panda.vecteur_def_vertical.x = 0;
  panda.vecteur_def_vertical.y = 0;
  panda.vecteur_def_vertical.z = 1;
  
  panda.englobant.point[0].x = panda.position.x + 2.5;
  panda.englobant.point[0].y = panda.position.y + 2;
  panda.englobant.point[1].x = panda.position.x + 2.5;
  panda.englobant.point[1].y = panda.position.y - 2;
  panda.englobant.point[2].x = panda.position.x - 6;
  panda.englobant.point[2].y = panda.position.y - 2;
  panda.englobant.point[3].x = panda.position.x - 6;
  panda.englobant.point[3].y = panda.position.y + 2;
}

void panda_actualiser_position()
{
  // Rotation par rapport au vecteur définissant la verticale de la caméra
  addition_vectorielle(&panda.direction,
                       cos(panda.angle), panda.direction_initial,
                       sin(panda.angle), panda.direction_normal_initial);
  // On recalcule le nouveau vecteur normal
  produit_vectoriel(&panda.direction_normal, panda.direction, panda.vecteur_def_vertical);
  
  // Déplacement englobant
  actualisation_englobant(&panda.englobant, panda.position.x, panda.position.y, panda.angle);

  // On met à jour l'affichage
  glutPostRedisplay();
}

void actualiser_AABB()
{
  /*panda.box.largeur = panda.largeur * cos(PI/2.0 - panda.angle) + longeur * cos(panda.angle);
  panda.box.longeur = panda.largeur * cos(PI/2.0 - panda.angle) + longeur * cos(panda.angle);
  panda.box.position = panda.position;*/
}