//----------------------------------------------------------------------------*/
// File : gmsUtilities.cpp
// Date : 14-Jun-99 : Initial defintion
//        01-Aug-99 : Ported to Sun Solaris
//        06-Aug-99 : Ported back to VC++
//        25-Sep-99 : Convert x to Long and y to Lat for readability
//        07-Oct-99 : Clean-up due to code-inspection
//        17-Nov-99 : Added utils to access root dir (et.al.) of VPF files
//        18-Nov-99 : Use capital letters for names of VPF files
//        21-Dec-99 : Added util to check for existence of a file
//        22-Dec-99 : Added mechanism to specify where DCW files are
//        06-Jan-00 : Added 'ctype.h' to access 'toupper' util
//        11-Feb-00 : HP and SUN C++ compilers complain about init of char *
//        21-Feb-00 : 'gmsReadxxx' utils fail on SUN.  Memory mis-align
//        03-Mar-00 : look for dcw_path.txt in current working dir too
//        10-Mar-26 : DCW/VPF only has signed short, int, float, double
//
// Description:
//    Utilities that support the "Geographic Map System" (GMS) being
//    developed for MS Project at CSUN.
//
//    See also : Mil-D-89009, pg 11, Table 2
//               Mil-Std-600006, pg 45, Table 11
//               Mil-Std-600006, pg 79, Table 56
//               Mil-Std-600006, pg 80, section 5.4.6
//
//    Reference:
//        1) Mil-Std-600006
//        2) Mil-D-89009
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <gmsUtilities.h>

#ifndef IS_UNIX
   #include <windows.h>
#endif


//--------------------------*/
// Declare Local Constants
//--------------------------*/
#define emptyBuffer (unsigned char *) NULL

#define bytesPerShort sizeof(short)

#define bytesPerInt   sizeof(int)

#define bytesPerLong  sizeof(long)

#define bytesPerFlt   sizeof(float)

#define bytesPerDbl   sizeof(double)

static char *VPF_File_Root = NULL;

static const int  bitsPerByte = 8;

typedef enum { idMask_empty            = 0x00,
               idMask_oneByte          = 0x40,
               idMask_twoByte          = 0x80,
               idMask_fourByte         = 0xC0,
               tileIdMask_empty        = 0x00,
               tileIdMask_oneByte      = 0x10,
               tileIdMask_twoByte      = 0x20,
               tileIdMask_fourByte     = 0x30,
               externalIdMask_empty    = 0x00,
               externalIdMask_oneByte  = 0x04,
               externalIdMask_twoByte  = 0x08,
               externalIdMask_fourByte = 0x0C } tripletIdMaskType;

//--------------------------*/
// Declare Local Variables
//--------------------------*/
static unsigned char *swappedBitsBuffer = emptyBuffer;


//--------------------------*/
// Declare Local Functions
//--------------------------*/
static unsigned char swapBitsOfByte
                        (const unsigned char theByte);

static void convertStringToUpperCase
                        (char *string);

static char *gmsGetDcwLibrary
                        (const char *root);

static char *buildVpfFileRoot(void);



//---------------------------------------------*/
// gmsClearMemory
//
// Description:
//    Utility to zero out the memory of a
//    specified data structure.  This is
//    essentially the same utility as "ZeroMemory"
//    from the Windows API.  It's very useful.
//    However, this utility re-defines it just
//    in case this code is ever ported to
//    another platform that does not have
//    "ZeroMemory".
//---------------------------------------------*/
void gmsClearMemory
        (char       *ptrToBlock,
         const int  numBytes)

{
         int i;

   if (ptrToBlock == NULL)
      return;

   for (i = 0; i < numBytes; i++)
      ptrToBlock[i] = (char) 0;
}


//---------------------------------------------*/
// gmsSwapBitPattern
//
// Description:
//    Utility to swap the bits of a given data-
//    item.  The low bits become the high bits,
//    the high bits become the low bits.  The
//    caller passes in a pointer to a block of
//    data and the size (in bytes) of that block.
//    This utility then returns a pointer to
//    a newly allocated block whose bit pattern
//    is the opposite of the specified block.
//
//    The caller must "free" the block by
//    calling "gmsFreeSwappedBits" when finished.
//---------------------------------------------*/
unsigned char *gmsSwapBitPattern
                 (unsigned char *ptrToBlock,
                  const int     numBytes)

