//----------------------------------------------------------------------------*/
// File : schema_class_test.c
// Date : 19-Sep-99 : initial definition
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    A test driver targeted specifically for the utilities that manipulate
//    the "Feature Class Schema Tables".
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsFeatureClassSchemaTable.h>


#define Num_Records 28


static int getSelectedTestFromOperator ();

static char *getFile (int index);


//---------------------------------------------*/
// main
//
// Description:
//---------------------------------------------*/
int main (int  argc,
          char *argv)

{
         int                    i;
         featureClassSchemaType *theFCS;
         char                   *fileName;

   printf("_________________________________\n");
   printf("Begin : Feature Class Schema Test\n");

   while (true)
      {
       i = getSelectedTestFromOperator ();

       if ( (i < 0) || (i >= Num_Records) ) break;

       fileName = getFile (i);

       printf("---> Schema Tbl : %s\n", fileName);

       theFCS = gmsGetFeatureClassSchemaTable (fileName);

       gmsPrintFeatureClassSchemaTable (theFCS);

       gmsFreeFeatureClassSchemaTable (theFCS);
      }

   printf("End : Feature Class Schem Test\n");
   printf("______________________________\n");

   return 0;
}


//---------------------------------------------*/
// getSelectedTestFromOperator
//
// Description:
//---------------------------------------------*/
static int getSelectedTestFromOperator ()

{
         int  theAnswer;
         int  i;
         char *fileName;

   printf("-----------------------\n");
   printf("Which test do you want?\n");

   for (i = 0; i < Num_Records; i++)
      {
       fileName = getFile (i);

       printf("%d) %s        \t\t ",
              i, &(fileName[9]) );

       if (i % 2) printf("\n");
      }

   printf("Enter number (-1 to exit): ");

   scanf("%d", &theAnswer);

   printf("\n");

   return theAnswer;
}


//---------------------------------------------*/
// getFile
//
// Description:
//---------------------------------------------*/
static char *getFile (int index)

{
      char        *fileName;
      static char *testFiles[Num_Records] =
                           { { "vg/fcs" },
                             { "ut/fcs" },
                             { "ts/fcs" },
                             { "tileref/fcs" },
                             { "rr/fcs" },
                             { "rd/fcs" },
                             { "pp/fcs" },
                             { "po/fcs" },
                             { "ph/fcs" },
                             { "of/fcs" },
                             { "libref/fcs" },
                             { "lc/fcs" },
                             { "hy/fcs" },
                             { "hs/fcs" },
                             { "gazette/fcs" },
                             { "ds/fcs" },
                             { "dq/fcs" },
                             { "dn/fcs" },
                             { "cl/fcs" },
                             { "ae/fcs" },
                             // BROWSE */
                             { "pp/fcs" },
                             { "po/fcs" },
                             { "in/fcs" },
                             { "gr/fcs" },
                             { "dv/fcs" },
                             { "dn/fcs" },
                             { "da/fcs" },
                             { "co/fcs" } };

   if (index < 20)
      fileName = gmsGetDcwFullPath(testFiles[index]);

   else
      fileName = gmsGetBrowseFullPath (testFiles[index]);

   return fileName;
}
