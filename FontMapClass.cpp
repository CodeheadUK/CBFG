#include "stdafx.h"
#include <memory.h>
#include "FontMapClass.h"
#include "UtilFunctions.h"

BFontMap::BFontMap()
 {
  int loop;

  BaseChar=32;

  MapWidth=256;
  MapHeight=256;
  CellHeight=32;
  CellWidth=32;
  gHMod=0;
  gVMod=0;
  gWidthMod=0;
  
   for(loop=0;loop!=256;loop++)
    {
     HMod[loop]=0;
     VMod[loop]=0;
     WidthMod[loop]=0;
    }

  fnt=NULL;

  FntDef.lfHeight=20;
  FntDef.lfWidth=0;
  FntDef.lfEscapement=0;
  FntDef.lfOrientation=0;
  FntDef.lfWeight=FW_NORMAL;
  FntDef.lfItalic=FALSE;
  FntDef.lfUnderline=FALSE;
  FntDef.lfStrikeOut=FALSE;
  FntDef.lfCharSet=DEFAULT_CHARSET;
  FntDef.lfOutPrecision=OUT_DEFAULT_PRECIS;
  FntDef.lfClipPrecision=CLIP_DEFAULT_PRECIS;
  FntDef.lfQuality=NONANTIALIASED_QUALITY;
  FntDef.lfPitchAndFamily=DEFAULT_PITCH;
  FntDef.lfFaceName[0]=NULL;

  BkCol.Red=0;
  BkCol.Green=0;
  BkCol.Blue=0;

  TextCol.Red=255;
  TextCol.Green=255;
  TextCol.Blue=255;

  GridCol.Red=170;
  GridCol.Green=0;
  GridCol.Blue=170;

  WidthCol.Red=170;
  WidthCol.Green=170;
  WidthCol.Blue=0;

  SelCol.Red=0;
  SelCol.Green=154;
  SelCol.Blue=0;
 }

BFontMap::~BFontMap()
 {
  DeleteObject(fnt);
 }

int BFontMap::SetSize(int Which, int NewSize)
 {
   switch(Which)
    {
     case MAPWIDTH:
       if(!IsPower(NewSize))
        NewSize=256;

      MapWidth=NewSize;
      return MapWidth;


     case MAPHEIGHT:
       if(!IsPower(NewSize))
        NewSize=256;

      MapHeight=NewSize;
      return MapHeight;


     case CELLWIDTH:
       if(NewSize<8)
        CellWidth=8;
       else if(NewSize>256)
        CellWidth=256;
       else
        CellWidth=NewSize;

      return CellWidth;
       

     case CELLHEIGHT:
       if(NewSize<8)
        CellHeight=8;
       else if(NewSize>256)
        CellHeight=256;
       else
        CellHeight=NewSize;

      return CellHeight;
    }

  return 0;
 }

int BFontMap::GetSize(int Which)
 {
   switch(Which)
    {
     case MAPWIDTH:
      return MapWidth;

     case MAPHEIGHT:
      return MapHeight;

     case CELLWIDTH:
      return CellWidth;

     case CELLHEIGHT:
      return CellHeight;

     case MAXCHARS:
      return (MapWidth/CellWidth)*(MapHeight/CellHeight);
    }

  return 0;
 }

unsigned char BFontMap::SetBaseChar(int NewBase)
 {
  if(NewBase<0)
   NewBase=0;

  if(NewBase>255)
   NewBase=255;

  BaseChar=NewBase;
  return BaseChar;
 }

unsigned char BFontMap::GetBaseChar()
 {
  return BaseChar;
 }

char BFontMap::SetGlobal(int Which, char Value)
 {
  switch(Which)
   {
    case VOFFSET:
     gVMod=Value;
     break;

    case HOFFSET:
     gHMod=Value;
     break;

    case WIDTH:
     gWidthMod=Value;
     break;
   }

  return Value;
 }

