#include "heightmap.h"
#include "common.h"

int bRender = 1;
GLuint texture_herbe;

// Loads The .RAW File And Stores It In pHeightMap
void LoadRawFile(char* strName, int nSize, char *pHeightMap)
{
	FILE *pFile = NULL;

	// Open The File In Read / Binary Mode.
	pFile = fopen( strName, "rb" );

	// Check To See If We Found The File And Could Open It
	if ( pFile == NULL )	
	{
		return;
	}
  // Here We Load The .RAW File Into Our pHeightMap Data Array
	// We Are Only Reading In '1', And The Size Is (Width * Height)
	fread( pHeightMap, 1, nSize, pFile );

	// Close The File
	fclose(pFile);
}

int Height(char *pHeightMap, int X, int Y)			// This Returns The Height From A Height Map Index
{
	int x = (X + MAP_SIZE/2) % MAP_SIZE;					// Error Check Our x Value
	int y = (Y + MAP_SIZE/2) % MAP_SIZE;					// Error Check Our y Value

	if(!pHeightMap) return 0;				// Make Sure Our Data Is Valid

  return pHeightMap[x + (y * MAP_SIZE)];			// Index Into Our Height Array And Return The Height
}

void SetVertexColor(char *pHeightMap, int x, int y)		// This Sets The Color Value For A Particular Index
{
  //float fColor;
  // Depending On The Height Index
	if(!pHeightMap) 
    return;					// Make Sure Our Height Data Is Valid

	//fColor = (Height(pHeightMap, x, y) / 256.0f);


 /* if(Height(pHeightMap, x, y) < 0)
    glColor3f(0.0f, 0.0f, 0.0f );
  else if(Height(pHeightMap, x, y) < 50)
    glColor3f(0.0f, 1.0f, 0.0f );
  else if(Height(pHeightMap, x, y))
    glColor3f(0.5f, 0.5f, 0.5f );*/
	// Assign This Blue Shade To The Current Vertex
	glColor3f(1.0f, 1.0f, 1.0f );
}

void RenderHeightMap(char pHeightMap[])				// This Renders The Height Map As Quads
{
	int X = 0, Y = 0;					// Create Some Variables To Walk The Array With.
	int x, y, z;				// Create Some Variables For Readability

  texture_herbe = LoadTextureRAW("herbe.raw", 1, 256, 256);

  glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_herbe);

	  if(!pHeightMap) return;					// Make Sure Our Height Data Is Valid
    if(bRender)						// What We Want To Render
		  glBegin(GL_QUADS);				// Render Polygons
	  else 
		  glBegin(GL_LINES);				// Render Lines Instead
    
    for ( X = -MAP_SIZE; X < (MAP_SIZE/2-STEP_SIZE); X += STEP_SIZE )
    {
		  for ( Y = -MAP_SIZE/2; Y < (MAP_SIZE/2-STEP_SIZE); Y += STEP_SIZE )
		  {
			  // Get The (X, Y, Z) Value For The Bottom Left Vertex
			  x = X;							
			  y = Y;	
			  z = Height(pHeightMap, X, Y );							

			  // Set The Color Value Of The Current Vertex
			  SetVertexColor(pHeightMap, x, y);
        
        glTexCoord2i(0, 0);
			  glVertex3i(x, y, z);			// Send This Vertex To OpenGL To Be Rendered

			  // Get The (X, Y, Z) Value For The Top Left Vertex
			  x = X;										
			  y = Y + STEP_SIZE;
			  z = Height(pHeightMap, X, Y + STEP_SIZE );							
  			
			  // Set The Color Value Of The Current Vertex
			  SetVertexColor(pHeightMap, x, y);

        glTexCoord2i(0, 1);
			  glVertex3i(x, y, z);			// Send This Vertex To OpenGL To Be Rendered

			  // Get The (X, Y, Z) Value For The Top Right Vertex
			  x = X + STEP_SIZE; 
			  y = Y + STEP_SIZE;
			  z = Height(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE );

			  // Set The Color Value Of The Current Vertex
			  SetVertexColor(pHeightMap, x, y);
  			
        glTexCoord2i(1, 1);
			  glVertex3i(x, y, z);			// Send This Vertex To OpenGL To Be Rendered

			  // Get The (X, Y, Z) Value For The Bottom Right Vertex
			  x = X + STEP_SIZE; 
			  y = Y;
			  z = Height(pHeightMap, X + STEP_SIZE, Y );

			  // Set The Color Value Of The Current Vertex
			  SetVertexColor(pHeightMap, x, y);

        glTexCoord2i(1, 0);
			  glVertex3i(x, y, z);			// Send This Vertex To OpenGL To Be Rendered
		  }
    }
	  glEnd();
    glDisable(GL_TEXTURE_2D);
  glPopMatrix();
  //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);			// Reset The Color
}