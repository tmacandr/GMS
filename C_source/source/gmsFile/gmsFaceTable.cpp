/*----------------------------------------------------------------------------*/
/* File : gmsFaceTable.cpp
/* Date : 07-Aug-99 : initial definition
/*        21-Sep-99 : browse record is different from regional-lib record
/*        13-Oct-99 : routine figures out which format of record
/*
/* Description:
/*    Utilities to "read" any "Face Table" file of the Digital Chart of
/*    the World (DCW).  Consider the following definition:
/*
/*       Face:
/*       -----
/*          A region enclosed by an edge or set of
/*          edges.  Faces are topologically linked
/*          to their surrounding edges as well as to
/*          the other faces that surround them.  Faces
/*          are always non-overlappoing, exhausting
/*          the area of a plane.
/*
/*    There are two types of face tables.  The first is used for the BROWSE
/*    library.  Its format is:
/*
/*       BROWSE Face Record:
/*       -------------------
/*             ID      =I,1,P,Row ID                   ,-,-,
/*             RING_PTR=I,1,F,Foreign Key to Ring Table,-,-,
/*
/*    The second type is used for the other REGIONAL libraries.  Its format
/*    is:
/*
/*       REGIONAL Face Record:
/*       ---------------------
/*             ID      =I,1,P,Row ID                           ,-,-
/*             *.AFT_ID=I,1,F,Foreign Key to Area Feature Table,-,-,
/*             RING_PTR=I,1,F,Foreign Key to Ring Table        ,-,-
/*
/*             where * is the name of the corresponding feature class.
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsFaceTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


/*-----------------------------*/
/*     Local Type Declares
/*-----------------------------*/
typedef enum { gmsBrowseLibFace,
               gmsRegionalLibFace } gmsKindOfFaceType;


/*-----------------------------*/
/*     Local Variables
/*-----------------------------*/
static FILE *face_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms
/*-----------------------------*/
static void readPastFormatInformation ();

static void buildFaceTable
               (faceTableType *theFaceTbl);

static void printFaceTable
               (faceTableType *theFaceTbl);

static gmsKindOfFaceType determineKindOfFaceRecord ();


/*---------------------------------------------*/
/* gmsGetFaceTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Face Table".  This routine determines
/*    which format of face-record to use to build
/*    the table based on the specified file.
/*
/*    A pointer to a newly allocated table is
/*    returned to the caller.  It is the caller's
/*    responsibility to free the item by using
/*    the utility
/*    "gmsFreeFaceTable" (see below).
/*---------------------------------------------*/
faceTableType *gmsGetFaceTable
                   (const char *faceTableFilePath)

{
         const char    readOnlyMode[] = { "rb" };
         faceTableType *theFaceTbl;

   if (faceTableFilePath == NULL)
      return (faceTableType *) NULL;

   face_fd = fopen(faceTableFilePath,
                   readOnlyMode);

   if (face_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open FAC file: %s\n",
              faceTableFilePath);

       return (faceTableType *) NULL;
      }

   theFaceTbl =
      (faceTableType *) malloc (sizeof(faceTableType));

   gmsClearMemory
      ( (char *) theFaceTbl,
        sizeof(faceTableType));

   buildFaceTable (theFaceTbl);

   fclose (face_fd);

   return theFaceTbl;
}


/*---------------------------------------------*/
/* gmsFreeFaceTable
/*
/* Description:
/*    This utility frees a "Face Table" that had
/*    been previously allocated using
/*    "gmsGetFaceTable".
/*---------------------------------------------*/
void gmsFreeFaceTable
           (faceTableType *theFaceTable)

{
   if (theFaceTable == NULL)
      return;

   if (theFaceTable->faceRecords != NULL)
      free (theFaceTable->faceRecords);

   free (theFaceTable);
}


/*---------------------------------------------*/
/* gmsPrintFaceTable	
/*
/* Description:
/*    This function will print the "Face Table"
/*    object to standard out.
/*---------------------------------------------*/
void gmsPrintFaceTable
           (faceTableType *theFaceTable)