char BFontMap::GetGlobal(int Which)
 {
  switch(Which)
   {
    case VOFFSET:
     return gVMod;

    case HOFFSET:
     return gHMod;

    case WIDTH:
     return gWidthMod;
   }

  return 0;
 }

char BFontMap::SetCharVal(int Char, int Which, char NewVal)
 {
  switch(Which)
   {
    case WOFFSET:
     WidthMod[Char]=NewVal;
     break;

    case HOFFSET:
     HMod[Char]=NewVal;
     break;

    case VOFFSET:
     VMod[Char]=NewVal;
     break;
   }

  return NewVal;
 }


char BFontMap::GetCharVal(int Char, int Which)
 {
  switch(Which)
   {
    case WIDTH:
     return BaseWidth[Char];

    case HOFFSET:
     return HMod[Char];

    case VOFFSET:
     return VMod[Char];

    case WOFFSET:
     return WidthMod[Char];

    case EWIDTH:
     return WidthMod[Char]+BaseWidth[Char]+gWidthMod;
   }
  return 0;
 }

long BFontMap::SetFontHeight(long NewHeight)
 {
   if(NewHeight<1)
    NewHeight=1;
   if(NewHeight>256)
    NewHeight=256;

  FntDef.lfHeight=NewHeight;
  return FntDef.lfHeight;
 }

long BFontMap::GetFontHeight()
 {
  return FntDef.lfHeight;
 }

long BFontMap::SetFontWidth(long NewWidth)
 {
   if(NewWidth<0)
    NewWidth=0;
   if(NewWidth>256)
    NewWidth=256;


  FntDef.lfWidth=NewWidth;
  return FntDef.lfWidth;
 }

long BFontMap::GetFontWidth()
 {
  return FntDef.lfWidth;
 }

bool BFontMap::SetFontName(char* NewName)
 {
   if(lstrcpy(FntDef.lfFaceName,NewName))
    return true;
   else
    return false;
 }

char* BFontMap::GetFontName()
 {
  return FntDef.lfFaceName;
 }

long BFontMap::SetFontWeight(long NewWeight)
 {
  FntDef.lfWeight=NewWeight;
  return FntDef.lfWeight;
 }

long BFontMap::GetFontWeight()
 {
  return FntDef.lfWeight;
 }

long BFontMap::SetFontQuality(long NewQual)
 {
  FntDef.lfQuality=(BYTE)NewQual;
  return FntDef.lfQuality;
 }

long BFontMap::GetFontQuality()
 {
  return FntDef.lfQuality;
 }

long BFontMap::SetFontItalic(long NewItal)
 {
  FntDef.lfItalic=(BYTE)NewItal;
  return FntDef.lfItalic;
 }

long BFontMap::GetFontItalic()
 {
  return FntDef.lfItalic;
 }

void BFontMap::SetCol(int Which, BFG_RGB NewCol)
 {
  BFG_RGB *Tgt;

   switch(Which)
    { 
     case GRIDCOL:
      Tgt=&GridCol;
      break;

     case WIDTHCOL:
      Tgt=&WidthCol;
      break;

     case SELCOL:
      Tgt=&SelCol;
      break;

     case TEXTCOL:
      Tgt=&TextCol;
      break;

     case BACKCOL:
      Tgt=&BkCol;
      break;

     default:
      return;
    }

  Tgt->Red=NewCol.Red;
  Tgt->Green=NewCol.Green;
  Tgt->Blue=NewCol.Blue;
 }


void BFontMap::SetCol(int Which, unsigned char Red, unsigned char Green, unsigned char Blue)
 {
  BFG_RGB *Tgt;

   switch(Which)
    { 
     case GRIDCOL:
      Tgt=&GridCol;
      break;

     case WIDTHCOL:
      Tgt=&WidthCol;
      break;

     case SELCOL:
      Tgt=&SelCol;
      break;

     case TEXTCOL:
      Tgt=&TextCol;
      break;

     case BACKCOL:
      Tgt=&BkCol;
      break;

     default:
      return;
    }

  Tgt->Red=Red;
  Tgt->Green=Green;
  Tgt->Blue=Blue;
 }

