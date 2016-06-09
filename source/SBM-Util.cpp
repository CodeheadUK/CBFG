#include "StdAfx.h"
#include "SBM-Util.h"

SBM_Image::SBM_Image()
 {
  Width=Height=0;
  FileSize=ImageSize=0;
  BPP=Encode=0;
  ImgData=PalData=FileData=NULL;
 }

SBM_Image::~SBM_Image()
 {
   FreeMem((void**)&ImgData);
   FreeMem((void**)&PalData);
   FreeMem((void**)&FileData);
 }

// Create an empty image with specified size and colour depth
int SBM_Image::Create(int width, int height, int bpp)
 {
   switch(bpp)
    {
     case 24:
     case 32:
      FreeMem((void**)&ImgData);
      FreeMem((void**)&PalData);
      FreeMem((void**)&FileData);

      ImgData=new unsigned char[(width*height)*(bpp/8)];

       if(ImgData==NULL)
        return SBM_ERR_MEM_FAIL;

      Width=width;
      Height=height;
      BPP=bpp;

      break;

     default:
      return SBM_ERR_UNSUPPORTED;      
    }

  return SBM_OK;
 }

void SBM_Image::Reset()
 {
  Width=Height=0;
  FileSize=ImageSize=0;
  BPP=Encode=0;

  FreeMem((void**)&ImgData);
  FreeMem((void**)&PalData);
  FreeMem((void**)&FileData);
 }

// Get file into memory, find extension and call relevant loader
int SBM_Image::Load(char* filename)
 {
  bool MyStrCmp(char* Str1, char* Str2);

  ifstream In;
  char *StrPtr;
  int RetVal;

  // Clear any existing image
  FreeMem((void**)&ImgData);
  FreeMem((void**)&PalData);
  FreeMem((void**)&FileData);

  // Open the specified file
  In.open(filename,ios::binary);
    
   if(!In)
    return SBM_ERR_NO_FILE;

  // Get file size
  In.seekg(0,ios_base::end);
  FileSize=In.tellg();
  In.seekg(0,ios_base::beg);

  // Allocate some space
  FileData=new unsigned char[FileSize];

   if(FileData==NULL)
    {
     In.close();
     return SBM_ERR_MEM_FAIL;
    }

  // Read the file into memory
  In.read((char*)FileData,FileSize);
  In.close();


  StrPtr=filename;

   while(*StrPtr!=NULL) // Seek end of string
    ++StrPtr;

   while(*StrPtr!='.') // Wind back to first '.'
    {
      if(StrPtr==filename) // Prevent rewind past start of filename
       return SBM_ERR_UNSUPPORTED;

      --StrPtr;
    }

  // Check extensions and call loader
   if(MyStrCmp(StrPtr,".bmp"))
    {
     return LoadBMP();
    }
   else if(MyStrCmp(StrPtr,".pcx"))
    {
     return LoadPCX();
    }
   else if(MyStrCmp(StrPtr,".tga"))
    {
     RetVal=LoadTGA();
    }
   else if(MyStrCmp(StrPtr,".sbm"))
    {
     return LoadSBM();
    }
   else
    {
     return SBM_ERR_UNSUPPORTED;
    }

  // Free the filedata memory
  FreeMem((void**)&FileData);

   if(RetVal!=SBM_OK)
    return RetVal;
  
  return SBM_OK;
 }


bool MyStrCmp(char* Str1, char* Str2) // Case insenstive string comparison
 {
   while((*Str1 > 96 ? *Str1-32 : *Str1)==(*Str2 > 96 ? *Str2-32 : *Str2))
    {
     if(*Str1==NULL)
      return 1;
     Str1++;
     Str2++;
    }
  return 0;
 }


