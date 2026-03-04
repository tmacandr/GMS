//----------------------------------------------------------------------------*/
// File : gmsEdgeTable.cpp
// Date : 26-Jul-99 : Initial Definition
//        01-Aug-99 : Convert to utilites on port to Sun Solaris
//        18-Aug-99 : "stumbled" on attribute "Number-of-Coordinates"  
//        07-Oct-99 : Clean-up due to code-inspection
//        12-Oct-99 : There are 4 variations to the edge record
//        16-Nov-99 : mod to let ANSI C compiler work
//
// Description:
//    Utilities to "read" any "Edge Table" file of the Digital Chart of the
//    World (DCW).  Consider the following definitions:
//
//       Edge:
//       -----
//          A one-dimensional primitive used to represent the
//          location of a linear feature and/or the borders
//          of faces.  Depending upon the level of topology,
//          edges may be topologically linked to nodes, [other]
//          edges, and faces.  Edges are composed of an ordered
//          collection of two or more coordinate tuples (pairs
//          or tiplets).  At least two of the coordinate tuples
//          must be distinct.  The orientation of an edge can be
//          recognized by the ordering of the coordinate tuples
//          [i.e. the first tuple is the start node and the last
//          tuple is the end node].
//
//       Coordinate:
//       -----------
//          A specified position in Cartesian space.  The
//          value takes the form of a short or long floating
//          point value.  A Z value (if any) is ignored during
//          the enforcement and use of planar graphs.
//
//       Entity:
//       -------
//          The terminating node of an edge.
//
//    There are three types of edge records in the DCW database.  The first
//    describes edge records at level-3 topology.  The format is:
//
//       Topology level 3 Edge Primitive Table;-;
//       -----------------------------------------
//             ID          =I,1,P,Row ID,-,-,:
//             START_NODE  =I,1,N,"Start/Left Node",-,-,:
//             END_NODE    =I,1,N,"End/Right Node",-,-,:
//             RIGHT_FACE  =K,1,F,Right Face,-,-,:
//             LEFT_FACE   =K,1,F,Left Face,-,-,:
//             RIGHT_EDGE  =K,1,F,Right Edge from End Node,-,-,:
//             LEFT_EDGE   =K,1,F,Left Edge from Start Node,-,-,:
//             [num-coords]=I,1,N,Number of coords in array
//             COORDINATES =C,*,N,Edge Coordinates,-,-,:;
//
//          This format applies to the following themes from REGIONAL
//          libraries:
//
//                    VG, PP, LC, and TileRef
//
//          Also, this format applies to the following themes from the
//          BROWSE library:
//
//                    B-PO      B-GR
//                    B-LibRef  B-IN
//                    B-DV      B-DN
//                    B-DA      B-CO
//
//    The second variant makes use of the optional attribute called "Foreign
//    Key to Line Feature Table".  It too is a level-3 topology record.  The
//    format is:
//
//       Level 3 topology with Fgn Key to Feature Table:
//       -----------------------------------------------
//             ID           =I,1,P,Row Identifier,-,-,:
//             *LINE.LFT_ID =I,1,F,Foreign Key to <Line> Feature Table,-,-,:
//             START_NODE   =I,1,F,Start/Left Node,-,-,:
//             END_NODE     =I,1,F,End/Right Node,-,-,:
//             RIGHT_FACE   =K,1,F,Right Face,-,-,:
//             LEFT_FACE    =K,1,F,Left Face,-,-,:
//             RIGHT_EDGE   =K,1,F,Right Edge from End Node,-,-,:
//             LEFT_EDGE    =K,1,F,Left Edge from Start Node,-,-,:
//             [num-coords] =I,1,N,Number of coords in array
//             COORDINATES  =C,*,N,Coordinates of Edge,-,-,:;
//
//          This format applies to the following themes from REGIONAL
//          libraries:
//
//                    PO, HY, DQ, DN, CL
//
//    The third variant describes edge records at level 1-2 topology.  However,
//    this format also makes use of the optional attribute "Foreign Key to
//    Line Feature Table"  The format is:
//
//       Level 1-2 topology - Edge Primitives:
//       -------------------------------------
//             ID           =I,1,P,Row Identifier,-,-,:
//             *LINE.LFT_ID =I,1,F,Foreign Key to Line Feature Table,-,-,:
//             START_NODE   =I,1,F,Start/Left Node,-,-,:
//             END_NODE     =I,1,F,End/Right Node,-,-,:
//             RIGHT_EDGE   =K,1,F,Right Edge from End Node,-,-,:
//             LEFT_EDGE    =K,1,F,Left Edge from Start Node,-,-,:
//             [num-coords] =I,1,N,Number of coords in array
//             COORDINATES  =C,*,N,Coordinates of Edge,-,-,:;
//
//          This format applies to the following themes from REGIONAL
//          libraries:
//
//                    UT, TS, RR, RD, PH, OF, HS
//
//    The third variant describes edge records from the LIBREF theme of a
//    REGIONAL library.  It is a level 1-2 record but does NOT make use of
//    the optional attribute "Foreign Key to Line Feature Table".  The
//    format is:
//
//       LibRef - Edge Primitive;-;
//       --------------------------
//             ID           =I,1,P,Row Identifier,-,-,:
//             START_NODE   =I,1,F,Start/Left Node,-,-,:
//             END_NODE     =I,1,F,End/Right Node,-,-,:
//             RIGHT_EDGE   =K,1,F,Right Edge from End Node,-,-,:
//             LEFT_EDGE    =K,1,F,Left Edge from Start Node,-,-,:
//             [num-coords] =I,1,N,Number of coords in array
//             COORDINATES  =C,*,N,Coordinates of Edge,-,-,:;
//
//          This format applies ONLY to the LibRef theme from a
//          REGIONAL library.
//
//    Reference:
//        1) Mil-Std-600006
//        2) Mil-D-89009
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsEdgeTable.h>
#include <gmsIndexFile.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { gmsUnknownEdge,
               gmsLevel_3_No_Feature_Tbl,
               gmsLevel_3_With_Feature_Tbl,
               gmsLevel_1_2_With_Feature_Tbl,
               gmsLibRefEdge } kindOfEdgeRecordType;