BFG_RGB BFontMap::GetCol(int Which)
 {
   switch(Which)
    {
     case GRIDCOL:
      return GridCol;
      break;

     case WIDTHCOL:
      return WidthCol;
      break;

     case SELCOL:
      return SelCol;
      break;

     case TEXTCOL:
      return TextCol;
      break;

     case BACKCOL:
      return BkCol;
      break;
    }

  return BkCol; // Default
 }

bool BFontMap::CalcWidths(HDC hdc)
 {
  BOOL Test;
  int Letter;
  ABC CharWidth[256];
  int nttWidth[256];

  // Populate Width data
  Test=GetCharABCWidths(hdc,0,255,CharWidth);

   if(Test)
    {
      for(Letter=0;Letter!=256;Letter++)
       BaseWidth[Letter]=(unsigned char)(CharWidth[Letter].abcA+
                                         CharWidth[Letter].abcB+
                                         CharWidth[Letter].abcC);
    }
   else
    {
     // GetCharWidth32 for non truetype fonts
     Test=GetCharWidth32(hdc,0,255,nttWidth);

      if(Test)
       for(Letter=0;Letter!=256;Letter++)
        BaseWidth[Letter]=(unsigned char)nttWidth[Letter];
    }
   
  return true;
 }


HBITMAP* BFontMap::DrawFontMap(int Flags, int Sel)
 {
  HDC wDC,mDC;
  HBITMAP *fDIB;
  BITMAPINFO BMDat;
  HBRUSH Brush;
  HPEN Pen;
  int RowDex,ColDex,Letter;
  HRGN ClipRgn;
  RECT CharArea;
  char Symbol[2];
  unsigned char eVal;

  // Create Device context
  wDC=CreateDC("DISPLAY",NULL,NULL,NULL);
  mDC=CreateCompatibleDC(wDC);

   if(!wDC || !mDC)
    return NULL;

  // Create bitmap for font rendering
  fDIB=new HBITMAP;
   if(!fDIB)
    return NULL;

  BMDat.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
  BMDat.bmiHeader.biWidth=MapWidth;
  BMDat.bmiHeader.biHeight=MapHeight;
  BMDat.bmiHeader.biPlanes=1;
  BMDat.bmiHeader.biBitCount=24;
  BMDat.bmiHeader.biCompression=BI_RGB;
  BMDat.bmiHeader.biSizeImage=(MapWidth*MapHeight)*3;  

  *fDIB=CreateDIBSection(mDC,&BMDat,DIB_RGB_COLORS,NULL,NULL,0);

   if(!fDIB)
    return NULL;

   if(!SelectObject(mDC,*fDIB))
    return NULL;

  // Fill background
   if(Flags & DFM_ALPHA)
    {
     Brush=CreateSolidBrush(RGB(0,0,0));
     Pen=CreatePen(PS_SOLID,0,RGB(0,0,0));
    }
   else
    {
     Brush=CreateSolidBrush(RGB(BkCol.Red,BkCol.Green,BkCol.Blue));
     Pen=CreatePen(PS_SOLID,0,RGB(BkCol.Red,BkCol.Green,BkCol.Blue));
    }

  SelectObject(mDC,Brush);
  SelectObject(mDC,Pen);

  Rectangle(mDC,0,0,MapWidth,MapHeight);
 
  DeleteObject(Pen);
  DeleteObject(Brush);

  // Draw Selection
  Pen=CreatePen(PS_SOLID,0,RGB(SelCol.Red,SelCol.Green,SelCol.Blue));
  Brush=CreateSolidBrush(RGB(SelCol.Red,SelCol.Green,SelCol.Blue));

   if(Sel>-1)
    {
     SelectObject(mDC,Pen);
     SelectObject(mDC,Brush);
     RowDex=(Sel/(MapWidth/CellWidth));
     ColDex=(Sel-((MapWidth/CellWidth)*RowDex));
     ColDex*=CellWidth;
     RowDex*=CellHeight;
     Rectangle(mDC,ColDex,RowDex,ColDex+CellWidth,RowDex+CellHeight);
    }

  DeleteObject(Brush);
  DeleteObject(Pen);

  // Draw letters
  // Create the font
   if(fnt)
    DeleteObject(fnt);

  fnt=CreateFontIndirect(&FntDef);

  SelectObject(mDC,fnt);
  
  CalcWidths(mDC);

   if(Flags & DFM_ALPHA)
    {
     SetTextColor(mDC,RGB(255,255,255));
     SetBkColor(mDC,RGB(0,0,0));
    }
   else
    {
     SetTextColor(mDC,RGB(TextCol.Red,TextCol.Green,TextCol.Blue));
     SetBkColor(mDC,RGB(BkCol.Red,BkCol.Green,BkCol.Blue));
    }

  SetBkMode(mDC,TRANSPARENT);
  
  Pen=CreatePen(PS_SOLID,0,RGB(WidthCol.Red,WidthCol.Green,WidthCol.Blue));
  SelectObject(mDC,Pen);

  Letter=BaseChar;

   for(RowDex=0;RowDex<(MapHeight-CellHeight)+1;RowDex+=CellHeight)
    {
      for(ColDex=0;ColDex<(MapWidth-CellWidth)+1 && Letter<256;ColDex+=CellWidth)
       {
        // Set Clipping Region
        ClipRgn=CreateRectRgn(ColDex,RowDex,ColDex+CellWidth,RowDex+CellHeight);
        SelectClipRgn(mDC,ClipRgn);

       // Draw width marker
         if(Flags & DFM_WIDTHLINE)
          {
           eVal=BaseWidth[Letter]+WidthMod[Letter]+gWidthMod;
           MoveToEx(mDC,ColDex+eVal,RowDex,NULL);
           LineTo(mDC,ColDex+eVal,RowDex+CellHeight);
          }

        // Render Char
        CharArea.left=ColDex+HMod[Letter]+gHMod;
        CharArea.right=ColDex+CellWidth;
        CharArea.top=RowDex+VMod[Letter]+gVMod;
        CharArea.bottom=RowDex+CellHeight;
        wsprintf(Symbol,"%c",Letter);
        Letter++;
        DrawText(mDC,Symbol,-1,&CharArea,DT_LEFT | DT_NOPREFIX | DT_NOCLIP);
        

        // Remove clip region
        SelectClipRgn(mDC,NULL);
        DeleteObject(ClipRgn);
       }
    }

  DeleteObject(Pen);

  // Draw grid lines
  Pen=CreatePen(PS_SOLID,0,RGB(GridCol.Red,GridCol.Green,GridCol.Blue));

   if(Flags & DFM_GRIDLINES)
    {
     SelectObject(mDC,Pen);

      for(RowDex=CellHeight-1;RowDex<MapHeight;RowDex+=CellHeight)
       {
        MoveToEx(mDC,0,RowDex,NULL);
        LineTo(mDC,MapWidth,RowDex);
       }

     for(ColDex=CellWidth-1;ColDex<MapWidth;ColDex+=CellWidth)
      {
       MoveToEx(mDC,ColDex,0,NULL);
       LineTo(mDC,ColDex,MapHeight);
      }
    }

  DeleteObject(Pen);
  DeleteDC(wDC);
  DeleteDC(mDC);

  return fDIB;
 }

