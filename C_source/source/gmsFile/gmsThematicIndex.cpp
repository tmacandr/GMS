//----------------------------------------------------------------------------*/
// File : gmsThematicIndex.cpp
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

#include <gmsThematicIndex.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------*/
//     Local Variables       --*/
//-----------------------------*/
static FILE *theme_fd = (FILE *) NULL;

static int numChars = 0;


//-----------------------------*/
// Declare Local Subprograms   */
//-----------------------------*/
static void readThematicIndexFileForDebug ();

static void buildHeaderRecord
               (thematicHeaderType *theHdr);

static void buildThematicIndexObject
               (thematicIndexType *theTI);

static void buildThematicShortIntRecords
               (int                    numDirs,
                shortIntThematicRecord *recordBuffer,
                int                    numRows,
                short                  *bufferOfShorts);

static void printHeaderRecord
               (thematicHeaderType theHdr);

static void printShortIntThematicRecord
               (int                    numDirs,
                shortIntThematicRecord *recordBuffer,
                int                    numRows,
                short                  *bufferOfShorts);


//---------------------------------------------*/
// gmsGetThematicIndex
//
// Description:
//    This utility reads the file that contains
//    "Thematic Index" data.  A pointer to
//    a newly allocated table is returned to the
//    caller.  It is the caller's responsibility
//    to free the item by using the utility
//    "gmsFreeThematicIndex" (see below).
//---------------------------------------------*/
thematicIndexType *gmsGetThematicIndex
                             (const char *filePath)

{
         const char        readOnlyMode[] = { "rb" };
         thematicIndexType *theTI;

   if (filePath == NULL)
      return (thematicIndexType *) NULL;

   theme_fd = fopen(filePath,
                    readOnlyMode);

   if (theme_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open thematic file: %s\n", filePath);

       return (thematicIndexType *) NULL;
      }

   // readThematicIndexFileForDebug (); */

   theTI = (thematicIndexType *) malloc (sizeof(thematicIndexType));

   gmsClearMemory
      ( (char *) theTI,
       sizeof (thematicIndexType));

   buildThematicIndexObject (theTI);

   fclose (theme_fd);

   return theTI;
}


//---------------------------------------------*/
// gmsFreeThematicIndex
//
// Description:
//    This utility frees a "Thematic Index"
//    object that had been previously allocated
//    using "gmsGetThematicIndex".
//---------------------------------------------*/
void gmsFreeThematicIndex
           (thematicIndexType *theTI)

{
   if (theTI == NULL)
      return;

   if (theTI->buffer != NULL)
      free (theTI->buffer);

   if (theTI->recordIds != NULL)
      free (theTI->recordIds);

   free (theTI);
}


//---------------------------------------------*/
// gmsPrintThematicIndex
//
// Description:
//    This function will print the contents of
//    the specified "Thematic Index" object.
//    to stdout.
//---------------------------------------------*/
void gmsPrintThematicIndex
           (thematicIndexType *theTI)

{
   printf("______________________________\n");
   printf("Thematic Index Table:\n");

   if (theTI == NULL)
      return;

   printHeaderRecord (theTI->header);

   if (theTI->buffer == NULL)
      return;

   if (theTI->recordIds == NULL)
      return;

   if (theTI->header.kindOfDataElement == 'T')
      {
       printf("Print of CHAR thematic index data - NOT IMPLEMENTED\n");
      }
   else if (theTI->header.kindOfDataElement == 'S')
      {
       printShortIntThematicRecord
          (theTI->header.numOfDirEntries,
           (shortIntThematicRecord *) theTI->buffer,
           theTI->header.numOfRows,
           (short *) theTI->recordIds);
      }
   else if (theTI->header.kindOfDataElement == 'I')
      {
       printf("Print of INT thematic index data - NOT IMPLEMENTED\n");
      }
   else if (theTI->header.kindOfDataElement == 'F')
      {
       printf("Print of FLOAT thematic index data - NOT IMPLEMENTED\n");
      }
   else if (theTI->header.kindOfDataElement == 'R')
      {
       printf("Print of DOUBLE thematic index data - NOT IMPLEMENTED\n");
      }
   else
      {
       printf("*** ERROR : can't print unknown data from thematic index\n");
      }

   printf("______________________________\n");
}


     //-----------------------*/
     //   Local Subprograms
     //-----------------------*/


