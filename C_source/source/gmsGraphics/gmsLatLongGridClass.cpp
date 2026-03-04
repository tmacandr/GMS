//-----------------------------------------------------------------------------
// File : gmsLatLongGridClass.cpp
// Date : 14-Nov-99 : initial definition
//        20-Jan-00 : combine with "gmsFlatLatLongGridClass".  Both models.
//
// Description:
//    This class defines a grid of lat/long reference lines.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <gmsLatLongGridClass.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real grid 'object'
//---------------------------------

typedef struct
   {
    gms_3D_WorldImageType  latGrid_3D;
    gms_3D_WorldImageType  longGrid_3D;
    gms_2D_ScreenImageType latGrid_2D;
    gms_2D_ScreenImageType longGrid_2D;
    double                 deltaDeg;
   } gmsGridStructType;

typedef enum {
                  gmsLatGrid,
                  gmsLongGrid
             } gmsGridType;


//---------------------------------
// Declare some constants
//---------------------------------
static const double                 Lat_Grid_Deg_Start  = 75.0;

static const double                 Lat_Grid_Deg_Stop   = -75.0;

static const double                 Long_Grid_Deg_Start = -180.0;

static const double                 Long_Grid_Deg_Stop  = 165.0;

static const int                    Num_Points_Per_Grid_Line = 256;

static const gms_2D_ScreenImageType Null_2D_Image = {0, NULL};



//---------------------------------
// Declare Static Member Functions
//---------------------------------
static void buildWorldGrid
               (double                gridDeltaDeg,
                double                latDegreesStart,
                gms_3D_WorldImageType &latGridOut,
                double                longDegreesStart,
                gms_3D_WorldImageType &longGridOut);

static void convertWorldPolylineTo_2D_ScreenPolyline
               (gms_3D_WorldPolylineType  worldPolyLineIn,
                gms_2D_ScreenPolylineType &screenPolyline_2D_Out);

static gms_3D_WorldImageType allocate_3D_Grid
                                 (double Grid_Deg_Start,
                                  double Grid_Deg_Stop,
                                  double Grid_Delta_Deg);

static gms_2D_ScreenImageType allocate_2D_Grid
                                 (double Grid_Deg_Start,
                                  double Grid_Deg_Stop,
                                  double Grid_Delta_Deg);

static gms_2D_ScreenPointType translateAndProjectPoint
                                 (gms_3D_ScreenPointType the_3D_Point);

static void convert_3D_ScreenPolylineTo_2D_Polyline
               (gms_3D_ScreenPolylineType thePolylineIn,
                gms_2D_ScreenPolylineType &screenPolyline_2D_Out);

static void allocateFlatMap_2D_GridImages
               (gms_2D_ScreenImageType &latImage,
                gms_2D_ScreenImageType &longImage,
                double                 deltaDeg);

static void buildFlatLatitudeGrid
               (gmsGridStructType *ptrToGrid);

static void buildFlatLongitudeGrid
               (gmsGridStructType *ptrToGrid);

static void switchToFlatEarthGrid
               (gmsGridStructType *ptrToGrid,
                gmsGridType       buildWhichGrid);


//-----------------------------------------------
// gmsLatLongGridClass
//
// Description:
//    Constructor.  Creates a 
//-----------------------------------------------
gmsLatLongGridClass::gmsLatLongGridClass
                        (double            deltaDeg,
                         gmsEarthModelType whichModel)

