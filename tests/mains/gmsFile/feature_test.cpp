//----------------------------------------------------------------------------*/
// File : feature_test.c
// Date : 25-Aug-99 : initial defintion
//        15-Sep-99 : still working it due to redesign of interface
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    Console application to exercise the "Feature Table" utilities.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsFeatureTable.h>

typedef struct
   {
    featureTableType *theFT;
    char             *featureFileName;
    gmsThematicType  theme;
    gmsFeatureType   feature;
   } testRecordType;


#define Num_Records 46

static int getSelectedTestFromOperator ();

static testRecordType getRecord (int index);


//---------------------------------------------*/
// Main - test driver
//
// Description:
//---------------------------------------------*/
int main (int  argc,
          char **argv)

{
         int            i;
         testRecordType tempRec;

   printf("_______________________________\n");
   printf("Begin : Feature Test\n");

   while (true)
      {
       i = getSelectedTestFromOperator ();

       if ( (i < 0) || (i >= Num_Records) ) break;

       tempRec = getRecord (i);

       printf("---> Feature Tbl : %s\n", tempRec.featureFileName);

       tempRec.theFT = gmsGetFeatureTable
                                (tempRec.theme,
                                 tempRec.featureFileName,
                                 tempRec.feature);

       gmsPrintFeatureTable (tempRec.theFT);

       gmsFreeFeatureTable (tempRec.theFT);
      }

   printf("End : Feature Test\n");
   printf("_______________________________\n");

   return 0;
}


//---------------------------------------------*/
// getSelectedTestFromOperator
//
// Description:
//---------------------------------------------*/
static int getSelectedTestFromOperator ()

{
         int            theAnswer;
         int            i;
         testRecordType tempRec;

   printf("Which test do you want?\n");

   for (i = 0; i < Num_Records; i++)
      {
       tempRec = getRecord (i);

       printf("%d) %s \t \t",
              i, &(tempRec.featureFileName[19]));

       if (i % 2)
          printf("\n");
      }

   printf("Enter number: ");

   scanf("%d", &theAnswer);

   printf("\n");

   return theAnswer;
}



//---------------------------------------------*/
// getRecord
//
// Description:
//---------------------------------------------*/
static testRecordType getRecord (int index)

{
      static testRecordType testRecords[Num_Records] =
                           { { NULL,
                               NULL,
                               gms_VG,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_UT,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_UT,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_TS,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_TS,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_TS,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_TileRef,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_RR,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_RR,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_RD,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_RD,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_PP,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_PP,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_PP,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_PO,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_PO,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_PO,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_PO,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_PH,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_PH,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_OF,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_OF,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_OF,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_LibRef,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_LC,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_LC,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_LC,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_HY,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_HY,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_HY,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_HS,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_HS,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_Gazetteer,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_DS,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_DQ,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_DQ,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_DQ,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_DN,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_DN,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_DN,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_DN,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_CL,
                               gmsText },
                             { NULL,
                               NULL,
                               gms_CL,
                               gmsPoint },
                             { NULL,
                               NULL,
                               gms_CL,
                               gmsLine },
                             { NULL,
                               NULL,
                               gms_CL,
                               gmsArea },
                             { NULL,
                               NULL,
                               gms_AE,
                               gmsPoint } };
      static const char *fileNames[Num_Records] =
                           { { "vg/vgarea.aft"},
                             { "ut/utline.lft"},
                             { "ut/uttext.tft"},
                             { "ts/tstext.tft"},
                             { "ts/tspoint.pft"},
                             { "ts/tsline.lft"},
                             { "tileref/tileref.aft"},
                             { "rr/rrtext.tft"},
                             { "rr/rrline.lft"},
                             { "rd/rdtext.tft"},
                             { "rd/rdline.lft"},
                             { "pp/pptext.tft"},
                             { "pp/pppoint.pft"},
                             { "pp/pparea.aft"},
                             { "po/potext.tft"},
                             { "po/popoint.pft"},
                             { "po/poline.lft"},
                             { "po/poarea.aft"},
                             { "ph/phtext.tft"},
                             { "ph/phline.lft"},
                             { "of/oftext.tft"},
                             { "of/ofpoint.pft"},
                             { "of/ofline.lft"},
                             { "libref/libref.lft"},
                             { "lc/lctext.tft"},
                             { "lc/lcpoint.pft"},
                             { "lc/lcarea.aft"},
                             { "hy/hypoint.pft"},
                             { "hy/hyline.lft"},
                             { "hy/hyarea.aft"},
                             { "hs/hspoint.pft"},
                             { "hs/hsline.lft"},
                             { "gazette/Gazette.pft"},
                             { "ds/dspoint.pft"},
                             { "dq/dqtext.tft"},
                             { "dq/dqline.lft"},
                             { "dq/dqarea.aft"},
                             { "dn/dntext.tft"},
                             { "dn/dnpoint.pft"},
                             { "dn/dnline.lft"},
                             { "dn/dnarea.aft"},
                             { "cl/cltext.tft"},
                             { "cl/clpoint.pft"},
                             { "cl/clline.lft"},
                             { "cl/clarea.aft"},
                             { "ae/aepoint.pft"} };

   testRecords[index].featureFileName = gmsGetDcwFullPath(fileNames[index]);

   return testRecords[index];
}