//---------------------------------------------*/
// readThematicIndexFileForDebug
//
// Description:
//    This routine is used for debug purposes
//    only.  This routine will read the contents
//    of the index file ... byte-by-byte ... and
//    print the contents as characters to stdout.
//    The file-descriptor is reset to the top
//    of the file.
//---------------------------------------------*/
static void readThematicIndexFileForDebug ()

{
         int numBytes  = 0;
         int lineCount = 0;
         int tempChar;

   rewind (theme_fd);

   printf("______________________________________\n");
   printf("Thematic Index File:\n");
   printf("   Header: (size = %d bytes)\n", Num_Bytes_In_Thematic_Header);
   printf("   File offset at start of hdr : %ld\n", ftell(theme_fd));

   for (numBytes = 0; numBytes < Num_Bytes_In_Thematic_Header; numBytes++)
      {
       tempChar = fgetc (theme_fd);

       printf("%.2x ", tempChar);

       if (lineCount == 3)
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;
      }

   printf("\n");
   printf("   End Header\n");
   printf("   File offset at END of hdr    : %ld\n", ftell(theme_fd));
   printf("----------------\n");
   printf("   Data:\n");
   printf("   File offset at START of data : %ld\n", ftell(theme_fd));

   numBytes = 0;

   lineCount = 0;

   tempChar = fgetc (theme_fd);

   while (tempChar != EOF)
      {
       numBytes++;

       printf("%.2x ", tempChar);

       if (lineCount == 3)
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;

       tempChar = fgetc (theme_fd);
      }

   printf("\n");
   printf("---> Num Bytes of data       = %d\n", numBytes);
   printf("---> Total Num bytes in file = %d \n",
          (numBytes + Num_Bytes_In_Thematic_Header) );
   printf("   File offset at END of data : %ld\n", ftell(theme_fd));
   printf("______________________________________\n");

   rewind (theme_fd);
}


//---------------------------------------------*/
// buildHeaderRecord
//
// Description:
//    This utility will construct the "thematic-
//    HeaderType" object from the contents of
//    the file.
//---------------------------------------------*/
static void buildHeaderRecord
               (thematicHeaderType *theHdr)

{
         int i;

   if (theHdr == NULL)
      {
       printf("*** ERROR : thematic index header is NULL\n");

       return;
      }

   theHdr->numBytesThisHdr = gmsReadInteger (theme_fd);

   theHdr->numOfDirEntries = gmsReadInteger (theme_fd);

   theHdr->numOfRows = gmsReadInteger (theme_fd);

   theHdr->kindOfFile = fgetc (theme_fd);

   theHdr->kindOfDataElement = fgetc (theme_fd);

   theHdr->numDataItemsPerDir = gmsReadInteger (theme_fd);

   theHdr->typeSpecifier = fgetc (theme_fd);

   fgets
      (theHdr->nameOfVpfTable,
       sizeof(theHdr->nameOfVpfTable),
       theme_fd);

   fgets
      (theHdr->columnName,
       sizeof(theHdr->columnName),
       theme_fd);

   for (i = 0; i < sizeof(theHdr->notUsed); i++)
      theHdr->notUsed[i] = fgetc(theme_fd);
}


//---------------------------------------------*/
// buildThematicIndexObject
//
// Description:
//    This function will read the actual data
//    from the Index file.  The data read will
//    be used to build the object (i.e. the
//    list of indexes).
//---------------------------------------------*/
static void buildThematicIndexObject
               (thematicIndexType *theTI)

