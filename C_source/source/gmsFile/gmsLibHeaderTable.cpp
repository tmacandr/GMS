//----------------------------------------------------------------------------*/
// File : gmsLibHeaderTable.c
// Date : 26-Jul-99 : Initial definition
//        01-Aug-99 : Convert to utilities on port to Sun Solaris
//        05-Oct-99 : Clean-up due to code inspection
//
// Description:
//    Utilities to "read" the "Library Header Table" file of the Digital
//    Chart of the World (DCW).  The "Library Header Table" contains the
//    data set, source and security information for the library.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsLibHeaderTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


//---------------------------*/
//     Local Variables       */
//---------------------------*/
static FILE *lht_fd = (FILE *) NULL;


//---------------------------*/
// Declare Local Subprograms */
//---------------------------*/
static void readPastFormatInformation ();

static void buildLibHeaderTblRecord
               (libHeaderTableType *theLHT);

static void debugReadFile();


//-------------------------------------------*/
// gmsGetLibHeaderTable
//
// Description:
//    This utility reads the file that contains
//    the "Library Header Table".  A pointer
//    to a newly allocated table is returned to
//    the caller.  It is the caller's responsibility
//    to free the item by using the utility
//    "gmsFreeLibHeaderTable" (see below).
//---------------------------------------------*/
libHeaderTableType *gmsGetLibHeaderTable
                                 (const char *filePath)

{
         const char         readOnlyMode[] = { "rb" };
         libHeaderTableType *theLHT;

   if (filePath == NULL)
      return (libHeaderTableType *) NULL;

   lht_fd = fopen(filePath,
                  readOnlyMode);

   if (lht_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open LHT file: %s\n", filePath);

       return (libHeaderTableType *) NULL;
      }

   //debugReadFile (); */

   readPastFormatInformation ();

   theLHT = (libHeaderTableType *) malloc(sizeof(libHeaderTableType));

   gmsClearMemory
      ((char *) theLHT,
       sizeof(libHeaderTableType) );

   buildLibHeaderTblRecord (theLHT);

   fclose (lht_fd);

   return theLHT;
}


//-------------------------------------------*/
// gmsFreeLibHeaderTable
//
// Description:
//    This utility frees a "Library Header
//    Table" that had been previously allocated
//    using "gmsGetLibHeaderTable".
//---------------------------------------------*/
void gmsFreeLibHeaderTable
                  (libHeaderTableType *theLHT)

{
   free (theLHT);
}


//-------------------------------------------*/
// gmsPrintLibHeaderTable	
//
// Description:
//    This function will print the "Lht" object
//    to stdout.
//-------------------------------------------*/
void gmsPrintLibHeaderTable
        (libHeaderTableType *theLHT)

{
   printf("______________________________\n");
   printf("Library Header Table:\n");

   if (theLHT == NULL)
      return;

   printf("   ---> ID             = %d\n",
          theLHT->ID);
   printf("   ---> Product Type   = %s\n",
          theLHT->productType);
   printf("   ---> Library Name   = %s\n",
          theLHT->libraryName);
   printf("   ---> Description    = %s\n",
          theLHT->description);
   printf("   ---> Data Struc Code= %c\n",
          theLHT->dataStructCode);
   printf("   ---> Scale          = %d\n",
          theLHT->scale);
   printf("   ---> Source Series  = %s\n",
          theLHT->sourceSeries);
   printf("   ---> Source ID      = %s\n",
          theLHT->sourceID);
   printf("   ---> Source Edition = %s\n",
          theLHT->sourceEdition);
   printf("   ---> Source Name    = %s\n",
          theLHT->sourceName);
   printf("   ---> Source Date    = %s\n",
          theLHT->sourceDate);
   printf("   ---> Security Class = %c\n",
          theLHT->securityClassif);
   printf("   ---> Downgrading    = %s\n",
          theLHT->downgrading);
   printf("   ---> Downgrade Date = %s\n",
          theLHT->downgradeDate);
   printf("   ---> Releasability  = %s\n",
          theLHT->releasability);
   printf("______________________________\n");
}


     //---------------------*/
     //   Local Subprograms */
     //---------------------*/


//-------------------------------------------*/
// readPastFormatInformation
//
// Description:
//    This function will read the format data
//    located at the front of the Library
//    Header Table file.
//-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (lht_fd);

   tempChar = fgetc (lht_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (lht_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (lht_fd);
      }
}


//-------------------------------------------*/
// buildLibHeaderTblRecord
//
// Description:
//    This function will read the actual data
//    from the Library Header Table file.  The
//    data read will be used to populate the
//    attributes of the object.
//-------------------------------------------*/
static void buildLibHeaderTblRecord
               (libHeaderTableType *theLHT)

{
   theLHT->ID  = gmsReadInteger (lht_fd);

   fgets(theLHT->productType,
         sizeof(theLHT->productType),
         lht_fd);

   fgets(theLHT->libraryName,
         sizeof(theLHT->libraryName),
         lht_fd);

   fgets(theLHT->description,
         sizeof(theLHT->description),
         lht_fd);

   theLHT->dataStructCode = (char ) fgetc(lht_fd);

   theLHT->scale = (unsigned) gmsReadInteger (lht_fd);

   fgets(theLHT->sourceSeries,
         sizeof(theLHT->sourceSeries),
         lht_fd);

   fgets(theLHT->sourceID,
         sizeof(theLHT->sourceID),
         lht_fd);

   fgets(theLHT->sourceEdition,
         sizeof(theLHT->sourceEdition),
         lht_fd);

   fgets(theLHT->sourceName,
         sizeof(theLHT->sourceName),
         lht_fd);

   fgets(theLHT->sourceDate,
         sizeof(theLHT->sourceDate),
         lht_fd);

   theLHT->securityClassif = (char ) fgetc(lht_fd);

   fgets(theLHT->downgrading,
         sizeof(theLHT->downgrading),
         lht_fd);

   fgets(theLHT->downgradeDate,
         sizeof(theLHT->downgradeDate),
         lht_fd);

   fgets(theLHT->releasability,
         sizeof(theLHT->releasability),
         lht_fd);
}


//-------------------------------------------*/
// debugReadFile
//
// Description:
//    Debug utility to read the contents of the
//    "Lht" file.
//-------------------------------------------*/
static void debugReadFile()

{
         int tempChar;

   rewind (lht_fd);

   readPastFormatInformation ();

   tempChar = fgetc (lht_fd);

   while (tempChar != EOF)
      {
       printf("%c", (char) tempChar);

       tempChar = fgetc (lht_fd);
      }

   printf("\n");

   rewind (lht_fd);
}
