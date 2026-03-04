/*---------------------------------------------------------------------------*/
/* File: gmsScreenCoordUtils.cpp
/* Date: 21-Sep-99 : initial definition
/*       14-Oct-99 : operate only in screen coordinates
/*       11-Nov-99 : polyline(s) changed to 'dynamic' length
/*       22-Nov-99 : distinguish flat-earth zoom vs globe zoom
/*       09-Dec-99 : rename file
/*       27-Jan-00 : remove zoom-factor as state, use 'gmsMapStateMgr'
/*
/* Description:
/*    Utilities to convert from real-world coordinates to screen coordinates.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/

#include <math.h>
#include <float.h>
#include <gmsScreenCoordUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsMapStateMgr.h>
/*--------------*/
#include <gmsDebugUtil.h>
#include <stdio.h>

/*----------------------*/
/* Constants/Globals    */
/*----------------------*/

static char msg[96];



/*-----------------------------------------------------*/
/* gmsConvertWorldCoordToScreenCoord
/*
/* Description:
/*    This utility converts a given (x, y, z) real-world
/*    coordinate (in meters) to an (x, y, z) screen
/*    coordinate (in pixels).
/*-----------------------------------------------------*/
gms_3D_ScreenPointType gmsConvertWorldCoordToScreenCoord
                             (gms_3D_WorldPointType theCoord)

