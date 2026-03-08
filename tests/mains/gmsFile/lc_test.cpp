//----------------------------------------------------------------------------*/
// File : lc_test.c
// Date : 22-Nov-99 : initial definition
//
// Description:
//    Console application to process (all?) files defined in the "lc"
//    directory.  The "lc" directory contains "Land Cover" geographic data
//    which is information about ???.  The LC theme (i.e. directory)
//    contains the following files:
//
//                 LCPOINT.DOC - Point narrative table
//                 LCAREA.DOC  - Area narrative table
//                 LCPOINT.PFT - Point feature table
//                 LCPOINT.PTI - Point thematic index
//                 LCAREA.AFT  - Area feature table
//                 LCAREA.ATI  - Area thematic index
//                 LCTEXT.TFT  - Text feature table
//                 LCTEXT.TFX  - Text feature index
//                 LCTEXT.TTI  - Text thematic index
//                 FCS         - Feature class schema table
//                 INT.VDT     - Integer value description table
//
//    The tile subdirectories all contain the following two types of files:
//
//                 EBR - Edge bounding rectangle
//                 EDG - Edge table
//                 EDX - Edge index
//                 ESI - Edge spatial index
//                 END - Entity node table
//                 NSI - Node spatial index
//                 FAC - Face table
//                 FBR - Face bounding rectangle
//                 FSI - Face spatial index
//                 RNG - Ring table
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <gmsUtilities.h>
#include <gmsNarrativeTable.h>
#include <gmsFeatureTable.h>
#include <gmsThematicIndex.h>
#include <gmsFeatureClassSchemaTable.h>
#include <gmsValueDescriptionTable.h>
#include <gmsNodeTable.h>
#include <gmsMinBoundRectTable.h>
#include <gmsEdgeTable.h>


int main (int  argc,
          char **argv)

{
         narrativeTableType           *theDOC;
         featureTableType             *ptrToFT;
         thematicIndexType            *ptrToTI;
         featureClassSchemaType       *theFCS;
         intValueDescriptionTableType *intVDT;
         nodeTableType                *nodeTbl;
         minBoundRectTableType        *ptrToMbr;
         edgeTableType                *ptrToEdg;


   printf("__________________________________\n");
   printf("Begin : Land Cover Test\n");

   //------------------------*/
   // Narrative Table
   //------------------------*/
   printf("---> Narrative Table - point:\n");

   theDOC = gmsGetNarrativeTable
               (gmsGetDcwFullPath("LC/LCPOINT.DOC"));

   gmsPrintNarrativeTable (theDOC);

   gmsFreeNarrativeTable (theDOC);

   printf("---> Narrative Table - area:\n");

   theDOC = gmsGetNarrativeTable
               (gmsGetDcwFullPath("LC/LCAREA.DOC"));

   gmsPrintNarrativeTable (theDOC);

   gmsFreeNarrativeTable (theDOC);

   //------------------------*/
   // Point feature table
   //------------------------*/
   printf("---> Point Feature Table:\n");

   ptrToFT = gmsGetFeatureTable
                   (gms_LC,
                    gmsGetDcwFullPath("LC/LCPOINT.PFT"),
                    gmsPoint);

   gmsPrintFeatureTable (ptrToFT);

   gmsFreeFeatureTable (ptrToFT);

   //------------------------*/
   // Point thematic index
   //------------------------*/
   printf("---> Point Thematic Index\n");

   ptrToTI = gmsGetThematicIndex
               (gmsGetDcwFullPath("LC/LCPOINT.PTI"));

   gmsPrintThematicIndex (ptrToTI);

   gmsFreeThematicIndex (ptrToTI);

   //------------------------*/
   // LCAREA.AFT - Area feature table
   //------------------------*/
   printf("---> Area Feature Table:\n");

   ptrToFT = gmsGetFeatureTable
                   (gms_LC,
                    gmsGetDcwFullPath("LC/LCAREA.AFT"),
                    gmsArea);

   gmsPrintFeatureTable (ptrToFT);

   gmsFreeFeatureTable (ptrToFT);

   //------------------------*/
   // LCAREA.ATI  - Area thematic index
   //------------------------*/
   printf("---> Area Thematic Index\n");

   ptrToTI = gmsGetThematicIndex
               (gmsGetDcwFullPath("LC/LCAREA.ATI"));

   gmsPrintThematicIndex (ptrToTI);

   gmsFreeThematicIndex (ptrToTI);

   //------------------------*/
   // LCTEXT.TFT  - Text feature table
   // (and LCTEXT.TFX)
   //------------------------*/
   printf("---> Text Feature Table:\n");

   ptrToFT = gmsGetFeatureTable
                   (gms_LC,
                    gmsGetDcwFullPath("LC/LCTEXT.TFT"),
                    gmsText);

   gmsPrintFeatureTable (ptrToFT);

   gmsFreeFeatureTable (ptrToFT);

   //------------------------*/
   // LCTEXT.TTI - Text thematic index (don't care?)
   //------------------------*/

   //------------------------*/
   // Feature class schema table
   //------------------------*/
   printf("---> Feature Class Schema Table\n");

   theFCS = gmsGetFeatureClassSchemaTable
               (gmsGetDcwFullPath("LC/FCS"));

   gmsPrintFeatureClassSchemaTable (theFCS);

   gmsFreeFeatureClassSchemaTable (theFCS);

   //------------------------*/
   // Integer value description table
   //------------------------*/
   printf("---> Integer - Value Description Table\n");

   intVDT = gmsGetIntValueDescriptionTable
               (gmsGetDcwFullPath("LC/INT.VDT"));

   gmsPrintIntValueDescriptionTable (intVDT);

   gmsFreeIntValueDescriptionTable (intVDT);

   //------------------------*/
   // EBR - Edge bounding rectangle
   //------------------------*/
   ptrToMbr = gmsGetMbrTable (gmsGetDcwFullPath("LC/A/A/11/EBR"));

   gmsPrintMbrTable (ptrToMbr);

   gmsFreeMbrTable (ptrToMbr);

   //------------------------*/
   // EDG - Edge table
   // (and EDX - Edge index)
   //------------------------*/
   ptrToEdg = gmsGetEdgeTable (gmsGetDcwFullPath("LC/A/A/11/edg"));

   gmsPrintEdgeTable (ptrToEdg);

   gmsFreeEdgeTable (ptrToEdg);

   //------------------------*/
   // Edge spatial index (don't care?)
   //------------------------*/

   //------------------------*/
   // Entity node table
   //------------------------*/
   printf("---> Entity node table:\n");

   nodeTbl = gmsGetNodeTable
                (gmsGetDcwFullPath("LC/K/E/12/END"),
                 gmsThemeNode);

   gmsPrintNodeTable (nodeTbl);

   gmsFreeNodeTable (nodeTbl);

   //------------------------*/
   // Node spatial index = "don't care"?
   //------------------------*/

   printf("End : Land Cover Test\n");
   printf("________________________________\n");

   return 0;
}