int SBM_Image::LoadBMP()
 {
  int iRet;

  // Process the header
  iRet=ReadBMPHeader();

   if(iRet!=SBM_OK)
    return iRet;

   if(BPP<8) // We'll only bother with 8 bit and above
    return SBM_ERR_UNSUPPORTED;

  // Get the image data
   switch(Encode)
    {
     case 0: // Uncompressed
       iRet=LoadBMPRawData(); // 8 / 24 Bit. (24 bit is in BGR order)
       BGRtoRGB();
      break;

     case 1: // RLE 8 (Indexed 256 colour only)
       iRet=LoadBMPRLE8Data();
      break;

     case 2: // RLE 4 (16 Colour indexed, Outdated, not covered here)
      return SBM_ERR_UNSUPPORTED;

     case 3: // Bitfields (16/32 bit only, Rare, not covered here)
      return SBM_ERR_UNSUPPORTED;

     default:
      return SBM_ERR_UNSUPPORTED;
    }

   if(iRet!=SBM_OK)
    return iRet;

  // Flip image to correct scanline reversal
  FlipImg();

  // Load palette if present
  iRet=LoadBMPPalette();

   if(iRet!=SBM_OK)
    return iRet;

  return SBM_OK;
 }


int SBM_Image::ReadBMPHeader()
 {
  int iInfo,iImgOffset,iEnc;
  unsigned int iWidth,iHeight;
  short iPlanes,iBPP;
  
   if(FileData==NULL)
    return SBM_ERR_NO_FILE;

   if(FileData[0x0]!='B' || FileData[0x1]!='M') // BMP ID Bytes, should be 'BM'
    return SBM_ERR_BAD_FORMAT;

  memcpy(&iImgOffset,&FileData[0xA],4);   // Offset to image data
  Offset=iImgOffset;
   
  memcpy(&iInfo,&FileData[0xE],4);        // Info header size, should be 0x28
   if(iInfo!=0x28)
    return SBM_ERR_BAD_FORMAT;
 
  memcpy(&iWidth,&FileData[0x12],4);   // Image width
  Width=(short)iWidth;
  memcpy(&iHeight,&FileData[0x16],4);  // Image height
  Height=(short)iHeight;
  memcpy(&iPlanes,&FileData[0x1A],2);  // Colour planes
  Planes=(char)iPlanes;
  memcpy(&iBPP,&FileData[0x1C],2);     // BPP
  BPP=(char)iBPP;
  memcpy(&iEnc,&FileData[0x1E],4);     // Encoding
  Encode=(char)iEnc;

  ImageSize=(iWidth*iHeight*(iBPP/8)); // Calculate Image Data size

  return SBM_OK;
 }


int SBM_Image::LoadBMPRawData()
 {
   FreeMem((void**)&ImgData);

  // Allocate space for the image data
  ImgData=new unsigned char[ImageSize];

   if(ImgData==NULL)
    return SBM_ERR_MEM_FAIL;

  memcpy(ImgData,&FileData[Offset],ImageSize);

  return SBM_OK;
 }


int SBM_Image::LoadBMPRLE8Data()
 {
	 unsigned char bOpCode,bVal;
	 unsigned char *pSrc;
	 int iDcode=1,iCount,iPos;
  unsigned long iIndex;

  FreeMem((void**)ImgData);

  ImgData=new unsigned char[ImageSize];

   if(ImgData==NULL)
    return SBM_ERR_MEM_FAIL;

 // Get the start of the RLE data
 pSrc=&FileData[Offset];

	iPos=0;
	iIndex=0;

  while(iDcode)
		 {
     // Stay on even bytes
		   while(iPos%2)
			   {
			    iPos++;
			   }

    bOpCode=pSrc[iPos];
			 bVal=pSrc[iPos+1];
			 iPos+=2;

			  if(bOpCode>0) // Run mode, Repeat 'bVal' 'OpCode' times
			   {
         for(iCount=0;iCount!=bOpCode;iCount++)
					     {
					      ImgData[iIndex]=bVal;
					      ++iIndex;
					     }
			   }
			  else // Absolute Mode (Opcode=0), various options
			   {
			     switch(bVal)
					    {
					     case 0:  // EOL, no action
						     break;

					     case 1: 	// EOF, STOP!
						      iDcode=0;
						     break;

					     case 2:  // Reposition, Never used
						     break;

					     default: // Copy the next 'bVal' bytes direct to the image
						       for(iCount=bVal;iCount!=0;iCount--)
							       {
							        ImgData[iIndex]=pSrc[iPos];
					  		      ++iIndex;
							        ++iPos;
							       }
						   	 break;
 					   }

			   }

			  if(iIndex>ImageSize) // Stop if image size exceeded.
			   iDcode=0;
		 }
  
  return SBM_OK;
 }


