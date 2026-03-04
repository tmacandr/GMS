//----------------------------------------------------------------------------*/
// File : gmsJoinTable.cpp
// Date : 26-Aug-99 : initial definition
//
// Description:
//    Utilities to read/process any "Join Table" file of the Digital Chart of
//    the World (DCW) database.  A "Join Table" is a file of n records of the
//    following format:
//
//       1) Primary Key to primitive is 32-bit integer
//
//       2) Primary key to primitive is 16-bit integer
//
//       3) Primary key to primitive is triplet-ID record
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsJoinTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------*/
//     Local Variables
//-----------------------------*/
static FILE *join_fd = (FILE *) NULL;


//-----------------------------*/
// Declare Local Subprograms
//-----------------------------*/
static void readPastFormatInformation ();

static void buildJoinTable
               (joinTableType *theJoinTbl);

static void printOneJoinRecord
               (joinRecordType joinRecord);


//---------------------------------------------*/
// gmsGetJoinTable
//
// Description:
//    This utility reads the file that contains
//    the "Join Table".  A pointer to a newly
//    allocated table is returned to the caller.
//    It is the caller's responsibility to free
//    the item by using the utility
//    "gmsFreeJoinTable" (see below).
//---------------------------------------------*/
joinTableType *gmsGetJoinTable
                     (const char *joinTableFilePath)

{
         const char    readOnlyMode[] = { "rb" };
         joinTableType *theJoinTbl;

   if (joinTableFilePath == NULL)
      return (joinTableType *) NULL;

   join_fd = fopen(joinTableFilePath,
                   readOnlyMode);

   if (join_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open RNG file: %s\n",
              joinTableFilePath);

       return (joinTableType *) NULL;
      }

   readPastFormatInformation ();

   theJoinTbl = (joinTableType *) malloc (sizeof(joinTableType));

   buildJoinTable (theJoinTbl);

   fclose (join_fd);

   return theJoinTbl;
}


//---------------------------------------------*/
// gmsFreeJoinTable
//
// Description:
//    This utility frees a "Join Table" that had
//    been previously allocated using
//    "gmsGetJoinTable".
//---------------------------------------------*/
void gmsFreeJoinTable
           (joinTableType *theJoinTable)

{
   if (theJoinTable == NULL)
      return;

   free (theJoinTable->joinRecords);

   free (theJoinTable);
}


//---------------------------------------------*/
// gmsPrintJoinTable	
//
// Description:
//    This function will print the "Join Table"
//    object to standard out.
//---------------------------------------------*/
void gmsPrintJoinTable
           (joinTableType *theJoinTable)

{
         int i;

   printf("________________________________________\n");
   printf("Join Table:\n");

   if (theJoinTable == NULL)
      return;

   printf("Num Records : %d\n", theJoinTable->numRecords);

   if (theJoinTable->joinRecords == NULL)
      return;

   for (i = 0; i < theJoinTable->numRecords; i++)
      {
       printOneJoinRecord (theJoinTable->joinRecords[i]);
      }
}


     //-----------------------*/
     //   Local Subprograms
     //-----------------------*/


//---------------------------------------------*/
// readPastFormatInformation
//
// Description:
//    This function will read the format data
//    located at the front of the "Join Table"
//    file.
//---------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (join_fd);

   tempChar = gmsReadInteger (join_fd);

   tempChar = fgetc (join_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (join_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (join_fd);
      }
}


//---------------------------------------------*/
// buildJoinTable
//
// Description:
//    This function will read the actual data
//    from the Join Table file.  The data read
//    will be used to populate the attributes of
//    the object.
//---------------------------------------------*/
static void buildJoinTable
               (joinTableType *theJoinTbl)

{
         int numBytes = 0;
         int numRecords;
         int tempChar = '\0';
         int index;

   while ( tempChar != EOF )
      {
       tempChar = fgetc(join_fd);

       numBytes++;
      }

   numRecords = numBytes / sizeof(joinRecordType);

   rewind (join_fd);

   readPastFormatInformation ();

   numBytes = numRecords * sizeof(joinRecordType);

   theJoinTbl->joinRecords = (joinRecordType *) malloc (numBytes);

   for (index = 0; index < numRecords; index++)
      {
       theJoinTbl->joinRecords[index].ID = gmsReadInteger (join_fd);

       theJoinTbl->joinRecords[index].featureKey = gmsReadInteger (join_fd);

       printf("NOT FINISHED CODING\n"); exit (0);
      }

   theJoinTbl->numRecords = numRecords;
}


//---------------------------------------------*/
// printOneJoinRecord
//
// Description:
//    This function will print the specified
//    "joinRecordType" record to stdout.
//---------------------------------------------*/
static void printOneJoinRecord 
                  (joinRecordType joinRecord)

{
   printf("%d \t %d \t %d \t %d\n",
          joinRecord.ID,
          joinRecord.featureKey,
          (int) joinRecord.tileReferenceId,
          joinRecord.primaryKeyOfPrimitive);
}

