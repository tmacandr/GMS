/*----------------------------------------------------------------------------*/
/* File : gmsNarrativeTable.cpp
/* Date : 02-Sep-99 : Initial Definition
/*        05-Oct-99 : Clean-up due to code inspection
/*
/* Description:
/*    Utilities to "read" a "narrative" file from the Digital Chart of
/*    the World (DCW) database.
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsNarrativeTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------*/
/*     Local Variables       --*/
/*-----------------------------*/
static FILE *doc_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms   */
/*-----------------------------*/
static void readDocForDebug ();

static void readPastFormatInformation ();

static void buildNarrativeTableObject
               (narrativeTableType *theDOC);


/*---------------------------------------------*/
/* gmsGetNarrativeTable
/*
/* Description:
/*    This utility reads the file that contains
/*    "Narrative" data.  A pointer to a newly
/*    allocated table is returned to the caller.
/*    It is the caller's responsibility to free
/*    the item by using the utility
/*    "gmsFreeNarrativeTable" (see below).
/*---------------------------------------------*/
narrativeTableType *gmsGetNarrativeTable
                             (const char *filePath)

{
         const char        readOnlyMode[] = { "rb" };
         narrativeTableType *theDOC;

   if (filePath == NULL)
      return (narrativeTableType *) NULL;

   doc_fd = fopen(filePath,
                  readOnlyMode);

   if (doc_fd == (FILE *) NULL)
      {
       printf("---> ERROR : can't open narrative table: %s\n", filePath);

       return (narrativeTableType *) NULL;
      }

   /* readDocForDebug (); */

   theDOC = (narrativeTableType *) malloc (sizeof(narrativeTableType));

   gmsClearMemory
      ( (char *) theDOC,
       sizeof (narrativeTableType));

   buildNarrativeTableObject (theDOC);

   fclose (doc_fd);

   return theDOC;
}


/*---------------------------------------------*/
/* gmsFreeNarrativeTable
/*
/* Description:
/*    This utility frees a "Narrative" table that
/*    had been previously allocated using
/*    "gmsGetNarrativeTable".
/*---------------------------------------------*/
void gmsFreeNarrativeTable
           (narrativeTableType *theDOC)

{
   if (theDOC == NULL)
      return;

   if (theDOC->theDoc != NULL)
      free (theDOC->theDoc);

   free (theDOC);
}


/*---------------------------------------------*/
/* gmsPrintNarrativeTable
/*
/* Description:
/*    This function will print the contents of
/*    the specified "Narrative" table to stdout.
/*---------------------------------------------*/
void gmsPrintNarrativeTable
           (narrativeTableType *theDOC)

{
   printf("______________________________\n");
   printf("Narrative Table:\n");

   if (theDOC == NULL)
      return;

   if (theDOC->theDoc == NULL)
      return;

   printf("%s\n", theDOC->theDoc);

   printf("______________________________\n");
}


     /*-----------------------*/
     /*   Local Subprograms
     /*-----------------------*/


/*---------------------------------------------*/
/* readDocForDebug
/*
/* Description:
/*    This routine is used for debug purposes
/*    only.  This routine will read the contents
/*    of the narrative file ... byte-by-byte ... and
/*    print the contents as characters to stdout.
/*    The file-descriptor is reset to the top
/*    of the file.
/*---------------------------------------------*/
static void readDocForDebug ()

{
         int numBytes  = 0;
         int lineCount = 0;
         int tempChar;

   rewind (doc_fd);

   printf("______________________________________\n");
   printf("Narrative Table:\n");

   tempChar = fgetc (doc_fd);

   while (tempChar != EOF)
      {
       numBytes++;

       printf("%.2x ", tempChar);

       if (lineCount == 3)
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;

       tempChar = fgetc (doc_fd);
      }

   printf("\n");
   printf("---> Num Bytes of data          = %d\n", numBytes);
   printf("---> File offset at END of data = %ld\n", ftell(doc_fd));
   printf("______________________________________\n");

   rewind (doc_fd);
}


/*-------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the Narrative
/*    Table file.
/*-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (doc_fd);

   tempChar = fgetc (doc_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (doc_fd);

           if ( (char) tempChar == ';')
              return;
          }

       tempChar = fgetc (doc_fd);
      }

   printf("---> ERROR - unexpected EOF reading narrative table\n");
}


/*---------------------------------------------*/
/* buildNarrativeTableObject
/*
/* Description:
/*    This function will read the actual data
/*    from the "Narrative" file.  The data read
/*    will be used to build the data-structure.
/*---------------------------------------------*/
static void buildNarrativeTableObject
               (narrativeTableType *theDOC)

{
         int       numBytes = 0;
         int       count    = 0;
         int       ID;
         int       index;
         int       i;
         int       tempChar;
         long      dataOffset;
         const int charsPerLine = 80;

   readPastFormatInformation ();

   dataOffset = ftell (doc_fd);

   tempChar = fgetc (doc_fd);

   while (tempChar != EOF)
      {
       numBytes++;

       tempChar = fgetc (doc_fd);
      }

   theDOC->theDoc = (char *) malloc (numBytes);

   gmsClearMemory
      (theDOC->theDoc,
       numBytes);

   fseek
      (doc_fd,
       dataOffset,
       SEEK_SET);

   index = 0;

   count = 0;

   do
      {
       ID = gmsReadInteger (doc_fd);  /* don't care */

       for (i = 0; i < charsPerLine; i++)
          {
           theDOC->theDoc[index] = fgetc (doc_fd);

           index++;
          }

       theDOC->theDoc[index] = '\n';

       index++;

       count = count + sizeof(int) + charsPerLine;
      } while (count < numBytes);

   theDOC->theDoc[index] = '\0';
}