int SBM_Image::LoadBMPPalette()
 {
  int iIndex;
  unsigned char *pPalPos, *pDatPos;
  
  FreeMem((void**)&PalData);

   if(BPP>8) // NULL Palette for RGB images
    return SBM_OK;

  // Create space for palette
  PalData=new unsigned char[768];

   if(PalData==NULL)
    return SBM_ERR_MEM_FAIL;

  // Set starting position for pointers
  pPalPos=PalData;
  pDatPos=&FileData[0x36];

  // Get colour values, skip redundant 4th value
   for(iIndex=0;iIndex!=256;++iIndex)
    {
     pPalPos[0]=pDatPos[2]; // Red
     pPalPos[1]=pDatPos[1]; // Green
     pPalPos[2]=pDatPos[0]; // Blue

     pPalPos+=3;
     pDatPos+=4;
    }

  return SBM_OK;
 }

int SBM_Image::SaveBMP(char* fname)
 {
  ofstream out;
  int Res=SBM_ERR_UNSUPPORTED;
  DWORD Data;
  WORD wData;
  
   switch(BPP)
    {
     case 24:
      out.open(fname,ios::binary | ios::trunc);
       if(out.fail())
        {
         Res=SBM_ERR_NO_FILE;
         break;
        }

      // Write ID
      out.write("BM",2);

      // Write filesize
      Data=/*0x436*/54+((Width*Height)*(BPP/8));
      out.write((char*)&Data,4);

      // Write reserved area
      Data=0;
      out.write((char*)&Data,4);

      // Write offset
      Data=0x36;
      out.write((char*)&Data,4);

      // Write header size
      Data=0x28;
      out.write((char*)&Data,4);

      // Write width
      Data=Width;
      out.write((char*)&Data,4);

      // Write height
      Data=Height;
      out.write((char*)&Data,4);

      // Write planes
      wData=1;
      out.write((char*)&wData,2);

      // Write BPP
      wData=BPP;
      out.write((char*)&wData,2);

      // Write Compression
      Data=0;
      out.write((char*)&Data,4);

      // Write data size
      Data=(Width*Height)*(BPP/8);
      out.write((char*)&Data,4);

      // Write Resolutions and Colors
      Data=0;
      out.write((char*)&Data,4);
      out.write((char*)&Data,4);
      out.write((char*)&Data,4);
      out.write((char*)&Data,4);

      // Write image data
      out.write((char*)ImgData,(Width*Height)*(BPP/8));

      out.close();

      Res=SBM_OK;
      break;

     default:
      Res=SBM_ERR_UNSUPPORTED;
      break;
    }

  return Res;
 }


int SBM_Image::LoadPCX()
 {
  int iRet;

   if(FileData==NULL)
    return SBM_ERR_NO_FILE;
  
  // Process the header
  iRet=ReadPCXHeader();

   if(iRet!=SBM_OK)
    return iRet;

   if(BPP!=8) // We'll only bother with 8 bit indexed and 24 bit RGB images
    return SBM_ERR_UNSUPPORTED;

   if(Encode!=1) // We only know about RLE compressed images
    return SBM_ERR_UNSUPPORTED;

  // Get the image data
  iRet=LoadPCXRLEData();

   if(iRet!=SBM_OK)
    return iRet;

  // Load palette if present
  iRet=LoadPCXPalette();

   if(iRet!=SBM_OK)
    return iRet;

  // Update the BPP value to reflect the image format
  BPP*=Planes;
 
  return SBM_OK;
 }


