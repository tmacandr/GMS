/*--------------------------------------------------------------------------*/
/* File : gmsAdrgTestPatchImage.h
/* Date : 03-Dec-99 : Initial Definition
/*
/* Description:
/*    Utilities to "read" the "Test Patch Image" file from a specified
/*    ADRG database.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*--------------------------------------------------------------------------*/

#ifndef ADRG_CPH

   #define ADRG_CPH

   typedef struct _gmsAdrgTestPatchImageType
      {
       int           kindOfRecord;
       int           idNum;
       unsigned char pixel;
      } gmsAdrgTestPatchImageType;


   /*-------------------------------------------*/
   /* gmsAdrgGetTestPatchImage
   /*
   /* Description:
   /*    This utility reads the specified file
   /*    that contains a "Test Patch Image".  A
   /*    pointer to a newly allocated image is
   /*    returned to the caller.  It is the caller's
   /*    responsibility to free the item by using
   /*    the utility "gmsAdrgFreeTestPatchImage"
   /*    (see below).
   /*-------------------------------------------*/
   gmsAdrgTestPatchImageType *gmsAdrgGetTestPatchImage ();


   /*-------------------------------------------*/
   /* gmsAdrgFreeTestPatchImage
   /*
   /* Description:
   /*    This utility frees a "Test Patch Image
   /*    that had been previously allocated
   /*    using "gmsAdrgGetTestPatchImage".
   /*-------------------------------------------*/
   void gmsAdrgFreeTestPatchImage
           (gmsAdrgTestPatchImageType *theTPI);


   /*-------------------------------------------*/
   /* gmsAdrgPrintTestPatchImage	
   /*
   /* Description:
   /*    This function will print the "Test Patch
   /*    Image" object to standard out.
   /*-------------------------------------------*/
   void gmsAdrgPrintTestPatchImage
           (gmsAdrgTestPatchImageType *theTPI);

#endif