{
         gmsGridStructType *ptrToNewGrid;

   ptrToNewGrid = new gmsGridStructType;

   ptrToNewGrid->deltaDeg = deltaDeg;

   if (whichModel == gmsFlat)
      {
       allocateFlatMap_2D_GridImages
          (ptrToNewGrid->latGrid_2D,
           ptrToNewGrid->longGrid_2D,
           deltaDeg);

       ptrToNewGrid->latGrid_3D.numLines = 0;

       ptrToNewGrid->latGrid_3D.imageLines = NULL;

       ptrToNewGrid->longGrid_3D.numLines = 0;

       ptrToNewGrid->longGrid_3D.imageLines = NULL;
      }

   else
      {
       ptrToNewGrid->latGrid_3D  = allocate_3D_Grid
                                      (Lat_Grid_Deg_Start,
                                       Lat_Grid_Deg_Stop,
                                       deltaDeg);

       ptrToNewGrid->longGrid_3D = allocate_3D_Grid
                                     (Long_Grid_Deg_Stop,  // kluge
                                      Long_Grid_Deg_Start,
                                      deltaDeg);

       buildWorldGrid
          (deltaDeg,
           Lat_Grid_Deg_Start,
           ptrToNewGrid->latGrid_3D,
           Long_Grid_Deg_Start,
           ptrToNewGrid->longGrid_3D);

       ptrToNewGrid->latGrid_2D  = allocate_2D_Grid
                                     (Lat_Grid_Deg_Start,
                                      Lat_Grid_Deg_Stop,
                                      deltaDeg);

       ptrToNewGrid->longGrid_2D = allocate_2D_Grid
                                     (Long_Grid_Deg_Stop,  // kluge
                                      Long_Grid_Deg_Start,
                                      deltaDeg);
      }

   gmsLatLongGridClass::theGrid = (void *) ptrToNewGrid;

   gmsLatLongGridClass::theModel = whichModel;
}


//-----------------------------------------------
// ~gmsLatLongGridClass
//
// Description:
//    Destructor.  Deletes the 
//-----------------------------------------------
gmsLatLongGridClass::~gmsLatLongGridClass ()

{
         gmsGridStructType *ptrToGrid;

   ptrToGrid = (gmsGridStructType *) gmsLatLongGridClass::theGrid;

   if (ptrToGrid == NULL)
      return;

   //
   // Delete the polylines of LATITUDE grid
   //
   if (ptrToGrid->latGrid_2D.imageLines != NULL)
      {
       if (ptrToGrid->latGrid_2D.imageLines[0].points != NULL)
          delete [] ptrToGrid->latGrid_2D.imageLines[0].points;

       delete [] ptrToGrid->latGrid_2D.imageLines;
      }

   if (ptrToGrid->latGrid_3D.imageLines != NULL)
      {
       if (ptrToGrid->latGrid_3D.imageLines[0].points != NULL)
          delete [] ptrToGrid->latGrid_3D.imageLines[0].points;

       delete [] ptrToGrid->latGrid_3D.imageLines;
      }

   //
   // Delete the polylines of LONGITUDE grid
   //
   if (ptrToGrid->longGrid_2D.imageLines != NULL)
      {
       if (ptrToGrid->longGrid_2D.imageLines[0].points != NULL)
          delete [] ptrToGrid->longGrid_2D.imageLines[0].points;

       delete [] ptrToGrid->longGrid_2D.imageLines;
      }

   if (ptrToGrid->longGrid_3D.imageLines != NULL)
      {
       if (ptrToGrid->longGrid_3D.imageLines[0].points != NULL)
          delete [] ptrToGrid->longGrid_3D.imageLines[0].points;

       delete [] ptrToGrid->longGrid_3D.imageLines;
      }

   //
   // ... and now the grid struct
   //
   delete ptrToGrid;

   gmsLatLongGridClass::theGrid = NULL;
}


//-----------------------------------------------
// gmsGetLatitudeGrid
//
// Description:
//-----------------------------------------------
gms_2D_ScreenImageType gmsLatLongGridClass::gmsGetLatitudeGrid ()

{
         gmsGridStructType *ptrToGrid;
         int               i;

   ptrToGrid = (gmsGridStructType *) gmsLatLongGridClass::theGrid;

   if (ptrToGrid == NULL)
      {
       return Null_2D_Image;
      }

   if (gmsLatLongGridClass::theModel == gmsFlat)
      {
       buildFlatLatitudeGrid (ptrToGrid);
      }

   else
      {
       for (i = 0; i < ptrToGrid->latGrid_3D.numLines; i++)
          {
           convertWorldPolylineTo_2D_ScreenPolyline
                  (ptrToGrid->latGrid_3D.imageLines[i],
                   ptrToGrid->latGrid_2D.imageLines[i]);
          }
      }

   return ptrToGrid->latGrid_2D;
}