int SBM_Image::ReadPCXHeader()
 {
  int x1,x2,y1,y2;

   if(FileData==NULL)
    return SBM_ERR_NO_FILE;

   if(FileData[0]!=0xA) // PCX ID Byte, should be 0xA
    return SBM_ERR_BAD_FORMAT;

   if(FileData[1]>5)    // Version, we don't know about anything after v5
    return SBM_ERR_UNSUPPORTED;

  Encode=FileData[2];     // Encode flag 1 = RLE Compression

   if(FileData[3]==1 || FileData[3]==2 || FileData[3]==4 || FileData[3]==8)   // BPP value
    BPP=FileData[3];  
   else
    return SBM_ERR_BAD_FORMAT;

  // Get image window and produce width & height values
  memcpy(&x1,&FileData[4],2);
  memcpy(&y1,&FileData[6],2);
  memcpy(&x2,&FileData[8],2);
  memcpy(&y2,&FileData[10],2);

  Width=(x2-x1)+1;
  Height=(y2-y1)+1;

   if(Width<1 || Height<1)
    return SBM_ERR_BAD_FORMAT;

  // Planes byte.  1 = Indexed,  3 = RGB
  Planes=FileData[65];

  // Bits per line for decoding purposes,
  memcpy(&BPL,&FileData[66],2);

  return SBM_OK;
 }


int SBM_Image::LoadPCXRLEData()
 {
  int iLineCount,iBufferLineLen,iImageLineLen;
  long lLinePos=0;
  unsigned char bRunLen;
  unsigned char *pCur,*pLine,*pInterLine;

  // Set our pointer to the beginning of the image data
  pCur=&FileData[128];

  // Calculate line lengths for image and buffer, Allocate the buffer scan line
  iBufferLineLen=BPL*Planes;
  iImageLineLen =Width*Planes;
  pLine=new unsigned char[iBufferLineLen];
   
   if(pLine==NULL)
    return SBM_ERR_MEM_FAIL;

  FreeMem((void**)&ImgData);

  ImgData=new unsigned char[(iImageLineLen * Height)+1];

   if(ImgData==NULL)
    return SBM_ERR_MEM_FAIL;

   // Decode each scanline
   for(iLineCount=0;iLineCount<Height;++iLineCount)
    {
     lLinePos=0;
      while(lLinePos<iBufferLineLen)
       {
         if(*pCur > 0xC0) // First 2 bits indicate run of next byte value
          {
           bRunLen=*pCur & 0x3F; // Remaining 6 bits indicate run length
           ++pCur;  // Repeated value 
            for( ;bRunLen!=0;bRunLen--,lLinePos++)
             pLine[lLinePos]=*pCur;
     
           ++pCur; 
          } 
         else
          {
           pLine[lLinePos]=*pCur; // Other bytes are directly copied
           ++lLinePos;
           ++pCur;
          }
       }

     // Once we've decoded a line, copy it to the image.
     // This disregards any end-of-line padding inserted during the compression

      if(Planes==1) // 8 bit images, straight copy
       {
        memcpy(&ImgData[iLineCount*iImageLineLen],pLine,iImageLineLen);
       }
      else if(Planes==3) // for 24 bit, We have to interleave the RGB values
       {
        pInterLine=&ImgData[iLineCount*iImageLineLen];
         for(lLinePos=0;lLinePos!=Width;++lLinePos,pInterLine+=3)
          { 
           pInterLine[0]=pLine[lLinePos];
           pInterLine[1]=pLine[lLinePos+Width];
           pInterLine[2]=pLine[lLinePos+(Width*2)];
          }
       }

    } 

  return SBM_OK;
 }


