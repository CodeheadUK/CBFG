#include "stdafx.h"
#include "Procs.h"
#include "CBFGDefs.h"
#include "FontMapClass.h"

extern HINSTANCE G_Inst;
extern HWND hMain;
extern BFontMap *Fnt;
extern AppInfo *info;


void SetConfigRGB(HWND Win, BFG_RGB Cols)
  {
   char Text[256];
   wsprintf(Text,"%d",Cols.Red);
   SendDlgItemMessage(Win,TXT_RED,WM_SETTEXT,0,(LPARAM)Text);
   SendDlgItemMessage(Win,SLD_RED,TBM_SETPOS,TRUE,(LPARAM)Cols.Red);
   wsprintf(Text,"%d",Cols.Green);
   SendDlgItemMessage(Win,TXT_GREEN,WM_SETTEXT,0,(LPARAM)Text);
   SendDlgItemMessage(Win,SLD_GREEN,TBM_SETPOS,TRUE,(LPARAM)Cols.Green);
   wsprintf(Text,"%d",Cols.Blue);
   SendDlgItemMessage(Win,TXT_BLUE,WM_SETTEXT,0,(LPARAM)Text);
   SendDlgItemMessage(Win,SLD_BLUE,TBM_SETPOS,TRUE,(LPARAM)Cols.Blue);
   InvalidateRect(GetDlgItem(Win,ODR_COLOR),NULL,FALSE);
  }