{
         int  numBytes = 0;

   rewind (theme_fd);

   buildHeaderRecord (&theTI->header);

   if (theTI->header.kindOfDataElement == 'T')
   {
       printf("Build of CHAR thematic index tbl - NOT IMPLEMENTED\n");
   }
   else if (theTI->header.kindOfDataElement == 'S')
   {
       numBytes = sizeof(shortIntThematicRecord) *
                  theTI->header.numOfDirEntries;

       theTI->buffer = (void *) malloc (numBytes);

       gmsClearMemory
          ( (char *) theTI->buffer,
           numBytes);

       numBytes = sizeof(short) * theTI->header.numOfRows;

       theTI->recordIds = (void *) malloc (numBytes);

       gmsClearMemory
          ( (char *) theTI->recordIds,
           numBytes);

       buildThematicShortIntRecords
          (theTI->header.numOfDirEntries,
           (shortIntThematicRecord *) theTI->buffer,
           theTI->header.numOfRows,
           (short *) theTI->recordIds);
   }
   else if (theTI->header.kindOfDataElement == 'I')
   {
       printf("Build of INT thematic index tbl - NOT IMPLEMENTED\n");
   }
   else if (theTI->header.kindOfDataElement == 'F')
   {
       printf("Build of FLOAT thematic index tbl - NOT IMPLEMENTED\n");
   }
   else if (theTI->header.kindOfDataElement == 'R')
   {
       printf("Build of DOUBLE thematic index tbl - NOT IMPLEMENTED\n");
   }
   else
   {
       printf("*** ERROR : unknown data type in thematic index file\n");
   }
}


//---------------------------------------------*/
// getIndexFileType 
//
// Description:
//    Utility that converts the single character
//    of the "Type of index file" to an ASCII
//    string.
//       "I" - Inverted list index file
//       "T" - (simple) list index file
//       "B" - Bit array index
//       "G" - (other?) bit array index
//    REF:
//       Mil-Std-600006
//       Table 54, pg 77
//---------------------------------------------*/
static const char *getIndexFileType
                      (const char type)

{
   switch (type)
   {
       case 'I':
          return "Inverted list";

       case 'T':
          return "List";

       case 'B':
          return "Bit array";

       case 'G':
          return "TBD bit array";

       default:
          return "ERROR - Unknown type";
   }
}


//---------------------------------------------*/
// getDataElementType
//
// Description:
//    Utility that converts the single character
//    of the "Type of the data element being
//    indexed" to an ASCII string.
//       "I" - 4-byte integer
//       "T" - Character string
//       "S" - 2-byte integer
//       "F" - 4-byte float
//       "R" - 8 byte float (double)
//    REF:
//       Mil-Std-600006
//       Table 54, pg 77
//---------------------------------------------*/
static const char *getDataElementType
                      (const char type)

{
   switch (type)
   {
       case 'I':
          return "Integer";

       case 'T':
          return "char";

       case 'S':
          return "short";

       case 'F':
          return "float";

       case 'R':
          return "double";

       default:
          return "ERROR - Unknown element type";
   }
}


//---------------------------------------------*/
// getTypeSpecifier
//
// Description:
//    Utility that converts the single character
//    of the "Type specifier for the data portion
//    of the index" to an ASCII string.
//       "S" - 2-byte integer
//       "I" - 4-bypte integer
//       "S" - bit array index
//    REF:
//       Mil-Std-600006
//       Table 54, pg 77
//---------------------------------------------*/
static const char *getTypeSpecifier
                      (const char type)

{
   switch (type)
   {
       case 'S': 
          return "short or bit-array";

       case 'I': 
          return "int";

       default:
          return "ERROR - Unknown type specifier";
   }
}


//---------------------------------------------*/
// printHeaderRecord
//
// Description:
//    Utility that prints the contents of the
//    specified thematic-header record.
//---------------------------------------------*/
static void printHeaderRecord
               (thematicHeaderType theHdr)