int SBM_Image::LoadPCXPalette()
 {
  // Load a 256 color palette
  
 FreeMem((void**)&PalData);

   if(Planes==3) // NULL Palette for RGB images
    return SBM_OK;

  // Create space for palette
  PalData=new unsigned char[768];

   if(PalData==NULL)
    return SBM_ERR_MEM_FAIL;

  // Start of palette entries should be 769 bytes back from the end of the file
  // First byte is 0x0C
   if(FileData[FileSize-769]!=0x0C)
    return SBM_ERR_BAD_FORMAT;

  memcpy(PalData,&FileData[FileSize-768],768);

  return SBM_OK;
 }


int SBM_Image::SavePCX(char* fname)
 {
  return SBM_ERR_UNSUPPORTED;
 }


int SBM_Image::LoadTGA()
 {
  int iRet,ImageType,ImgDataOffset;

  iRet=ReadTGAHeader();

   if(iRet!=SBM_OK)
    return iRet;

  ImgDataOffset=FileData[0];
  ImageType=FileData[1];

   switch(Encode)
    {
     case 1: // Raw Indexed
      {
       // Check filesize against header values
        if((ImageSize+18+ImgDataOffset+768)>FileSize)
         return SBM_ERR_BAD_FORMAT;

       // Double check image type field
        if(ImageType!=1)
         return SBM_ERR_BAD_FORMAT;

       // Load image data
       iRet=LoadTGARawData();
        if(iRet!=SBM_OK)
         return iRet;

       // Load palette
       iRet=LoadTGAPalette();
        if(iRet!=SBM_OK)
         return iRet;

       break;
      }

     case 2: // Raw RGB
      {
       // Check filesize against header values
        if((ImageSize+18+ImgDataOffset)>FileSize)
         return SBM_ERR_BAD_FORMAT;

       // Double check image type field
        if(ImageType!=0)
         return SBM_ERR_BAD_FORMAT;

       // Load image data
       iRet=LoadTGARawData();
        if(iRet!=SBM_OK)
         return iRet;

       BGRtoRGB(); // Convert to RGB
       break;
      }

     case 9: // RLE Indexed
      {
       // Double check image type field
        if(ImageType!=1)
         return SBM_ERR_BAD_FORMAT;

       // Load image data
       iRet=LoadTGARLEData();
        if(iRet!=SBM_OK)
         return iRet;

       // Load palette
       iRet=LoadTGAPalette();
        if(iRet!=SBM_OK)
         return iRet;

       break;
      }
 
     case 10: // RLE RGB
      {
       // Double check image type field
        if(ImageType!=0)
         return SBM_ERR_BAD_FORMAT;

       // Load image data
       iRet=LoadTGARLEData();
        if(iRet!=SBM_OK)
         return iRet;

       BGRtoRGB(); // Convert to RGB
       break;
      }

     default:
      return SBM_ERR_UNSUPPORTED;
    }

  // Check flip bit
   if((FileData[17] & 0x20)==0) 
    FlipImg();

  return SBM_OK;
 }

