/*----------------------------------------------------------------------------*/
/* File : gmsTextTable.cpp
/* Date : 17-Aug-99 : Initial definition
/*        07-Oct-99 : Clean-up due to code-inspection
/*        13-Oct-99 : routine figures out which format of record
/*
/* Description:
/*    Utilities to "read/process" any "Text Table" file of the Digital Chart
/*    of the World (DCW) database.  There are two type of TEXT tables.  One
/*    is for the BROWSE library and the second is for the REGIONAL libraries.
/*
/*       Text Feature:
/*       -------------
/*          A cartographic entity that relates a textual
/*          description to a zero- or one-dimensional location.
/*          A text feature usually contains information such as
/*          font, color, and height [i.e. primitives].
/*
/*       Text Feature Class:
/*       -------------------
/*          A collection of text features that maintains a
/*          homogeneous set of attributes.  [i.e. it is a set
/*          of text primitives].
/*
/*    A BROWSE "Text Table" is an array of records of the format:
/*
/*       Browse : Text Primitive:
/*       ------------------------
/*          ID           =I, 1, P, Row ID,-,-,:
/*          <num-chars>  =I, 1, N, Number of chars in STRING:
/*          STRING       =T, *, N, Text String,-,-,:
/*          <num-coords> =I, 1, N, Number of coordinates in SHAPE_LINE:
/*          SHAPE_LINE   =C, *, N, Shape Coordinate String,-,-,:
/*
/*    A "Text Table" from a REGIONAL library has the format:
/*
/*       Regional : Text Primitive:
/*       -----------------------
/*          ID          =I,1,P,Row Identifier,-,-,:
/*          *TEXT.TFT_ID=I,1,F,Foreign Key to Text Feature Table,-,-,:
/*          <num-chars> =I,1,N,Number of chars in STRING:
/*          STRING      =T,*,N,Text String,-,-,:
/*          <num-coords>=I,1,N,Number of coordinates in SHAPE_LINE:
/*          SHAPE_LINE  =C,*,N,Shape of Text String,-,-,:;
/*
/*          where the * in "*TEXT.TFT_ID" corresponds to the theme
/*          of the text feature table.
/*
/*    The "string" is an array of n characters.  The "shape-line" is an array
/*    of coordinates (2-dimensional) that indicate the placement of the
/*    string (on the map).  The "shape-line" has 1 .. n coordinates.  If
/*    only one coordinate is defined, then it is the lower-left point in
/*    the string.  If two coordinates are defined, the second coordinate
/*    defines the lower right of the string.  If 3 or more coordinates are
/*    defined, then the array defines a curve and the 2 .. (n - 1) points
/*    define the control points of the curve.
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsTextTable.h>
#include <gmsIndexFile.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-----------------------------*/
/*     Local Type Declares
/*-----------------------------*/
typedef enum { gmsBrowseLibText,
               gmsRegionalLibText } gmsKindOfTextType;

/*-----------------------------*/
/*     Local Variables
/*-----------------------------*/
static FILE *txt_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms
/*-----------------------------*/
static void buildTextTable
               (textTableType  *theTxtTbl,
                indexTableType *theIndexFile);

static void printTextTable
                  (textTableType *theTbl);

static void debugPrintTextRecord
                  (int numBytes);

static void getTextIndexFileName
                (const char *txtFileNameIn,
                 char       *txxFileNameOut);

static gmsKindOfTextType determineKindOfTextRecord ();


/*---------------------------------------------*/
/* gmsGetTextTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Text Table".  This routine determines
/*    the corresponding "Text Index File" (TXX)
/*    based on the specified "Text Table".  Also,
/*    this routine determines which format to use
/*    for the records of the Text Table.
/*
/*    It is the caller's responsibility to free
/*    the item by using the utility
/*    "gmsFreeTextTable" (see below).
/*---------------------------------------------*/
textTableType *gmsGetTextTable
                     (const char *textTableFilePath)

{
         const char     readOnlyMode[] = { "rb" };
         indexTableType *theIndexFile;
         textTableType  *theTextTable;
         char           textIndexFilePath[512];

   if (textTableFilePath == NULL)
      return (textTableType *) NULL;

   getTextIndexFileName
      (textTableFilePath,
       textIndexFilePath);

   theIndexFile = gmsGetIndexFile (textIndexFilePath);

   if (theIndexFile == NULL)
      {
       printf("*** ERROR : the text index table (TXX) %s is NULL.\n",
              textIndexFilePath);

       return (textTableType *) NULL;
      }

   txt_fd = fopen(textTableFilePath,
                  readOnlyMode);

   if (txt_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open TEXT Table file: %s\n",
              textTableFilePath);

       gmsFreeIndexFile (theIndexFile);
 
       return (textTableType *) NULL;
      }

   theTextTable = (textTableType *) malloc (sizeof(textTableType));

   gmsClearMemory
      ((char *) theTextTable,
       sizeof(textTableType));

   buildTextTable
      (theTextTable,
       theIndexFile);

   fclose (txt_fd);

   gmsFreeIndexFile (theIndexFile);

   return theTextTable;
}