int BFontMap::LoadConfig(char *fname)
 {
  ifstream cfgfile;
  long fSize;
  char *dat;
  char Hdr[7];
  int tVal,Flags;

  cfgfile.open(fname,ios::binary);

   if(cfgfile.fail())
    return -1;

  cfgfile.seekg(0,ios_base::end);
  fSize=cfgfile.tellg();
  cfgfile.seekg(0,ios_base::beg);
  
  dat=new char[fSize];

   if(!dat)
    return -1;

  cfgfile.read(dat,fSize);

  cfgfile.close();
  
  // Check ID
  lstrcpyn(Hdr,dat,7);
  Hdr[6]=NULL;

   if(lstrcmp(Hdr,"BFGCFG"))
    {
     delete [] dat;
     return -1;
    }
 
  memcpy(&MapWidth,&dat[6],4);
  memcpy(&MapHeight,&dat[10],4);
  memcpy(&CellWidth,&dat[14],4);
  memcpy(&CellHeight,&dat[18],4);
  memcpy(&tVal,&dat[22],4);
  FntDef.lfHeight=tVal;
  memcpy(&tVal,&dat[26],4);
  FntDef.lfWidth=tVal;
  memcpy(&Flags,&dat[30],4);
  memcpy(&GridCol,&dat[34],3);
  memcpy(&WidthCol,&dat[37],3);
  memcpy(&SelCol,&dat[40],3);
  memcpy(&TextCol,&dat[43],3);
  memcpy(&BkCol,&dat[46],3);

  delete [] dat;

  return Flags; 
 }

