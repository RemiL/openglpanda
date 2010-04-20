/*****************************************************************************
File: TD3-avatar-skel.c

Informatique Graphique IG1
IFIPS
Christian Jacquemin, Université Paris 11

Copyright (C) 2007 University Paris 11 
This file is provided without support, instruction, or implied
warranty of any kind.  University Paris 11 makes no guarantee of its
fitness for a particular purpose and is not liable under any
circumstances for any damages or loss whatsoever arising from the use
or inability to use this file or items derived from it.
******************************************************************************/
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define    windowWidth 600
#define    windowHeight 600

#define RED   0
#define GREEN 0
#define BLUE  1
#define ALPHA 1

#define true  1
#define false 0

#define KEY_ESC 27

#define PI 3.1415926535898

#define position_Ini -60.0

float t = 0.f;
float delta = 10.f;
float k = 0.001f;
float K = 0.002f;
int IdleRunning = false;
int TraceEcran = false;
int RangFichierStockage = 0;
float position = position_Ini;

int  Ma_Tete;
int  Ma_Tache;
int  Mon_Oeil;
int  Ma_Pupille;
int  Mon_Oreille;
int  Mon_Tronc;
int  Mon_Museau;
int  Mon_Nez;
int  Mon_AvantBras;
int  Ma_Cuisse;
int  Mon_Mollet;
int  Mon_Morceau_Bambou;
int  Mon_Bambou;
int  Mon_Morceau_Bambou2;
int  Ma_Feuille;
int  Ma_Branche;
int  Ma_Branche_Feuille;

int  Mon_Repere;
int  Mon_Carre;
int  Mon_Cube;

enum lateralite{ Gauche = 0, Droit };

float angle_Bras[2];
float angle_AvantBras[2];
float angle_Cuisse[2];
float angle_Mollet[2];

float angle_Bras_Ini[2] = {-30.0, 15.0};
float angle_AvantBras_Ini[2] = {0.0, 15.0};
float angle_Cuisse_Ini[2] = {20.0, -20.0};
float angle_Mollet_Ini[2] = {0.0, -20.0};

float amplitude_Bras;
float amplitude_AvantBras;
float amplitude_Cuisse;
float amplitude_Mollet;
float med_Bras;
float med_AvantBras;
float med_Cuisse;
float med_Mollet;

static GLfloat mat_specular[] = { 1.0 , 1.0 , 1.0 , 1.0 };
static GLfloat mat_ambientanddiffuse[] = { 0.4, 0.4 , 0.0 , 1.0 };
static GLfloat mat_shininess[] = { 20.0};

static GLfloat light_position0[] = { 15.0 , 15.0 , 15.0 , 0.0 };
static GLfloat light_position1[] = { 15.0 , 15.0 , -15.0 , 0.0 };

static GLfloat ambient_light0[] = { 0.0 , 0.0 , 0.0 , 0.0 };
static GLfloat diffuse_light0[] = { 0.7 , 0.7 , 0.7 , 1.0 };
static GLfloat specular_light0[] = { 0.1 , 0.1 , 0.1 , 0.1 };

static GLfloat ambient_light1[] = { 0.50 , 0.50 , 0.50 , 1.0 };
static GLfloat diffuse_light1[] = { 0.5 , 1.0 , 1.0 , 1.0 };
static GLfloat specular_light1[] = { 0.5 , 1.0 , 1.0 , 1.0 };

int window_width, window_height;

// Gestion caméra
typedef struct
{
  double x;
  double y;
  double z;
} t_coordonnees;

typedef struct
{
  t_coordonnees position;
  t_coordonnees vecteur_observation;
  t_coordonnees vecteur_def_vertical;
  t_coordonnees vecteur_normal;
  t_coordonnees vecteur_observation_initial;
  t_coordonnees vecteur_def_vertical_initial;
  t_coordonnees vecteur_normal_initial;
  double angle_h;
  double angle_v;
  double pas;
} t_camera;

t_camera camera;
int position_clique_x, position_clique_y;
int camera_deplacement_active = false;

int Step = 0;
int latence = 4;

