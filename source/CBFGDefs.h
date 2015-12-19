#include "stdafx.h"

#ifndef _CBFGDEFS_H
#define _CBFGDEFS_H

#define APP_VERSION 1.41

#define SHOW_GRID   0x1
#define SHOW_WIDTH  0x2
#define FONT_BOLD   0x4
#define FONT_ITALIC 0x8

#define WIDTH   1
#define HEIGHT  2
#define HOFFSET 3
#define VOFFSET 4
#define WOFFSET 5
#define EWIDTH  6

// SetSize Constants
#define MAPWIDTH    1
#define MAPHEIGHT   2
#define CELLWIDTH   3
#define CELLHEIGHT  4
#define MAXCHARS   10

// DrawFontMap Constants
#define DFM_WIDTHLINE 2
#define DFM_GRIDLINES 4
#define DFM_ALPHA     8

// Config Window Constants
#define GRIDCOL  0
#define WIDTHCOL 1
#define SELCOL   2
#define TEXTCOL  3
#define BACKCOL  4

// FontSave Constants
#define SAVE_BFF8  101
#define SAVE_BFF24 102
#define SAVE_BFF32 103
#define SAVE_CSV   110
#define SAVE_BIN   120
#define SAVE_BMP   130
#define SAVE_TGA   140
#define SAVE_TGA32 141

#define SAVE_RGB_SAT   0x100
#define SAVE_INV_ALPHA 0x200

typedef struct
 {
  unsigned char Red,Green,Blue;
 }BFG_RGB;

typedef struct
 {
  int hScr,vScr;
  bool Grid,wMarker,ModAll,vScroll,hScroll;
  float Zoom;
  int Select,MaxChars;
 }AppInfo;

typedef struct
 {
  unsigned char ID1,ID2;
  int ImageWidth,ImageHeight,CellWidth,CellHeight;
  unsigned char BPP,StartPoint;
 }FontFileHeader;

typedef struct
 {
  char ID[7];
  int ImgWidth,ImgHeight,CellWidth,CellHeight,FontHeight,FontWidth;
  BFG_RGB BackCol,ForeCol,GridCol,WidthCol,SelCol;
  int Flags;
 }AppConfig;

#endif
