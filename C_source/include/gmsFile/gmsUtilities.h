//----------------------------------------------------------------------------*/
// File : gmsUtilities.h
// Date : 14-Jun-99 : Initial definition
//        04-Jul-99 : Pass device context (hDC) to text routine
//        26-Jul-99 : Gutted.  Made into a real utilities package.
//        25-Sep-99 : Convert x to Long and y to Lat for readability
//        07-Oct-99 : Clean-up due to code-inspection
//        17-Nov-99 : Added utils to access root dir (et.al.) of VPF files
//        21-Dec-99 : Added util to check for existence of a file
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

#ifndef GMS_UTILITIES

   #define GMS_UTILITIES

   #ifdef NOT_CPP
      #define bool  int
      #define true  1
      #define false 0
   #endif

   #include <stdio.h>

   typedef struct _twoDimCoordType
      {
       float Long;
       float Lat;
      } twoDimCoordType;

   typedef struct _twoDimDoubleCoordType
      {
       double Long;
       double Lat;
      } twoDimDoubleCoordType;

   typedef struct _threeDimCoordType
      {
       float Long;
       float Lat;
       float Alt;
      } threeDimCoordType;

   typedef struct _threeDimDoubleCoordType
      {
       double Long;
       double Lat;
       double Alt;
      } threeDimDoubleCoordType;

   typedef struct _idTripletType
      {
       int id;
       int tileId;
       int externalId;
      } idTripletType;


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
            const int  numBytes);


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
                     const int     numBytes);


   //---------------------------------------------*/
   // gmsFreeSwappedBits
   //
   // Description:
   //    This utility frees the block of data that
   //    was allocated by the utility "gmsSwapBitPattern".
   //---------------------------------------------*/
   void gmsFreeSwappedBits ();


   //---------------------------------------------*/
   // gmsSwapBytes
   //
   // Description:
   //    This utility swaps the bytes of the specified
   //    object.  The caller must pass a buffer whose
   //    size is an even number of bytes.
   //---------------------------------------------*/
   void gmsSwapBytes
           (unsigned char *ptrToBlock,
            const int     numBytes);


   //---------------------------------------------*/
   // gmsReadShortInteger
   //
   // Description:
   //    This utility reads a short integer from
   //    the specified file.
   //---------------------------------------------*/
   short gmsReadShortInteger (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadInteger
   //
   // Description:
   //    This utility reads an integer from the
   //    specified file.
   //---------------------------------------------*/
   int gmsReadInteger (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadLongInteger
   //
   // Description:
   //    This utility reads a LONG integer from the
   //    specified file.
   //---------------------------------------------*/
   long gmsReadLongInteger (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadFloat 
   //
   // Description:
   //    This utility reads a float from the
   //    specified file.
   //---------------------------------------------*/
   float gmsReadFloat (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadDouble
   //
   // Description:
   //    This utility reads a double from the
   //    specified file.
   //---------------------------------------------*/
   double gmsReadDouble (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadTwoDimCoord
   //
   // Description:
   //    This utility reads a "twoDimCoordType" from
   //    the specified file.
   //---------------------------------------------*/
   twoDimCoordType gmsReadTwoDimCoord (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadTwoDimDoubleCoord
   //
   // Description:
   //    This utility reads a "twoDimDoubleCoordType"
   //    from the specified file.
   //---------------------------------------------*/
   twoDimDoubleCoordType gmsReadTwoDimDoubleCoord (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadThreeDimCoord
   //
   // Description:
   //    This utility reads a "threeDimCoordType" from
   //    the specified file.
   //---------------------------------------------*/
   threeDimCoordType gmsReadThreeDimCoord (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadThreeDimDoubleCoord
   //
   // Description:
   //    This utility reads a "threeDimDoubleCoordType"
   //    from the specified file.
   //---------------------------------------------*/
   threeDimDoubleCoordType gmsReadThreeDimDoubleCoord (FILE *theFd);


   //---------------------------------------------*/
   // gmsReadIdTripletRecord
   //
   // Description:
   //    This utility reads a "idTripletType" from
   //    the specified file.
   //---------------------------------------------*/
   idTripletType gmsReadIdTripletRecord (FILE *theFd);


   //---------------------------------------------*/
   // gmsDirectoryExists
   //
   // Description:
   //    This utility determines if the specified
   //    directory exists.
   //---------------------------------------------*/
   bool gmsDirectoryExists
           (const char *dirName);


   //---------------------------------------------*/
   // gmsFileExists
   //
   // Description:
   //    This utility determines if the specified
   //    file exists.
   //---------------------------------------------*/
   bool gmsFileExists
           (const char *fileName);


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
   //
   //    The following protocol is used to determine
   //    the path to the root of the DCW database:
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
   //             (a) /tmp/dcw_path.txt
   //
   //             (b) /temp/dcw_path.txt
   //
   //             (c) /MyFiles/dcw_path.txt
   //
   //             (d) /My Documents/dcw_path.txt
   //
   //       5) If in a UNIX environment, the
   //          "HOME" env is checked.  The
   //          path will be set to "$HOME/DCW".
   //
   //       6) Finally, the default path is set
   //          to "/DCW".
   //---------------------------------------------*/
   char *gmsGetDcwFullPath
                  (const char *whichFile);


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
   //
   //    See 'gmsGetDcwFullPath' for description
   //    of how the path to the DCW database is
   //    determined.
   //---------------------------------------------*/
   char *gmsGetBrowseFullPath
                  (const char *whichFile);


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
   //
   //    See 'gmsGetDcwFullPath' for description
   //    of how the path to the DCW database is
   //    determined.
   //---------------------------------------------*/
   char *getDhtFile ();


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
   //
   //    See 'gmsGetDcwFullPath' for description
   //    of how the path to the DCW database is
   //    determined.
   //---------------------------------------------*/
   char *getLatFile ();

#endif
