/*****************************************************************************
MON PANDA

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "bambou.h"
#include "panda.h"
#include "camera.h"

#define windowWidth 600
#define windowHeight 600

#define RED   0
#define GREEN 0
#define BLUE  1
#define ALPHA 1

#define true  1
#define false 0

#define KEY_ESC 27

#define PI 3.1415926535898

#define position_Ini -60.0
#define positionCote_Ini 0.0

float t = 0.f;
float tPosition = 0.f;
float tCote = 0.f;
float delta = 50.f;
float k = 0.001f;
float K = 0.001f;
int IdleRunning = false;
int TraceEcran = false;
int RangFichierStockage = 0;
float position = position_Ini;
float positionCote = positionCote_Ini;
float dzArret;

// Permet de savoir les touches directionnelles apuyées
int isKeyUp = 0;
int isKeyDown = 0;
int isKeyRight = 0;
int isKeyLeft = 0;

// Display lists du Panda
GLuint Ma_Tete;
GLuint Mon_Corps;
GLuint Ma_Cuisse;
GLuint Mon_Mollet;
// Display lists du décor
GLuint Ma_Foret;

// Textures
GLuint texture_herbe;

enum lateralite{ Gauche = 0, Droit };

enum allures{Arret = 0, Pas = 1, Trot = 2, Galop = 3};
enum modes{Spectateur = 0, Panda = 1};

int allure = Arret;
int mode = Spectateur;

// Variables pour la marche
enum etats{ArriereAvance = 0, AvantAvance = 1, Recule = 2};
int etatMarche[2] = {Recule, AvantAvance};

float angle_Bras[2];
float angle_AvantBras[2];
float angle_Cuisse[2];
float angle_Mollet[2];
float angle_Corps;
float angle_Tete;
float angle_Corps_Arret;
float angle_Tete_Arret;

float angle_Bras_Ini[2] = {30.0, -10.0};
float angle_AvantBras_Ini[2] = {-30.0, -30.0};
float angle_Cuisse_Ini[2] = {30.0, -40.0};
float angle_Mollet_Ini[2] = {0.0, 40.0};
float angle_Corps_Ini[2] = {-10, 10};
float angle_Tete_Ini[2] = {3, -3};

float amplitude_Bras;
float amplitude_AvantBras;
float amplitude_Cuisse;
float amplitude_Mollet;
float amplitude_Corps;
float amplitude_Tete;
float med_Bras;
float med_AvantBras;
float med_Cuisse;
float med_Mollet;
float med_Corps;
float med_Tete;

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
t_camera camera;
int position_clique_x, position_clique_y;
int camera_deplacement_active = false;

int Step = 0;
int latence = 4;

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

void init_scene();
void dessiner_decor();
void dessiner_panda();
void render_scene();
void init_angles();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y);
GLvoid window_up_key(unsigned char key, int x, int y);
GLvoid window_special_key(int key, int x, int y);
GLvoid window_special_up_key(int key, int x, int y);
GLvoid window_cliques_souris(int button, int state, int x, int y);
GLvoid window_mouvements_souris(int x, int y);
GLvoid window_mouvements_passifs_souris(int x, int y);
GLvoid window_timer(); 
void Faire_Composantes();
GLuint LoadTextureRAW( const char * filename, int wrap );

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
  // la gestion des relâchements de touche
  glutKeyboardUpFunc(&window_up_key);
  // pour la pression sur les touches spéciales (flèches directionnelles, ...)
  glutSpecialFunc(&window_special_key);
  // pour le relâchement des touches spéciales
  glutSpecialUpFunc(&window_special_up_key); 
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
  //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_ambientanddiffuse);
  //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glShadeModel( GL_SMOOTH );
  glEnable(GL_COLOR_MATERIAL);

  // initialisation du fond
  glClearColor(RED, GREEN, BLUE, ALPHA);        
  // z-buffer
  glEnable(GL_DEPTH_TEST);

  texture_herbe = LoadTextureRAW("herbe.raw", 1);
}
  
void init_scene()
{  
  // initialise les display lists
  Faire_Composantes();

  // Initialisation camera
  init_camera(30, 0, 5, -1, 0, 0, 0, 0, 1, 1);

  etatMarche[Droit] = ArriereAvance;
  etatMarche[Gauche] = Recule;

  amplitude_Bras 
    = .5 * (angle_Bras_Ini[ Droit ] - angle_Bras_Ini[ Gauche ]);
  amplitude_AvantBras 
    = .5 * (angle_AvantBras_Ini[ Droit ] - angle_AvantBras_Ini[ Gauche ]);
  amplitude_Cuisse 
    = .5 * (angle_Cuisse_Ini[ Droit ] - angle_Cuisse_Ini[ Gauche ]);
  amplitude_Mollet 
    = .5 * (angle_Mollet_Ini[ Droit ] - angle_Mollet_Ini[ Gauche ]);
  amplitude_Corps 
    = .5 * (angle_Corps_Ini[ Droit ] - angle_Corps_Ini[ Gauche ]);
  amplitude_Tete 
    = .5 * (angle_Tete_Ini[ Droit ] - angle_Tete_Ini[ Gauche ]);
  med_Bras 
    = .5 * (angle_Bras_Ini[ Droit ] + angle_Bras_Ini[ Gauche ]);
  med_AvantBras 
    = .5 * (angle_AvantBras_Ini[ Droit ] + angle_AvantBras_Ini[ Gauche ]);
  med_Cuisse 
    = .5 * (angle_Cuisse_Ini[ Droit ] + angle_Cuisse_Ini[ Gauche ]);
  med_Mollet 
    = .5 * (angle_Mollet_Ini[ Droit ] + angle_Mollet_Ini[ Gauche ]);
  med_Corps 
    = .5 * (angle_Corps_Ini[ Droit ] + angle_Corps_Ini[ Gauche ]);
  med_Tete
    = .5 * (angle_Tete_Ini[ Droit ] + angle_Tete_Ini[ Gauche ]);
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
  switch (key)
  {
    case KEY_ESC:  
      exit(1);                    
      break; 
    default:
      printf ("La touche %d n´est pas active.\n", key);
      break;
  }     
}

// fonction de call-back pour la gestion des événements clavier (relâchement)
GLvoid window_up_key(unsigned char key, int x, int y) 
{  
  switch (key)
  {
    // Augmentation de l'allure
    case '+':
      if(allure < Galop)
      {
        allure++;
      }
      break; 
    // Diminution de l'allure
    case '-':
      printf("%d\n", allure);
      if(allure > Pas || (allure == Pas && mode == Spectateur))
      {
        allure--;

        if(allure == Arret)
          t = 0;
      }
      break;
    // Changement du mode de jeu
    case '0':
      if(mode == Spectateur)
      {
        mode = Panda;
        isKeyUp = 0;
        isKeyDown = 0;
        isKeyRight = 0;
        isKeyLeft = 0;
        // Mettre la camera sur la tete du panda !!!!!!!!!!!!!!!
      }
      else
      {
        mode = Spectateur;
        allure = Arret;
      }
      break;
    // Gestion mode caméra
    case 'f':
      printf("mode fps\n");
      camera_activer_mode_fps(!camera.mode_fps);
      break;
    default:
      printf ("La touche %d n´est pas active.\n", key);
      break;
  }     
}

// Gestion de la pression des touches spéciales
GLvoid window_special_key(int key, int x, int y)
{
  switch(key)
  {
    // Gestion mouvements caméra
    case GLUT_KEY_UP:
      printf("GLUT_KEY_UP\n");
      isKeyUp = 1;
      isKeyDown = 0;
      // DEPLACEMENT DE LA CAMERA AVEC LE PANDA
      //addition_vectorielle(&camera.position, 1, camera.position, K, camera.vecteur_observation);
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      printf("GLUT_KEY_DOWN\n");
      isKeyDown = 1;
      isKeyUp = 0;
      // DEPLACEMENT DE LA CAMERA AVEC LE PANDA
      //addition_vectorielle(&camera.position, 1, camera.position, -K, camera.vecteur_observation);
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      printf("GLUT_KEY_LEFT\n");
      isKeyRight = 1;
      isKeyLeft = 0;
      // DEPLACEMENT DE LA CAMERA AVEC LE PANDA
      //addition_vectorielle(&camera.position, 1, camera.position, K, camera.vecteur_normal);
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      printf("GLUT_KEY_RIGHT\n");
      isKeyLeft = 1;
      isKeyRight = 0;
      // DEPLACEMENT DE LA CAMERA AVEC LE PANDA
      //addition_vectorielle(&camera.position, 1, camera.position, -K, camera.vecteur_normal);
      glutPostRedisplay();
      break;
    default:
      printf ("La touche %d n´est pas active.\n", key);
      break;
  }
}

// Gestion du relâchement des touches spéciales
GLvoid window_special_up_key(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP:
      printf("GLUT_KEY_UP_UP\n");
      isKeyUp = 0;
      glutPostRedisplay();
      break;
    case GLUT_KEY_DOWN:
      printf("GLUT_KEY_DOWN_UP\n");
      isKeyDown = 0;
      glutPostRedisplay();
      break;
    case GLUT_KEY_RIGHT:
      printf("GLUT_KEY_LEFT_UP\n");
      isKeyRight = 0;
      glutPostRedisplay();
      break;
    case GLUT_KEY_LEFT:
      printf("GLUT_KEY_RIGHT_UP\n");
      isKeyLeft = 0;
      glutPostRedisplay();
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
  if (camera_deplacement_active && !camera.mode_fps)
  {
    camera.angle_h += (double)(x-position_clique_x) * PI / window_width;
    camera.angle_v += (double)(y-position_clique_y) * PI / window_height;

    position_clique_x = x;
    position_clique_y = y;

    camera_actualiser_position();
  }
}

GLvoid window_mouvements_passifs_souris(int x, int y)
{
  camera.angle_h += (double)(x - (window_width/2)) * PI / window_width;
  camera.angle_v += (double)(y - (window_height/2)) * PI / window_height;

  camera_actualiser_position();
}

// fonction de call-back appelée régulièrement

GLvoid window_timer() 
{
  // On met à jour les variables en fonction de des touches appuyées
  if(mode == Panda)
  {
    if(isKeyUp)
    {
      t += delta*allure;
      tPosition += delta*allure;
    }
    else if(isKeyDown)
    {
      t += delta;
      tPosition -= delta;
    }

    if(isKeyRight)
    {
      if(!isKeyUp && !isKeyDown)
        t += delta;
      tCote -= delta;
    }
    else if(isKeyLeft)
    {
      if(!isKeyUp && !isKeyDown)
        t += delta;
      tCote += delta;
    }

    if(allure < Pas)
    {
      allure++;
    }
  }
  else
  {
    if(isKeyUp)
    {
      addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_observation);
    }
    else if(isKeyDown)
    {
      addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_observation);
    }

    if(isKeyRight)
    {
      addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_normal);
    }
    else if(isKeyLeft)
    {
      addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_normal);  
    }

    if(mode == Spectateur && allure > Arret)
    {
      tPosition += delta*allure;
      t += delta*allure;
    }
  }

  // On effecture une variation des angles de chaque membre
  // de l'amplitude associée et de la position médiane

  // Si on est a l'arrêt
  if((mode == Panda && !isKeyUp && !isKeyDown && !isKeyRight && !isKeyLeft))
  {
    dzArret = 0;
    angle_Corps_Arret = 0;
    angle_Tete_Arret = 0;

    t = 0;
    angle_Bras[ Gauche ] = med_Bras + sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Gauche ] = med_AvantBras + sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Gauche ] = med_Cuisse + sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Gauche ] = med_Mollet + sin(k*t)*amplitude_Mollet;

    angle_Bras[ Droit ] = med_Bras - sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Droit ] = med_AvantBras - sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Droit ] = med_Cuisse - sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Droit ] = med_Mollet - sin(k*t)*amplitude_Mollet;

    angle_Corps = med_Corps - sin(k*t)*amplitude_Corps;
    angle_Tete = med_Tete - sin(k*t)*amplitude_Tete;
  }
  // Si on est au pas
  else if(allure == Pas)
  {
    dzArret = 0;
    angle_Corps_Arret = 0;
    angle_Tete_Arret = 0;

    angle_Bras[ Gauche ] = med_Bras + sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Gauche ] = med_AvantBras + sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Gauche ] = med_Cuisse + sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Gauche ] = med_Mollet + sin(k*t)*amplitude_Mollet;

    angle_Bras[ Droit ] = med_Bras - sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Droit ] = med_AvantBras - sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Droit ] = med_Cuisse - sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Droit ] = med_Mollet - sin(k*t)*amplitude_Mollet;

    angle_Corps = med_Corps - sin(k*t)*amplitude_Corps;
    angle_Tete = med_Tete - sin(k*t)*amplitude_Tete;
  }
  // Si on est au trot
  else if(allure == Trot)
  {
    dzArret = 0;
    angle_Corps_Arret = 0;
    angle_Tete_Arret = 0;

    angle_Bras[ Gauche ] = med_Bras + sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Gauche ] = med_AvantBras + sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Gauche ] = med_Cuisse + sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Gauche ] = med_Mollet + sin(k*t)*amplitude_Mollet;

    angle_Bras[ Droit ] = med_Bras - sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Droit ] = med_AvantBras - sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Droit ] = med_Cuisse - sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Droit ] = med_Mollet - sin(k*t)*amplitude_Mollet;

    angle_Corps = med_Corps - sin(k*t)*amplitude_Corps;
    angle_Tete = med_Tete - sin(k*t)*amplitude_Tete;
  }
  // Si on est au galop
  else if(allure == Galop)
  {
    dzArret = 0;
    angle_Corps_Arret = 0;
    angle_Tete_Arret = 0;

    angle_Bras[ Gauche ] = med_Bras + sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Gauche ] = med_AvantBras + sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Gauche ] = med_Cuisse + sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Gauche ] = med_Mollet + sin(k*t)*amplitude_Mollet;

    angle_Bras[ Droit ] = med_Bras - sin(k*t)*amplitude_Bras;
    angle_AvantBras[ Droit ] = med_AvantBras - sin(k*t)*amplitude_AvantBras;
    angle_Cuisse[ Droit ] = med_Cuisse - sin(k*t)*amplitude_Cuisse;
    angle_Mollet[ Droit ] = med_Mollet - sin(k*t)*amplitude_Mollet;

    angle_Corps = med_Corps - sin(k*t)*amplitude_Corps;
    angle_Tete = med_Tete - sin(k*t)*amplitude_Tete;
  }
  else
  {
    float ampBrasG = 25 - angle_Bras[ Gauche ];
    float ampAvBrasG = -20 - angle_AvantBras[ Gauche ];
    float ampCuisseG = -20 - angle_Cuisse[ Gauche ];
    float ampMolletG = 0 - angle_Mollet[ Gauche ];

    float ampBrasD = 25 - angle_Bras[ Droit ];
    float ampAvBrasD = -20 - angle_AvantBras[ Droit ];
    float ampCuisseD = -20 - angle_Cuisse[ Droit ];
    float ampMolletD = 0 - angle_Mollet[ Droit ];

    float ampTete = 70;
    float ampCorps = -70;

    angle_Corps = 0;
    angle_Tete = 0;

    if(t < PI/2.0)
    {
      angle_Bras[ Gauche ] = sin(t)*ampBrasG;
      angle_AvantBras[ Gauche ] = sin(t)*ampAvBrasG;
      angle_Cuisse[ Gauche ] = sin(t)*ampCuisseG;
      angle_Mollet[ Gauche ] = sin(t)*ampMolletG;

      angle_Bras[ Droit ] = sin(t)*ampBrasD;
      angle_AvantBras[ Droit ] = sin(t)*ampAvBrasD;
      angle_Cuisse[ Droit ] = sin(t)*ampCuisseD;
      angle_Mollet[ Droit ] = sin(t)*ampMolletD;

      angle_Corps_Arret = sin(t)*ampCorps;
      angle_Tete_Arret = sin(t)*ampTete;
    }
    else
    {
      angle_Bras[ Gauche ] = 25;
      angle_AvantBras[ Gauche ] = -20;
      angle_Cuisse[ Gauche ] = -20;
      angle_Mollet[ Gauche ] = 0;

      angle_Bras[ Droit ] = 25;
      angle_AvantBras[ Droit ] = -20;
      angle_Cuisse[ Droit ] = -20;
      angle_Mollet[ Droit ] = 0;

      angle_Corps_Arret = -70;
      angle_Tete_Arret = 70;
    }

    t += 0.1;
    dzArret = 1.3;
  }

  // On déplace la position de l'avatar pour qu'il avance
  position = position_Ini + K*tPosition;
  positionCote = positionCote_Ini + K*tCote;
  
  if (!IdleRunning)
    glutTimerFunc(latence,&window_timer,++Step);

  glutPostRedisplay();
}

void Faire_Composantes()
{
  GLUquadricObj* qobj; /*GLAPIENTRY*/

   // allocation d´une description de quadrique
  qobj = gluNewQuadric();
  // la quadrique est pleine 
  gluQuadricDrawStyle(qobj, GLU_FILL); 
  // les ombrages, s´il y en a, sont doux
  gluQuadricNormals(qobj, GLU_SMOOTH);

  // Panda
  Ma_Tete = faire_tete_panda();
  Mon_Corps = faire_corps_panda();
  Ma_Cuisse = faire_cuisse_panda();
  Mon_Mollet = faire_mollet_panda();
  
  // Décor
  Ma_Foret = faire_foret_bambous(60, 20);
}

