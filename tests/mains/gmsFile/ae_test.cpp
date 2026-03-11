//----------------------------------------------------------------------------*/
// File : ae_test.c
// Date : 05-Sep-99 : initial defintion
//        17-Nov-99 : use utils to get file paths
//        18-Nov-99 : use capital letters for names of VPF files
//        22-Nov-99 : read at least one of each file in the AE directory
//
// Description:
//    Console application to process all files defined in the "ae" directory
//    The "ae" directory contains "Aeronautical" geographic data which is
//    information about airports.  The AE theme (i.e. directory) contains the
//    following files:
//
//                 AEPOINT.DOC - Narrative table
//                 AEPOINT.PFT - Point feature table
//                 AEPOINT.PTI - Thematic index
//                 FCS         - Feature class schema table
//                 INT.VDT     - Integer value description table
//
//    The tile subdirectories all contain the following two types of files:
//
//                 END         - Entity node table
//                 NSI         - Node spatial index
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


//---------------------------------------------------------
// FUNCTION: main
//
// DESCRIPTION:
//    Main entry point to exercise "Aeronautical" (AE)
//    geographic data.
//---------------------------------------------------------
int main (int  argc,
          char **argv)

{
         narrativeTableType           *theDOC;
         featureTableType             *thePFT;
         thematicIndexType            *thePTI;
         featureClassSchemaType       *theFCS;
         intValueDescriptionTableType *intVDT;
         nodeTableType                *nodeTbl;

   printf("__________________________________\n");
   printf("Begin : Aeronautical Coverage Test\n");

   //------------------------*/
   // Narrative Table
   //------------------------*/
   printf("---> Narrative Table:\n");

   theDOC = gmsGetNarrativeTable
               (gmsGetDcwFullPath("AE/AEPOINT.DOC"));

   if ( ! theDOC )
   {
       printf("***> ERROR - get narrative table fail\n");
       return EXIT_FAILURE;
   }

   gmsPrintNarrativeTable (theDOC);

   gmsFreeNarrativeTable (theDOC);

   //------------------------*/
   // Point feature table
   //------------------------*/
   printf("---> Point Feature Table:\n");

   thePFT = gmsGetFeatureTable
                   (gms_AE,
                    gmsGetDcwFullPath("AE/AEPOINT.PFT"),
                    gmsPoint);

   gmsPrintFeatureTable (thePFT);

   gmsFreeFeatureTable (thePFT);

   //------------------------*/
   // Point thematic index
   //------------------------*/
   printf("---> Point Thematic Index\n");

   thePTI = gmsGetThematicIndex
               (gmsGetDcwFullPath("AE/AEPOINT.PTI"));

   gmsPrintThematicIndex (thePTI);

   gmsFreeThematicIndex (thePTI);

   //------------------------*/
   // Feature class schema table
   //------------------------*/
   printf("---> Feature Class Schema Table\n");

   theFCS = gmsGetFeatureClassSchemaTable
               (gmsGetDcwFullPath("AE/FCS"));

   gmsPrintFeatureClassSchemaTable (theFCS);

   gmsFreeFeatureClassSchemaTable (theFCS);

   //------------------------*/
   // Integer value description table
   //------------------------*/
   printf("---> Integer - Value Description Table\n");

   intVDT = gmsGetIntValueDescriptionTable
               (gmsGetDcwFullPath("AE/INT.VDT"));

   gmsPrintIntValueDescriptionTable (intVDT);

   gmsFreeIntValueDescriptionTable (intVDT);

   //------------------------*/
   // Entity node table
   //------------------------*/
   printf("---> Entity node table:\n");

   nodeTbl = gmsGetNodeTable
                (gmsGetDcwFullPath("AE/D/J/31/END"),
                 gmsThemeNode);

   gmsPrintNodeTable (nodeTbl);

   gmsFreeNodeTable (nodeTbl);

   //------------------------*/
   // Node spatial index = "don't care"?
   //------------------------*/

   printf("End : Aeronautical Coverage Test\n");
   printf("________________________________\n");

   return 0;
}
