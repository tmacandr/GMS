//-----------------------------------------------------------------------------
// File : gmsMbrClass.cpp
// Date : 10-Dec-99 : initial definition
//        18-Jan-00 : implement ellipsoid earth model
//        12-Feb-00 : "int" in const declaration - flagged by HP C++ compiler
//
// Description:
//    This class defines a "Minimum Bounding Rectangle" object.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <gmsMbrClass.h>
#include <gmsMinBoundRectTable.h>
     //------------//
#include <gmsWorldCoordUtils.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real flat map 'object'
//---------------------------------

typedef struct
   {
    minBoundRectTableType  *mbrTbl;
    gms_2D_ScreenImageType image;
   } gmsMbrRecType;


//---------------------------------
// Declare some constants
//---------------------------------
static const int g_numPoints = 5;  // num points to "close" the rectangle


//---------------------------------
// Declare Static Member Functions
//---------------------------------

static gms_2D_ScreenImageType allocate_2D_MbrImage
                                  (minBoundRectTableType *mbrTbl);

static void build_2D_MbrImage
               (gmsMbrRecType *mbrObj);

static void buildFlat_2D_MbrImage
               (gmsMbrRecType *mbrObj);




//-----------------------------------------------
// gmsMbrClass
//
// Description:
//    Constructor.  Creates an object that
//    represents a minimum bounding rectangle
//    object.  The MBR to be constructed is
//    specified by the "mbrTblName".  The width
//    and height parameters are used to set the
//    center point of the image.  The parameter
//    "whichModel" indicates flat, sphere, or
//    ellipsoid model.
//-----------------------------------------------
gmsMbrClass::gmsMbrClass
             (gmsEarthModelType whichModel,
              const char        *mbrTblName)

{
         gmsMbrRecType *ptrToNewMbr;

   ptrToNewMbr = new gmsMbrRecType;

   ptrToNewMbr->mbrTbl = gmsGetMbrTable (mbrTblName);

   if (ptrToNewMbr->mbrTbl == NULL)
      {
       gmsWriteDebug("ERROR : gmsMbrClass : mbr tbl is NULL\n");

       return;
      }

   ptrToNewMbr->image = allocate_2D_MbrImage (ptrToNewMbr->mbrTbl);

   //gmsSetWindowDimensions
   //   (screenWidth,
   //    screenHeight);

   gmsMbrClass::theMbr = (void *) ptrToNewMbr;

   gmsMbrClass::earthModel = whichModel;
}


//-----------------------------------------------
// ~gmsMbrClass
//
// Description:
//    Destructor.  Deletes the object.
//-----------------------------------------------
gmsMbrClass::~gmsMbrClass ()

{
         gmsMbrRecType *ptrToMbr;

   ptrToMbr = (gmsMbrRecType *) gmsMbrClass::theMbr;

   if (ptrToMbr == NULL)
      {
       gmsWriteDebug("ERROR : ~gmsMbrClass : object is NULL\n");

       return;
      }

   if (ptrToMbr->mbrTbl != NULL)
      gmsFreeMbrTable (ptrToMbr->mbrTbl);

   if (ptrToMbr->image.imageLines != NULL)
      {
       if (ptrToMbr->image.imageLines[0].points != NULL)
          delete [] ptrToMbr->image.imageLines[0].points;

       delete [] ptrToMbr->image.imageLines;
      }

   delete ptrToMbr;

   gmsMbrClass::theMbr = (void *) NULL;
}


//-----------------------------------------------
// gmsGetMbrImage
//
// Description:
//    Returns the 2-D image of the MBR in screen
//    coordinates.
//-----------------------------------------------
gms_2D_ScreenImageType gmsMbrClass::gmsGetMbrImage ()

{
         gmsMbrRecType *ptrToMbr;

   ptrToMbr = (gmsMbrRecType *) gmsMbrClass::theMbr;

   if (ptrToMbr == NULL)
      {
             gms_2D_ScreenImageType bogus = {0, NULL};

       gmsWriteDebug("ERROR : gmsGetMbrImage : object is NULL\n");

       return bogus;
      }

   if (gmsMbrClass::earthModel == gmsFlat)
      {
       buildFlat_2D_MbrImage (ptrToMbr);
      }

   else
      {
       build_2D_MbrImage (ptrToMbr);
      }

   return ptrToMbr->image;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// allocate_2D_MbrImage
//
// Description:
//-----------------------------------------------
static gms_2D_ScreenImageType allocate_2D_MbrImage
                                  (minBoundRectTableType *mbrTbl)

{
         gms_2D_ScreenImageType new_2D_MapImage = { 0, NULL };
         int                    totalNumPoints = 0;
         int                    i, index;
         gms_2D_ScreenPointType *arrayOfPoints;

   if (mbrTbl == NULL)
      {
       gmsWriteDebug ("*** ERROR : allocate_2D_MbrImage : MBR tbl NULL\n");

       return new_2D_MapImage;
      }

   //
   // Allocate a table of 'polyline' records
   //
   new_2D_MapImage.numLines = mbrTbl->numRecords;

   new_2D_MapImage.imageLines =
      new gms_2D_ScreenPolylineType[mbrTbl->numRecords];

   //
   // Now allocate the points of each rectangle
   //
   totalNumPoints = mbrTbl->numRecords * g_numPoints;

   arrayOfPoints = new gms_2D_ScreenPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_2D_MapImage.numLines; i++)
      {
       new_2D_MapImage.imageLines[i].numPoints = g_numPoints;

       new_2D_MapImage.imageLines[i].points = &arrayOfPoints[index];

       index = index + g_numPoints;
      }

   return new_2D_MapImage;
}


