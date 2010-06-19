#include "bambou.h"

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
    // La t�te en elle m�me
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