#include "stdafx.h"
#include "CBFGDefs.h"
#include "Procs.h"

BOOL CALLBACK SaveOptProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  static int BPP,Flags;

  switch(msg)
   {
    case WM_INITDIALOG:
     SendDlgItemMessage(hDlg,CMD_SAVEOPT_32,BM_SETCHECK,BST_CHECKED,0);
     BPP=32;
     Flags=0;
     return false;

    case WM_COMMAND:
     {
       switch(LOWORD(wParam)) // Buttons & Menu items
        {
         case IDOK:
           switch(BPP)
            {
             case 8:
               if(SendDlgItemMessage(hDlg,CHK_SAVEOPT_INVERT,BM_GETCHECK,0,0)==BST_CHECKED)
                Flags |= SAVE_INV_ALPHA;
              break;

             case 24:
              Flags=0;
              break;

             case 32:
               if(SendDlgItemMessage(hDlg,CHK_SAVEOPT_INVERT,BM_GETCHECK,0,0)==BST_CHECKED)
                Flags |= SAVE_INV_ALPHA;

               if(SendDlgItemMessage(hDlg,CHK_SAVEOPT_SATURATE,BM_GETCHECK,0,0)==BST_CHECKED)
                Flags |= SAVE_RGB_SAT;
              break;
            }
          EndDialog(hDlg,(int)(BPP | Flags));
          return true;

         case IDCANCEL:
          EndDialog(hDlg,0);
          return true;
         
         case CMD_SAVEOPT_8:
          BPP=8;
          EnableWindow(GetDlgItem(hDlg,CHK_SAVEOPT_SATURATE),false);
          SendDlgItemMessage(hDlg,CHK_SAVEOPT_SATURATE,BM_SETCHECK,BST_UNCHECKED,0);
          SendDlgItemMessage(hDlg,CHK_SAVEOPT_INVERT,WM_SETTEXT,0,(LPARAM)"Invert");

          EnableWindow(GetDlgItem(hDlg,CHK_SAVEOPT_INVERT),true);
          return true;

         case CMD_SAVEOPT_24:
          BPP=24;
          EnableWindow(GetDlgItem(hDlg,CHK_SAVEOPT_SATURATE),false);
          SendDlgItemMessage(hDlg,CHK_SAVEOPT_SATURATE,BM_SETCHECK,BST_UNCHECKED,0);

          EnableWindow(GetDlgItem(hDlg,CHK_SAVEOPT_INVERT),false);
          SendDlgItemMessage(hDlg,CHK_SAVEOPT_INVERT,BM_SETCHECK,BST_UNCHECKED,0);
          return true;

         case CMD_SAVEOPT_32:
          BPP=32;
          EnableWindow(GetDlgItem(hDlg,CHK_SAVEOPT_SATURATE),true);
          EnableWindow(GetDlgItem(hDlg,CHK_SAVEOPT_INVERT),true);
          SendDlgItemMessage(hDlg,CHK_SAVEOPT_INVERT,WM_SETTEXT,0,(LPARAM)"Invert Alpha Channel");
          return true;
        }
     }
   }
  return FALSE;
 }