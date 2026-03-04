//----------------------------------------------------------------------------*/
// File : gmsThematicIndex.h
// Date : 02-Sep-99 : Initial Definition
//        07-Oct-99 : Clean-up due to code-inspection
//
// Description:
//    Utilities to "read" a "thematic index" file from the Digital Chart of
//    the World (DCW) database.  Thematic Index files are implemented for
//    feature tables in tiled databases.  Consider the following definitions:
//
//       Theme:
//       ------
//          An organizational concept used in the
//          design of spatial databases.  Common
//          themes are transportation, hydrology,
//          and soil/land suitability.
//
//       Thematic Index :
//       ----------------
//          A file that allows software to access
//          the row ids of its associated table.
//          In a VPF table, the index is created
//          on a column.  Four special indexes are
//          used for feature tables: point, line,
//          area, and text.
//
//       Thematic Attribute:
//       -------------------
//          A column in a table that provides a
//          thematic description of a feature.  For
//          example, a feature class that contains
//          rivers may have attributes such as width
//          depth, and name.
//
//    As mentioned, the following kinds of tables:
//
//                area features  - ATI
//                line features  - LTI
//                point features - PTI
//                text features  - TTI
//
//    contain variable length records.  These files have an associated
//    "thematic index file" ("*TI").
//
//    A "thematic index file" is a sequential file of data partitioned into
//    three groups:
//
//         1) a fixed-lenght header of 60 bytes that specifies
//            the associated feature table and the column
//            within that table being indexed.
//
//         2) A "directory" consisting of a repeating number
//            of records that identifies the location of the
//            rows with the following data list (see below).
//
//         3) A list of data records.  Each record consists of
//            row-IDs from the table that is being indexed.
//
//    Reference:
//        1) Mil-Std-600006
//        2) Mil-D-89009 (pg 37-38)
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#ifndef THEMATIC

   #define THEMATIC

   typedef struct _thematicHeaderType
      {
       int  numBytesThisHdr;
       int  numOfDirEntries;
       int  numOfRows;
       char kindOfFile;
       char kindOfDataElement;
       int  numDataItemsPerDir;
       char typeSpecifier;
       char nameOfVpfTable[12 + 1];
       char columnName[25 + 1];
       char notUsed[4];
      } thematicHeaderType;

   //------------------------------------*/
   // Note:
   //   Size of above record is 64 bytes
   //   due to padding.  However, the num
   //   of bytes in the file is just 60.
   //------------------------------------*/
   #define Num_Bytes_In_Thematic_Header 60

   typedef struct _byteThematicRecord
      {
       unsigned char whichRow;
       long          fileOffsetToRow;
       int           numColumnsInRow;
      } byteThematicRecord;

   typedef struct _shortIntThematicRecord
      {
       short whichRow;
       long  fileOffsetToRow;
       int   numColumnsInRow;
      } shortIntThematicRecord;

   typedef struct _wordThematicRecord
      {
       int  whichRow;
       long fileOffsetToRow;
       int  numColumnsInRow;
      } wordThematicRecord;

   typedef struct _doubleWordThematicRecord
      {
       char whichRow;
       long fileOffsetToRow;
       int  numColumnsInRow;
      } charThematicRecord;

   typedef struct _thematicIndexType
      {
       thematicHeaderType header;
       void               *buffer;
       void               *recordIds;
      } thematicIndexType;

   //---------------------------------------------*/
   // gmsGetThematicIndex
   //
   // Description:
   //    This utility reads the file that contains
   //    "Thematic Index" data.  A pointer to a
   //    newly allocated table is returned to the caller.
   //    It is the caller's responsibility to free
   //    the item by using the utility
   //    "gmsFreeThematicIndex" (see below).
   //---------------------------------------------*/
   thematicIndexType *gmsGetThematicIndex
                             (const char *filePath);


   //---------------------------------------------*/
   // gmsFreeThematicIndex
   //
   // Description:
   //    This utility frees a "Thematic Index"
   //    object that had been previously allocated
   //    using "gmsGetThematicIndex".
   //---------------------------------------------*/
   void gmsFreeThematicIndex
           (thematicIndexType *theTI);


   //---------------------------------------------*/
   // gmsPrintThematicIndex
   //
   // Description:
   //    This function will print the contents of
   //    the specified "Thematic Index" object.
   //    to stdout.
   //---------------------------------------------*/
   void gmsPrintThematicIndex
           (thematicIndexType *theTI);

#endif
