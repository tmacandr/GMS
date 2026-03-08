/*----------------------------------------------------------------------------
 * File : narrative_test.c
 * Date : 03-Dec-99 : initial defintion
 *
 * Description:
 *    Console application to exercise the "Test Patch Image" utilities
 *    of the ADRG database.
 *
 *    Reference:
 *        1) Mil-A-89007
 *        2) Mil-Std-2411
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsAdrgTestPatchImage.h>


int main (int  argc,
          char *argv)

{
         gmsAdrgTestPatchImageType *ptrToTPI;

   printf("__________________________________\n");
   printf("Begin : ADRG Test Patch Image Test\n");

   ptrToTPI = gmsAdrgGetTestPatchImage ();

   gmsAdrgPrintTestPatchImage (ptrToTPI);

   gmsAdrgFreeTestPatchImage (ptrToTPI);

   printf("End : ADRG Test Patch Image Test\n");
   printf("__________________________________\n");

   return 0;
}
