//----------------------------------------------------------------------------*/
// File : node_test.c
// Date : 20-Sep-99 : initial defintion
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    Console application to exercise the "Node" utilities of the DCW
//    database.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsNodeTable.h>

#define Num_Records 1 + 11 + 1



static int getTestFromOperator ();

static char *getFile (int index);


//---------------------------------------------*/
// Main - test driver
//
// Description:
//---------------------------------------------*/
int main (int  argc,
          char **argv)

{
         int           i;
         gmsNodeType   kindOfNode;
         nodeTableType *tempNodeTbl;
         char          *fileName;

   printf("_______________________________\n");
   printf("Begin : Node Test\n");

   while (true)
      {
       i = getTestFromOperator ();

       if ( (i < 0) || (i >= Num_Records) ) break;

       if (i == 0)
          kindOfNode = gmsBrowseNode;
       else if (i == 12)
          kindOfNode = gmsGazetteNode;
       else
          kindOfNode = gmsThemeNode;

       fileName = getFile (i);

       printf("---> Node File : %s\n", fileName);

       tempNodeTbl = gmsGetNodeTable
                        (fileName,
                         kindOfNode);

       gmsPrintNodeTable (tempNodeTbl);

       gmsFreeNodeTable (tempNodeTbl);
      }

   printf("End : Node Test\n");
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

   printf("Select Node test:\n");
   printf("-----------------\n");

   for (i = 0; i < Num_Records; i++)
      {
       fileName = getFile(i);

       printf("%d) %s\n",
              i, &(fileName[9]) );
      }

   printf("Your selection (-1 to exit) = ");

   scanf ("%d", &theAnswer);

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
      static const char *testFiles[Num_Records] =
                           { { "pp/end" },  // <--- browse */
                             { "ae/r/j/21/end" },
                             { "cl/z/c/12/end" },
                             { "dn/r/j/21/end" },
                             { "ds/y/c/22/end" },
                             { "hy/z/c/12/end" },
                             { "hs/z/c/12/end" },
                             { "lc/u/a/11/end" },
                             { "of/y/c/22/end" },
                             { "po/z/c/12/end" },
                             { "pp/r/j/21/end" },
                             { "ts/r/j/21/end" },
                             { "gazette/end" } };

   if (index > 0)
      fileName = gmsGetDcwFullPath(testFiles[index]);

   else
      fileName = gmsGetBrowseFullPath (testFiles[index]);

   return fileName;
}
