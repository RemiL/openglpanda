#ifndef __camera_h__
#define __camera_h__

#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define true 1
#define false 0

#define PI 3.1415926535898

// Définition des structures de données
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

extern t_camera camera;
extern int window_width;
extern int window_height;

// Prototypes des fonctions
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

extern void window_mouvements_passifs_souris(int x, int y);

#endif /* __camera_h__ */