void init_scene();
void render_scene();
void init_angles();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y);
GLvoid window_cliques_souris(int button, int state, int x, int y);
GLvoid window_mouvements_souris(int x, int y);
GLvoid window_timer(); 
void Faire_Composantes();
void Dessine_Repere();
void faire_cube(int cote);
void init_camera(double x, double y, double z,
                 double vect_ob_x, double vect_ob_y, double vect_ob_z,
                 double vect_vert_x, double vect_vert_y, double vect_vert_z,
                 double pas);
void addition_vectorielle(t_coordonnees *r, double coeff_a, t_coordonnees a, double coeff_b, t_coordonnees b);
double produit_scalaire(t_coordonnees a, t_coordonnees b);
void produit_vectoriel(t_coordonnees *r, t_coordonnees a, t_coordonnees b);
void normaliser_vecteur(t_coordonnees *v);

int main(int argc, char **argv) 
{
  srand(time(NULL));
  // initialisation  des paramètres de GLUT en fonction
  // des arguments sur la ligne de commande
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  // définition et création de la fenêtre graphique
  glutInitWindowSize(windowWidth,windowHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Projet OpenGL");

  // initialisation de OpenGL et de la scène
  initGL();  
  init_scene();

  // choix des procédures de callback pour 
  // le tracé graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fenêtre
  glutReshapeFunc(&window_reshape);
  // la gestion des événements clavier
  glutKeyboardFunc(&window_key);
  glutSpecialFunc(&window_key); // pour les touches spéciales (flèches directionnelles, ...)
  // Gestion des cliques souris
  glutMouseFunc(&window_cliques_souris);
  // Gestion des mouvements de la souris
  glutMotionFunc(&window_mouvements_souris);
  // fonction appelée régulièrement entre deux gestions d´événements
  glutTimerFunc(latence,&window_timer,Step);

  // la boucle prinicipale de gestion des événements utilisateur
  glutMainLoop();  

  return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque

GLvoid initGL() 
{
  // initialisation de l´éclairement

  // définition de deux source lumineuses
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light0);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

  // activation de l´éclairement
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // propriétés matérielles des objets
  // glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambientanddiffuse);
  // glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  // glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glShadeModel( GL_SMOOTH );
  glEnable(GL_COLOR_MATERIAL);

  // initialisation du fond
  glClearColor(RED, GREEN, BLUE, ALPHA);        
  // z-buffer
  glEnable(GL_DEPTH_TEST);
}
  
void init_scene()
{
  // Dessine un cube centré sur l'origine
  faire_cube(100);
  
  // initialise des display lists des composantes cylindriques du corps
  Faire_Composantes();

  // Initialisation camera
  init_camera(30, 0, 0, -1, 0, 0, 0, 0, 1, 1);

  amplitude_Bras 
    = .5 * (angle_Bras_Ini[ Droit ] - angle_Bras_Ini[ Gauche ]);
  amplitude_AvantBras 
    = .5 * (angle_AvantBras_Ini[ Droit ] - angle_AvantBras_Ini[ Gauche ]);
  amplitude_Cuisse 
    = .5 * (angle_Cuisse_Ini[ Droit ] - angle_Cuisse_Ini[ Gauche ]);
  amplitude_Mollet 
    = .5 * (angle_Mollet_Ini[ Droit ] - angle_Mollet_Ini[ Gauche ]);
  med_Bras 
    = .5 * (angle_Bras_Ini[ Droit ] + angle_Bras_Ini[ Gauche ]);
  med_AvantBras 
    = .5 * (angle_AvantBras_Ini[ Droit ] + angle_AvantBras_Ini[ Gauche ]);
  med_Cuisse 
    = .5 * (angle_Cuisse_Ini[ Droit ] + angle_Cuisse_Ini[ Gauche ]);
  med_Mollet 
    = .5 * (angle_Mollet_Ini[ Droit ] + angle_Mollet_Ini[ Gauche ]);
}

// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Positionnement caméra
  gluLookAt(camera.position.x,
            camera.position.y,
            camera.position.z,
            camera.position.x + camera.vecteur_observation.x,
            camera.position.y + camera.vecteur_observation.y,
            camera.position.z + camera.vecteur_observation.z,
            camera.vecteur_def_vertical.x,
            camera.vecteur_def_vertical.y,
            camera.vecteur_def_vertical.z);
  
  // Tracé de la scène
  render_scene();

  // trace la scène grapnique qui vient juste d'être définie
  glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{  
  window_width = width;
  window_height = height;
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // glOrtho(-20, 20, -20, 20, -1000, 1000);
  // glFrustum(-20, 20, -20, 20, 10, 1000);
  gluPerspective(70, (float) width / height, 1, 1000);
  // glScalef(10, 10, 10);

  // toutes les transformations suivantes s´appliquent au modèle de vue 
  glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y) 
{  
  switch (key) {
    case KEY_ESC:  
      exit(1);                    
      break; 
    case ' ':
      if (IdleRunning) {
        glutTimerFunc(latence,&window_timer,Step);
        IdleRunning = false;
      } 
      else {
        //glutTimerFunc(latence,NULL,Step);
        IdleRunning = true;
      }
      break; 
    case '+':  
      delta *= 1.01;
      break; 
    case '-':  
      delta /= 1.01;
      break;
    // Gestion mouvements caméra
    case GLUT_KEY_UP:
      printf("GLUT_KEY_UP\n");
      addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_observation);
      break;
    case GLUT_KEY_DOWN:
      printf("GLUT_KEY_DOWN\n");
      addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_observation);
      break;
    case GLUT_KEY_RIGHT:
      printf("GLUT_KEY_LEFT\n");
      addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_normal);
      break;
    case GLUT_KEY_LEFT:
      printf("GLUT_KEY_RIGHT\n");
      addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_normal);
      break;
    default:
      printf ("La touche %d n´est pas active.\n", key);
      break;
  }     
}

