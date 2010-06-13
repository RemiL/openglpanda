/*****************************************************************************
MON PANDA


par

R�mi LACROIX et Nicolas POIRIER
******************************************************************************/
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
float tCote = 0.f;
float delta = 50.f;
float k = 0.001f;
float K = 0.001f;
int IdleRunning = false;
int TraceEcran = false;
int RangFichierStockage = 0;
float position = position_Ini;
float positionCote = positionCote_Ini;

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
#define NB_BAMBOUS 20
int  Mes_Bambous[NB_BAMBOUS];
int  Mon_Morceau_Bambou2;
int  Ma_Feuille;
int  Ma_Branche;
int  Ma_Branche_Feuille;
int  Ma_Foret;
int  Mon_Corps;
int  Ma_Cuisse;
int  Mon_Mollet;

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

// Gestion cam�ra
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
  int mode_fps;
} t_camera;

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
void render_scene();
void init_angles();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y);
GLvoid window_special_key(int key, int x, int y);
GLvoid window_cliques_souris(int button, int state, int x, int y);
GLvoid window_mouvements_souris(int x, int y);
GLvoid window_mouvements_passifs_souris(int x, int y);
GLvoid window_timer(); 
void Faire_Composantes();
void faire_bambou(int liste, int hauteur, int nb_branches);
void init_camera(double x, double y, double z,
                 double vect_ob_x, double vect_ob_y, double vect_ob_z,
                 double vect_vert_x, double vect_vert_y, double vect_vert_z,
                 double pas);
void camera_activer_mode_fps(int active);
void camera_centrer_pointeur();
void camera_actualiser_position();
void addition_vectorielle(t_coordonnees *r, double coeff_a, t_coordonnees a, double coeff_b, t_coordonnees b);
double produit_scalaire(t_coordonnees a, t_coordonnees b);
void produit_vectoriel(t_coordonnees *r, t_coordonnees a, t_coordonnees b);
void normaliser_vecteur(t_coordonnees *v);
GLuint LoadTextureRAW( const char * filename, int wrap );

int main(int argc, char **argv) 
{
  srand(time(NULL));
  // initialisation  des param�tres de GLUT en fonction
  // des arguments sur la ligne de commande
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  // d�finition et cr�ation de la fen�tre graphique
  glutInitWindowSize(windowWidth,windowHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Projet OpenGL");

  // initialisation de OpenGL et de la sc�ne
  initGL();  
  init_scene();

  // choix des proc�dures de callback pour 
  // le trac� graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fen�tre
  glutReshapeFunc(&window_reshape);
  // la gestion des �v�nements clavier
  glutKeyboardFunc(&window_key);
  glutSpecialFunc(&window_special_key); // pour les touches sp�ciales (fl�ches directionnelles, ...)
  // Gestion des cliques souris
  glutMouseFunc(&window_cliques_souris);
  // Gestion des mouvements de la souris
  glutMotionFunc(&window_mouvements_souris);
  // fonction appel�e r�guli�rement entre deux gestions d��v�nements
  glutTimerFunc(latence,&window_timer,Step);

  // la boucle prinicipale de gestion des �v�nements utilisateur
  glutMainLoop();  

  return 1;
}

// initialisation du fond de la fen�tre graphique : noir opaque

GLvoid initGL() 
{
  // initialisation de l��clairement

  // d�finition de deux source lumineuses
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light0);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

  // activation de l��clairement
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  // propri�t�s mat�rielles des objets
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
  // initialise des display lists des composantes du corps
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

// fonction de call-back pour l�affichage dans la fen�tre

GLvoid window_display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Positionnement cam�ra
  gluLookAt(camera.position.x,
            camera.position.y,
            camera.position.z,
            camera.position.x + camera.vecteur_observation.x,
            camera.position.y + camera.vecteur_observation.y,
            camera.position.z + camera.vecteur_observation.z,
            camera.vecteur_def_vertical.x,
            camera.vecteur_def_vertical.y,
            camera.vecteur_def_vertical.z);
  
  // Trac� de la sc�ne
  render_scene();

  // trace la sc�ne grapnique qui vient juste d'�tre d�finie
  glFlush();
}

// fonction de call-back pour le redimensionnement de la fen�tre

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

  // toutes les transformations suivantes s�appliquent au mod�le de vue 
  glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des �v�nements clavier

