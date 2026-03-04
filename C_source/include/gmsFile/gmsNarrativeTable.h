//----------------------------------------------------------------------------*/
// File : gmsNarrativeTable.h
// Date : 02-Sep-99 : Initial Definition
//        05-Oct-99 : Clean-up due to code-inspection
//
// Description:
//    Utilities to "read" a "narrative" file from the Digital Chart of
//    the World (DCW) database.  The format of a narrative table is:
//
//           ID  =I,1 ,P,Feature Table Primary Key,-,-,:
//           TEXT=T,80,N,Text Information,-,-,:;
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#ifndef DOC

   #define DOC

   typedef struct _narrativeTableType
      {
       int  numChars;
       char *theDoc;
      } narrativeTableType;

   //---------------------------------------------*/
   // gmsGetNarrativeTable
   //
   // Description:
   //    This utility reads the file that contains
   //    "Narrative" data.  A pointer to a newly
   //    allocated table is returned to the caller.
   //    It is the caller's responsibility to free
   //    the item by using the utility
   //    "gmsFreeNarrativeTable" (see below).
   //---------------------------------------------*/
   narrativeTableType *gmsGetNarrativeTable
                             (const char *filePath);


   //---------------------------------------------*/
   // gmsFreeNarrativeTable
   //
   // Description:
   //    This utility frees a "Narrative" table that
   //    had been previously allocated using
   //    "gmsGetNarrativeTable".
   //---------------------------------------------*/
   void gmsFreeNarrativeTable
           (narrativeTableType *theDOC);


   //---------------------------------------------*/
   // gmsPrintNarrativeTable
   //
   // Description:
   //    This function will print the contents of
   //    the specified "Narrative" table to stdout.
   //---------------------------------------------*/
   void gmsPrintNarrativeTable
           (narrativeTableType *theDOC);

#endif
