#ifndef _FONTMAPCLASS_H
#define _FONTMAPCLASS_H

#include <windows.h>
#include "cbfgdefs.h"
#include "UtilFunctions.h"

class BFontMap
 {
  public:
   BFontMap();
   ~BFontMap();
   int  GetSize(int Which);
   int  SetSize(int Which, int NewSize);
   unsigned char GetBaseChar();
   unsigned char SetBaseChar(int);
   char GetGlobal(int Which);
   char SetGlobal(int Which, char NewVal);
   char GetCharVal(int Char, int Which);
   char SetCharVal(int Char, int Which, char NewVal );
   long GetFontHeight();
   long SetFontHeight(long);
   long GetFontWidth();
   long SetFontWidth(long);
   long GetFontWeight();
   long SetFontWeight(long);
   long GetFontQuality();
   long SetFontQuality(long);
   long GetFontItalic();
   long SetFontItalic(long);
   char* GetFontName();
   bool SetFontName(char*);
   bool CalcWidths(HDC);
   HBITMAP* DrawFontMap(int Flags, int Sel);

   int  LoadConfig(char *fname);
   bool SaveConfig(char *fname, bool Grid, bool Width);
   void ResetOffsets();

   bool SaveFont(int Format, char *fname, int Flags=0);
   int  ExportMap(char* fname, int fmt);
   bool ImportData(char *fname);

   void SetCol(int Which, BYTE Red, BYTE Green, BYTE Blue);
   void SetCol(int Which, BFG_RGB Col);
   BFG_RGB GetCol(int Which);

  private:
   LOGFONT FntDef;
   HFONT fnt;
   int  MapWidth,MapHeight;
   int  CellHeight,CellWidth;
   unsigned char BaseChar;
   unsigned char BaseWidth[256];
   char WidthMod[256];
   char VMod[256];
   char HMod[256];
   char gWidthMod,gHMod,gVMod;
   BFG_RGB BkCol,TextCol,GridCol,WidthCol,SelCol;

   bool IsPower(int TestVal);
   bool SaveBFF2(char *fname, char OutputBPP, bool Invert, bool RGBSat);
   bool ExportCSVData(char *fname);
   bool ExportBinData(char *fname);
 };

#endif