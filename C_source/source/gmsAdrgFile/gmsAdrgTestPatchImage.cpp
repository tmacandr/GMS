/*--------------------------------------------------------------------------
 * File : gmsAdrgTestPatchImage.cpp
 * Date : 03-Dec-99 : Initial Definition
 *
 * Description:
 *    Utilities to "read" the "Test Patch Image" file from a specified
 *    ADRG database.
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *--------------------------------------------------------------------------*/


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmsUtilities.h>
#include <gmsAdrgUtilities.h>
#include <gmsAdrgTestPatchImage.h>


//---------------------------*/
//     Local Variables       */
//---------------------------*/
static FILE *tpi_fd = (FILE *) NULL;


//---------------------------*/
// Declare Local Subprograms */
//---------------------------*/

static void buildTestPatchImage
               (gmsAdrgTestPatchImageType *theTPI);

static void readPastFormatInformation ();

static void debug_showContentsOfFile ();


//-------------------------------------------*/
// gmsAdrgGetTestPatchImage
//
// Description:
//    This utility reads the specified file
//    that contains a "Test Patch Image".  A
//    pointer to a newly allocated image is
//    returned to the caller.  It is the caller's
//    responsibility to free the item by using
//    the utility "gmsAdrgFreeTestPatchImage"
//    (see below).
//-------------------------------------------*/
gmsAdrgTestPatchImageType *gmsAdrgGetTestPatchImage ()

{

         char                      *filePath;
         const char                readOnlyMode[] = { "rb" };
         gmsAdrgTestPatchImageType *theTPI;
         int                       numBytes;

   filePath = gmsGetAdrgFullPath ("TESTPA01.CPH");

   tpi_fd = fopen(filePath,
                  readOnlyMode);

   if (tpi_fd == (FILE *) NULL)
      {
       printf("---> ADRG ERROR : unable to open file: %s\n", filePath);

       return (gmsAdrgTestPatchImageType *) NULL;
      }

   numBytes = sizeof(gmsAdrgTestPatchImageType);

   theTPI = (gmsAdrgTestPatchImageType *) malloc (numBytes);

   gmsClearMemory
      ((char *) theTPI,
       numBytes );

   buildTestPatchImage (theTPI);

   fclose (tpi_fd);

   return theTPI;
}


//-------------------------------------------*/
// gmsAdrgFreeTestPatchImage
//
// Description:
//    This utility frees a "Test Patch Image
//    that had been previously allocated
//    using "gmsAdrgGetTestPatchImage".
//-------------------------------------------*/
void gmsAdrgFreeTestPatchImage
           (gmsAdrgTestPatchImageType *theTPI)

{
   free (theTPI);
}


//-------------------------------------------*/
// gmsAdrgPrintTestPatchImage	
//
// Description:
//    This function will print the "Test Patch
//    Image" object to standard out.
//-------------------------------------------*/
void gmsAdrgPrintTestPatchImage
           (gmsAdrgTestPatchImageType *theTPI)

{
   if (theTPI == NULL)
      return;

   printf("______________________________\n");
   printf("Test Patch Image:\n");
   
   printf("______________________________\n");
}


     //---------------------*/
     //   Local Subprograms */
     //---------------------*/


//-------------------------------------------*/
// buildTestPatchImage
//
// Description:
//    This function will read the actual data
//    from the "Test Patch Image" file.  The
//    data read will be used to populate the
//    attributes of the object.
//-------------------------------------------*/
static void buildTestPatchImage
              (gmsAdrgTestPatchImageType *theTPI)

{
   debug_showContentsOfFile ();

   if (true) return;

   readPastFormatInformation ();

   theTPI->kindOfRecord = gmsReadInteger (tpi_fd);
}


//-------------------------------------------*/
// readPastFormatInformation
//
// Description:
//    This function will read the format data
//    located at the front of the Database
//    Header Table file.
//-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (tpi_fd);

   tempChar = fgetc (tpi_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (tpi_fd);

           if ( (char) tempChar == ';')
              return;
          }

       tempChar = fgetc (tpi_fd);
      }

   printf("---> ERROR - unexpected EOF reading ADRG TESTPA01.CPH file.\n");
}



//-------------------------------------------*/
// printChar
//
// Description:
//-------------------------------------------*/
static void printChar (int theChar, int &lineCount)

{
   static const char *ctrlChars[32] = { {"<ctrl-space>"},
                                        {"<ctrl-A>"},
                                        {"<ctrl-B>"},
                                        {"<ctrl-C>"},
                                        {"<ctrl-D>"},
                                        {"<ctrl-E>"},
                                        {"<ctrl-F>"},
                                        {"<ctrl-G>"},
                                        {"<ctrl-H>"},
                                        {"<ctrl-I>"},
                                        {"<ctrl-J>"},
                                        {"<ctrl-K>"},
                                        {"<ctrl-L>"},
                                        {"<ctrl-M>"},
                                        {"<ctrl-N>"},
                                        {"<ctrl-O>"},
                                        {"<ctrl-P>"},
                                        {"<ctrl-Q>"},
                                        {"<ctrl-R>"},
                                        {"<ctrl-S>"},
                                        {"<ctrl-T>"},
                                        {"<ctrl-U>"},
                                        {"<ctrl-V>"},
                                        {"<ctrl-W>"},
                                        {"<ctrl-X>"},
                                        {"<ctrl-Y>"},
                                        {"<ctrl-Z>"},
                                        {"<ctrl-[>"},
                                        {"<ctrl-bkslash>"},
                                        {"<ctrl-]>"},
                                        {"<ctrl-~>"},
                                        {"<ctrl-?>"} };

   if ( iscntrl(theChar) )
      {
       printf("%s\n", ctrlChars[theChar]);

       lineCount = 0;
      }
   else if (theChar > 127)
      {
       printf("NON ASCIII byte : %x\n", theChar);

       lineCount++;
      }
   else
      printf("%c ", theChar);
}


//-------------------------------------------*/
// debug_showContentsOfFile
//
// Description:
//    This is a debug utility.  It reads the
//    contents of the "Test Patch Image" file
//    as a stream of bytes and prints those
//    values to stdout.  The file-descriptor
//    is returned to the top of the file at
//    the end of this routine.
//-------------------------------------------*/
static void debug_showContentsOfFile ()

{
         int tempChar;
         int lineCount = 0;
         int byteCount = 0;

   rewind (tpi_fd);

   tempChar = fgetc (tpi_fd);

   while (tempChar != EOF)
      {
       byteCount++;

       lineCount++;

       printChar(tempChar, lineCount);

       if (lineCount == 30)
          {
           printf("\n");

           lineCount = 0;
          }

       tempChar = fgetc (tpi_fd);

       if (byteCount == 2048)
          {
           printf("*************************\n");
           printf("*** BYTE COUNT = 2048 ***\n");
           printf("*************************\n");

           byteCount = 0;
          }
      }

   printf("\n");

   rewind (tpi_fd);
}