// Fonction de gestion des cliques souris
GLvoid window_cliques_souris(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
  {
    position_clique_x = x;
    position_clique_y = y;
    camera_deplacement_active = true;
  }
  else
    camera_deplacement_active = false;
}

// Fonction de gestion des mouvements de la souris
GLvoid window_mouvements_souris(int x, int y)
{
  if (camera_deplacement_active)
  {
    camera.angle_h += (double)(x-position_clique_x) * PI / window_width;
    camera.angle_v += (double)(y-position_clique_y) * PI / window_height;

    position_clique_x = x;
    position_clique_y = y;
    
    // On limite l'angle d'inclinaison de la caméra à +/- PI/2
    if (camera.angle_v < -PI/2)
      camera.angle_v = -PI/2;
    else if (camera.angle_v > PI/2)
      camera.angle_v = PI/2;
    
    // Rotation par rapport au vecteur définissant la verticale de la caméra
    addition_vectorielle(&camera.vecteur_observation,
                         cos(camera.angle_h), camera.vecteur_observation_initial,
                         sin(camera.angle_h), camera.vecteur_normal_initial);
    // On recalcule le nouveau vecteur normal
    produit_vectoriel(&camera.vecteur_normal, camera.vecteur_observation, camera.vecteur_def_vertical_initial);

    // Rotation par rapport au vecteur normal de la caméra
    addition_vectorielle(&camera.vecteur_observation,
                         cos(camera.angle_v), camera.vecteur_observation,
                         -sin(camera.angle_v), camera.vecteur_def_vertical_initial);
    // On recalcule le nouveau vecteur définissant la verticale
    produit_vectoriel(&camera.vecteur_def_vertical, camera.vecteur_normal, camera.vecteur_observation);
    
    // On met à jour l'affichage
    glutPostRedisplay();
  }
}

// fonction de call-back appelée régulièrement

GLvoid window_timer() 
{
  // On effecture une variation des angles de chaque membre
  // de l'amplitude associée et de la position médiane
  angle_Bras[ Gauche ] = med_Bras + sin(k*t)*amplitude_Bras;
  angle_Bras[ Droit ] = med_Bras - sin(k*t)*amplitude_Bras;
  angle_AvantBras[ Gauche ] = med_AvantBras + sin(k*t)*amplitude_AvantBras;
  angle_AvantBras[ Droit ] = med_AvantBras - sin(k*t)*amplitude_AvantBras;
  angle_Cuisse[ Gauche ] = med_Cuisse + sin(k*t)*amplitude_Cuisse;
  angle_Cuisse[ Droit ] = med_Cuisse - sin(k*t)*amplitude_Cuisse;
  angle_Mollet[ Gauche ] = med_Mollet + sin(k*t)*amplitude_Mollet;
  angle_Mollet[ Droit ] = med_Mollet - sin(k*t)*amplitude_Mollet;

  t += delta;

  // On déplace la position de l'avatar pour qu'il avance
  position = position_Ini + K*t;
  
  if (!IdleRunning)
    glutTimerFunc(latence,&window_timer,++Step);

  glutPostRedisplay();
}

