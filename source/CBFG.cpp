// CBFG.cpp

#include "stdafx.h"
#include "FontMapClass.h"
#include "SBM-Util.h"
#include "UtilFunctions.h"
#include "CBFGDefs.h"
#include "Procs.h"
#include "file-request.h"

#define SPLASH_DELAY 1000

// Globals
HWND hMain;
HWND hText;
HINSTANCE G_Inst;
BFontMap *Fnt;
AppInfo *info;
LONG OldProc;
void Msg(char *text);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
 {
  MSG msg;
  int Ret;
  G_Inst=hInstance;
  AppInfo AppInf;
  BFontMap Bf;

  _CrtSetDbgFlag ( _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF );

  AppInf.hScr=0;
  AppInf.vScr=0;
  AppInf.Zoom=1.0f;
  AppInf.hScroll=FALSE;
  AppInf.vScroll=FALSE;
  AppInf.ModAll=TRUE;
  AppInf.Select=0;

  Fnt=&Bf;
  info=&AppInf;

  hText=CreateDialog(G_Inst,MAKEINTRESOURCE(DLG_SPLASH),NULL,(DLGPROC)SplashWinProc);
  SetWindowText(hText,"Codehead's Bitmap Font Generator");
  SetWindowPos(hText,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);

  // Load config or set defaults
  Ret=Fnt->LoadConfig("bfg.cfg");
   if(Ret==-1)
    {
     AppInf.Grid=true;
     AppInf.wMarker=true;
     Fnt->SaveConfig("bfg.cfg",true,true);
    }
   else
    {
     info->Grid=Ret & SHOW_GRID;
     info->wMarker=Ret & SHOW_WIDTH;
    }

  Sleep(SPLASH_DELAY);

  hMain=CreateDialog(G_Inst,MAKEINTRESOURCE(DLG_MAIN),NULL,(DLGPROC)MainProc);
  Sleep(SPLASH_DELAY);

  hText=CreateWindow("STATIC","TextWin",WS_POPUP,0,0,100,100,hMain,NULL,hInstance,NULL);
  Sleep(SPLASH_DELAY/2);

  OldProc=GetWindowLong(GetDlgItem(hMain,IMG_TEXT),GWL_WNDPROC);
  SetWindowLong(GetDlgItem(hMain,IMG_TEXT),GWL_WNDPROC,(LPARAM)TextWinProc);

  SetClassLong(GetDlgItem(hMain,IMG_TEXT),GCL_HBRBACKGROUND,NULL);

   // Main Message Loop
   while(GetMessage(&msg,NULL,0,0))
    {
      if(!IsDialogMessage(hMain,&msg))
       {
	       TranslateMessage(&msg); 
	       DispatchMessage(&msg);
       }
    }
   _CrtCheckMemory();
  return 0;
 }
