//-----------------------------------------------------------------------------
// File : gms_PO_PolygonMapClass.cpp
// Date : 23-Jan-00 : initial definition
//        30-Jan-00 : rename.  For "PO" theme only
//        04-Feb-00 : 'free' polygon info struct
//
// Description:
//    This class defines a "map of polygons" from the PO theme.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <string.h>
#include <gms_PO_PolygonMapClass.h>
#include <gmsEdgeTable.h>
#include <gmsRingTable.h>
#include <gmsFaceTable.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsPolygonUtilities.h>
#include <gmsMapStateMgr.h>
#include <gmsDcwUtilities.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real map 'object'
//---------------------------------

typedef struct
   {
    gms_3D_WorldImageType  ellipsoidLandAreas;
    gms_3D_WorldImageType  ellipsoidOceanAreas;
    gms_2D_WorldImageType  flatLandAreas;
    gms_2D_WorldImageType  flatOceanAreas;
    gms_2D_ScreenImageType screenLandAreas;
    gms_2D_ScreenImageType screenOceanAreas;
   } gmsPolygonMapStructType;


//---------------------------------
// Declare some constants
//---------------------------------

static const gms_3D_WorldImageType  Null_3D_World_Image = {0, NULL};

static const gms_2D_WorldImageType  Null_2D_World_Image = {0, NULL};

static const gms_2D_ScreenImageType Null_2D_Screen_Image = {0, NULL};


//---------------------------------
// Declare Static Member Functions
//---------------------------------

static ringTableType *getPtrToRingTable
                         (const char *edgTblName);

static faceTableType *getPtrToFaceTable
                         (const char *edgTblName);

static void buildScreenImage
               (gms_3D_WorldImageType  fromWorldImage,
                gms_2D_ScreenImageType &toScreenImage);

static void buildFlatScreenImage
               (gms_2D_WorldImageType  fromFlatWorldImage,
                gms_2D_ScreenImageType &toScreenImage);



//-----------------------------------------------
// gms_PO_PolygonMapClass
//
// Description:
//    Constructor.  Creates an object that
//    represents polygons from the edge table
//    from the PO theme.
//-----------------------------------------------
gms_PO_PolygonMapClass::gms_PO_PolygonMapClass
                (gmsEarthModelType whichModel,
                 const char        *edgeTblName,
                 featureTableType  *areaFeatureTbl)

{
         gmsPolygonMapStructType *ptrToNewMap;
         gmsPolygonInfoType      landPolygonInfo;
         gmsPolygonInfoType      oceanPolygonInfo;
         ringTableType           *tempRngTbl;
         faceTableType           *tempFacTbl;
         edgeTableType           *tempEdgTbl;

   if ( (edgeTblName == NULL) || (areaFeatureTbl == NULL) )
      {
       gms_PO_PolygonMapClass::thePolygonMap = (void *) NULL;

       return;
      }

   ptrToNewMap = new gmsPolygonMapStructType;

   tempEdgTbl = gmsGetEdgeTable (edgeTblName);

   tempRngTbl = getPtrToRingTable (edgeTblName);

   tempFacTbl = getPtrToFaceTable (edgeTblName);

   if ( (ptrToNewMap == NULL) || (tempEdgTbl == NULL)  ||
        (tempRngTbl == NULL)  || (tempFacTbl == NULL) )
      {
       gmsWriteDebug ("*** ERROR : 'gms_PO_PolygonMapClass' failed\n");

       gms_PO_PolygonMapClass::thePolygonMap = NULL;

       return;
      }

            const int Land_Id  = 1;
            const int Ocean_Id = 2;

   landPolygonInfo = gmsGet_PO_PolygonInfoFromEdgeTable
                         (tempEdgTbl,
                          tempRngTbl,
                          tempFacTbl,
                          areaFeatureTbl,
                          Land_Id);

   oceanPolygonInfo = gmsGet_PO_PolygonInfoFromEdgeTable
                         (tempEdgTbl,
                          tempRngTbl,
                          tempFacTbl,
                          areaFeatureTbl,
                          Ocean_Id);

   if (whichModel == gmsFlat)
      {
       ptrToNewMap->ellipsoidLandAreas = Null_3D_World_Image;

       ptrToNewMap->ellipsoidOceanAreas = Null_3D_World_Image;

       ptrToNewMap->flatLandAreas =
          gmsBuildFlatWorldPolygonsFromEdgeTable
             (landPolygonInfo,
              tempEdgTbl);

       ptrToNewMap->flatOceanAreas =
          gmsBuildFlatWorldPolygonsFromEdgeTable
             (oceanPolygonInfo,
              tempEdgTbl);
      }

   else
      {
       ptrToNewMap->ellipsoidLandAreas =
          gmsBuildWorldPolygonsFromEdgeTable
             (landPolygonInfo,
              tempEdgTbl);

       ptrToNewMap->ellipsoidOceanAreas =
          gmsBuildWorldPolygonsFromEdgeTable
             (oceanPolygonInfo,
              tempEdgTbl);

       ptrToNewMap->flatLandAreas = Null_2D_World_Image;

       ptrToNewMap->flatOceanAreas = Null_2D_World_Image;
      }

   ptrToNewMap->screenLandAreas =
       gmsAllocateScreenPolygonsFromEdgeTable
          (landPolygonInfo,
           tempEdgTbl);

   ptrToNewMap->screenOceanAreas =
       gmsAllocateScreenPolygonsFromEdgeTable
          (oceanPolygonInfo,
           tempEdgTbl);

   gms_PO_PolygonMapClass::theModel = whichModel;

   gms_PO_PolygonMapClass::thePolygonMap = (void *) ptrToNewMap;

   gmsFreePolygonInfo (&landPolygonInfo);

   gmsFreePolygonInfo (&oceanPolygonInfo);

   gmsFreeEdgeTable (tempEdgTbl);

   gmsFreeRingTable (tempRngTbl);

   gmsFreeFaceTable (tempFacTbl);
}


