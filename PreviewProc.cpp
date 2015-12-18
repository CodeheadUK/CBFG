#include "stdafx.h"
#include "Procs.h"
#include "CBFGDefs.h"
#include "FontMapClass.h"
#include "SBM-Util.h"
#include <time.h>
#include <gl\gl.h>

GLuint TexName; 
HDC glDC;
HGLRC glRC;
HWND hGL;

extern HINSTANCE G_Inst;
extern HWND hMain;
extern BFontMap *Fnt;
extern AppInfo *info;

BOOL CALLBACK PreviewWinProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
 {
  int nLines,Loop,chLoop,offset;
  int CurX,CurY;
  float RowFactor,ColFactor,U,V;
  int SrcCol,SrcRow,RowPitch;
  unsigned char Text[255];
  HBITMAP *hBMP;
  DIBSECTION bmInfo;
  SBM_Image FntImg;
  RECT glRect;
  BFG_RGB BkCol;
  WORD BufSz=1023;
  LRESULT lTxt;

  PIXELFORMATDESCRIPTOR pfd, *ppfd; 
  int pixelformat;
  
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
 	   // Init random text string
      if(!lstrlen(PText))
       {
        srand((unsigned)time(NULL));
        offset=rand()%13;
        lstrcpy(PText,&Sample[offset][0]);
       }
   
     // Limit text length
     SendDlgItemMessage(hDlg,TXT_PREVIEW,EM_LIMITTEXT,254,0);

	    // Init GL Window
     hGL=GetDlgItem(hDlg,IDC_GL);
     glDC=GetDC(hGL);
      if(glDC==NULL)
       MessageBox(NULL,"GetDC failed","Error",MB_OK);
       	 
	    ppfd = &pfd; 
     ZeroMemory(ppfd,sizeof(PIXELFORMATDESCRIPTOR));

	    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR); 
     ppfd->nVersion = 1; 
     ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  PFD_DOUBLEBUFFER; 
     ppfd->dwLayerMask = PFD_MAIN_PLANE; 
     ppfd->iPixelType = PFD_TYPE_RGBA; 
     ppfd->cColorBits = 32; 
     ppfd->cDepthBits = 8; 
     ppfd->cAccumBits = 0; 
     ppfd->cStencilBits = 0; 

     pixelformat=ChoosePixelFormat(glDC,ppfd);

      if(pixelformat==0)
       {
        MessageBox(NULL,"ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
       }
   
      if(SetPixelFormat(glDC,pixelformat,ppfd)==FALSE) 
       { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
       } 

     glRC=wglCreateContext(glDC);
     wglMakeCurrent(glDC, glRC);

     glEnable(GL_TEXTURE_2D);

     // Build Texture
     hBMP=Fnt->DrawFontMap(FALSE,-1);
     GetObject(*hBMP,sizeof(DIBSECTION),&bmInfo);
     FntImg.Create(Fnt->GetSize(MAPWIDTH),Fnt->GetSize(MAPHEIGHT),24);
     memcpy(FntImg.GetImg(),bmInfo.dsBm.bmBits,(Fnt->GetSize(MAPWIDTH)*Fnt->GetSize(MAPHEIGHT)*3));
     FntImg.FlipImg();

     glGenTextures(1,&TexName);
     glBindTexture(GL_TEXTURE_2D,TexName);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,0x812F);
     glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,0x812F);
     glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Fnt->GetSize(MAPWIDTH),Fnt->GetSize(MAPHEIGHT),0,GL_BGR_EXT,GL_UNSIGNED_BYTE,FntImg.GetImg());

     DeleteObject(*hBMP);
     SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_SETTEXT,0,(LPARAM)PText);
     return 0;


    case WM_APP:
     CurX=CurY=0;

     GetClientRect(hGL,&glRect);
     glViewport(0,0,glRect.right,glRect.bottom);
     BkCol=Fnt->GetCol(BACKCOL);
     glClearColor(((float)BkCol.Red/255.0f),((float)BkCol.Green/255.0f),((float)BkCol.Blue/255.0f),0.0f);
 
     glMatrixMode(GL_PROJECTION); 
     glLoadIdentity();
     glOrtho(0.0f,glRect.right,glRect.bottom,0.0f,-10.0f,10.0f); 
     glMatrixMode(GL_MODELVIEW); 
     glLoadIdentity();

     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     nLines=SendDlgItemMessage(hDlg,TXT_PREVIEW,EM_GETLINECOUNT,0,0);

     RowPitch=Fnt->GetSize(MAPWIDTH)/Fnt->GetSize(CELLWIDTH);
     RowFactor=(float)Fnt->GetSize(CELLHEIGHT)/(float)Fnt->GetSize(MAPHEIGHT);
     ColFactor=(float)Fnt->GetSize(CELLWIDTH)/(float)Fnt->GetSize(MAPWIDTH);

     glBegin(GL_QUADS);
      for(Loop=0;Loop!=nLines;++Loop)
       {
        Text[0]=0xFF;
        Text[1]=0;
        lTxt=SendDlgItemMessage(hDlg,TXT_PREVIEW,EM_GETLINE,Loop,(LPARAM)Text);
        Text[lTxt]=NULL;

         for(chLoop=0;chLoop!=lTxt;++chLoop)
          {
           SrcRow=(Text[chLoop]-Fnt->GetBaseChar())/RowPitch;
           SrcCol=(Text[chLoop]-Fnt->GetBaseChar())-(SrcRow*RowPitch);
           U=ColFactor*SrcCol;
           V=RowFactor*SrcRow;
        
           glTexCoord2f(U,V);                     glVertex2i(CurX,CurY);
           glTexCoord2f(U+ColFactor,V);           glVertex2i(CurX+Fnt->GetSize(CELLWIDTH),CurY);
           glTexCoord2f(U+ColFactor,V+RowFactor); glVertex2i(CurX+Fnt->GetSize(CELLWIDTH),CurY+Fnt->GetSize(CELLHEIGHT));
           glTexCoord2f(U,V+RowFactor);           glVertex2i(CurX,CurY+Fnt->GetSize(CELLHEIGHT));
           CurX+=Fnt->GetCharVal(Text[chLoop],EWIDTH);
          }
        CurX=0;
        CurY+=Fnt->GetSize(CELLHEIGHT);
       }

     glEnd();
     
     SwapBuffers(glDC);
     return 0;

    case WM_DRAWITEM:
      if(wParam==IDC_GL)
       {
        SendMessage(hDlg,WM_APP,0,0);
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
           SendMessage(hDlg,WM_APP,0,0);
           return 0;
        }

       switch(LOWORD(wParam)) // Buttons & Menu items
        {
          case CMD_OK:
           SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_GETTEXT,1024,(LPARAM)PText);
           wglDeleteContext(glRC);
           EndDialog(hDlg,0);  
           return 0;

          case CMD_TEST_CLEAR:
           SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_SETTEXT,0,(LPARAM)"");
           SendMessage(hDlg,WM_APP,0,0);
           return 0;

          case CMD_TEST_PANGRAM:
           offset=rand()%13;
           SendDlgItemMessage(hDlg,TXT_PREVIEW,WM_SETTEXT,0,(LPARAM)&Sample[offset][0]);
           SendMessage(hDlg,WM_APP,0,0);
           return 0;
        }

     }

    default:
     return 0;
   }
 }