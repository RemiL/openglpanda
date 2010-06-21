#include "bambou.h"

GLuint Mon_Morceau_Bambou;
GLuint Mon_Morceau_Bambou2;
GLuint Ma_Feuille;
GLuint Ma_Branche;
GLuint Ma_Branche_Feuille;
GLuint* Mes_Bambous;

void faire_composantes_bambou()
{
  GLUquadricObj* qobj; /*GLAPIENTRY*/

   // allocation d´une description de quadrique
  qobj = gluNewQuadric();
  // la quadrique est pleine 
  gluQuadricDrawStyle(qobj, GLU_FILL); 
  // les ombrages, s´il y en a, sont doux
  gluQuadricNormals(qobj, GLU_SMOOTH);
  
  Mon_Morceau_Bambou =  glGenLists(5);
  Mon_Morceau_Bambou2 = Mon_Morceau_Bambou + 1;
  Ma_Feuille = Mon_Morceau_Bambou2 + 1;
  Ma_Branche = Ma_Feuille + 1;
  Ma_Branche_Feuille = Ma_Branche + 1;

  glNewList(Mon_Morceau_Bambou, GL_COMPILE);
    glColor3f(0.69, 0.79, 0.38);
    gluCylinder(qobj, 0.2, 0.2, 3, 50, 1);
    gluDisk(qobj, 0, 0.2, 50, 1);
    glPushMatrix();
      glTranslatef(0, 0, 3);
      gluDisk(qobj, 0, 0.2, 50, 1);
    glPopMatrix();
  glEndList();

  glNewList(Mon_Morceau_Bambou2, GL_COMPILE);
    glColor3f(0.79, 0.89, 0.48);
    gluCylinder(qobj, 0.21, 0.21, 0.14, 50, 1);
    gluDisk(qobj, 0, 0.21, 50, 1);
    glTranslatef(0, 0, 0.14);
    gluDisk(qobj, 0, 0.21, 50, 1);
  glEndList();

  glNewList(Ma_Feuille, GL_COMPILE);
    glColor3f(0.32, 0.50, 0.07);
    glBegin(GL_POLYGON);
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(-0.15, 0.0, 0.25);
      glVertex3f(-0.3, 0.0, 0.5);
      glVertex3f(-0.0, 0.0, 3.0);
      glVertex3f(0.3, 0.0, 0.5);
      glVertex3f(0.15, 0.0, 0.25);
    glEnd();
  glEndList();

  glNewList(Ma_Branche, GL_COMPILE);
    glColor3f(0.69, 0.79, 0.38);
    gluCylinder(qobj, 0.04, 0.04, 5, 50, 1);
    gluDisk(qobj, 0, 0.04, 50, 1);
    glPushMatrix();
      glTranslatef(0, 0, 5);
      gluDisk(qobj, 0, 0.04, 50, 1);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, 0, 1.2);
      glRotatef(20, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, 0, 3.4);
      glRotatef(-20, 1, 0, 0);   
      glCallList(Ma_Feuille);
    glPopMatrix();
  glEndList();

  glNewList(Ma_Branche_Feuille, GL_COMPILE);
    glPushMatrix();
      glColor3f(0.69, 0.79, 0.38);
      gluCylinder(qobj, 0.04, 0.04, 5, 50, 1);
      gluDisk(qobj, 0, 0.04, 50, 1);
      glPushMatrix();
        glTranslatef(0, 0, 5);
        gluDisk(qobj, 0, 0, 50, 1);
      glPopMatrix();
    glPopMatrix();
    glTranslatef(0, 0, 1);
    glPushMatrix();
      glTranslatef(0, 0, 1.2);
      glRotatef(20, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, 0, 3);
      glRotatef(20, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, 0, 4);
      glRotatef(10, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, 0, 2.5);
      glRotatef(-20, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, 0, 3.6);
      glRotatef(-20, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0, -0.03, 4);
      glRotatef(-10, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
  glEndList();
}

void faire_bambou(GLuint liste, int hauteur, int nb_branches)
{
  int i;
  
  //Bambou
  glNewList(liste, GL_COMPILE);
  glPushMatrix();
    glPushMatrix();
      glTranslatef(0.0, 0.0, -5.0);
      glCallList(Mon_Morceau_Bambou);
      for(i=0; i<hauteur; i++)
      {
        glTranslatef(0.0, 0.0, 3.0);
        glCallList(Mon_Morceau_Bambou2);
        glCallList(Mon_Morceau_Bambou);
      }
      glTranslatef(0.0, 0.0, 3.0);
      glCallList(Mon_Morceau_Bambou2);
    glPopMatrix();

    glTranslatef(0.0, 0.0, 3.0);
    for(i=0; i<nb_branches; i++)
    {
      glTranslatef(0.0, 0.0, 3.0*hauteur/nb_branches);

      glPushMatrix();
        glRotatef(120*(i%3 + rand()%11/10.0), 0, 0, 1);
        glTranslatef(0, 0, -9);
        glRotatef(10, 1, 0, 0);
        glCallList(Ma_Branche);
        
        glPushMatrix();
          glTranslatef(0, 0, 5);
          glRotatef(10, 1, 0, 0);
          glCallList(Ma_Branche_Feuille);
        glPopMatrix();
        glPushMatrix();
          glTranslatef(0, 0, 5);
          glRotatef(20, 1, 0, 0);
          glRotatef(30, 0, 1, 0);
          glRotatef(90, 0, 0, 1);
          glCallList(Ma_Branche_Feuille);
        glPopMatrix();
        
      glPopMatrix();
    }
  glPopMatrix();
  glEndList();
}

int faire_foret_bambous(int nb_bambous, int nb_varietes, char* pHeightMap)
{
  int i, x, y;
  GLuint foret;
  
  faire_composantes_bambou();
  
  Mes_Bambous = (GLuint*) malloc(sizeof(GLuint)*nb_varietes);
  
  Mes_Bambous[0] = glGenLists(nb_varietes+1);
  for (i=1; i<nb_varietes; i++)
    Mes_Bambous[i] = Mes_Bambous[i-1] + 1;
  foret = Mes_Bambous[nb_varietes-1] + 1;
  
  for (i=0; i<nb_bambous/3; i++)
    faire_bambou(Mes_Bambous[i], nb_aleatoire(7,12), nb_aleatoire(7,12)*1.7);

  glNewList(foret, GL_COMPILE);
    for (i=0; i<nb_bambous; i++)
    {
      glPushMatrix();
        x = (i/10)*17+nb_aleatoire(-2,10);
        y = (i%10)*17+nb_aleatoire(-2,10);
        // IL FAUT TROUVER LA BONNE VALEUR
        glTranslatef(x, y, Height(pHeightMap, x, y)+3);
        glCallList(Mes_Bambous[nb_aleatoire(0,nb_varietes-1)]);
      glPopMatrix();
    }
  glEndList();
  
  return foret;
}