//-----------------------------------------------
// gmsGetLongitudeGrid
//
// Description:
//-----------------------------------------------
gms_2D_ScreenImageType gmsLatLongGridClass::gmsGetLongitudeGrid ()

{
         gmsGridStructType     *ptrToGrid;
         int                   i;

   ptrToGrid = (gmsGridStructType *) gmsLatLongGridClass::theGrid;

   if (ptrToGrid == NULL)
      {
       return Null_2D_Image;
      }

   if (gmsLatLongGridClass::theModel == gmsFlat)
      {
       buildFlatLongitudeGrid (ptrToGrid);
      }

   else
      {
       for (i = 0; i < ptrToGrid->longGrid_3D.numLines; i++)
          {
           convertWorldPolylineTo_2D_ScreenPolyline
                  (ptrToGrid->longGrid_3D.imageLines[i],
                   ptrToGrid->longGrid_2D.imageLines[i]);
          }
      }

   return ptrToGrid->longGrid_2D;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// buildWorldGrid
//
// Description:
//    This utility builds two images of 3D world
//    points; one is the latitude grid, the other
//    is the longitude grid.  There is no reference
//    to screen screen coordinates at this point.
//    A series of polylines (of 3D world points)
//    is used to make up each grid (i.e. image).
//-----------------------------------------------
static void buildWorldGrid
               (double                gridDeltaDeg,
                double                latDegreesStart,
                gms_3D_WorldImageType &latGridOut,
                double                longDegreesStart,
                gms_3D_WorldImageType &longGridOut)

{
         double degrees;
         int    i;

   //
   // Build latitude grid of world points
   //
   degrees = Lat_Grid_Deg_Start;

   for (i = 0; i < latGridOut.numLines; i++)
      {
       gmsBuildLatitudeLine
          (degrees,
           gmsEllipsoid,
           &latGridOut.imageLines[i]);

       degrees = degrees - gridDeltaDeg;
      }

   //
   // Build longitude grid of world points
   //
   degrees = Long_Grid_Deg_Start;

   for (i = 0; i < longGridOut.numLines; i++)
      {
       gmsBuildLongitudeLine
          (degrees,
           gmsEllipsoid,
           &longGridOut.imageLines[i]);

       degrees = degrees + gridDeltaDeg;
      }
}


//-----------------------------------------------
// convertWorldPolylineTo_2D_ScreenPolyline
//
// Description:
//-----------------------------------------------
static void convertWorldPolylineTo_2D_ScreenPolyline
               (gms_3D_WorldPolylineType  worldPolyLineIn,
                gms_2D_ScreenPolylineType &screenPolyline_2D_Out)

{
         static gms_3D_ScreenPolylineType temp_3D_ScreenPolyline;
         static bool                      isInitialized = false;
         gmsMapStateStructType            mapState;

   if ( ! isInitialized )
      {
       temp_3D_ScreenPolyline.numPoints = Num_Points_Per_Grid_Line;

       temp_3D_ScreenPolyline.points =
           new gms_3D_ScreenPointType[Num_Points_Per_Grid_Line];

       isInitialized = true;
      }

   gmsConvertWorldPolylineToScreenPolyline
      (&worldPolyLineIn,
       &temp_3D_ScreenPolyline);

   mapState = gmsGetMapState ();

   gmsRotateThreeDimPolyline
      (mapState.angle_X_Rotation,
       mapState.angle_Y_Rotation,
       &temp_3D_ScreenPolyline);

   convert_3D_ScreenPolylineTo_2D_Polyline
      (temp_3D_ScreenPolyline,
       screenPolyline_2D_Out);
}


//-----------------------------------------------
// allocate_3D_Grid
//
// Description:
//-----------------------------------------------
static gms_3D_WorldImageType allocate_3D_Grid
                                 (double Grid_Deg_Start,
                                  double Grid_Deg_Stop,
                                  double Grid_Delta_Deg)

{
         gms_3D_WorldImageType  new_3D_Grid;
         int                    totalNumPoints;
         gms_3D_WorldPointType  *arrayOfPoints;
         int                    i;
         int                    index;

   //
   // Allocate the array of polyline points
   //
   new_3D_Grid.numLines =
      (int) ((Grid_Deg_Start - Grid_Deg_Stop) / Grid_Delta_Deg);

   new_3D_Grid.imageLines =
      new gms_3D_WorldPolylineType[new_3D_Grid.numLines];

   gmsClearMemory
      ( (char *) new_3D_Grid.imageLines,
        (sizeof(gms_3D_WorldPolylineType) * new_3D_Grid.numLines) );

   //
   // Now allocate an array for all the points
   //
   totalNumPoints = new_3D_Grid.numLines * Num_Points_Per_Grid_Line;

   arrayOfPoints = new gms_3D_WorldPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_3D_Grid.numLines; i++)
      {
       new_3D_Grid.imageLines[i].numPoints = Num_Points_Per_Grid_Line;

       new_3D_Grid.imageLines[i].points = &arrayOfPoints[index];

       index = index + Num_Points_Per_Grid_Line;
      }

   return new_3D_Grid;
}


