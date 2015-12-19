#ifndef _SBM_UTIL
#define _SBM_UTIL
// SBM (Simple BitMap) Loader

// Change Log
//
// 27/05/06 - Added SaveRaw function.
// Writes pixel data only to file.
//
// 26/05/06 - Added Saturate function.
// Sets color values other than the specified key values to sat value.
//
// 26/05/06 - Added InvertCols function.
// Inverts pixel colour values.
// Currently only supports 8 bit source images
//
// 26/05/06 - Added Grayscale function.
// Reduces images to 8 bit grayscale using R 0.3 G 0.59 B 0.11
// Currently only supports 24 bit source images

#define SBM_OK              0x1
#define SBM_ERR_NO_FILE     0x2
#define SBM_ERR_MEM_FAIL    0x4
#define SBM_ERR_BAD_FORMAT  0x8
#define SBM_ERR_UNSUPPORTED 0x40

#pragma once
#include <fstream>
#include <memory.h>
using namespace std;

class SBM_Image
 {
  public:
   SBM_Image();
   ~SBM_Image();
   int Create(int width, int height, int bpp);
   void Reset();                  // Clear Image
   int Load(char* filename);      // Load BMP,PCX,TGA or SBM image
   int GetBPP();
   int GetWidth();
   int GetHeight();
   unsigned char* GetImg();       // Return a pointer to image data
   unsigned char* GetPalette();   // Return a pointer to VGA palette

   // Utility Functions
   void FlipImg();   // Invert image vertically
   int InsertAlpha(unsigned char *Alpha); // Adds an alpha channel to image
   int Grayscale();  // Converts image to 8 bit gray
   int InvertCol();  // Inverts colour values
   void BGRtoRGB();  // Convert between RGB and BGR formats

   // Sets all non-KeyCol pixels to SatCol
   int Saturate(unsigned char KeyR, unsigned char KeyG, unsigned char KeyB,
                unsigned char SatR, unsigned char SatG, unsigned char SatB);   
                     

   int SaveBMP(char *filename);
   int SavePCX(char *filename);
   int SaveTGA(char *filename);
   int SaveRaw(char *filename);

  private:
   short Width,Height;
   char BPP,Encode,Planes;
   unsigned long FileSize,ImageSize,Offset;
   unsigned char *ImgData,*PalData,*FileData;
   short BPL;

   int LoadBMP();
   int ReadBMPHeader();
   int LoadBMPRawData();
   int LoadBMPRLE8Data();
   int LoadBMPPalette();

   int LoadPCX();
   int ReadPCXHeader();
   int LoadPCXRLEData();
   int LoadPCXPalette();
   
   int LoadTGA();
   int ReadTGAHeader();
   int LoadTGARawData();
   int LoadTGARLEData();
   int LoadTGAPalette();

   int LoadSBM();

   void FreeMem(void **Ptr); // Safe delete []
 };

#endif