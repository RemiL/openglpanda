/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#include "collisions.h"

void actualisation_englobant(t_rectangle *r, double x, double y, double angle)
{
  double c = cos(angle);
  double s = sin(angle);
  
  r->point[0].x = x + 2.5*c + 2*s;
  r->point[0].y = y + 2*c - 2.5*s;
  r->point[1].x = x + 2.5*c - 2*s;
  r->point[1].y = y - 2*c - 2.5*s;
  r->point[2].x = x - 6*c - 2*s;
  r->point[2].y = y - 2*c + 6*s;
  r->point[3].x = x - 6*c + 2*s;
  r->point[3].y = y + 2*c + 6*s;
}

int collision_segment_cercle(t_coordonnees A, t_coordonnees B, t_cercle cercle)
{
  double a, b, delta, x1, x2, y1, y2;

  if (A.y > B.y)
  {
    t_coordonnees tmp = A;
    
    A = B;
    B = tmp;
  }
  
  if (A.x == B.x)
  {
    delta = pow(cercle.rayon,2) - pow(A.x - cercle.centre.x,2);
   
    if (delta < 0)
      return false;
    else
    {
      y1 = cercle.centre.y + sqrt(delta);
      y2 = cercle.centre.y - sqrt(delta);
    }
  }
  else
  {
    a = (A.y - B.y) / (A.x - B.x);
    b = A.y - a*A.x;

    if (a != 0)
    {
      delta = 4*pow((b/a + cercle.centre.x)/a + cercle.centre.y,2) - 4*(1+1/pow(a,2))*(pow(b/a+cercle.centre.x,2)+pow(cercle.centre.y,2)-pow(cercle.rayon,2));
      
      if (delta < 0)
        return false;
      else
      {
        y1=(2*b+2*a*(cercle.centre.x+a*cercle.centre.y)+sqrt(delta)*pow(a,2))/(2*pow(a,2)+2);
        y2=(2*b+2*a*(cercle.centre.x+a*cercle.centre.y)-sqrt(delta)*pow(a,2))/(2*pow(a,2)+2);
      }
    }
    else
    {
      delta = pow(cercle.rayon,2)-pow(b-cercle.centre.y,2);

      if (delta < 0)
        return false;
      else
      {
        x1 = cercle.centre.x + sqrt(delta);
        x2 = cercle.centre.x - sqrt(delta);
        if (A.x > B.x)
        {
          t_coordonnees tmp = A;
          
          A = B;
          B = tmp;
        }
        
        return !((x1 < A.x || x1 > B.x) && (x2 < A.x || x2 > B.x));
      }
    }
  }

  return !((y1 < A.y || y1 > B.y) && (y2 < A.y || y2 > B.y));
}

int collision_rectangle_cercle(t_rectangle *r, t_cercle cercle)
{
  int i;

  for (i=0; i<4; i++)
  {
    if (collision_segment_cercle(r->point[i], r->point[(i+1)%4], cercle))
      return true;
  }

  return false;
}

int collision_bambous(t_rectangle *r, t_cercle *cercles, int nb_cercles)
{
  int i;

  for (i=0; i<nb_cercles; i++)
  {
    if (collision_rectangle_cercle(r, cercles[i]))
      return true;
  }

  return false;
}

int deplacement_camera_valide(t_coordonnees p)
{
  if (p.z <= 1)
    return false;

  return (sqrt(p.x*p.x + p.y*p.y + p.z*p.z) < 990);
}

int collisionTerrain(t_rectangle r, char pHeightMap[])
{
  int i;
  for(i=0; i<4; i++)
  {
    float x = r.point[i].x;
    float y = r.point[i].y;
    float z = 0;
    int ix = (int)(x);
    int iy = (int)(y);
    float dx = x - ix;
    float dy = y - iy;
    float hauteursol = ((1-dx)*Height(pHeightMap, ix, iy) + dx*Height(pHeightMap, ix+1, iy)) * (1-dy)
                     +((1-dx)*Height(pHeightMap, ix, iy+1) + dx*Height(pHeightMap, ix+1, iy+1)) * dy;

    if (hauteursol != z)
          return true;
  }
  return false;
}