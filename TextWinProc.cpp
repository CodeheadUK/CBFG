#include "stdafx.h"
#include "Procs.h"
#include "CBFGDefs.h"
#include "FontMapClass.h"

extern HINSTANCE G_Inst;
extern HWND hMain;
extern BFontMap *Fnt;
extern AppInfo *info;

BOOL CALLBACK TextWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  extern long OldProc;

  int Row,Col,Sel;

   switch(msg)
    {
      case WM_ERASEBKGND:
       return TRUE;
       break;

      case WM_LBUTTONDOWN:
       SetFocus(hDlg);
       if((LOWORD(lParam)/info->Zoom)<=Fnt->GetSize(MAPWIDTH) && (HIWORD(lParam)/info->Zoom)<=Fnt->GetSize(MAPHEIGHT))
         {
          Row=(int)((HIWORD(lParam)+info->vScr)/info->Zoom)/Fnt->GetSize(CELLHEIGHT);
          Col=(int)((LOWORD(lParam)+info->hScr)/info->Zoom)/Fnt->GetSize(CELLWIDTH);

          // Limit selection
          Sel=(Row*(Fnt->GetSize(MAPWIDTH)/Fnt->GetSize(CELLWIDTH)))+Col;
           if(Sel+Fnt->GetBaseChar()>255)
            info->Select=255-Fnt->GetBaseChar();
           else
            info->Select=Sel;

           if(info->ModAll==TRUE)
            {
             SendDlgItemMessage(hMain,RAD_SEL,BM_SETCHECK,BST_CHECKED,0);
             SendDlgItemMessage(hMain,RAD_ALL,BM_SETCHECK,BST_UNCHECKED,0);
             EnableWindow(GetDlgItem(hMain,TXT_WIDTH),TRUE); 
             EnableWindow(GetDlgItem(hMain,STA_WIDTH),TRUE);
             info->ModAll=FALSE;
            }

          SendMessage(hMain,WM_APP+1,0,0);
          CreateFontMap();
         }
       return TRUE;    
    }
 
  return CallWindowProc((WNDPROC)OldProc,hDlg,msg,wParam,lParam);
 }