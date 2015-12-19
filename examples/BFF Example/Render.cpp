// This code will be executed by the OpenGL base app's render thread
#include "Framework.h"
#include "BitmapFontClass.h"
 
void Render(void)
{
 RunLevel=1;
 CBitmapFont Font;
 float Rot=0.0f,ScreenRatio;

  // Set the background to black
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
 
   if(!Font.Load("TestFont.bff"))
    RunLevel=0;

  // Set the font color (defaults to white if not set)
  Font.SetColor(0.6f,1.0f,0.4f);

  // Set the initial display size
  glViewport(0,0,800,600);
  ScreenRatio=600.0f/800.0f;

  // This loop will run until Esc is pressed
  while(RunLevel)
   {
    if(Keys[VK_ESCAPE]) // Esc Key
      RunLevel=0;
 
    // Set up a perspective display and draw something 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(1.0f, -1.0f, -1.0f * ScreenRatio, 1.0f * ScreenRatio, 1.0f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Add some rotation
    glTranslatef(0.0f,0.0f,-2.0f);
    glRotatef(Rot,0,0,1);
    Rot-=0.5f;

    // Simple coloured triangle
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
     glColor3f(1,0,0); glVertex3f(-1.6f,-0.80f,0.0f);
     glColor3f(0,1,0); glVertex3f( 0.0f, 1.5f ,0.0f);
     glColor3f(0,0,1); glVertex3f( 1.6f,-0.80f,0.0f);
    glEnd();


    // Draw text the easy (but inefficient) way
    Font.ezPrint("I'm an ezPrint text string",250,400);


    // Draw text using FontClass functions
    Font.SetScreen(800,600);
    Font.Select();
    Font.Print("I'm a FontClass rendered string",100,100);


    // Draw text with manual setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,800,0,600,-1,1); // Ortho screen with the origin at bottom left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    Font.Bind();

    glEnable(GL_BLEND); // We know this is a 32bit font so set blending to suit
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(1.0f,1.0f,1.0f);

    Font.SetCursor(400,250);
    Font.Print("I'm a manually set up string");

    // Demo of results of incorrect Ortho handling
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,800,600,0,-1,1); // Ortho screen with the origin at top left
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Font.SetCursor(450,400);
    Font.Print("I have the wrong YAxis setting !");




    // We're using double buffers, so we need to swap to see our results
    FlipBuffers();
   }
}