//-----------------------------------------------------------------------------
// File : gmsMapClass.cpp
// Date : 10-Nov-99 : initial definition
//        14-Nov-99 : move lat/long grid to separate class
//        09-Dec-99 : update due to file renames, etc
//        09-Jan-00 : export the edge table
//        19-Jan-00 : provide access to node table
//        20-Jan-00 : put switch for earth model
//        23-Jan-00 : put in then took out polygon functions
//        24-Jan-00 : take out export of 'edge' table.  Can't remember why.
//        24-Jan-00 : make use of '*LINE.LFT' table to clear tile boundaries
//        28-Jan-00 : make use of new transformation utils
//
// Description:
//    This class defines a "map" object.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <string.h>
#include <gmsMapClass.h>
#include <gmsNodeClass.h>
    //--------//
#include <gmsEdgeTable.h>
    //-------//
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDcwUtilities.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real map 'object'
//---------------------------------

typedef struct
   {
    gmsThematicType        whichTheme;
    gms_3D_WorldImageType  worldImage_3D;
    gms_2D_WorldImageType  flatWorldImage;
    gms_2D_ScreenImageType mapScreenImage_2D;
    gmsNodeClass           *nodes;
   } gmsMapStructType;


//---------------------------------
// Declare some constants
//---------------------------------

static const gms_3D_WorldImageType  Null_3D_World_Image = {0, NULL};

static const gms_2D_WorldImageType  Null_2D_World_Image = {0, NULL};

static const gms_2D_ScreenImageType Null_2D_ScreenImage = {0, NULL};


//---------------------------------
// Declare Static Member Functions
//---------------------------------

static void build_3D_WorldImage
               (edgeTableType    *edgTbl,
                gmsMapStructType *ptrToMap);

static void buildFlatWorldImage
               (edgeTableType    *edgTbl,
                gmsMapStructType *ptrToMap);

static void buildFiltered_3D_WorldImage
               (edgeTableType          *edgTbl,
                const featureTableType *featureTbl,
                gmsMapStructType       *ptrToMap);

static void buildFilteredFlatWorldImage
               (edgeTableType          *edgTbl,
                const featureTableType *featureTbl,
                gmsMapStructType       *ptrToMap);

static void buildMapScreenImage
               (gmsMapStructType *ptrToMap);

static void buildFlatMapScreenImage
               (gmsMapStructType *ptrToMap);

static void instantiateNodeObject
               (gmsMapStructType *ptrToMap,
                const char       *edgeTblName);




//-----------------------------------------------
// gmsMapClass
//
// Description:
//    Constructor.  Creates an object that
//    represents a map.  The specified edge table
//    is used 'as is'.  No modifications and
//    filtering is applied to the data from the
//    edge table.
//-----------------------------------------------
gmsMapClass::gmsMapClass
                (gmsEarthModelType whichModel,
                 const char        *edgeTblName)

{
         gmsMapStructType   *ptrToNewMap;
         edgeTableType      *edgTbl;

   ptrToNewMap = new gmsMapStructType;

   edgTbl = gmsGetEdgeTable (edgeTblName);

   if (edgTbl == NULL)
      {
       gmsWriteDebug ("*** ERROR : 'gmsGetEdgeTable' failed\n");

       gmsMapClass::theMap = NULL;

       return;
      }


   if (whichModel == gmsFlat)
      {
       ptrToNewMap->worldImage_3D = Null_3D_World_Image;

       ptrToNewMap->flatWorldImage =
          gmsAllocate_2D_FlatWorldImageFromEdgeTable ( *edgTbl );

       buildFlatWorldImage
          (edgTbl,
           ptrToNewMap);
      }

   else
      {
       ptrToNewMap->worldImage_3D =
          gmsAllocate_3D_WorldImageFromEdgeTable ( *edgTbl );

       build_3D_WorldImage
          (edgTbl,
           ptrToNewMap);

       ptrToNewMap->flatWorldImage = Null_2D_World_Image;
      }

   ptrToNewMap->mapScreenImage_2D =
      gmsAllocate_2D_ScreenImageFromEdgeTable ( *edgTbl );

   instantiateNodeObject
      (ptrToNewMap,
       edgeTblName);

   gmsFreeEdgeTable (edgTbl);

   gmsMapClass::theModel = whichModel;

   gmsMapClass::theMap = (void *) ptrToNewMap;
}


