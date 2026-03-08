//----------------------------------------------------------------------------*/
// File : vdt_test.c
// Date : 25-Aug-99 : initial defintion
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    Console application to exercise the "Value-Description-Table" utilities.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsValueDescriptionTable.h>


#define Num_Int_Records  8 + 17  // 8 browse + 17 themes */
#define Num_Char_Records 1 + 1

#define Int_VDT_Test     0
#define Char_VDT_Test    1

static int getMainTestFromOperator ();

static int getIntVdtSubtestFromOperator ();

static int getCharVdtSubtestFromOperator ();

static void performIntVdtTest
              (int whichSubtest);

static void performCharVdtTest
              (int whichSubtest);

static char *getIntFile (int index);

static char *getCharFile (int index);


//---------------------------------------------*/
// Main - test driver
//
// Description:
//---------------------------------------------*/
int main (int  argc,
          char *argv)

{
         int whichMainTest;
         int whichSubtest;

   printf("_______________________________\n");
   printf("Begin : Value Description Test\n");

   while (true)
      {
       whichMainTest = getMainTestFromOperator ();

       if (whichMainTest == Int_VDT_Test)
          {
           whichSubtest = getIntVdtSubtestFromOperator();

           if ( (whichSubtest < 0) || (whichSubtest >= Num_Int_Records) )
              break;

           performIntVdtTest (whichSubtest);
          }

       else if (whichMainTest == Char_VDT_Test)
          {
           whichSubtest = getCharVdtSubtestFromOperator();

           if ( (whichSubtest < 0) || (whichSubtest >= Num_Char_Records) )
              break;

           performCharVdtTest (whichSubtest);
          }

       else
          break;
      }

   printf("End : Value Description Test\n");
   printf("_______________________________\n");

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

   printf("Select VDT main test:\n");
   printf("---------------------\n");
   printf("Enter %d for INT VDT\n", Int_VDT_Test);
   printf("Enter %d for CHAR VDT\n", Char_VDT_Test);
   printf("Your selection = ");

   scanf ("%d", &theAnswer);

   printf("\n");

   return theAnswer;
}


//---------------------------------------------*/
// getIntVdtSubtestFromOperator
//
// Description:
//---------------------------------------------*/
static int getIntVdtSubtestFromOperator ()

{
         int  i;
         int  theAnswer;
         char *fileName;

   for (i = 0; i < Num_Int_Records; i++)
      {
       fileName = getIntFile (i);

       printf("%d) %s\n", i, &(fileName[11]));
      }

   printf("Your selection = ");

   scanf ("%d", &theAnswer);

   printf("\n");

   return theAnswer;
}


//---------------------------------------------*/
// getCharVdtSubtestFromOperator
//
// Description:
//---------------------------------------------*/
static int getCharVdtSubtestFromOperator ()

{
         int  i;
         int  theAnswer;
         char *fileName;

   for (i = 0; i < Num_Char_Records; i++)
      {
       fileName = getCharFile (i);

       printf("%d) %s \t ", i, fileName);

       if (i % 2) printf("\n");
      }

   printf("Your selection = ");

   scanf ("%d", &theAnswer);

   printf("\n");

   return theAnswer;
}


//---------------------------------------------*/
// performIntVdtTest
//
// Description:
//---------------------------------------------*/
static void performIntVdtTest
              (int whichSubtest)

{
         intValueDescriptionTableType *intVDT;
         char                         *fileName;

   printf("---> Integer Value Description Table\n");

   fileName = getIntFile (whichSubtest);

   intVDT = gmsGetIntValueDescriptionTable (fileName);

   gmsPrintIntValueDescriptionTable (intVDT);

   gmsFreeIntValueDescriptionTable (intVDT);
}


//---------------------------------------------*/
// performCharVdtTest
//
// Description:
//---------------------------------------------*/
static void performCharVdtTest
              (int whichSubtest)

{
         charValueDescriptionTableType *charVDT;
         bool                          isBrowse = (whichSubtest == 0);
         char                          *fileName;

   printf("---> Character Value Description Table\n");

   fileName = getCharFile (whichSubtest);

   charVDT = gmsGetCharValueDescriptionTable
                (fileName,
                 isBrowse);

   gmsPrintCharValueDescriptionTable (charVDT);

   gmsFreeCharValueDescriptionTable (charVDT);
}


//---------------------------------------------*/
// getIntFile
//
// Description:
//---------------------------------------------*/
static char *getIntFile (int index)

{
      char        *fileName;
      static char *intTestFiles[Num_Int_Records] =
                           { // BROWSE */
                             { "pp/int.vdt" },
                             { "po/int.vdt" },
                             { "in/int.vdt" },
                             { "co/int.vdt" },
                             { "gr/int.vdt" },
                             { "dv/int.vdt" },
                             { "dn/int.vdt" },
                             { "da/int.vdt" },
                             // REGIONAL library */
                             { "vg/int.vdt" },
                             { "ut/int.vdt" },
                             { "ts/int.vdt" },
                             { "rr/int.vdt" },
                             { "rd/int.vdt" },
                             { "pp/int.vdt" },
                             { "po/int.vdt" },
                             { "ph/int.vdt" },
                             { "of/int.vdt" },
                             { "lc/int.vdt" },
                             { "hy/int.vdt" },
                             { "hs/int.vdt" },
                             { "ds/int.vdt" },
                             { "dq/int.vdt" },
                             { "dn/int.vdt" },
                             { "cl/int.vdt" },
                             { "ae/int.vdt" } };

   if (index > 7)
      fileName = gmsGetDcwFullPath(intTestFiles[index]);

   else
      fileName = gmsGetBrowseFullPath (intTestFiles[index]);

   return fileName;
}


//---------------------------------------------*/
// getCharFile
//
// Description:
//---------------------------------------------*/
static char *getCharFile (int index)

{
      char        *fileName;
      static char *charTestFiles[Num_Char_Records] =
                           { { "gr/char.vdt"  },
                             { "po/char.vdt" } };

   if (index == 1)
      fileName = gmsGetDcwFullPath(charTestFiles[index]);

   else
      fileName = gmsGetBrowseFullPath (charTestFiles[index]);


   return fileName;
}
