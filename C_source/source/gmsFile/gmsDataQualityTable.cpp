/*----------------------------------------------------------------------------*/
/* File : gmsDataQualityTable.c
/* Date : 12-Jul-99 : Initial definition
/*        01-Aug-99 : Convert to utilities on port to Sun Solaris
/*        07-Oct-99 : Clean-up due to code-inspection
/*
/* Description:
/*    Utilities to "read" the "Data Quality Table" file from the BROWSE
/*    library of the Digital Chart of the World (DCW).
/*
/*    The "Data Quality Table" contains information on the completeness,
/*    consistency, date status, attribute accuracy, and positional accuracy
/*    of the data.
/*
/*    This component implements a "singleton".  There will be (only) one
/*    "Data Quality" structure allocated.  Multiple references will access
/*    the same component.  When all references to the object are terminated,
/*    the single object will then be "freed".
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsDataQualityTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


/*---------------------------*/
/*     Local Variables       */
/*---------------------------*/
static FILE                 *dqt_fd = (FILE *) NULL;

static DataQualityTableType *theOneDataQualTbl = NULL;

static int                  refCount = 0; /* implement singleton */


/*---------------------------*/
/* Declare Local Subprograms */
/*---------------------------*/
static void readPastFormatInformation ();

static void buildDataQualityTable ();


/*-------------------------------------------*/
/* gmsGetDataQualityTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Data Quality Table".  A pointer
/*    to a newly allocated table is returned to
/*    the caller.  It is the caller's responsibility
/*    to free the item by using the utility
/*    "gmsFreeDataQualityTable" (see below).
/*-------------------------------------------*/
DataQualityTableType *gmsGetDataQualityTable
                                   (const char *filePath)

{
         const char readOnlyMode[] = { "rb" };

   /*--------------------------------------*/
   /* Actual implementation is a singleton
   /*--------------------------------------*/

   if (theOneDataQualTbl != NULL)
      {
       refCount++;

       return theOneDataQualTbl;
      }

   if (filePath == NULL)
      return (DataQualityTableType *) NULL;

   dqt_fd = fopen(filePath,
                  readOnlyMode);

   if (dqt_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open file: %s\n", filePath);

       return (DataQualityTableType *) NULL;
      }

   theOneDataQualTbl =
      (DataQualityTableType *) malloc (sizeof(DataQualityTableType));

   gmsClearMemory
      ((char *) theOneDataQualTbl,
       sizeof (DataQualityTableType) );

   readPastFormatInformation ();

   buildDataQualityTable ();

   fclose (dqt_fd);

   refCount = 1;

   return theOneDataQualTbl;
}


/*-------------------------------------------*/
/* gmsFreeDataQualityTable
/*
/* Description:
/*    This utility frees a "Data Quality
/*    Table" that had been previously allocated
/*    using "gmsGetDataQualityTable".
/*-------------------------------------------*/
void gmsFreeDataQualityTable
           (DataQualityTableType *theDQT)

{
   if (refCount == 1)
      {
       free (theOneDataQualTbl);

       theOneDataQualTbl = NULL;
      }

   refCount--;

   theDQT = NULL;
}


/*-------------------------------------------*/
/* gmsPrintDataQualityTable	
/*
/* Description:
/*    This function will print the "Dqt" object
/*    to stdout.
/*-------------------------------------------*/
void gmsPrintDataQualityTable
        (DataQualityTableType *theDQT)

{
   printf("______________________________\n");
   printf("Data Quality Table:\n");

   if (theDQT == NULL)
      {
       printf("DQT is null\n");

       return;
      }

   printf("   ---> ID                 = %d\n",
          theDQT->ID);
   printf("   ---> VPF Level          = %s\n",
          theDQT->VPF_level);
   printf("   ---> VPF Level Name     = %s\n",
          theDQT->VPF_levelName);
   printf("   ---> Feature Complete %% = %s\n",
          theDQT->featureCompletePercent);
   printf("   ---> Attrib Complete %%  = %s\n",
          theDQT->attributeCompletePercent);
   printf("   ---> Logical Consistency= %s\n",
          theDQT->logicalConsistency);
   printf("   ---> Edition Num        = %s\n",
          theDQT->editionNumber);
   printf("   ---> Creation Date      = %s\n",
          theDQT->creationDate);
   printf("   ---> Revision Date      = %s\n",
          theDQT->revisionDate);
   printf("   ---> Specification Name = %s\n",
          theDQT->specificationName);
   printf("   ---> Specificaion Date  = %s\n",
          theDQT->specificationDate);
   printf("   ---> Earliest Src Date  = %s\n",
          theDQT->dateOfEarliestSrc);
   printf("   ---> Latest Src Date    = %s\n",
          theDQT->dateOfLatestSrc);
   printf("   ---> Std Dev of Quant Attrib = %s\n",
          theDQT->stdDevOfQuantAttrib);
   printf("   ---> %% Relia Of Qual Att= %s\n",
          theDQT->percentReliabOfQualAttrib);
   printf("   ---> Collectn Spec Name = %s\n",
          theDQT->collectionSpecName);
   printf("   ---> Name Included Src  = %s\n",
          theDQT->nameOfIncludedSrcFile);
   printf("   ---> Abs Horiz Accuracy = %s\n",
          theDQT->absoluteHorizAccuracy);
   printf("   ---> Abs Horiz Units    = %s\n",
          theDQT->absoluteHorizUnits);
   printf("   ---> Abs Vert Accuracy  = %s\n",
          theDQT->absoluteVertAccuracy);
   printf("   ---> Abs Vert Units     = %s\n",
          theDQT->absoluteVertUnits);
   printf("   ---> Relat Horiz Accur  = %s\n",
          theDQT->relativeHorizAccuracy);
   printf("   ---> Relat Horiz Units  = %s\n",
          theDQT->relativeHorizUnits);
   printf("   ---> Relat Vert Accur   = %s\n",
          theDQT->relativeVertAccuracy);
   printf("   ---> Relat Vert Units   = %s\n",
          theDQT->relativeVertUnits);
   printf("   ---> Comments           = %s\n",
          theDQT->comments);
   printf("______________________________\n");
}


     /*---------------------*/
     /*   Local Subprograms */
     /*---------------------*/