//-----------------------------------------------
// ~gms_PO_PolygonMapClass
//
// Description:
//    Destructor.  Deletes the map object.
//-----------------------------------------------
gms_PO_PolygonMapClass::~gms_PO_PolygonMapClass ()

{
         gmsPolygonMapStructType *ptrToMap;

   if (gms_PO_PolygonMapClass::thePolygonMap == NULL)
      return;

   ptrToMap =
      (gmsPolygonMapStructType *) gms_PO_PolygonMapClass::thePolygonMap;

   gmsFree_3D_WorldImage (&ptrToMap->ellipsoidLandAreas);

   gmsFree_3D_WorldImage (&ptrToMap->ellipsoidOceanAreas);

   gmsFree_2D_WorldImage (&ptrToMap->flatLandAreas);

   gmsFree_2D_WorldImage (&ptrToMap->flatOceanAreas);

   gmsFree_2D_ScreenImage (&ptrToMap->screenLandAreas);

   gmsFree_2D_ScreenImage (&ptrToMap->screenOceanAreas);

   delete ptrToMap;

   gms_PO_PolygonMapClass::thePolygonMap = NULL;
}


//-----------------------------------------------
// gmsGetLandAreas
//
// Description:
//    Retrieves a 2D screen image of polygons of
//    LAND areas.  The lines in the returned data
//    structure represent one or more polygons.
//    Each polygon represents a land area
//-----------------------------------------------
gms_2D_ScreenImageType gms_PO_PolygonMapClass::gmsGetLandAreas ()

{
         gmsPolygonMapStructType *ptrToMap;

   if (gms_PO_PolygonMapClass::thePolygonMap == NULL)
      return Null_2D_Screen_Image;

   ptrToMap =
      (gmsPolygonMapStructType *) gms_PO_PolygonMapClass::thePolygonMap;

   if (gms_PO_PolygonMapClass::theModel == gmsFlat)
      {
       buildFlatScreenImage
          (ptrToMap->flatLandAreas,
           ptrToMap->screenLandAreas);
      }

   else
      {
       buildScreenImage
          (ptrToMap->ellipsoidLandAreas,
           ptrToMap->screenLandAreas);
      }

   return ptrToMap->screenLandAreas;
}


//-----------------------------------------------
// gmsGetOceanAreas
//
// Description:
//    Retrieves a 2D screen image of polygons of
//    OCEAN areas.  The lines in the returned data
//    structure represent one or more polygons.
//    Each polygon represents a part of an ocean.
//-----------------------------------------------
gms_2D_ScreenImageType gms_PO_PolygonMapClass::gmsGetOceanAreas ()