{
         int i;

   printf("   Header of Thematic Index File:\n");
   printf("      Num bytes in this hdr  : %d\n", theHdr.numBytesThisHdr);
   printf("      Num of Dir Entries     : %d\n", theHdr.numOfDirEntries);
   printf("      Num of Rows            : %d\n", theHdr.numOfRows);
   printf("      Type of Index File     : %c (%s)\n",
          theHdr.kindOfFile,
          getIndexFileType(theHdr.kindOfFile));
   printf("      Kind of Data Elem      : %c (%s)\n",
          theHdr.kindOfDataElement,
          getDataElementType(theHdr.kindOfDataElement));
   printf("      Num Data Items per Dir : %d\n", theHdr.numDataItemsPerDir);
   printf("      Type Specifier         : %c (%s)\n",
          theHdr.typeSpecifier,
          getTypeSpecifier(theHdr.typeSpecifier));
   printf("      Name of VPF Table      : %s\n", theHdr.nameOfVpfTable);
   printf("      Column Name            : %s\n", theHdr.columnName);

   for (i = 0; i < sizeof(theHdr.notUsed); i++)
      printf("      Not Used[%d] (hex)  : %x\n", i, theHdr.notUsed[i]);
}


//---------------------------------------------*/
// buildThematicShortIntRecords
//
// Description:
//    This utility reads the data of the thematic
//    index file (i.e. the info after the header).
//    The "format" of the data is based on the
//    record "shortIntThematicRecord" (i.e. the
//    "directory" data) followed by an array of
//    "short" integers (i.e. the record Ids).
//---------------------------------------------*/
static void buildThematicShortIntRecords
               (int                    numDirs,
                shortIntThematicRecord *recordBuffer,
                int                    numRows,
                short                  *bufferOfShorts)

{
         int i;
         int nxt;

   #if 0
   for (i = 0; i < numDirs; i++)
   {
       nxt = fgetc(theme_fd);

       if ( isprint((char) nxt) != 0 )
       {
           printf(" %c ", (char) nxt);
       }
       else
       {
           printf("[0x%.2X]", nxt);
       }

       if ( (i != 0) && (i % 8 == 7) )
          printf("\n");
   }
   printf("\n");
   #else
   for (i = 0; i < numDirs; i++)
   {
       recordBuffer[i].whichRow        = gmsReadShortInteger (theme_fd);

       recordBuffer[i].fileOffsetToRow = gmsReadInteger (theme_fd);

       recordBuffer[i].numColumnsInRow = gmsReadInteger (theme_fd);
   }
   #endif

   for (i = 0; i < numRows; i++)
      bufferOfShorts[i] = gmsReadShortInteger (theme_fd);
}


//---------------------------------------------*/
// printShortIntThematicRecord
//
// Description:
//    Utility to print the contents of the
//    "directory" data and also the array of
//    record Ids.
//---------------------------------------------*/
static void printShortIntThematicRecord
               (int                    numDirs,
                shortIntThematicRecord *recordBuffer,
                int                    numRows,
                short                  *bufferOfShorts)

{
         int i;
         int lineCount = 0;

   printf("     Record \t Row \t Offset \t Num-Columns\n");
   printf("     -------------------------------------------------\n");

   for (i = 0; i < numDirs; i++)
      {
       printf("      %d) \t\t %d \t %ld \t\t %d\n",
             i,
             (int) recordBuffer[i].whichRow,
             recordBuffer[i].fileOffsetToRow,
             recordBuffer[i].numColumnsInRow);
      }

   printf("     Record-Ids:\n");
   printf("     ------------------------\n");

   for (i = 0; i < numRows; i++)
      {
       if (lineCount == 0)
          printf("     %d)", i + 1);

       printf(" %d, ", (int) bufferOfShorts[i]);

       if (lineCount == 9)
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;
      }

   printf("\n");
}
