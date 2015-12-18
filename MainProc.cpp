#include "stdafx.h"
#include "CBFGDefs.h"
#include "Procs.h"
#include "FontMapClass.h"
#include "file-request.h"
#include "WriteFont.h"
#include <shellapi.h>

// Required includes for Win2000
#include "Ws2tcpip.h"
#include "Wspiapi.h"

extern HINSTANCE G_Inst;
extern HWND hMain;
extern BFontMap *Fnt;
extern AppInfo *info;
extern bool cfgChange;

int CALLBACK EnumFontMgr(ENUMLOGFONTEX *lpelfe,NEWTEXTMETRICEX *lpntme,int FontType,LPARAM lParam)
 {
  SendDlgItemMessage(hMain,CBO_FONTS,CB_ADDSTRING,0,(LPARAM)lpelfe->elfFullName);
  return 1;
 }

BOOL CALLBACK MainProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  HDC dc;
  LOGFONT fDef;
  char Text[256];
  int RowDex,Index,Start;
  int tVal,BPPVal,Flags,RetVal;
  SCROLLINFO scrInf;
  float Version;
  string VerData,VerNum;
  RECT rcArea;
  HBRUSH hBr;
  BFG_RGB ColVal;
  CHOOSECOLOR SelCol;
  static COLORREF CustCol[16]; // array of custom colors for color picker

   switch(msg)
  	 {
	    case WM_INITDIALOG:
      SendMessage(hDlg,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(G_Inst,MAKEINTRESOURCE(MAKEINTRESOURCE(APP_ICON))));
      SendDlgItemMessage(hDlg,CMD_UP,BM_SETIMAGE,IMAGE_ICON,(LPARAM)LoadIcon(G_Inst,MAKEINTRESOURCE(ICO_UP)));
      SendDlgItemMessage(hDlg,CMD_DOWN,BM_SETIMAGE,IMAGE_ICON,(LPARAM)LoadIcon(G_Inst,MAKEINTRESOURCE(ICO_DOWN)));
      SendDlgItemMessage(hDlg,CMD_RIGHT,BM_SETIMAGE,IMAGE_ICON,(LPARAM)LoadIcon(G_Inst,MAKEINTRESOURCE(ICO_RIGHT)));
      SendDlgItemMessage(hDlg,CMD_LEFT,BM_SETIMAGE,IMAGE_ICON,(LPARAM)LoadIcon(G_Inst,MAKEINTRESOURCE(ICO_LEFT)));
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"16");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"32");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"64");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"128");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"256");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"512");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"1024");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"2048");
      SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_ADDSTRING,0,(LPARAM)"4096");

      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"16");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"32");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"64");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"128");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"256");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"512");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"1024");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"2048");
      SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_ADDSTRING,0,(LPARAM)"4096");

      tVal=Fnt->GetSize(MAPWIDTH);
       if(tVal==32)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,1,0);
       else if(tVal==64)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,2,0);
       else if(tVal==128)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,3,0);
       else if(tVal==256)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,4,0);
       else if(tVal==512)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,5,0);
       else if(tVal==1024)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,6,0);
       else if(tVal==2048)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,7,0);
       else if(tVal==4096)
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,8,0);
       else
        SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,0,0);

      tVal=Fnt->GetSize(MAPHEIGHT);
       if(tVal==32)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,1,0);
       if(tVal==64)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,2,0);
       if(tVal==128)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,3,0);
       else if(tVal==256)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,4,0);
       else if(tVal==512)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,5,0);
       else if(tVal==1024)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,6,0);
       else if(tVal==2048)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,7,0);
       else if(tVal==4096)
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,8,0);
       else
        SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,0,0);

      SendDlgItemMessage(hDlg,CBO_ALIAS,CB_ADDSTRING,0,(LPARAM)"None");
      SendDlgItemMessage(hDlg,CBO_ALIAS,CB_ADDSTRING,0,(LPARAM)"Normal Anti-Alias");
      SendDlgItemMessage(hDlg,CBO_ALIAS,CB_ADDSTRING,0,(LPARAM)"ClearType (WinXP Only)");
      SendDlgItemMessage(hDlg,CBO_ALIAS,CB_SETCURSEL,0,0);

      SendDlgItemMessage(hDlg,CBO_ZOOM,CB_ADDSTRING,0,(LPARAM)"25%");
      SendDlgItemMessage(hDlg,CBO_ZOOM,CB_ADDSTRING,0,(LPARAM)"50%");
      SendDlgItemMessage(hDlg,CBO_ZOOM,CB_ADDSTRING,0,(LPARAM)"100%");
      SendDlgItemMessage(hDlg,CBO_ZOOM,CB_ADDSTRING,0,(LPARAM)"200%");
      SendDlgItemMessage(hDlg,CBO_ZOOM,CB_ADDSTRING,0,(LPARAM)"400%");
      SendDlgItemMessage(hDlg,CBO_ZOOM,CB_SETCURSEL,2,0);

      wsprintf(Text,"%d",Fnt->GetSize(CELLWIDTH));
      SendDlgItemMessage(hDlg,TXT_CELLWIDTH,WM_SETTEXT,0,(LPARAM)Text);
      wsprintf(Text,"%d",Fnt->GetSize(CELLHEIGHT));
      SendDlgItemMessage(hDlg,TXT_CELLHEIGHT,WM_SETTEXT,0,(LPARAM)Text);
      wsprintf(Text,"%d",Fnt->GetFontWidth());
      SendDlgItemMessage(hDlg,TXT_FONTWIDTH,WM_SETTEXT,0,(LPARAM)Text);
      wsprintf(Text,"%d",Fnt->GetFontHeight());
      SendDlgItemMessage(hDlg,TXT_FONTHEIGHT,WM_SETTEXT,0,(LPARAM)Text);

      SendDlgItemMessage(hDlg,SPN_CELLWIDTH,UDM_SETRANGE,0,MAKELONG(256,8));
      SendDlgItemMessage(hDlg,SPN_CELLHEIGHT,UDM_SETRANGE,0,MAKELONG(256,8));
      SendDlgItemMessage(hDlg,SPN_FONTHEIGHT,UDM_SETRANGE,0,MAKELONG(256,1));
      SendDlgItemMessage(hDlg,SPN_FONTWIDTH,UDM_SETRANGE,0,MAKELONG(256,0));
      SendDlgItemMessage(hDlg,SPN_WIDTH,UDM_SETRANGE,0,MAKELONG(100,-100));
      SendDlgItemMessage(hDlg,SPN_START,UDM_SETRANGE,0,MAKELONG(254,0));

      SendDlgItemMessage(hDlg,RAD_ALL,BM_SETCHECK,BST_CHECKED,0);

      info->MaxChars=Fnt->GetSize(MAXCHARS);

      PostMessage(hDlg,WM_APP,0,0);
	    return TRUE;

    case WM_DRAWITEM:
      if(wParam==ODR_FORECOL)
       {
        dc=((LPDRAWITEMSTRUCT)lParam)->hDC;
        ColVal=Fnt->GetCol(TEXTCOL);
        GetClientRect(hDlg, &rcArea);
        hBr=CreateSolidBrush(RGB(ColVal.Red,ColVal.Green,ColVal.Blue));
        FillRect(dc,&rcArea,hBr);
        DeleteObject(hBr);
       }    

      if(wParam==ODR_BACKCOL)
       {
        dc=((LPDRAWITEMSTRUCT)lParam)->hDC;
        ColVal=Fnt->GetCol(BACKCOL);
        GetClientRect(hDlg, &rcArea);
        hBr=CreateSolidBrush(RGB(ColVal.Red,ColVal.Green,ColVal.Blue));
        FillRect(dc,&rcArea,hBr);
        DeleteObject(hBr);
       }    

      CreateFontMap();
     return TRUE;

     case WM_APP:
      SendDlgItemMessage(hDlg,CBO_FONTS,CB_RESETCONTENT,0,0);
      fDef.lfCharSet=ANSI_CHARSET;
      fDef.lfFaceName[0]=NULL;
      fDef.lfPitchAndFamily=0;

      dc=GetDC(hMain);

      EnumFontFamiliesEx(dc,&fDef,(FONTENUMPROC)EnumFontMgr,0,0);
      ReleaseDC(hMain,dc);

      SendDlgItemMessage(hDlg,CBO_FONTS,CB_SETCURSEL,0,0);
      SendDlgItemMessage(hDlg,CBO_FONTS,CB_GETLBTEXT,0,(LPARAM)Text);
      Fnt->SetFontName(Text);

       if(info->Grid)
        {
         SendDlgItemMessage(hMain,CHK_GRID,BM_SETCHECK,BST_CHECKED,0);
         CheckMenuItem(GetMenu(hMain),ID_VIEW_SHOWGRID,MF_CHECKED);
        }
       else
        {
         SendDlgItemMessage(hMain,CHK_GRID,BM_SETCHECK,BST_UNCHECKED,0);
         CheckMenuItem(GetMenu(hMain),ID_VIEW_SHOWGRID,MF_UNCHECKED);
        }

       if(info->wMarker)
        {
         SendDlgItemMessage(hDlg,CHK_WIDTH,BM_SETCHECK,BST_CHECKED,0);
         CheckMenuItem(GetMenu(hMain),ID_VIEW_WIDTHMARKERS,MF_CHECKED);
        }
       else
        {
         SendDlgItemMessage(hDlg,CHK_WIDTH,BM_SETCHECK,BST_UNCHECKED,0);
         CheckMenuItem(GetMenu(hMain),ID_VIEW_WIDTHMARKERS,MF_UNCHECKED);
        }

      EnableWindow(GetDlgItem(hMain,SCR_HOR),FALSE);
      EnableWindow(GetDlgItem(hMain,SCR_VERT),FALSE);

      Fnt->SetBaseChar(32);
      wsprintf(Text,"%d",Fnt->GetBaseChar());
      SendDlgItemMessage(hDlg,TXT_START,WM_SETTEXT,0,(LPARAM)Text);

      SendMessage(hMain,WM_APP+1,0,0);
      EnableWindow(GetDlgItem(hMain,TXT_WIDTH),FALSE);
      EnableWindow(GetDlgItem(hMain,STA_WIDTH),FALSE);

      CalcScroll();
      CreateFontMap();
      return FALSE;

     case WM_APP+1: // Control Update
       if(info->ModAll==TRUE)
        {
         wsprintf(Text,"%d",Fnt->GetGlobal(HOFFSET));
         SendDlgItemMessage(hMain,TXT_XADJ,WM_SETTEXT,0,(LPARAM)Text);
         wsprintf(Text,"%d",Fnt->GetGlobal(VOFFSET));
         SendDlgItemMessage(hMain,TXT_YADJ,WM_SETTEXT,0,(LPARAM)Text);
         wsprintf(Text,"%d",Fnt->GetGlobal(WIDTH));
         SendDlgItemMessage(hMain,TXT_WADJ,WM_SETTEXT,0,(LPARAM)Text);
         SendDlgItemMessage(hMain,TXT_WIDTH,WM_SETTEXT,0,(LPARAM)"");
        }
       else
        {
         wsprintf(Text,"%d",Fnt->GetCharVal(info->Select+Fnt->GetBaseChar(),HOFFSET));
         SendDlgItemMessage(hMain,TXT_XADJ,WM_SETTEXT,0,(LPARAM)Text);
         wsprintf(Text,"%d",Fnt->GetCharVal(info->Select+Fnt->GetBaseChar(),VOFFSET));
         SendDlgItemMessage(hMain,TXT_YADJ,WM_SETTEXT,0,(LPARAM)Text);
         wsprintf(Text,"%d",Fnt->GetCharVal(info->Select+Fnt->GetBaseChar(),WOFFSET));
         SendDlgItemMessage(hMain,TXT_WADJ,WM_SETTEXT,0,(LPARAM)Text);
         wsprintf(Text,"%d",Fnt->GetCharVal(info->Select+Fnt->GetBaseChar(),EWIDTH));
         SendDlgItemMessage(hMain,TXT_WIDTH,WM_SETTEXT,0,(LPARAM)Text);
         wsprintf(Text,"Adjust Selection (%d) Only",info->Select+Fnt->GetBaseChar());
         SendDlgItemMessage(hMain,RAD_SEL,WM_SETTEXT,0,(LPARAM)Text);
        }
      return TRUE;

     case WM_CLOSE:
  	  case WM_DESTROY:
	     EndDialog(hDlg,0);
      PostQuitMessage(0);
	     return TRUE;

     case WM_HSCROLL:
      {
       switch(LOWORD(wParam))
        {
         case SB_THUMBTRACK:
          SetScrollPos((HWND)lParam,SB_CTL,HIWORD(wParam),TRUE);
          info->hScr=HIWORD(wParam);
          CreateFontMap();
          return 0;

         case SB_LINELEFT:
           if(info->hScroll==FALSE)
            return 0;
          info->hScr-=8;
           if(info->hScr<0)
            info->hScr=0;
          SetScrollPos(GetDlgItem(hMain,SCR_HOR),SB_CTL,info->hScr,TRUE);
          CreateFontMap();
          return 0;

         case SB_LINERIGHT:
           if(info->hScroll==FALSE)
            return 0;
          info->hScr+=8;
          scrInf.cbSize=sizeof(SCROLLINFO);
          scrInf.fMask=SIF_RANGE;
          GetScrollInfo(GetDlgItem(hMain,SCR_HOR),SB_CTL,&scrInf);
           if(info->hScr>scrInf.nMax)
            info->hScr=scrInf.nMax;
          SetScrollPos(GetDlgItem(hMain,SCR_HOR),SB_CTL,info->hScr,TRUE);
          CreateFontMap();
          return 0;

         case SB_PAGELEFT:
           info->hScr-=24;
           SetScrollPos(GetDlgItem(hMain,SCR_HOR),SB_CTL,info->hScr,TRUE);
           CreateFontMap();
          return 0;

         case SB_PAGERIGHT:
           info->hScr+=24;
           SetScrollPos(GetDlgItem(hMain,SCR_HOR),SB_CTL,info->hScr,TRUE);
           CreateFontMap();
          return 0;
        }

       return FALSE;
      }

     case WM_VSCROLL:
      {
       switch(LOWORD(wParam))
        {
         case SB_THUMBTRACK:
          SetScrollPos((HWND)lParam,SB_CTL,HIWORD(wParam),TRUE);
          info->vScr=HIWORD(wParam);
          CreateFontMap();
          return 0;

         case SB_LINEUP:
           if(info->vScroll==FALSE)
            return 0;
          info->vScr-=8;
           if(info->vScr<0)
            info->vScr=0;
          SetScrollPos(GetDlgItem(hMain,SCR_VERT),SB_CTL,info->vScr,TRUE);
          CreateFontMap();
          return 0;

         case SB_LINEDOWN:
           if(info->vScroll==FALSE)
            return 0;
          info->vScr+=8;
          scrInf.cbSize=sizeof(SCROLLINFO);
          scrInf.fMask=SIF_RANGE;
          GetScrollInfo(GetDlgItem(hMain,SCR_VERT),SB_CTL,&scrInf);
           if(info->vScr>scrInf.nMax)
            info->vScr=scrInf.nMax;
          SetScrollPos(GetDlgItem(hMain,SCR_VERT),SB_CTL,info->vScr,TRUE);
          CreateFontMap();
          return 0;

         case SB_PAGEDOWN:
          info->vScr+=24;
          SetScrollPos(GetDlgItem(hMain,SCR_VERT),SB_CTL,info->vScr,TRUE);
          CreateFontMap();
          return 0;

         case SB_PAGEUP:
          info->vScr-=24;
          SetScrollPos(GetDlgItem(hMain,SCR_VERT),SB_CTL,info->vScr,TRUE);
          CreateFontMap();
          return 0;
        }

       return FALSE;
      }

     case WM_NOTIFY:
      {
       NMUPDOWN *Hdr;
       Hdr=(LPNMUPDOWN) lParam;

        if(Hdr->hdr.code==UDN_DELTAPOS)
         {
          switch(Hdr->hdr.idFrom)
           {
            case SPN_CELLHEIGHT:
             Fnt->SetSize(CELLHEIGHT,Hdr->iPos+Hdr->iDelta);
             info->MaxChars=Fnt->GetSize(MAXCHARS);
             info->Select=LimitSelection(info->Select,info->MaxChars);
             CreateFontMap();
             return 0;

            case SPN_CELLWIDTH:
             Fnt->SetSize(CELLWIDTH,Hdr->iPos+Hdr->iDelta);
             info->MaxChars=Fnt->GetSize(MAXCHARS);
             info->Select=LimitSelection(info->Select,info->MaxChars);
             CreateFontMap();
             return 0;

            case SPN_FONTHEIGHT:
             Fnt->SetFontHeight(Hdr->iPos+Hdr->iDelta);
             CreateFontMap();
             return 0;

            case SPN_FONTWIDTH:
             Fnt->SetFontWidth(Hdr->iPos+Hdr->iDelta);
             CreateFontMap();
             return 0;

            case SPN_WIDTH:
              if(info->ModAll)
               {
                Fnt->SetGlobal(WIDTH,Hdr->iPos+Hdr->iDelta);
                CreateFontMap();
               }
              else
               {
                Fnt->SetCharVal(info->Select+Fnt->GetBaseChar(),WOFFSET,Hdr->iPos+Hdr->iDelta);
                wsprintf(Text,"%d",Fnt->GetCharVal(info->Select+Fnt->GetBaseChar(),EWIDTH));
                SendDlgItemMessage(hMain,TXT_WIDTH,WM_SETTEXT,0,(LPARAM)Text);
                CreateFontMap();
               }
              return 0;

            case SPN_START:
              if(Hdr->iPos>0)
               Fnt->SetBaseChar(Hdr->iPos+Hdr->iDelta);

              if(Fnt->GetBaseChar()+info->Select>255)
               info->Select=255-Fnt->GetBaseChar();

             SendMessage(hMain,WM_APP+1,0,0);
             CreateFontMap();
             return 0;

           }
         }
       return 0;
       break;
      }

     case WM_COMMAND:
      {
        switch(LOWORD(wParam)) // Buttons & Menu items
         {
          case ID_COLOUR_SETTEXTCOLOUR:
          case ODR_FORECOL:
           ColVal=Fnt->GetCol(TEXTCOL);
           SelCol.lStructSize=sizeof(CHOOSECOLOR);
           SelCol.hwndOwner=hDlg;
           SelCol.rgbResult=RGB(ColVal.Red,ColVal.Green,ColVal.Blue);
           SelCol.lpCustColors=(LPDWORD)CustCol;
           SelCol.Flags=CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR;
            if(ChooseColor(&SelCol))
             Fnt->SetCol(TEXTCOL,GetRValue(SelCol.rgbResult),GetGValue(SelCol.rgbResult),GetBValue(SelCol.rgbResult));

           InvalidateRgn(hDlg,NULL,NULL);
           return TRUE;

          case ID_COLOUR_SETBACKGROUNDCOLOUR:
          case ODR_BACKCOL:
           ColVal=Fnt->GetCol(BACKCOL);
           SelCol.lStructSize=sizeof(CHOOSECOLOR);
           SelCol.hwndOwner=hDlg;
           SelCol.rgbResult=RGB(ColVal.Red,ColVal.Green,ColVal.Blue);
           SelCol.lpCustColors=(LPDWORD)CustCol;
           SelCol.Flags=CC_FULLOPEN | CC_RGBINIT | CC_ANYCOLOR;
            if(ChooseColor(&SelCol))
             Fnt->SetCol(BACKCOL,GetRValue(SelCol.rgbResult),GetGValue(SelCol.rgbResult),GetBValue(SelCol.rgbResult));

           InvalidateRgn(hDlg,NULL,NULL);
           return TRUE;

          case ID_VIEW_SHOWGRID:
          case CHK_GRID:
           info->Grid^=1;
            if(info->Grid)
             {
              SendDlgItemMessage(hMain,CHK_GRID,BM_SETCHECK,BST_CHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_SHOWGRID,MF_CHECKED);
             }
            else
             {
              SendDlgItemMessage(hMain,CHK_GRID,BM_SETCHECK,BST_UNCHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_SHOWGRID,MF_UNCHECKED);
             }
           CreateFontMap();
           return TRUE;

          case ID_VIEW_WIDTHMARKERS:
          case CHK_WIDTH:
           info->wMarker^=1;
            if(info->wMarker)
             {
              SendDlgItemMessage(hMain,CHK_WIDTH,BM_SETCHECK,BST_CHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_WIDTHMARKERS,MF_CHECKED);
             }
            else
             {
              SendDlgItemMessage(hMain,CHK_WIDTH,BM_SETCHECK,BST_UNCHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_WIDTHMARKERS,MF_UNCHECKED);
             }
           CreateFontMap();
           return TRUE;

          case CHK_BOLD:
            if(Fnt->GetFontWeight()==FW_NORMAL)
             Fnt->SetFontWeight(FW_BOLD);
            else
             Fnt->SetFontWeight(FW_NORMAL);
           CreateFontMap();
           return TRUE;

          case CHK_ITAL:
            if(Fnt->GetFontItalic())
             Fnt->SetFontItalic(FALSE);
            else
             Fnt->SetFontItalic(TRUE);
           CreateFontMap();
           return TRUE;

          case CMD_LEFT:
            if(info->ModAll)
             {
              tVal=Fnt->GetGlobal(HOFFSET);
              Fnt->SetGlobal(HOFFSET,tVal-1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
            else
             {
              tVal=Fnt->GetCharVal(Fnt->GetBaseChar()+info->Select,HOFFSET);
              Fnt->SetCharVal(Fnt->GetBaseChar()+info->Select,HOFFSET,tVal-1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
           CreateFontMap();
           return TRUE;

          case CMD_RIGHT:
            if(info->ModAll)
             {
              tVal=Fnt->GetGlobal(HOFFSET);
              Fnt->SetGlobal(HOFFSET,tVal+1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
            else
             {
              tVal=Fnt->GetCharVal(Fnt->GetBaseChar()+info->Select,HOFFSET);
              Fnt->SetCharVal(Fnt->GetBaseChar()+info->Select,HOFFSET,tVal+1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
           CreateFontMap();
           return TRUE;

          case CMD_UP:
            if(info->ModAll)
             {
              tVal=Fnt->GetGlobal(VOFFSET);
              Fnt->SetGlobal(VOFFSET,tVal-1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
            else
             {
              tVal=Fnt->GetCharVal(Fnt->GetBaseChar()+info->Select,VOFFSET);
              Fnt->SetCharVal(Fnt->GetBaseChar()+info->Select,VOFFSET,tVal-1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
           CreateFontMap();
           return TRUE;

          case CMD_DOWN:
            if(info->ModAll)
             {
              tVal=Fnt->GetGlobal(VOFFSET);
              Fnt->SetGlobal(VOFFSET,tVal+1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
            else
             {
              tVal=Fnt->GetCharVal(Fnt->GetBaseChar()+info->Select,VOFFSET);
              Fnt->SetCharVal(Fnt->GetBaseChar()+info->Select,VOFFSET,tVal+1);
              SendMessage(hMain,WM_APP+1,0,0);
             }
           CreateFontMap();
           return TRUE;

          case RAD_ALL:
           info->ModAll=TRUE;
           EnableWindow(GetDlgItem(hMain,TXT_WIDTH),FALSE);
           EnableWindow(GetDlgItem(hMain,STA_WIDTH),FALSE);
           SendDlgItemMessage(hMain,RAD_SEL,WM_SETTEXT,0,(LPARAM)"Adjust Selection Only");
           SendMessage(hMain,WM_APP+1,0,0);
           CreateFontMap();
           return TRUE;

          case RAD_SEL:
           info->ModAll=FALSE;
           SendMessage(hMain,WM_APP+1,0,0);
           EnableWindow(GetDlgItem(hMain,TXT_WIDTH),TRUE);
           EnableWindow(GetDlgItem(hMain,STA_WIDTH),TRUE);
           wsprintf(Text,"Adjust Selection (%d) Only",info->Select+Fnt->GetBaseChar());
           SendDlgItemMessage(hMain,RAD_SEL,WM_SETTEXT,0,(LPARAM)Text);
           CreateFontMap();
           return TRUE;

          case ID_FILE_RESET:
           Flags=Fnt->LoadConfig("bfg.cfg");
           tVal=Fnt->GetSize(MAPWIDTH);
            if(tVal==32)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,1,0);
            else if(tVal==64)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,2,0);
            else if(tVal==128)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,3,0);
            else if(tVal==256)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,4,0);
            else if(tVal==512)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,5,0);
            else if(tVal==1024)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,6,0);
            else if(tVal==2048)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,7,0);
            else if(tVal==4096)
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,8,0);
            else
             SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_SETCURSEL,0,0);

           tVal=Fnt->GetSize(MAPHEIGHT);
            if(tVal==32)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,1,0);
            else if(tVal==64)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,2,0);
            else if(tVal==128)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,3,0);
            else if(tVal==256)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,4,0);
            else if(tVal==512)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,5,0);
            else if(tVal==1024)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,6,0);
            else if(tVal==2048)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,7,0);
            else if(tVal==4096)
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,8,0);
            else
             SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_SETCURSEL,0,0);

           wsprintf(Text,"%d",Fnt->GetSize(CELLHEIGHT));
           SendDlgItemMessage(hMain,TXT_CELLHEIGHT,WM_SETTEXT,0,(LPARAM)Text);

           wsprintf(Text,"%d",Fnt->GetSize(CELLWIDTH));
           SendDlgItemMessage(hMain,TXT_CELLWIDTH,WM_SETTEXT,0,(LPARAM)Text);

           info->MaxChars=Fnt->GetSize(MAXCHARS);

           info->hScr=0;
           info->vScr=0;
           info->Zoom=1.0f;
           SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,1,0);

            if(Flags & SHOW_GRID)
             {
              info->Grid=true; 
              SendDlgItemMessage(hMain,CHK_GRID,BM_SETCHECK,BST_CHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_SHOWGRID,MF_CHECKED);
             }
            else
             {
              info->Grid=false;
              SendDlgItemMessage(hMain,CHK_GRID,BM_SETCHECK,BST_UNCHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_SHOWGRID,MF_UNCHECKED);
             }

            if(Flags & SHOW_WIDTH)
             {
              info->wMarker=true;
              SendDlgItemMessage(hDlg,CHK_WIDTH,BM_SETCHECK,BST_CHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_WIDTHMARKERS,MF_CHECKED);
             }
            else
             {
              info->wMarker=false;
              SendDlgItemMessage(hDlg,CHK_WIDTH,BM_SETCHECK,BST_UNCHECKED,0);
              CheckMenuItem(GetMenu(hMain),ID_VIEW_WIDTHMARKERS,MF_UNCHECKED);
             }

           SendDlgItemMessage(hMain,CBO_FONTS,CB_SETCURSEL,0,0);
           SendDlgItemMessage(hDlg,CBO_FONTS,CB_GETLBTEXT,0,(LPARAM)Text);
           Fnt->SetFontName(Text);

           Fnt->SetBaseChar(32);
           wsprintf(Text,"%d",32);
           SendDlgItemMessage(hMain,TXT_START,WM_SETTEXT,0,(LPARAM)Text);
           
           wsprintf(Text,"%d",Fnt->GetFontHeight());
           SendDlgItemMessage(hMain,TXT_FONTHEIGHT,WM_SETTEXT,0,(LPARAM)Text);

           wsprintf(Text,"%d",Fnt->GetFontWidth());
           SendDlgItemMessage(hMain,TXT_FONTWIDTH,WM_SETTEXT,0,(LPARAM)Text);

           Fnt->SetFontWeight(FW_NORMAL);
           SendDlgItemMessage(hMain,CHK_BOLD,BM_SETCHECK,BST_UNCHECKED,0);

           Fnt->SetFontItalic(FALSE);
           SendDlgItemMessage(hMain,CHK_ITAL,BM_SETCHECK,BST_UNCHECKED,0);

           Fnt->SetFontQuality(NONANTIALIASED_QUALITY);
           SendDlgItemMessage(hDlg,CBO_ALIAS,CB_SETCURSEL,0,0);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NONE,MF_CHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NORMAL,MF_UNCHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_CLEARTYPE,MF_UNCHECKED);

           Fnt->ResetOffsets();
           info->ModAll=TRUE;
           info->Select=0;
           SendDlgItemMessage(hMain,RAD_ALL,BM_SETCHECK,BST_CHECKED,0);
           SendDlgItemMessage(hMain,RAD_SEL,BM_SETCHECK,BST_UNCHECKED,0);
           SendDlgItemMessage(hMain,RAD_SEL,WM_SETTEXT,0,(LPARAM)"Adjust Selection Only");
           EnableWindow(GetDlgItem(hMain,TXT_WIDTH),FALSE);
           EnableWindow(GetDlgItem(hMain,STA_WIDTH),FALSE);
           SendMessage(hMain,WM_APP+1,0,0);

           CreateFontMap();
           return TRUE;

          case ID_FILE_SAVEBFF:
           lstrcpy(Text,Fnt->GetFontName());
           lstrcat(Text,".bff");
            if(GetTargetName(Text,"Save BFF","Bitmap Font Files (BFF)\0*.bff\0All Files\0*.*\0\0","bff"))
             {
               if(CheckOverwrite(Text))
                {
                 tVal=DialogBox(G_Inst,MAKEINTRESOURCE(DLG_SAVEOPT),hMain,SaveOptProc);
                  if(tVal)
                   {
                    // Extract BPP and pre-processing flags from dialog return value
					               BPPVal=tVal & 0x3F;

                     switch(BPPVal)
                      {
                       case 8:
                        RetVal=Fnt->SaveFont(SAVE_BFF8,Text,tVal);
                        break;

                       case 24:
                        RetVal=Fnt->SaveFont(SAVE_BFF24,Text);
                        break;

                       case 32:
                        RetVal=Fnt->SaveFont(SAVE_BFF32,Text,tVal);
                        break;
                      }

                    if(RetVal)
                     MessageBox(hDlg,"Save Complete","File Operation",MB_OK);
                    else
                     MessageBox(hDlg,"Save Failed","Error",MB_OK | MB_ICONEXCLAMATION);
                   }
                }
             }
           return TRUE;

          case ID_IMPORT_FONTDATA:
           Text[0]=NULL;
            if(GetSourceName(Text,"Import Font Data","Font Data Files (CSV)\0*.csv\0All Files\0*.*\0\0","csv"))
             { 
               if(Fnt->ImportData(Text))
                {
                 // Set font face
                 wsprintf(Text,"%d",Fnt->GetFontName());
                 Index=SendDlgItemMessage(hMain,CBO_FONTS,CB_FINDSTRING,-1,(LPARAM)Text);

                 // Set Start Char
                 wsprintf(Text,"%d",Fnt->GetBaseChar());
                 SendDlgItemMessage(hMain,TXT_START,WM_SETTEXT,0,(LPARAM)Text);

                 // Set Bold Checkbox
                  if(Fnt->GetFontWeight()==FW_NORMAL)
                   SendDlgItemMessage(hMain,CHK_BOLD,BM_SETCHECK,BST_UNCHECKED,0);
                  else
                   SendDlgItemMessage(hMain,CHK_BOLD,BM_SETCHECK,BST_CHECKED,0);

                 // Set Italic Checkbox
                  if(Fnt->GetFontItalic())
                   SendDlgItemMessage(hMain,CHK_ITAL,BM_SETCHECK,BST_CHECKED,0);
                  else
                   SendDlgItemMessage(hMain,CHK_ITAL,BM_SETCHECK,BST_UNCHECKED,0);

                 CreateFontMap();
                }
               else
                {
                 MessageBox(hDlg,"Import Failed","Error",MB_OK | MB_ICONEXCLAMATION);
                }
             }
           return TRUE;

          case ID_EXPORT_BITMAP:
           lstrcpy(Text,"ExportedFont.bmp");
            if(GetTargetName(Text,"Export BMP","Bitmap Images (BMP)\0*.bmp\0All Files\0*.*\0\0","bmp"))
             {
               if(CheckOverwrite(Text))
                {
                 if(Fnt->ExportMap(Text,EXPORT_BMP)==SBM_OK)
                  MessageBox(hDlg,"Export Complete","BMP Export",MB_OK);
                 else
                  MessageBox(hDlg,"Export Failed","Error",MB_OK | MB_ICONEXCLAMATION);
                }
             }
           return TRUE;

          case ID_EXPORT_TARGA:
           lstrcpy(Text,"ExportedFont.tga");
            if(GetTargetName(Text,"Export TGA","Targa Images (TGA)\0*.tga\0All Files\0*.*\0\0","tga"))
             {
               if(CheckOverwrite(Text))
                {
                  if(Fnt->ExportMap(Text,EXPORT_TGA)==SBM_OK)
                   MessageBox(hDlg,"Export Complete","TGA Export",MB_OK);
                  else
                   MessageBox(hDlg,"Export Failed","Error",MB_OK | MB_ICONEXCLAMATION);
                }
             }
           return TRUE;

          case ID_EXPORT_TARGA32:
           lstrcpy(Text,"ExportedFont.tga");
            if(GetTargetName(Text,"Export TGA","Targa Images (TGA)\0*.tga\0All Files\0*.*\0\0","tga"))
             {
               if(CheckOverwrite(Text))
                {
                  if(Fnt->ExportMap(Text,EXPORT_TGA32)==SBM_OK)
                   MessageBox(hDlg,"Export Complete","TGA Export",MB_OK);
                  else
                   MessageBox(hDlg,"Export Failed","Error",MB_OK | MB_ICONEXCLAMATION);
                }
             }
           return TRUE;


          case ID_EXPORT_FONTDATA:
           lstrcpy(Text,"FontData.csv");
            if(GetTargetName(Text,"Export Font Data","Comma Separated Values (CSV)\0*.csv\0All Files\0*.*\0\0","csv"))
             {
               if(CheckOverwrite(Text))
                {
                  if(Fnt->SaveFont(SAVE_CSV,Text))
                   MessageBox(hDlg,"Export Complete","Font Data Export",MB_OK);
                  else
                   MessageBox(hDlg,"Export Failed","Error",MB_OK | MB_ICONEXCLAMATION);
                }
             }
           return TRUE;

          case ID_EXPORT_BIN:
           lstrcpy(Text,"FontData.dat");
            if(GetTargetName(Text,"Export Binary Font Data","Binary Font Files (dat)\0*.dat\0All Files\0*.*\0\0","dat"))
             {
               if(CheckOverwrite(Text))
                {
                 if(Fnt->SaveFont(SAVE_BIN,Text))
                   MessageBox(hDlg,"Export Complete","Font Data Export",MB_OK);
                  else
                   MessageBox(hDlg,"Export Failed","Error",MB_OK | MB_ICONEXCLAMATION);
                }
             }
           return TRUE;
           break;

          case ID_FILE_EXIT:
           EndDialog(hDlg,0);
           PostQuitMessage(0);
	          return TRUE;

          case ID_VIEW_ZOOMIN:
           RowDex=SendDlgItemMessage(hMain,CBO_ZOOM,CB_GETCURSEL,0,0);
            switch(RowDex)
             {
              case 0:
                info->Zoom=0.5f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,1,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;

              case 1:
                info->Zoom=1.0f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,2,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;

              case 2:
                info->Zoom=2.0f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,3,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;


              case 3:
                info->Zoom=4.0f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,4,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;
             }
           return TRUE;

          case ID_VIEW_ZOOMOUT:
           RowDex=SendDlgItemMessage(hMain,CBO_ZOOM,CB_GETCURSEL,0,0);
            switch(RowDex)
             {
              case 1:
                info->Zoom=0.25f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,0,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;

              case 2:
                info->Zoom=0.5f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,1,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;

              case 3:
                info->Zoom=1.0f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,2,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;

              case 4:
                info->Zoom=2.0f;
                SendDlgItemMessage(hMain,CBO_ZOOM,CB_SETCURSEL,3,0);
                CalcScroll();
                CreateFontMap();
               return TRUE;
             }
           return TRUE;

          case ID_ANTIALIAS_NONE:
           SendDlgItemMessage(hDlg,CBO_ALIAS,CB_SETCURSEL,0,0);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NONE,MF_CHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NORMAL,MF_UNCHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_CLEARTYPE,MF_UNCHECKED);
           Fnt->SetFontQuality(NONANTIALIASED_QUALITY);
           CreateFontMap();
           return TRUE;

          case ID_ANTIALIAS_NORMAL:
           SendDlgItemMessage(hDlg,CBO_ALIAS,CB_SETCURSEL,1,0);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NONE,MF_UNCHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NORMAL,MF_CHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_CLEARTYPE,MF_UNCHECKED);
           Fnt->SetFontQuality(ANTIALIASED_QUALITY);
           CreateFontMap();
           return TRUE;

          case ID_ANTIALIAS_CLEARTYPE:
           SendDlgItemMessage(hDlg,CBO_ALIAS,CB_SETCURSEL,2,0);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NONE,MF_UNCHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NORMAL,MF_UNCHECKED);
           CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_CLEARTYPE,MF_CHECKED);
           Fnt->SetFontQuality(5); // CLEARTYPE_QUALITY;
           CreateFontMap();
           return TRUE;

          case ID_TOOLS_PREVIEW:
           DialogBox(G_Inst,MAKEINTRESOURCE(DLG_PREVIEW),hDlg,PreviewWinProc);
           return TRUE;

          case ID_TOOLS_CONFIGURATION:
           DialogBox(G_Inst,MAKEINTRESOURCE(DLG_CONFIG),hDlg,ConfigWinProc);
           info->MaxChars=Fnt->GetSize(MAXCHARS);
           info->Select=LimitSelection(info->Select,info->MaxChars);
           SendMessage(hMain,WM_APP+1,0,0);
           InvalidateRgn(hDlg,NULL,NULL);
           CreateFontMap();
           return TRUE;

          case ID_HELP_CONTENTS:
            if((int)ShellExecute(hDlg,"open","CBFGHelp.chm",NULL,NULL,SW_SHOWMAXIMIZED)<32)
             MessageBox(hDlg,"Unable to open Help file","Error",MB_OK | MB_ICONERROR);
           return TRUE;

          case ID_HELP_ABOUT:
           DialogBox(G_Inst,MAKEINTRESOURCE(DLG_ABOUT),hMain,AboutProc);
           return TRUE;

         } // End Switch LOWORD(wParam)

        switch(HIWORD(wParam)) // Notifications
         {
          case EN_KILLFOCUS:
            switch(LOWORD(wParam))
             {
              case TXT_CELLWIDTH:
               SendDlgItemMessage(hDlg,TXT_CELLWIDTH,WM_GETTEXT,256,(LPARAM)Text);
               tVal=Fnt->SetSize(CELLWIDTH,atoi(Text));
			            wsprintf(Text,"%d",tVal);
			            SendDlgItemMessage(hDlg,TXT_CELLWIDTH,WM_SETTEXT,0,(LPARAM)Text);
               info->MaxChars=Fnt->GetSize(MAXCHARS);
               CreateFontMap();
               return TRUE;

              case TXT_CELLHEIGHT:
               SendDlgItemMessage(hDlg,TXT_CELLHEIGHT,WM_GETTEXT,256,(LPARAM)Text);
               tVal=Fnt->SetSize(CELLHEIGHT,atoi(Text));
	              wsprintf(Text,"%d",tVal);
			            SendDlgItemMessage(hDlg,TXT_CELLHEIGHT,WM_SETTEXT,0,(LPARAM)Text);
               info->MaxChars=Fnt->GetSize(MAXCHARS);
               CreateFontMap();
               return TRUE;

              case TXT_FONTWIDTH:
               SendDlgItemMessage(hDlg,TXT_FONTWIDTH,WM_GETTEXT,256,(LPARAM)Text);
               tVal=Fnt->SetFontWidth(atoi(Text));
			            wsprintf(Text,"%d",tVal);
			            SendDlgItemMessage(hDlg,TXT_FONTWIDTH,WM_SETTEXT,0,(LPARAM)Text);
               CreateFontMap();
               return TRUE;

              case TXT_FONTHEIGHT:
               SendDlgItemMessage(hDlg,TXT_FONTHEIGHT,WM_GETTEXT,256,(LPARAM)Text);
               tVal=Fnt->SetFontHeight(atoi(Text));
	              wsprintf(Text,"%d",tVal);
			            SendDlgItemMessage(hDlg,TXT_FONTHEIGHT,WM_SETTEXT,0,(LPARAM)Text);
               CreateFontMap();
               return TRUE;

              case TXT_START:
               SendDlgItemMessage(hDlg,TXT_START,WM_GETTEXT,256,(LPARAM)Text);
               tVal=Fnt->SetBaseChar(atoi(Text));
	              wsprintf(Text,"%d",tVal);
			            SendDlgItemMessage(hDlg,TXT_START,WM_SETTEXT,0,(LPARAM)Text);
               CreateFontMap();
               return TRUE;

              case TXT_XADJ:
               if(info->ModAll)
                {
                 SendDlgItemMessage(hDlg,TXT_XADJ,WM_GETTEXT,256,(LPARAM)Text);
                 tVal=Limit(atoi(Text));
                 tVal=Fnt->SetGlobal(HOFFSET,tVal);
  	              wsprintf(Text,"%d",tVal);
		  	            SendDlgItemMessage(hDlg,TXT_XADJ,WM_SETTEXT,0,(LPARAM)Text);
                 CreateFontMap();
                }
               else
                {
                 SendDlgItemMessage(hDlg,TXT_XADJ,WM_GETTEXT,256,(LPARAM)Text);
                 tVal=Limit(atoi(Text));
                 tVal=Fnt->SetCharVal(info->Select+Fnt->GetBaseChar(),HOFFSET,tVal);
  	              wsprintf(Text,"%d",tVal);
		  	            SendDlgItemMessage(hDlg,TXT_XADJ,WM_SETTEXT,0,(LPARAM)Text);
                 CreateFontMap();
                }

              case TXT_YADJ:
               if(info->ModAll)
                {
                 SendDlgItemMessage(hDlg,TXT_YADJ,WM_GETTEXT,256,(LPARAM)Text);
                 tVal=Limit(atoi(Text));
                 tVal=Fnt->SetGlobal(VOFFSET,tVal);
  	              wsprintf(Text,"%d",tVal);
		  	            SendDlgItemMessage(hDlg,TXT_YADJ,WM_SETTEXT,0,(LPARAM)Text);
                 CreateFontMap();
                }
               else
                {
                 SendDlgItemMessage(hDlg,TXT_YADJ,WM_GETTEXT,256,(LPARAM)Text);
                 tVal=Limit(atoi(Text));
                 tVal=Fnt->SetCharVal(info->Select+Fnt->GetBaseChar(),VOFFSET,tVal);
  	              wsprintf(Text,"%d",tVal);
		  	            SendDlgItemMessage(hDlg,TXT_YADJ,WM_SETTEXT,0,(LPARAM)Text);
                 CreateFontMap();
                }


              case TXT_WADJ:
               if(info->ModAll)
                {
                 SendDlgItemMessage(hDlg,TXT_WADJ,WM_GETTEXT,256,(LPARAM)Text);
                 tVal=Limit(atoi(Text));
                 tVal=Fnt->SetGlobal(WIDTH,tVal);
  	              wsprintf(Text,"%d",tVal);
		  	            SendDlgItemMessage(hDlg,TXT_WADJ,WM_SETTEXT,0,(LPARAM)Text);
                 CreateFontMap();
                }
               else
                {
                 SendDlgItemMessage(hDlg,TXT_WADJ,WM_GETTEXT,256,(LPARAM)Text);
                 tVal=Limit(atoi(Text));
                 tVal=Fnt->SetCharVal(info->Select+Fnt->GetBaseChar(),WOFFSET,tVal);
  	              wsprintf(Text,"%d",tVal);
		  	            SendDlgItemMessage(hDlg,TXT_WADJ,WM_SETTEXT,0,(LPARAM)Text);
               
                 CreateFontMap();
                 wsprintf(Text,"%d",Fnt->GetCharVal(info->Select+Fnt->GetBaseChar(),EWIDTH));
                 SendDlgItemMessage(hMain,TXT_WIDTH,WM_SETTEXT,0,(LPARAM)Text);
                }
              return TRUE;
             }
          return FALSE;
          break; // End EN_KILLFOCUS

          case CBN_SELCHANGE:
           switch(LOWORD(wParam))
            {
             case CBO_FONTS:
              RowDex=SendDlgItemMessage(hDlg,CBO_FONTS,CB_GETCURSEL,0,0);
              SendDlgItemMessage(hDlg,CBO_FONTS,CB_GETLBTEXT,RowDex,(LPARAM)Text);
              Fnt->SetFontName(Text);
              CreateFontMap();
              return TRUE;

             case CBO_ALIAS:
              RowDex=SendDlgItemMessage(hDlg,CBO_ALIAS,CB_GETCURSEL,0,0);
               if(RowDex==0)
                {
                 Fnt->SetFontQuality(NONANTIALIASED_QUALITY);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NONE,MF_CHECKED);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NORMAL,MF_UNCHECKED);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_CLEARTYPE,MF_UNCHECKED);
                }
               else if(RowDex==1)
                {
                 Fnt->SetFontQuality(ANTIALIASED_QUALITY);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NONE,MF_UNCHECKED);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NORMAL,MF_CHECKED);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_CLEARTYPE,MF_UNCHECKED);
                }
               else if(RowDex==2)
                {
                 Fnt->SetFontQuality(5); //CLEARTYPE_QUALITY;
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NONE,MF_UNCHECKED);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_NORMAL,MF_UNCHECKED);
                 CheckMenuItem(GetMenu(hMain),ID_ANTIALIAS_CLEARTYPE,MF_CHECKED);
                }
              CreateFontMap();
              return TRUE;

             case CBO_IMGXRES:
              RowDex=SendDlgItemMessage(hDlg,CBO_IMGXRES,CB_GETCURSEL,0,0);
               if(RowDex==0)
                Fnt->SetSize(MAPWIDTH,16);
               else if(RowDex==1)
                Fnt->SetSize(MAPWIDTH,32);
               else if(RowDex==2)
                Fnt->SetSize(MAPWIDTH,64);
               else if(RowDex==3)
                Fnt->SetSize(MAPWIDTH,128);
               else if(RowDex==4)
                Fnt->SetSize(MAPWIDTH,256);
               else if(RowDex==5)
                Fnt->SetSize(MAPWIDTH,512);
               else if(RowDex==6)
                Fnt->SetSize(MAPWIDTH,1024);
               else if(RowDex==7)
                Fnt->SetSize(MAPWIDTH,2048);
               else if(RowDex==8)
                Fnt->SetSize(MAPWIDTH,4096);

              info->MaxChars=Fnt->GetSize(MAXCHARS);
              CalcScroll();
              CreateFontMap();
              return TRUE;

             case CBO_IMGYRES:
              RowDex=SendDlgItemMessage(hDlg,CBO_IMGYRES,CB_GETCURSEL,0,0);
               if(RowDex==0)
                Fnt->SetSize(MAPHEIGHT,16);
               else if(RowDex==1)
                Fnt->SetSize(MAPHEIGHT,32);
               else if(RowDex==2)
                Fnt->SetSize(MAPHEIGHT,64);
               else if(RowDex==3)
                Fnt->SetSize(MAPHEIGHT,128);
               else if(RowDex==4)
                Fnt->SetSize(MAPHEIGHT,256);
               else if(RowDex==5)
                Fnt->SetSize(MAPHEIGHT,512);
               else if(RowDex==6)
                Fnt->SetSize(MAPHEIGHT,1024);
               else if(RowDex==7)
                Fnt->SetSize(MAPHEIGHT,2048);
               else if(RowDex==8)
                Fnt->SetSize(MAPHEIGHT,4096);

              info->MaxChars=Fnt->GetSize(MAXCHARS);
              CalcScroll();
              CreateFontMap();
              return TRUE;

             case CBO_ZOOM:
              RowDex=SendDlgItemMessage(hDlg,CBO_ZOOM,CB_GETCURSEL,0,0);
               if(RowDex==0)
                info->Zoom=0.25;
               else if(RowDex==1)
                info->Zoom=0.5f;
               else if(RowDex==2)
                info->Zoom=1.0f;
               else if(RowDex==3)
                info->Zoom=2.0f;
               else if(RowDex==4)
                info->Zoom=4.0f;

              CalcScroll();
              CreateFontMap();
              return TRUE;
            }
          return FALSE;
          break;
 
         }
    
       default:
  	    return 0;
      } // End WM_COMMAND
	  
	   } // End Switch MSG

 }// End FontProc

