#include "collisions.h"

int collisionTerrain(t_panda panda, char pHeightMap[])
{
  int dx, dy;
  if(panda.direction.x >= 0)
  {
    dx = STEP_SIZE;
  }
  else
  {
    dx = -STEP_SIZE;
  }
  if(panda.direction.y >= 0)
  {
    dy = STEP_SIZE;
  }
  else
  {
    dy = STEP_SIZE;
  }
  printf("%f, %f - %f, %f : %d\n", panda.position.x, panda.position.y, panda.direction.x, panda.direction.y, Height(pHeightMap, panda.position.x, panda.position.y) - Height(pHeightMap, panda.position.x+dx, panda.position.y+dy));
  if(Height(pHeightMap, panda.position.x, panda.position.y) - Height(pHeightMap, panda.position.x+dx, panda.position.y+dy) > 1 || Height(pHeightMap, panda.position.x, panda.position.y) - Height(pHeightMap, panda.position.x+dx, panda.position.y+dy) < -1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}