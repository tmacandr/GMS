/*----------------------------------------------------------------------------*/
/* File : gmsUtilities.cpp
/* Date : 03-Dec-99 : Initial defintion
/*
/* Description:
/*    Utilities that support the ADRG portion of the "Geographic Map System"
/*    (GMS) being developed for MS Project at CSUN.
/*
/*    Reference:
/*        1) Mil-A-89007  : ARC Digitized Raster Graphics
/*        2) Mil-Std-2411 : Raster Gridded Product Format
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmsAdrgUtilities.h>


/*--------------------------*/
/* Declare Local Constants
/*--------------------------*/

static const char *ADRG_File_Root = { "/temp/fakeAdrg" };

static const int  bitsPerByte = 8;


/*--------------------------*/
/* Declare Local Variables
/*--------------------------*/


/*--------------------------*/
/* Declare Local Functions
/*--------------------------*/
static void convertStringToUpperCase (char *string);




/*---------------------------------------------*/
/* gmsGetAdrgFullPath
/*
/* Description:
/*    This utility catenates the specified string
/*    with the string that is the root path to
/*    the ADRG files.  A new string is returned
/*    to the caller.
/*
/*    NOTE:
/*       The caller must NOT free the string
/*       returned.  Subsequent calls overwrite
/*       the previous string.
/*---------------------------------------------*/
char *gmsGetAdrgFullPath
                  (const char *whichFile)

{
      static char tempPath[512];
      static char fullPath[1024];

   sprintf(tempPath, "%s", whichFile);

   #ifdef IS_UNIX

      convertStringToUpperCase (tempPath);

   #endif

   sprintf(fullPath, "%s/%s",
           ADRG_File_Root,
           tempPath);

   return fullPath;
}


         /*--------------------------*/
         /*   static local functions */
         /*--------------------------*/


/*---------------------------------------------*/
/* convertStringToUpperCase
/*
/* Description:
/*    This utility converts the characters of
/*    the specified string to upper case.  This
/*    is done because names of the files and
/*    directories on a ADRG CD-ROM are capitalized.
/*    On a "MS Windows" box, it is not a problem
/*    if names of files are capitalized or not.
/*    But on a UNIX box, this matters.  Hence,
/*    this routine ensures file/directory
/*    names are capitalized.
/*---------------------------------------------*/
static void convertStringToUpperCase (char *string)

{
      int strLength;
      int i;
      int tempChar;

   strLength = strlen (string);

   for (i = 0; i < strLength; i++)
      {
       tempChar = toupper (string[i]);

       string[i] = (char) tempChar;
      }
}
