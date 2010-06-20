#ifndef __heightmap_h__
#define __heightmap_h__

#include <stdio.h>	

#define		MAP_SIZE	1024				// Size Of Our .RAW Height Map ( NEW )
#define		STEP_SIZE	16				// Width And Height Of Each Quad ( NEW )
#define		HEIGHT_RATIO	1.5f

void LoadRawFile(char* strName, int nSize, char *pHeightMap);
void RenderHeightMap(char pHeightMap[]);
void SetVertexColor(char *pHeightMap, int x, int y);
int Height(char *pHeightMap, int X, int Y);

#endif /* __heightmap_h__ */