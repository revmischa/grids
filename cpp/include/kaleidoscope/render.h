/*
 *  render.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/9/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */



// Our display mode settings.
//int Light_On = 0;
//int Blend_On = 0;
//int Texture_On = 0;
//int Filtering_On = 0;
//int Alpha_Add = 0;
//
//int Curr_TexMode = 0;
//char *TexModesStr[] = {"GL_DECAL","GL_MODULATE","GL_BLEND","GL_REPLACE"};
//GLint TexModes[] = {GL_DECAL,GL_MODULATE,GL_BLEND,GL_REPLACE};
//
// Object and scene global variables.
//
// Cube position and rotation speed variables.
//float X_Rot   = 0.9f;
//float Y_Rot   = 0.0f;
//float X_Speed = 0.0f;
//float Y_Speed = 0.5f;
//float Z_Off   =-5.0f;
//
// Settings for our light.  Try playing with these (or add more lights).
float Light_Ambient[]=  { 0.1f, 0.1f, 0.1f, 1.0f };
float Light_Diffuse[]=  { 1.2f, 1.2f, 1.2f, 1.0f }; 
float Light_Position[]= { 2.0f, 2.0f, 0.0f, 1.0f };


// ------
// Frames per second (FPS) statistic variables and routine.

#define FRAME_RATE_SAMPLES 50
int FrameCount=0;
float FrameRate=0;

static void ourDoFPS(
) 
{
   static clock_t last=0;
   clock_t now;
   float delta;

   if (++FrameCount >= FRAME_RATE_SAMPLES) {
      now  = clock();
      delta= (now - last) / (float) CLOCKS_PER_SEC;
      last = now;

      FrameRate = FRAME_RATE_SAMPLES / delta;
      FrameCount = 0;
   }
}


// ------
// String rendering routine; leverages on GLUT routine.

static void ourPrintString(
   void *font,
   char *str
)
{
   int i,l=strlen(str);

   for(i=0;i<l;i++)
      glutBitmapCharacter(font,*str++);
}


// ------
// Routine which actually does the drawing


// ------
// Function to build a simple full-color texture with alpha channel,
// and then create mipmaps.  This could instead load textures from
// graphics files from disk, or render textures based on external
// input.

int Texture_ID;

void ourBuildTextures(
   void
)
{
   GLenum gluerr;
   GLubyte tex[128][128][4];
   int x,y,t;
   int hole_size = 3300; // ~ == 57.45 ^ 2.

   // Generate a texture index, then bind it for future operations.
   glGenTextures(1, (GLuint*)&Texture_ID);
   glBindTexture(GL_TEXTURE_2D,Texture_ID);

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


// ------
// Callback routine executed whenever our window is resized.  Lets us
// request the newly appropriate perspective projection matrix for 
// our needs.  Try removing the gluPerspective() call to see what happens.


