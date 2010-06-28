/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#include "common.h"

#include "textures.h"
#include "camera.h"
#include "bambou.h"
#include "panda.h"
#include "heightmap.h"

#define windowWidth 600
#define windowHeight 600

#define KEY_ESC 27

#define NB_BAMBOUS 40

// Permet de savoir les touches directionnelles appuyées
int isDownKeyUp = 0;
int isDownKeyDown = 0;
int isDownKeyRight = 0;
int isDownKeyLeft = 0;

// Display lists du Panda
GLuint Ma_Tete;
GLuint Mon_Corps;
GLuint Ma_Cuisse;
GLuint Mon_Mollet;
// Display lists du décor
GLuint Mon_Sol;
GLuint Mon_Ciel;
GLuint Ma_Foret;

// Textures
GLuint texture_herbe;
GLuint texture_ciel;
GLuint texture_roche;

// Carte pour le heightmapping
char g_HeightMap[MAP_SIZE*MAP_SIZE];    // Holds The Height Map Data ( NEW )

// Variables pour la marche
enum etats{ArriereAvance = 0, AvantAvance, Recule};
enum lateralite{Gauche = 0, Droit};
enum allures{Arret = 0, Pas, Trot, Galop};
enum modes{Spectateur = 0, Panda};
int etatMarche[2] = {Recule, AvantAvance};
int allure = Arret;
int mode = Spectateur;

// Pour les mouvements du panda
t_panda panda;
float t = 0.f;
float delta = 50.f;
float k = 0.001f;
float K = 0.001f;
float angleRotationPanda;

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

// Gestion lumière
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

// Gestion taille de la fenêtre
int window_width, window_height;

// Gestion caméra
t_camera camera;
int position_clique_x, position_clique_y;
int camera_deplacement_active = false;

// Gestion collision
t_cercle *englobants_bambous;

// Gestion timer
int Step = 0;
int latence = 4;

GLvoid initGL();
GLvoid window_display(void);
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);
GLvoid window_up_key(unsigned char key, int x, int y);
GLvoid window_special_key(int key, int x, int y);
GLvoid window_special_up_key(int key, int x, int y);
GLvoid window_cliques_souris(int button, int state, int x, int y);
GLvoid window_mouvements_souris(int x, int y);
GLvoid window_mouvements_passifs_souris(int x, int y);
GLvoid window_timer(int value);
void init_scene();
void faire_composantes();
void dessiner_decor();
void dessiner_panda();
void render_scene();

int main(int argc, char **argv)
{
  srand(time(NULL));
  // initialisation  des paramètres de GLUT
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  // définition et création de la fenêtre graphique
  glutInitWindowSize(windowWidth, windowHeight);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Panda - Projet OpenGL 2010");
  
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

// initialisation des options OpenGL
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
  glClearColor(0, 0, 0, 0);
  // z-buffer
  glEnable(GL_DEPTH_TEST);
}

