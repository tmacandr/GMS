//----------------------------------------------------------------------------*/
// File : gmsRingTable.cpp
// Date : 18-Aug-99 : Initial definition
//        05-Oct-99 : Clean-up due to code inspection
//
// Description:
//    Utilities to "read" any "Ring Table" file of the Digital Chart of the
//    World (DCW) database.  A "Ring Table" is a file of n records of the
//    following format:
//
//             ID        =I, 1, P, Row ID,-,-,:
//             FACE_ID   =I, 1, F, Foreign key to face table,-,-,:
//             START_EDGE=I, 1, F, Foreign Key to Edge Table,-,-,:
//
//    A ring table contains one reference to the edge table for each ring of
//    a face.  The first row in the ring table for each face refers to the
//    outer ring of that face.  Outer rings are traversed in clockwise
//    direction.  Each inner ring has one reference to a first edge on
//    that ring.  The ring table maintains an order relationship for its
//    rows.  The first record of a new face id will always be defined as the
//    outer ring.  Any repeating records with an identical face value will
//    define inner rings.
//
//    Also, consider the following:
//
//       Ring:
//       -----
//          A connected set of edges that composes a closed
//          face border.  Any single ring is only referenced
//          to and by a single face.  If the same set of edges
//          is shared by two different faces, two rings that
//          correspond to the two faces are created from the
//          single edge set.  Rings only occur at level 3
//          topology (when faces are also present).
//
//       Face:
//       -----
//          A region enclosed by an edge or set of edges.
//          Faces are topologically linked to their surrounding
//          edges as well as to the other faces that surround them.
//          Faces are always nonoverlapping, exhausting the area
//          of a plane
//
//       Edge:
//       -----
//          A one-dimensional primitive used to represent the
//          location of a linear feature and/or the borders of
//          faces.  Depending upon the level of topology, edges
//          may be linked to nodes, [other] edges, and faces.
//          Edges are composed of an ordered collection of two or
//          more coordinate tuples (pairs or triplets).  At least
//          two of the coordinate tuples must be distinct.  The
//          orientation of an edge can be recognized by the
//          ordering of the coordinate tuples.  [i.e. the first
//          coordinate is the START node, the last coordinate is
//          the END node.]
//
//    Reference:
//        1) Mil-Std-600006: Section 5.3.2.3(b) - pg 54
//        2) Mil-D-89009   : Section 6.4 - pg 57-68
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsRingTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------*/
//     Local Variables
//-----------------------------*/
static FILE *ring_fd = (FILE *) NULL;


//-----------------------------*/
// Declare Local Subprograms
//-----------------------------*/
static void readPastFormatInformation ();

static void buildRingTable
               (ringTableType *theRingTbl);

static void printOneRingRecord
               (ringRecordType ringRecord);

static void debugPrintContentsOfFile ();


//---------------------------------------------*/
// gmsGetRingTable
//
// Description:
//    This utility reads the file that contains
//    the "Ring Table".  A pointer to a newly
//    allocated table is returned to the caller.
//    It is the caller's responsibility to free
//    the item by using the utility
//    "gmsFreeRingTable" (see below).
//---------------------------------------------*/
ringTableType *gmsGetRingTable
                     (const char *ringTableFilePath)

{
         const char    readOnlyMode[] = { "rb" };
         ringTableType *theRingTbl;

   if (ringTableFilePath == NULL)
      return (ringTableType *) NULL;

   ring_fd = fopen(ringTableFilePath,
                   readOnlyMode);

   if (ring_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open RNG file: %s\n",
              ringTableFilePath);

       return (ringTableType *) NULL;
      }

   readPastFormatInformation ();

   theRingTbl = (ringTableType *) malloc (sizeof(ringTableType));

   gmsClearMemory
      ( (char *) theRingTbl,
        sizeof(ringTableType) );

   buildRingTable (theRingTbl);

   fclose (ring_fd);

   return theRingTbl;
}


//---------------------------------------------*/
// gmsFreeRingTable
//
// Description:
//    This utility frees a "Ring Table" that had
//    been previously allocated using
//    "gmsGetRingTable".
//---------------------------------------------*/
void gmsFreeRingTable
           (ringTableType *theRingTable)