{
   printf("________________________________________\n");
   printf("Face Table:\n");

   if (theFaceTable == NULL)
      return;

   printf("Num Records : %d\n", theFaceTable->numRecords);

   if (theFaceTable->faceRecords == NULL)
      return;

   printFaceTable (theFaceTable);
}


     /*-----------------------*/
     /*   Local Subprograms
     /*-----------------------*/


/*---------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the "Face Table"
/*    file.
/*---------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (face_fd);

   tempChar = fgetc (face_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (face_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (face_fd);
      }
}


/*---------------------------------------------*/
/* buildFaceTable
/*
/* Description:
/*    This function will read the actual data
/*    from the Face Table file.  The data read
/*    will be used to populate the attributes of
/*    the object.
/*---------------------------------------------*/
static void buildFaceTable
               (faceTableType *theFaceTbl)

{
         int               numBytes = 0;
         int               numRecords;
         int               tempChar;
         int               i;
         const int         Size_Of_Browse_Face_Rec = 8;
         const int         Size_Of_Regional_Face_Rec = 12;
         faceRecordType    *buffer;
         gmsKindOfFaceType kindOfFace;

   kindOfFace = determineKindOfFaceRecord ();

   rewind (face_fd);

   readPastFormatInformation ();

   tempChar = fgetc(face_fd);

   while ( tempChar != EOF )
      {
       numBytes++;

       tempChar = fgetc(face_fd);
      }

   if (kindOfFace == gmsBrowseLibFace)

      numRecords = numBytes / Size_Of_Browse_Face_Rec;

   else if (kindOfFace == gmsRegionalLibFace)

      numRecords = numBytes / Size_Of_Regional_Face_Rec;

   else
      {
       printf("*** ERROR : Unknown format for face table\n");

       return;
      }

   numBytes = numRecords * sizeof(faceRecordType);

   theFaceTbl->faceRecords = (faceRecordType *) malloc (numBytes);

   gmsClearMemory
      ( (char *) theFaceTbl->faceRecords,
        numBytes);

   theFaceTbl->numRecords = numRecords;

   buffer = theFaceTbl->faceRecords;

   rewind (face_fd);

   readPastFormatInformation ();

   for (i = 0; i < numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (face_fd);

       if (kindOfFace == gmsBrowseLibFace)

          buffer[i].fgnKeyToFeatureTbl = -1;

       else

          buffer[i].fgnKeyToFeatureTbl = gmsReadInteger (face_fd);

       buffer[i].fgnKeyToRngTable = gmsReadInteger (face_fd);
      }

}


/*---------------------------------------------*/
/* printFaceTable
/*
/* Description:
/*    This function will print the specified
/*    "face-table" to stdout.
/*---------------------------------------------*/
static void printFaceTable
               (faceTableType *theFaceTbl)

{
         int i;

   printf("ID \t Attrib-Fgn-Key \t Rng-Tbl-Fgn-Key\n");
   printf("------------------------------------------\n");

   for (i = 0; i < theFaceTbl->numRecords; i++)
      printf("%d \t %d \t\t\t %d\n",
             theFaceTbl->faceRecords[i].ID,
             theFaceTbl->faceRecords[i].fgnKeyToFeatureTbl,
             theFaceTbl->faceRecords[i].fgnKeyToRngTable);
}


/*---------------------------------------------*/
/* determineKindOfFaceRecord
/*
/* Description:
/*    This function reads information from the
/*    file header to determine which of the two
/*    variants of the face record to use in order
/*    to build the face-table.
/*---------------------------------------------*/
static gmsKindOfFaceType determineKindOfFaceRecord ()

{
         int       sizeOfHdr;
         const int sizeOfBrowseHdr = 93;

   rewind (face_fd);

   sizeOfHdr = gmsReadInteger (face_fd);

   if (sizeOfHdr == sizeOfBrowseHdr)

      return gmsBrowseLibFace;

   else

      return gmsRegionalLibFace;
}
