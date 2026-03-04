//-----------------------------------------------------------------------------
// File : gmsBrowseMapClass.cpp
// Date : 12-Jan-00 : initial definition
//        25-Jan-00 : make use of 'gmsDcwTypesAndConstants.h'
//
// Description:
//    This class defines a browse "map" object.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <gmsBrowseMapClass.h>
#include <gmsMapClass.h>
#include <gmsLatLongGridClass.h>
#include <gmsNodeClass.h>
#include <gmsMbrClass.h>
#include <gmsDataTablesClass.h>
#include <gmsUtilities.h>
#include <gmsFileNameUtilities.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real map 'objects'
//---------------------------------

typedef struct
   {
    gmsLatLongGridClass    *latLongGrid;
    gmsMapClass            *libRef;
    gmsMapClass            *poMap;
    gmsMapClass            *coMap;
    gmsMapClass            *daMap;
    gmsMapClass            *dnMap;
    gmsMapClass            *dvMap;
    gmsMapClass            *grMap;
    gmsMapClass            *inMap;
   } gmsBrowseMapsStructType;

typedef struct  // should really be a union struct
   {
    gmsBrowseFilesListType      *fileList;
    gmsBrowseMapsStructType     maps;
    gmsNodeClass                *ppNodes;
    gmsTextClass                *ppText;
    gmsTextClass                *poText;
    gmsTextClass                *inText;
    gmsTextClass                *grText;
    gmsMbrClass                 *poEbr;
    gmsMbrClass                 *inEbr;
    gmsMbrClass                 *grEbr;
    gmsMbrClass                 *dvEbr;
    gmsMbrClass                 *dnEbr;
    gmsMbrClass                 *daEbr;
    gmsMbrClass                 *coEbr;
    gmsMbrClass                 *poFbr;
    gmsMbrClass                 *inFbr;
    gmsMbrClass                 *grFbr;
    gmsMbrClass                 *dvFbr;
    gmsMbrClass                 *dnFbr;
    gmsMbrClass                 *daFbr;
    gmsMbrClass                 *coFbr;
    gmsDataTablesClass          *dataTbls;
   } gmsBrowseStructType;


//---------------------------------
// Declare some constants
//---------------------------------
static const gms_2D_ScreenImageType Null_2D_Image = {0, NULL};


//---------------------------------
// Declare Static Member Functions
//---------------------------------
static void instantiateDataObjects
               (gmsEarthModelType   whichModel,
                gmsBrowseStructType *browseObj);

static void deallocateDataObjects
               (gmsBrowseStructType *browseObj);

static void instantiateMapObjects
               (gmsEarthModelType       whichModel,
                gmsBrowseMapsStructType &mapObj);

static void deallocateMapObjects
               (gmsBrowseMapsStructType &mapObj);

static gms_2D_ScreenImageType retrieveMapImage
                                 (gmsBrowseMapsStructType mapObj,
                                  gmsBrowseThematicType   whichTheme);




//-----------------------------------------------
// gmsBrowseMapClass
//
// Description:
//    Constructor.  Creates an object that
//    represents a map based on the BROWSE
//    library.
//-----------------------------------------------
gmsBrowseMapClass::gmsBrowseMapClass
                      (gmsEarthModelType whichModel)

{
         gmsBrowseStructType *ptrToNewMap = NULL;

   gmsBrowseMapClass::theModel = whichModel;

   ptrToNewMap = new gmsBrowseStructType;

   if (ptrToNewMap == NULL)
      {
       gmsWriteDebug("*** ERR : gmsBrowseMapClass : 'new' FAIL\n");

       gmsBrowseMapClass::theBrowseMap = NULL;

       return; 
      }

   instantiateDataObjects
      (whichModel,
       ptrToNewMap);

   instantiateMapObjects
      (whichModel,
       ptrToNewMap->maps);

   gmsBrowseMapClass::theBrowseMap = (void *) ptrToNewMap;
}


//-----------------------------------------------
// ~gmsBrowseMapClass
//
// Description:
//    Destructor.  Deletes the (BROWSE) map
//    object.
//-----------------------------------------------
gmsBrowseMapClass::~gmsBrowseMapClass ()