void faire_cube(int cote)
{
  Mon_Carre = glGenLists(2);
  Mon_Cube = Mon_Carre + 1;
  
  glNewList(Mon_Carre, GL_COMPILE);
    glBegin(GL_POLYGON);
      glVertex3f(cote/2, -cote/2, 0);
      glVertex3f(cote/2, cote/2, 0);
      glVertex3f(-cote/2, cote/2, 0);
      glVertex3f(-cote/2, -cote/2, 0);  
    glEnd();  
  glEndList();

  glNewList(Mon_Cube, GL_COMPILE);
    glColor3f(1, 0, 0);
    glPushMatrix();
      glTranslatef(0, 0, -cote/2);
      glCallList(Mon_Carre);
    glPopMatrix();

    glColor3f(1, 1, 0);
    glPushMatrix();
      glTranslatef(0, 0, cote/2);
      glCallList(Mon_Carre);
    glPopMatrix();
    
    glColor3f(0, 1, 0);
    glPushMatrix();
      glTranslatef(-cote/2, 0, 0);
      glRotatef(90, 0, 1, 0);
      glCallList(Mon_Carre);
    glPopMatrix();

    glColor3f(0, 1, 1);
    glPushMatrix();
      glTranslatef(cote/2, 0, 0);
      glRotatef(90, 0, 1, 0);
      glCallList(Mon_Carre);
    glPopMatrix();
    
    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
      glTranslatef(0, -cote/2, 0);
      glRotatef(-90, 1, 0, 0);
      glCallList(Mon_Carre);
    glPopMatrix();

    glColor3f(1, 0, 1);
    glPushMatrix();
      glTranslatef(0, cote/2, 0);
      glRotatef(-90, 1, 0, 0);
      glCallList(Mon_Carre);
    glPopMatrix();
  glEndList();
}

