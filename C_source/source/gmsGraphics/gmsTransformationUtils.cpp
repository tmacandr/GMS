/*---------------------------------------------------------------------------*/
/* File: gmsTransformationUtils.h
/* Date: 25-Sep-99 : initial definition
/*       13-Oct-99 : operate only in screen coordinates
/*       19-Oct-99 : implement rotation utilities
/*       11-Nov-99 : polyline types changed to 'dynamic' form
/*       13-Nov-99 : add utility to rotate points of 3D polyline
/*       09-Dec-99 : rename file
/*       28-Jan-00 : get "operator distance" from "gmsMapStateMgr"
/*
/* Description:
/*    Utilities to support 3-D graphics operations.  These utilities operate
/*    only on screen coordinates.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <gmsTransformationUtils.h>
#include <gmsMapStateMgr.h>


/*----------------------*/
/* Constants/Globals    */
/*----------------------*/


/*-----------------------------------------------------*/
/* gmsConvertToProjectedPoint
/*
/* Description:
/*    This utility converts the specified 3-D point
/*    (x, y, z) into a 2-D point projected onto the
/*    screen (x*, y*).
/*-----------------------------------------------------*/
gms_2D_ScreenPointType gmsConvertToProjectedPoint
                          (gms_3D_ScreenPointType theCoord,
                           int                    screenWidth,
                           int                    screenHeight)

