//-----------------------------------------------------------------------------
// File : gmsDataTablesClass.cpp
// Date : 14-Jan-00 : initial definition
//        12-Feb-00 : "int" in const declaration - flagged by HP C++ compiler
//
// Description:
//    This class defines an object that is made up of tables of data from
//    the DCW database.  Specifically, an object of this class contains
//    the following tables of information:
//
//           Database Level Data:
//           --------------------
//              1) DHT - Database Header Table
//              2) LAT - Library Attribute Table
//
//           Library Level Data:
//           -------------------
//              1) CAT - Coverage Attribute Table
//              2) DQT - Data Quality Table
//              3) GRT - Geographic Reference Table
//              4) LHT - Library Header Table
//
//    The "library" tables apply to both REGIONAL and BROWSE libraries.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <string.h>
#include <gmsDataTablesClass.h>
#include <gmsTypesAndConstants.h>
#include <gmsUtilities.h>
#include <gmsDebugUtil.h>
#include <gmsDbHeaderTable.h>
#include <gmsLibAttribTable.h>
#include <gmsCoverageAttribTable.h>
#include <gmsDataQualityTable.h>
#include <gmsGeoReferenceTable.h>
#include <gmsLibHeaderTable.h>


//---------------------------------
// Define the real text 'object'
//---------------------------------


//---------------------------------
// Declare class objects
//---------------------------------

static DbHeaderTableType    *g_dht = NULL;

static LibAttribTableType   *g_lat = NULL;

static DataQualityTableType *g_dqt = NULL;

static DataQualityTableType *g_browseDQT = NULL;

static GeoRefTableType      *g_grt = NULL;

static GeoRefTableType      *g_browseGRT = NULL;

static libHeaderTableType   *g_lht = NULL;

static libHeaderTableType   *g_browseLHT = NULL;

static CovAttribTableType   *g_cat = NULL;

static CovAttribTableType   *g_browseCAT = NULL;

static const int            Size_Of_Buffer = 1024 * 1024;

static char                 g_charBuffer[Size_Of_Buffer];

#ifdef IS_UNIX

   static const char *CR = { "\n" };

#else

   static const char *CR = { "\r\n" };

#endif




//---------------------------------
// Declare Static Member Functions
//---------------------------------

static void buildDhtString
               (DbHeaderTableType *theDHT,
                char              *toBuffer);

static void buildLatString
               (LibAttribTableType *theLAT,
                char               *toBuffer);

static void buildDqtString
               (DataQualityTableType *theDQT,
                char                 *toBuffer);

static void buildGrtString
               (GeoRefTableType *theGRT,
                char            *toBuffer);

static void buildLhtString
               (libHeaderTableType *theLHT,
                char               *toBuffer);

static void buildCatString
               (CovAttribTableType *theCAT,
                char               *toBuffer);


//-----------------------------------------------
// gmsDataTablesClass
//
// DesCRiption:
//    Constructor.  CReates an object that
//    represents the data tables from a DCW
//    database.
//-----------------------------------------------
gmsDataTablesClass::gmsDataTablesClass ()

{
         static int id = 1;

   gmsDataTablesClass::theTables = (void *) &id;
}


//-----------------------------------------------
// ~gmsDataTablesClass
//
// DesCRiption:
//    Destructor.  Deletes the text object.
//-----------------------------------------------
gmsDataTablesClass::~gmsDataTablesClass ()

{
   if (gmsDataTablesClass::theTables == NULL)
      return;

   if (g_dht != NULL)
      gmsFreeDbHeaderTable (g_dht);

   if (g_lat != NULL)
      gmsFreeLibAttribTable (g_lat);

   if (g_dqt != NULL)
      gmsFreeDataQualityTable (g_dqt);

   if (g_browseDQT != NULL)
      gmsFreeDataQualityTable (g_browseDQT);

   if (g_grt != NULL)
      gmsFreeGeoReferenceTable (g_grt);

   if (g_browseGRT != NULL)
      gmsFreeGeoReferenceTable (g_browseGRT);

   if (g_lht != NULL)
      gmsFreeLibHeaderTable (g_lht);

   if (g_browseLHT != NULL)
      gmsFreeLibHeaderTable (g_browseLHT);

   if (g_browseCAT != NULL)
      gmsFreeCoverageAttribTable (g_browseCAT);

   if (g_cat != NULL)
      gmsFreeCoverageAttribTable (g_cat);

   gmsDataTablesClass::theTables = NULL;
}