BOOL CALLBACK ConfigWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  HDC dc;
  RECT rcArea;
  HBRUSH hBr;
  int Sel,Val,Red,Green,Blue;
  char Text[256];
  static AppConfig lCfg;

  switch(msg)
   {
    case WM_INITDIALOG:
     // Preserve Current Config
     lCfg.CellHeight=Fnt->GetSize(CELLHEIGHT);
     lCfg.CellWidth=Fnt->GetSize(CELLWIDTH);
     lCfg.FontHeight=Fnt->GetFontHeight();
     lCfg.FontWidth=Fnt->GetFontWidth();
     lCfg.ImgWidth=Fnt->GetSize(MAPWIDTH);
     lCfg.ImgHeight=Fnt->GetSize(MAPHEIGHT);
     lCfg.Flags=0;
      if(info->Grid)
       lCfg.Flags |= SHOW_GRID;
      if(info->wMarker)
       lCfg.Flags |= SHOW_WIDTH;
     lCfg.BackCol=Fnt->GetCol(BACKCOL);
     lCfg.ForeCol=Fnt->GetCol(TEXTCOL);
     lCfg.GridCol=Fnt->GetCol(GRIDCOL);
     lCfg.WidthCol=Fnt->GetCol(WIDTHCOL);
     lCfg.SelCol=Fnt->GetCol(SELCOL);

     // Populate item combo
     SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_ADDSTRING,0,(LPARAM)"Grid Lines");
     SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_ADDSTRING,0,(LPARAM)"Width Marker");
     SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_ADDSTRING,0,(LPARAM)"Selection Marker");
     SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_ADDSTRING,0,(LPARAM)"Font Colour");
     SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_ADDSTRING,0,(LPARAM)"Background Colour");
     SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_SETCURSEL,0,0);
     
     // Set range of sliders
     SendDlgItemMessage(hDlg,SLD_RED,TBM_SETRANGE,TRUE,MAKELONG(0,255));
     SendDlgItemMessage(hDlg,SLD_GREEN,TBM_SETRANGE,TRUE,MAKELONG(0,255));
     SendDlgItemMessage(hDlg,SLD_BLUE,TBM_SETRANGE,TRUE,MAKELONG(0,255));

     // Set initial slider pos
     SetConfigRGB(hDlg,lCfg.GridCol);

     // Populate image size combos
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"16");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"32");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"64");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"128");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"256");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"512");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"1024");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"2048");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_ADDSTRING,0,(LPARAM)"4096");

     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"16");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"32");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"64");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"128");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"256");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"512");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"1024");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"2048");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_ADDSTRING,0,(LPARAM)"4096");

      if(lCfg.ImgWidth==32)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,1,0);
      else if(lCfg.ImgWidth==64)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,2,0);
      else if(lCfg.ImgWidth==128)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,3,0);
      else if(lCfg.ImgWidth==256)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,4,0);
      else if(lCfg.ImgWidth==512)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,5,0);
      else if(lCfg.ImgWidth==1024)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,6,0);
      else if(lCfg.ImgWidth==2048)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,7,0);
      else if(lCfg.ImgWidth==4096)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,8,0);
      else
       SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,0,0);

      if(lCfg.ImgHeight==32)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,1,0);
      else if(lCfg.ImgHeight==64)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,2,0);
      else if(lCfg.ImgHeight==128)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,3,0);
      else if(lCfg.ImgHeight==256)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,4,0);
      else if(lCfg.ImgHeight==512)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,5,0);
      else if(lCfg.ImgHeight==1024)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,6,0);
      else if(lCfg.ImgHeight==2048)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,7,0);
      else if(lCfg.ImgHeight==4096)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,8,0);
      else
       SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,0,0);

     // Set grid and width checks
     SendDlgItemMessage(hDlg,CHK_CFG_GRID,BM_SETCHECK,lCfg.Flags & SHOW_GRID,0);
     SendDlgItemMessage(hDlg,CHK_CFG_WIDTH,BM_SETCHECK,lCfg.Flags & SHOW_WIDTH,0);

     // Populate textboxes
     wsprintf(Text,"%d",lCfg.CellWidth);
     SendDlgItemMessage(hDlg,TXT_CFG_CELLWIDTH,WM_SETTEXT,0,(LPARAM)Text);
     wsprintf(Text,"%d",lCfg.CellHeight);
     SendDlgItemMessage(hDlg,TXT_CFG_CELLHEIGHT,WM_SETTEXT,0,(LPARAM)Text);
     wsprintf(Text,"%d",lCfg.FontHeight);
     SendDlgItemMessage(hDlg,TXT_CFG_FONTHEIGHT,WM_SETTEXT,0,(LPARAM)Text);
     wsprintf(Text,"%d",lCfg.FontWidth);
     SendDlgItemMessage(hDlg,TXT_CFG_FONTWIDTH,WM_SETTEXT,0,(LPARAM)Text);

     // Set spin ranges
     SendDlgItemMessage(hDlg,SPN_CFG_CELLWIDTH,UDM_SETRANGE,0,MAKELONG(128,8));
     SendDlgItemMessage(hDlg,SPN_CFG_CELLHEIGHT,UDM_SETRANGE,0,MAKELONG(128,8));
     SendDlgItemMessage(hDlg,SPN_CFG_FONTHEIGHT,UDM_SETRANGE,0,MAKELONG(100,1));
     SendDlgItemMessage(hDlg,SPN_CFG_FONTWIDTH,UDM_SETRANGE,0,MAKELONG(100,0));

     return 0;

    case WM_DRAWITEM:
      if(wParam==ODR_COLOR)
       {
        dc=((LPDRAWITEMSTRUCT)lParam)->hDC;
        Red=SendDlgItemMessage(hDlg,SLD_RED,TBM_GETPOS,0,0);
        Green=SendDlgItemMessage(hDlg,SLD_GREEN,TBM_GETPOS,0,0);
        Blue=SendDlgItemMessage(hDlg,SLD_BLUE,TBM_GETPOS,0,0);
        GetClientRect(hDlg, &rcArea);
        hBr=CreateSolidBrush(RGB(Red,Green,Blue));
        FillRect(dc,&rcArea,hBr);
        DeleteObject(hBr);
       }    
     return TRUE;

    case WM_CLOSE:
     EndDialog(hDlg,0);
     return 0;

    case WM_HSCROLL:
      Sel=SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_GETCURSEL,0,0);

      if((HWND)lParam==GetDlgItem(hDlg,SLD_RED))
       {
        Red=SendDlgItemMessage(hDlg,SLD_RED,TBM_GETPOS,0,0);
        wsprintf(Text,"%d",Red);
        SendDlgItemMessage(hDlg,TXT_RED,WM_SETTEXT,0,(LPARAM)Text);
        
         switch(Sel)
          {
           case GRIDCOL:
            lCfg.GridCol.Red=Red;
            break;

           case WIDTHCOL:
            lCfg.WidthCol.Red=Red;
            break;

           case SELCOL:
            lCfg.SelCol.Red=Red;
            break;

           case BACKCOL:
            lCfg.BackCol.Red=Red;
            break;

           case TEXTCOL:
            lCfg.ForeCol.Red=Red;
            break;
          }
       }

      if((HWND)lParam==GetDlgItem(hDlg,SLD_GREEN))
       {
        Green=SendDlgItemMessage(hDlg,SLD_GREEN,TBM_GETPOS,0,0);
        wsprintf(Text,"%d",Green);
        SendDlgItemMessage(hDlg,TXT_GREEN,WM_SETTEXT,0,(LPARAM)Text);
        
         switch(Sel)
          {
           case GRIDCOL:
            lCfg.GridCol.Green=Green;
            break;

           case WIDTHCOL:
            lCfg.WidthCol.Green=Green;
            break;

           case SELCOL:
            lCfg.SelCol.Green=Green;
            break;

           case BACKCOL:
            lCfg.BackCol.Green=Green;
            break;

           case TEXTCOL:
            lCfg.ForeCol.Green=Green;
            break;
          }
       }

      if((HWND)lParam==GetDlgItem(hDlg,SLD_BLUE))
       {
        Blue=SendDlgItemMessage(hDlg,SLD_BLUE,TBM_GETPOS,0,0);
        wsprintf(Text,"%d",Blue);
        SendDlgItemMessage(hDlg,TXT_BLUE,WM_SETTEXT,0,(LPARAM)Text);
        
         switch(Sel)
          {
           case GRIDCOL:
            lCfg.GridCol.Blue=Blue;
            break;

           case WIDTHCOL:
            lCfg.WidthCol.Blue=Blue;
            break;

           case SELCOL:
            lCfg.SelCol.Blue=Blue;
            break;

           case BACKCOL:
            lCfg.BackCol.Blue=Blue;
            break;

           case TEXTCOL:
            lCfg.ForeCol.Blue=Blue;
            break;
          }
       }

     InvalidateRect(GetDlgItem(hDlg,ODR_COLOR),NULL,FALSE);
     return TRUE;

    case WM_COMMAND:
     {
      switch(HIWORD(wParam))// Notifications
       {
        case EN_KILLFOCUS:
          if(LOWORD(wParam)==TXT_RED)
           {
            Sel=SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_GETCURSEL,0,0);
            SendDlgItemMessage(hDlg,TXT_RED,WM_GETTEXT,256,(LPARAM)Text);
            Val=atoi(Text);
             if(Val<0)
              {
               Val=0;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_RED,WM_SETTEXT,256,(LPARAM)Text);
              }
             if(Val>255)
              {
               Val=255; 
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_RED,WM_SETTEXT,256,(LPARAM)Text);
              }

             switch(Sel)
              {
               case GRIDCOL:
                lCfg.GridCol.Red=Val;
                break;

               case WIDTHCOL:
                lCfg.WidthCol.Red=Val;
                break;

               case SELCOL:
                lCfg.SelCol.Red=Val;
                break;

               case BACKCOL:
                lCfg.BackCol.Red=Val;
                break;

               case TEXTCOL:
                lCfg.ForeCol.Red=Val;
                break;
              }

            SendDlgItemMessage(hDlg,SLD_RED,TBM_SETPOS,TRUE,(LPARAM)Val);
            InvalidateRect(GetDlgItem(hDlg,ODR_COLOR),NULL,FALSE);
           }

          if(LOWORD(wParam)==TXT_GREEN)
           {
            Sel=SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_GETCURSEL,0,0);
            SendDlgItemMessage(hDlg,TXT_GREEN,WM_GETTEXT,256,(LPARAM)Text);
            Val=atoi(Text);
             if(Val<0)
              {
               Val=0;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_GREEN,WM_SETTEXT,256,(LPARAM)Text);
              }
             if(Val>255)
              {
               Val=255;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_GREEN,WM_SETTEXT,256,(LPARAM)Text);
              }

             switch(Sel)
              {
               case GRIDCOL:
                lCfg.GridCol.Green=Val;
                break;

               case WIDTHCOL:
                lCfg.WidthCol.Green=Val;
                break;

               case SELCOL:
                lCfg.SelCol.Green=Val;
                break;

               case BACKCOL:
                lCfg.BackCol.Green=Val;
                break;

               case TEXTCOL:
                lCfg.ForeCol.Green=Val;
                break;
              }

            SendDlgItemMessage(hDlg,SLD_GREEN,TBM_SETPOS,TRUE,(LPARAM)Val);
            InvalidateRect(GetDlgItem(hDlg,ODR_COLOR),NULL,FALSE);
           }

          if(LOWORD(wParam)==TXT_BLUE)
           {
            Sel=SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_GETCURSEL,0,0);
            SendDlgItemMessage(hDlg,TXT_BLUE,WM_GETTEXT,256,(LPARAM)Text);
            Val=atoi(Text);
             if(Val<0)
              {
               Val=0;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_BLUE,WM_SETTEXT,256,(LPARAM)Text);
              }
             if(Val>255)
              {
               Val=255;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_BLUE,WM_SETTEXT,256,(LPARAM)Text);
              }

             switch(Sel)
              {
               case GRIDCOL:
                lCfg.GridCol.Blue=Val;
                break;

               case WIDTHCOL:
                lCfg.WidthCol.Blue=Val;
                break;

               case SELCOL:
                lCfg.SelCol.Blue=Val;
                break;

               case BACKCOL:
                lCfg.BackCol.Blue=Val;
                break;

               case TEXTCOL:
                lCfg.ForeCol.Blue=Val;
                break;
              }
            SendDlgItemMessage(hDlg,SLD_BLUE,TBM_SETPOS,TRUE,(LPARAM)Val);
            InvalidateRect(GetDlgItem(hDlg,ODR_COLOR),NULL,FALSE);
           }

          if(LOWORD(wParam)==TXT_CFG_CELLWIDTH)
           {
            SendDlgItemMessage(hDlg,TXT_CFG_CELLWIDTH,WM_GETTEXT,256,(LPARAM)Text);
            Val=atoi(Text);
             if(Val<8)
              {
               Val=8;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_CELLWIDTH,WM_SETTEXT,256,(LPARAM)Text);
              }
             
             if(Val>128)
              {
               Val=128;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_CELLWIDTH,WM_SETTEXT,256,(LPARAM)Text);
              }

            lCfg.CellWidth=Val;
           }

          if(LOWORD(wParam)==TXT_CFG_CELLHEIGHT)
           {
            SendDlgItemMessage(hDlg,TXT_CFG_CELLHEIGHT,WM_GETTEXT,256,(LPARAM)Text);
            Val=atoi(Text);
             if(Val<8)
              {
               Val=8;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_CELLHEIGHT,WM_SETTEXT,256,(LPARAM)Text);
              }
            
             if(Val>128)
              {
               Val=128;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_CELLHEIGHT,WM_SETTEXT,256,(LPARAM)Text);
              }

            lCfg.CellHeight=Val;
           }

          if(LOWORD(wParam)==TXT_CFG_FONTWIDTH)
           {
            SendDlgItemMessage(hDlg,TXT_CFG_FONTWIDTH,WM_GETTEXT,256,(LPARAM)Text);
            Val=atoi(Text);
             if(Val<0)
              {
               Val=0;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_FONTWIDTH,WM_SETTEXT,256,(LPARAM)Text);
              }

             if(Val>100)
              {
               Val=100;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_FONTWIDTH,WM_SETTEXT,256,(LPARAM)Text);
              }

            lCfg.FontWidth=Val;
           }

          if(LOWORD(wParam)==TXT_CFG_FONTHEIGHT)
           {
            SendDlgItemMessage(hDlg,TXT_CFG_FONTHEIGHT,WM_GETTEXT,256,(LPARAM)Text);
            Val=atoi(Text);
             if(Val<1)
              {
               Val=1;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_FONTHEIGHT,WM_SETTEXT,256,(LPARAM)Text);
              }
            
             if(Val>100)
              {
               Val=100;
               wsprintf(Text,"%d",Val);
               SendDlgItemMessage(hDlg,TXT_CFG_FONTHEIGHT,WM_SETTEXT,256,(LPARAM)Text);
              }

            lCfg.FontHeight=Val;
           }

         return TRUE;

        case CBN_SELCHANGE:
          switch(LOWORD(wParam))
           {
            case CBO_CFG_ITEM:
             Sel=SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_GETCURSEL,0,0);

              switch(Sel)
               {
                case GRIDCOL:
                 SetConfigRGB(hDlg,lCfg.GridCol);
                 break;

                case WIDTHCOL:
                 SetConfigRGB(hDlg,lCfg.WidthCol);
                 break;

                case SELCOL:
                 SetConfigRGB(hDlg,lCfg.SelCol);
                 break;

                case BACKCOL:
                 SetConfigRGB(hDlg,lCfg.BackCol);
                 break;

                case TEXTCOL:
                 SetConfigRGB(hDlg,lCfg.ForeCol);
                 break;
               }

             InvalidateRect(GetDlgItem(hDlg,ODR_COLOR),NULL,FALSE);
             return TRUE;

            case CBO_CFG_IMGXSIZE:
             Sel=SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_GETCURSEL,0,0);
              if(Sel==0)
               {
                lCfg.ImgWidth=16;
                return TRUE;
               }
              else if(Sel==1)
               {
                lCfg.ImgWidth=32;
                return TRUE;
               }
              else if(Sel==2)
               {
                lCfg.ImgWidth=64;
                return TRUE;
               }
              else if(Sel==3)
               {
                lCfg.ImgWidth=128;
                return TRUE;
               }
              else if(Sel==4)
               {
                lCfg.ImgWidth=256;
                return TRUE;
               }
              else if(Sel==5)
               {
                lCfg.ImgWidth=512;
                return TRUE;
               }
              else if(Sel==6)
               {
                lCfg.ImgWidth=1024;
                return TRUE;
               }
              else if(Sel==7)
               {
                lCfg.ImgWidth=2048;
                return TRUE;
               }
              else if(Sel==8)
               {
                lCfg.ImgWidth=4096;
                return TRUE;
               }
              else
               return FALSE;

            case CBO_CFG_IMGYSIZE:
             Sel=SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_GETCURSEL,0,0);
              if(Sel==0)
               {
                lCfg.ImgHeight=16;
                return TRUE;
               }
              else if(Sel==1)
               {
                lCfg.ImgHeight=32;
                return TRUE;
               }
              else if(Sel==2)
               {
                lCfg.ImgHeight=64;
                return TRUE;
               }
              else if(Sel==3)
               {
                lCfg.ImgHeight=128;
                return TRUE;
               }
              else if(Sel==4)
               {
                lCfg.ImgHeight=256;
                return TRUE;
               }
              else if(Sel==5)
               {
                lCfg.ImgHeight=512;
                return TRUE;
               }
              else if(Sel==6)
               {
                lCfg.ImgHeight=1024;
                return TRUE;
               }
              else if(Sel==7)
               {
                lCfg.ImgHeight=2048;
                return TRUE;
               }
              else if(Sel==8)
               {
                lCfg.ImgHeight=4096;
                return TRUE;
               }
              else
               return FALSE;
           }
          return FALSE;
       }

      switch(LOWORD(wParam)) // Buttons
       {
        case CHK_CFG_GRID:
          if(SendDlgItemMessage(hDlg,CHK_CFG_GRID,BM_GETCHECK,0,0)==BST_CHECKED)
           lCfg.Flags |= SHOW_GRID;
          else
           lCfg.Flags ^=SHOW_GRID;
         return TRUE;

        case CHK_CFG_WIDTH:
          if(SendDlgItemMessage(hDlg,CHK_CFG_WIDTH,BM_GETCHECK,0,0)==BST_CHECKED)
           lCfg.Flags |= SHOW_WIDTH;
          else
           lCfg.Flags ^=SHOW_WIDTH;
         return TRUE;

         case ID_OK:
          // Set config
          Fnt->SetSize(CELLHEIGHT,lCfg.CellHeight);
          Fnt->SetSize(CELLWIDTH,lCfg.CellWidth);
          Fnt->SetFontHeight(lCfg.FontHeight);
          Fnt->SetFontWidth(lCfg.FontWidth);
          Fnt->SetSize(MAPWIDTH,lCfg.ImgWidth);
          Fnt->SetSize(MAPHEIGHT,lCfg.ImgHeight);
          info->Grid=lCfg.Flags & SHOW_GRID;
          info->wMarker=lCfg.Flags & SHOW_WIDTH;
          Fnt->SetCol(BACKCOL,lCfg.BackCol);
          Fnt->SetCol(TEXTCOL,lCfg.ForeCol);
          Fnt->SetCol(GRIDCOL,lCfg.GridCol);
          Fnt->SetCol(WIDTHCOL,lCfg.WidthCol);
          Fnt->SetCol(SELCOL,lCfg.SelCol);

          EndDialog(hDlg,0);
          return 0;

         case IDSAVE:
          // Set config
          Fnt->SetSize(CELLHEIGHT,lCfg.CellHeight);
          Fnt->SetSize(CELLWIDTH,lCfg.CellWidth);
          Fnt->SetFontHeight(lCfg.FontHeight);
          Fnt->SetFontWidth(lCfg.FontWidth);
          Fnt->SetSize(MAPWIDTH,lCfg.ImgWidth);
          Fnt->SetSize(MAPHEIGHT,lCfg.ImgHeight);
          info->Grid=lCfg.Flags & SHOW_GRID;
          info->wMarker=lCfg.Flags & SHOW_WIDTH;
          Fnt->SetCol(BACKCOL,lCfg.BackCol);
          Fnt->SetCol(TEXTCOL,lCfg.ForeCol);
          Fnt->SetCol(GRIDCOL,lCfg.GridCol);
          Fnt->SetCol(WIDTHCOL,lCfg.WidthCol);
          Fnt->SetCol(SELCOL,lCfg.SelCol);

           if(!Fnt->SaveConfig("bfg.cfg",info->Grid,info->wMarker))
												{
												 MessageBox(hDlg,"Unable to save config file","File Error",MB_OK|MB_ICONERROR);
													return 0;
												}

          EndDialog(hDlg,0);
          return 0;

         case IDDEFAULT:
           if(MessageBox(hDlg,"Are you sure you want to revert to the default settings?",
                              "Reset all?",MB_YESNO | MB_ICONQUESTION)==IDNO)
            return TRUE;

          lCfg.ImgWidth=256;
          SendDlgItemMessage(hDlg,CBO_CFG_IMGXSIZE,CB_SETCURSEL,2,0);

          lCfg.ImgHeight=256;
          SendDlgItemMessage(hDlg,CBO_CFG_IMGYSIZE,CB_SETCURSEL,2,0);

          lCfg.CellHeight=32;
          SendDlgItemMessage(hDlg,TXT_CFG_CELLHEIGHT,WM_SETTEXT,256,(LPARAM)"32");
          lCfg.CellWidth=32;
          SendDlgItemMessage(hDlg,TXT_CFG_CELLWIDTH,WM_SETTEXT,256,(LPARAM)"32");
          lCfg.FontHeight=20;
          SendDlgItemMessage(hDlg,TXT_CFG_FONTHEIGHT,WM_SETTEXT,256,(LPARAM)"20");
          lCfg.FontWidth=0;
          SendDlgItemMessage(hDlg,TXT_CFG_FONTWIDTH,WM_SETTEXT,256,(LPARAM)"0");

          lCfg.BackCol = MakeRGB(0,0,0);
          lCfg.ForeCol=MakeRGB(255,255,255);
          lCfg.GridCol=MakeRGB(170,0,170);
          lCfg.WidthCol=MakeRGB(170,170,0);
          lCfg.SelCol=MakeRGB(0,154,0);

          SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_SETCURSEL,0,0);

          SetConfigRGB(hDlg,lCfg.GridCol);

          lCfg.Flags=SHOW_GRID | SHOW_WIDTH;
          SendDlgItemMessage(hDlg,CHK_CFG_GRID,BM_SETCHECK,BST_CHECKED,0);
          SendDlgItemMessage(hDlg,CHK_CFG_WIDTH,BM_SETCHECK,BST_CHECKED,0);
          return 0;

         case IDCANCEL:
          EndDialog(hDlg,0);
          return 0;
       }
     }

    default:
     return 0;
   }
 }