//-----------------------------*/
//     Local Variables       --*/
//-----------------------------*/
static FILE *edg_fd = (FILE *) NULL;


//-----------------------------*/
// Declare Local Subprograms --*/
//-----------------------------*/
static void getEdgeIndexFileName
                (const char *edgFileNameIn,
                 char       *edxFileNameOut);

static void buildEdgeTable
               (edgeTableType  *theEdgTbl,
                indexTableType *theIndexFile);

static void printOneEdgeHeaderRecord 
               (edgeRecordHeaderType headerInfo);

static kindOfEdgeRecordType determineKindOfEdgeRecord ();

static edgeRecordHeaderType readOneHeaderRecord
                               (kindOfEdgeRecordType kindOfEdgeRecord);

static void debug_printEdgeRecord
               (int numBytes);



//---------------------------------------------*/
// gmsGetEdgeTable
//
// Description:
//    This utility reads the file that contains
//    the "Edge Table" (EDG).  This routine will
//    determine which "Edge Index" (EDX) file to
//    use based on the name of the EDG file.
//    Furthermore, this routine will determine
//    which of the 4 formats of edge records is
//    applicable.
//
//    It is the caller's responsibility
//    to free the item by using the utility
//    "gmsFreeEdgeTable" (see below).
//---------------------------------------------*/
edgeTableType *gmsGetEdgeTable
                     (const char *edgeTableFilePath)

{
         const char     readOnlyMode[] = { "rb" };
         edgeTableType  *theEdgTbl;
         indexTableType *theIndexFile;
         #define        Num_Chars  512
         const int      numChars = Num_Chars;
         char           edgeIndexFilePath[Num_Chars];

   if (edgeTableFilePath == NULL)
      return (edgeTableType *) NULL;

   getEdgeIndexFileName
      (edgeTableFilePath,
       edgeIndexFilePath);

   theIndexFile = gmsGetIndexFile (edgeIndexFilePath);

   if (theIndexFile == NULL)
      {
       printf("*** ERROR : the edge index table %s is NULL\n",
              edgeIndexFilePath);

       return (edgeTableType *) NULL;
      }

   edg_fd = fopen(edgeTableFilePath,
                  readOnlyMode);

   if (edg_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open Edge file: %s\n",
              edgeTableFilePath);

       gmsFreeIndexFile (theIndexFile);

       return (edgeTableType *) NULL;
      }

   theEdgTbl = (edgeTableType *) malloc (sizeof(edgeTableType));

   gmsClearMemory
      ((char *) theEdgTbl,
       sizeof(edgeTableType));

   buildEdgeTable
      (theEdgTbl,
       theIndexFile);

   fclose (edg_fd);

   gmsFreeIndexFile (theIndexFile);

   return theEdgTbl;
}


