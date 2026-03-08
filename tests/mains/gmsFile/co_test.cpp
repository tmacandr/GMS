//----------------------------------------------------------------------------
// File : co_test.c
// Date : 27-Aug-99 : initial defintion
//        09-Oct-99 : Name change: "gmsSpatial.h" to "gmsSpatial.h"
//        17-Nov-99 : use utils to get paths to VPF files
//
// Description:
//    Console application to process all files defined in the "co" directory
//    of the "browse" library.  The "po" directory contains "Coverage" data.
//    Coverage data is analogous to photographic separates in conventional
//    cartography.  Coverage data is contained in a single directory and
//    contains primitives that support a specific topological level (e.g.
//    leve 0, 1, 2, or 3).
// 
//    The following items are contained in the "browse" coverage directory:
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
//         rng         - Ring Table
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsMinBoundRectTable.h>
#include <gmsEdgeTable.h>
#include <gmsSpatialIndex.h>
#include <gmsFaceTable.h>
#include <gmsFeatureClassSchemaTable.h>
#include <gmsRingTable.h>
#include <gmsValueDescriptionTable.h>


int main (int  argc,
          char **argv)

{
         minBoundRectTableType *theEBR;
         edgeTableType         *theEDG;
         spatialIndexType      *theESI;
            /*-----------*/         
         faceTableType         *theFAC; 
         minBoundRectTableType *theFBR;
         spatialIndexType      *theFSI;
            /*-----------*/
         featureClassSchemaType *theFCS;
            /*-----------*/
         intValueDescriptionTableType *theIntVDT;
            /*-----------*/
         ringTableType         *theRNG;

   printf("_______________________________\n");
   printf("Begin : Coverage Test\n");

   printf("---> Edge Bounding Rectangle:\n");

   theEBR = gmsGetMbrTable (gmsGetBrowseFullPath("co/ebr"));

   gmsPrintMbrTable (theEBR);

   gmsFreeMbrTable (theEBR);

   printf("---> Edge Table (and Edge Index):\n");

   theEDG = gmsGetEdgeTable (gmsGetBrowseFullPath("co/edg"));

   gmsPrintEdgeTable (theEDG);

   gmsFreeEdgeTable (theEDG);

   printf("---> Edge Spatial Index (what does this tell me?)\n");

   theESI = gmsGetSpatialIndex (gmsGetBrowseFullPath("co/esi"));

   gmsPrintSpatialIndex (theESI);

   gmsFreeSpatialIndex (theESI);

   printf("---> Face Bounding Rectangle\n");

   theFBR = gmsGetMbrTable (gmsGetBrowseFullPath("co/fbr"));

   gmsPrintMbrTable (theFBR);

   gmsFreeMbrTable (theFBR);

   printf("---> Face Table\n");

   theFAC = gmsGetFaceTable
               (gmsGetBrowseFullPath("co/fac"));

   gmsPrintFaceTable (theFAC);

   gmsFreeFaceTable (theFAC);

   printf("---> Face Spatial Index\n");

   theFSI = gmsGetSpatialIndex (gmsGetBrowseFullPath("co/fsi"));

   gmsPrintSpatialIndex (theFSI);

   gmsFreeSpatialIndex (theFSI);

   printf("\n\n_____*****__________*****__________*****_____\n\n");

   theIntVDT = gmsGetIntValueDescriptionTable
                  (gmsGetBrowseFullPath("co/int.vdt"));

   gmsPrintIntValueDescriptionTable (theIntVDT);

   gmsFreeIntValueDescriptionTable (theIntVDT);

   printf("\n\n_____*****__________*****__________*****_____\n\n");

   theFCS = gmsGetFeatureClassSchemaTable (gmsGetBrowseFullPath("co/fcs"));

   gmsPrintFeatureClassSchemaTable (theFCS);

   gmsFreeFeatureClassSchemaTable (theFCS);

   printf("---> Ring Table\n");

   theRNG = gmsGetRingTable (gmsGetBrowseFullPath("co/rng"));

   gmsPrintRingTable (theRNG);

   gmsFreeRingTable (theRNG);

   printf("End : Coverage Test\n");
   printf("_______________________________\n");

   return 0;
}