//-----------------------------------------------
// gmsGet_DHT
//
// DesCRiption:
//    This method retrieves the "database header
//    table" from the DCW database.
//-----------------------------------------------
const char *gmsDataTablesClass::gmsGet_DHT ()

{
   if (g_dht == NULL)
      {
       g_dht = gmsGetDbHeaderTable (getDhtFile());
      }

   buildDhtString
      (g_dht,
       g_charBuffer);

   return g_charBuffer;
}


//-----------------------------------------------
// gmsGet_LAT
//
// DesCRiption:
//    This method retrieves the "library attribute
//    table" from the DCW database.
//-----------------------------------------------
const char *gmsDataTablesClass::gmsGet_LAT ()

{
   if (g_lat == NULL)
      {
       g_lat = gmsGetLibAttribTable (getLatFile());
      }

   buildLatString
      (g_lat,
       g_charBuffer);

   return g_charBuffer;
}


//-----------------------------------------------
// gmsGet_CAT
//
// DesCRiption:
//    This method retrieves the "Coverage Attribute
//    Table" from either the current REGIONAL library
//    or the BROWSE library.  A string that is the
//    data is returned to the caller.
//-----------------------------------------------
const char *gmsDataTablesClass::gmsGet_CAT
                             (bool isBrowse)

{
   if (isBrowse)
      {
       if (g_browseCAT == NULL)
          {
           g_browseCAT = gmsGetCoverageAttribTable
                            (gmsGetBrowseFullPath("CAT"));
          }

       buildCatString
              (g_browseCAT,
               g_charBuffer);
      }

   else
      {
       if (g_cat == NULL)
          {
           g_cat = gmsGetCoverageAttribTable
                            (gmsGetDcwFullPath("CAT"));
          }

       buildCatString
              (g_cat,
               g_charBuffer);
      }

   return g_charBuffer;
}


//-----------------------------------------------
// gmsGet_DQT
//
// DesCRiption:
//    This method retrieves the "Data Quality
//    Table" from either the current REGIONAL library
//    or the BROWSE library.  A string that is the
//    data is returned to the caller.
//-----------------------------------------------
const char *gmsDataTablesClass::gmsGet_DQT
                             (bool isBrowse)

{
   if (isBrowse)
      {
       if (g_browseDQT == NULL)
          {
           g_browseDQT = gmsGetDataQualityTable
                            (gmsGetBrowseFullPath("DQT"));
          }

       buildDqtString
              (g_browseDQT,
               g_charBuffer);
      }

   else
      {
       if (g_dqt == NULL)
          {
           g_dqt = gmsGetDataQualityTable
                            (gmsGetDcwFullPath("DQT"));
          }

       buildDqtString
              (g_dqt,
               g_charBuffer);
      }

   return g_charBuffer;
}


//-----------------------------------------------
// gmsGet_GRT
//
// DesCRiption:
//    This method retrieves the "Geographic Reference
//    Table" from either the current REGIONAL library
//    or the BROWSE library.  A string that is the
//    data is returned to the caller.
//-----------------------------------------------
const char *gmsDataTablesClass::gmsGet_GRT
                             (bool isBrowse)

{
   if (isBrowse)
      {
       if (g_browseGRT == NULL)
          {
           g_browseGRT = gmsGetGeoReferenceTable
                            (gmsGetBrowseFullPath("GRT"));
          }

       buildGrtString
              (g_browseGRT,
               g_charBuffer);
      }

   else
      {
       if (g_grt == NULL)
          {
           g_grt = gmsGetGeoReferenceTable
                            (gmsGetDcwFullPath("GRT"));
          }

       buildGrtString
              (g_grt,
               g_charBuffer);
      }

   return g_charBuffer;
}


