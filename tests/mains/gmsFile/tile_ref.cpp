//----------------------------------------------------------------------------*/
// File : tile_ref.c
// Date : 10-Jan-00 : initial defintion
//
// Description:
//    Console application that accesses all the files of the "TILEREF"
//    directory.  These files are:
//
//                  EBR
//                  EDG (EDX)
//                  FAC
//                  FBR
//                  FCS
//                  RNG
//                  TILEREF.AFT
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsMinBoundRectTable.h>
#include <gmsEdgeTable.h>
#include <gmsFaceTable.h>
#include <gmsFeatureClassSchemaTable.h>
#include <gmsRingTable.h>
#include <gmsFeatureTable.h>


static const int Num_Tests = 7;

static int getTestSelection ();

static void performTest
               (int whichTest);



//-------------------------------------*/
// main
//
// Description:
//    Main test driver.
//-------------------------------------*/
int main (int  argc,
          char **argv)

{
         int whichTest = 1;

   printf("_______________________________\n");
   printf("Begin : Tile-Ref Test\n");

   while ( (whichTest >= 0) && (whichTest <= Num_Tests) )
      {
       whichTest = getTestSelection ();

       performTest (whichTest);
      }

   printf("End : Tile-Ref Test\n");
   printf("_______________________________\n");

   return 0;
}

//---------------------------------------------*/
// getTestSelection
//
// Description:
//---------------------------------------------*/
static int getTestSelection ()

{
         static const char *theTests[Num_Tests] = { {"EBR"},
                                                    {"EDG"},
                                                    {"FAC"},
                                                    {"FBR"},
                                                    {"FCS"},
                                                    {"RNG"},
                                                    {"TILEREF.AFT"} };
         int i;

   printf("________________\n");

   for (i = 0; i < Num_Tests; i++)
      printf("%d) %s\n", i, theTests[i]);

   printf("Which Test :");

   scanf("%d", &i);

   printf("\n");

   return i;
}


//---------------------------------------------*/
// performTest
//
// Description:
//    Accesses and prints each of the files from
//    the tile-ref directory.
//---------------------------------------------*/
static void performTest
               (int whichTest)

{
         minBoundRectTableType  *mbrPtr;
         edgeTableType          *edgTblPtr;
         faceTableType          *facTblPtr;
         featureClassSchemaType *fcs;
         featureTableType       *ftrTbl;
         ringTableType          *rngTblPtr;

   switch (whichTest)
      {
       case 0:
          {
           printf("_________________\n");
           printf("Edge Bounding Rectangle:\n");

           mbrPtr = gmsGetMbrTable (gmsGetDcwFullPath("tileref/ebr"));

           gmsPrintMbrTable (mbrPtr);

           gmsFreeMbrTable (mbrPtr);

           break;
          }

       case 1:
          {
           printf("_________________\n");
           printf("Edge Table:\n");

           edgTblPtr = gmsGetEdgeTable (gmsGetDcwFullPath("tileref/edg"));

           gmsPrintEdgeTable (edgTblPtr);

           gmsFreeEdgeTable (edgTblPtr);

           break;
          }

       case 2:
          {
           printf("_________________\n");
           printf("Face Table:\n");

           facTblPtr = gmsGetFaceTable (gmsGetDcwFullPath("tileref/fac"));

           gmsPrintFaceTable (facTblPtr);

           gmsFreeFaceTable (facTblPtr);

           break;
          }

       case 3:
          {
           printf("_________________\n");
           printf("Face Bounding Rectangle:\n");

           mbrPtr = gmsGetMbrTable (gmsGetDcwFullPath("tileref/fbr"));

           gmsPrintMbrTable (mbrPtr);

           gmsFreeMbrTable (mbrPtr);

           break;
          }

       case 4:
          {
           printf("_________________\n");
           printf("Feature Class Schema Table:\n");
           fcs = gmsGetFeatureClassSchemaTable
                              (gmsGetDcwFullPath("tileref/fcs") );

           gmsPrintFeatureClassSchemaTable (fcs);

           gmsFreeFeatureClassSchemaTable (fcs);

           break;
          }

       case 5:
          {
           printf("_________________\n");
           printf("Ring Table:\n");

           rngTblPtr = gmsGetRingTable (gmsGetDcwFullPath("tileref/rng"));

           gmsPrintRingTable (rngTblPtr);

           gmsFreeRingTable (rngTblPtr);

           break;
          }

       case 6:
          {
           printf("_________________\n");
           printf("(Area) Featur Table:\n");

           ftrTbl = gmsGetFeatureTable
                           (gms_TileRef,
                            gmsGetDcwFullPath("tileref/tileref.aft"),
                            gmsArea);

           gmsPrintFeatureTable (ftrTbl);

           gmsFreeFeatureTable (ftrTbl);

           break;
          }

       default:
          {
           printf("No test selected\n");

           break;
          }
      }
}
