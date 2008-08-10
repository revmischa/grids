/*
 *  renderer.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/9/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "renderer.h"


namespace Kaleidoscope
{
	Renderer::Renderer( int window_width, int window_height)
		:	width( window_width), height( window_height ), 
			Texture_On( false) , Light_On( false), Alpha_Add( false ), Blend_On( false ), Filtering_On( false )
	{
		
	
	}
	
	void Renderer::drawBox()
	{
		if (Texture_On)
		  glEnable(GL_TEXTURE_2D);
	   else
		  glDisable(GL_TEXTURE_2D);

	   if (Light_On) 
		  glEnable(GL_LIGHTING);
	   else 
		  glDisable(GL_LIGHTING);

		if (Alpha_Add)
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
		else
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		// If we're blending, we don't want z-buffering.
		if (Blend_On)
		   glDisable(GL_DEPTH_TEST); 
		else
		   glEnable(GL_DEPTH_TEST); 

		if (Filtering_On) {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_LINEAR_MIPMAP_LINEAR);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		} else {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_NEAREST_MIPMAP_NEAREST);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		}


	   // Need to manipulate the ModelView matrix to move our model around.
	   glMatrixMode(GL_MODELVIEW);

	   // Reset to 0,0,0; no rotation, no scaling.
	   glLoadIdentity(); 

	   // Move the object back from the screen.
	   //glTranslatef(0.0f,0.0f,Z_Off);

	   // Rotate the calculated amount.
	   //glRotatef(X_Rot,1.0f,0.0f,0.0f);
	   //glRotatef(Y_Rot,0.0f,1.0f,0.0f);


	   // Clear the color and depth buffers.
	   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	   // OK, let's start drawing our planer quads.
	   glBegin(GL_QUADS); 


	   // Bottom Face.  Red, 75% opaque, magnified texture
	 
	   glNormal3f( 0.0f, -1.0f, 0.0f); // Needed for lighting
	   glColor4f(0.9,0.2,0.2,.75); // Basic polygon color

	   glTexCoord2f(0.800f, 0.800f); glVertex3f(-1.0f, -1.0f, -1.0f); 
	   glTexCoord2f(0.200f, 0.800f); glVertex3f( 1.0f, -1.0f, -1.0f);
	   glTexCoord2f(0.200f, 0.200f); glVertex3f( 1.0f, -1.0f,  1.0f);
	   glTexCoord2f(0.800f, 0.200f); glVertex3f(-1.0f, -1.0f,  1.0f);


	   // Top face; offset.  White, 50% opaque.
	 
	   glNormal3f( 0.0f, 1.0f, 0.0f);  glColor4f(0.5,0.5,0.5,.5);

	   glTexCoord2f(0.005f, 1.995f); glVertex3f(-1.0f,  1.3f, -1.0f);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f(-1.0f,  1.3f,  1.0f);
	   glTexCoord2f(1.995f, 0.005f); glVertex3f( 1.0f,  1.3f,  1.0f);
	   glTexCoord2f(1.995f, 1.995f); glVertex3f( 1.0f,  1.3f, -1.0f);


	   // Far face.  Green, 50% opaque, non-uniform texture cooridinates.

	   glNormal3f( 0.0f, 0.0f,-1.0f);  glColor4f(0.2,0.9,0.2,.5); 

	   glTexCoord2f(0.995f, 0.005f); glVertex3f(-1.0f, -1.0f, -1.3f);
	   glTexCoord2f(2.995f, 2.995f); glVertex3f(-1.0f,  1.0f, -1.3f);
	   glTexCoord2f(0.005f, 0.995f); glVertex3f( 1.0f,  1.0f, -1.3f);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f( 1.0f, -1.0f, -1.3f);


	   // Right face.  Blue; 25% opaque
	   
	   glNormal3f( 1.0f, 0.0f, 0.0f);  glColor4f(0.2,0.2,0.9,.25);

	   glTexCoord2f(0.995f, 0.005f); glVertex3f( 1.0f, -1.0f, -1.0f); 
	   glTexCoord2f(0.995f, 0.995f); glVertex3f( 1.0f,  1.0f, -1.0f);
	   glTexCoord2f(0.005f, 0.995f); glVertex3f( 1.0f,  1.0f,  1.0f);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f( 1.0f, -1.0f,  1.0f);


	   // Front face; offset.  Multi-colored, 50% opaque.

	   glNormal3f( 0.0f, 0.0f, 1.0f); 

	   glColor4f( 0.9f, 0.2f, 0.2f, 0.5f);
	   glTexCoord2f( 0.005f, 0.005f); glVertex3f(-1.0f, -1.0f,  1.3f);
	   glColor4f( 0.2f, 0.9f, 0.2f, 0.5f);
	   glTexCoord2f( 0.995f, 0.005f); glVertex3f( 1.0f, -1.0f,  1.3f);
	   glColor4f( 0.2f, 0.2f, 0.9f, 0.5f);
	   glTexCoord2f( 0.995f, 0.995f); glVertex3f( 1.0f,  1.0f,  1.3f); 
	   glColor4f( 0.1f, 0.1f, 0.1f, 0.5f);
	   glTexCoord2f( 0.005f, 0.995f); glVertex3f(-1.0f,  1.0f,  1.3f);


	   // Left Face; offset.  Yellow, varying levels of opaque.
	   
	   glNormal3f(-1.0f, 0.0f, 0.0f);  
	   
	   glColor4f(0.9,0.9,0.2,0.0);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f(-1.3f, -1.0f, -1.0f); 
	   glColor4f(0.9,0.9,0.2,0.66);
	   glTexCoord2f(0.995f, 0.005f); glVertex3f(-1.3f, -1.0f,  1.0f);
	   glColor4f(0.9,0.9,0.2,1.0);
	   glTexCoord2f(0.995f, 0.995f); glVertex3f(-1.3f,  1.0f,  1.0f);
	   glColor4f(0.9,0.9,0.2,0.33);
	   glTexCoord2f(0.005f, 0.995f); glVertex3f(-1.3f,  1.0f, -1.0f);


	   // All polygons have been drawn.
	   glEnd();

	   // Move back to the origin (for the text, below).
	   glLoadIdentity();

	   // We need to change the projection matrix for the text rendering.  
	   glMatrixMode(GL_PROJECTION);

	   // But we like our current view too; so we save it here.
	   glPushMatrix();

	   // Now we set up a new projection for the text.
	   glLoadIdentity();
	 //  glOrtho(0,Window_Width,0,Window_Height,-1.0,1.0);

	   // Lit or textured text looks awful.
	   glDisable(GL_TEXTURE_2D);
	   glDisable(GL_LIGHTING);

	   // We don't want depth-testing either.
	   glDisable(GL_DEPTH_TEST); 

	   // But, for fun, let's make the text partially transparent too.
	   glColor4f(0.6,1.0,0.6,.75);

	   // Render our various display mode settings.
	  // sprintf(buf,"Mode: %s", TexModesStr[Curr_TexMode]);
	  // glRasterPos2i(2,2); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);

	  // sprintf(buf,"AAdd: %d", Alpha_Add);
	//   glRasterPos2i(2,14); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
	//
	//   sprintf(buf,"Blend: %d", Blend_On);
	//   glRasterPos2i(2,26); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
	//
	//   sprintf(buf,"Light: %d", Light_On);
	//   glRasterPos2i(2,38); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
	//
	//   sprintf(buf,"Tex: %d", Texture_On);
	//   glRasterPos2i(2,50); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
	//
	//   sprintf(buf,"Filt: %d", Filtering_On);
	//   glRasterPos2i(2,62); ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);
	//

	   // Now we want to render the calulated FPS at the top.
	   
	   // To ease, simply translate up.  Note we're working in screen
	   // pixels in this projection.
	   
	  // glTranslatef(6.0f,Window_Height - 14,0.0f);

	   // Make sure we can read the FPS section by first placing a 
	   // dark, mostly opaque backdrop rectangle.
	   glColor4f(0.2,0.2,0.2,0.75);

	   glBegin(GL_QUADS);
	   glVertex3f(  0.0f, -2.0f, 0.0f);
	   glVertex3f(  0.0f, 12.0f, 0.0f);
	   glVertex3f(140.0f, 12.0f, 0.0f);
	   glVertex3f(140.0f, -2.0f, 0.0f);
	   glEnd();

	   //glColor4f(0.9,0.2,0.2,.75);
	//   sprintf(buf,"FPS: %f F: %2d", FrameRate, FrameCount);
	//   glRasterPos2i(6,0);
	//  // ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);

	   // Done with this special projection matrix.  Throw it away.
	   glPopMatrix();

	   // All done drawing.  Let's show it.
	   glutSwapBuffers();

	   // Now let's do the motion calculations.
	  // X_Rot+=X_Speed; 
	//   Y_Rot+=Y_Speed; 

	}
	
	int Renderer::getWidth()
	{
		return width;
	}
	
	int Renderer::getHeight()
	{
		return height;
	}
	
	void Renderer::renderAll()
	{
	
	}
	
	void Renderer::prepare()
	{
		buildTextures();   

	   // Color to clear color buffer to.
	   glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	   // Depth to clear depth buffer to; type of test.
	   glClearDepth(1.0);
	   glDepthFunc(GL_LESS); 

	   // Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
	   glShadeModel(GL_SMOOTH);

	   // Load up the correct perspective matrix; using a callback directly.
	   resizeScene(width , height);

	   // Set up a light, turn it on.
	   glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
	   glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
	   glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse); 
	   glEnable (GL_LIGHT1); 

	   // A handy trick -- have surface material mirror the color.
	   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	   glEnable(GL_COLOR_MATERIAL);
	}
	
	void Renderer::buildTextures()
	{
		
		GLenum gluerr;
		GLubyte tex[128][128][4];
		int x,y,t;
		int hole_size = 3300; // ~ == 57.45 ^ 2.

		// Generate a texture index, then bind it for future operations.
		glGenTextures(1, (GLuint*)&texture_id);
		glBindTexture(GL_TEXTURE_2D,texture_id);

		// Iterate across the texture array.

		for(y=0;y<128;y++) {
		  for(x=0;x<128;x++) {

			 // A simple repeating squares pattern.
			 // Dark blue on white.

			 if ( ( (x+4)%32 < 8 ) && ( (y+4)%32 < 8)) {
				tex[x][y][0]=tex[x][y][1]=0; tex[x][y][2]=120;
			 } else {
				tex[x][y][0]=tex[x][y][1]=tex[x][y][2]=240;
			 }

					 // Make a round dot in the texture's alpha-channel.

					 // Calculate distance to center (squared).
			 t = (x-64)*(x-64) + (y-64)*(y-64) ;

			 if ( t < hole_size) // Don't take square root; compare squared.
				tex[x][y][3]=255; // The dot itself is opaque.
			 else if (t < hole_size + 100)
				tex[x][y][3]=128; // Give our dot an anti-aliased edge.
			 else
				tex[x][y][3]=0;   // Outside of the dot, it's transparent.

		  }
		}

		// The GLU library helps us build MipMaps for our texture.

		if ((gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 128, 128, GL_RGBA,
					 GL_UNSIGNED_BYTE, (void *)tex))) {

		  fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
		  exit(-1);
		}

		// Some pretty standard settings for wrapping and filtering.
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

		// We start with GL_DECAL mode.
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	}
	
	void Renderer::resizeScene( int new_width, int new_height )
	{
		// Let's not core dump, no matter what.
	   if (new_height == 0)
		  new_height = 1;

	   glViewport(0, 0, new_width, new_height);

	   glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	   gluPerspective(45.0f,(GLfloat)new_width/(GLfloat)new_height,0.1f,100.0f);

	   glMatrixMode(GL_MODELVIEW);

	   width  = new_width;
	   height = new_height;
	}
	
	void Renderer::setWindowID( int id )
	{
		window_id = id;
	}
	
	
}