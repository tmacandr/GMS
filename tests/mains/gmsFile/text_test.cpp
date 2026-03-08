//----------------------------------------------------------------------------*/
// File : text_test.c
// Date : 20-Aug-99 : initial defintion
//        21-Sep-99 : apply to both browse and themes
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    Console application to exercise the "text" utilities.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsTextTable.h>

#define Num_Txt_Files  12 + 4

static char *getFile (int index);

static int getTestFromOperator ();

static void performTextTest
              (int whichSubtest);


//-------------------------------------*/
// main
//
// Description:
//    Main test driver.
//-------------------------------------*/
int main (int  argc,
          char **argv)

{
         int i;

   printf("_______________________________\n");
   printf("Begin : Text Table Test\n");

   while (true)
      {
       i = getTestFromOperator ();

       if ( (i < 0) || (i >= Num_Txt_Files) ) break;

       performTextTest (i);
      }

   printf("End : Text Table Test\n");
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

   for (i = 0; i < Num_Txt_Files; i++)
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
// performTextTest
//
// Description:
//---------------------------------------------*/
static void performTextTest
              (int whichSubtest)

{
         char          *fileName;
         textTableType *txtTbl;

   fileName = getFile (whichSubtest);

   printf("---> Text Table : %s\n", fileName);

   txtTbl = gmsGetTextTable (fileName);

   gmsPrintTextTable (txtTbl);

   gmsFreeTextTable (txtTbl);
}


//---------------------------------------------*/
// getFile
//
// Description:
//---------------------------------------------*/
static char *getFile (int index)

{
      char        *fileName;
      static const char *testFiles[Num_Txt_Files] =
                              { { "cl/r/j/21/txt"},
                                { "dq/p/f/23/txt"},
                                { "dn/r/j/21/txt"},
                                { "lc/r/j/21/txt"},
                                { "of/m/j/11/txt"},
                                { "ph/r/j/11/txt"},
                                { "po/r/j/21/txt"},
                                { "pp/r/j/21/txt"},
                                { "rr/r/e/32/txt"},
                                { "rd/r/j/21/txt"},
                                { "ts/r/j/12/txt"},
                                { "ut/r/j/21/txt"},
                                // BROWSE */
                                { "gr/txt"},
                                { "in/txt"},
                                { "po/txt"},
                                { "pp/txt"} };

   if (index < 12)
      fileName = gmsGetDcwFullPath(testFiles[index]);

   else
      fileName = gmsGetBrowseFullPath (testFiles[index]);

   return fileName;
}