// un cylindre
void Faire_Composantes() {
  int i;
  GLUquadricObj* qobj; /*GLAPIENTRY*/

   // allocation d´une description de quadrique
  qobj = gluNewQuadric();
  // la quadrique est pleine 
  gluQuadricDrawStyle(qobj, GLU_FILL); 
  // les ombrages, s´il y en a, sont doux
  gluQuadricNormals(qobj, GLU_SMOOTH);

  // attribution des indentificateurs de display lists
  Ma_Tete =  glGenLists(17);
  Ma_Tache = Ma_Tete + 1;
  Mon_Oeil = Ma_Tete + 2;
  Ma_Pupille = Ma_Tete + 3;
  Mon_Oreille = Ma_Tete + 4;
  Mon_Museau = Ma_Tete + 5;
  Mon_Nez = Ma_Tete + 6;
  Mon_Tronc = Ma_Tete + 7;
  Mon_AvantBras = Ma_Tete + 8;
  Ma_Cuisse = Ma_Tete + 9;
  Mon_Mollet = Ma_Tete + 10;
  Mon_Morceau_Bambou = Ma_Tete + 11;
  Mon_Bambou = Ma_Tete + 12;
  Mon_Morceau_Bambou2 = Ma_Tete + 13;
  Ma_Feuille = Ma_Tete + 14;
  Ma_Branche = Ma_Tete + 15;
  Ma_Branche_Feuille = Ma_Tete + 16;

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

  glNewList(Mon_Morceau_Bambou, GL_COMPILE);
    glColor3f(0.69, 0.79, 0.38);
    gluCylinder(qobj, 0.3, 0.3, 3, 50, 1);
    gluDisk(qobj, 0, 0.3, 50, 1);
    glPushMatrix();
      glTranslatef(0, 0, 3);
      gluDisk(qobj, 0, 0.3, 50, 1);
    glPopMatrix();
  glEndList();

  glNewList(Mon_Morceau_Bambou2, GL_COMPILE);
    glColor3f(0.79, 0.89, 0.48);
    gluCylinder(qobj, 0.32, 0.32, 0.14, 50, 1);
    gluDisk(qobj, 0, 0.32, 50, 1);
    glTranslatef(0, 0, 0.14);
    gluDisk(qobj, 0, 0.32, 50, 1);
  glEndList();

  glNewList(Ma_Feuille, GL_COMPILE);
    glColor3f(0.32, 0.50, 0.07);
    glBegin(GL_POLYGON);
      glVertex3f(0.0, 0.05, 0.0);
      glVertex3f(-0.1, 0.15, 1.0);
      glVertex3f(-0.3, 0.0, 2.0);
      glVertex3f(-0.1, -0.15, 1.0);
      glVertex3f(0.0, -0.05, 0.0);
    glEnd();
  glEndList();

  glNewList(Ma_Branche, GL_COMPILE);
    glColor3f(0.69, 0.79, 0.38);
    gluCylinder(qobj, 0.04, 0.04, 5, 50, 1);
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
      glTranslatef(0, 0, 4);
      glRotatef(-10, 1, 0, 0);
      glCallList(Ma_Feuille);
    glPopMatrix();
  glEndList();

  // compilation de la display list de la sphère servant de tête
  glNewList(Ma_Tete, GL_COMPILE);
    // Tête en elle même
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

  //Bambou
  glNewList(Mon_Bambou, GL_COMPILE);
    glPushMatrix();
      glTranslatef(4.0, 4.0, -5.0);
      glCallList(Mon_Morceau_Bambou);
      for(i=0; i<6; i++)
      {
        glTranslatef(0.0, 0.0, 3.0);
        glCallList(Mon_Morceau_Bambou2);
        glCallList(Mon_Morceau_Bambou);
      }
      glTranslatef(0.0, 0.0, 3.0);
      glCallList(Mon_Morceau_Bambou2);

      for(i=0; i<200; i++)
      {
        float trans;
        int angle;
        int angle2;
        int angle3;
        int angle4;
        int angle5;
        do
        {
          trans = -5*(rand()%50 + 5) / (float)(rand()%50 + 5);
        }while(trans < -10);

        angle = rand();
        angle2 = (rand() % 100) + 80;
        if(rand() % 2)
          angle3 = (rand() % 50) + 20;
        else
          angle3 = -(rand() % 50) + 20;
        angle4 = -(rand() % 50) + 20;
        angle5 = -(rand() % 15);

        glPushMatrix();
          glScalef(0.5, 0.5, 0.5);
          glTranslatef(0, 0, trans);
          glRotatef(angle, 0, 0, 1);
          glRotatef(angle2, 0, 1, 0);
          glRotatef(angle3, 0, 0, 1);
          glRotatef(angle4, 0, 1, 0);
          if(rand() % 2)
          {
            glCallList(Ma_Branche);
            glTranslatef(0, 0, 5);
          }
          glRotatef(angle5, 0, 1, 0);
          glCallList(Ma_Branche_Feuille);
        glPopMatrix();
      }
      /*glPushMatrix();
        glScalef(0.8, 0.8, 0.8);
        glRotatef(90, 0, 0, 1);
        glRotatef(90, 0, 1, 0);
        glRotatef(30, 0, 0, 1);
        glRotatef(30, 0, 1, 0);
        glCallList(Ma_Branche);
      glPopMatrix();
      glPushMatrix();
        glScalef(0.8, 0.8, 0.8);
        glRotatef(90, 0, 0, 1);
        glRotatef(-90, 0, 1, 0);
        glRotatef(30, 0, 0, 1);
        glRotatef(30, 0, 1, 0);
        glCallList(Ma_Branche);
        glTranslatef(0, 0, 5);
        glRotatef(-10, 0, 1, 0);
        glCallList(Ma_Branche_Feuille);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(0, 0, -1);
        glScalef(0.8, 0.8, 0.8);
        glRotatef(70, 0, 0, 1);
        glRotatef(-70, 0, 1, 0);
        glRotatef(30, 0, 0, 1);
        glRotatef(-40, 0, 1, 0);
        glCallList(Ma_Branche);
      glPopMatrix();*/
    glPopMatrix();
  glEndList();

  // compilation des display lists des cylindres
  glNewList(Mon_Tronc, GL_COMPILE);
    glColor3f(1.0, 1, 1);
    glScalef(1.75, 1, 1);
    glutSolidSphere(6, 50, 50);
  glEndList();
/*
  glNewList(Mon_Bras, GL_COMPILE);
  gluCylinder(qobj, 0.5, 0.5, 5, 30, 30);
  glEndList();
*/
  glNewList(Mon_AvantBras, GL_COMPILE);
  gluCylinder(qobj, 0.5, 0.25, 5, 30, 30);
  glEndList();

  glNewList(Ma_Cuisse, GL_COMPILE);
  gluCylinder(qobj, 1.25, 0.75, 5, 30, 30);
  glEndList();

  glNewList(Mon_Mollet, GL_COMPILE);
  gluCylinder(qobj, 0.75, 0.25, 5, 30, 30);
  glEndList();
}