{
      const int              a = (screenWidth / 2);
      const int              b = (screenHeight / 2);
      int                    denominator;
      gms_2D_ScreenPointType theAnswer   = {0, 0};
      int                    temp;
      gmsMapStateStructType  mapState;

   mapState = gmsGetMapState ();

   denominator = mapState.operatorDistance + theCoord.z;

   if ( denominator == 0 ) // prevent divide-by-zero
      {
       return theAnswer;
      }

   //
   // Project the 3-D point on to the 2-D screen
   //
   temp = a * theCoord.z + mapState.operatorDistance * theCoord.x;

   theAnswer.x = (int) (temp / denominator );

   temp = b * theCoord.z + mapState.operatorDistance * theCoord.y;

   theAnswer.y = (int) (temp / denominator );

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvertToProjectedPolyline
/*
/* Description:
/*    This utility converts the specified polyline of
/*    3-D points (x, y, z) into an array of 2-D points
/*    projected onto the screen (x*, y*).
/*-----------------------------------------------------*/
void gmsConvertToProjectedPolyline
           (gms_3D_ScreenPolylineType *ptrTo_3D_PolylineIn,
            int                       screenWidth,
            int                       screenHeight,
            gms_2D_ScreenPolylineType *ptrTo_2D_PolylineOut)

{
         int i;

   if ( ptrTo_3D_PolylineIn == NULL)
      return;

   if (ptrTo_2D_PolylineOut == NULL)
      return;

   for (i = 0; i < ptrTo_3D_PolylineIn->numPoints; i++)
      {
       ptrTo_2D_PolylineOut->points[i] =
             gmsConvertToProjectedPoint
                          (ptrTo_3D_PolylineIn->points[i],
                           screenWidth,
                           screenHeight);
      }
}


/*-----------------------------------------------------*/
/* gmsTranslateTwoDimPoint
/*
/* Description:
/*    This utility applies 2-dimensional translation to
/*    the specified point.
/*-----------------------------------------------------*/
gms_2D_ScreenPointType gmsTranslateTwoDimPoint
                             (gms_2D_ScreenPointType thePoint,
                              int                    x,
                              int                    y)

{
         gms_2D_ScreenPointType theAnswer;

   theAnswer.x = thePoint.x + x;

   theAnswer.y = thePoint.y + y;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsTranslateThreeDimPoint
/*
/* Description:
/*    This utility applies 3-dimensional translation to
/*    the specified point.
/*-----------------------------------------------------*/
gms_3D_ScreenPointType gmsTranslateThreeDimPoint
                           (gms_3D_ScreenPointType thePoint,
                            int                    x,
                            int                    y,
                            int                    z)

{
         gms_3D_ScreenPointType theAnswer;

   theAnswer.x = thePoint.x + x;

   theAnswer.y = thePoint.y + y;

   theAnswer.z = thePoint.z + z;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsTranslate_3D_Polyline
/*
/* Description:
/*    This utility applies 3-dimensional translation to
/*    each of the points of the specified polyline (of
/*    screen coordinates).
/*-----------------------------------------------------*/
void gmsTranslate_3D_Polyline
           (gms_3D_ScreenPolylineType *ptrToPolyline,
            int                       x,
            int                       y,
            int                       z)

{
         int i;

   if ( (int *) ptrToPolyline == NULL)
      return;

   for (i = 0; i < ptrToPolyline->numPoints; i++)
      {
       ptrToPolyline->points[i] =
             gmsTranslateThreeDimPoint
                (ptrToPolyline->points[i],
                 x,
                 y,
                 z);
      }
}


/*-----------------------------------------------------*/
/* gmsRotateThreeDimPointAbout_X_Axis
/*
/* Description:
/*    This utility applies a 3-dimensional rotation of
/*    the specified point around the x-axis.  The caller
/*    specifies the degrees of rotation.  The trans-
/*    formation matrix is:
/*
/*                | 1      0      0      0 |
/*                |                        |
/*                | 0   cos(x)  -sin(x)  0 |
/*           Tx = |                        |
/*                | 0   sin(x)  cos(x)   0 |
/*                |                        |
/*                | 0      0      0      1 |
/*
/*    where x is the angle-of-rotation.
/*-----------------------------------------------------*/
gms_3D_ScreenPointType gmsRotateThreeDimPointAbout_X_Axis
                           (gms_3D_ScreenPointType thePoint,
                            double                 angleOfRotation)

{
         const double           theSin = sin(angleOfRotation * degToRad);
         const double           theCos = cos(angleOfRotation * degToRad);
         gms_3D_ScreenPointType theAnswer = {0, 0, 0};

   theAnswer.x = thePoint.x;

   theAnswer.y = (int) ( (thePoint.y * theCos) + (thePoint.z * theSin) );

   theAnswer.z = (int) ( (-thePoint.y * theSin) + (thePoint.z * theCos));

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsRotateThreeDimPointAbout_Y_Axis
/*
/* Description:
/*    This utility applies a 3-dimensional rotation of
/*    the specified point around the y-axis.  The caller
/*    specifies the degrees of rotation.  The trans-
/*    formation matrix is:
/*
/*                | cos(x) 0    -sin(x)  0 |
/*                |                        |
/*                | 0      1      0      0 |
/*           Ty = |                        |
/*                | sin(x) 0    cos(x)   0 |
/*                |                        |
/*                | 0      0      0      1 |
/*
/*    where x is the angle-of-rotation about the y-axis.
/*-----------------------------------------------------*/
gms_3D_ScreenPointType gmsRotateThreeDimPointAbout_Y_Axis
                           (gms_3D_ScreenPointType thePoint,
                            double                 angleOfRotation)

{
         const double           theSin = sin(angleOfRotation * degToRad);
         const double           theCos = cos(angleOfRotation * degToRad);
         gms_3D_ScreenPointType theAnswer = {0, 0, 0};

   theAnswer.x = (int) ( (thePoint.x * theCos) + (thePoint.z * theSin) );

   theAnswer.y = thePoint.y;

   theAnswer.z = (int) ( (-thePoint.x * theSin) + (thePoint.z * theCos));

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsRotateThreeDimPointAbout_Z_Axis
/*
/* Description:
/*    This utility applies a 3-dimensional rotation of
/*    the specified point around the z-axis.  The caller
/*    specifies the degrees of rotation.  The trans-
/*    formation matrix is:
/*
/*                | cos(x) -sin(x) 0    0 |
/*                |                       |
/*                | sin(x) cos(x)  0    0 |
/*           Tx = |                       |
/*                |   0      0     1    0 |
/*                |                       |
/*                |   0      0     1    1 |
/*
/*    where x is the angle-of-rotation about the z-axis.
/*-----------------------------------------------------*/
gms_3D_ScreenPointType gmsRotateThreeDimPointAbout_Z_Axis
                           (gms_3D_ScreenPointType thePoint,
                            double                 angleOfRotation)

{
         const double           theSin = sin(angleOfRotation * degToRad);
         const double           theCos = cos(angleOfRotation * degToRad);
         gms_3D_ScreenPointType theAnswer = {0, 0, 0};

   theAnswer.x = (int) ( (thePoint.x * theCos) + (thePoint.y * theSin) );

   theAnswer.y = (int) ( (-thePoint.x * theSin) + (thePoint.y * theCos));

   theAnswer.z = thePoint.z;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsRotateThreeDimPolyline
/*
/* Description:
/*    This utility applies a 3-dimensional rotation of
/*    the specified point around all axes.  The caller
/*    specifies the degrees of rotation for each axis.
/*
/*    Note:
/*       The rotations are applied to the polyline passed
/*       in by the client.  The original values of the
/*       incoming polyline are overwritten with the
/*       rotated values.
/*-----------------------------------------------------*/
void gmsRotateThreeDimPolyline
           (double                    xRotation,
            double                    yRotation,
            gms_3D_ScreenPolylineType *ptrTo_3D_PolylineInOut)

{
         int                    i;
         gms_3D_ScreenPointType temp_3D_Point;

   for (i = 0; i < ptrTo_3D_PolylineInOut->numPoints; i++)
      {
       temp_3D_Point = gmsRotateThreeDimPointAbout_Y_Axis
                          (ptrTo_3D_PolylineInOut->points[i],
                           yRotation);

       temp_3D_Point = gmsRotateThreeDimPointAbout_X_Axis
                          (temp_3D_Point,
                           xRotation);

       ptrTo_3D_PolylineInOut->points[i] = temp_3D_Point;
      }
}
