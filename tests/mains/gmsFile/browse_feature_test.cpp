//----------------------------------------------------------------------------*/
// File : browse_feature_test.c
// Date : 08-Sep-99 : initial defintion
//        17-Nov-99 : use utils to access VPF files
//        18-Nov-99 : use capital letters in VPF file names
//
// Description:
//    Console application to exercise the "Browse Feature Table" utilities.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <gmsUtilities.h>
#include <gmsBrowseFeatureTable.h>

typedef struct
   {
    browseFeatureTableType *theFT;
    char                   *featureFileName;
    gmsBrowseThematicType  theme;
    gmsBrowseFeatureType   feature;
   } testRecordType;

#define Num_Records 15


static testRecordType getRecord (int index);


int main (int  argc,
          char **argv)

{
         int            i;
         testRecordType tempRec;

   printf("_______________________________\n");
   printf("Begin : Browse Feature Test\n");

   for (i = 0; i < Num_Records; i++)
   {
       tempRec = getRecord(i);

       tempRec.theFT = gmsGetBrowseFeatureTable
                                 (tempRec.theme,
                                  tempRec.featureFileName,
                                  tempRec.feature);

       if (tempRec.theFT == NULL)
          {
           printf("*** ERROR : pointer to feature table is NULL\n");
           printf("*** ERROR : this test FAILS\n");

           exit (0);
          }

       gmsPrintBrowseFeatureTable (tempRec.theFT);

       gmsFreeBrowseFeatureTable (tempRec.theFT);
   }

   printf("End : Browse Feature Test\n");
   printf("_______________________________\n");

   return 0;
}


//--------------------------------------------*/
// getRecord
//
// Description:
//--------------------------------------------*/
static testRecordType getRecord (int index)

{
         static testRecordType testRecords[Num_Records] =
                           { { NULL,
                               NULL,
                               gmsBrowse_CO,
                               gmsBrowseArea },
                             { NULL,
                               NULL,
                               gmsBrowse_DA,
                               gmsBrowseArea },
                             { NULL,
                               NULL,
                               gmsBrowse_DN,
                               gmsBrowseArea },
                             { NULL,
                               NULL,
                               gmsBrowse_DV,
                               gmsBrowseArea },
                             { NULL,
                               NULL,
                               gmsBrowse_GR,
                               gmsBrowseArea },
                             { NULL,
                               NULL,
                               gmsBrowse_IN,
                               gmsBrowseArea },
                             { NULL,
                               NULL,
                               gmsBrowse_PO,
                               gmsBrowseArea },
                             { NULL,
                               NULL,
                               gmsBrowse_DN,
                               gmsBrowseLine },
                             { NULL,
                               NULL,
                               gmsBrowse_GR,
                               gmsBrowseLine },
                             { NULL, 
                               NULL,
                               gmsBrowse_PO,
                               gmsBrowseLine },
                             { NULL,
                               NULL,
                               gmsBrowse_PP,
                               gmsBrowsePoint },
                             { NULL,
                               NULL,
                               gmsBrowse_GR,
                               gmsBrowseText },
                             { NULL,
                               NULL,
                               gmsBrowse_IN,
                               gmsBrowseText },
                             { NULL,
                               NULL,
                               gmsBrowse_PO,
                               gmsBrowseText },
                             { NULL,
                               NULL,
                               gmsBrowse_PP,
                               gmsBrowseText } };

         static const char *testFile[Num_Records] =
                           { { "co/COAREA.AFT" },
                             { "da/DAAREA.AFT" },
                             { "dn/DNAREA.AFT" },
                             { "dv/DVAREA.AFT" },
                             { "gr/GRAREA.AFT" },
                             { "in/INAREA.AFT" },
                             { "po/POAREA.AFT" },
                             { "dn/DNLINE.LFT" },
                             { "gr/GRLINE.LFT" },
                             { "po/POLINE.LFT" },
                             { "pp/PPPOINT.PFT" },
                             { "gr/GRTEXT.TFT" },
                             { "in/INTEXT.TFT" },
                             { "po/POTEXT.TFT" },
                             { "pp/PPTEXT.TFT" } };

   testRecords[index].featureFileName =
      gmsGetBrowseFullPath (testFile[index]);

   printf("---> Feature Table: %s\n", testRecords[index].featureFileName);

   return testRecords[index];
}