bool BFontMap::SaveConfig(char *fname, bool Grid, bool Width)
 {
  ofstream cfgfile;
  int tVal,Flags=0;

  cfgfile.open(fname,ios_base::binary | ios_base::trunc );

   if(cfgfile.fail())
    return false;

  cfgfile.write("BFGCFG",6);
  cfgfile.write((char*)&MapWidth,sizeof(int));
  cfgfile.write((char*)&MapHeight,sizeof(int));
  cfgfile.write((char*)&CellWidth,sizeof(int));
  cfgfile.write((char*)&CellHeight,sizeof(int));
  tVal=(int)FntDef.lfHeight;
  cfgfile.write((char*)&tVal,sizeof(int));
  tVal=(int)FntDef.lfWidth;
  cfgfile.write((char*)&tVal,sizeof(int));
   if(Grid)
    Flags |= SHOW_GRID;
   if(Width)
    Flags |= SHOW_WIDTH;
  cfgfile.write((char*)&Flags,sizeof(int));
  cfgfile.write((char*)&GridCol,sizeof(BFG_RGB));
  cfgfile.write((char*)&WidthCol,sizeof(BFG_RGB));
  cfgfile.write((char*)&SelCol,sizeof(BFG_RGB));
  cfgfile.write((char*)&TextCol,sizeof(BFG_RGB));
  cfgfile.write((char*)&BkCol,sizeof(BFG_RGB));

  cfgfile.close();

  return true;
 }

void BFontMap::ResetOffsets()
 {
  int Loop;

   for(Loop=0;Loop!=256;++Loop)
    {
     WidthMod[Loop]=0;
     VMod[Loop]=0;
     HMod[Loop]=0;
    }

  gWidthMod=gHMod=gVMod=0;
 }

bool BFontMap::SaveFont(int Format, char *fname, int flags)
 {
  bool Inv,Sat;
  Inv=Sat=false;

   if(flags & SAVE_INV_ALPHA)
    Inv=true;
   
   if(flags & SAVE_RGB_SAT)
    Sat=true;

  switch(Format)
  {
   case SAVE_BFF8:
    return SaveBFF2(fname,8,Inv,false);
    break;

   case SAVE_BFF24:
    return SaveBFF2(fname,24,false,false);
    break;

   case SAVE_BFF32:
    return SaveBFF2(fname,32,Inv,Sat);
    break;

   case SAVE_BIN:
    return ExportBinData(fname);
    break;

   case SAVE_CSV:
    return ExportCSVData(fname);
  }

  return false;
 }