//-----------------------------------------------
// gmsMapClass
//
// Description:
//    Constructor.  Creates an object that
//    represents a map.  The 'lineFeatureTblName'
//    is used to access the corresponding *LINE.LFT
//    file.
//
//    This method makes use of the line feature
//    table for the following themes when building
//    the map image:
//
//            DN, HY, HS, PO, RR, RD, TS
//-----------------------------------------------
gmsMapClass::gmsMapClass
             (gmsEarthModelType      whichModel,
              const char             *edgeTblName,
              const featureTableType *lineFeatureTbl)

{
         gmsMapStructType *ptrToNewMap;
         edgeTableType    *edgTbl;

   ptrToNewMap = new gmsMapStructType;

   if (lineFeatureTbl == NULL)
      {
       gmsWriteDebug ("*** ERROR : gmsMapClass : LFT is NULL\n");

       gmsMapClass::theMap = NULL;

       return;
      }

   edgTbl = gmsGetEdgeTable (edgeTblName);

   if (edgTbl == NULL)
      {
             char msg[96];

       sprintf(msg, "*** ERROR : gmsMapClass : EDG table %s is NULL\n",
               edgeTblName);

       gmsWriteDebug (msg);

       gmsMapClass::theMap = NULL;

       return;
      }

   if (whichModel == gmsFlat)
      {
       ptrToNewMap->worldImage_3D = Null_3D_World_Image;

       ptrToNewMap->flatWorldImage =
          gmsAllocate_2D_FlatWorldImageFromEdgeTable ( *edgTbl );

       buildFilteredFlatWorldImage
          (edgTbl,
           lineFeatureTbl,
           ptrToNewMap);
      }

   else
      {
       ptrToNewMap->worldImage_3D =
          gmsAllocate_3D_WorldImageFromEdgeTable ( *edgTbl );

       buildFiltered_3D_WorldImage
          (edgTbl,
           lineFeatureTbl,
           ptrToNewMap);

       ptrToNewMap->flatWorldImage = Null_2D_World_Image;
      }

   ptrToNewMap->mapScreenImage_2D =
      gmsAllocate_2D_ScreenImageFromEdgeTable ( *edgTbl );

   instantiateNodeObject
      (ptrToNewMap,
       edgeTblName);

   gmsFreeEdgeTable (edgTbl);

   gmsMapClass::theModel = whichModel;

   gmsMapClass::theMap = (void *) ptrToNewMap;
}


//-----------------------------------------------
// ~gmsMapClass
//
// Description:
//    Destructor.  Deletes the map object.
//-----------------------------------------------
gmsMapClass::~gmsMapClass ()

{
         gmsMapStructType *ptrToMap;

   if (gmsMapClass::theMap == NULL)
      return;

   ptrToMap = (gmsMapStructType *) gmsMapClass::theMap;

   gmsFree_3D_WorldImage (&ptrToMap->worldImage_3D);

   gmsFree_2D_WorldImage (&ptrToMap->flatWorldImage);

   gmsFree_2D_ScreenImage (&ptrToMap->mapScreenImage_2D);

   if (ptrToMap->nodes != NULL)
      {
       ptrToMap->nodes->~gmsNodeClass();
      }

   delete ptrToMap;

   gmsMapClass::theMap = NULL;
}


//-----------------------------------------------
// gmsGetMapImage
//
// Description:
//    Retrieves a 2D screen image of the current
//    map object.
//-----------------------------------------------
gms_2D_ScreenImageType gmsMapClass::gmsGetMapImage ()

{
         gmsMapStructType *ptrToMap;

   if (gmsMapClass::theMap == NULL)
      {
       return Null_2D_ScreenImage;
      }

   ptrToMap = (gmsMapStructType *) gmsMapClass::theMap;

   if (gmsMapClass::theModel == gmsFlat)
      {
       buildFlatMapScreenImage (ptrToMap);
      }

   else
      {
       buildMapScreenImage (ptrToMap);
      }

   return ptrToMap->mapScreenImage_2D;
}


//-----------------------------------------------
// gmsGetMapNodePoints
//
// Description:
//    Returns an array of 2D points that represent
//    the entity nodes that correspond with the
//    edge table.  Although the returned struct
//    is a 'polyline', it actually represents
//    a series of independent points.
//-----------------------------------------------
gms_2D_ScreenPolylineType gmsMapClass::gmsGetMapNodePoints ()

