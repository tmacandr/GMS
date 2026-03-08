//----------------------------------------------------------------------------*/
// File : edge_test_2.c
// Date : 15-Dec-99 : initial definition.  Evolve from first edge test
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

#define  numFiles  9 + 1 + 1 + 1

static char *getFile (int index);

static void performEdgeTest
                  (int whichFile);

static int getFileFromOperator();


int main (int  argc,
          char *argv)

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
                       {"po/n/a/11/edg"},
                       {"po/n/a/12/edg"},
                       {"po/n/a/13/edg"},
                       {"po/n/a/21/edg"},
                       {"po/n/a/22/edg"},
                       {"po/n/a/23/edg"},
                       {"po/n/a/31/edg"},
                       {"po/n/a/32/edg"},
                       {"po/n/a/33/edg"},
                       {"po/n/g/13/edg"},
                       {"po/n/g/21/edg"},
                       // LibRef    */
                       {"libref/edg"} };

   fileName = gmsGetDcwFullPath(edgeTables[index]);

   return fileName;
}