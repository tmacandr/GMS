//----------------------------------------------------------------------------*/
// File : po_test.c
// Date : 16-Aug-99 : initial defintion
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    Console application to process all files defined in the "po" directory
//    of the "browse" library.  The "po" directory contains "Politcal/Ocean"
//    geographic data.  The following items are contained:
//
//         ebr         - Edge Bounding Rectangle Table
//         edg         - Edge Table
//         edx         - Edge Index Table
//         esi         - Edge Spatial Index Table
//         fac         - Face Table
//         fbr         - Face Bounding Rectangle Table
//         fcs         - Feature Class Schema Table
//         fsi         - Feature Spatial Index Table
//         int.vdt     - Integer Value Description Table
//         poarea.aft  - Area Feature Table
//         poline.lft  - Line Feature Table
//         potext.tft  - Text Feature Table
//         potext.tfx  - Text Feature Index
//         rng         - Ring Table
//         txt         - Text Table
//         txx         - Text Index Table
//         tsi         - Text Spatial Index Table
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsMinBoundRectTable.h>
#include <gmsEdgeTable.h>
#include <gmsIndexFile.h>
#include <gmsSpatialIndex.h>
#include <gmsFaceTable.h>
#include <gmsFeatureClassSchemaTable.h>
#include <gmsTextTable.h>
#include <gmsRingTable.h>

int main (int  argc,
          char **argv)

{
         minBoundRectTableType *theEBR;
         edgeTableType         *theEDG;
         spatialIndexType      *theESI;
            //-----------*/         
         faceTableType         *theFAC; 
         minBoundRectTableType *theFBR;
            //-----------*/
         featureClassSchemaType *theFCS;
            //-----------*/
         textTableType         *theTXT;
         spatialIndexType      *theTSI;
         ringTableType         *theRNG;

   printf("_______________________________\n");
   printf("Begin : Political-Oceans\n");

   printf("---> Edge Bounding Rectangle:\n");

   theEBR = gmsGetMbrTable (gmsGetBrowseFullPath("po/ebr"));

   gmsPrintMbrTable (theEBR);

   gmsFreeMbrTable (theEBR);

   printf("---> Edge Table (and Edge Index):\n");

   theEDG = gmsGetEdgeTable
                   (gmsGetBrowseFullPath("po/edg"));

   gmsPrintEdgeTable (theEDG);

   gmsFreeEdgeTable (theEDG);

   printf("---> Edge Spatial Index (what does this tell me?)\n");

   theESI = gmsGetSpatialIndex (gmsGetBrowseFullPath("po/esi"));

   gmsPrintSpatialIndex (theESI);

   gmsFreeSpatialIndex (theESI);

   printf("---> Face Bounding Rectangle\n");

   theFBR = gmsGetMbrTable (gmsGetBrowseFullPath("po/fbr"));

   gmsPrintMbrTable (theFBR);

   gmsFreeMbrTable (theFBR);

   printf("---> Face Table\n");

   theFAC = gmsGetFaceTable
               (gmsGetBrowseFullPath("po/fac"));

   gmsPrintFaceTable (theFAC);

   gmsFreeFaceTable (theFAC);

   printf("\n\n_____*****__________*****__________*****_____\n\n");

   theFCS = gmsGetFeatureClassSchemaTable (gmsGetBrowseFullPath("po/fcs"));

   gmsPrintFeatureClassSchemaTable (theFCS);

   gmsFreeFeatureClassSchemaTable (theFCS);

   printf("---> Text Table\n");

   theTXT = gmsGetTextTable
               (gmsGetBrowseFullPath("po/txt"));

   gmsPrintTextTable (theTXT);

   gmsFreeTextTable (theTXT);

   printf("---> Text Spatial Index (what does this tell me?)\n");

   theTSI = gmsGetSpatialIndex (gmsGetBrowseFullPath("po/tsi"));

   gmsPrintSpatialIndex (theTSI);

   gmsFreeSpatialIndex (theTSI);

   printf("---> Ring Table\n");

   theRNG = gmsGetRingTable (gmsGetBrowseFullPath("po/rng"));

   gmsPrintRingTable (theRNG);

   gmsFreeRingTable (theRNG);

   printf("End : Political-Oceans\n");
   printf("_______________________________\n");

   return 0;
}
