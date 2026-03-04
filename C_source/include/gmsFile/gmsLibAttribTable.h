//----------------------------------------------------------------------------*/
// File : gmsLibAttribTable.h
// Date : 07-Jul-99 : Initial Definition
//        01-Aug-99 : Convert to utilites on port to Sun Solaris
//        05-Oct-99 : Clean-up due to code inspection
//
// Description:
//    Utilities to "read" the "Library Attribute Table" file of the Digital
//    Chart of the World (DCW).  The "Library Attribute Table describes the
//    properties of each library in the database.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#ifndef LAT

   #define LAT

   typedef enum { NOAMER   = 0,
                  EURNASIA = 1,
                  SOAMAFR  = 2,
                  SASAUS   = 3,
                  BROWSE   = 4} CoverageType;

   typedef struct _LibAttribRecordType
      {
       int      ID;
       char     libraryName[8 + 1];
       float    westMostLong;
       float    southMostLat;
       float    eastMostLong;
       float    northMostLat;
      } LibAttribRecordType;

   typedef LibAttribRecordType LibAttribTableType[5];


   //---------------------------------------------*/
   // gmsGetLibAttribTable
   //
   // Description:
   //    This utility reads the file that contains
   //    the "Library Attribute Table".  A pointer
   //    to a newly allocated table is returned to
   //    the caller.  It is the caller's responsibility
   //    to free the item by using the utility
   //    "gmsFreeLibAttribTable" (see below).
   //---------------------------------------------*/
   LibAttribTableType *gmsGetLibAttribTable
                                  (const char *filePath);


   //---------------------------------------------*/
   // gmsFreeLibAttribTable
   //
   // Description:
   //    This utility frees a "Library Attribute
   //    Table" that had been previously allocated
   //    using "gmsGetLibAttribTable".
   //---------------------------------------------*/
   void gmsFreeLibAttribTable
           (LibAttribTableType *theLAT);


   //---------------------------------------------*/
   // gmsPrintLibAttribTable
   //
   // Description:
   //    This function will print the "lat" object.
   //---------------------------------------------*/
   void gmsPrintLibAttribTable
           (LibAttribTableType *theLAT);

#endif