bool BFontMap::SaveBFF2(char *fname, char OutputBPP, bool Invert, bool RGBSat)
 {
  ofstream out;
  HBITMAP *hBMP;
  FontFileHeader Hdr;
  DIBSECTION bmInfo;
  SBM_Image FntImg,AlphaImg;
  int Loop;
  unsigned char EffWidth[256];

  out.open(fname, ios::binary | ios::trunc);
   if(out.fail())
    return false;

  // Populate header
  Hdr.ID1 = 0xBF;
  Hdr.ID2 = 0xF2;
  Hdr.BPP=24;
  Hdr.ImageWidth=MapWidth;
  Hdr.ImageHeight=MapHeight;
  Hdr.CellWidth=CellWidth;
  Hdr.CellHeight=CellHeight;
  Hdr.StartPoint=BaseChar;

  // Create the SBM image
  FntImg.Create(Hdr.ImageWidth,Hdr.ImageHeight,Hdr.BPP);

  // Render the font image
   if(OutputBPP==8)
    hBMP=DrawFontMap(DFM_ALPHA,-1);
   else
    hBMP=DrawFontMap(0,-1);

  // Grab the bitmap information
   if(!GetObject(*hBMP,sizeof(DIBSECTION),&bmInfo))
    return FALSE;

  // Copy bitmap to SBM
  memcpy(FntImg.GetImg(),bmInfo.dsBm.bmBits,(Hdr.ImageWidth*Hdr.ImageHeight)*(Hdr.BPP/8));

  // Flip memory bitmap BGR to BFF RGB
  FntImg.BGRtoRGB();

  // Free the bitmap
  delete hBMP;

  // Add in alpha channel if required
   if(OutputBPP==32)
    {
     // Render new alpha fontmap
     hBMP=DrawFontMap(DFM_ALPHA,-1);

     // Create the SBM alpha image
     AlphaImg.Create(Hdr.ImageWidth,Hdr.ImageHeight,Hdr.BPP);

     // Get RGB data ptr from Img
      if(!GetObject(*hBMP,sizeof(DIBSECTION),&bmInfo))
       return FALSE;

     // Copy bitmap to alpha SBM
     memcpy(AlphaImg.GetImg(),bmInfo.dsBm.bmBits,(Hdr.ImageWidth*Hdr.ImageHeight)*(Hdr.BPP/8));

     // Free the bitmap
     delete hBMP;

     // Post-process images and insert alpha channel into font map
     AlphaImg.Grayscale();

      if(RGBSat)
       FntImg.Saturate(0,0,0,255,255,255);

      if(Invert)
       AlphaImg.InvertCol();

     FntImg.InsertAlpha(AlphaImg.GetImg());
	 Hdr.BPP=32;
    }

   if(OutputBPP==8)
    {
     FntImg.Grayscale();

      if(Invert)
       FntImg.InvertCol();

     Hdr.BPP=8;
    }

  // Invert image
  FntImg.FlipImg();

  // Write header data
  out.write((char*)&Hdr,sizeof(Hdr));

  // Write char widths
   for(Loop=0;Loop!=256;++Loop)
    EffWidth[Loop]=BaseWidth[Loop]+WidthMod[Loop]+gWidthMod;

  out.write((char*)EffWidth,256);
  
  // Write bitmap
  out.write((char*)FntImg.GetImg(),(Hdr.ImageWidth*Hdr.ImageHeight)*(OutputBPP/8));

  out.close();

  return true;
 }


 int BFontMap::ExportMap(char* fname, int fmt)
 {
  ofstream out;
  HBITMAP *hBMP;
  FontFileHeader Hdr;
  DIBSECTION bmInfo;
  SBM_Image FntImg,AlphaImg;
  int Result;

  out.open(fname, ios::binary | ios::trunc);
   if(out.fail())
    return false;

  // Populate header
  Hdr.ID1 = 0xBF;
  Hdr.ID2 = 0xF2;
  Hdr.BPP=24;
  Hdr.ImageWidth=MapWidth;
  Hdr.ImageHeight=MapHeight;
  Hdr.CellHeight=CellHeight;
  Hdr.CellWidth=CellHeight;
  Hdr.StartPoint=BaseChar;

  // Create the SBM image
  FntImg.Create(Hdr.ImageWidth,Hdr.ImageHeight,Hdr.BPP);

  // Render the font image
  hBMP=DrawFontMap(0,-1);

  // Grab the bitmap information
   if(!GetObject(*hBMP,sizeof(DIBSECTION),&bmInfo))
    return false;

  // Copy bitmap to SBM
  memcpy(FntImg.GetImg(),bmInfo.dsBm.bmBits,(Hdr.ImageWidth*Hdr.ImageHeight)*(Hdr.BPP/8));

  // Free the bitmap
  delete hBMP;

  // Add in alpha channel if required
   if(fmt==EXPORT_TGA32)
    {
     // Render new alpha fontmap
     hBMP=DrawFontMap(DFM_ALPHA,-1);

     // Create the SBM alpha image
     AlphaImg.Create(Hdr.ImageWidth,Hdr.ImageHeight,Hdr.BPP);

     // Get RGB data ptr from Img
      if(!GetObject(*hBMP,sizeof(DIBSECTION),&bmInfo))
       return false;

     // Copy bitmap to alpha SBM
     memcpy(AlphaImg.GetImg(),bmInfo.dsBm.bmBits,(Hdr.ImageWidth*Hdr.ImageHeight)*(Hdr.BPP/8));

     // Free the bitmap
     delete hBMP;

     // Grayscale the alphamap
     AlphaImg.Grayscale();

     // Insert alpha channel into font map
     FntImg.InsertAlpha(AlphaImg.GetImg());
    }

  switch(fmt)
   {
    case EXPORT_TGA32:
     Result=FntImg.SaveTGA(fname);
     break;

    case EXPORT_TGA: 
     Result=FntImg.SaveTGA(fname);
     break;

    case EXPORT_BMP:
     Result=FntImg.SaveBMP(fname);
     break;

    default:
     Result=false;
     break;
   }

  return Result;
 }