GLvoid window_key(unsigned char key, int x, int y) 
{  
  switch (key)
  {
    case KEY_ESC:  
      exit(1);                    
      break; 
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
      }
      break;
    // Changement du mode de jeu
    case '0':
      if(mode == Spectateur)
      {
        mode = Panda;
        // Mettre la camera sur la tete du panda !!!!!!!!!!!!!!!
      }
      else
      {
        mode = Spectateur;
      }
      break;
    // Gestion mode cam�ra
    case 'f':
      printf("mode fps\n");
      camera_activer_mode_fps(!camera.mode_fps);
      break;
    default:
      printf ("La touche %d n�est pas active.\n", key);
      break;
  }     
}

// Gestion des touches sp�ciales
GLvoid window_special_key(int key, int x, int y)
{
  if(mode == Spectateur)
  {
    switch(key)
    {
      // Gestion mouvements cam�ra
      case GLUT_KEY_UP:
        printf("GLUT_KEY_UP\n");
        addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_observation);
        glutPostRedisplay();
        break;
      case GLUT_KEY_DOWN:
        printf("GLUT_KEY_DOWN\n");
        addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_observation);
        glutPostRedisplay();
        break;
      case GLUT_KEY_RIGHT:
        printf("GLUT_KEY_LEFT\n");
        addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_normal);
        glutPostRedisplay();
        break;
      case GLUT_KEY_LEFT:
        printf("GLUT_KEY_RIGHT\n");
        addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_normal);
        glutPostRedisplay();
        break;
      default:
        printf ("La touche %d n�est pas active.\n", key);
        break;
    }
  }
  else
  {
    switch(key)
    {
      // Gestion mouvements cam�ra
      case GLUT_KEY_UP:
        printf("GLUT_KEY_UP\n");
        addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_observation);
        if(allure < Pas)
        {
           allure++;
        }
        t += delta;
        glutPostRedisplay();
        break;
      case GLUT_KEY_DOWN:
        printf("GLUT_KEY_DOWN\n");
        addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_observation);
        if(allure < Pas)
        {
           allure++;
        }
        t -= delta;
        glutPostRedisplay();
        break;
      case GLUT_KEY_RIGHT:
        printf("GLUT_KEY_LEFT\n");
        addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_normal);
        if(allure < Pas)
        {
           allure++;
        }
        tCote -= delta;
        glutPostRedisplay();
        break;
      case GLUT_KEY_LEFT:
        printf("GLUT_KEY_RIGHT\n");
        addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_normal);
        if(allure < Pas)
        {
           allure++;
        }
        tCote += delta;
        glutPostRedisplay();
        break;
      default:
        printf ("La touche %d n�est pas active.\n", key);
        break;
    }
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

// fonction de call-back appel�e r�guli�rement