/*-------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the Data
/*    Quality Table file.
/*-------------------------------------------*/
static void readPastFormatInformation ()

{
         int currentChar;

   rewind (dqt_fd);

   currentChar = fgetc (dqt_fd);

   while (currentChar != EOF)
      {
       if ( (char) currentChar == ':')
          {
           currentChar = fgetc (dqt_fd);

           if ( (char) currentChar == ';')
              break;
          }

       currentChar = fgetc (dqt_fd);
      }
}


/*-------------------------------------------*/
/* buildDataQualityTable
/*
/* Description:
/*    This function will read the actual data
/*    from the Data Quality Table file.  The
/*    data read will be used to populate the
/*    attributes of the object.
/*-------------------------------------------*/
static void buildDataQualityTable ()

{
         int numChars = 0;

   /* ID */
   theOneDataQualTbl->ID = gmsReadInteger (dqt_fd);

   /* VPF_level */
   fgets
      (theOneDataQualTbl->VPF_level,
       sizeof(theOneDataQualTbl->VPF_level),
       dqt_fd);

   /* VPF_levelName */
   fgets
      (theOneDataQualTbl->VPF_levelName,
       sizeof(theOneDataQualTbl->VPF_levelName),
       dqt_fd);

   /* Feature Completeness Percent */
   numChars = gmsReadInteger(dqt_fd);

   fgets
      (theOneDataQualTbl->featureCompletePercent,
       (numChars + 1),
       dqt_fd);

   /* Attribute Completeness Percent */
   numChars = gmsReadInteger(dqt_fd);

   fgets
      (theOneDataQualTbl->attributeCompletePercent,
       (numChars + 1),
       dqt_fd);

   /* Logical Consistency */
   numChars = gmsReadInteger(dqt_fd);

   fgets
      (theOneDataQualTbl->logicalConsistency,
       (numChars + 1),
       dqt_fd);

   /* Edition Number */
   fgets
      (theOneDataQualTbl->editionNumber,
       sizeof(theOneDataQualTbl->editionNumber),
       dqt_fd);

   /* Creation Date */
   fgets
      (theOneDataQualTbl->creationDate,
       sizeof(theOneDataQualTbl->creationDate),
       dqt_fd);

   /* Revision Date */
   fgets
      (theOneDataQualTbl->revisionDate,
       sizeof(theOneDataQualTbl->revisionDate),
       dqt_fd);

   /* Specification Name */
   fgets
      (theOneDataQualTbl->specificationName,
       sizeof(theOneDataQualTbl->specificationName),
       dqt_fd);

   /* Specification Date */
   fgets
      (theOneDataQualTbl->specificationDate,
       sizeof(theOneDataQualTbl->specificationDate),
       dqt_fd);

   /* Date of Earliest Source */
   fgets
     (theOneDataQualTbl->dateOfEarliestSrc,
      sizeof(theOneDataQualTbl->dateOfEarliestSrc),
      dqt_fd);

   /* Date of Latest Source */
   fgets
     (theOneDataQualTbl->dateOfLatestSrc,
      sizeof(theOneDataQualTbl->dateOfLatestSrc),
      dqt_fd);

   /* Standard Deviation of Quatitative Attributes */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->stdDevOfQuantAttrib,
       numChars + 1,
       dqt_fd);

   /* Percent Reliability of Qualitative Attributes */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->percentReliabOfQualAttrib,
       numChars + 1,
       dqt_fd);

   /* Collection Specification Name */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->collectionSpecName,
       numChars + 1,
       dqt_fd);

   /* Name of Included Source File */
   fgets
     (theOneDataQualTbl->nameOfIncludedSrcFile,
      sizeof(theOneDataQualTbl->nameOfIncludedSrcFile),
      dqt_fd);

   /* Absolute Horizontal Accuracy */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->absoluteHorizAccuracy,
       numChars + 1,
       dqt_fd);

   /* Absolute Horizontal Units */
   fgets
      (theOneDataQualTbl->absoluteHorizUnits,
       sizeof(theOneDataQualTbl->absoluteHorizUnits),
       dqt_fd);

   /* Absolute Vertical Accuracy */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->absoluteVertAccuracy,
       numChars + 1,
       dqt_fd);

   /* Absolute Vertical Units */
   fgets
      (theOneDataQualTbl->absoluteVertUnits,
       sizeof(theOneDataQualTbl->absoluteVertUnits),
       dqt_fd);

   /* Relative Horizontal Accuracy */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->relativeHorizAccuracy,
       numChars + 1,
       dqt_fd);

   /* Relative Horizontal Units */
   fgets
      (theOneDataQualTbl->relativeHorizUnits,
       sizeof(theOneDataQualTbl->relativeHorizUnits),
       dqt_fd);

   /* Relative Horizontal Accuracy */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->relativeVertAccuracy,
       numChars + 1,
       dqt_fd);

   /* Relative Vertical Units */
   fgets
      (theOneDataQualTbl->relativeVertUnits,
       sizeof(theOneDataQualTbl->relativeVertUnits),
       dqt_fd);

   /* Comments */
   numChars = gmsReadInteger (dqt_fd);

   fgets
      (theOneDataQualTbl->comments,
       numChars + 1,
       dqt_fd);
}