bool BFontMap::ImportData(char *fname)
 {
/*  extern BFontMap *Fnt;

  FILE *in;
  long fsize,datptr;
  int Index,Val;
  char *data;

  in=fopen(fname,"r");

   if(in==NULL)
    return FALSE;

  // Get filesize
  fseek(in,0,SEEK_END);
  fsize=ftell(in);
  rewind(in);

  // Allocate space for file contents
  data = new char[fsize];

   if(data==NULL)
    {
     fclose(in);
     return FALSE;
    }

  // Read in the file contents
  fread(data,fsize,1,in);
  fclose(in);

  // Extract the font data
  datptr=0;

  // Image Width
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&(cfg->ImgSize));


  // Image Height
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&(cfg->ImgSize));


  // Cell Width
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&(cfg->CellHeight));


  // Cell Height
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&(cfg->CellHeight));


  // Start char
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&(cfg->CharBase));


  // Font Name
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  Index=0;
   while(data[datptr]!='\n')
    {
     cfg->FntDef.lfFaceName[Index]=data[datptr];
     ++Index;
     ++datptr;
    }
  cfg->FntDef.lfFaceName[Index]=NULL;


  // Font Height
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&(cfg->FntDef.lfHeight));


  // Font Width
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&(cfg->FntDef.lfWidth));


  // Char Widths
   for(Index=0;Index!=256;++Index)
    {
      while(data[datptr]!=',')
       ++datptr;

     datptr++;
     sscanf(&data[datptr],"%d",&Val);
     cfg->width[Index]=Val;  // Prevents stack damage
    }


  // Char X Offsets
   for(Index=0;Index!=256;++Index)
    {
      while(data[datptr]!=',')
       ++datptr;

     datptr++;
     sscanf(&data[datptr],"%d",&Val);
     cfg->hAdj[Index]=Val;
    }


  // Char Y Offsets
   for(Index=0;Index!=256;++Index)
    {
      while(data[datptr]!=',')
       ++datptr;

     datptr++;
     sscanf(&data[datptr],"%d",&Val);
     cfg->vAdj[Index]=Val;
    }


  // Global Width Offset
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&Val);
  cfg->gwAdj=Val;


  // Global X Offset
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&Val);
  cfg->ghAdj=Val;


  // Global Y Offset
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&Val);
  cfg->gvAdj=Val;


  // Bold Value
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&Val);
  cfg->FntDef.lfWeight=Val;


  // Italic Value
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&Val);
  cfg->FntDef.lfItalic=Val;


  // AntiAlias Value
   while(data[datptr]!=',')
    ++datptr;

  datptr++;
  sscanf(&data[datptr],"%d",&Val);
  cfg->FntDef.lfQuality=Val;


  delete [] data;*/

  return TRUE;
 }