//-----------------------------------------------
// build_2D_MbrImage
//
// Description:
//-----------------------------------------------
static void build_2D_MbrImage
               (gmsMbrRecType *mbrObj)

{
         gms_3D_WorldPointType  worldPoint;
         gms_3D_ScreenPointType screenPoint_3D;
         gms_2D_ScreenPointType screenPoint_2D;
         int                    i, j;
         twoDimCoordType        tempCoord[g_numPoints];
         gmsMapStateStructType  mapState;

   mapState = gmsGetMapState ();

   for (i = 0; i < mbrObj->mbrTbl->numRecords; i++)
      {
       tempCoord[0].Long = mbrObj->mbrTbl->mbrRecords[i].xMax;
       tempCoord[0].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMax;

       tempCoord[1].Long = mbrObj->mbrTbl->mbrRecords[i].xMin;
       tempCoord[1].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMax;

       tempCoord[2].Long = mbrObj->mbrTbl->mbrRecords[i].xMin;
       tempCoord[2].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMin;

       tempCoord[3].Long = mbrObj->mbrTbl->mbrRecords[i].xMax;
       tempCoord[3].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMin;

       tempCoord[4].Long = mbrObj->mbrTbl->mbrRecords[i].xMax;
       tempCoord[4].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMax;

       mbrObj->image.imageLines[i].numPoints = g_numPoints;

       for (j = 0; j < g_numPoints; j++)
          {
           worldPoint = gmsConvertLatLongToEllipsoidPoint (tempCoord[j]);

           screenPoint_3D = gmsConvertWorldCoordToScreenCoord (worldPoint);

           screenPoint_3D = gmsRotateThreeDimPointAbout_Y_Axis
                               (screenPoint_3D,
                                mapState.angle_Y_Rotation);

           screenPoint_3D = gmsRotateThreeDimPointAbout_X_Axis
                               (screenPoint_3D,
                                mapState.angle_X_Rotation);

           if (screenPoint_3D.z >= 0)
              {
               mbrObj->image.imageLines[i].numPoints = 0;

               break; // out of inner 'for loop'?
              }

           screenPoint_3D = gmsTranslateThreeDimPoint
                                   (screenPoint_3D,
                                    (mapState.screenWidth / 2),
                                    (mapState.screenHeight / 2),
                                    mapState.zTranslation);

           screenPoint_2D = gmsConvertToProjectedPoint
                                 (screenPoint_3D,
                                  mapState.screenWidth,
                                  mapState.screenHeight);

           mbrObj->image.imageLines[i].points[j] = screenPoint_2D;
          }
      }
}


//-----------------------------------------------
// buildFlat_2D_MbrImage
//
// Description:
//-----------------------------------------------
static void buildFlat_2D_MbrImage
               (gmsMbrRecType *mbrObj)

{
         gms_2D_WorldPointType  worldPoint;
         gms_2D_ScreenPointType screenPoint;
         gms_2D_ScreenPointType translations;
         int                    i, j;
         twoDimCoordType        tempCoord[g_numPoints];

   translations = gmsCalculateFlatMapTranslations ();

   for (i = 0; i < mbrObj->mbrTbl->numRecords; i++)
      {
       tempCoord[0].Long = mbrObj->mbrTbl->mbrRecords[i].xMax;
       tempCoord[0].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMax;

       tempCoord[1].Long = mbrObj->mbrTbl->mbrRecords[i].xMin;
       tempCoord[1].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMax;

       tempCoord[2].Long = mbrObj->mbrTbl->mbrRecords[i].xMin;
       tempCoord[2].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMin;

       tempCoord[3].Long = mbrObj->mbrTbl->mbrRecords[i].xMax;
       tempCoord[3].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMin;

       tempCoord[4].Long = mbrObj->mbrTbl->mbrRecords[i].xMax;
       tempCoord[4].Lat  = mbrObj->mbrTbl->mbrRecords[i].yMax;

       for (j = 0; j < g_numPoints; j++)
          {
           worldPoint = gmsConvertLatLongToFlatPoint (tempCoord[j]);

           screenPoint = gmsConvertFlatWorldCoordToScreenCoord
                                (worldPoint);

           screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

           mbrObj->image.imageLines[i].points[j] = screenPoint;
          }
      }
}
