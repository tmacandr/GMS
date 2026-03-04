//---------------------------------------------------------------------------
// File: gmsTransformationUtils.h
// Date: 25-Sep-99 : initial definition
//       13-Oct-99 : operate only in screen coordinates
//       13-Nov-99 : add utility to rotate points of 3D polyline
//       09-Dec-99 : rename file
//
// Description:
//    Utilities to support 3-D graphics operations.  These utilities operate
//    only on screen coordinates.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//---------------------------------------------------------------------------
#ifndef GMS_TXF_UTILS

   #define GMS_TXF_UTILS

   #include <gmsTypesAndConstants.h>

   //-----------------------------------------------------*/
   // gmsConvertToProjectedPoint
   //
   // Description:
   //    This utility converts the specified 3-D point
   //    (x, y, z) into a 2-D point projected onto the
   //    screen (x*, y*).
   //-----------------------------------------------------*/
   gms_2D_ScreenPointType gmsConvertToProjectedPoint
                             (gms_3D_ScreenPointType theCoord,
                              int                    screenWidth,
                              int                    screenHeight);


   //-----------------------------------------------------*/
   // gmsConvertToProjectedPolyline
   //
   // Description:
   //    This utility converts the specified polyline of
   //    3-D points (x, y, z) into an array of 2-D points
   //    projected onto the screen (x*, y*).
   //-----------------------------------------------------*/
   void gmsConvertToProjectedPolyline
           (gms_3D_ScreenPolylineType *ptrTo_3D_PolylineIn,
            int                       screenWidth,
            int                       screenHeight,
            gms_2D_ScreenPolylineType *ptrTo_2D_PolylineOut);


   //-----------------------------------------------------*/
   // gmsTranslateTwoDimPoint
   //
   // Description:
   //    This utility applies 2-dimensional translation to
   //    the specified point.
   //-----------------------------------------------------*/
   gms_2D_ScreenPointType gmsTranslateTwoDimPoint
                             (gms_2D_ScreenPointType thePoint,
                              int                    x,
                              int                    y);


   //-----------------------------------------------------*/
   // gmsTranslateThreeDimPoint
   //
   // Description:
   //    This utility applies 3-dimensional translation to
   //    the specified point.
   //-----------------------------------------------------*/
   gms_3D_ScreenPointType gmsTranslateThreeDimPoint
                             (gms_3D_ScreenPointType thePoint,
                              int                    x,
                              int                    y,
                              int                    z);


   //-----------------------------------------------------*/
   // gmsTranslate_3D_Polyline
   //
   // Description:
   //    This utility applies 3-dimensional translation to
   //    each of the points of the specified polyline (of
   //    screen coordinates).
   //-----------------------------------------------------*/
   void gmsTranslate_3D_Polyline
           (gms_3D_ScreenPolylineType *ptrToPolyline,
            int                       x,
            int                       y,
            int                       z);


   //-----------------------------------------------------*/
   // gmsRotateThreeDimPointAbout_X_Axis
   //
   // Description:
   //    This utility applies a 3-dimensional rotation of
   //    the specified point around the x-axis.  The caller
   //    specifies the degrees of rotation.
   //-----------------------------------------------------*/
   gms_3D_ScreenPointType gmsRotateThreeDimPointAbout_X_Axis
                                (gms_3D_ScreenPointType thePoint,
                                 double                 angleOfRotation);


   //-----------------------------------------------------*/
   // gmsRotateThreeDimPointAbout_Y_Axis
   //
   // Description:
   //    This utility applies a 3-dimensional rotation of
   //    the specified point around the y-axis.  The caller
   //    specifies the degrees of rotation.
   //-----------------------------------------------------*/
   gms_3D_ScreenPointType gmsRotateThreeDimPointAbout_Y_Axis
                                (gms_3D_ScreenPointType thePoint,
                                 double                 angleOfRotation);


   //-----------------------------------------------------*/
   // gmsRotateThreeDimPointAbout_Z_Axis
   //
   // Description:
   //    This utility applies a 3-dimensional rotation of
   //    the specified point around the z-axis.  The caller
   //    specifies the degrees of rotation.
   //-----------------------------------------------------*/
   gms_3D_ScreenPointType gmsRotateThreeDimPointAbout_Z_Axis
                                (gms_3D_ScreenPointType thePoint,
                                 double                 angleOfRotation);


   //-----------------------------------------------------*/
   // gmsRotateThreeDimPolyline
   //
   // Description:
   //    This utility applies a 3-dimensional rotation of
   //    the specified point around all axes.  The caller
   //    specifies the degrees of rotation for each axis.
   //
   //    Note:
   //       The rotations are applied to the polyline passed
   //       in by the client.  The original values of the
   //       incoming polyline are overwritten with the
   //       rotated values.
   //-----------------------------------------------------*/
   void gmsRotateThreeDimPolyline
           (double                    xRotation,
            double                    yRotation,
            gms_3D_ScreenPolylineType *ptrTo_3D_PolylineInOut);

#endif
