//-----------------------------------------------------------------------------
// File : gms_DN_PolygonMapClass.cpp
// Date : 04-Feb-00 : initial definition
//
// Description:
//    This class defines a "map of polygons" from the DRAINAGE (DN) theme.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <gms_DN_PolygonMapClass.h>
#include <gmsEdgeTable.h>
#include <gmsRingTable.h>
#include <gmsFaceTable.h>
#include <gmsPolygonUtilities.h>
#include <gmsDcwUtilities.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real map 'object'
//---------------------------------

typedef struct
   {
    gms_3D_WorldImageType  ellipsoidWaterAreas;
    gms_2D_WorldImageType  flatWaterAreas;
    gms_2D_ScreenImageType screenWaterAreas;
   } gms_DN_PolygonMapStructType;


//---------------------------------
// Declare some constants
//---------------------------------

static const gms_3D_WorldImageType  Null_3D_World_Image = {0, NULL};

static const gms_2D_WorldImageType  Null_2D_World_Image = {0, NULL};

static const gms_2D_ScreenImageType Null_2D_Screen_Image = {0, NULL};


//---------------------------------
// Declare Static Member Functions
//---------------------------------


//-----------------------------------------------
// gms_DN_PolygonMapClass
//
// Description:
//    Constructor.  Creates an object that
//    represents polygons from the edge table
//    from the DN theme.
//-----------------------------------------------
gms_DN_PolygonMapClass::gms_DN_PolygonMapClass
                (gmsEarthModelType whichModel,
                 const char        *edgeTblName)

{
         gms_DN_PolygonMapStructType *ptrToNewMap;
         gmsPolygonInfoType          polygonInfo;
         ringTableType               *tempRngTbl;
         edgeTableType               *tempEdgTbl;

   if (edgeTblName == NULL)
      {
       gmsWriteDebug
          ("*** ERROR : 'gms_DN_PolygonMapClass' NULL client data\n");

       gms_DN_PolygonMapClass::dnPolygonMap = (void *) NULL;

       return;
      }

   ptrToNewMap = new gms_DN_PolygonMapStructType;

   tempEdgTbl = gmsGetEdgeTable (edgeTblName);

   tempRngTbl = gmsGetPtrToRingTable (edgeTblName);

   if ( (tempEdgTbl == NULL) || (tempRngTbl == NULL) )
      {
       gmsWriteDebug
          ("*** ERROR : 'gms_DN_PolygonMapClass' NULL EDG and/or RNG tbl\n");

       gms_DN_PolygonMapClass::dnPolygonMap = NULL;

       return;
      }

   polygonInfo = gmsGet_DN_PolygonInfoFromEdgeTable
                         (tempEdgTbl,
                          tempRngTbl);

   if (whichModel == gmsFlat)
      {
       ptrToNewMap->ellipsoidWaterAreas = Null_3D_World_Image;

       ptrToNewMap->flatWaterAreas =
          gmsBuildFlatWorldPolygonsFromEdgeTable
             (polygonInfo,
              tempEdgTbl);
      }

   else
      {
       ptrToNewMap->ellipsoidWaterAreas =
          gmsBuildWorldPolygonsFromEdgeTable
             (polygonInfo,
              tempEdgTbl);

       ptrToNewMap->flatWaterAreas = Null_2D_World_Image;
      }

   ptrToNewMap->screenWaterAreas =
       gmsAllocateScreenPolygonsFromEdgeTable
          (polygonInfo,
           tempEdgTbl);

   gms_DN_PolygonMapClass::theModel = whichModel;

   gms_DN_PolygonMapClass::dnPolygonMap = (void *) ptrToNewMap;

   gmsFreePolygonInfo (&polygonInfo);

   gmsFreeEdgeTable (tempEdgTbl);

   gmsFreeRingTable (tempRngTbl);
}


//-----------------------------------------------
// ~gms_DN_PolygonMapClass
//
// Description:
//    Destructor.  Deletes the map object.
//-----------------------------------------------
gms_DN_PolygonMapClass::~gms_DN_PolygonMapClass ()

{
         gms_DN_PolygonMapStructType *ptrToMap;

   if (gms_DN_PolygonMapClass::dnPolygonMap == NULL)
      return;

   ptrToMap =
      (gms_DN_PolygonMapStructType *) gms_DN_PolygonMapClass::dnPolygonMap;

   gmsFree_3D_WorldImage (&ptrToMap->ellipsoidWaterAreas);

   gmsFree_2D_WorldImage (&ptrToMap->flatWaterAreas);

   gmsFree_2D_ScreenImage (&ptrToMap->screenWaterAreas);

   delete ptrToMap;

   gms_DN_PolygonMapClass::dnPolygonMap = NULL;
}


//-----------------------------------------------
// gmsGetInlandWaterAreas
//
// Description:
//    Retrieves a 2D screen image of polygons of
//    inland water areas (e.g. lakes, ponds).  The
//    lines in the returned data structure represent
//    one or more polygons.  Each polygon
//    represents an inland water area
//-----------------------------------------------
gms_2D_ScreenImageType gms_DN_PolygonMapClass::gmsGetInlandWaterAreas()

{
         gms_DN_PolygonMapStructType *ptrToMap;

   if (gms_DN_PolygonMapClass::dnPolygonMap == NULL)
      return Null_2D_Screen_Image;

   ptrToMap =
      (gms_DN_PolygonMapStructType *) gms_DN_PolygonMapClass::dnPolygonMap;

   if (gms_DN_PolygonMapClass::theModel == gmsFlat)
      {
       gmsBuildFlatScreenImage
          (ptrToMap->flatWaterAreas,
           ptrToMap->screenWaterAreas);
      }

   else
      {
       gmsBuildScreenImage
          (ptrToMap->ellipsoidWaterAreas,
           ptrToMap->screenWaterAreas);
      }

   return ptrToMap->screenWaterAreas;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