// Initialisation de la scène
void init_scene()
{
  // Chargement des textures
  texture_herbe = LoadTextureRAW("herbe.raw", 1, 256, 256);
  texture_ciel  = LoadTextureRAW("ciel.raw", 1, 2048, 1024);

  // Chargement de la heighmap
  LoadRawFile("Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);
  
  // initialise les display lists
  faire_composantes();

  // Initialisation camera
  init_camera(30, 0, 5, -1, 0, 0, 0, 0, 1, 1);
  
  // Initialisation Panda
  init_panda();
  
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
GLvoid window_display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  // Positionnement caméra
  if (mode == Panda)
  {
    gluLookAt(panda.position.x - 16*cos(panda.angle),
              panda.position.y + 16*sin(panda.angle),
              panda.position.z + 10,
              panda.position.x - 16*cos(panda.angle) + panda.direction.x,
              panda.position.y + 16*sin(panda.angle) + panda.direction.y,
              panda.position.z + 9.75,
              panda.vecteur_def_vertical.x,
              panda.vecteur_def_vertical.y,
              panda.vecteur_def_vertical.z);
  }
  else
  {
      gluLookAt(camera.position.x,
                camera.position.y,
                camera.position.z,
                camera.position.x + camera.vecteur_observation.x,
                camera.position.y + camera.vecteur_observation.y,
                camera.position.z + camera.vecteur_observation.z,
                camera.vecteur_def_vertical.x,
                camera.vecteur_def_vertical.y,
                camera.vecteur_def_vertical.z);
  }

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
  gluPerspective(70, (float) width / height, 1, 2000);

  // toutes les transformations suivantes s´appliquent au modèle de vue
  glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des événements clavier
GLvoid window_key(unsigned char key, int x, int y) 
{  
  switch (key)
  {
    case KEY_ESC:
      exit(0);
      break;
    default:
      //printf ("La touche %d n´est pas active.\n", key);
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
        if(allure == Pas)
          t = -PI/2.0;
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
        isDownKeyUp = 0;
        isDownKeyDown = 0;
        isDownKeyRight = 0;
        isDownKeyLeft = 0;
      }
      else
      {
        mode = Spectateur;
        allure = Arret;
      }
      break;
    // Gestion mode caméra
    case 'f':
      camera_activer_mode_fps(!camera.mode_fps);
      break;
    // Action devant un bambou
    case 13:
      printf("ENTER\n");
      break;
    default:
      //printf ("La touche %d n´est pas active.\n", key);
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
      isDownKeyUp = 1;
      isDownKeyDown = 0;
      break;
    case GLUT_KEY_DOWN:
      isDownKeyDown = 1;
      isDownKeyUp = 0;
      break;
    case GLUT_KEY_RIGHT:
      isDownKeyRight = 1;
      isDownKeyLeft = 0;
      break;
    case GLUT_KEY_LEFT:
      isDownKeyLeft = 1;
      isDownKeyRight = 0;
      break;
    default:
      //printf ("La touche %d n´est pas active.\n", key);
      break;
  }
}

// Gestion du relâchement des touches spéciales
GLvoid window_special_up_key(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP:
      isDownKeyUp = 0;
      break;
    case GLUT_KEY_DOWN:
      isDownKeyDown = 0;
      break;
    case GLUT_KEY_RIGHT:
      isDownKeyRight = 0;
      break;
    case GLUT_KEY_LEFT:
      isDownKeyLeft = 0;
      break;
    default:
      //printf ("La touche %d n´est pas active.\n", key);
      break;
  }
}

// Fonction de gestion des clics souris
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
    if (mode == Panda)
    {
      panda.angle += (double)(x-position_clique_x) * PI / 2 / window_width;

      panda_actualiser_position();

      // Verification collision
      if(collision_bambous(&panda.englobant, englobants_bambous, NB_BAMBOUS) || collisionTerrain(panda.box, g_HeightMap))
      {
        panda.angle -= (double)(x-position_clique_x) * PI / 2 / window_width;
        panda_actualiser_position();
      }
    }
    else
    {
      camera.angle_h += (double)(x-position_clique_x) * PI / window_width;
      camera.angle_v += (double)(y-position_clique_y) * PI / window_height;

      camera_actualiser_position();
    }

    position_clique_x = x;
    position_clique_y = y;
  }
}

GLvoid window_mouvements_passifs_souris(int x, int y)
{
  if (mode == Panda)
  {
    panda.angle += (double)(x - (window_width/2)) * PI / 2 / window_width;
    
    panda_actualiser_position();

    // Verification collision
    if(collision_bambous(&panda.englobant, englobants_bambous, NB_BAMBOUS) || collisionTerrain(panda.box, g_HeightMap))
    {
      panda.angle -= (double)(x - (window_width/2)) * PI / 2 / window_width;
      panda_actualiser_position();
    }
  }
  else
  {
    camera.angle_h += (double)(x - (window_width/2)) * PI / window_width;
    camera.angle_v += (double)(y - (window_height/2)) * PI / window_height;
    
    camera_actualiser_position();
  }
}

