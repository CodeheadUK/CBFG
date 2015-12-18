#include <time.h>
#include <stdlib.h>

BOOL CALLBACK TestWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  HDC dc,fdc;
  RECT rcArea;
  int nLines,Loop,chLoop,lTxt,offset;
  int CurX,CurY;
  int SrcCol,SrcRow;
  char Text[1024];
  HBITMAP *hBMP;
  BFG_RGB TransCol;
  static char PText[1024];
  char Sample[13][128] = {{"THE QUICK BROWN FOX JUMPS OVER A LAZY DOG"},
                         {"JACKDAWS LOVE MY BIG SPHINX OF QUARTZ"},
                         {"QUICK WAFTING ZEPHYRS VEX BOLD JIM"},
                         {"PACK MY BOX WITH FIVE DOZEN LIQUOR JUGS"},
                         {"SIX BIG DEVILS FROM JAPAN QUICKLY FORGOT HOW TO WALTZ"},
                         {"CRAZY FREDERICKA BOUGHT MANY VERY EXQUISITE OPAL JEWELS"},
                         {"FEW QUIPS GALVANIZED THE MOCK JURY BOX"},
                         {"THE FIVE BOXING WIZARDS JUMP QUICKLY"},
                         {"A SHORT BRIMLESS FELT HAT BARELY BLOCKS OUT THE SOUND OF A CELTIC VIOLIN"},
                         {"BEING BOUNCED AROUND QUICKLY ANNOYED THE DISHEVELED TAXI DRIVERS"},
                         {"MY GIRL WOVE SIX DOZEN PLAID JACKETS BEFORE SHE QUIT"},
                         {"SIXTY ZIPPERS WERE QUICKLY PICKED FROM THE WOVEN JUTE BAG"},
                         {"MY FAXED JOKE WON A PAGER IN THE CABLE TV QUIZ SHOW"}};

  switch(msg)
   {
    case WM_INITDIALOG:
      if(!lstrlen(PText))
       {
        srand((unsigned)time(NULL));
        offset=rand()%13;
        lstrcpy(PText,&Sample[offset][0]);
       }
     SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_SETTEXT,0,(LPARAM)PText);
     return 0;

    case EN_CHANGE:
     UpdateWindow(GetDlgItem(hDlg,OWN_MAIN));
     return 1;

    case WM_DRAWITEM:
      if(wParam==OWN_MAIN)
       {
        TransCol=Fnt->GetCol(BACKCOL);

        dc=((LPDRAWITEMSTRUCT)lParam)->hDC;
        GetClientRect(hDlg, &rcArea);
        FillRect(dc,&rcArea,(HBRUSH)CreateSolidBrush(RGB(TransCol.Red,TransCol.Green,TransCol.Blue)));

        hBMP=Fnt->DrawFontMap(FALSE,0);

        fdc=CreateCompatibleDC(dc);

        SelectObject(fdc,*hBMP);
        
        nLines=SendDlgItemMessage(hDlg,TXT_PREVIEW,EM_GETLINECOUNT,0,0);

        Text[0]=NULL;
        CurX=CurY=0;

         for(Loop=0;Loop<nLines;++Loop)
          {
           SendDlgItemMessage(hDlg,TXT_PREVIEW,EM_GETLINE,Loop,(LPARAM)Text);
           offset=SendDlgItemMessage(hDlg,TXT_PREVIEW,EM_LINEINDEX,Loop,0);
           lTxt=SendDlgItemMessage(hDlg,TXT_PREVIEW,EM_LINELENGTH,offset,0);

           CurX=0;

            for(chLoop=0;chLoop<lTxt;++chLoop)
             {
              SrcRow=(Text[chLoop]-Fnt->GetBaseChar())/(Fnt->GetSize(MAPWIDTH)/Fnt->GetSize(CELLWIDTH));
              SrcCol=(Text[chLoop]-Fnt->GetBaseChar())-((Fnt->GetSize(MAPWIDTH)/Fnt->GetSize(CELLWIDTH))*SrcRow);

              TransparentBlt(dc,CurX,CurY,Fnt->GetSize(CELLWIDTH),Fnt->GetSize(CELLHEIGHT),fdc,SrcCol*Fnt->GetSize(CELLWIDTH),SrcRow*Fnt->GetSize(CELLHEIGHT),Fnt->GetSize(CELLWIDTH),Fnt->GetSize(CELLHEIGHT),RGB(TransCol.Red,TransCol.Green,TransCol.Blue));              
              CurX+=Fnt->GetCharVal(Text[chLoop],WIDTH);
             }

            CurY+=Fnt->GetSize(CELLHEIGHT);
         
          }

        ReleaseDC(hDlg,fdc);

        delete hBMP;
       }    
     return TRUE;

    case WM_CLOSE:
     SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_GETTEXT,1024,(LPARAM)PText);
     EndDialog(hDlg,0);
     return 0;

    case WM_COMMAND:
     {
       switch(HIWORD(wParam))
        {
          case EN_CHANGE:
           InvalidateRect(GetDlgItem(hDlg,OWN_MAIN),NULL,FALSE);
           return 0;
        }

       switch(LOWORD(wParam)) // Buttons & Menu items
        {
          case CMD_OK:
           SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_GETTEXT,1024,(LPARAM)PText);
           EndDialog(hDlg,0);  
           return 0;

          case CMD_TEST_CLEAR:
           SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_SETTEXT,0,(LPARAM)"");
           InvalidateRect(GetDlgItem(hDlg,OWN_MAIN),NULL,FALSE);
           return 0;

          case CMD_TEST_PANGRAM:
           offset=rand()%13;
           SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_SETTEXT,0,(LPARAM)&Sample[offset][0]);
           InvalidateRect(GetDlgItem(hDlg,OWN_MAIN),NULL,FALSE);
           return 0;
        }

     }

    default:
     return 0;
   }
 }