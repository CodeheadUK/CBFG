#include "stdafx.h"
#include "stdio.h"
#include <windows.h>
#include "FontMapClass.h"
#include "UtilFunctions.h"
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment (lib,"ws2_32.lib")

extern HWND hMain;
extern BFontMap *Fnt;
extern AppInfo *info;

using namespace std;

BFG_RGB MakeRGB(unsigned char Red,unsigned char Green,unsigned char Blue)
 {
  BFG_RGB Pack;

  Pack.Red=Red;
  Pack.Green=Green;
  Pack.Blue=Blue;

  return Pack;
 }

char Limit(int Val)
 {
  if(Val<-128)
   Val=-128;
  if(Val>127)
   Val=127;

  return (char) Val;
 }

int LimitSelection(int Sel, int MaxChar)
 {
   if(Sel>MaxChar)
    Sel=MaxChar-1;
  
  return Sel;
 }

void CreateFontMap()
 {
  HDC Wdc,Mdc,Fdc;
  HWND hImgWin;
  HBITMAP *FntMap;
  HBITMAP mMap;
  BITMAPINFO BMDat;
  RECT ClipArea;
  HRGN ClipRgn;
  int Opt=0,Selection;

  const int MapWidth=Fnt->GetSize(MAPWIDTH);
  const int MapHeight=Fnt->GetSize(MAPHEIGHT);


  // Get the target window
  hImgWin=GetDlgItem(hMain,IMG_TEXT);
   if(hImgWin==NULL)
    return;

  // Get target's DC
  Wdc=GetDC(hImgWin);
   if(Wdc==NULL)
    return;

  // Create memory DC
  Mdc=CreateCompatibleDC(Wdc);
   if(Mdc==NULL)
    return;

  // Create DC for font
  Fdc=CreateCompatibleDC(Wdc);
   if(Fdc==NULL)
    return;

  // Get size of target window
  GetClientRect(hImgWin, &ClipArea); 

  // Specify bitmap type and size
  BMDat.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
  BMDat.bmiHeader.biWidth=ClipArea.right;
  BMDat.bmiHeader.biHeight=ClipArea.right;
  BMDat.bmiHeader.biPlanes=1;
  BMDat.bmiHeader.biBitCount=24;
  BMDat.bmiHeader.biCompression=BI_RGB;
  BMDat.bmiHeader.biSizeImage=(ClipArea.right*ClipArea.right)*3;  

  // Create the bitmap
  mMap=CreateDIBSection(Mdc,&BMDat,DIB_RGB_COLORS,NULL,NULL,0);
   if(!mMap)
    return;

  // Select into memory DC
   if(!SelectObject(Mdc,mMap))
    return;

  // Render the font
   if(info->Grid)
    Opt|=DFM_GRIDLINES;

   if(info->wMarker)
    Opt|=DFM_WIDTHLINE;

   if(info->ModAll)
    Selection=-1;
   else
    Selection=info->Select;

  FntMap=Fnt->DrawFontMap(Opt,Selection);

  // Select Font Map into Font DC
  SelectObject(Fdc,*FntMap);

  // Gray out bitmap
  FillRect(Mdc, &ClipArea,(HBRUSH)GetStockObject(GRAY_BRUSH));

  // Set Clipping Region
  ClipRgn=CreateRectRgn(ClipArea.left,ClipArea.top,ClipArea.right,ClipArea.bottom);
  SelectClipRgn(Mdc,ClipRgn);

  // Copy Font into buffer
  SetStretchBltMode(Mdc,WHITEONBLACK);
  StretchBlt(Mdc,0,0,(int)(MapWidth*info->Zoom),(int)(MapHeight*info->Zoom),
             Fdc,(int)(info->hScr/info->Zoom),(int)(info->vScr/info->Zoom),
             MapWidth,MapHeight,SRCCOPY);
  
  // Copy Font into window
  SetStretchBltMode(Wdc,WHITEONBLACK);
  BitBlt(Wdc,0,0,(int)ClipArea.right,(int)ClipArea.bottom,
             Mdc,0,0,SRCCOPY);

  // Clean up
  DeleteObject(*FntMap);
  DeleteObject(mMap);
  DeleteObject(ClipRgn);
  DeleteObject(mMap);
  ReleaseDC(hImgWin,Wdc);
  DeleteDC(Mdc);
  DeleteDC(Fdc);
 }

