/*----------------------------------------------------------------------------*/
/* File : gmsCoverageAttribTable.c
/* Date : 12-Jul-99 : Initial definition
/*        01-Aug-99 : Ported to Sun Solaris
/*        07-Oct-99 : Clean-up due to code-inspection
/*
/* Description:
/*    Utilities to "read" the "Coverage Attribute Table" file from the
/*    BROWSE library of the Digital Chart of the World (DCW) database.
/*
/*    This table contains four columns (i.e. records with 4 attributes):
/*          1) Row ID
/*          2) Coverage Name
/*          3) Coverage Description
/*          4) Topology Level
/*    The coverage description is equivalent to the name of the thematic
/*    layer.
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsCoverageAttribTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*---------------------------*/
/*     Local Variables       */
/*---------------------------*/
static FILE *cat_fd = (FILE *) NULL;


/*---------------------------*/
/* Declare Local Subprograms */
/*---------------------------*/
static void readPastFormatInformation ();

static int countNumRecords ();

static CoverageRecordType readOneCoverageRecord();

static void buildCovAttribTable
               (CovAttribTableType *theCAT);

static void debugPrintCat ();


/*-------------------------------------------*/
/* gmsGetCoverageAttribTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Coverage Attribute Table".  A pointer
/*    to a newly allocated table is returned to
/*    the caller.  It is the caller's responsibility
/*    to free the item by using the utility
/*    "gmsFreeCoverageAttribTable" (see below).
/*-------------------------------------------*/
CovAttribTableType *gmsGetCoverageAttribTable
                                 (const char *filePath)

{
         const char  readOnlyMode[] = { "rb" };
         CovAttribTableType *theCAT;

   if (filePath == NULL)
      return (CovAttribTableType *) NULL;

   cat_fd = fopen(filePath,
                  readOnlyMode);

   if (cat_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open CAT file: %s\n", filePath);

       return (CovAttribTableType *) NULL;
      }

   theCAT = (CovAttribTableType *) malloc (sizeof(CovAttribTableType));

   gmsClearMemory
      ( (char *) theCAT,
        sizeof(CovAttribTableType) );
 
   buildCovAttribTable (theCAT);

   fclose (cat_fd);

   return theCAT;
}


/*-------------------------------------------*/
/* gmsFreeCoverageAttribTable
/*
/* Description:
/*    This utility frees a "Coverage Attribute
/*    Table" that had been previously allocated
/*    using "gmsGetCoverageAttribTable".
/*-------------------------------------------*/
void gmsFreeCoverageAttribTable
           (CovAttribTableType *theCAT)

{
   if (theCAT == NULL)
      return;

   if (theCAT->theCATRecords != NULL)
      free (theCAT->theCATRecords);

   free (theCAT);
}


/*-------------------------------------------*/
/* gmsPrintCoverageAttribTable	
/*
/* Description:
/*    This function will print the "CAT" object
/*    to stdout.
/*-------------------------------------------*/
void gmsPrintCoverageAttribTable
           (CovAttribTableType *theCAT)

{
         int i;

   printf("______________________________\n");
   printf("Coverage Attribute Table:\n");

   if (theCAT == NULL)
      return;

   printf("Num Records = %d\n", theCAT->numRecords);

   printf
      ("ID  Name       Description                      \t      Level\n");

   for (i = 0;
        i < theCAT->numRecords;
        i++)
      {
       printf(" %d  %s %s %d\n",
           theCAT->theCATRecords[i].ID,
           theCAT->theCATRecords[i].coverageName,
           theCAT->theCATRecords[i].description,
           theCAT->theCATRecords[i].level);
      }

   printf("______________________________\n");
}


     /*---------------------*/
     /*   Local Subprograms */
     /*---------------------*/


/*-------------------------------------------*/
/* debugPrintCat 
/*
/* Description:
/*-------------------------------------------*/
static void debugPrintCat ()

{
         int tempChar;

   rewind (cat_fd);

   tempChar = fgetc (cat_fd);

   while (tempChar != EOF)
      {
       printf("%c", (char) tempChar);

       if ( (char) tempChar == ':')
          printf("\n");

       tempChar = fgetc (cat_fd);
      }

   printf("\n");
}


/*-------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the "Coverage
/*    Attribute Table".
/*-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (cat_fd);

   tempChar = fgetc (cat_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (cat_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (cat_fd);
      }
}


/*-------------------------------------------*/
/* countNumRecords
/*
/* Description:
/*    This function will count the number of
/*    coverage records contained in the
/*    "Coverage Attribute Table".
/*-------------------------------------------*/
static int countNumRecords ()

{
         int       tempChar;
         int       numBytes = 0;
         const int Size_Of_Cov_Attrib_Rec = 66;  /* bytes, no padding */
         int       numRecords;

   rewind (cat_fd);

   readPastFormatInformation ();

   tempChar = fgetc (cat_fd);

   while (tempChar != EOF)
      {
       numBytes++;

       tempChar = fgetc (cat_fd);
      }

   numRecords = numBytes / Size_Of_Cov_Attrib_Rec;

   return numRecords;
}


/*-------------------------------------------*/
/* readOneCoverageRecord
/*
/* Description:
/*    This function will read one "Coverage"
/*    record from the Coverage Attribute
/*    Table (i.e. file).
/*-------------------------------------------*/
static CoverageRecordType readOneCoverageRecord()

{
         static CoverageRecordType theRecord;

   theRecord.ID = gmsReadInteger (cat_fd);

   fgets(theRecord.coverageName,
         sizeof(theRecord.coverageName),
         cat_fd);

   fgets(theRecord.description,
         sizeof(theRecord.description),
         cat_fd);

   theRecord.level = gmsReadInteger (cat_fd);

   return theRecord;
}


/*-------------------------------------------*/
/* buildCovAttribTable
/*
/* Description:
/*    This function will read the actual data
/*    from the Coverage Attribute file.  The
/*    data read will be used to populate the
/*    attributes of the object.
/*-------------------------------------------*/
static void buildCovAttribTable
               (CovAttribTableType *theCAT)

{
         int i;
         int numBytes;

   theCAT->numRecords = countNumRecords ();

   if (theCAT->numRecords <= 0)
      return;

   numBytes = sizeof(CoverageRecordType) * theCAT->numRecords;

   theCAT->theCATRecords = (CoverageRecordType *) malloc(numBytes);

   gmsClearMemory
      ( (char *) theCAT->theCATRecords,
        numBytes);

   rewind (cat_fd);

   readPastFormatInformation ();

   for (i = 0; i < theCAT->numRecords; i++)
      {
       theCAT->theCATRecords[i] = readOneCoverageRecord ();
      }
}