int SBM_Image::ReadTGAHeader()
 {
  short ColMapStart,ColMapLen;
  short x1,y1,x2,y2;

   if(FileData==NULL)
    return SBM_ERR_NO_FILE;

   if(FileData[1]>1)    // 0 (RGB) and 1 (Indexed) are the only types we know about
    return SBM_ERR_UNSUPPORTED;

   Encode=FileData[2];// Encoding flag  1 = Raw indexed image
                      //                2 = Raw RGB
                      //                3 = Raw greyscale
                      //                9 = RLE indexed
                      //               10 = RLE RGB
                      //               11 = RLE greyscale
                      //               32 & 33 Other compression, indexed

    if(Encode>11)     // We don't want 32 or 33
     return SBM_ERR_UNSUPPORTED;

  // Get palette info
  memcpy(&ColMapStart,&FileData[3],2);
  memcpy(&ColMapLen,&FileData[5],2);

  // Reject indexed images if not a VGA palette (256 entries with 24 bits per entry)
   if(FileData[1]==1) // Indexed
    {
     if(ColMapStart!=0 || ColMapLen!=256 || FileData[7]!=24)
      return SBM_ERR_UNSUPPORTED;
    }

  // Get image window and produce width & height values
  memcpy(&x1,&FileData[8],2);
  memcpy(&y1,&FileData[10],2);
  memcpy(&x2,&FileData[12],2);
  memcpy(&y2,&FileData[14],2);

  Width=(x2-x1);
  Height=(y2-y1);

   if(Width<1 || Height<1)
    return SBM_ERR_BAD_FORMAT;

  // Bits per Pixel
  BPP=FileData[16];

  // Check flip / interleave byte
   if(FileData[17]>32) // Interleaved data
    return SBM_ERR_UNSUPPORTED;

  // Calculate image size
   ImageSize=(Width * Height * (BPP/8));

  return SBM_OK;
 }

int SBM_Image::LoadTGARawData()
 {
  short iOffset;

  FreeMem((void**)&ImgData);

  ImgData=new unsigned char[ImageSize];

   if(ImgData==NULL)
    return SBM_ERR_MEM_FAIL;

  iOffset=FileData[0]+18; // Add header to ident field size

   if(FileData[1]==1) // Indexed images
    iOffset+=768;  // Add palette offset

   memcpy(ImgData,&FileData[iOffset],ImageSize);

  return SBM_OK;
 }


int SBM_Image::LoadTGARLEData()
 {
  short iOffset,iPixelSize;
  unsigned char *pCur;
  unsigned long Index=0;
  unsigned char bLength,bLoop;

  // Calculate offset to image data
  iOffset=FileData[0]+18;

  // Add palette offset for indexed images
   if(FileData[1]==1)
    iOffset+=768; 

  // Get pixel size in bytes
  iPixelSize=BPP/8;

  // Set our pointer to the beginning of the image data
  pCur=&FileData[iOffset];

  // Allocate space for the image data
  FreeMem((void**)&ImgData);
  ImgData=new unsigned char[ImageSize];

   if(ImgData==NULL)
    return SBM_ERR_MEM_FAIL;

  // Decode
   while(Index<ImageSize) 
    {
      if(*pCur & 0x80) // Run length chunk (High bit = 1)
       {
        bLength=*pCur-127; // Get run length
        pCur++;            // Move to pixel data  

        // Repeat the next pixel bLength times
         for(bLoop=0;bLoop!=bLength;++bLoop,Index+=iPixelSize)
          memcpy(&ImgData[Index],pCur,iPixelSize);
  
        pCur+=iPixelSize; // Move to the next descriptor chunk
       }
      else // Raw chunk
       {
        bLength=*pCur+1; // Get run length
        pCur++;          // Move to pixel data

        // Write the next bLength pixels directly
         for(bLoop=0;bLoop!=bLength;++bLoop,Index+=iPixelSize,pCur+=iPixelSize)
          memcpy(&ImgData[Index],pCur,iPixelSize);
       }
    }
 
  return SBM_OK;
 }


int SBM_Image::LoadTGAPalette()
 {
  unsigned char bTemp;
  short iIndex,iPalPtr;

  // Load a 256 color palette
  
  // Delete old palette if present
  FreeMem((void**)&PalData);
  // Create space for new palette
  PalData=new unsigned char[768];

   if(PalData==NULL)
    return SBM_ERR_MEM_FAIL;

  // VGA palette is the 768 bytes following the header
  memcpy(PalData,&FileData[FileData[0]+18],768);

  // Palette entries are BGR ordered so we have to convert to RGB
   for(iIndex=0,iPalPtr=0;iIndex!=256;++iIndex,iPalPtr+=3)
    {
     bTemp=PalData[iPalPtr];               // Get Blue value
     PalData[iPalPtr]=PalData[iPalPtr+2]; // Copy Red to Blue
     PalData[iPalPtr+2]=bTemp;             // Replace Blue at the end
    }

  return SBM_OK;
 }