{
         gmsBrowseStructType *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      return;

   deallocateDataObjects (ptrToMap);

   deallocateMapObjects (ptrToMap->maps);

   delete ptrToMap;

   gmsBrowseMapClass::theBrowseMap = NULL;
}


//-----------------------------------------------
// gmsGetBrowseMapImage
//
// Description:
//    Returns a 2D image of the edge table from
//    the specified BROWSE theme.
//-----------------------------------------------
gms_2D_ScreenImageType gmsBrowseMapClass::gmsGetBrowseMapImage
                                    (gmsBrowseThematicType whichTheme)

{
         gmsBrowseStructType    *ptrToMap;
         gms_2D_ScreenImageType tempMapImage;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetBrowseMapImage - null obj\n");

       return Null_2D_Image;
      }

   tempMapImage = retrieveMapImage
                     (ptrToMap->maps,
                      whichTheme);

   return tempMapImage;
}


//-----------------------------------------------
// gmsGetLatitudeGrid
//
// Description:
//    Returns a 2D image of the latitude grid for
//    the map.
//-----------------------------------------------
gms_2D_ScreenImageType gmsBrowseMapClass::gmsGetLatitudeGrid ()

{
         gmsBrowseStructType *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetLatitudeGrid - null obj\n");

       return Null_2D_Image;
      }

   return ptrToMap->maps.latLongGrid->gmsGetLatitudeGrid ();
}


//-----------------------------------------------
// gmsGetLongitudeGrid
//
// Description:
//    Returns a 2D image of the longitude grid for
//    the map.
//-----------------------------------------------
gms_2D_ScreenImageType gmsBrowseMapClass::gmsGetLongitudeGrid ()

{
         gmsBrowseStructType *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetLongitudeGrid - null obj\n");

       return Null_2D_Image;
      }

   return ptrToMap->maps.latLongGrid->gmsGetLongitudeGrid ();
}


//-----------------------------------------------
// gmsGetBrowseNodes
//
// Description:
//    Returns an array of 2D points that were
//    constructed from the node table of the
//    specified BROWSE theme.
//-----------------------------------------------
gms_2D_ScreenPolylineType gmsBrowseMapClass::gmsGetBrowseNodes
                             (gmsBrowseThematicType whichTheme)

{
         const gms_2D_ScreenPolylineType noNodes = {0, NULL};
         gmsBrowseStructType             *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetBrowseNodes - null obj\n");

       return noNodes;
      }

   if (whichTheme == gmsBrowse_PP)
      {
       if (ptrToMap->ppNodes == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseNodes - PP nodes null.\n");

           return noNodes;
          }

       return ptrToMap->ppNodes->gmsGetNodePoints ();
      }

   return noNodes;
}


//-----------------------------------------------
// gmsGetTextItems
//
// Description:
//    This method retrieves the text strings
//    from the specified BROWSE theme.  These
//    strings are used to annotate the map.
//-----------------------------------------------
gmsMapTextArrayType gmsBrowseMapClass::gmsGetTextItems
                          (gmsBrowseThematicType whichTheme)

{
         const gmsMapTextArrayType noText = {0, NULL};
         gmsBrowseStructType       *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetTextItems - null obj\n");

       return noText;
      }

   if (whichTheme == gmsBrowse_PP)
      {
       if (ptrToMap->ppText == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetTextItems - PP text null.\n");

           return noText;
          }

       return ptrToMap->ppText->gmsGetTextItems ();
      }

   if (whichTheme == gmsBrowse_PO)
      {
       if (ptrToMap->poText == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetTextItems - PO text null.\n");

           return noText;
          }

       return ptrToMap->poText->gmsGetTextItems ();
      }

   if (whichTheme == gmsBrowse_IN)
      {
       if (ptrToMap->inText == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetTextItems - IN text null.\n");

           return noText;
          }

       return ptrToMap->inText->gmsGetTextItems ();
      }

   if (whichTheme == gmsBrowse_GR)
      {
       if (ptrToMap->grText == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetTextItems - GR text null.\n");

           return noText;
          }

       return ptrToMap->grText->gmsGetTextItems ();
      }

   return noText;
}