// fonction de call-back appelée régulièrement
GLvoid window_timer(int value) 
{
  // On met à jour les variables en fonction de des touches appuyées
  if(mode == Panda)
  {
    angleRotationPanda = -180 * panda.angle / PI;
    
    if(isDownKeyUp)
    {
      t += delta*allure;

      // On déplace la position de l'avatar pour qu'il avance
      panda.position.x += K*delta*allure*cos(panda.angle);
      panda.position.y -= K*delta*allure*sin(panda.angle);
      // Déplacement englobant
      actualisation_englobant(&panda.englobant, panda.position.x, panda.position.y, panda.angle);
      // Verification collision
      if(collision_bambous(&panda.englobant, englobants_bambous, NB_BAMBOUS) || collisionTerrain(panda.box, g_HeightMap))
      {
        panda.position.x -= K*delta*allure*cos(panda.angle);
        panda.position.y += K*delta*allure*sin(panda.angle);
      }
    }
    else if(isDownKeyDown)
    {
      t += delta;

      panda.position.x -= K*delta*cos(panda.angle);
      panda.position.y += K*delta*sin(panda.angle);
      // Déplacement englobant
      actualisation_englobant(&panda.englobant, panda.position.x, panda.position.y, panda.angle);
      // Verification collision
      if(collision_bambous(&panda.englobant, englobants_bambous, NB_BAMBOUS) || collisionTerrain(panda.box, g_HeightMap))
      {
        panda.position.x += K*delta*cos(panda.angle);
        panda.position.y -= K*delta*sin(panda.angle);
      }
    }

    if(isDownKeyRight)
    {
      if(!isDownKeyUp && !isDownKeyDown)
        t += delta;
      panda.position.x -= K*delta*sin(panda.angle);
      panda.position.y -= K*delta*cos(panda.angle);
      // Déplacement englobant
      actualisation_englobant(&panda.englobant, panda.position.x, panda.position.y, panda.angle);
      // Verification collision
      if(collision_bambous(&panda.englobant, englobants_bambous, NB_BAMBOUS) || collisionTerrain(panda.box, g_HeightMap))
      {
        panda.position.x += K*delta*sin(panda.angle);
        panda.position.y += K*delta*cos(panda.angle);
      }
    }
    else if(isDownKeyLeft)
    {
      if(!isDownKeyUp && !isDownKeyDown)
        t += delta;

      panda.position.x += K*delta*sin(panda.angle);
      panda.position.y += K*delta*cos(panda.angle);
      // Déplacement englobant
      actualisation_englobant(&panda.englobant, panda.position.x, panda.position.y, panda.angle);
      // Verification collision
      if(collision_bambous(&panda.englobant, englobants_bambous, NB_BAMBOUS) || collisionTerrain(panda.box, g_HeightMap))
      {
        panda.position.x -= K*delta*sin(panda.angle);
        panda.position.y -= K*delta*cos(panda.angle);
      }
    }

    if(allure < Pas)
    {
      allure++;
    }
  }
  else
  {
    if(isDownKeyUp)
    {
      addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_observation);
      if (!deplacement_camera_valide(camera.position))
        addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_observation);
    }
    else if(isDownKeyDown)
    {
      addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_observation);
      if (!deplacement_camera_valide(camera.position))
        addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_observation);
    }

    if(isDownKeyRight)
    {
      addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_normal);
      if (!deplacement_camera_valide(camera.position))
        addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_normal);
    }
    else if(isDownKeyLeft)
    {
      addition_vectorielle(&camera.position, 1, camera.position, -camera.pas, camera.vecteur_normal);
      if (!deplacement_camera_valide(camera.position))
        addition_vectorielle(&camera.position, 1, camera.position, camera.pas, camera.vecteur_normal);
    }

    if(mode == Spectateur && allure > Arret)
    {
      panda.position.x += K*delta*allure*cos(panda.angle);
      panda.position.y -= K*delta*allure*sin(panda.angle);
      // Déplacement englobant
      actualisation_englobant(&panda.englobant, panda.position.x, panda.position.y, panda.angle);
      // Verification collision
      if(collision_bambous(&panda.englobant, englobants_bambous, NB_BAMBOUS) || collisionTerrain(panda.box, g_HeightMap))
      {
        panda.position.x -= K*delta*allure*cos(panda.angle);
        panda.position.y += K*delta*allure*sin(panda.angle);
        allure = Arret;
      }
      t += delta*allure;
    }
  }

  // On effecture une variation des angles de chaque membre
  // de l'amplitude associée et de la position médiane

  // Si on est a l'arrêt
  if((mode == Panda && !isDownKeyUp && !isDownKeyDown && !isDownKeyRight && !isDownKeyLeft))
  {
    panda.position.z = 0;
    angle_Corps_Arret = 0;
    angle_Tete_Arret = 0;

    t = 0;
    angle_Bras[ Gauche ] = med_Bras;
    angle_AvantBras[ Gauche ] = med_AvantBras;
    angle_Cuisse[ Gauche ] = med_Cuisse;
    angle_Mollet[ Gauche ] = med_Mollet;

    angle_Bras[ Droit ] = med_Bras;
    angle_AvantBras[ Droit ] = med_AvantBras;
    angle_Cuisse[ Droit ] = med_Cuisse;
    angle_Mollet[ Droit ] = med_Mollet;

    angle_Corps = med_Corps;
    angle_Tete = med_Tete;
  }
  // Si on est au pas
  else if(allure == Pas)
  {
    if(angle_Corps_Arret != 0)
    {
      float ampBrasG = -25;
      float ampAvBrasG = 20;
      float ampCuisseG = 20;
      float ampMolletG = 0;

      float ampBrasD = -25;
      float ampAvBrasD = 20;
      float ampCuisseD = 20;
      float ampMolletD = 0;

      float ampTete = -70;
      float ampCorps = 70;
      float ampCorpsHauteur = -1.3;
      
      angle_Corps = 0;
      angle_Tete = 0;

      t -= delta*allure;

      if(t < 0)
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

        panda.position.z = sin(t)*ampCorpsHauteur;
      }
      else
      {
        angle_Bras[ Gauche ] = 0;
        angle_AvantBras[ Gauche ] = 0;
        angle_Cuisse[ Gauche ] = 0;
        angle_Mollet[ Gauche ] = 0;

        angle_Bras[ Droit ] = 0;
        angle_AvantBras[ Droit ] = 0;
        angle_Cuisse[ Droit ] = 0;
        angle_Mollet[ Droit ] = 0;

        angle_Corps_Arret = 0;
        angle_Tete_Arret = 0;

        panda.position.z = 0;
      }

      t += 0.1;
    }
    else
    {
      panda.position.z = 0;
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
  }
  // Si on est au trot
  else if(allure == Trot)
  {
    panda.position.z = 0;
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
    panda.position.z = 0;
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
    float ampBrasG = 25;
    float ampAvBrasG = -20;
    float ampCuisseG = -20;
    float ampMolletG = 0;

    float ampBrasD = 25;
    float ampAvBrasD = -20;
    float ampCuisseD = -20;
    float ampMolletD = 0;

    float ampTete = 70;
    float ampCorps = -70;
    float ampCorpsHauteur = 1.4;

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

      if(t < PI/4.0)
      {
        panda.position.z = sin(2*t)*ampCorpsHauteur;
      }
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

      panda.position.z = 1.3;
    }

    t += 0.1;
  }
  
  glutTimerFunc(latence,&window_timer,++Step);

  glutPostRedisplay();
}