/*---------------------------------------------*/
/* gmsFreeTextTable
/*
/* Description:
/*    This utility frees a "Text Table" that had
/*    been previously allocated using
/*    "gmsGetTextTable" (see above).
/*---------------------------------------------*/
void gmsFreeTextTable
           (textTableType *theTextTbl)

{
   if (theTextTbl == NULL)
      return;

   if (theTextTbl->textRecords != NULL)
      {
       if (theTextTbl->textRecords[0].theString != NULL)
         free (theTextTbl->textRecords[0].theString);

       if (theTextTbl->textRecords[0].theCoords != NULL)
          free (theTextTbl->textRecords[0].theCoords);
      } 

   free (theTextTbl);
}


/*---------------------------------------------*/
/* gmsPrintTextTable
/*
/* Description:
/*    This function will print the "Text Table"
/*    object to standard out.
/*---------------------------------------------*/
void gmsPrintTextTable
           (textTableType *theTextTbl)

{
   printf("________________________________________\n");
   printf("Text Table:\n");

   if (theTextTbl == NULL)
      return;

   printf("Num Records : %d\n", theTextTbl->numRecords);

   if (theTextTbl->textRecords == NULL)
      return;

   printTextTable (theTextTbl);
}


     /*-----------------------*/
     /*   Local Subprograms
     /*-----------------------*/


/*---------------------------------------------*/
/* buildTextTable
/*
/* Description:
/*    This function will read the actual data
/*    from the Text Table file.  The data read
/*    will be used to populate the attributes of
/*    the object.
/*
/*    The "index file" is used to locate the
/*    beginning of each text record.  Each text
/*    record is a variant length record.  The "index
/*    file" specifies the offsets to the start of
/*    each text record.
/*
/*    Note:
/*    -----
/*       This utility is "expensive" because it
/*       does 2 passes through the TEXT file.
/*---------------------------------------------*/
static void buildTextTable
               (textTableType  *theTxtTbl,
                indexTableType *theIndexFile)

{
         int               i;
         int               j;
         int               stringIndex = 0;
         int               coordIndex = 0;
         int               numBytes;
         int               numCharsInRec = 0;
         int               totalNumChars = 0;
         int               numCoordsInRec = 0;
         int               totalNumCoords = 0;
         int               ignoreInt;
         char              *stringBuffer;
         twoDimCoordType   *coordBuffer;
         textRecordType    *theBuffer;
         gmsKindOfTextType kindOfTextRecord;

   kindOfTextRecord = determineKindOfTextRecord ();

   theTxtTbl->numRecords = theIndexFile->numberRecordsInTable;

   numBytes = theTxtTbl->numRecords * sizeof(textRecordType);

   theTxtTbl->textRecords = (textRecordType *) malloc (numBytes);

   theBuffer = theTxtTbl->textRecords;

   gmsClearMemory
       ( (char *) theBuffer,
        numBytes);

   for (i = 0; i < theTxtTbl->numRecords; i++)
      {
       /*
       fseek
          (txt_fd,
           theIndexFile->indexList[i].byteOffset,
           SEEK_SET);

       debugPrintTextRecord (theIndexFile->indexList[i].numBytes);
       */

       fseek
          (txt_fd,
           theIndexFile->indexList[i].byteOffset,
           SEEK_SET);

       theBuffer[i].ID = gmsReadInteger (txt_fd);

       if (kindOfTextRecord == gmsBrowseLibText)
          theBuffer[i].fgnKeyToFeatureTable = -1;
       else
          theBuffer[i].fgnKeyToFeatureTable = gmsReadInteger (txt_fd);

       numCharsInRec = gmsReadInteger (txt_fd);

       totalNumChars = totalNumChars + numCharsInRec + 1; /* for '/0' char */

       for (j = 0; j < numCharsInRec; j++)
          ignoreInt = fgetc (txt_fd);

       numCoordsInRec = gmsReadInteger (txt_fd);

       totalNumCoords = totalNumCoords + numCoordsInRec;
      }

   stringBuffer = (char *) malloc (totalNumChars);

   numBytes = totalNumCoords * sizeof(twoDimCoordType);

   coordBuffer = (twoDimCoordType *) malloc (numBytes);

   stringIndex = 0;

   coordIndex = 0;

   for (i = 0; i < theTxtTbl->numRecords; i++)
      {
       fseek
          (txt_fd,
           theIndexFile->indexList[i].byteOffset,
           SEEK_SET);

       ignoreInt = gmsReadInteger (txt_fd);    /* already read the ID  */

       if (kindOfTextRecord == gmsRegionalLibText)
          ignoreInt = gmsReadInteger (txt_fd); /* already read Fgn-Key */

       numCharsInRec = gmsReadInteger (txt_fd);

       theBuffer[i].numChars = numCharsInRec;

       fgets
          (&stringBuffer[stringIndex],
           (numCharsInRec + 1),
           txt_fd);

       theBuffer[i].theString = &stringBuffer[stringIndex];

       stringIndex = stringIndex + (numCharsInRec + 1);

       numCoordsInRec = gmsReadInteger (txt_fd);

       theBuffer[i].numCoords = numCoordsInRec;

       theBuffer[i].theCoords = &coordBuffer[coordIndex];

       for (j = 0; j < numCoordsInRec; j++)
          {
           coordBuffer[coordIndex + j] = gmsReadTwoDimCoord (txt_fd);
          }

       coordIndex = coordIndex + numCoordsInRec;
      }
}