void  Dessine_Repere() {
  glNewList(Mon_Repere, GL_COMPILE);
    glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(-10 , 0 , 0);
      glVertex3f(10 , 0 , 0);
    glEnd();
    glBegin(GL_LINES);
      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(0 , -10 , 0);
      glVertex3f(0 , 10 , 0);
    glEnd();
    glPointSize( 10.0 );
    glBegin(GL_POINTS);
      glColor3f(1.0, 1.0, 1.0);
      glVertex3f(10.0 , 0 , 0);
    glEnd();
    glBegin(GL_POINTS);
      glColor3f(1.0, 1.0, 1.0);
      glVertex3f(0 , 10.0 , 0);
    glEnd();
  glEndList();
}

void render_scene()
{
  // rotation de 90 degres autour de Ox pour mettre l'axe Oz 
  // vertical comme sur la figure
  //glRotatef(-90, 1, 0, 0);

  //glRotatef(140, 1, 1, 1);

  // rotation de 160 degres autour de l'axe Oz pour faire
  // avancer l'avatar vers le spectateur
  //glRotatef(-180, 0, 0, 1);

  // rotation de 25 degres autour de la bissectrice de $Oy$ pour
  // voir la figure en perspective
  // glRotatef(25, 0, 1, 0);

  // déplacement horizontal selon l´axe Oy pour donner 
  // une impression de déplacement horizontal accompagnant
  // la marche
  // glTranslatef( 0, position, 0);
  
  // Tracé d'un cube pour délimiter l'espace
  glPushMatrix();
    glCallList(Mon_Cube);
  glPopMatrix();

  // tracé du tronc, aucune transformation n´est
  // requise
  glPushMatrix();
    //glCallList(Mon_Tronc);
  glPopMatrix();
  // tracé de la tête avec une translation positive
  // selon Oz pour la placer au-dessus du tronc
  // les appels à glPushMatrix et glPopMatrix servent 
  // à sauvegarder et restaurer le contexte graphique
  glPushMatrix();
    glTranslatef(11.0, 0.0, 3.0);
    glCallList(Ma_Tete);
  glPopMatrix();

  glCallList(Mon_Bambou);

/*
  // tracé de la cuisse droite avec une translation vers
  // la droite et une rotation de 180° autour de Ox
  // pour l´orienter vers le bas
  glPushMatrix();
  glColor3f(0.0, 1.0, 1.0); // cyan
  glTranslatef(1.25, 0.0, 0.0);
  glRotatef(180, 1.0, 0, 0);
  glRotatef(angle_Cuisse[ Droit ], 1.0, 0.0, 0.0);
  glCallList(Ma_Cuisse);

  // pour tracer le mollet, on reste dans le même
  // contexte graphique et on translate de
  // +5 selon Oz afin de mettre le repère au niveau
  // du genou
  glPushMatrix();
    glTranslatef(0.0, 0.0, 5.0);
    glRotatef(angle_Mollet[ Droit ], 1.0, 0.0, 0.0);
    glCallList(Mon_Mollet);
  glPopMatrix();
  glPopMatrix();

  // cuisse et mollet gauches
  // seule la translation initiale change
  glPushMatrix();
    glColor3f(0.0, 0.0, 1.0); // bleu
    glTranslatef(-1.25, 0.0, 0.0);
    glRotatef(180, 1.0, 0, 0);
    glRotatef(angle_Cuisse[ Gauche ], 1.0, 0.0, 0.0);
    glCallList(Ma_Cuisse);

    glPushMatrix();
      glTranslatef(0.0, 0.0, 5.0);
      glRotatef(angle_Mollet[ Gauche ], 1.0, 0.0, 0.0);
      glCallList(Mon_Mollet);
    glPopMatrix();
  glPopMatrix();

  // tracé du bras droit avec une translation vers
  // la droite et vers le haut composée avec une 
  // rotation de 180° autour de Ox pour l´orienter
  // vers le bas
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0);
  glTranslatef(3.0, 0.0, 0.0);
  glTranslatef(0.0, 0.0, 7.0);
  glRotatef(180, 1.0, 0, 0);
  glRotatef(angle_Bras[ Droit ], 1.0, 0, 0);
  glCallList(Mon_Bras);

  // pour tracer l´avant-bras, on reste dans le même
  // contexte graphique et on translate de
  // +5 selon Oz afin de mettre le repère au niveau
  // du coude
  glPushMatrix();
    glTranslatef(0.0, 0.0, 5.0);
    glRotatef(angle_AvantBras[ Droit ], 1.0, 0, 0);
    glCallList(Mon_AvantBras);
  glPopMatrix();
  glPopMatrix();

  // bras et avant-bras gauches
  // seule la translation initiale change
  glPushMatrix();
  glColor3f(1.0, 0.0, 1.0); // violet
  glTranslatef(-3.0, 0.0, 0.0);
  glTranslatef(0.0, 0.0, 7.0);
  glRotatef(180, 1.0, 0, 0);
  glRotatef(angle_Bras[ Gauche ], 1.0, 0, 0);
  glCallList(Mon_Bras);

  glPushMatrix();
    glTranslatef(0.0, 0.0, 5.0);
    glRotatef(angle_AvantBras[ Gauche ], 1.0, 0, 0);
    glCallList(Mon_AvantBras);
  glPopMatrix();
  glPopMatrix();
*/
  // permutation des buffers lorsque le tracé est achevé
  glutSwapBuffers();
}

