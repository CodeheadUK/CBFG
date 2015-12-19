#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
 
// Functions
void FlipBuffers();
void Render();
 
// A struct to hold mouse data
struct MouseInfo
{  
  int Mx,My;
  bool Mleft, Mright;
};
 
// Globals
extern int volatile RunLevel;
extern bool Keys[256];  // Key monitor
extern MouseInfo Mouse; // Mouse monitor 