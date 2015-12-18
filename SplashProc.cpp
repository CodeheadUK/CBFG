#include "stdafx.h"
#include "CBFGDefs.h"
#include "Procs.h"

extern HWND hMain;

BOOL CALLBACK SplashWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
   switch(msg)
    {
     case WM_INITDIALOG:
      SetTimer(hDlg,246,2800,NULL);
      return true;

     case WM_MOUSEACTIVATE:
      KillTimer(hDlg,246);
      EndDialog(hDlg,0);
      InvalidateRgn(hMain,NULL,true);
      return 0;

     case WM_TIMER:
      KillTimer(hDlg,246);
      EndDialog(hDlg,0);
      InvalidateRgn(hMain,NULL,true);      
      return 0;

      default:
       return 0;
    }
 }

BOOL CALLBACK AboutProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  switch(msg)
   {
     case WM_INITDIALOG:

      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Release Notes and Credits - Thanks to all those who have helped out on CBFG");
	  SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"v1.45");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Open Sourcing the code on GitHub.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"v1.44");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Erwan Bancal for finding a clipping error on fancy glyphs.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"v1.43");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Zauropod for spotting a rendering bug and missing width data from the output files.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"v1.41");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Frank Van Hooft for suggesting smaller texture size options.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"v1.4");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Almar Joling for suggesting the main window colour picker.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Sadistic Penguin for pointing out the missing data in CSV exports.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Nicolai de Hann Brøgger for spotting the memory leak during display updates.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Spodi for spotting the BMP filesize export bug.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"v1.3");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"The-Moon from GPWiki.org for persuading me to include binary font data export.");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"v1.0");
      SendDlgItemMessage(hDlg,LST_CREDITS,LB_INSERTSTRING,-1,(LPARAM)"Rene from GPWiki.org for suggesting the 'Test/Preview' idea.");
      return TRUE;
      

     case WM_COMMAND:
      {
        switch(LOWORD(wParam)) // Buttons & Menu items
         {
          case IDOK:
           EndDialog(hDlg,0);
           return TRUE;
         }
      }
   }
  return FALSE;
 }