{
         gmsMapStructType *ptrToMap;

   if (gmsMapClass::theMap == NULL)
      {
             gms_2D_ScreenPolylineType nullPoints = {0, NULL};

       return nullPoints;
      }

   ptrToMap = (gmsMapStructType *) gmsMapClass::theMap;

   return ptrToMap->nodes->gmsGetNodePoints();
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// build_3D_WorldImage
//
// Description:
//    This routine uses the (lat/long) coordinates
//    from the edge table to build a 3D world
//    image.  This world image has no reference
//    point to the screen.
//-----------------------------------------------
static void build_3D_WorldImage
               (edgeTableType    *edgTbl,
                gmsMapStructType *ptrToMap)

{
         int  i, j;

   ptrToMap->worldImage_3D.numLines = edgTbl->numRecords;

   for (i = 0; i < edgTbl->numRecords; i++)
      {
       ptrToMap->worldImage_3D.imageLines[i].numPoints =
          edgTbl->edgeRecords[i].numCoords;

       for (j = 0; j < edgTbl->edgeRecords[i].numCoords; j++)
          {
           ptrToMap->worldImage_3D.imageLines[i].points[j] =
              gmsConvertLatLongToEllipsoidPoint
                 (edgTbl->edgeRecords[i].theCoords[j]);
          }
      }
}


//-----------------------------------------------
// buildFlatWorldImage
//
// Description:
//    This routine uses the (lat/long) coordinates
//    from the edge table to build a 2D FLAT world
//    image.  This world image has no reference
//    point to the screen.
//-----------------------------------------------
static void buildFlatWorldImage
               (edgeTableType    *edgTbl,
                gmsMapStructType *ptrToMap)

{
         int                    i, j;
         gms_2D_WorldPointType  worldPoint;

   for (i = 0; i < edgTbl->numRecords; i++)
      for (j = 0; j < edgTbl->edgeRecords[i].numCoords; j++)
         {
          worldPoint = gmsConvertLatLongToFlatPoint
                               (edgTbl->edgeRecords[i].theCoords[j]);

          ptrToMap->flatWorldImage.imageLines[i].points[j] = worldPoint;
         }
}


//-----------------------------------------------
// buildFiltered_3D_WorldImage
//
// Description:
//-----------------------------------------------
static void buildFiltered_3D_WorldImage
               (edgeTableType          *edgTbl,
                const featureTableType *featureTbl,
                gmsMapStructType       *ptrToMap)

{
         int                       i, j;
         int                       LFT_FgnKey;
         const int                 Null_Value = -1;
         generalLineFeatureRecType *ptrToLFT;

   ptrToMap->worldImage_3D.numLines = edgTbl->numRecords;

   ptrToLFT = (generalLineFeatureRecType *) featureTbl->featureRecords;

   for (i = 0; i < edgTbl->numRecords; i++)
      {
       LFT_FgnKey = edgTbl->edgeRecords[i].headerInfo.fgnKeyToFeatureTbl;

       if (LFT_FgnKey == Null_Value)
          {
           ptrToMap->worldImage_3D.imageLines[i].numPoints = 0;

           continue;
          }

       LFT_FgnKey--; // we're C indexing here ...

       if (ptrToLFT[LFT_FgnKey].type == Null_Value)
          {
           ptrToMap->worldImage_3D.imageLines[i].numPoints = 0;

           continue;
          }

       if (featureTbl->whichTheme == gms_HY)
          {
           if ( (ptrToLFT[LFT_FgnKey].type == 99999) ||
                (ptrToLFT[LFT_FgnKey].type == 9) )
              {
               ptrToMap->worldImage_3D.imageLines[i].numPoints = 0;

               continue;
              }
          }

       ptrToMap->worldImage_3D.imageLines[i].numPoints =
             edgTbl->edgeRecords[i].numCoords;

       for (j = 0; j < edgTbl->edgeRecords[i].numCoords; j++)
          {
           ptrToMap->worldImage_3D.imageLines[i].points[j] =
              gmsConvertLatLongToEllipsoidPoint
                 (edgTbl->edgeRecords[i].theCoords[j]);
          }
      }
}


//-----------------------------------------------
// buildFilteredFlatWorldImage
//
// Description:
//-----------------------------------------------
static void buildFilteredFlatWorldImage
               (edgeTableType          *edgTbl,
                const featureTableType *featureTbl,
                gmsMapStructType       *ptrToMap)

{
         int                       i, j;
         gms_2D_WorldPointType     worldPoint;
         int                       LFT_FgnKey;
         const int                 Null_Value = -1;
         generalLineFeatureRecType *ptrToLFT;

   ptrToMap->flatWorldImage.numLines = edgTbl->numRecords;

   ptrToLFT = (generalLineFeatureRecType *) featureTbl->featureRecords;

   for (i = 0; i < edgTbl->numRecords; i++)
      {
       LFT_FgnKey = edgTbl->edgeRecords[i].headerInfo.fgnKeyToFeatureTbl;

       if (LFT_FgnKey == Null_Value)
          {
           ptrToMap->flatWorldImage.imageLines[i].numPoints = 0;

           continue;
          }

       LFT_FgnKey--; // C indexing here ...

       if (ptrToLFT[LFT_FgnKey].type == Null_Value)
          {
           ptrToMap->flatWorldImage.imageLines[i].numPoints = 0;

           continue;
          }

       if (featureTbl->whichTheme == gms_HY)
          {
           if ( (ptrToLFT[LFT_FgnKey].type == 99999) ||
                (ptrToLFT[LFT_FgnKey].type == 9) )
              {
               ptrToMap->flatWorldImage.imageLines[i].numPoints = 0;

               continue;
              }
          }

       ptrToMap->flatWorldImage.imageLines[i].numPoints =
             edgTbl->edgeRecords[i].numCoords;

       for (j = 0; j < edgTbl->edgeRecords[i].numCoords; j++)
          {
           worldPoint = gmsConvertLatLongToFlatPoint
                               (edgTbl->edgeRecords[i].theCoords[j]);

           ptrToMap->flatWorldImage.imageLines[i].points[j] = worldPoint;
          }
      }
}


//-----------------------------------------------
// buildMapScreenImage
//
// Description:
//    (Re) constructs the (non-flat) map image
//    based on the current zoom factor, rotations,
//    etc.
//-----------------------------------------------
static void buildMapScreenImage
               (gmsMapStructType *ptrToMap)

{
         gms_2D_ScreenPointType       screenPnt_2D;
         gms_3D_WorldPointType        worldPnt_3D;
         int                          numPoints;
         int                          i, j;
         bool                         isValid;
         gmsMapStateStructType        mapState;

   if (ptrToMap->worldImage_3D.numLines == 0)
      {
       gmsWriteDebug ("*** ERR - buildMapScreenImage : no lines in image\n");

       return;
      }

   mapState = gmsGetMapState ();

   for (i = 0; i < ptrToMap->worldImage_3D.numLines; i++)
      {
       numPoints = 0;

       for (j = 0; j < ptrToMap->worldImage_3D.imageLines[i].numPoints; j++)
          {
           worldPnt_3D = ptrToMap->worldImage_3D.imageLines[i].points[j];

           screenPnt_2D = gmsConvert_3D_WorldPointTo_2D_ScreenPoint
                             (worldPnt_3D,
                              isValid);

           if (isValid)
              {
               ptrToMap->mapScreenImage_2D.imageLines[i].points[numPoints] =
                  screenPnt_2D;

               numPoints++;
              }
          }

       ptrToMap->mapScreenImage_2D.imageLines[i].numPoints = numPoints;
      }
}


//-----------------------------------------------
// buildFlatMapScreenImage
//
// Description:
//    Constructs a 2D screen image based on the
//    current state of the map (zoom factor,
//    translation, etc).  The earth model is
//    for a FLAT map.
//-----------------------------------------------
static void buildFlatMapScreenImage
               (gmsMapStructType *ptrToMap)

{
         gms_2D_ScreenPointType screenPoint;
         int                    i, j;
         gmsMapStateStructType  mapState;

   mapState = gmsGetMapState ();

   for (i = 0; i < ptrToMap->flatWorldImage.numLines; i++)
      for (j = 0; j < ptrToMap->flatWorldImage.imageLines[i].numPoints; j++)
         {
          screenPoint = gmsConvert_2D_WorldPointTo_2D_ScreenPoint
                          (ptrToMap->flatWorldImage.imageLines[i].points[j]);

          ptrToMap->mapScreenImage_2D.imageLines[i].points[j] = screenPoint;
         }
}


//-----------------------------------------------
// instantiateNodeObject
//
// Description:
//    Instantiates a "node" object that corresponds
//    to the specified EDG table.
//-----------------------------------------------
static void instantiateNodeObject
               (gmsMapStructType *ptrToMap,
                const char       *edgeTblName)

{
         char *copy;
         int  len;

   len = strlen (edgeTblName);

   copy = new char[len + 4];

   sprintf (copy, "%s", edgeTblName);

   copy[len - 3] = 'E';
   copy[len - 2] = 'N';
   copy[len - 1] = 'D';

   ptrToMap->nodes = new gmsNodeClass (gmsEllipsoid, copy);

   delete [] copy;
}