{
         gmsPolygonMapStructType *ptrToMap;

   if (gms_PO_PolygonMapClass::thePolygonMap == NULL)
      return Null_2D_Screen_Image;

   ptrToMap =
      (gmsPolygonMapStructType *) gms_PO_PolygonMapClass::thePolygonMap;

   if (gms_PO_PolygonMapClass::theModel == gmsFlat)
      {
       buildFlatScreenImage
          (ptrToMap->flatOceanAreas,
           ptrToMap->screenOceanAreas);
      }

   else
      {
       buildScreenImage
          (ptrToMap->ellipsoidOceanAreas,
           ptrToMap->screenOceanAreas);
      }

   return ptrToMap->screenOceanAreas;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// getPtrToRingTable
//
// Description:
//-----------------------------------------------
static ringTableType *getPtrToRingTable
                         (const char *edgTblName)

{
         int           length;
         char          *rngTblName;
         ringTableType *ptrToRngTbl;

   if (edgTblName == NULL)
      return (ringTableType *) NULL;

   length = strlen(edgTblName);

   rngTblName = new char[length + 4];

   sprintf(rngTblName, "%s", edgTblName);

   rngTblName[length - 3] = 'R';
   rngTblName[length - 2] = 'N';
   rngTblName[length - 1] = 'G';

   ptrToRngTbl = gmsGetRingTable (rngTblName);

   delete [] rngTblName;

   return ptrToRngTbl;
}


//-----------------------------------------------
// getPtrToFaceTable
//
// Description:
//-----------------------------------------------
static faceTableType *getPtrToFaceTable
                         (const char *edgTblName)

{
         int           length;
         char          *facTblName;
         faceTableType *ptrToFacTbl;

   if (edgTblName == NULL)
      return (faceTableType *) NULL;

   length = strlen(edgTblName);

   facTblName = new char[length + 4];

   sprintf(facTblName, "%s", edgTblName);

   facTblName[length - 3] = 'F';
   facTblName[length - 2] = 'A';
   facTblName[length - 1] = 'C';

   ptrToFacTbl = gmsGetFaceTable (facTblName);

   delete [] facTblName;

   return ptrToFacTbl;
}


//-----------------------------------------------
// buildScreenImage
//
// Description:
//-----------------------------------------------
static void buildScreenImage
               (gms_3D_WorldImageType  fromWorldImage,
                gms_2D_ScreenImageType &toScreenImage)

{
         gms_2D_ScreenPointType screenPnt_2D;
         gms_3D_ScreenPointType screenPnt_3D;
         gms_3D_WorldPointType  worldPnt_3D;
         int                    numPoints;
         int                    i, j;
         gmsMapStateStructType  mapState;

   if (fromWorldImage.numLines == 0)
      {
       gmsWriteDebug
          ("*** ERR - buildScreenImage : no lines in image\n");

       toScreenImage.numLines = 0;

       return;
      }

   mapState = gmsGetMapState ();

   for (i = 0; i < fromWorldImage.numLines; i++)
      {
       numPoints = 0;

       for (j = 0; j < fromWorldImage.imageLines[i].numPoints; j++)
          {
           worldPnt_3D = fromWorldImage.imageLines[i].points[j];

           screenPnt_3D = gmsConvertWorldCoordToScreenCoord (worldPnt_3D);

           screenPnt_3D = gmsRotateThreeDimPointAbout_Y_Axis
                              (screenPnt_3D,
                               mapState.angle_Y_Rotation);

           screenPnt_3D = gmsRotateThreeDimPointAbout_X_Axis
                              (screenPnt_3D,
                               mapState.angle_X_Rotation);

           if (screenPnt_3D.z >= 0)
              {
               screenPnt_3D = gmsTranslateThreeDimPoint
                                 (screenPnt_3D,
                                  (mapState.screenWidth / 2),
                                  (mapState.screenHeight / 2),
                                  mapState.zTranslation);

               screenPnt_2D = gmsConvertToProjectedPoint
                                 (screenPnt_3D,
                                  mapState.screenWidth,
                                  mapState.screenHeight);

               toScreenImage.imageLines[i].points[numPoints] = screenPnt_2D;

               numPoints++;
              }
          }

       toScreenImage.imageLines[i].numPoints = numPoints;
      }
}


//-----------------------------------------------
// buildFlatScreenImage
//
// Description:
//-----------------------------------------------
static void buildFlatScreenImage
               (gms_2D_WorldImageType  fromFlatWorldImage,
                gms_2D_ScreenImageType &toScreenImage)

{
         gms_2D_WorldPointType  worldPoint;
         gms_2D_ScreenPointType screenPoint;
         int                    i, j;
         gmsMapStateStructType  mapState;
         gms_2D_ScreenPointType translations;

   if (fromFlatWorldImage.numLines == 0)
      {
       gmsWriteDebug
          ("*** ERR - buildFlatScreenImage : no lines in image\n");

       toScreenImage.numLines = 0;

       return;
      }

   mapState = gmsGetMapState ();

   translations = gmsCalculateFlatMapTranslations ();

   for (i = 0; i < fromFlatWorldImage.numLines; i++)
      {
       toScreenImage.imageLines[i].numPoints =
          fromFlatWorldImage.imageLines[i].numPoints;

       for (j = 0; j < fromFlatWorldImage.imageLines[i].numPoints; j++)
          {
           worldPoint = fromFlatWorldImage.imageLines[i].points[j];

           screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

           screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

           toScreenImage.imageLines[i].points[j] = screenPoint;
          }
      }
}