{
         gms_3D_ScreenPointType theAnswer;
         gmsMapStateStructType  mapState;
         double                 tempZoomFactor;

   mapState = gmsGetMapState ();

   if (mapState.zoomFactor <= mapState.minEellipsoidZoom)
      tempZoomFactor = mapState.minEellipsoidZoom;

   else
      tempZoomFactor = mapState.zoomFactor;

   theAnswer.x = (int) (theCoord.x_meters / tempZoomFactor);

   /*-------------------------------*/
   /* y-coordinate on screen is
   /* upside-down.
   /*-------------------------------*/
   theAnswer.y = (int) (-theCoord.y_meters / tempZoomFactor);

   theAnswer.z = (int) (theCoord.z_meters / tempZoomFactor);

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvertWorldPolylineToScreenPolyline
/*
/* Description:
/*    This utility converts a given array of (x, y, z)
/*    real-world coordinates (in meters) to an array of
/*    (x, y, z) screen coordinates (in pixels).
/*-----------------------------------------------------*/
void gmsConvertWorldPolylineToScreenPolyline
              (gms_3D_WorldPolylineType  *worldPolylineIn,
               gms_3D_ScreenPolylineType *screenPolylineOut)

{
         int i;

   if (worldPolylineIn == NULL)
      return;

   if (screenPolylineOut == NULL)
      return;

   if (worldPolylineIn->numPoints > screenPolylineOut->numPoints)
      return;

   screenPolylineOut->numPoints = worldPolylineIn->numPoints;

   for (i = 0; i < worldPolylineIn->numPoints; i++)
      {
       screenPolylineOut->points[i] = gmsConvertWorldCoordToScreenCoord
                                         (worldPolylineIn->points[i]);
      }
}


/*-----------------------------------------------------*/
/* gmsConvertFlatWorldCoordToScreenCoord
/*
/* Description:
/*    This utility converts a given (x, y) real-world
/*    coordinate (in meters) to an (x, y) screen
/*    coordinate (in pixels).
/*-----------------------------------------------------*/
gms_2D_ScreenPointType gmsConvertFlatWorldCoordToScreenCoord
                             (gms_2D_WorldPointType theCoord)

{
         gms_2D_ScreenPointType theAnswer;
         gmsMapStateStructType  mapState;

   mapState = gmsGetMapState ();

   theAnswer.x = (int) (theCoord.x_meters / mapState.zoomFactor);

   /*-------------------------------*/
   /* y-coordinate on screen is
   /* upside-down.
   /*-------------------------------*/
   theAnswer.y = (int) (-theCoord.y_meters / mapState.zoomFactor);

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvert_3D_WorldPointTo_2D_ScreenPoint
/*
/* Description:
/*    This utility converts a given 3D World point to a
/*    2D screen coordinate.  This utility is used in
/*    support of the "ellipsoid earth" model.  The flag
/*    'isValid' indicates if the conversion from the
/*    3D world-point to an (intermediate) 3D screen point
/*    resulted in a negative z value.  Such points are
/*    filtered out.
/*-----------------------------------------------------*/
gms_2D_ScreenPointType gmsConvert_3D_WorldPointTo_2D_ScreenPoint
                             (gms_3D_WorldPointType  worldPoint_3D,
                              bool                   &isValid)

{
         gms_3D_ScreenPointType screenPnt_3D;
         gms_2D_ScreenPointType screenPnt_2D;
         gmsMapStateStructType  mapState;

   mapState = gmsGetMapState ();

   screenPnt_3D = gmsConvertWorldCoordToScreenCoord (worldPoint_3D);

   screenPnt_3D = gmsRotateThreeDimPointAbout_Y_Axis
                                     (screenPnt_3D,
                                      mapState.angle_Y_Rotation);

   screenPnt_3D = gmsRotateThreeDimPointAbout_X_Axis
                                     (screenPnt_3D,
                                      mapState.angle_X_Rotation);

   if (screenPnt_3D.z < 0)
      {
       screenPnt_2D.x = -1;
       screenPnt_2D.y = -1;

       isValid = false;

       return screenPnt_2D;
      }

   screenPnt_3D = gmsTranslateThreeDimPoint
                     (screenPnt_3D,
                      (mapState.screenWidth / 2),
                      (mapState.screenHeight / 2),
                      mapState.zTranslation);

   screenPnt_2D = gmsConvertToProjectedPoint
                     (screenPnt_3D,
                      mapState.screenWidth,
                      mapState.screenHeight);

   isValid = true;

   return screenPnt_2D;
}


/*-----------------------------------------------------*/
/* gmsConvert_2D_WorldPointTo_2D_ScreenPoint
/*
/* Description:
/*    This utility converts a given 2D World point to a
/*    2D screen coordinate.  This utility is used in
/*    support of the "flat earth" model.
/*-----------------------------------------------------*/
gms_2D_ScreenPointType gmsConvert_2D_WorldPointTo_2D_ScreenPoint
                             (gms_2D_WorldPointType worldPoint_2D)

{
         gms_2D_ScreenPointType screenPoint_2D;
         gms_2D_ScreenPointType translations;
         gmsMapStateStructType  mapState;

   mapState = gmsGetMapState ();

   screenPoint_2D = gmsConvertFlatWorldCoordToScreenCoord (worldPoint_2D);

   translations = gmsCalculateFlatMapTranslations ();

   screenPoint_2D = gmsTranslateTwoDimPoint
                       (screenPoint_2D,
                        translations.x,
                        translations.y);

   return screenPoint_2D;
}


/*-----------------------------------------------------*/
/* gmsCalculateFlatMapTranslations
/*
/* Description:
/*-----------------------------------------------------*/
gms_2D_ScreenPointType gmsCalculateFlatMapTranslations ()

{
         gms_2D_WorldPointType  worldPoint_2D;
         gms_2D_ScreenPointType screenPoint_2D;
         gmsMapStateStructType  mapState;

   mapState = gmsGetMapState ();

   worldPoint_2D = gmsConvertLatLongToFlatPoint (mapState.flatMapCenter);

   screenPoint_2D = gmsConvertFlatWorldCoordToScreenCoord (worldPoint_2D);

   screenPoint_2D.x = screenPoint_2D.x + (mapState.screenWidth / 2);

   screenPoint_2D.y = screenPoint_2D.y + (mapState.screenHeight / 2);

   return screenPoint_2D;
}


/*-----------------------------------------------------*/
/* gmsIsVisibleLine
/*
/* Description:
/*    This utility determines if a line, defined by the
/*    two screen points, would be visible on "most" CRTs.
/*    Often, when world points are converted to screen
/*    coordinates, the values of the screen coordinates
/*    are very large.  For example, a screen coordinate
/*    of (16,0000, 16,0000) would not be visible on any
/*    video hardware system ... and would most likely
/*    cause a constraint error.
/*
/*    This utility checks for bounds of 16K X 16K.  This
/*    will be applicable to several video hardware systems.
/*    However, even these range constraints may be too
/*    large for some video hardware systems.
/*-----------------------------------------------------*/
bool gmsIsVisibleLine
               (gms_2D_ScreenPointType point_1,
                gms_2D_ScreenPointType point_2)

{
         bool             isOk;
         static const int Max_X = 1024 * 24;
         static const int Min_X = -Max_X;
         static const int Max_Y = Max_X;
         static const int Min_Y = -Min_Y;

   isOk = ( (point_1.x > Min_X) &&
            (point_1.x < Max_X) &&
            (point_2.x > Min_X) &&
            (point_2.x < Max_X) &&
                //----------//
            (point_1.y > Min_Y) &&
            (point_1.y < Max_Y) &&
            (point_2.y > Min_Y) &&
            (point_2.y < Max_Y) );

   return isOk;
}