//-----------------------------------------------
// allocate_2D_Grid
//
// Description:
//-----------------------------------------------
static gms_2D_ScreenImageType allocate_2D_Grid
                                 (double Grid_Deg_Start,
                                  double Grid_Deg_Stop,
                                  double Grid_Delta_Deg)

{
         gms_2D_ScreenImageType new_2D_Grid;
         int                    totalNumPoints;
         gms_2D_ScreenPointType *arrayOfPoints;
         int                    i;
         int                    index;

   //
   // Allocate the array of polyline points
   //
   new_2D_Grid.numLines =
      (int) ((Grid_Deg_Start - Grid_Deg_Stop) / Grid_Delta_Deg);

   new_2D_Grid.imageLines =
      new gms_2D_ScreenPolylineType[new_2D_Grid.numLines];

   gmsClearMemory
      ( (char *) new_2D_Grid.imageLines,
        (sizeof(gms_2D_ScreenPolylineType) * new_2D_Grid.numLines) );

   //
   // Now allocate an array for all the points
   //
   totalNumPoints = new_2D_Grid.numLines * Num_Points_Per_Grid_Line;

   arrayOfPoints = new gms_2D_ScreenPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_2D_Grid.numLines; i++)
      {
       new_2D_Grid.imageLines[i].numPoints = Num_Points_Per_Grid_Line;

       new_2D_Grid.imageLines[i].points = &arrayOfPoints[index];

       index = index + Num_Points_Per_Grid_Line;
      }

   return new_2D_Grid;
}


//-----------------------------------------------
// translateAndProjectPoint
//
// Description:
//-----------------------------------------------
static gms_2D_ScreenPointType translateAndProjectPoint
                                 (gms_3D_ScreenPointType the_3D_Point)

{
         gms_3D_ScreenPointType temp_3D_ScreenPoint;
         gmsMapStateStructType  mapState;
         gms_2D_ScreenPointType theAnswer;

   mapState = gmsGetMapState ();

   temp_3D_ScreenPoint = gmsTranslateThreeDimPoint
                            (the_3D_Point,
                             (mapState.screenWidth / 2),
                             (mapState.screenHeight / 2),
                             mapState.zTranslation);

   theAnswer = gmsConvertToProjectedPoint
                  (temp_3D_ScreenPoint,
                   mapState.screenWidth,
                   mapState.screenHeight);

   return theAnswer;
}


//-----------------------------------------------
// convert_3D_ScreenPolylineTo_2D_Polyline
//
// Description:
//-----------------------------------------------
static void convert_3D_ScreenPolylineTo_2D_Polyline
               (gms_3D_ScreenPolylineType thePolylineIn,
                gms_2D_ScreenPolylineType &screenPolyline_2D_Out)

