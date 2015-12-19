// OpenGLBase.cpp - A multi-threaded OpenGL base application for GPWiki tutorials.
// Windows version.
 
#include "Framework.h"
 
#pragma comment (lib , "opengl32.lib") // Makes VC link the GL libs, 
#pragma comment (lib , "glu32.lib")    // other compliers will have to do it manually
 
// Globals
HINSTANCE gInst;
HWND hGLWin;
HDC  GLDC;
int  volatile RunLevel = 1;
bool Keys[256];  // Key monitor
MouseInfo Mouse; // Mouse monitor
 
bool RegisterWin();
bool StartGL(int ScrX, int ScrY, int BPP);
bool RenderProc(LPVOID lpParam);
 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
 MSG msg;
 DEVMODE ScrRes;
 HANDLE hThr;
 DWORD Res;
 
 // Set the global instance
 gInst=hInstance;
 
 // Store the current screen resolution
 EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&ScrRes);
 
 // Register our Window Class
  if(!RegisterWin())
   {
    MessageBox(NULL,"Register Window Class Failed!","Error",MB_OK | MB_ICONERROR);
    return 0;
   }
  
 // Start GL Window
  if(!StartGL(800,600,32))
   {
    MessageBox(NULL,"GL Startup Failed!","Error",MB_OK | MB_ICONERROR);
    return 0;
   }
 
  // Launch rendering thread
  hThr=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)RenderProc,0,0,NULL);
 
   if(hThr) 
    {
     RunLevel=1;
      // Main message loop:
      while(RunLevel) 
       {
        GetMessage(&msg, NULL, 0, 0);
	       TranslateMessage(&msg);
	       DispatchMessage(&msg);
       }
    }
 
 
 // Shutdown and cleanup
 
 // Wait for thread to stop
 Res=STILL_ACTIVE;
 
  while(Res==STILL_ACTIVE)
   GetExitCodeThread(hThr,&Res);
 
 // Close window
  if(hGLWin)
   DestroyWindow(hGLWin);
 
 UnregisterClass("OpenGLBaseWin",gInst);
 
 // Restore Original Screen Mode
 //ChangeDisplaySettings(&ScrRes,CDS_RESET);
 
 return (int) msg.wParam;
}
 
 
// Message Handler for our Window
LRESULT CALLBACK GLWinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
 {
  switch(Msg) 
   {
    case WM_DESTROY:
     PostQuitMessage(0);
     break;
 
    // Grab inputs
    case WM_KEYDOWN:
     Keys[wParam] = TRUE;
     return 0;
 
    case WM_KEYUP:							
     Keys[wParam] = FALSE;	
     return 0;
 
    case WM_MOUSEMOVE:
     Mouse.Mx=LOWORD(lParam);
     Mouse.My=HIWORD(lParam);
     return 0;
 
    case WM_LBUTTONDOWN:
     Mouse.Mleft=TRUE;
     return 0;
 
    case WM_LBUTTONUP:
     Mouse.Mleft=FALSE;
     return 0;
    
    case WM_RBUTTONDOWN:
     Mouse.Mright=TRUE;
     return 0;
 
    case WM_RBUTTONUP:
     Mouse.Mright=FALSE;
     return 0;
 
    default:
     return DefWindowProc(hWnd, Msg, wParam, lParam);
   }
 
  return 0;
 }
 
// Register a Window Class
bool RegisterWin()
 {
  WNDCLASSEX glWin;
 
  glWin.cbSize=sizeof(WNDCLASSEX); 
  glWin.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC; // Window has it's own context
  glWin.lpfnWndProc = GLWinProc;				         
  glWin.cbClsExtra = 0;
  glWin.cbWndExtra = 0;
  glWin.hInstance = gInst;
  glWin.hIcon = LoadIcon(NULL, IDI_WINLOGO);      // Default icon
  glWin.hCursor = LoadCursor(NULL, IDC_ARROW);    // Default pointer
  glWin.hbrBackground = NULL;
  glWin.lpszMenuName = NULL;
  glWin.lpszClassName = "OpenGLBaseWin";
  glWin.hIconSm=NULL;
 
   if(RegisterClassEx(&glWin))
    return TRUE;				
   else
    return FALSE;
 }
 
bool StartGL(int ScrX, int ScrY, int BPP)
 {
  DEVMODE ScrMode;
  PIXELFORMATDESCRIPTOR PixFmtReq;
  int PixFmt;
  HGLRC GLRC;
 
 
  // Set the screen mode
  ZeroMemory(&ScrMode,sizeof(DEVMODE));
  ScrMode.dmSize=sizeof(DEVMODE);
  ScrMode.dmPelsWidth=ScrX;
  ScrMode.dmPelsHeight=ScrY;
  ScrMode.dmBitsPerPel=BPP;
  ScrMode.dmFields=DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
 
  // if(ChangeDisplaySettings(&ScrMode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
    //return FALSE;

   // Create our window
   hGLWin=CreateWindowEx(WS_EX_LEFT,
                         "OpenGLBaseWin",
                         "OpenGL Test",
                         WS_POPUP | WS_VISIBLE,
                         0,0,ScrX,ScrY,
                         NULL,NULL,gInst,NULL);
    if(hGLWin==NULL)
     return FALSE;
     
 
  // Define pixel format for our window
  ZeroMemory(&PixFmtReq,sizeof(PIXELFORMATDESCRIPTOR));
  PixFmtReq.nSize=sizeof (PIXELFORMATDESCRIPTOR);
  PixFmtReq.nVersion=1;
  PixFmtReq.dwFlags= PFD_DRAW_TO_WINDOW |	PFD_SUPPORT_OPENGL |	PFD_DOUBLEBUFFER;
  PixFmtReq.iPixelType=PFD_TYPE_RGBA;
  PixFmtReq.cColorBits=BPP; // Color depth as specified in arguments
  PixFmtReq.cDepthBits=16;
  PixFmtReq.iLayerType=PFD_MAIN_PLANE;
 
  // Get the device context
  GLDC=GetDC(hGLWin);
 
   if(!GLDC)
    return FALSE;
 
  // Match our specified pixel format to device
  PixFmt=ChoosePixelFormat(GLDC,&PixFmtReq);
 
   if(PixFmt==0)
    return FALSE;
 
  // Set pixel format 
   if(!SetPixelFormat(GLDC,PixFmt,&PixFmtReq))
    return FALSE;
 
  // Create the OpenGL render context and bind to this thread current
  GLRC=wglCreateContext(GLDC);
 
   if(!GLRC)
    return FALSE;
 
   if(!wglMakeCurrent(GLDC,GLRC))
    return FALSE;
 
  // Clear to black
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  SwapBuffers(GLDC);
 
  return TRUE;
 }
 
// The render thread start point
bool RenderProc(LPVOID lpParam)
 {
  HGLRC glRC;
 
  // Re-aquire the context as we are in a different thread
  glRC=wglCreateContext(GLDC);
  wglMakeCurrent(GLDC,glRC);
 
  // Here's were we bring in the Render funtion
  Render();
 
  RunLevel=0; // Make sure the app stops
  return 0;
 }
 
void FlipBuffers()
{
  SwapBuffers(GLDC);
}