bool BFontMap::ExportCSVData(char *fname)
 {
  ofstream out;
  int Loop;

  out.open(fname, ios::out | ios::trunc);
   if(out.fail())
    return false;

  out<<"Image Width,"<<MapWidth<<"\n";
  out<<"Image Height,"<<MapHeight<<"\n";
  out<<"Cell Width,"<<CellWidth<<"\n";
  out<<"Cell Height,"<<CellHeight<<"\n";
  out<<"Start Char,"<<(int)BaseChar<<"\n";
  out<<"Font Name,"<<FntDef.lfFaceName<<"\n";
  out<<"Font Height,"<<FntDef.lfHeight<<"\n";
  out<<"Font Width (0 is default),"<<FntDef.lfWidth<<"\n";

   for(Loop=0;Loop!=256;++Loop)
    {
     out<<"Char "<<Loop<<" Base Width,"<<(int)BaseWidth[Loop]<<"\n";
    }

   for(Loop=0;Loop!=256;++Loop)
    {
     out<<"Char "<<Loop<<" Width Offset,"<<(int)WidthMod[Loop]<<"\n";
    }

   for(Loop=0;Loop!=256;++Loop)
    {
     out<<"Char "<<Loop<<" X Offset,"<<(int)HMod[Loop]<<"\n";
    }

   for(Loop=0;Loop!=256;++Loop)
    {
     out<<"Char "<<Loop<<" Y Offset,"<<(int)VMod[Loop]<<"\n";
    }

  out<<"Global Width Offset,"<<(int)gWidthMod<<"\n";
  out<<"Global X Offset,"<<(int)gHMod<<"\n";
  out<<"Global Y Offset,"<<(int)gVMod<<"\n";
  out<<"Bold,"<<FntDef.lfWeight<<"\n";
  out<<"Italic,"<<(int)FntDef.lfItalic<<"\n";
  out<<"AntiAlias,"<<(int)FntDef.lfQuality<<"\n";

  out.close();

  return TRUE;
 }


bool BFontMap::ExportBinData(char *fname)
 {
  ofstream out;
  int Loop;
  unsigned char eWidth[256];

  out.open(fname, ios::binary | ios::trunc);
   if(out.fail())
    return false;

  // Image dims
  out.write((char*)&MapWidth,sizeof(int));
  out.write((char*)&MapHeight,sizeof(int));

  // Cell dims
  out.write((char*)&CellWidth,sizeof(int));
  out.write((char*)&CellHeight,sizeof(int));

  // Start char
  out.write((char*)&BaseChar,1);
  
  // Font Widths  
   for(Loop=0;Loop!=256;++Loop)
    eWidth[Loop]=BaseWidth[Loop]+WidthMod[Loop]+gWidthMod;

  out.write((char*)eWidth,256);

  out.close();

  return TRUE;
 }


bool BFontMap::IsPower(int TestValue)
 {
  bool Ret=FALSE;
  float Val;

  Val=(float)TestValue;

   while(Val>=2.0f)
    {
      if(Val==2.0f)
       Ret=TRUE;

     Val=Val/2.0f;
    }

  return Ret;
 }