void CalcScroll()
 {
  extern BFontMap *Fnt;
  extern AppConfig *AppCfg;
  extern AppInfo *info;

  RECT WinSize;
  int XDelta,YDelta;
  SCROLLINFO sInf;
  int TexWidth,TexHt;
  int CharRow,CharCol,RowPitch;

  TexWidth=Fnt->GetSize(MAPWIDTH);
  TexHt=Fnt->GetSize(MAPHEIGHT);
  RowPitch=TexWidth/Fnt->GetSize(CELLWIDTH);
 
  GetClientRect(GetDlgItem(hMain,IMG_TEXT), &WinSize);

   if(!info->ModAll) // Check for active selection
    {
     // Calculate position of char
     CharCol=info->Select;
     CharRow=CharCol/RowPitch;
     CharCol-=CharRow*RowPitch;

     // Convert to pixels (Zoom?)
     CharCol*=Fnt->GetSize(CELLWIDTH);
     CharRow*=Fnt->GetSize(CELLHEIGHT);
    }

  // Calculate something?
  XDelta=(int)(TexWidth*info->Zoom)-WinSize.right;
  YDelta=(int)(TexHt*info->Zoom)-WinSize.bottom;
   
   if(XDelta>0)
    {
     SetScrollRange(GetDlgItem(hMain,SCR_HOR),SB_CTL,0,XDelta,FALSE);
   
     sInf.cbSize=sizeof(SCROLLINFO);
     sInf.fMask=SIF_RANGE;
     GetScrollInfo(GetDlgItem(hMain,SCR_HOR),SB_CTL,&sInf);
      if(info->hScr>sInf.nMax)
       info->hScr=sInf.nMax;

     SetScrollPos(GetDlgItem(hMain,SCR_HOR),SB_CTL,info->hScr,TRUE);
     info->hScroll=TRUE;
    }
   else // Prevent offset pushing texture off left edge of window
    {
     SetScrollPos(GetDlgItem(hMain,SCR_HOR),SB_CTL,0,TRUE);
     info->hScr=0;
     EnableWindow(GetDlgItem(hMain,SCR_HOR),FALSE);
     info->hScroll=FALSE;
    }

   if(YDelta>0)
    {
     SetScrollRange(GetDlgItem(hMain,SCR_VERT),SB_CTL,0,YDelta,FALSE);
   
     sInf.cbSize=sizeof(SCROLLINFO);
     sInf.fMask=SIF_RANGE;
     GetScrollInfo(GetDlgItem(hMain,SCR_VERT),SB_CTL,&sInf);
      if(info->vScr>sInf.nMax)
       info->vScr=sInf.nMax;

     SetScrollPos(GetDlgItem(hMain,SCR_VERT),SB_CTL,info->vScr,TRUE);
     info->vScroll=TRUE;
    }
   else // Prevent offset pushing texture off top edge of window
    {
     SetScrollPos(GetDlgItem(hMain,SCR_VERT),SB_CTL,0,TRUE);
     info->vScr=0;
     EnableWindow(GetDlgItem(hMain,SCR_VERT),FALSE);
     info->vScroll=FALSE;
    }
 }


BOOL CheckOverwrite(char *fname)
 {
  ifstream testfile;


  testfile.open(fname);

   if(!testfile.fail())
    {
     testfile.close();
      if(MessageBox(hMain,"Overwrite existing file?","Filename already exists",MB_YESNO | MB_ICONQUESTION)==IDNO)
       return FALSE;
    }

  return TRUE;
 }