//-----------------------------------------------
// gmsGetBrowseEbrImage
//
// Description:
//    Returns the "Edge-Bounding-Rectangles" from
//    the specified browse theme.
//-----------------------------------------------
gms_2D_ScreenImageType gmsBrowseMapClass::gmsGetBrowseEbrImage
                                    (gmsBrowseThematicType whichTheme)

{
         gmsBrowseStructType *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - null obj\n");

       return Null_2D_Image;
      }

   if (whichTheme == gmsBrowse_PO)
      {
       if (ptrToMap->poEbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - PO null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->poEbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_IN)
      {
       if (ptrToMap->inEbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - IN null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->inEbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_GR)
      {
       if (ptrToMap->grEbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - GR null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->grEbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_DV)
      {
       if (ptrToMap->dvEbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - DV null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->dvEbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_DN)
      {
       if (ptrToMap->dnEbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - DN null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->dnEbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_DA)
      {
       if (ptrToMap->daEbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - DA null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->daEbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_CO)
      {
       if (ptrToMap->coEbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseEbrImage - CO null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->coEbr->gmsGetMbrImage ();
      }

   return Null_2D_Image;
}


//-----------------------------------------------
// gmsGetBrowseFbrImage
//
// Description:
//    Returns the "Face-Bounding-Rectangles" from
//    the specified browse theme.
//-----------------------------------------------
gms_2D_ScreenImageType gmsBrowseMapClass::gmsGetBrowseFbrImage
                          (gmsBrowseThematicType whichTheme)

{
         gmsBrowseStructType *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (ptrToMap == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - null obj\n");

       return Null_2D_Image;
      }

   if (whichTheme == gmsBrowse_PO)
      {
       if (ptrToMap->poFbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - PO null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->poFbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_IN)
      {
       if (ptrToMap->inFbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - IN null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->inFbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_GR)
      {
       if (ptrToMap->grFbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - GR null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->grFbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_DV)
      {
       if (ptrToMap->dvFbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - DV null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->dvFbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_DN)
      {
       if (ptrToMap->dnFbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - DN null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->dnFbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_DA)
      {
       if (ptrToMap->daFbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - DA null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->daFbr->gmsGetMbrImage ();
      }

   if (whichTheme == gmsBrowse_CO)
      {
       if (ptrToMap->coFbr == NULL)
          {
           gmsWriteDebug ("*** ERR : gmsGetBrowseFbrImage - CO null.\n");

           return Null_2D_Image;
          }

       return ptrToMap->coFbr->gmsGetMbrImage ();
      }

   return Null_2D_Image;
}


//-----------------------------------------------
// gmsGetDataTable
//
// Description:
//    This method retrieves a text string that
//    is the data from one of the data tables
//    from BROWSE library.
//-----------------------------------------------
const char *gmsBrowseMapClass::gmsGetDataTable
              (gmsDataTablesType whichTable)

{
         gmsBrowseStructType *ptrToMap;

   ptrToMap = (gmsBrowseStructType *) gmsBrowseMapClass::theBrowseMap;

   if (whichTable == gms_DatabaseHeaderTable)
      {
       return ptrToMap->dataTbls->gmsGet_DHT ();
      }

   else if (whichTable == gms_LibraryAttributeTable)
      {
       return ptrToMap->dataTbls->gmsGet_LAT ();
      }

   else if (whichTable == gms_CoverageAttributeTable)
      {
       return ptrToMap->dataTbls->gmsGet_CAT (true);
      }

   else if (whichTable == gms_DataQualityTable)
      {
       return ptrToMap->dataTbls->gmsGet_DQT (true);
      }

   else if (whichTable == gms_GeographicReferenceTable)
      {
       return ptrToMap->dataTbls->gmsGet_GRT (true);
      }

   else // whichTable == gms_LibraryHeaderTable
      {
       return ptrToMap->dataTbls->gmsGet_LHT (true);
      }
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// instantiateDataObjects
//
// Description:
//-----------------------------------------------
static void instantiateDataObjects
               (gmsEarthModelType   whichModel,
                gmsBrowseStructType *browseObj)

{
   browseObj->ppNodes = new gmsNodeClass
                                (whichModel,
                                 gmsGetBrowseFullPath("pp/end"));

   browseObj->ppText = new gmsTextClass
                                (whichModel,
                                 gmsGetBrowseFullPath("pp/txt"));

   browseObj->poText = new gmsTextClass
                                (whichModel,
                                 gmsGetBrowseFullPath("po/txt"));

   browseObj->inText = new gmsTextClass
                                (whichModel,
                                 gmsGetBrowseFullPath("in/txt"));

   browseObj->grText = new gmsTextClass
                                (whichModel,
                                 gmsGetBrowseFullPath("gr/txt"));

   browseObj->poEbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("po/ebr"));

   browseObj->inEbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("in/ebr"));

   browseObj->grEbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("gr/ebr"));

   browseObj->dvEbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("dv/ebr"));

   browseObj->dnEbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("dn/ebr"));

   browseObj->daEbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("da/ebr"));

   browseObj->coEbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("co/ebr"));

   browseObj->poFbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("po/fbr"));

   browseObj->inFbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("in/fbr"));

   browseObj->grFbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("gr/fbr"));

   browseObj->dvFbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("dv/fbr"));

   browseObj->dnFbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("dn/fbr"));

   browseObj->daFbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("da/fbr"));

   browseObj->coFbr = new gmsMbrClass
                             (whichModel,
                              gmsGetBrowseFullPath("co/fbr"));

   browseObj->dataTbls = new gmsDataTablesClass ();
}


