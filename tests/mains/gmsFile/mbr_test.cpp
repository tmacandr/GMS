//----------------------------------------------------------------------------*/
// File : mbr_test.c
// Date : 20-Sep-99 : initial defintion
//        17-Nov-99 : use utils to get path to VPF files
//
// Description:
//    Console application to exercise the "Minimum-Bounding-Rectangle"
//    utilities for the DCW database.  There are two types of MBRs:
//            1) Edge-Bounding-Rectangles (EBR)
//            2) Face-Bounding-Rectangles (FBR)
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsMinBoundRectTable.h>


#define Num_EBR_Records 17 + 7
#define Num_FBR_Records 9 + 7


static int getMainTestFromOperator ();

static int getEbrSubtestFromOperator ();

static int getFbrSubtestFromOperator ();

static void performMbrTest
              (bool isEbr,
               int  whichSubtest);

static char *get_EBR_File (int index);

static char *get_FBR_File (int index);


//---------------------------------------------*/
// Main - test driver
//
// Description:
//---------------------------------------------*/
int main (int  argc,
          char **argv)

{
         int i, j;

   printf("_______________________________________\n");
   printf("Begin : Minimum-Bounding-Rectangle Test\n");

   while (true)
      {
       i = getMainTestFromOperator ();

       if (i == 0)
          {
           j = getEbrSubtestFromOperator ();

           performMbrTest
              (true, j);
          }

       else if (i == 1)
          {
           j = getFbrSubtestFromOperator ();

           performMbrTest
              (false, j);
          }

       else
          break;
      }

   printf("End : Minimum Bounding Rectangle Test\n");
   printf("_______________________________________\n");

   return 0;
}


//---------------------------------------------*/
// getMainTestFromOperator
//
// Description:
//---------------------------------------------*/
static int getMainTestFromOperator ()

{
         int theAnswer;

   printf("Which test:\n");
   printf("-----------\n");

   printf("Enter 0 for EDGE BR\n");
   printf("Enter 1 for FACE BR\n");

   printf("Your selection (-1 to exit) = ");

   scanf ("%d", &theAnswer);

   return theAnswer;
}


//---------------------------------------------*/
// getEbrSubtestFromOperator
//
// Description:
//---------------------------------------------*/
static int getEbrSubtestFromOperator ()

{
         int  i;
         int  theAnswer;
         char *fileName;

   printf("Select EBR test:\n");
   printf("----------------\n");

   for (i = 0; i < Num_EBR_Records; i++)
      {
       fileName = get_EBR_File (i);

       printf("%d) %s\n",
              i, &(fileName[9]));
      }

   printf("Your selection (-1 to exit) = ");

   scanf ("%d", &theAnswer);

   return theAnswer;
}


//---------------------------------------------*/
// getFbrSubtestFromOperator
//
// Description:
//---------------------------------------------*/
static int getFbrSubtestFromOperator ()

{
         int  i;
         int  theAnswer;
         char *fileName;

   printf("Select FBR test:\n");
   printf("----------------\n");

   for (i = 0; i < Num_FBR_Records; i++)
      {
       fileName = get_FBR_File (i);

       printf("%d) %s\n",
              i, &(fileName[9]));
      }

   printf("Your selection (-1 to exit) = ");

   scanf ("%d", &theAnswer);

   return theAnswer;
}


//---------------------------------------------*/
// performMbrTest
//
// Description:
//---------------------------------------------*/
static void performMbrTest
              (bool isEbr,
               int  whichSubtest)
{
         char                  *nameOfFile;
         minBoundRectTableType *theMbr;

   if (isEbr)
      nameOfFile = get_EBR_File(whichSubtest);
   else
      nameOfFile = get_FBR_File(whichSubtest);

   printf("---> MBR File : %s\n", nameOfFile);

   theMbr = gmsGetMbrTable (nameOfFile);

   gmsPrintMbrTable (theMbr);

   gmsFreeMbrTable (theMbr);
}


//---------------------------------------------*/
// get_EBR_File
//
// Description:
//---------------------------------------------*/
static char *get_EBR_File (int index)

{
      static char *fileName;
      static const char *ebrTestRecords[Num_EBR_Records] =
                           { "vg/g/h/23/ebr",
                             "ut/r/j/21/ebr",
                             "ts/r/j/21/ebr",
                             "tileref/ebr",
                             "rr/r/j/21/ebr",
                             "rd/r/j/21/ebr",
                             "pp/r/j/21/ebr",
                             "po/r/j/21/ebr",
                             "ph/r/j/21/ebr",
                             "of/r/j/21/ebr",
                             "libref/ebr",
                             "lc/r/j/21/ebr",
                             "hy/r/j/21/ebr",
                             "hs/r/j/21/ebr",
                             "dq/r/j/21/ebr",
                             "dn/r/j/21/ebr",
                             "cl/r/j/12/ebr",
                             // BROWSE */
                             "po/ebr",
                             "in/ebr",
                             "gr/ebr",
                             "dv/ebr",
                             "dn/ebr",
                             "da/ebr",
                             "co/ebr"
                            };

   if (index < 17)
      fileName = gmsGetDcwFullPath(ebrTestRecords[index]);

   else
      fileName = gmsGetBrowseFullPath (ebrTestRecords[index]);

   return fileName;
}


//---------------------------------------------*/
// get_EBR_File
//
// Description:
//---------------------------------------------*/
static char *get_FBR_File (int index)

{
      char        *fileName;
      static const char *fbrTestRecords[Num_FBR_Records] =
                           { "vg/g/h/23/fbr",
                             "tileref/fbr",
                             "pp/r/j/21/fbr",
                             "po/r/j/21/fbr",
                             "lc/r/j/21/fbr",
                             "hy/r/j/21/fbr",
                             "dq/r/j/21/fbr",
                             "dn/r/j/21/fbr",
                             "cl/f/h/12/fbr",
                             // BROWSE */
                             "po/fbr",
                             "in/fbr",
                             "gr/fbr",
                             "dv/fbr",
                             "dn/fbr",
                             "da/fbr",
                             "co/fbr"
                            };

   if (index < 9)
      fileName = gmsGetDcwFullPath(fbrTestRecords[index]);

   else
      fileName = gmsGetBrowseFullPath (fbrTestRecords[index]);

   return fileName;
}
