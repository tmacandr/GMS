//----------------------------------------------------------------------------*/
// File : ring_test.c
// Date : 21-Sep-99 : initial defintion
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    Console application to exercise the "Ring-Table" utilities.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsRingTable.h>


static int getTestFromOperator ();

static void performRingTest
              (int whichSubtest);


#define Num_Ring_Files  9 + 7

static char *getFile (int index);


//---------------------------------------------*/
// Main - test driver
//
// Description:
//---------------------------------------------*/
int main (int  argc,
          char **argv)

{
         int i;

   printf("_______________________________\n");
   printf("Begin : Ring Table Test\n");

   while (true)
      {
       i = getTestFromOperator ();

       if ( (i < 0) || (i >= Num_Ring_Files) ) break;

       performRingTest (i);
      }

   printf("End : Ring Table Test\n");
   printf("_______________________________\n");

   return 0;
}


//---------------------------------------------*/
// getTestFromOperator
//
// Description:
//---------------------------------------------*/
static int getTestFromOperator ()

{
         int  i;
         int  theAnswer;
         char *fileName;

   for (i = 0; i < Num_Ring_Files; i++)
      {
       fileName = getFile (i);

       printf("%d) %s\n",
              i, &(fileName[9]));
      }

   printf("Your selection = ");

   scanf ("%d", &theAnswer);

   return theAnswer;
}


//---------------------------------------------*/
// performRingTest
//
// Description:
//---------------------------------------------*/
static void performRingTest
              (int whichSubtest)

{
         ringTableType *rngTbl;
         char          *fileName;

   fileName = getFile (whichSubtest);

   printf("---> Ring Table : %s\n", fileName);

   rngTbl = gmsGetRingTable (fileName);

   gmsPrintRingTable (rngTbl);

   gmsFreeRingTable (rngTbl);
}


//---------------------------------------------*/
// getFile
//
// Description:
//---------------------------------------------*/
static char *getFile (int index)

{
      static char *fileName;
      static const char *ringFileNames[Num_Ring_Files] =
                  { "vg/g/h/23/rng",
                    "tileref/rng",
                    "pp/p/d/23/rng",
                    "po/n/g/22/rng",
                    "lc/p/d/23/rng",
                    "hy/p/d/23/rng",
                    "dq/p/d/23/rng",
                    "dn/p/d/23/rng",
                    "cl/p/f/32/rng",
                    // BROWSE */
                    "po/rng",
                    "in/rng",
                    "gr/rng",
                    "dv/rng",
                    "dn/rng",
                    "da/rng",
                    "co/rng" };

   if (index < 9)
      fileName = gmsGetDcwFullPath(ringFileNames[index]);

   else
      fileName = gmsGetBrowseFullPath (ringFileNames[index]);

   return fileName;
}