/*---------------------------------------------*/
/* debugPrintTextRecord 
/*
/* Description:
/*---------------------------------------------*/
static void debugPrintTextRecord
               (int numBytes)

{
         int i;
         int lineCount = 0;
         int tempChar;
         int tempInt;
         int numChars = 0;
         int numBytesLeft = 0;

   tempInt = gmsReadInteger (txt_fd);
 
   printf("________________________\n");
   printf("Text Record         : %d\n", tempInt);

   numChars = gmsReadInteger (txt_fd);

   printf("Num Chars in String : %d\n", numChars);
   printf("String              : >>>");

   for (i = 0; i < numChars; i++)
      {
       tempChar = fgetc (txt_fd);

       printf("%c", (char) tempChar);
      }

   printf("<<<\n");

   tempInt = gmsReadInteger (txt_fd);

   numBytesLeft = numBytes - sizeof(int) - sizeof(int) - numChars - sizeof(int);

   printf("Num bytes left      : %d\n", numBytesLeft);
   printf("Num Coords          : %d\n", tempInt);
   printf("   ");

   for (i = 0; i < numBytesLeft; i++)
      {
       tempChar = fgetc (txt_fd);

       printf("%.2x ", tempChar);

       if (lineCount == 3)
          {
           printf("\n   ");

           lineCount = 0;
          }
       else
          lineCount++;
      }

   printf("\n");
}


/*---------------------------------------------*/
/* printTextTable
/*
/* Description:
/*    Utility that prints the contents of the
/*    specified "text-table" to stdout.
/*---------------------------------------------*/
static void printTextTable
                  (textTableType *theTbl)

{
         int            i, j;
         textRecordType theRec;

   printf("ID \t F-Key \t String \t Num-Coords\n");
   printf("--------------------------------------------\n");

   for (i = 0; i < theTbl->numRecords; i++)
      {
       theRec = theTbl->textRecords[i];

       printf("%d \t %d \t %s \t\t %d\n",
              theRec.ID,
              theRec.fgnKeyToFeatureTable,
              theRec.theString,
              theRec.numCoords);

       for (j = 0; j < theRec.numCoords; j++)
          printf("\t\t\t\t(%f, %f)\n",
                 theRec.theCoords[j].Long,
                 theRec.theCoords[j].Lat);
       printf("________\n");
      }
}


/*---------------------------------------------*/
/* getTextIndexFileName 
/*
/* Description:
/*    This function will determine the name of the
/*    Text-Index-File (TXX) based on the given
/*    name of the Text-File (TXT).  It is required
/*    that the caller pass in an "txxFileNameOut"
/*    string that is the same size or bigger as
/*    the "txtFileNameIn" string.
/*---------------------------------------------*/
static void getTextIndexFileName
                (const char *txtFileNameIn,
                 char       *txxFileNameOut)

{
         const size_t numChars = strlen (txtFileNameIn);

   strncpy
      (txxFileNameOut,
       txtFileNameIn,
       numChars);

   /* be case sensitive */
   if (txtFileNameIn[numChars - 1] == 'T')

      txxFileNameOut[numChars - 1] = 'X';

   else

      txxFileNameOut[numChars - 1] = 'x';

   txxFileNameOut[numChars] = '\0';
}


/*---------------------------------------------*/
/* determineKindOfTextRecord
/*
/* Description:
/*    This function reads information from the
/*    file header to determine which of the two
/*    variants of the text record to use in order
/*    to build the text-table.
/*---------------------------------------------*/
static gmsKindOfTextType determineKindOfTextRecord ()

{
         int       sizeOfHdr;
         const int sizeOfBrowseHdr = 151;

   rewind (txt_fd);

   sizeOfHdr = gmsReadInteger (txt_fd);

   if (sizeOfHdr == sizeOfBrowseHdr)

      return gmsBrowseLibText;

   else

      return gmsRegionalLibText;
}