int SBM_Image::SaveTGA(char *filename)
 {
  ofstream out;


   switch(BPP)
    {
     case 24:
     case 32:
      // Open output file
   out.open(filename,ios::binary | ios::trunc);

       if(out.fail())
        return SBM_ERR_NO_FILE;

   // Write ID,PalType and ImgType
   out.put(0);
   out.put(0);
   out.put(2);

   // Write zeros into PalInfo area
   out.put(0);
   out.put(0);
   out.put(0);
   out.put(0);

   // Write BPP into PalInfo
   out.write(&BPP,1);

   // Write zeros into Img Start co-ords
   out.put(0);
   out.put(0);
   out.put(0);
   out.put(0);

   // Write Width, Height and BPP
   out.write((char*)&Width,2);
   out.write((char*)&Height,2);
   out.write((char*)&BPP,1);

   // Write descriptor
   out.put(0);

   // Write Image data
   out.write((char*)ImgData,((Width*Height)*(BPP/8)));

   // Write Footer
   out.write("\0\0\0\0\0\0\0\0TRUEVISION-XFILE.\0",26);

      // Close file
      out.close();
      break;

     default:
      return SBM_ERR_UNSUPPORTED;
    }

  return SBM_OK;
 }


int SBM_Image::SaveRaw(char *filename)
 {
  ofstream out;

  // Open output file
  out.open(filename,ios::binary | ios::trunc);

   if(out.fail())
    return SBM_ERR_NO_FILE;

  // Write Image data
  out.write((char*)ImgData,((Width*Height)*(BPP/8)));

  // Close file
  out.close();

  return SBM_OK;
 }


int SBM_Image::LoadSBM()
 {
  memcpy(&Width,&FileData[0],2);
  memcpy(&Height,&FileData[2],2);
  BPP=FileData[4];

  ImageSize=(Height*Width*(BPP/8));

  // Check filesize is imagesize+5
   if(ImageSize+5 != FileSize)
    return SBM_ERR_BAD_FORMAT;

  ImgData=new unsigned char[ImageSize];

   if(ImgData==NULL)
    return SBM_ERR_MEM_FAIL;

  memcpy(ImgData,&FileData[5],ImageSize);

  return SBM_OK;
 }

void SBM_Image::FreeMem(void **Ptr)
 {
   if(*Ptr!=NULL)
    {
     delete [] *Ptr;
     *Ptr=NULL;
    }
 }

void SBM_Image::BGRtoRGB()
 {
  unsigned long Index,nPixels;
  unsigned char *bCur;
  unsigned char bTemp;
  short iPixelSize;

  // Set ptr to start of image
  bCur=ImgData;

  // Calc number of pixels
  nPixels=Width*Height;

  // Get pixel size in bytes
  iPixelSize=BPP/8;

   for(Index=0;Index!=nPixels;Index++)  // For each pixel
    {
     bTemp=*bCur;      // Get Blue value
     *bCur=*(bCur+2);  // Swap red value into first position
     *(bCur+2)=bTemp;  // Write back blue to last position

     bCur+=iPixelSize; // Jump to next pixel
    }
 }

int SBM_Image::InsertAlpha(unsigned char *Alpha)
 {
  unsigned char *NewImg;
  int PixLoop;
  int RGBPtr,ImgPtr;

   if(BPP!=24)
    return SBM_ERR_UNSUPPORTED;

  NewImg=new unsigned char[(Width*Height)*4];

   if(NewImg==NULL)
    return SBM_ERR_MEM_FAIL;

   for(PixLoop=0,RGBPtr=0,ImgPtr=0;PixLoop!=(Width*Height);++PixLoop)
    {
     NewImg[ImgPtr]=ImgData[RGBPtr];
     NewImg[ImgPtr+1]=ImgData[RGBPtr+1];
     NewImg[ImgPtr+2]=ImgData[RGBPtr+2];
     NewImg[ImgPtr+3]=Alpha[PixLoop];
     RGBPtr+=3;
     ImgPtr+=4;
    }

  FreeMem((void**)&ImgData);
  ImgData=NewImg;
  BPP=32;
  ImageSize=((Width*Height)*(BPP/8));

  return SBM_OK;
 }


