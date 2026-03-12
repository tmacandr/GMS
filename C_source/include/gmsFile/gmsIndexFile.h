//----------------------------------------------------------------------------*/
// File : gmsIndexFile.h
// Date : 13-Jul-99 : Initial Definition
//        01-Aug-99 : Convert to utilities on port to Sun Solaris
//
// Description:
//    Utilities to "read" an "index" file from the Digital Chart of the
//    World (DCW) database.  Index files are implemented for tables with
//    variable length records, primitive tables, and feature tables in
//    tiled databases that require query response capabilities.  In the
//    DCW, tables that have an associated index are all those with variable-
//    length records.
//
//    These utilities are used for indexing "variable-length" fields.
//    These utilities can be applied to tables of:
//
//                 face
//                 edge
//                 node
//                 text
//
//    primitives.
//
//    For "spatial-indexing" of tables, refer to the utilities defined in
//    "gmsSpatialIndex.h".  For "thematic-indexing" of "feature-tables"
//    (i.e. area, line, point, and text feature tables) refer to the
//    utilities defined in: "gmsThematicIndex.h".
//
//    Consider the following definitions:
//
//       Variable-length Index Files:
//       ----------------------------
//          The contents of such files contain starting
//          positions (in bytes) of records in support
//          of "Text" files and "Coordinate" files.  The
//          data is an array that specifies the byte-offset
//          from the beginning of the file to the start
//          of each record.
//
//       Index:
//       ------
//          A mechanism used to quickly identify a particular
//          record or group of records based on a tables primary
//          key.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#ifndef INDEX

   #define INDEX

   typedef struct _indexRecordType
   {
       long byteOffset;
       int  numBytes;
   } indexRecordType;

   typedef struct _indexTableType
      {
       int             numberRecordsInTable;
       int             bytesInTableHeader;
       indexRecordType *indexList;
      } indexTableType;


   //---------------------------------------------*/
   // gmsGetIndexFile
   //
   // Description:
   //    This utility reads the file that contains
   //    the "Index File".  A pointer to a newly
   //    allocated table is returned to the caller.
   //    It is the caller's responsibility to free
   //    the item by using the utility
   //    "gmsFreeIndexFile" (see below).
   //---------------------------------------------*/
   indexTableType *gmsGetIndexFile
                             (const char *filePath);


   //---------------------------------------------*/
   // gmsFreeIndexFile
   //
   // Description:
   //    This utility frees a "Index File" that had
   //    been previously allocated using
   //    "gmsGetIndexFile".
   //---------------------------------------------*/
   void gmsFreeIndexFile
           (indexTableType *theIndexTable);


   //---------------------------------------------*/
   // gmsPrintIndexRecord	
   //
   // Description:
   //    This function will print "theIndexes"
   //    record (to stdout).
   //---------------------------------------------*/
   void gmsPrintIndexRecord
           (indexTableType *theIndexTable);

#endif