{
         int                    i;
         int                    startPoint;
         int                    endPoint;
         int                    nextStartPoint;
         int                    nextEndPoint;
         int                    index;
         gms_2D_ScreenPointType tempScreenPnt_2D;

   //
   // Find the first point with a positive z value
   //
   startPoint = 0;

   for (i = 0; i < thePolylineIn.numPoints; i++)
      {
       if (thePolylineIn.points[i].z >= 0)
          {
           startPoint = i;

           break;
          }
      }

   //
   // Now find the last contiguous point with a positive z value
   //
   endPoint = thePolylineIn.numPoints;

   for (i = startPoint; i < thePolylineIn.numPoints; i++)
      {
       if (thePolylineIn.points[i].z < 0)
          {
           endPoint = i;

           break;
          }
      }

   //
   // Now find out if there's another slice of positive z values
   //
   nextStartPoint = thePolylineIn.numPoints + 1;

   for (i = endPoint; i < thePolylineIn.numPoints; i++)
      {
       if (thePolylineIn.points[i].z >= 0)
          {
           nextStartPoint = i;

           break;
          }
      }

   //
   // ... and the corresponding end of the second slice
   //
   nextEndPoint = thePolylineIn.numPoints;

   for (i = nextStartPoint; i < thePolylineIn.numPoints; i++)
      {
       if (thePolylineIn.points[i].z < 0)
          {
           nextEndPoint = i;

           break;
          }
      }

   //
   // Now, use only the points in the slice (i.e. of the points
   // with positive z values) to build the 2D polyline.
   //
   index = 0;

   for (i = nextStartPoint; i < nextEndPoint; i++)
      {
       tempScreenPnt_2D = translateAndProjectPoint
                             (thePolylineIn.points[i]);

       screenPolyline_2D_Out.points[index] = tempScreenPnt_2D;

       index++;
      }

   for (i = startPoint; i < endPoint; i++)
      {
       tempScreenPnt_2D = translateAndProjectPoint
                             (thePolylineIn.points[i]);

       screenPolyline_2D_Out.points[index] = tempScreenPnt_2D;

       index++;
      }

   screenPolyline_2D_Out.numPoints = index;
}


//-----------------------------------------------
// allocateFlatMap_2D_GridImages
//
// Description:
//-----------------------------------------------
static void allocateFlatMap_2D_GridImages
               (gms_2D_ScreenImageType &latImage,
                gms_2D_ScreenImageType &longImage,
                double                 deltaDeg)

{
         int                    i, index;
         int                    totalNumPoints;
         gms_2D_ScreenPointType *arrayOfPoints;

   //
   // Allocate mem for LATITUDE image
   //
   latImage.numLines = (int) (180.0 / deltaDeg) + 1;

   latImage.imageLines = new gms_2D_ScreenPolylineType[latImage.numLines];

   totalNumPoints = latImage.numLines * 2; // two points make a line

   arrayOfPoints = new gms_2D_ScreenPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < latImage.numLines; i++)
      {
       latImage.imageLines[i].numPoints = 2;

       latImage.imageLines[i].points = &arrayOfPoints[index];

       index = index + 2;
      }

   //
   // Allocate mem for LONGITUDE image
   //
   longImage.numLines = (int) (360.0 / deltaDeg) + 1;

   longImage.imageLines = new gms_2D_ScreenPolylineType[longImage.numLines];

   totalNumPoints = longImage.numLines * 2; // two points make a line

   arrayOfPoints = new gms_2D_ScreenPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < longImage.numLines; i++)
      {
       longImage.imageLines[i].numPoints = 2;

       longImage.imageLines[i].points = &arrayOfPoints[index];

       index = index + 2;
      }
}


//-----------------------------------------------
// buildFlatLatitudeGrid
//
// Description:
//-----------------------------------------------
static void buildFlatLatitudeGrid
               (gmsGridStructType *ptrToGrid)

