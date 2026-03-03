/*----------------------------------------------------------------------------*/
/* File : gmsMinBoundRectTable.cpp
/* Date : 06-Aug-99 : initial definition
/*
/* Description:
/*    Utilities to "read" any "Minimum-Bounding-Rectangle" file of the Digital
/*    Chart of the World (DCW).  This includes:
/*
/*                Edge Bounding Rectangles
/*                Face Bounding Rectangles
/*
/*    Consider the following definition(s):
/*
/*       Minimum Bounding Rectangle:
/*       ---------------------------
/*          A rectangle of coordinate tuples that defines
/*          the minimum and maximum coordinates of an entity.
/*          Abbreviated as MBR.
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsMinBoundRectTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------*/
/*     Local Variables
/*-----------------------------*/
static FILE *mbr_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms
/*-----------------------------*/
static void readPastFormatInformation ();

static void buildMbrTable
               (minBoundRectTableType *theMbrTable);

static void printMbrTable
               (minBoundRectTableType *mbrTbl);


/*---------------------------------------------*/
/* gmsGetMbrTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Minimum Bounding Rectangle Table".  A
/*    pointer to a newly allocated table is returned
/*    to the caller.  It is the caller's responsibility
/*    to free the item by using the utility
/*    "gmsFreeMbrTable" (see below).
/*---------------------------------------------*/
minBoundRectTableType *gmsGetMbrTable
                           (const char *mbrTableFilePath)

{
         const char            readOnlyMode[] = { "rb" };
         minBoundRectTableType *theMbrTable;

   if (mbrTableFilePath == NULL)
      return (minBoundRectTableType *) NULL;

   mbr_fd = fopen(mbrTableFilePath,
                  readOnlyMode);

   if (mbr_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open MBR file: %s\n",
              mbrTableFilePath);

       return (minBoundRectTableType *) NULL;
      }

   theMbrTable =
      (minBoundRectTableType *) malloc (sizeof(minBoundRectTableType));

   gmsClearMemory
      ( (char *) theMbrTable,
        sizeof(minBoundRectTableType));

   buildMbrTable (theMbrTable);

   fclose (mbr_fd);

   return theMbrTable;
}


/*---------------------------------------------*/
/* gmsFreeMbrTable
/*
/* Description:
/*    This utility frees a "MBR Table" that had
/*    been previously allocated using
/*    "gmsGetMbrTable".
/*---------------------------------------------*/
void gmsFreeMbrTable
           (minBoundRectTableType *theMbrTable)

{
   if (theMbrTable == NULL)
      return;

   if (theMbrTable->mbrRecords != NULL)
      free (theMbrTable->mbrRecords);

   free (theMbrTable);
}


/*---------------------------------------------*/
/* gmsPrintMbrTable	
/*
/* Description:
/*    This function will print the "MBR Table"
/*    object to standard out.
/*---------------------------------------------*/
void gmsPrintMbrTable
           (minBoundRectTableType *theMbrTable)

{
   printf("________________________________________\n");
   printf("MBR Table:\n");

   if (theMbrTable == NULL)
      return;

   printf("Num Records : %d\n", theMbrTable->numRecords);

   if (theMbrTable->mbrRecords == NULL)
      return;

   printMbrTable (theMbrTable);
}


     /*-----------------------*/
     /*   Local Subprograms
     /*-----------------------*/


/*---------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the MBR Table
/*    file.
/*---------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (mbr_fd);

   tempChar = fgetc (mbr_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (mbr_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (mbr_fd);
      }
}


/*---------------------------------------------*/
/* buildMbrTable
/*
/* Description:
/*    This function will read the actual data
/*    from the MBR Table file.  The data read
/*    will be used to populate the attributes of
/*    the object.
/*---------------------------------------------*/
static void buildMbrTable
               (minBoundRectTableType *theMbrTable)

{
         int numBytes = 0;
         int numRecords;
         int tempChar;
         int index;

   readPastFormatInformation ();

   tempChar = fgetc(mbr_fd);

   while ( tempChar != EOF )
      {
       numBytes++;

       tempChar = fgetc(mbr_fd);
      }

   numRecords = numBytes / sizeof(minBoundRectType);

   rewind (mbr_fd);

   readPastFormatInformation ();

   numBytes = numRecords * sizeof(minBoundRectType);

   theMbrTable->mbrRecords = (minBoundRectType *) malloc (numBytes);

   gmsClearMemory
      ( (char *) theMbrTable->mbrRecords,
        numBytes);

   theMbrTable->numRecords = numRecords;

   for (index = 0; index < numRecords; index++)
      {
       theMbrTable->mbrRecords[index].ID   = gmsReadInteger (mbr_fd);

       theMbrTable->mbrRecords[index].xMin = gmsReadFloat (mbr_fd);

       theMbrTable->mbrRecords[index].yMin = gmsReadFloat (mbr_fd);

       theMbrTable->mbrRecords[index].xMax = gmsReadFloat (mbr_fd);

       theMbrTable->mbrRecords[index].yMax = gmsReadFloat (mbr_fd);
      }
}


/*---------------------------------------------*/
/* printMbrTable
/*
/* Description:
/*    This function will print the records of
/*    the specified "Min-Bounding-Rectangle"
/*    to stdout.
/*---------------------------------------------*/
static void printMbrTable
                  (minBoundRectTableType *mbrTbl)

{
         int i;

   printf("ID \t (Xmin, Ymin) \t (Xmax, Ymax)\n");
   printf("---------------------------------------------------\n");

   for (i = 0; i < mbrTbl->numRecords ; i++)
      printf("%d) \t (%f, %f) \t (%f, %f)\n",
             mbrTbl->mbrRecords[i].ID,
             mbrTbl->mbrRecords[i].xMin,
             mbrTbl->mbrRecords[i].yMin,
             mbrTbl->mbrRecords[i].xMax,
             mbrTbl->mbrRecords[i].yMax);
}
