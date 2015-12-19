#include "stdafx.h"
#include "WriteFont.h"

int SaveFont(char *fname)
 {
  //extern HWND hMain;
  extern BFontMap *Fnt;
  extern AppConfig *AppCfg;
  extern AppInfo *info;

  FILE *out;
  FontFileHeader Hdr;
  HBITMAP *hBMP;
  unsigned char *Alpha;
  DIBSECTION bmInfo;
  SBM_Image Bits;

  // Populate header
  lstrcpy(Hdr.ID,"CBFG");
  Hdr.VerMaj=1;
  Hdr.VerMin=0;
  Hdr.BPP=32;
  Hdr.ImageWidth=Fnt->GetSize(MAPWIDTH);
  Hdr.ImageHeight=Fnt->GetSize(MAPHEIGHT);
  Hdr.CellHeight=Fnt->GetSize(CELLHEIGHT);
  Hdr.CellWidth=Fnt->GetSize(CELLWIDTH);
  Hdr.StartPoint=Fnt->GetBaseChar();

  //GetWidths();

  //hBMP=RenderFont(FALSE);

  Bits.Create(Hdr.ImageWidth,Hdr.ImageHeight,24);

   if(!GetObject(*hBMP,sizeof(DIBSECTION),&bmInfo))
    return FALSE;

  // Copy bitmap to img
  memcpy(Bits.GetImg(),bmInfo.dsBm.bmBits,(Hdr.ImageWidth*Hdr.ImageHeight)*(24/8));

  delete hBMP;

  // Add in Alpha Channel
  //Alpha=MakeAlpha();
  Bits.InsertAlpha(Alpha);

  // Invert image
  Bits.FlipImg();

  out=fopen(fname,"wb");

   if(out==NULL)
    return FALSE;

  // Write header data
  fwrite(&Hdr,sizeof(Hdr),1,out);

  // Write char widths
  //fwrite(cfg->width,256,1,out);
  
  // Write bitmap
  fwrite(Bits.GetImg(),(Hdr.ImageWidth*Hdr.ImageHeight)*(Hdr.BPP/8),1,out);

  fclose(out);

  return TRUE;
 }