{
         gms_2D_WorldPointType  worldPoint;
         gms_2D_ScreenPointType screenPoint;
         gms_2D_ScreenPointType translations;
         twoDimCoordType        coord_1, coord_2;
         int                    i;

   coord_1.Lat = 90.0f;
   coord_1.Long = -180.0f;

   coord_2.Lat = coord_1.Lat;
   coord_2.Long = 180.0f;

   translations = gmsCalculateFlatMapTranslations ();

   for (i = 0; i < ptrToGrid->latGrid_2D.numLines; i++)
      {
       worldPoint = gmsConvertLatLongToFlatPoint (coord_1);

       screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

       screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

       ptrToGrid->latGrid_2D.imageLines[i].points[0] = screenPoint;

       worldPoint = gmsConvertLatLongToFlatPoint (coord_2);

       screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

       screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

       ptrToGrid->latGrid_2D.imageLines[i].points[1] = screenPoint;

       coord_1.Lat = coord_1.Lat - (float) ptrToGrid->deltaDeg;

       coord_2.Lat = coord_1.Lat;
      }
}


//-----------------------------------------------
// buildFlatLongitudeGrid
//
// Description:
//-----------------------------------------------
static void buildFlatLongitudeGrid
               (gmsGridStructType *ptrToGrid)

{
         gms_2D_WorldPointType  worldPoint;
         gms_2D_ScreenPointType translations;
         gms_2D_ScreenPointType screenPoint;
         twoDimCoordType        coord_1, coord_2;
         int                    i;

   coord_1.Lat = 90.0f;
   coord_1.Long = -180.0f;

   coord_2.Lat = -90.0f;
   coord_2.Long = coord_1.Long;

   translations = gmsCalculateFlatMapTranslations ();

   for (i = 0; i < ptrToGrid->longGrid_2D.numLines; i++)
      {
       worldPoint = gmsConvertLatLongToFlatPoint (coord_1);

       screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

       screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

       ptrToGrid->longGrid_2D.imageLines[i].points[0] = screenPoint;

       worldPoint = gmsConvertLatLongToFlatPoint (coord_2);

       screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

       screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

       ptrToGrid->longGrid_2D.imageLines[i].points[1] = screenPoint;

       coord_1.Long = coord_1.Long + (float) ptrToGrid->deltaDeg;

       coord_2.Long = coord_1.Long;
      }
}


//-----------------------------------------------
// switchToFlatEarthGrid
//
// Description:
//    This function is called when the zoom factor
//    on the lat/long grid of an ellipsoid earth
//    model will cause a very close-up image.  At
//    such a zoom factor, it is best to convert
//    to a flat-earth model to prevent image
//    distortion (e.g. z components on the other
//    side of the screen).  This function "activates"
//    the flat-earth grid images.
//-----------------------------------------------
static void switchToFlatEarthGrid
               (gmsGridStructType *ptrToGrid,
                gmsGridType       buildWhichGrid)

{
         int i;

   if ( (ptrToGrid->latGrid_2D.imageLines == NULL) ||
        (ptrToGrid->longGrid_2D.imageLines == NULL) )
      {
       allocateFlatMap_2D_GridImages
          (ptrToGrid->latGrid_2D,
           ptrToGrid->longGrid_2D,
           ptrToGrid->deltaDeg);
      }

   else
      {
       for (i = 0; i < ptrToGrid->latGrid_2D.numLines; i++)
          ptrToGrid->latGrid_2D.imageLines[i].numPoints = 2;

       for (i = 0; i < ptrToGrid->longGrid_2D.numLines; i++)
          ptrToGrid->longGrid_2D.imageLines[i].numPoints = 2;
      }

   if (buildWhichGrid == gmsLatGrid)
      {
       buildFlatLatitudeGrid (ptrToGrid);
      }

   else /* must be gmsLongGrid */
      {
       buildFlatLongitudeGrid (ptrToGrid);
      }
}