{
         int byteIndex;

   // Do some defensive programming */
   if ( (ptrToBlock == NULL) || (swappedBitsBuffer != emptyBuffer) )
      {
       printf("---> ERROR : previous bit buffer not freed\n");

       return emptyBuffer;
      }

   if (numBytes == 0)
      return emptyBuffer;

   if (ptrToBlock == emptyBuffer)
      return emptyBuffer;

   // Allocate the corresponding buffer */
   swappedBitsBuffer = (unsigned char *) malloc (numBytes);

   gmsClearMemory
      ( (char *) swappedBitsBuffer,
        numBytes);

   // Do the work ... */
   for (byteIndex = 0; byteIndex < numBytes; byteIndex++)
      {
       swappedBitsBuffer[byteIndex] =
             ptrToBlock[(numBytes - 1) - byteIndex];

       swappedBitsBuffer[byteIndex] =
             swapBitsOfByte (swappedBitsBuffer[byteIndex]);
      }

   // Give the answer */
   return swappedBitsBuffer;
}


//---------------------------------------------*/
// gmsFreeSwappedBits
//
// Description:
//    This utility frees the block of data that
//    was allocated by the utility "gmsSwapBitPattern".
//---------------------------------------------*/
void gmsFreeSwappedBits (void)

{
   if (swappedBitsBuffer != emptyBuffer)
      {
       free (swappedBitsBuffer);

       swappedBitsBuffer = emptyBuffer;
      }
}


//---------------------------------------------*/
// gmsSwapBytes
//
// Description:
//    This utility swaps the bytes of the specified
//    object.
//---------------------------------------------*/
void gmsSwapBytes
        (unsigned char *ptrToBlock,
         const int     numBytes)

{
         int           isEvenNumber;
         unsigned char tempByte;
         int           i;

   if (ptrToBlock == NULL)
      return;

   isEvenNumber = numBytes % 2;

   if ( ! (isEvenNumber == 0) )
   {
       printf("*** ERROR : gmsSwapBytes - odd number of bytes specified\n");

       return;
   }

   for (i = 0; i < (numBytes / 2); i++)
   {
       tempByte = ptrToBlock[i];

       ptrToBlock[i] = ptrToBlock[(numBytes - 1) - i];

       ptrToBlock[(numBytes - 1) - i] = tempByte;
   }
}


//---------------------------------------------*/
// gmsReadShortInteger
//
// Description:
//    This utility reads a short integer from
//    the specified file.
//---------------------------------------------*/
short gmsReadShortInteger (FILE *theFd)

