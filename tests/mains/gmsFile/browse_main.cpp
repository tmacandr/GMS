//----------------------------------------------------------------------------*/
// File : browse_main.cpp
// Date : 04-Jul-99 : initial definition
//        01-Aug-99 : converted S/W to utilities on port to Sun Solaris
//        11-Aug-99 : made a SUN specific version
//        18-Nov-99 : use capital letters in VPF file names
//
// Description:
//    Console application to process the data in the DCW "browse"
//    directory.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsDbHeaderTable.h>
#include <gmsLibAttribTable.h>
#include <gmsGeoReferenceTable.h>
#include <gmsCoverageAttribTable.h>
#include <gmsDataQualityTable.h>
#include <gmsIndexFile.h>
#include <gmsLibHeaderTable.h>
#include <gmsFaceTable.h>
#include <gmsSpatialIndex.h>
#include <gmsNodeTable.h>

int main (int  argc,
          char *argv)

{
         DbHeaderTableType    *theDBH;
         LibAttribTableType   *theLAT;
         GeoRefTableType      *theGRT;
         CovAttribTableType   *theCAT;
         indexTableType       *theDQX;
         DataQualityTableType *theDQT;
         libHeaderTableType   *theLHT;
         faceTableType        *theFAC;
         spatialIndexType     *theFSI;
         nodeTableType        *theEND;
             //--------*/
         unsigned char        before    = 0xAA;
         unsigned char        after     = 0x00;
         unsigned int         beforeInt = 0xAAAAAAAA;
         unsigned int         *afterInt;

   printf("Size of Long Int = %d\n", sizeof(long int) );

   printf("    ___________\n");
   printf("---> DBH :\n");

   theDBH = gmsGetDbHeaderTable (getDhtFile());

   gmsPrintDbHeaderTable (theDBH);

   printf("---> LAT :\n");

   theLAT = gmsGetLibAttribTable (getLatFile());

   gmsPrintLibAttribTable (theLAT);

   printf("---> GRT\n");

   theGRT = gmsGetGeoReferenceTable (gmsGetBrowseFullPath("GRT"));

   gmsPrintGeoReferenceTable (theGRT);

   printf("---> CAT\n");

   theCAT = gmsGetCoverageAttribTable (gmsGetBrowseFullPath("CAT"));

   gmsPrintCoverageAttribTable (theCAT);

   printf("---> Dqx\n");

   theDQX = gmsGetIndexFile (gmsGetBrowseFullPath("DQX"));

   gmsPrintIndexRecord (theDQX);

   gmsFreeIndexFile (theDQX);

   printf("---> DQT\n");

   theDQT = gmsGetDataQualityTable (gmsGetBrowseFullPath("DQT"));

   gmsPrintDataQualityTable (theDQT);

   printf("---> LHT\n");

   theLHT = gmsGetLibHeaderTable (gmsGetBrowseFullPath("LHT"));

   gmsPrintLibHeaderTable (theLHT);

   printf("---> Utilities test\n");

   printf("   *** Before : %x\n", before);

   after = *gmsSwapBitPattern
                    (&before,
                     sizeof(before));

   printf("   *** After  : %x\n", after);

   gmsFreeSwappedBits ();

   printf("   *** Before Int : %x\n", beforeInt);

   afterInt = (unsigned int *) gmsSwapBitPattern
                                  ((unsigned char *) &beforeInt,
                                   sizeof(beforeInt));

   printf("   *** After Int  : %x\n", *afterInt);

   gmsFreeSwappedBits ();

   printf("_______________\n");
   printf("Face Table\n");

   theFAC = gmsGetFaceTable
               (gmsGetBrowseFullPath("PO/FAC"));

   gmsPrintFaceTable (theFAC);

   gmsFreeFaceTable (theFAC);

   printf("__________________\n");
   printf("Face Spatial Index\n");

   theFSI = gmsGetSpatialIndex (gmsGetBrowseFullPath("PO/FSI"));

   gmsPrintSpatialIndex (theFSI);

   gmsFreeSpatialIndex (theFSI);

   printf("_______________\n");
   printf("Node Table\n");

   theEND = gmsGetNodeTable
               (gmsGetBrowseFullPath("PP/END"),
                gmsBrowseNode);

   gmsPrintNodeTable (theEND);

   gmsFreeNodeTable (theEND);

   printf("    ___________\n");

   return 0;
}