//---------------------------------------------*/
// gmsFreeEdgeTable
//
// Description:
//    This utility frees an "Edge Table" that had
//    been previously allocated using
//    "gmsGetEdgeTable".
//---------------------------------------------*/
void gmsFreeEdgeTable
           (edgeTableType *theEdgTbl)

{
   if (theEdgTbl == NULL)
      return;

   if (theEdgTbl->edgeRecords != NULL)
      {
       free (theEdgTbl->edgeRecords[0].theCoords);

       free (theEdgTbl->edgeRecords);
      }

   free (theEdgTbl);
}


//---------------------------------------------*/
// gmsPrintEdgeTable	
//
// Description:
//    This function will print the "Edge Table"
//    object to stdout.
//---------------------------------------------*/
void gmsPrintEdgeTable
        (edgeTableType *theEdgTbl)

{
         int i;
         int j;

   printf("________________________________________\n");
   printf("Edge Table:\n");

   if (theEdgTbl == NULL)
      return;

   printf("Num Records : %d\n", theEdgTbl->numRecords);

   if (theEdgTbl->edgeRecords == NULL)
      return;

   for (i = 0; i < theEdgTbl->numRecords; i++)
      {
       printOneEdgeHeaderRecord (theEdgTbl->edgeRecords[i].headerInfo);

       printf("   Num Coords : %d\n", theEdgTbl->edgeRecords[i].numCoords);

       for (j = 0; j < theEdgTbl->edgeRecords[i].numCoords; j++)
          printf("      %d) (%f, %f)\n",
                 j,
                 theEdgTbl->edgeRecords[i].theCoords[j].Long,
                 theEdgTbl->edgeRecords[i].theCoords[j].Lat);
      }
}


     //-----------------------*/
     //   Local Subprograms
     //-----------------------*/


//---------------------------------------------*/
// buildEdgeTable
//
// Description:
//    This function will read the actual data
//    from the Edge Table file.  The data read
//    will be used to populate the attributes of
//    the object.
//---------------------------------------------*/
static void buildEdgeTable
               (edgeTableType  *theEdgTbl,
                indexTableType *theIndexFile)

{
         int                  numBytes = 0;
         int                  i;
         int                  j;
         int                  numCoordsInRec = 0;
         twoDimCoordType      *coordsBuffer;
         int                  coordIndex = 0;
         kindOfEdgeRecordType kindOfEdgeRecord;

   kindOfEdgeRecord = determineKindOfEdgeRecord ();

   if (kindOfEdgeRecord == gmsUnknownEdge)
      {
       printf("*** ERROR : unknown EDGE record.\n");

       return;
      }

   theEdgTbl->numRecords = theIndexFile->numberRecordsInTable;

   numBytes = sizeof(edgeRecordType) * theEdgTbl->numRecords;

   theEdgTbl->edgeRecords = (edgeRecordType *) malloc (numBytes);

   gmsClearMemory
      ( (char *) theEdgTbl->edgeRecords,
        numBytes);

   numBytes = 0;

   for (i = 0; i < theIndexFile->numberRecordsInTable; i++)
      numBytes = numBytes + theIndexFile->indexList[i].numBytes;

   //--------------------------------------------------*/
   // To avoid using too much heap memory, adjust
   // (i.e. subtract) for the smallest version of the
   // 4 versions of the edge records ... which is the
   // "LibRef" version.
   //
   //     ID         - 4 bytes
   //     Start Node - 4 bytes
   //     End Node   - 4 bytes
   //     Right Edge - only 1 byte (don't know if there's more)
   //     Left Edge  - only 1 byte (           "              )
   //     Num Coords - 4 bytes
   //     ----------------------
   //                  18 bytes
   //--------------------------------------------------*/
   numBytes = numBytes - (theIndexFile->numberRecordsInTable * 18);

   //--------------------------------------------------*/
   // Buffer will be just big enough ... or maybe even
   // a little too big (due to not subtracting for the
   // triplet-id records).  But this is done rather
   // than two passes through the file.
   //--------------------------------------------------*/
   coordsBuffer = (twoDimCoordType *) malloc (numBytes);

   gmsClearMemory
      ( (char *) coordsBuffer,
        numBytes);

   coordIndex = 0;

   for (i = 0; i < theEdgTbl->numRecords; i++)
      {
       fseek
          (edg_fd,
           theIndexFile->indexList[i].byteOffset,
           SEEK_SET);

       theEdgTbl->edgeRecords[i].headerInfo =
          readOneHeaderRecord (kindOfEdgeRecord);

       theEdgTbl->edgeRecords[i].theCoords = &(coordsBuffer[coordIndex]);

       numCoordsInRec = gmsReadInteger (edg_fd);

       theEdgTbl->edgeRecords[i].numCoords = numCoordsInRec;

       for (j = 0; j < numCoordsInRec; j++)
          {
           coordsBuffer[coordIndex] = gmsReadTwoDimCoord (edg_fd);

           coordIndex++;
          }
      }
}