void SBM_Image::FlipImg()
 {
  unsigned char bTemp;
  unsigned char *pLine1, *pLine2;
  int iLineLen,iIndex;
 
  iLineLen=Width*(BPP/8);
  pLine1=ImgData;
  pLine2=&ImgData[iLineLen * (Height - 1)];

   for( ;pLine1<pLine2;pLine2-=(iLineLen*2))
    {
     for(iIndex=0;iIndex!=iLineLen;pLine1++,pLine2++,iIndex++)
      {
       bTemp=*pLine1;
       *pLine1=*pLine2;
       *pLine2=bTemp;       
      }
    } 
 }


int SBM_Image::Grayscale()
 {
  unsigned char *NewData;
  unsigned long ImgPtr,AlphaPtr;
  float AlphaVal;

  NewData=new unsigned char[Width*Height];

   if(NewData==NULL)
    return SBM_ERR_MEM_FAIL;

   switch(BPP)
    {
     case 24:
      ImageSize=Width*Height;
       for(ImgPtr=0,AlphaPtr=0;AlphaPtr<ImageSize;AlphaPtr++,ImgPtr+=3)
        {
         AlphaVal=ImgData[ImgPtr]*0.3f;
         AlphaVal+=ImgData[ImgPtr+1]*0.59f;
         AlphaVal+=ImgData[ImgPtr+2]*0.11f;
         NewData[AlphaPtr]=(int)AlphaVal;           
        }

      delete [] ImgData;
      ImgData=NewData;
      BPP=8;
      break;

     default:
      delete [] NewData;
      return SBM_ERR_UNSUPPORTED;
      break;
    }

   return SBM_OK;
 }


int SBM_Image::InvertCol()
 {
  unsigned long Loop;

   switch(BPP)
    {
     case 8:
       if(PalData==NULL) // Grayscale image
        {
          for(Loop=0;Loop<ImageSize;++Loop)
           {
            ImgData[Loop]=255-ImgData[Loop];
           }
        }
       else
        return SBM_ERR_UNSUPPORTED;

      break;

     default:
      return SBM_ERR_UNSUPPORTED;
      break;
    }

  return SBM_OK;
 }

int SBM_Image::Saturate(unsigned char KeyR, unsigned char KeyG, unsigned char KeyB,
                        unsigned char SatR, unsigned char SatG, unsigned char SatB)
 {
  long Loop,PixCount;

  PixCount=(Width*Height);

   switch(BPP)
    {
     case 24:
      PixCount*=3;
       for(Loop=0;Loop<PixCount;Loop+=3)
        {
          if(ImgData[Loop]!=KeyR || ImgData[Loop+1]!=KeyG || ImgData[Loop+2]!=KeyB)
           {
            ImgData[Loop]=SatR;
            ImgData[Loop+1]=SatG;
            ImgData[Loop+2]=SatB;
           } 
        }
       break;

     default:
      return SBM_ERR_UNSUPPORTED;
      break;
    }

  return SBM_OK;
 }

int SBM_Image::GetBPP()
 {
  return BPP;
 }


int SBM_Image::GetWidth()
 {
  return Width;
 }


int SBM_Image::GetHeight()
 {
  return Height;
 }

unsigned char* SBM_Image::GetImg()
 {
  return ImgData;
 }

unsigned char* SBM_Image::GetPalette()
 {
  return PalData;
 }
