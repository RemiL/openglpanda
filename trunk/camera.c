#include "camera.h"

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