//-----------------------------------------------
// deallocateDataObjects
//
// Description:
//-----------------------------------------------
static void deallocateDataObjects
               (gmsBrowseStructType *browseObj)

{
   if (browseObj->ppNodes != NULL)
      browseObj->ppNodes->~gmsNodeClass ();

   if (browseObj->ppText != NULL)
      browseObj->ppText->~gmsTextClass ();

   if (browseObj->poText)
      browseObj->poText->~gmsTextClass ();

   if (browseObj->inText)
      browseObj->inText->~gmsTextClass ();

   if (browseObj->grText)
      browseObj->grText->~gmsTextClass ();

   if (browseObj->poEbr)
      browseObj->poEbr->~gmsMbrClass ();

   if (browseObj->inEbr)
      browseObj->inEbr->~gmsMbrClass ();

   if (browseObj->grEbr)
      browseObj->grEbr->~gmsMbrClass ();

   if (browseObj->dvEbr)
      browseObj->dvEbr->~gmsMbrClass ();

   if (browseObj->dnEbr)
      browseObj->dnEbr->~gmsMbrClass ();

   if (browseObj->daEbr)
      browseObj->daEbr->~gmsMbrClass ();

   if (browseObj->coEbr)
      browseObj->coEbr->~gmsMbrClass ();

   if (browseObj->poFbr)
      browseObj->poFbr->~gmsMbrClass ();

   if (browseObj->inFbr)
      browseObj->inFbr->~gmsMbrClass ();

   if (browseObj->grFbr)
      browseObj->grFbr->~gmsMbrClass ();

   if (browseObj->dvFbr)
      browseObj->dvFbr->~gmsMbrClass ();

   if (browseObj->dnFbr)
      browseObj->dnFbr->~gmsMbrClass ();

   if (browseObj->daFbr)
      browseObj->daFbr->~gmsMbrClass ();

   if (browseObj->coFbr)
      browseObj->coFbr->~gmsMbrClass ();

   if (browseObj->dataTbls != NULL)
      browseObj->dataTbls->~gmsDataTablesClass ();
}


//-----------------------------------------------
// instantiateMapObjects
//
// Description:
//-----------------------------------------------
static void instantiateMapObjects
               (gmsEarthModelType       whichModel,
                gmsBrowseMapsStructType &mapObj)

