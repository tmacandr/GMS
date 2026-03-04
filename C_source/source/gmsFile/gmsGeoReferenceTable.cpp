//----------------------------------------------------------------------------*/
// File : gmsGeoReferenceTable.c
// Date : 11-Jul-99 : Initial definition
//        01-Aug-99 : Convert to utility on port to Sun Solaris
//        05-Oct-99 : Clean-up due to code inspection
//
// Description:
//    Utilities to "read" the "Geographic Reference Table" file of the
//    Digital Chart of the World (DCW).
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsGeoReferenceTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


//---------------------------*/
//     Local Variables       */
//---------------------------*/
static FILE *grt_fd = (FILE *) NULL;


//---------------------------*/
// Declare Local Subprograms */
//---------------------------*/
static void readPastFormatInformation ();

static void buildGeoReferenceTable
               (GeoRefTableType *theGRT);


//-------------------------------------------*/
// gmsGetGeoReferenceTable
//
// Description:
//    This utility reads the file that contains
//    the "Geographic Reference Table".  A pointer
//    to a newly allocated table is returned to
//    the caller.  It is the caller's responsibility
//    to free the item by using the utility
//    "gmsFreeGeoReferenceTable" (see below).
//-------------------------------------------*/
GeoRefTableType *gmsGetGeoReferenceTable
                             (const char *filePath)

{
         const char      readOnlyMode[] = { "rb" };
         GeoRefTableType *theGRT;

   if (filePath == NULL)
      return (GeoRefTableType *) NULL;

   grt_fd = fopen(filePath,
                  readOnlyMode);

   if (grt_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open file: %s\n", filePath);

       return (GeoRefTableType *) NULL;
      }

   readPastFormatInformation ();

   theGRT = (GeoRefTableType *) malloc (sizeof(GeoRefTableType));

   gmsClearMemory
      ((char *) theGRT,
       sizeof(GeoRefTableType));

   buildGeoReferenceTable (theGRT);

   fclose (grt_fd);

   return theGRT;
}


//-------------------------------------------*/
// gmsFreeGeoReferenceTable
//
// Description:
//    This utility frees a "Geographic Reference
//    Table" that had been previously allocated
//    using "gmsGetGeoReferenceTable".
//-------------------------------------------*/
void gmsFreeGeoReferenceTable
           (GeoRefTableType *theGRT)

{
   if (theGRT == NULL)
      return;

   free (theGRT);
}


//-------------------------------------------*/
// gmsPrintGeoReferenceTable 
//
// Description:
//    This function will print the "grf" object
//    to stdout.
//-------------------------------------------*/
void gmsPrintGeoReferenceTable 
        (GeoRefTableType *theGRT)

{
   if (theGRT == NULL)
      return;

   printf("______________________________\n");
   printf("Geographic Reference Table:\n");
   printf("   ---> ID                 = %d\n",
          theGRT->ID);
   printf("   ---> Data Type          = %s\n",
          theGRT->dataType);
   printf("   ---> Units              = %s\n",
          theGRT->units);
   printf("   ---> Ellipsoid          = %s\n",
          theGRT->ellipsoid);
   printf("   ---> Ellipsoid Details  = %s\n",
          theGRT->ellipsoidDetails);
   printf("   ---> Datum Vertical Ref = %s\n",
          theGRT->datumVerticalReference);
   printf("   ---> Vertical Datum Code= %s\n",
          theGRT->verticalDatumCode);
   printf("   ---> Sounding Datuam    = %s\n",
          theGRT->soundingDatum);
   printf("   ---> Sounding Datum Code= %s\n",
          theGRT->soundingDatumCode);
   printf("   ---> Datum Geodetic Name= %s\n",
          theGRT->datumGeodeticName);
   printf("   ---> Datum Geodetic Code= %s\n",
          theGRT->datumGeodeticCode);
   printf("   ---> Projection Name    = %s\n",
          theGRT->projectionName);
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
//    located at the front of the Geographic
//    Reference Table file.
//-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (grt_fd);

   tempChar = fgetc (grt_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (grt_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (grt_fd);
      }
}


//-------------------------------------------*/
// buildGeoReferenceTable
//
// Description:
//    This function will read the actual data
//    from the Geographic Reference Table file.
//    The data read will be used to populate the
//    attributes of the object.
//-------------------------------------------*/
static void buildGeoReferenceTable
               (GeoRefTableType *theGRT)

{
   theGRT->ID = gmsReadInteger (grt_fd);

   fgets(theGRT->dataType,
         sizeof(theGRT->dataType),
         grt_fd);

   fgets(theGRT->units,
         sizeof(theGRT->units),
         grt_fd);

   fgets(theGRT->ellipsoid,
         sizeof(theGRT->ellipsoid),
         grt_fd);

   fgets(theGRT->ellipsoidDetails,
         sizeof(theGRT->ellipsoidDetails),
         grt_fd);

   fgets(theGRT->datumVerticalReference,
         sizeof(theGRT->datumVerticalReference),
         grt_fd);

   fgets(theGRT->verticalDatumCode,
         sizeof(theGRT->verticalDatumCode),
         grt_fd);

   fgets(theGRT->soundingDatum,
         sizeof(theGRT->soundingDatum),
         grt_fd);

   fgets(theGRT->soundingDatumCode,
         sizeof(theGRT->soundingDatumCode),
         grt_fd);

   fgets(theGRT->datumGeodeticName,
         sizeof(theGRT->datumGeodeticName),
         grt_fd);

   fgets(theGRT->datumGeodeticCode,
         sizeof(theGRT->datumGeodeticCode),
         grt_fd);

   fgets(theGRT->projectionName,
         sizeof(theGRT->projectionName),
         grt_fd);
}