//---------------------------------------------*/
// debug_printEdgeRecord 
//
// Description:
//---------------------------------------------*/
static void debug_printEdgeRecord
               (int numBytes)

{
         int        tempChar;
         int        i;
         int        lineCount = 0;
         static int numRecords = 1;

   printf("________________________________\n");
   printf("Edge Record : %d (total bytes = %d)\n", numRecords, numBytes);

   for (i = 0; i < numBytes; i++)
      {
       tempChar = fgetc (edg_fd);

       printf("%.2x  ", tempChar);

       lineCount++;

       if (lineCount == 4)
          {
           printf("\n");

           lineCount = 0;
          }
      }

   if (lineCount != 0)
      printf("\n");

   numRecords++;
}


//---------------------------------------------*/
// printOneEdgeHeaderRecord
//
// Description:
//    This function will print the specified
//    "edgeRecordHeaderType" record to stdout.
//---------------------------------------------*/
static void printOneEdgeHeaderRecord 
                  (edgeRecordHeaderType headerInfo)

{
   printf("         ID : %d\n", headerInfo.ID);
   printf("Fgn Key     : %d\n", headerInfo.fgnKeyToFeatureTbl);
   printf("Start Node  : %d\n", headerInfo.startNode);
   printf("End Node    : %d\n", headerInfo.endNode);
   printf("Right Face  : (ID : %d, tile-ID : %d, external-ID : %d)\n",
              headerInfo.rightFace.id,
              headerInfo.rightFace.tileId,
              headerInfo.rightFace.externalId);
   printf("Left Face   : (ID : %d, tile-ID : %d, external-ID : %d)\n",
              headerInfo.leftFace.id,
              headerInfo.leftFace.tileId,
              headerInfo.leftFace.externalId);
   printf("Right Edge  : (ID : %d, tile-ID : %d, external-ID : %d)\n",
              headerInfo.rightEdge.id,
              headerInfo.rightEdge.tileId,
              headerInfo.rightEdge.externalId);
   printf("Left Edge   : (ID : %d, tile-ID : %d, external-ID : %d)\n",
              headerInfo.leftEdge.id,
              headerInfo.leftEdge.tileId,
              headerInfo.leftEdge.externalId);
}

//---------------------------------------------*/
// determineKindOfEdgeRecord 
//
// Description:
//    This function will read some of the header
//    information and then determine which of the
//    4 kinds of edge records is contained in
//    the edge table (EDG file).
//---------------------------------------------*/
static kindOfEdgeRecordType determineKindOfEdgeRecord ()

{
         int       sizeOfHeader;
         const int Level_3_No_Feature_Tbl     = 338;
         const int TileRef                    = 374;
         const int Level_3_With_Feature_Tbl   = 435;
         const int Level_1_2_With_Feature_Tbl = 360;
         const int LibRefEdge                 = 299;

   rewind (edg_fd);

   sizeOfHeader = gmsReadInteger (edg_fd);

   if ( (sizeOfHeader == Level_3_No_Feature_Tbl) ||
        (sizeOfHeader == TileRef) )

      return gmsLevel_3_No_Feature_Tbl;

   else if (sizeOfHeader == Level_3_With_Feature_Tbl)

      return gmsLevel_3_With_Feature_Tbl;

   else if (sizeOfHeader == Level_1_2_With_Feature_Tbl)

      return gmsLevel_1_2_With_Feature_Tbl;

   else if (sizeOfHeader == LibRefEdge)

      return gmsLibRefEdge;

   else

      return gmsUnknownEdge;
}


