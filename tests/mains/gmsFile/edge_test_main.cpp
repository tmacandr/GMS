//----------------------------------------------------------------------------*/
// File : edge_test_main.c
// Date : 06-Aug-99 : initial definition.  The "browse_main.cpp" was too big.
//        11-Aug-99 : made a version to run on SUN.
//        12-Oct-99 : expanded due to major mods to "gmsEdgeTable"
//        17-Nov-99 : use utils for path to VPF files
//
// Description:
//    Console application to process "edge" data from the DCW library.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsIndexFile.h>
#include <gmsEdgeTable.h>

#define  numFiles  34 + 9 * 4

static char *getFile (int index);

static void performEdgeTest
                  (int whichFile);

static int getFileFromOperator();


int main (int  argc,
          char **argv)

{
         int whichFile;

   printf("_______________________________\n");
   printf("Begin : edge_test\n");

   while (true)
      {
       whichFile = getFileFromOperator();

       if ( (whichFile < 0) || (whichFile >= numFiles) ) break;

       performEdgeTest(whichFile);
      }

   printf("End : edge_test\n");
   printf("_______________________________\n");

   return 0;
}


//---------------------------------------------*/
// performEdgeTest 
//
// Description:
//---------------------------------------------*/
static void performEdgeTest
                  (int whichFile)

{
         edgeTableType *theEDG;
         char          *fileName;

   fileName = getFile (whichFile);

   printf("---> Edge Table %s:\n", fileName);

   theEDG = gmsGetEdgeTable (fileName);

   gmsPrintEdgeTable (theEDG);

   gmsFreeEdgeTable (theEDG);
}


//---------------------------------------------*/
// getFileFromOperator 
//
// Description:
//---------------------------------------------*/
static int getFileFromOperator()

{
         int  theSelection;
         int  i;
         char *fileName;

   printf("____________________________________\n");
   printf("Which edge file do you want to test?\n");

   for (i = 0; i < numFiles; i++)
      {
       fileName = getFile (i);

       printf("%d) %s\n", i, &fileName[9]);
      }

   printf("Enter your selection: ");

   scanf("%d", &theSelection);

   printf("\n");

   return theSelection;
}


//---------------------------------------------*/
// getFile 
//
// Description:
//---------------------------------------------*/
static char *getFile (int index)

{
      char              *fileName;
      static const char *edgeTables[numFiles] =
                     {
                       // Level 3 - NO feature tbl */
                       {"po/edg"},
                       {"dv/edg"},
                       {"da/edg"},
                       {"gr/edg"},
                       {"in/edg"},
                       {"dn/edg"},
                       {"co/edg"},
                       {"libref/edg"},
                       {"vg/g/h/23/edg"},
                       {"pp/q/j/21/edg"},
                       {"lc/r/j/12/edg"},
                       {"tileref/edg"},
                       // Level 3 - WITH feature tbl */
                       {"po/u/c/22/edg"},


                       {"po/r/e/11/edg"},
                       {"po/r/e/12/edg"},
                       {"po/r/e/13/edg"},
                       {"po/r/e/21/edg"},
                       {"po/r/e/22/edg"},
                       {"po/r/e/23/edg"},
                       {"po/r/e/31/edg"},
                       {"po/r/e/32/edg"},
                       {"po/r/e/33/edg"},

                       {"po/m/f/11/edg"},
                       {"po/m/f/12/edg"},
                       {"po/m/f/13/edg"},
                       {"po/m/f/21/edg"},
                       {"po/m/f/22/edg"},
                       {"po/m/f/23/edg"},
                       {"po/m/f/31/edg"},
                       {"po/m/f/32/edg"},
                       {"po/m/f/33/edg"},

                       {"po/m/g/11/edg"},
                       {"po/m/g/12/edg"},
                       {"po/m/g/13/edg"},
                       {"po/m/g/21/edg"},
                       {"po/m/g/22/edg"},
                       {"po/m/g/23/edg"},
                       {"po/m/g/31/edg"},
                       {"po/m/g/32/edg"},
                       {"po/m/g/33/edg"},

                       {"po/n/f/11/edg"},
                       {"po/n/f/12/edg"},
                       {"po/n/f/13/edg"},
                       {"po/n/f/21/edg"},
                       {"po/n/f/22/edg"},
                       {"po/n/f/23/edg"},
                       {"po/n/f/31/edg"},
                       {"po/n/f/32/edg"},
                       {"po/n/f/33/edg"},

                       {"po/n/g/11/edg"},
                       {"po/n/g/12/edg"},
                       {"po/n/g/13/edg"},
                       {"po/n/g/21/edg"},
                       {"po/n/g/22/edg"},
                       {"po/n/g/23/edg"},
                       {"po/n/g/31/edg"},
                       {"po/n/g/32/edg"},
                       {"po/n/g/33/edg"},

                       {"hy/r/h/31/edg"},
                       {"dq/r/h/31/edg"},
                       {"dn/r/h/31/edg"},
                       {"cl/q/j/21/edg"},
                       // Level 1-2 */
                       {"ut/n/j/13/edg"},
                       {"ts/n/j/13/edg"},
                       {"rr/n/j/13/edg"},
                       {"rd/n/j/13/edg"},
                       {"ph/u/a/11/edg"},
                       {"of/q/h/23/edg"},
                       {"hs/r/e/11/edg"},
                       // LibRef    */
                       {"libref/edg"} };

   if (index < 8)

      fileName = gmsGetBrowseFullPath(edgeTables[index]);

   else

      fileName = gmsGetDcwFullPath(edgeTables[index]);

   return fileName;
}