//-----------------------------------------------
// gmsGet_LHT
//
// DesCRiption:
//    This method retrieves the "Library Header
//    Table" from either the current REGIONAL library
//    or the BROWSE library.  A string that is the
//    data is returned to the caller.
//-----------------------------------------------
const char *gmsDataTablesClass::gmsGet_LHT
                             (bool isBrowse)

{
   if (isBrowse)
      {
       if (g_browseLHT == NULL)
          {
           g_browseLHT = gmsGetLibHeaderTable
                            (gmsGetBrowseFullPath("LHT"));
          }

       buildLhtString
              (g_browseLHT,
               g_charBuffer);
      }

   else
      {
       if (g_lht == NULL)
          {
           g_lht = gmsGetLibHeaderTable
                            (gmsGetDcwFullPath("LHT"));
          }

       buildLhtString
              (g_lht,
               g_charBuffer);
      }

   return g_charBuffer;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// buildDhtString
//
// DesCRiption:
//-----------------------------------------------
static void buildDhtString
               (DbHeaderTableType *theDHT,
                char              *toBuffer)

{
         static char tempBuffer[256];

   gmsClearMemory
      (toBuffer, Size_Of_Buffer);

   if (theDHT == NULL)
      return;

   sprintf(toBuffer, "______________________________%s", CR);
   sprintf(tempBuffer, "Database Header Table:%s", CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> ID                 = %d%s",
          theDHT->ID, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Database Name      = %s%s",
          theDHT->databaseName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> DesCRiption        = %s%s",
          theDHT->databaseDesc, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Media Standard     = %s%s",
          theDHT->mediaStandard, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Originator         = %s%s",
          theDHT->originator, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Addressee          = %s%s",
          theDHT->addressee, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Media Volumes      = %c%s",
          theDHT->mediaVolumes, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Sequential Numbers = %c%s",
          theDHT->seqNumbers, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Number of Data Sets= %c%s",
          theDHT->numDataSets, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Security Classific = %c%s",
          theDHT->securityClass, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Downgrading        = %s%s",
          theDHT->downgrading, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Downgrade Date     = %s%s",
          theDHT->downgradeDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Releaseability     = %s%s",
          theDHT->releasability, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Other Std Name     = %s%s",
          theDHT->otherStdName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Other Std Date     = %s%s",
          theDHT->otherStdDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Other Std Version  = %s%s",
          theDHT->otherStdVersion, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Transmittal ID     = %c%s",
          theDHT->transmittalId, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Edition Number     = %s%s",
          theDHT->editionNumber, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Edition Date       = %s%s",
          theDHT->editionDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "______________________________%s", CR);
   strcat (toBuffer, tempBuffer);
}


//-----------------------------------------------
// buildLatString
//
// DesCRiption:
//-----------------------------------------------
static void buildLatString
               (LibAttribTableType *theLAT,
                char               *toBuffer)

{
         int       index;
         const int numRecords = 5;
         char      tempBuffer[64];

   gmsClearMemory
      (toBuffer, Size_Of_Buffer);

   sprintf(toBuffer, "______________________________%s", CR);
   sprintf(tempBuffer, "Library Attribute Table:%s", CR);
   strcat (toBuffer, tempBuffer);

   if (theLAT == NULL)
      return;

   for (index = 0; index < numRecords; index++)
      {
       sprintf(tempBuffer, "   ---> ID                 = %d%s",
              theLAT[0][index].ID, CR);
       strcat (toBuffer, tempBuffer);

       sprintf(tempBuffer, "   ---> libraryName        = %s%s",
              theLAT[0][index].libraryName, CR);
       strcat (toBuffer, tempBuffer);

       sprintf(tempBuffer, "   ---> Western-most Long  = %f%s",
              theLAT[0][index].westMostLong, CR);
       strcat (toBuffer, tempBuffer);

       sprintf(tempBuffer, "   ---> Southern-most Lat  = %f%s",
              theLAT[0][index].southMostLat, CR);
       strcat (toBuffer, tempBuffer);

       sprintf(tempBuffer, "   ---> Eastern-most Long  = %f%s",
              theLAT[0][index].eastMostLong, CR);
       strcat (toBuffer, tempBuffer);

       sprintf(tempBuffer, "   ---> Northern-most Lat  = %f%s",
              theLAT[0][index].northMostLat, CR);
       strcat (toBuffer, tempBuffer);

       sprintf(tempBuffer, "------------------%s", CR);
       strcat (toBuffer, tempBuffer);
      }
}



//-----------------------------------------------
// buildDqtString
//
// DesCRiption:
//-----------------------------------------------
static void buildDqtString
               (DataQualityTableType *theDQT,
                char                 *toBuffer)

{
         static char tempBuffer[256];

   gmsClearMemory
      (toBuffer, Size_Of_Buffer);

   sprintf(toBuffer, "______________________________%s", CR);
   sprintf(tempBuffer, "Data Quality Table:%s", CR);
   strcat (toBuffer, tempBuffer);

   if (theDQT == NULL)
      {
       return;
      }

   sprintf(tempBuffer, "   ---> ID                 = %d%s",
          theDQT->ID, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> VPF Level          = %s%s",
          theDQT->VPF_level, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> VPF Level Name     = %s%s",
          theDQT->VPF_levelName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Feature Complete %% = %s%s",
          theDQT->featureCompletePercent, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Attrib Complete %%  = %s%s",
          theDQT->attributeCompletePercent, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Logical Consistency= %s%s",
          theDQT->logicalConsistency, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Edition Num        = %s%s",
          theDQT->editionNumber, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> CReation Date      = %s%s",
          theDQT->creationDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Revision Date      = %s%s",
          theDQT->revisionDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Specification Name = %s%s",
          theDQT->specificationName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Specificaion Date  = %s%s",
          theDQT->specificationDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Earliest Src Date  = %s%s",
          theDQT->dateOfEarliestSrc, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Latest Src Date    = %s%s",
          theDQT->dateOfLatestSrc, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Std Dev of Quant Attrib = %s%s",
          theDQT->stdDevOfQuantAttrib, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> %% Relia Of Qual Att= %s%s",
          theDQT->percentReliabOfQualAttrib, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Collectn Spec Name = %s%s",
          theDQT->collectionSpecName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Name Included Src  = %s%s",
          theDQT->nameOfIncludedSrcFile, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Abs Horiz Accuracy = %s%s",
          theDQT->absoluteHorizAccuracy, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Abs Horiz Units    = %s%s",
          theDQT->absoluteHorizUnits, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Abs Vert Accuracy  = %s%s",
          theDQT->absoluteVertAccuracy, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Abs Vert Units     = %s%s",
          theDQT->absoluteVertUnits, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Relat Horiz Accur  = %s%s",
          theDQT->relativeHorizAccuracy, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Relat Horiz Units  = %s%s",
          theDQT->relativeHorizUnits, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Relat Vert Accur   = %s%s",
          theDQT->relativeVertAccuracy, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Relat Vert Units   = %s%s",
          theDQT->relativeVertUnits, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Comments           = %s%s",
          theDQT->comments, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "______________________________%s", CR);
   strcat (toBuffer, tempBuffer);
}



//-----------------------------------------------
// buildGrtString
//
// DesCRiption:
//-----------------------------------------------
static void buildGrtString
               (GeoRefTableType *theGRT,
                char            *toBuffer)

{
         static char tempBuffer[256];

   gmsClearMemory
      (toBuffer, Size_Of_Buffer);

   sprintf(toBuffer, "______________________________%s", CR);
   sprintf(tempBuffer, "Geographic Reference Table:%s", CR);
   strcat (toBuffer, tempBuffer);

   if (theGRT == NULL)
      return;

   sprintf(tempBuffer, "   ---> ID                 = %d%s",
          theGRT->ID, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Data Type          = %s%s",
          theGRT->dataType, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Units              = %s%s",
          theGRT->units, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Ellipsoid          = %s%s",
          theGRT->ellipsoid, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Ellipsoid Details  = %s%s",
          theGRT->ellipsoidDetails, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Datum Vertical Ref = %s%s",
          theGRT->datumVerticalReference, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Vertical Datum Code= %s%s",
          theGRT->verticalDatumCode, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Sounding Datuam    = %s%s",
          theGRT->soundingDatum, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Sounding Datum Code= %s%s",
          theGRT->soundingDatumCode, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Datum Geodetic Name= %s%s",
          theGRT->datumGeodeticName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Datum Geodetic Code= %s%s",
          theGRT->datumGeodeticCode, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Projection Name    = %s%s",
          theGRT->projectionName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "______________________________%s", CR);
   strcat (toBuffer, tempBuffer);
}



//-----------------------------------------------
// buildLhtString
//
// DesCRiption:
//-----------------------------------------------
static void buildLhtString
               (libHeaderTableType *theLHT,
                char               *toBuffer)

{
         static char tempBuffer[256];

   gmsClearMemory
      (toBuffer, Size_Of_Buffer);

   sprintf(toBuffer, "______________________________%s", CR);
   sprintf(tempBuffer, "Library Header Table:%s", CR);
   strcat (toBuffer, tempBuffer);

   if (theLHT == NULL)
      return;

   sprintf(tempBuffer, "   ---> ID             = %d%s",
          theLHT->ID, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Product Type   = %s%s",
          theLHT->productType, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Library Name   = %s%s",
          theLHT->libraryName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Description    = %s%s",
          theLHT->description, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Data Struc Code= %c%s",
          theLHT->dataStructCode, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Scale          = %d%s",
          theLHT->scale, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Source Series  = %s%s",
          theLHT->sourceSeries, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Source ID      = %s%s",
          theLHT->sourceID, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Source Edition = %s%s",
          theLHT->sourceEdition, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Source Name    = %s%s",
          theLHT->sourceName, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Source Date    = %s%s",
          theLHT->sourceDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Security Class = %c%s",
          theLHT->securityClassif, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Downgrading    = %s%s",
          theLHT->downgrading, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Downgrade Date = %s%s",
          theLHT->downgradeDate, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "   ---> Releasability  = %s%s",
          theLHT->releasability, CR);
   strcat (toBuffer, tempBuffer);

   sprintf(tempBuffer, "______________________________%s", CR);
   strcat (toBuffer, tempBuffer);
}


//-----------------------------------------------
// buildCatString
//
// DesCRiption:
//-----------------------------------------------
static void buildCatString
               (CovAttribTableType *theCAT,
                char               *toBuffer)

{
         int         i;
         int         numChars;
         static char tempBuffer[256];

   gmsClearMemory
      (toBuffer, Size_Of_Buffer);

   if (theCAT == NULL)
      return;

   sprintf(toBuffer,
           "______________________________%s", CR);

   sprintf(tempBuffer,
           "Coverage Attribute Table:%s", CR);

   strcat(toBuffer, tempBuffer);

   sprintf(tempBuffer,
           "Num Records = %d%s",
           theCAT->numRecords, CR);

   strcat(toBuffer, tempBuffer);

   sprintf(tempBuffer,
      "ID  Name       DesCRiption                               Level%s", CR);

   strcat(toBuffer, tempBuffer);

   for (i = 0; i < theCAT->numRecords; i++)
      {
       numChars = sprintf(tempBuffer,
                          " %d  %s %s %d%s",
                          theCAT->theCATRecords[i].ID,
                          theCAT->theCATRecords[i].coverageName,
                          theCAT->theCATRecords[i].description,
                          theCAT->theCATRecords[i].level,
                          CR);

       strcat(toBuffer, tempBuffer);
      }

   sprintf(tempBuffer,
           "______________________________%s", CR);

   strcat(toBuffer, tempBuffer);

   gmsWriteDebug (toBuffer);
}