//---------------------------------------------*/
// readOneHeaderRecord 
//
// Description:
//    This function will read the header
//    information at the start of a new edge
//    record.
//---------------------------------------------*/
static edgeRecordHeaderType readOneHeaderRecord
                               (kindOfEdgeRecordType kindOfEdgeRecord)

{
         edgeRecordHeaderType theAnswer;

   gmsClearMemory
      ( (char *) &theAnswer,
        sizeof(theAnswer) );

   if (kindOfEdgeRecord == gmsLevel_3_No_Feature_Tbl)
      {
       theAnswer.ID                 = gmsReadInteger (edg_fd);

       theAnswer.fgnKeyToFeatureTbl = -1;

       theAnswer.startNode          = gmsReadInteger (edg_fd);

       theAnswer.endNode            = gmsReadInteger (edg_fd);

       theAnswer.rightFace          = gmsReadIdTripletRecord (edg_fd);

       theAnswer.leftFace           = gmsReadIdTripletRecord (edg_fd);

       theAnswer.rightEdge          = gmsReadIdTripletRecord (edg_fd);

       theAnswer.leftEdge           = gmsReadIdTripletRecord (edg_fd);
      }

   else if (kindOfEdgeRecord == gmsLevel_3_With_Feature_Tbl)
      {
       theAnswer.ID                 = gmsReadInteger (edg_fd);

       theAnswer.fgnKeyToFeatureTbl = gmsReadInteger (edg_fd);

       theAnswer.startNode          = gmsReadInteger (edg_fd);

       theAnswer.endNode            = gmsReadInteger (edg_fd);

       theAnswer.rightFace          = gmsReadIdTripletRecord (edg_fd);

       theAnswer.leftFace           = gmsReadIdTripletRecord (edg_fd);

       theAnswer.rightEdge          = gmsReadIdTripletRecord (edg_fd);

       theAnswer.leftEdge           = gmsReadIdTripletRecord (edg_fd);
      }

   else if (kindOfEdgeRecord == gmsLevel_1_2_With_Feature_Tbl)
      {
       theAnswer.ID                 = gmsReadInteger (edg_fd);

       theAnswer.fgnKeyToFeatureTbl = gmsReadInteger (edg_fd);

       theAnswer.startNode          = gmsReadInteger (edg_fd);

       theAnswer.endNode            = gmsReadInteger (edg_fd);

       // theAnswer.rightFace       = leave blank */

       // theAnswer.leftFace        = leave blank */

       theAnswer.rightEdge          = gmsReadIdTripletRecord (edg_fd);

       theAnswer.leftEdge           = gmsReadIdTripletRecord (edg_fd);
      }

   else if (kindOfEdgeRecord == gmsLibRefEdge)
      {
       theAnswer.ID                 = gmsReadInteger (edg_fd);

       theAnswer.fgnKeyToFeatureTbl = -1;

       theAnswer.startNode          = gmsReadInteger (edg_fd);

       theAnswer.endNode            = gmsReadInteger (edg_fd);

       // theAnswer.rightFace       = leave blank */

       // theAnswer.leftFace        = leave blank */

       theAnswer.rightEdge          = gmsReadIdTripletRecord (edg_fd);

       theAnswer.leftEdge           = gmsReadIdTripletRecord (edg_fd);
      }
   else
      {
       printf("*** ERROR : unknown format of EDGE record.\n");
      }

   return theAnswer;
}


//---------------------------------------------*/
// getEdgeIndexFileName 
//
// Description:
//    This function will determine the name of the
//    Edge-Index-File (EDX) based on the given
//    name of the Edge-File (EDG).  It is required
//    that the caller pass in an "edxFileNameOut"
//    string that is the same size or bigger as
//    the "edgFileNameIn" string.
//---------------------------------------------*/
static void getEdgeIndexFileName
                (const char *edgFileNameIn,
                 char       *edxFileNameOut)

{
         const size_t numChars = strlen (edgFileNameIn);

   strncpy
      (edxFileNameOut,
       edgFileNameIn,
       numChars);

   // be case sensitive */
   if (edgFileNameIn[numChars - 1] == 'G')

      edxFileNameOut[numChars - 1] = 'X';

   else

      edxFileNameOut[numChars - 1] = 'x';

   edxFileNameOut[numChars] = '\0';
}
