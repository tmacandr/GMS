//----------------------------------------------------------------------------*/
// File : browse_dv.c
// Date : 22-Nov-99 : initial definition
//
// Description:
//    Console application to process (all?) files defined in the BROWSE
//    "DV" directory.  The "DV" directory contains "Data Volume" data
//    which is information about ???.  The DV theme (i.e. directory)
//    contains the following files:
//
//                 DVAREA.AFT
//                 EBR
//                 EDG
//                 EDX
//                 ESI
//                 FAC
//                 FBR
//                 FCS
//                 FSI
//                 INT.VDT
//                 RNG
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <gmsUtilities.h>
#include <gmsBrowseFeatureTable.h>
#include <gmsThematicIndex.h>
#include <gmsFeatureClassSchemaTable.h>
#include <gmsValueDescriptionTable.h>
#include <gmsMinBoundRectTable.h>
#include <gmsEdgeTable.h>


int main (int  argc,
          char **argv)

{
         browseFeatureTableType       *ptrToFT;
         featureClassSchemaType       *theFCS;
         intValueDescriptionTableType *intVDT;
         minBoundRectTableType        *ptrToMbr;
         edgeTableType                *ptrToEdg;


   printf("__________________________________\n");
   printf("Begin : Browse Data Volume Test\n");

   //------------------------*/
   // DVAREA.AFT - Area feature table
   //------------------------*/
   printf("---> Area Feature Table:\n");

   ptrToFT = gmsGetBrowseFeatureTable
                   (gmsBrowse_DV,
                    gmsGetBrowseFullPath("dv/DVAREA.AFT"),
                    gmsBrowseArea);

   gmsPrintBrowseFeatureTable (ptrToFT);

   gmsFreeBrowseFeatureTable (ptrToFT);

   //------------------------*/
   // Feature class schema table
   //------------------------*/
   printf("---> Feature Class Schema Table\n");

   theFCS = gmsGetFeatureClassSchemaTable
               (gmsGetBrowseFullPath("DV/FCS"));

   gmsPrintFeatureClassSchemaTable (theFCS);

   gmsFreeFeatureClassSchemaTable (theFCS);

   //------------------------*/
   // Integer value description table
   //------------------------*/
   printf("---> Integer - Value Description Table\n");

   intVDT = gmsGetIntValueDescriptionTable
               (gmsGetBrowseFullPath("dv/INT.VDT"));

   gmsPrintIntValueDescriptionTable (intVDT);

   gmsFreeIntValueDescriptionTable (intVDT);

   //------------------------*/
   // EBR - Edge bounding rectangle
   //------------------------*/
   ptrToMbr = gmsGetMbrTable (gmsGetBrowseFullPath("DV/EBR"));

   gmsPrintMbrTable (ptrToMbr);

   gmsFreeMbrTable (ptrToMbr);

   //------------------------*/
   // EDG - Edge table
   // (and EDX - Edge index)
   //------------------------*/
   ptrToEdg = gmsGetEdgeTable (gmsGetBrowseFullPath("dv/edg"));

   gmsPrintEdgeTable (ptrToEdg);

   gmsFreeEdgeTable (ptrToEdg);

   //------------------------*/
   // Edge spatial index (don't care?)
   //------------------------*/

   //------------------------*/
   // EBR - Edge bounding rectangle
   //------------------------*/
   ptrToMbr = gmsGetMbrTable (gmsGetBrowseFullPath("DV/EBR"));

   gmsPrintMbrTable (ptrToMbr);

   gmsFreeMbrTable (ptrToMbr);

   printf("End : Land Cover Test\n");
   printf("________________________________\n");

   return 0;
}
