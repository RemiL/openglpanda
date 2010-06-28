/*****************************************************************************
MON PANDA - Projet Informatique Graphique 2010

par

Rémi LACROIX et Nicolas POIRIER
******************************************************************************/
#ifndef __heightmap_h__
#define __heightmap_h__

#include "common.h"
#include "textures.h"

// Taille de la carte au format RAW
#define MAP_SIZE 1024
// Taille d'une "unité" de la carte
#define STEP_SIZE 16

void LoadRawFile(char* strName, int nSize, char *pHeightMap);
void RenderHeightMap(char pHeightMap[]);
void SetVertexColor(char *pHeightMap, int x, int y);
int Height(char *pHeightMap, int X, int Y);
void textureChoix(char pHeightMap[], int x, int y);

#endif /* __heightmap_h__ */