GLvoid window_timer() 
{
  // On effecture une variation des angles de chaque membre
  // de l'amplitude associ�e et de la position m�diane

  // Si on est au pas
  if(allure == Pas)
  {
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
  // Si on est � l'arr�t
  else
  {

  }

  // On d�place la position de l'avatar pour qu'il avance
  position = position_Ini + K*t;
  positionCote = positionCote_Ini + K*tCote;
  
  if (!IdleRunning)
    glutTimerFunc(latence,&window_timer,++Step);

  if(mode == Spectateur && allure > Arret)
  {
    t += delta;
  }

  glutPostRedisplay();
}

void Faire_Composantes()
{
  int i;
  GLUquadricObj* qobj; /*GLAPIENTRY*/

   // allocation d�une description de quadrique
  qobj = gluNewQuadric();
  // la quadrique est pleine 
  gluQuadricDrawStyle(qobj, GLU_FILL); 
  // les ombrages, s�il y en a, sont doux
  gluQuadricNormals(qobj, GLU_SMOOTH);

  // attribution des indentificateurs de display lists
  Ma_Tete =  glGenLists(19+NB_BAMBOUS);
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
  Mon_Morceau_Bambou2 = Ma_Tete + 12;
  Ma_Feuille = Ma_Tete + 13;
  Ma_Branche = Ma_Tete + 14;
  Ma_Branche_Feuille = Ma_Tete + 15;
  Mon_Corps = Ma_Tete + 16;
  Ma_Cuisse = Ma_Tete + 17;
  Mon_Mollet = Ma_Tete + 18;
  for (i=0; i<NB_BAMBOUS; i++)
    Mes_Bambous[i] = Mon_Mollet + i + 1;
  Ma_Foret = Mes_Bambous[NB_BAMBOUS-1] + 1;

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

  glNewList(Ma_Cuisse, GL_COMPILE);
    glPushMatrix();
      glColor3f(0.0, 0.0, 0.0);
      glScalef(0.5, 0.5, 1);
      glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
  glEndList();

  glNewList(Mon_Mollet, GL_COMPILE);
    glPushMatrix();
      glColor3f(0.0, 0.0, 0.0);
      glScalef(0.5, 0.5, 1);
      glutSolidSphere(2.5, 50, 50);
    glPopMatrix();
  glEndList();

  // compilation de la display list de la sph�re servant de t�te
  glNewList(Ma_Tete, GL_COMPILE);
    // T�te en elle m�me
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
  
  for (i=0; i<NB_BAMBOUS; i++)
    faire_bambou(Mes_Bambous[i], nb_aleatoire(7,12), nb_aleatoire(7,12)*1.7);

  glNewList(Ma_Foret, GL_COMPILE);
    for (i=0; i<3*NB_BAMBOUS; i++)
    {
      glPushMatrix();
        glTranslatef((i/10)*17+nb_aleatoire(-2,10), (i%10)*17+nb_aleatoire(-2,10), 0);
        glCallList(Mes_Bambous[nb_aleatoire(0,NB_BAMBOUS-1)]);
      glPopMatrix();
    }
  glEndList();
}

void faire_bambou(int liste, int hauteur, int nb_branches)
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

void render_scene()
{
  glPushMatrix();
    // d�placement horizontal selon l�axe Ox pour donner 
    // une impression de d�placement horizontal accompagnant
    // la marche
    glTranslatef(position, positionCote, 3.7);
    glScalef(0.5, 0.5, 0.5);
    glPushMatrix();
      glRotatef(angle_Tete, 1, 0, 0);
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

  /*glPushMatrix();
    // Position assise
    glTranslatef(0, 0, 5);
    glRotatef(-70, 0, 1, 0);
    glScalef(0.5, 0.5, 0.5);
    glPushMatrix();
      glRotatef(70, 0, 1, 0);
      glCallList(Ma_Tete);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-5.5, 0, -1);
      glPushMatrix();
        glCallList(Mon_Corps);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-2.6, 2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(-20, 0, 1, 0);
        glRotatef(20, 1, 0, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glCallList(Mon_Mollet);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(-2.6, -2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(-20, 0, 1, 0);
        glRotatef(-20, 1, 0, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glCallList(Mon_Mollet);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(2.4, 2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(20, 0, 1, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(-20,0,1,0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Mon_Mollet);
      glPopMatrix();
      glPushMatrix();
        glTranslatef(2.4, -2, -2.8);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(20, 0, 1, 0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Ma_Cuisse);
        glTranslatef(0, 0, -1.2);
        glTranslatef(0, 0, 1*0.5);
        glRotatef(-20,0,1,0);
        glTranslatef(0, 0, -1*0.5);
        glCallList(Mon_Mollet);
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();*/

  dessiner_decor();

  // permutation des buffers lorsque le trac� est achev�
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

  camera.mode_fps = false;
}

void camera_activer_mode_fps(int active)
{
  camera.mode_fps = active;
  
  if (active)
  {
    glutSetCursor(GLUT_CURSOR_NONE);
    camera_centrer_pointeur();
    glutPassiveMotionFunc(&window_mouvements_passifs_souris);
  }
  else
  {
    glutPassiveMotionFunc(NULL);
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
  }
}

void camera_centrer_pointeur()
{
  glutWarpPointer(window_width/2, window_height/2);

  if (camera.mode_fps)
    glutTimerFunc(40, &camera_centrer_pointeur, 0);
}

void camera_actualiser_position()
{
    // On limite l'angle d'inclinaison de la cam�ra � +/- PI/2
    if (camera.angle_v < -PI/2)
      camera.angle_v = -PI/2;
    else if (camera.angle_v > PI/2)
      camera.angle_v = PI/2;
    
    // Rotation par rapport au vecteur d�finissant la verticale de la cam�ra
    addition_vectorielle(&camera.vecteur_observation,
                         cos(camera.angle_h), camera.vecteur_observation_initial,
                         sin(camera.angle_h), camera.vecteur_normal_initial);
    // On recalcule le nouveau vecteur normal
    produit_vectoriel(&camera.vecteur_normal, camera.vecteur_observation, camera.vecteur_def_vertical_initial);

    // Rotation par rapport au vecteur normal de la cam�ra
    addition_vectorielle(&camera.vecteur_observation,
                         cos(camera.angle_v), camera.vecteur_observation,
                         -sin(camera.angle_v), camera.vecteur_def_vertical_initial);
    // On recalcule le nouveau vecteur d�finissant la verticale
    produit_vectoriel(&camera.vecteur_def_vertical, camera.vecteur_normal, camera.vecteur_observation);
    
    // On met � jour l'affichage
    glutPostRedisplay();
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