void faire_composantes()
{
  int taille = 1000;
  GLUquadricObj* qobj;

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
  Ma_Foret = faire_foret_bambous(NB_BAMBOUS, 20, g_HeightMap, &englobants_bambous);

  Mon_Sol = glGenLists(2);
  Mon_Ciel = Mon_Sol + 1;
  
  /*
  // Non utilisé pour cause de heightmap
  glNewList(Mon_Sol, GL_COMPILE);
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
  glEndList();
  */

  glNewList(Mon_Sol, GL_COMPILE);
    glPushMatrix();
      RenderHeightMap(g_HeightMap);
    glPopMatrix();
  glEndList();
  
  glNewList(Mon_Ciel, GL_COMPILE);
    glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture_ciel);
      gluQuadricTexture(qobj, GL_TRUE);
      gluSphere(qobj, taille, 30, 30);
      glDisable(GL_TEXTURE_2D);
    glPopMatrix();
  glEndList();
}

void dessiner_decor()
{
  glPushMatrix();
    glCallList(Ma_Foret);
    glCallList(Mon_Sol);
    glCallList(Mon_Ciel);
  glPopMatrix();
}

void dessiner_panda()
{
  glPushMatrix();
    // Translation pour la marche
    glTranslatef(panda.position.x, panda.position.y, 3.7+panda.position.z);
    // Rotation pour faire tourner le panda
    glRotatef(angleRotationPanda, 0, 0, 1);
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
  GLUquadricObj* qobj;

  dessiner_panda();
  dessiner_decor();

  // permutation des buffers lorsque le tracé est achevé
  glutSwapBuffers();
}