void init_camera(double x, double y, double z,
                 double vect_ob_x, double vect_ob_y, double vect_ob_z,
                 double vect_vert_x, double vect_vert_y, double vect_vert_z,
                 double pas)
{
  double vect_ob_norme = sqrt(vect_ob_x*vect_ob_x + vect_ob_y*vect_ob_y + vect_ob_z*vect_ob_z);
  double vect_vert_norme = sqrt(vect_vert_x*vect_vert_x + vect_vert_y*vect_vert_y + vect_vert_z*vect_vert_z);
  
  camera.position.x = x;
  camera.position.y = y;
  camera.position.z = z;

  camera.vecteur_observation_initial.x = camera.vecteur_observation.x = vect_ob_x / vect_ob_norme;
  camera.vecteur_observation_initial.y = camera.vecteur_observation.y = vect_ob_y / vect_ob_norme;
  camera.vecteur_observation_initial.z = camera.vecteur_observation.z = vect_ob_z / vect_ob_norme;

  camera.vecteur_def_vertical_initial.x = camera.vecteur_def_vertical.x = vect_vert_x / vect_vert_norme;
  camera.vecteur_def_vertical_initial.y = camera.vecteur_def_vertical.y = vect_vert_y / vect_vert_norme;
  camera.vecteur_def_vertical_initial.z = camera.vecteur_def_vertical.z = vect_vert_z / vect_vert_norme;

  produit_vectoriel(&camera.vecteur_normal, camera.vecteur_observation, camera.vecteur_def_vertical);
  produit_vectoriel(&camera.vecteur_normal_initial, camera.vecteur_observation_initial, camera.vecteur_def_vertical_initial);

  camera.angle_h = camera.angle_v = 0;

  camera.pas = pas;
}

void addition_vectorielle(t_coordonnees *r, double coeff_a, t_coordonnees a, double coeff_b, t_coordonnees b)
{  
  if (r != NULL)
  {
    r->x = coeff_a*a.x + coeff_b*b.x;
    r->y = coeff_a*a.y + coeff_b*b.y;
    r->z = coeff_a*a.z + coeff_b*b.z;
  }
}

double produit_scalaire(t_coordonnees a, t_coordonnees b)
{
  return (a.x*b.x + a.y*b.y + a.z*b.z);
}

void produit_vectoriel(t_coordonnees *r, t_coordonnees a, t_coordonnees b)
{
  if (r != NULL)
  {
    r->x = a.y*b.z - a.z*b.y;
    r->y = a.z*b.x - a.x*b.z;
    r->z = a.x*b.y - a.y*b.x;
  }
}

void normaliser_vecteur(t_coordonnees *v)
{
  double norme = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);

  v->x /= norme;
  v->y /= norme;
  v->z /= norme;
}