{
         int         i;
         short       theAnswer;
         char        *shortAsChars = (char *) &theAnswer;

   for (i = 0; i < bytesPerShort; i++)
      shortAsChars[i] = (char) fgetc(theFd);

   #ifdef GMS_BIG_ENDIAN

      gmsSwapBytes
         ((unsigned char *) shortAsChars,
          bytesPerShort);

   #endif

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadInteger
//
// Description:
//    This utility reads an integer from the
//    specified file.
//---------------------------------------------*/
int gmsReadInteger (FILE *theFd)

{
         int         i;
         int         theAnswer;
         char        *intAsChars = (char *) &theAnswer;

   for (i = 0; i < bytesPerInt; i++)
      intAsChars[i] = (char) fgetc(theFd);

   #ifdef GMS_BIG_ENDIAN 

      gmsSwapBytes
         ((unsigned char *) intAsChars,
          bytesPerInt);

   #endif

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadFloat 
//
// Description:
//    This utility reads a float from the
//    specified file.
//---------------------------------------------*/
float gmsReadFloat (FILE *theFd)

{
         int         i;
         float       theAnswer;
         char        *floatAsChars = (char *) &theAnswer;

   for (i = 0; i < bytesPerFlt; i++)
      floatAsChars[i] = (char) fgetc(theFd);

   #ifdef GMS_BIG_ENDIAN 

      gmsSwapBytes
         ((unsigned char *) floatAsChars,
          bytesPerFlt);

   #endif

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadDouble
//
// Description:
//    This utility reads a double from the
//    specified file.
//---------------------------------------------*/
double gmsReadDouble (FILE *theFd)

{
         int         i;
         double      theAnswer;
         char        *doubleAsChars = (char *) &theAnswer;

   for (i = 0; i < bytesPerDbl; i++)
      doubleAsChars[i] = (char) fgetc(theFd);

   #ifdef GMS_BIG_ENDIAN

      gmsSwapBytes
         ((unsigned char *) doubleAsChars,
          bytesPerDbl);

   #endif

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadTwoDimCoord
//
// Description:
//    This utility reads a "twoDimCoordType" from
//    the specified file.
//---------------------------------------------*/
twoDimCoordType gmsReadTwoDimCoord (FILE *theFd)

{
         twoDimCoordType theAnswer;

   theAnswer.Long = gmsReadFloat (theFd);

   theAnswer.Lat = gmsReadFloat (theFd);

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadTwoDimDoubleCoord
//
// Description:
//    This utility reads a "twoDimDoubleCoordType"
//    from the specified file.
//---------------------------------------------*/
twoDimDoubleCoordType gmsReadTwoDimDoubleCoord (FILE *theFd)

{
         twoDimDoubleCoordType theAnswer;

   theAnswer.Long = gmsReadDouble (theFd);

   theAnswer.Lat = gmsReadDouble (theFd);

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadThreeDimCoord
//
// Description:
//    This utility reads a "threeDimCoordType" from
//    the specified file.
//---------------------------------------------*/
threeDimCoordType gmsReadThreeDimCoord (FILE *theFd)

{
         threeDimCoordType theAnswer;

   theAnswer.Long = gmsReadFloat (theFd);

   theAnswer.Lat = gmsReadFloat (theFd);

   theAnswer.Alt = gmsReadFloat (theFd);

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadThreeDimDoubleCoord
//
// Description:
//    This utility reads a "threeDimDoubleCoordType"
//    from the specified file.
//---------------------------------------------*/
threeDimDoubleCoordType gmsReadThreeDimDoubleCoord (FILE *theFd)

{
         threeDimDoubleCoordType theAnswer;

   theAnswer.Long = gmsReadDouble (theFd);

   theAnswer.Lat = gmsReadDouble (theFd);

   theAnswer.Alt = gmsReadDouble (theFd);

   return theAnswer;
}


//---------------------------------------------*/
// gmsReadIdTripletRecord
//
// Description:
//    This utility reads a "idTripletType" from
//    the specified file.
//
//    A triplet ID record is used as a reference
//    mechanism for "tiled coverage".  The "triplet
//    ID" record is composed of a byte called the
//    "type byte".  This byte is broken down in to
//    four 2-bit fields.  Each 2-bit field describes
//    the length of succeeding fields:
//
//          2-bit pattern       meaning
//          ------------------------------
//            00          --> empty field
//            01          --> 8-bit field
//            10          --> 16-bit field
//            11          --> 32-bit field
//
//    The first 2-bit field (referred to as ID)
//    indicates the size of field that represents
//    a primitive ID without a tile-id predicate.
//
//    The second 2-bit field (referred to as
//    TILE-ID) indicates the size of the field
//    that represents the TILE-REFERENCE-ID
//
//    The third 2-bit field (referred to as
//    EXT-ID) indicates the size of the field
//    that represents the "EXTERNAL-PRIMITIVE-ID".
//
//    The fourth 2-bit field is currently not
//    used and is reserved for future growth.
//
//    See also:
//    ---------
//       Mil-Std-600006, pg 79-80
//---------------------------------------------*/
idTripletType gmsReadIdTripletRecord (FILE *theFd)

{
      unsigned char descriptorByte;
      idTripletType theAnswer;

   gmsClearMemory
      ( (char *) &theAnswer,
        sizeof(theAnswer) );

   descriptorByte = (unsigned char) fgetc (theFd);

   //-------------*/
   // ID
   //-------------*/
   if (descriptorByte & idMask_oneByte)
      {
       theAnswer.id = fgetc (theFd);
      }
   else if (descriptorByte & idMask_twoByte)
      {
       theAnswer.id = (int) gmsReadShortInteger (theFd); 
      }
   else if (descriptorByte & idMask_fourByte)
      {
       theAnswer.id = gmsReadInteger (theFd);
      }

   //-------------*/
   // Tile-ID
   //-------------*/
   if (descriptorByte & tileIdMask_oneByte)
      {
       theAnswer.tileId = fgetc (theFd);
      }
   else if (descriptorByte & tileIdMask_twoByte)
      {
       theAnswer.tileId = (int) gmsReadShortInteger (theFd);
      }
   else if (descriptorByte & tileIdMask_fourByte)
      {
       theAnswer.tileId = gmsReadInteger (theFd);
      }

   //-------------*/
   // External-ID
   //-------------*/
   if (descriptorByte & externalIdMask_oneByte)
      {
       theAnswer.externalId = fgetc (theFd);
      }
   else if (descriptorByte & externalIdMask_twoByte)
      {
       theAnswer.externalId = (int) gmsReadShortInteger (theFd);
      }
   else if (descriptorByte & externalIdMask_fourByte)
      {
       theAnswer.externalId = gmsReadInteger (theFd);
      }

   return theAnswer;
}


//---------------------------------------------*/
// gmsDirectoryExists
//
// Description:
//    This utility determines if the specified
//    directory exists.
//---------------------------------------------*/
bool gmsDirectoryExists
           (const char *dirName)

{
   #ifdef IS_UNIX

      return gmsFileExists(dirName);

   #else

            BOOL   wasSuccessful;
            char   currentDir[128];
            DWORD  status;

      status = GetCurrentDirectoryA
                  (sizeof (currentDir),
                   currentDir);

      wasSuccessful = SetCurrentDirectoryA (dirName);

      if (wasSuccessful)
         {
          wasSuccessful = SetCurrentDirectoryA (currentDir);

          return true;
         }

      return false;

   #endif
}


//---------------------------------------------*/
// gmsFileExists
//
// Description:
//    This utility determines if the specified
//    file exists.
//---------------------------------------------*/
bool gmsFileExists
           (const char *fileName)

{
            FILE *fd;

   fd = fopen (fileName, "r");

   if (fd == NULL)
      return false;

   fclose (fd);

   return true;
}


//---------------------------------------------*/
// gmsGetDcwFullPath
//
// Description:
//    This utility returns a string that is the
//    path to the root of the "REGIONAL" library
//    (i.e. NOAMER, SOAMAFR, EURNASIA, or SASAUS)
//    of the VPF database.
//
//    NOTE:
//       The caller must NOT free the string
//       returned.
//---------------------------------------------*/
char *gmsGetDcwFullPath
                  (const char *whichFile)

{
      static char *whichLibrary = NULL;
      static char tempPath[512];
      static char fullPath[1024];

   if (VPF_File_Root == NULL)
      {
       VPF_File_Root = buildVpfFileRoot();
      }

   if (whichLibrary == NULL)
      {
       whichLibrary = gmsGetDcwLibrary(VPF_File_Root);
      }

   sprintf(tempPath, "%s", whichFile);

   #ifdef IS_UNIX

      convertStringToUpperCase (tempPath);

   #endif

   sprintf(fullPath, "%s/%s/%s",
           VPF_File_Root,
           whichLibrary,
           tempPath);

   return fullPath;
}


//---------------------------------------------*/
// gmsGetBrowseFullPath
//
// Description:
//    This utility returns a string that is the
//    path to the root of the "BROWSE" files
//    of the VPF database.
//
//    NOTE:
//       The caller must NOT free the string
//       returned.
//---------------------------------------------*/
char *gmsGetBrowseFullPath
                  (const char *whichFile)

{
      static char tempPath[512];
      static char fullPath[1024];

   sprintf(tempPath, "%s", whichFile);

   #ifdef IS_UNIX

      convertStringToUpperCase (tempPath);

   #endif

   if (VPF_File_Root == NULL)
      {
       VPF_File_Root = buildVpfFileRoot();
      }

   sprintf(fullPath,
           "%s/BROWSE/%s", VPF_File_Root, tempPath);

   return fullPath;
}


//---------------------------------------------*/
// getDhtFile
//
// Description:
//    This utility returns a string that is the
//    full path to the "Database Header Table"
//    of the VPF database.  There is only one
//    DHT file per CD-ROM and is located at the
//    root of the database.
//
//    NOTE:
//       The caller must NOT free the string
//       returned.
//---------------------------------------------*/
char *getDhtFile (void)

{
         static char DhtFile[256];
         static char *theAnswer = NULL;

   if (theAnswer == NULL)
      {
       if (VPF_File_Root == NULL)
          {
           VPF_File_Root = buildVpfFileRoot();
          }

       sprintf(DhtFile, "%s/DHT", VPF_File_Root);

       theAnswer = DhtFile;
      }

   return theAnswer;
}


//---------------------------------------------*/
// getLatFile
//
// Description:
//    This utility returns a string that is the
//    full path to the "Library Attribute Table"
//    of the VPF database.  There is only one
//    LAT file per CD-ROM and is located at the
//    root of the database.
//
//    NOTE:
//       The caller must NOT free the string
//       returned.
//---------------------------------------------*/
char *getLatFile (void)

{
         static char LatFile[256];
         static char *theAnswer = NULL;

   if (theAnswer == NULL)
      {
       if (VPF_File_Root == NULL)
          {
           VPF_File_Root = buildVpfFileRoot();
          }

       sprintf(LatFile, "%s/LAT", VPF_File_Root);

       theAnswer = LatFile;
      }

   return theAnswer;
}




        //--------------------------*/
        //     Local Functions
        //--------------------------*/




//-------------------------------------------------*/
// swapBitsOfByte
//
// Description:
//    This utility swaps the bit pattern of the
//    specified byte.  The most-significant bit
//    becomes the least significant bit, etc.
//-------------------------------------------------*/
static unsigned char swapBitsOfByte
                        (const unsigned char theByte)

{
         unsigned char testPattern     = 0x80; // bit pattern = 1000_0000 */
         unsigned char oppositePattern = 0x01; // bit pattern = 0000_0001 */
         unsigned char result          = 0x00;
         unsigned char isOne           = 0x00;
         int           bitIndex;

   for (bitIndex = 0; bitIndex < bitsPerByte; bitIndex++)
      {
       isOne = testPattern & theByte;

       if (isOne)
         {
          result = result | oppositePattern; // bitwise OR */
         }

       testPattern = testPattern >> 1;   // shift right */

       oppositePattern = oppositePattern << 1; // shift left */
      }

   return result;
}


//---------------------------------------------*/
// convertStringToUpperCase
//
// Description:
//    This utility converts the characters of
//    the specified string to upper case.  This
//    is done because names of the files and
//    directories on a VPF CD-ROM are capitalized.
//    On a "MS Windows" box, it is not a problem
//    if names of files are capitalized or not.
//    But on a UNIX box, this matters.  Hence,
//    this routine ensures file/directory
//    names are capitalized.
//---------------------------------------------*/
static void convertStringToUpperCase (char *string)

{
      int strLength;
      int i;
      int tempChar;

   strLength = strlen (string);

   for (i = 0; i < strLength; i++)
      {
       tempChar = toupper (string[i]);

       string[i] = (char) tempChar;
      }
}


//---------------------------------------------*/
// gmsGetDcwLibrary
//
// Description:
//    This utility is used by the routine
//    "gmsGetDcwFullPath".  It determines which
//    one of the four REGIONAL libraries is
//    present so the path to that directory can
//    be constructed and used by a caller.
//
//    Note: this routine is called once by
//          the function 'gmsGetDcwFullPath'
//          at startup.
//---------------------------------------------*/
static char *gmsGetDcwLibrary
               (const char *root)

{
         int         i;
         char        tempDirName[512];
         static char *dcwLibs[4] = { (char *) "NOAMER",
                                     (char *) "EURNASIA",
                                     (char *) "SOAMAFR",
                                     (char *) "SASAUS" };

   #ifdef IS_UNIX

            FILE *fd;

      for (i = 0; i < 4; i++)
         {
          sprintf(tempDirName, "%s/%s", root, dcwLibs[i]);

          fd = fopen (tempDirName, "r");

          if (fd != NULL)
             {
              fclose (fd);

              return dcwLibs[i];
             }
         }

   #else

            BOOL   wasSuccessful;
            char   currentDir[512];
            DWORD  status;

      status = GetCurrentDirectoryA
                  (sizeof (currentDir),
                   currentDir);

      for (i = 0; i < 4; i++)
         {
          sprintf(tempDirName, "%s/%s", root, dcwLibs[i]);

          wasSuccessful = SetCurrentDirectoryA (tempDirName);

          if (wasSuccessful)
             {
              wasSuccessful = SetCurrentDirectoryA (currentDir);

              return dcwLibs[i];
             }
         }

   #endif

   return dcwLibs[0];  // default */
}


//---------------------------------------------*/
// buildVpfFileRoot
//
// Description:
//    Utility that determines where the DCW
//    files are located on the current system.
//    The following protocol is used:
//
//       1) If in a UNIX environment, the
//          environmental variable "DCW_PATH"
//          is examined.  If it has been
//          defined, it specifies the path
//          to the DCW files.
//
//       2) The following files are checked
//          in this order.  If a file exists,
//          it specifies the path (in ASCII).
//
//             (a) ./dcw_path.txt
//
//             (b) /tmp/dcw_path.txt
//
//             (c) /temp/dcw_path.txt
//
//             (d) /MyFiles/dcw_path.txt
//
//             (e) /My Documents/dcw_path.txt
//
//       5) If in a UNIX environment, the
//          "HOME" env is checked.  The
//          path will be set to "$HOME/DCW".
//
//       6) Finally, the default path is set
//          to "/DCW".
//---------------------------------------------*/
static char *buildVpfFileRoot(void)

{
         static char thePath[256];
         #define     Num_Path_Files 5
         char        *pathFiles[Num_Path_Files] =
                        {
                          (char *) "./dcw_path.txt",
                          (char *) "/tmp/dcw_path.txt",
                          (char *) "/temp/dcw_path.txt",
                          (char *) "/MyFiles/dcw_path.txt",
                          (char *) "/My Documents/dcw_path.txt"
                        };
         bool        fileExists;
         int         i;
         FILE        *fd;

   #ifdef IS_UNIX

         char        *envPtr;

      envPtr = getenv("DCW_PATH");

      if (envPtr != NULL)
         {
          sprintf(thePath, "%s", envPtr);

          return thePath;
         }

   #endif

   for (i = 0; i < Num_Path_Files; i++)
      {
       #ifdef IS_UNIX

          sprintf(thePath, "%s", pathFiles[i]);

       #else

          sprintf(thePath, "C:%s", pathFiles[i]);

       #endif

       fileExists = gmsFileExists(thePath);

       if (fileExists)
          {
           fd = fopen(thePath, "r");

           if (fd != NULL)
              {
               fgets(thePath, sizeof(thePath), fd);

               fclose(fd);

               return thePath;
              }
          }
      }

   #ifdef IS_UNIX

      envPtr = getenv("HOME");

      if (envPtr != NULL)
         {
          sprintf(thePath, "%s", envPtr);

          return thePath;
         }

   #endif

   //-------------------*/
   // Sneak this one in */
   //-------------------*/
   fileExists = gmsFileExists("/thesis/DCW/DHT");

   if (fileExists)
      {
       sprintf(thePath, "/thesis/DCW");

       return thePath;
      }

   //-------------------*/
   // All checks failed.*/
   // Go 'default'.     */
   //-------------------*/
   sprintf(thePath, "/DCW");

   return thePath;
}
