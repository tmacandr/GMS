//----------------------------------------------------------------------------*/
// File : face_test.c
// Date : 21-Sep-99 : initial defintion
//        17-Nov-99 : use utils for full path to VPF files
//
// Description:
//    Console application to exercise the "Face-Table" utilities.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsFaceTable.h>


static int getTestFromOperator ();

static void performFaceTest
              (int whichSubtest);


#define Num_Face_Files  9 + 7

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
   printf("Begin : Face Table Test\n");

   while (true)
      {
       i = getTestFromOperator ();

       if ( (i < 0) || (i >= Num_Face_Files) ) break;

       performFaceTest (i);
      }

   printf("End : Face Table Test\n");
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

   for (i = 0; i < Num_Face_Files; i++)
      {
       fileName = getFile (i);

       printf("%d) %s\n", i, &fileName[9]);
      }

   printf("Your selection = ");

   scanf ("%d", &theAnswer);

   return theAnswer;
}


//---------------------------------------------*/
// performFaceTest
//
// Description:
//---------------------------------------------*/
static void performFaceTest
              (int whichSubtest)

{
         faceTableType *facTbl;
         char          *fileName;

   fileName = getFile (whichSubtest);

   printf("---> Face Table : %s\n", fileName);

   facTbl = gmsGetFaceTable (fileName);

   gmsPrintFaceTable (facTbl);

   gmsFreeFaceTable (facTbl);
}


//---------------------------------------------*/
// getFile
//
// Description:
//---------------------------------------------*/
static char *getFile (int index)

{
      char        *fileName;
      static const char *faceFileNames[Num_Face_Files] =
                  { "vg/g/h/23/fac",
                    "tileref/fac",
                    "pp/p/d/23/fac",
                    "po/n/g/22/fac",
                    "lc/p/d/23/fac",
                    "hy/p/d/23/fac",
                    "dq/p/d/23/fac",
                    "dn/p/d/23/fac",
                    "cl/p/f/22/fac",
                    // BROWSE */
                    "po/fac",
                    "in/fac",
                    "gr/fac",
                    "dv/fac",
                    "dn/fac",
                    "da/fac",
                    "co/fac" };

   if (index < 9)
      fileName = gmsGetDcwFullPath(faceFileNames[index]);

   else
      fileName = gmsGetBrowseFullPath (faceFileNames[index]);

   return fileName;
}
