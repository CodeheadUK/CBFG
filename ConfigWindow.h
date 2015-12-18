void SetConfigRGB(HWND Win, BFG_RGB Cols);

BOOL CALLBACK ConfigWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  HDC dc;
  RECT rcArea;
  HBRUSH hBr;
  BFG_RGB Col;
  int Sel,Val,Red,Green,Blue;
  char Text[256];
  static AppConfig lCfg;
  extern AppInfo *info;

  switch(msg)
   {
    case WM_INITDIALOG:
     // Preserve Current Config
     lCfg.CellHeight=Fnt->GetSize(CELLHEIGHT);
     lCfg.CellWidth=Fnt->GetSize(CELLWIDTH);
     lCfg.FontHeight=Fnt->GetFontHeight();
     lCfg.FontWidth=Fnt->GetFontWidth();
     lCfg.ImgSize=Fnt->GetSize(MAPWIDTH);
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

     // Populate image size combo
     SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_ADDSTRING,0,(LPARAM)"64x64");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_ADDSTRING,0,(LPARAM)"128x128");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_ADDSTRING,0,(LPARAM)"256x256");
     SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_ADDSTRING,0,(LPARAM)"512x512");
      if(lCfg.ImgSize==128)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_SETCURSEL,1,0);
      else if(lCfg.ImgSize==256)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_SETCURSEL,2,0);
      else if(lCfg.ImgSize==512)
       SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_SETCURSEL,3,0);
      else
       SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_SETCURSEL,0,0);

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
      if((HWND)lParam==GetDlgItem(hDlg,SLD_RED))
       {
        Red=SendDlgItemMessage(hDlg,SLD_RED,TBM_GETPOS,0,0);
        wsprintf(Text,"%d",Red);
        SendDlgItemMessage(hDlg,TXT_RED,WM_SETTEXT,0,(LPARAM)Text);
       }

      if((HWND)lParam==GetDlgItem(hDlg,SLD_GREEN))
       {
        Green=SendDlgItemMessage(hDlg,SLD_GREEN,TBM_GETPOS,0,0);
        wsprintf(Text,"%d",Green);
        SendDlgItemMessage(hDlg,TXT_GREEN,WM_SETTEXT,0,(LPARAM)Text);
       }

      if((HWND)lParam==GetDlgItem(hDlg,SLD_BLUE))
       {
        Blue=SendDlgItemMessage(hDlg,SLD_BLUE,TBM_GETPOS,0,0);
        wsprintf(Text,"%d",Blue);
        SendDlgItemMessage(hDlg,TXT_BLUE,WM_SETTEXT,0,(LPARAM)Text);
       }
     InvalidateRect(GetDlgItem(hDlg,ODR_COLOR),NULL,FALSE);
					cfgChange=TRUE;
     return TRUE;

    case WM_COMMAND:
     {
      switch(HIWORD(wParam))// Notifications
       {
        case EN_CHANGE:
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

             return TRUE;

            case CBO_CFG_IMGSIZE:
													cfgChange=TRUE;
             Sel=SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_GETCURSEL,0,0);
              if(Sel==0)
               {
                lCfg.ImgSize=64;
                return TRUE;
               }
              else if(Sel==1)
               {
                lCfg.ImgSize=128;
                return TRUE;
               }
              else if(Sel==2)
               {
                lCfg.ImgSize=256;
                return TRUE;
               }
              else if(Sel==3)
               {
                lCfg.ImgSize=512;
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

									cfgChange=TRUE;
         return TRUE;

        case CHK_CFG_WIDTH:
          if(SendDlgItemMessage(hDlg,CHK_CFG_WIDTH,BM_GETCHECK,0,0)==BST_CHECKED)
           lCfg.Flags |= SHOW_WIDTH;
          else
           lCfg.Flags ^=SHOW_WIDTH;

									cfgChange=TRUE;
         return TRUE;

         case ID_OK:
          //memcpy(AppCfg,&lCfg,sizeof(AppConfig));
          EndDialog(hDlg,0);
          return 0;

         case IDSAVE:
          /*memcpy(AppCfg,&lCfg,sizeof(AppConfig));
           if(!SaveConfig("bfg.cfg",&lCfg))
												{
												 MessageBox(hDlg,"Unable to save config file","File Error",MB_OK|MB_ICONERROR);
													return 0;
												}*/
          EndDialog(hDlg,0);
										cfgChange=FALSE;
          return 0;

         case IDDEFAULT:
           if(MessageBox(hDlg,"Are you sure you want to revert to the default settings?",
                              "Reset all?",MB_YESNO | MB_ICONQUESTION)==IDNO)
            return TRUE;

										cfgChange=TRUE;

          lCfg.ImgSize=256;
          SendDlgItemMessage(hDlg,CBO_CFG_IMGSIZE,CB_SETCURSEL,2,0);

          lCfg.CellHeight=32;
          SendDlgItemMessage(hDlg,TXT_CFG_CELLHEIGHT,WM_SETTEXT,256,(LPARAM)"32");
          lCfg.CellWidth=32;
          SendDlgItemMessage(hDlg,TXT_CFG_CELLWIDTH,WM_SETTEXT,256,(LPARAM)"32");
          lCfg.FontHeight=20;
          SendDlgItemMessage(hDlg,TXT_CFG_FONTHEIGHT,WM_SETTEXT,256,(LPARAM)"20");
          lCfg.FontWidth=0;
          SendDlgItemMessage(hDlg,TXT_CFG_FONTWIDTH,WM_SETTEXT,256,(LPARAM)"0");

          Fnt->SetCol(BACKCOL,0,0,0);
          Fnt->SetCol(TEXTCOL,255,255,255);
          Fnt->SetCol(GRIDCOL,170,0,170);
          Fnt->SetCol(WIDTHCOL,170,170,0);
          Fnt->SetCol(SELCOL,0,154,0);

          SendDlgItemMessage(hDlg,CBO_CFG_ITEM,CB_SETCURSEL,0,0);
          SetConfigRGB(hDlg,Fnt->GetCol(BACKCOL));

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

 void SetConfigRGB(HWND Win, BFG_RGB Cols)
  {
   char Text[256];

   wsprintf(Text,"%d",Cols.Red);
   SendDlgItemMessage(Win,TXT_RED,WM_SETTEXT,0,(LPARAM)Text);
   wsprintf(Text,"%d",Cols.Green);
   SendDlgItemMessage(Win,TXT_GREEN,WM_SETTEXT,0,(LPARAM)Text);
   wsprintf(Text,"%d",Cols.Blue);
   SendDlgItemMessage(Win,TXT_BLUE,WM_SETTEXT,0,(LPARAM)Text);
  }