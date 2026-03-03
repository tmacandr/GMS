/*----------------------------------------------------------------------------*/
/* File : gmsDbHeaderTable.c
/* Date : 07-Apr-99 : Initial definition
/*        14-Jun-99 : Move from Turbo-C to VC++
/*        07-Jul-99 : Convert to C++ class
/*        01-Aug-99 : Converted back to utility on port to Sun Solaris
/*
/* Description:
/*    Utilities to "read" the "Database Header Table" file of the
/*    Digital Chart of the World (DCW).
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsDbHeaderTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


/*---------------------------*/
/*     Local Variables       /*
/*---------------------------*/
static FILE *dht_fd = (FILE *) NULL;


/*---------------------------*/
/* Declare Local Subprograms /*
/*---------------------------*/
static void readPastFormatInformation ();

static void buildDbHeaderTblRecord
               (DbHeaderTableType *theDHT);


/*-------------------------------------------*/
/* gmsGetDbHeaderTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Database Header Table".  A pointer
/*    to a newly allocated table is returned to
/*    the caller.  It is the caller's responsibility
/*    to free the item by using the utility
/*    "gmsFreeDbHeaderTable" (see below).
/*-------------------------------------------*/
DbHeaderTableType *gmsGetDbHeaderTable
                                (const char *filePath)

{
         const char        readOnlyMode[] = { "rb" };
         DbHeaderTableType *theDHT;

   if (filePath == NULL)
      return (DbHeaderTableType *) NULL;

   dht_fd = fopen(filePath,
                  readOnlyMode);

   if (dht_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open file: %s\n", filePath);

       return (DbHeaderTableType *) NULL;
      }

   readPastFormatInformation ();

   theDHT = (DbHeaderTableType *) malloc (sizeof(DbHeaderTableType));

   gmsClearMemory
      ((char *) theDHT,
       sizeof(DbHeaderTableType) );

   buildDbHeaderTblRecord (theDHT);

   fclose (dht_fd);

   return theDHT;
}


/*-------------------------------------------*/
/* gmsFreeDbHeaderTable
/*
/* Description:
/*    This utility frees a "Database Header
/*    Table" that had been previously allocated
/*    using "gmsGetDbHeaderTable".
/*-------------------------------------------*/
void gmsFreeDbHeaderTable
           (DbHeaderTableType *theDHT)

{
   free (theDHT);
}


/*-------------------------------------------*/
/* gmsPrintDbHeaderTable	
/*
/* Description:
/*    This function will print the "dht" object
/*    to stdout.
/*-------------------------------------------*/
void gmsPrintDbHeaderTable
        (DbHeaderTableType *theDHT)

{
   if (theDHT == NULL)
      return;

   printf("______________________________\n");
   printf("Database Header Table:\n");
   printf("   ---> ID                 = %d\n",
          theDHT->ID);
   printf("   ---> Database Name      = %s\n",
          theDHT->databaseName);
   printf("   ---> Description        = %s\n",
          theDHT->databaseDesc);
   printf("   ---> Media Standard     = %s\n",
          theDHT->mediaStandard);
   printf("   ---> Originator         = %s\n",
          theDHT->originator);
   printf("   ---> Addressee          = %s\n",
          theDHT->addressee);
   printf("   ---> Media Volumes      = %c\n",
          theDHT->mediaVolumes);
   printf("   ---> Sequential Numbers = %c\n",
          theDHT->seqNumbers);
   printf("   ---> Number of Data Sets= %c\n",
          theDHT->numDataSets);
   printf("   ---> Security Classific = %c\n",
          theDHT->securityClass);
   printf("   ---> Downgrading        = %s\n",
          theDHT->downgrading);
   printf("   ---> Downgrade Date     = %s\n",
          theDHT->downgradeDate);
   printf("   ---> Releaseability     = %s\n",
          theDHT->releasability);
   printf("   ---> Other Std Name     = %s\n",
          theDHT->otherStdName);
   printf("   ---> Other Std Date     = %s\n",
          theDHT->otherStdDate);
   printf("   ---> Other Std Version  = %s\n",
          theDHT->otherStdVersion);
   printf("   ---> Transmittal ID     = %c\n",
          theDHT->transmittalId);
   printf("   ---> Edition Number     = %s\n",
          theDHT->editionNumber);
   printf("   ---> Edition Date       = %s\n",
          theDHT->editionDate);
   printf("______________________________\n");
}


     /*---------------------*/
     /*   Local Subprograms /*
     /*---------------------*/


/*-------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the Database
/*    Header Table file.
/*-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (dht_fd);

   tempChar = fgetc (dht_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (dht_fd);

           if ( (char) tempChar == ';')
              return;
          }

       tempChar = fgetc (dht_fd);
      }

   printf("---> ERROR - unexpected EOF reading BROWSE DBH file.\n");
}


/*-------------------------------------------*/
/* buildDbHeaderTblRecord
/*
/* Description:
/*    This function will read the actual data
/*    from the Database Header Table file.  The
/*    data read will be used to populate the
/*    attributes of the object.
/*-------------------------------------------*/
static void buildDbHeaderTblRecord
              (DbHeaderTableType *theDHT)

{
   theDHT->ID = gmsReadInteger (dht_fd);

   fgets(theDHT->databaseName,
         sizeof(theDHT->databaseName),
         dht_fd);

   fgets(theDHT->databaseDesc,
         sizeof(theDHT->databaseDesc),
         dht_fd);

   fgets(theDHT->mediaStandard,
         sizeof(theDHT->mediaStandard),
         dht_fd);

   fgets(theDHT->originator,
         sizeof(theDHT->originator),
         dht_fd);

   fgets(theDHT->addressee,
         sizeof(theDHT->addressee),
         dht_fd);

   theDHT->mediaVolumes = (char) fgetc(dht_fd);

   theDHT->seqNumbers = (char) fgetc(dht_fd);

   theDHT->numDataSets = (char) fgetc(dht_fd);

   theDHT->securityClass = (char) fgetc(dht_fd);

   fgets(theDHT->downgrading,
         sizeof(theDHT->downgrading),
         dht_fd);

   fgets(theDHT->downgradeDate,
         sizeof(theDHT->downgradeDate),
         dht_fd);

   fgets(theDHT->releasability,
         sizeof(theDHT->releasability),
         dht_fd);

   fgets(theDHT->otherStdName,
         sizeof(theDHT->otherStdName),
         dht_fd);

   fgets(theDHT->otherStdDate,
         sizeof(theDHT->otherStdDate),
         dht_fd);

   fgets(theDHT->otherStdVersion,
         sizeof(theDHT->otherStdVersion),
         dht_fd);

   theDHT->transmittalId = (char) fgetc(dht_fd);

   fgets(theDHT->editionNumber,
         sizeof(theDHT->editionNumber),
         dht_fd);

   fgets(theDHT->editionDate,
         sizeof(theDHT->editionDate),
         dht_fd);
}
