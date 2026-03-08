/*----------------------------------------------------------------------------
 * File : narrative_test.c
 * Date : 04-Sep-99 : initial defintion
 *        17-Nov-99 : use utils for full path to VPF files
 *
 * Description:
 *    Console application to exercise the "Narrative" utilities of the
 *    DCW database.
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsUtilities.h>
#include <gmsNarrativeTable.h>

int main (int  argc,
          char **argv)

{
         narrativeTableType *theDOC;

   printf("_______________________________\n");
   printf("Begin : Narrative Table Test\n");

   printf("---> Narrative Table\n");

   theDOC = gmsGetNarrativeTable (gmsGetDcwFullPath("lineage.doc"));

   gmsPrintNarrativeTable (theDOC);

   gmsFreeNarrativeTable (theDOC);

   printf("End : Narrative Table Test\n");
   printf("_______________________________\n");

   return 0;
}
