/*----------------------------------------------------------------------------*/
/* File : gmsLibAttribTable.c
/* Date : 07-Jul-99 : Initial definition
/*        01-Aug-99 : Convert to utilites on port to Sun Solaris
/*        05-Oct-99 : Clean-up due to code inspection
/*
/* Description:
/*    Utilities to "read" the "Library Attribute Table" file of the Digital
/*    Chart of the World (DCW).  The "Library Attribute Table describes the
/*    properties of each library in the database.
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsLibAttribTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


/*---------------------------*/
/*     Local Variables       */
/*---------------------------*/
static FILE *lat_fd = (FILE *) NULL;

static const int numRecords = 5;


/*---------------------------*/
/* Declare Local Subprograms */
/*---------------------------*/
static void readPastFormatInformation ();

static void buildLibAttribTblRecord
               (LibAttribTableType *theLAT);



/*-------------------------------------------*/
/* gmsGetLibAttribTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Library Attribute Table".  A pointer
/*    to a newly allocated table is returned to
/*    the caller.  It is the caller's responsibility
/*    to free the item by using the utility
/*    "gmsFreeLibAttribTable" (see below).
/*---------------------------------------------*/
LibAttribTableType *gmsGetLibAttribTable
                                  (const char *filePath)

{
         const char         readOnlyMode[] = { "rb" };
         LibAttribTableType *theLAT;

   if (filePath == NULL)
      return (LibAttribTableType *) NULL;

   lat_fd = fopen(filePath,
                  readOnlyMode);

   if (lat_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open file: %s\n", filePath);

       return (LibAttribTableType *) NULL;
      }

   readPastFormatInformation ();

   theLAT = (LibAttribTableType *) malloc (sizeof(LibAttribTableType));

   gmsClearMemory
      ((char *) theLAT,
       sizeof(LibAttribTableType) );

   buildLibAttribTblRecord (theLAT);

   fclose (lat_fd);

   return theLAT;
}


/*-------------------------------------------*/
/* gmsFreeLibAttribTable
/*
/* Description:
/*    This utility frees a "Library Attribute
/*    Table" that had been previously allocated
/*    using "gmsGetLibAttribTable".
/*---------------------------------------------*/
void gmsFreeLibAttribTable
           (LibAttribTableType *theLAT)

{
   free (theLAT);
}


/*-------------------------------------------*/
/* gmsPrintLibAttribTable	
/*
/* Description:
/*    This function will read the "lat" file.
/*-------------------------------------------*/
void gmsPrintLibAttribTable
        (LibAttribTableType *theLAT)

{
         int index;

   printf("______________________________\n");
   printf("Library Attribute Table:\n");

   if (theLAT == NULL)
      return;

   for (index = 0; index < numRecords; index++)
      {
       printf("   ---> ID                 = %d\n",
              theLAT[0][index].ID);
       printf("   ---> libraryName        = %s\n",
              theLAT[0][index].libraryName);
       printf("   ---> Western-most Long  = %f\n",
              theLAT[0][index].westMostLong);
       printf("   ---> Southern-most Lat  = %f\n",
              theLAT[0][index].southMostLat);
       printf("   ---> Eastern-most Long  = %f\n",
              theLAT[0][index].eastMostLong);
       printf("   ---> Northern-most Lat  = %f\n",
              theLAT[0][index].northMostLat);
       printf("------------------\n");
      }

   printf("\n");
}


     /*---------------------*/
     /* Local Subprograms   */
     /*---------------------*/


/*-------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the Library
/*    Attribute Table file.
/*-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (lat_fd);

   tempChar = fgetc (lat_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (lat_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (lat_fd);
      }
}


/*-------------------------------------------*/
/* buildLibAttribTblRecord
/*
/* Description:
/*    This function will read the actual data
/*    from the Library Attribute Table file.  The
/*    data read will be used to populate the
/*    attributes of the object.
/*-------------------------------------------*/
static void buildLibAttribTblRecord
               (LibAttribTableType *theLAT)

{
         int index;

   for (index = 0; index < numRecords; index++)
      {
       theLAT[0][index].ID = gmsReadInteger (lat_fd);

       fgets(theLAT[0][index].libraryName,
             sizeof(theLAT[0][index].libraryName),
             lat_fd);

       theLAT[0][index].westMostLong = gmsReadFloat (lat_fd);

       theLAT[0][index].southMostLat = gmsReadFloat (lat_fd);

       theLAT[0][index].eastMostLong = gmsReadFloat (lat_fd);

       theLAT[0][index].northMostLat = gmsReadFloat (lat_fd);
      }
}