{
   if (theRingTable == NULL)
      return;

   if (theRingTable->ringRecords != NULL)
      free (theRingTable->ringRecords);

   free (theRingTable);
}


//---------------------------------------------*/
// gmsPrintRingTable	
//
// Description:
//    This function will print the "Ring Table"
//    object to standard out.
//---------------------------------------------*/
void gmsPrintRingTable
           (ringTableType *theRingTable)

{
         int i;

   printf("________________________________________\n");
   printf("Ring Table:\n");

   if (theRingTable == NULL)
      return;

   printf("Num Records : %d\n", theRingTable->numRecords);

   if (theRingTable->ringRecords == NULL)
      return;

   for (i = 0; i < theRingTable->numRecords; i++)
      {
       printOneRingRecord (theRingTable->ringRecords[i]);
      }
}


     //-----------------------*/
     //   Local Subprograms
     //-----------------------*/


//---------------------------------------------*/
// readPastFormatInformation
//
// Description:
//    This function will read the format data
//    located at the front of the "Ring Table"
//    file.
//---------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (ring_fd);

   tempChar = gmsReadInteger (ring_fd);

   tempChar = fgetc (ring_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (ring_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (ring_fd);
      }
}


//---------------------------------------------*/
// buildRingTable
//
// Description:
//    This function will read the actual data
//    from the Ring Table file.  The data read
//    will be used to populate the attributes of
//    the object.
//---------------------------------------------*/
static void buildRingTable
               (ringTableType *theRingTbl)

{
         int numBytes = 0;
         int numRecords;
         int tempChar;
         int index;

   tempChar = fgetc(ring_fd);

   while ( tempChar != EOF )
      {
       numBytes++;

       tempChar = fgetc(ring_fd);
      }

   numRecords = numBytes / sizeof(ringRecordType);

   rewind (ring_fd);

   readPastFormatInformation ();

   numBytes = numRecords * sizeof(ringRecordType);

   theRingTbl->ringRecords = (ringRecordType *) malloc (numBytes);

   theRingTbl->numRecords = numRecords;

   for (index = 0; index < numRecords; index++)
      {
       theRingTbl->ringRecords[index].ID = gmsReadInteger (ring_fd);

       theRingTbl->ringRecords[index].faceId = gmsReadInteger (ring_fd);

       theRingTbl->ringRecords[index].startEdge = gmsReadInteger (ring_fd);
      }
}


//---------------------------------------------*/
// printOneRingRecord
//
// Description:
//    This function will print the specified
//    "ringRecordType" record to stdout.
//---------------------------------------------*/
static void printOneRingRecord 
                  (ringRecordType ringRecord)

{
   printf("      %d) Face-ID = %d  -  Start-Edge = %d\n",
          ringRecord.ID,
          ringRecord.faceId,
          ringRecord.startEdge);
}


//---------------------------------------------*/
// debugPrintContentsOfFile
//
// Description:
//    Debug utility to print the contents of the
//    "Ring Table" file (in hex-byte format) to
//    stdout. 
//---------------------------------------------*/
static void debugPrintContentsOfFile ()

{
         int tempChar;
         int numBytes = 0;
         int lineCount = 0;
         int lengthOfHeader = 0;
         int i;

   printf("_________________________________________\n");
   printf("Bytes of the Ring Table\n");

   rewind (ring_fd);

   lengthOfHeader = gmsReadInteger (ring_fd);

   printf("Header:\n");

   for (i = 0; i < lengthOfHeader; i++)
      {
       tempChar = fgetc (ring_fd);

       printf("%.2x ", tempChar);

       if ( lineCount == 3)
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;
      }

   printf("\n");

   printf("Data:\n");

   lineCount = 0;

   do
      {
       tempChar = fgetc (ring_fd); 

       if ( tempChar == EOF ) break;

       numBytes++;

       printf("%.2x ", tempChar);

       if ( lineCount == 3 )
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;
      }
   while ( true );

   printf("---> Total Num of 'ring' bytes = %d\n", numBytes);
   printf("_________________________________________\n");

   rewind (ring_fd);
}