{
   mapObj.latLongGrid = new gmsLatLongGridClass (15.0, whichModel);

   mapObj.libRef = new gmsMapClass
                          (whichModel,
                           gmsGetBrowseFullPath("libref/edg"));

   mapObj.poMap = new gmsMapClass
                          (whichModel,
                           gmsGetBrowseFullPath("po/edg"));

   mapObj.coMap = new gmsMapClass
                          (whichModel,
                           gmsGetBrowseFullPath("co/edg"));

   mapObj.daMap = new gmsMapClass
                          (whichModel,
                           gmsGetBrowseFullPath("da/edg"));

   mapObj.dnMap = new gmsMapClass
                          (whichModel,
                           gmsGetBrowseFullPath("dn/edg"));

   mapObj.dvMap = new gmsMapClass
                         (whichModel,
                          gmsGetBrowseFullPath("dv/edg"));

   mapObj.grMap = new gmsMapClass
                          (whichModel,
                           gmsGetBrowseFullPath("gr/edg"));

   mapObj.inMap = new gmsMapClass
                          (whichModel,
                           gmsGetBrowseFullPath("in/edg"));
}


//-----------------------------------------------
// deallocateMapObjects
//
// Description:
//-----------------------------------------------
static void deallocateMapObjects
               (gmsBrowseMapsStructType &mapObj)

{
   if (mapObj.latLongGrid != NULL)
      mapObj.latLongGrid->~gmsLatLongGridClass ();

   if (mapObj.libRef != NULL)
      mapObj.libRef->~gmsMapClass ();

   if (mapObj.poMap != NULL)
      mapObj.poMap->~gmsMapClass ();

   if (mapObj.coMap != NULL)
      mapObj.coMap->~gmsMapClass ();

   if (mapObj.daMap != NULL)
      mapObj.daMap->~gmsMapClass ();

   if (mapObj.dnMap != NULL)
      mapObj.dnMap->~gmsMapClass ();

   if (mapObj.dvMap != NULL)
      mapObj.dvMap->~gmsMapClass ();

   if (mapObj.grMap != NULL)
      mapObj.grMap->~gmsMapClass ();

   if (mapObj.inMap != NULL)
      mapObj.inMap->~gmsMapClass ();
}


//-----------------------------------------------
// retrieveMapImage
//
// Description:
//-----------------------------------------------
static gms_2D_ScreenImageType retrieveMapImage
                                 (gmsBrowseMapsStructType mapObj,
                                  gmsBrowseThematicType   whichTheme)

{
   if (whichTheme == gmsBrowse_LibRef)
      {
       if (mapObj.libRef == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - libref null.\n");

           return Null_2D_Image;
          }

       return mapObj.libRef->gmsGetMapImage ();
      }

   if (whichTheme == gmsBrowse_PO)
      {
       if (mapObj.poMap == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - PO null.\n");

           return Null_2D_Image;
          }

       return mapObj.poMap->gmsGetMapImage ();
      }

   if (whichTheme == gmsBrowse_CO)
      {
       if (mapObj.coMap == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - CO null.\n");

           return Null_2D_Image;
          }

       return mapObj.coMap->gmsGetMapImage ();
      }

   if (whichTheme == gmsBrowse_DA)
      {
       if (mapObj.daMap == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - DA null.\n");

           return Null_2D_Image;
          }

       return mapObj.daMap->gmsGetMapImage ();
      }

   if (whichTheme == gmsBrowse_DN)
      {
       if (mapObj.dnMap == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - DN null.\n");

           return Null_2D_Image;
          }

       return mapObj.dnMap->gmsGetMapImage ();
      }

   if (whichTheme == gmsBrowse_DV)
      {
       if (mapObj.dvMap == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - DV null.\n");

           return Null_2D_Image;
          }

       return mapObj.dvMap->gmsGetMapImage ();
      }

   if (whichTheme == gmsBrowse_GR)
      {
       if (mapObj.grMap == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - GR null.\n");

           return Null_2D_Image;
          }

       return mapObj.grMap->gmsGetMapImage ();
      }

   if (whichTheme == gmsBrowse_IN)
      {
       if (mapObj.inMap == NULL)
          {
           gmsWriteDebug ("*** ERR : retrieveMapImage - IN null.\n");

           return Null_2D_Image;
          }

       return mapObj.inMap->gmsGetMapImage ();
      }

   return Null_2D_Image;
}
