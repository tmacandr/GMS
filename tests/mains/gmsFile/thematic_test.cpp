//----------------------------------------------------------------------------*/
// File : thematic_test.c
// Date : 02-Sep-99 : initial defintion
//        17-Nov-99 : use utils to get full path to VPF files
//
// Description:
//    Console application to exercise the "Thematic Index" utilities of the
//    DCW database.  There are five types of thematic indexes.  They are:
//
//                    1) Area Thematic Index
//                    2) Complex Thematic Index
//                    3) Line Thematic Index
//                    4) Point Thematic Index
//                    5) Text Thematic Index
//
//    A thematic index is used in conjunction with an "attribute" table.
//    There are two types of indexes, depending upon the attribute table
//    being used:
//
//                    (a) an inverted list
//                    (b) a bit array
//
//    An "inverted list" is used for "categorical data" or for data with few
//    distinct values.  A "bit array" is used for data in a column that has
//    all unique values.  Each bit inn the array representsa row in the indexed
//    table.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsThematicIndex.h>

int main (int  argc,
          char **argv)

{
         thematicIndexType *theTI;

   printf("_______________________________\n");
   printf("Begin : Thematic Index Test\n");

   printf("---> Area Thematic Index Table\n");

   theTI = gmsGetThematicIndex
                  (gmsGetDcwFullPath("cl/clarea.ati"));

   gmsPrintThematicIndex (theTI);

   gmsFreeThematicIndex (theTI);

   printf("---> Line Thematic Index Table\n");

   theTI = gmsGetThematicIndex
                  (gmsGetDcwFullPath("cl/clline.lti"));

   gmsPrintThematicIndex (theTI);

   gmsFreeThematicIndex (theTI);

   printf("---> Point Thematic Index Table\n");

   theTI = gmsGetThematicIndex
                  (gmsGetDcwFullPath("cl/clpoint.pti"));

   gmsPrintThematicIndex (theTI);

   gmsFreeThematicIndex (theTI);

   printf("---> Text Thematic Index Table\n");

   theTI = gmsGetThematicIndex
                  (gmsGetDcwFullPath("cl/cltext.tti"));

   gmsPrintThematicIndex (theTI);

   gmsFreeThematicIndex (theTI);

   printf("End : Thematic Index Test\n");
   printf("_______________________________\n");

   return 0;
}