void dessiner_decor()
{
  int taille = 500;

  glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_herbe);
    glBegin(GL_QUADS);
      glColor3f(1, 1, 1);
      glTexCoord2i(0, 0);
      glVertex2i(-taille, -taille);
      glTexCoord2i(taille/20, 0);
      glVertex2i(taille, -taille);
      glTexCoord2i(taille/20, taille/20);
      glVertex2i(taille, taille);
      glTexCoord2i(0, taille/20);
      glVertex2i(-taille, taille);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0, 0, 5);
    glCallList(Ma_Foret);
  glPopMatrix();
}

void dessiner_panda()
{
  glPushMatrix();
    // déplacement horizontal selon l´axe Ox pour donner 
    // une impression de déplacement horizontal accompagnant
    // la marche
    glTranslatef(position, positionCote, 3.7+dzArret);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(angle_Corps_Arret, 0, 1, 0);
    glPushMatrix();
      glRotatef(angle_Tete, 1, 0, 0);
      glRotatef(angle_Tete_Arret, 0, 1, 0);
      glCallList(Ma_Tete);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-5.5, 0, -1);
      glPushMatrix();
        glRotatef(angle_Corps, 1, 0, 0);
        glCallList(Mon_Corps);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-2.6, 2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_Cuisse[Gauche], 0, 1, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_Mollet[Gauche],0,1,0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Mon_Mollet);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-2.6, -2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_Cuisse[Droit], 0, 1, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_Mollet[Droit],0,1,0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Mon_Mollet);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(2.4, 2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_Bras[Gauche], 0, 1, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_AvantBras[Gauche],0,1,0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Mon_Mollet);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(2.4, -2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_Bras[Droit], 0, 1, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(angle_AvantBras[Droit],0,1,0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Mon_Mollet);
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
}

void render_scene()
{
  dessiner_panda();

  dessiner_decor();

  // permutation des buffers lorsque le tracé est achevé
  glutSwapBuffers();
}

// load a 256x256 RGB .RAW file as a texture
GLuint LoadTextureRAW( const char * filename, int wrap )
{
    GLuint texture;
    int width, height;
    char * data;
    FILE * file;

    // open texture data
    file = fopen( filename, "rb" );
    if ( file == NULL ) return 0;

    // allocate buffer
    width = 256;
    height = 256;
    data = (char *) malloc( width * height * 3 );

    // read texture data
    fread( data, width * height * 3, 1, file );
    fclose( file );

    // allocate a texture name
    glGenTextures( 1, &texture );

    // select our current texture
    glBindTexture( GL_TEXTURE_2D, texture );

    // select modulate to mix texture with color for shading
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // when texture area is small, bilinear filter the closest mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    // when texture area is large, bilinear filter the first mipmap
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     wrap ? GL_REPEAT : GL_CLAMP );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                     wrap ? GL_REPEAT : GL_CLAMP );

    // build our texture mipmaps
    gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,
                       GL_RGB, GL_UNSIGNED_BYTE, data );

    // free buffer
    free( data );

    return texture;
}