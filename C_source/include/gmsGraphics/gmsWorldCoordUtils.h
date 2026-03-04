/*---------------------------------------------------------------------------
 * File: gmsWorldCoordUtils.h
 * Date: 13-Oct-99 : initial definition
 *       20-Oct-99 : implement (x,y,z) to (lat,long) utilities
 *       21-Oct-99 : implement rotation utils for world coords
 *       11-Nov-99 : polyline types changed 'dynamic' size
 *       09-Dec-99 : rename file
 *       10-Dec-99 : move "model" type to global types package
 *
 * Description:
 *    This component defines a series of utilities to convert lat/long
 *    coordinates to "real-world" coordinates.  The output values
 *    are "Earth-Centered-Earth-Fixed" coordinates which is a
 *    cartesion-like coordinate (x, y, z).
 *
 *    These utilities use real-world values as input and output.  There are
 *    no screen/CRT specific operations performed by these utilities.
 *    The input (Long, Lat) values are expressed in degrees.  Output
 *    values are expressed in meters.
 *
 *    Flat-Earth Coordinate System:
 *    -----------------------------
 *       This coordinate system models the Earth as a rectangle.
 *
 *    Cylindrical Coordinate System:
 *    -----------------------------
 *       This coordinate system models the Earth as an unravelled cylinder.
 *
 *    Spherical Coordinate System:
 *    ----------------------------
 *       This coordinate system models the Earth as a (perfect) sphere.
 *
 *    Ellipsoid Coordinate System:
 *    ----------------------------
 *       This coordinate system models the Earth as an ellipsoid.
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *---------------------------------------------------------------------------*/
#ifndef GMS_WORLD_COORD_UTIL

   #define GMS_WORLD_COORD_UTIL

   #include <gmsTypesAndConstants.h>
   #include <gmsUtilities.h>


   /*-----------------------------------------------------
    * gmsConvertLatLongToFlatPoint
    *
    * Description:
    *    Converts the specified (Lat/Long) coordinate to
    *    an (x, y) real-world coordinate using a "flat-earth"
    *    algorithm.
    *-----------------------------------------------------*/
   gms_2D_WorldPointType gmsConvertLatLongToFlatPoint
                               (twoDimCoordType theCoord);


   /*-----------------------------------------------------
    * gmsConvertLatLongToCylindricalPoint
    *
    * Description:
    *    Converts the specified (Lat/Long) coordinate to
    *    an (x, y) real-world coordinate using a "cylindrical"
    *    model of the Earth.
    *-----------------------------------------------------*/
   gms_2D_WorldPointType gmsConvertLatLongToCylindricalPoint
                               (twoDimCoordType theCoord);


   /*-----------------------------------------------------
    * gmsConvertLatLongToSphericalPoint
    *
    * Description:
    *    This utility converts the specified (Lat, Long)
    *    coordinate to a 3-D point (x, y, z) of a sphere.
    *-----------------------------------------------------*/
   gms_3D_WorldPointType gmsConvertLatLongToSphericalPoint
                               (twoDimCoordType theCoord);


   /*-----------------------------------------------------
    * gmsConvertSphericalPointToLatLong
    *
    * Description:
    *    This utility converts the specified (x,y, z)
    *    point from a sphere to the corresponding (Lat, Long)
    *    coordinate.
    *-----------------------------------------------------*/
   twoDimCoordType gmsConvertSphericalPointToLatLong
                               (gms_3D_WorldPointType thePoint);


   /*-----------------------------------------------------
    * gmsConvertLatLongToEllipsoidPoint
    *
    * Description:
    *    This utility converts the specified (Lat, Long)
    *    coordinate to a 3-D point (x, y, z) of an ellipsoid.
    *-----------------------------------------------------*/
   gms_3D_WorldPointType gmsConvertLatLongToEllipsoidPoint
                               (twoDimCoordType theCoord);


   /*-----------------------------------------------------
    * gmsConvertEllipsoidPointToLatLong
    *
    * Description:
    *    This utility converts the specified (x,y, z)
    *    point from an ellipsoid to the corresponding
    *    (Lat, Long) coordinate.
    *-----------------------------------------------------*/
   twoDimCoordType gmsConvertEllipsoidPointToLatLong
                               (gms_3D_WorldPointType thePoint);


   /*-----------------------------------------------------
    * gmsBuildLongitudeLine
    *
    * Description:
    *    This utility generates a polyline that represents
    *    a longitudinal line on the globe (i.e. a meridian
    *    line from north-pole to south-pole).  The caller
    *    specifies a longitude value between (-180.0 to 180.0).
    *
    *    The caller allocates a polyline object (which also
    *    indicates the number of points in the polyline).
    *    The caller passes that (empty) object into this
    *    utility and an array of real-world coordinates is
    *    written to the polyline object.  Hence, it is the
    *    caller's resposibility to "free" the polyline once
    *    finished.
    *
    *    This utility can be used, for example, to construct
    *    a wire frame globe using a series of polylines at
    *    10 or 20 degree increments.
    *-----------------------------------------------------*/
   void gmsBuildLongitudeLine
           (double                   longitude,
            gmsEarthModelType        whichModel,
            gms_3D_WorldPolylineType *ptrToPolyline);


   /*-----------------------------------------------------
    * gmsBuildLatitudeLine
    *
    * Description:
    *    This utility generates a polyline that represents
    *    a laditudinal line on the globe (i.e. a parallel
    *    line from east to west).  The caller specifies a
    *    latitude value between (-90.0 to 90.0).
    *
    *    The caller allocates a polyline object (which also
    *    indicates the number of points in the polyline).
    *    The caller passes that (empty) object into this
    *    utility and an array of real-world coordinates is
    *    written to the polyline object.  Hence, it is the
    *    caller's resposibility to "free" the polyline once
    *    finished.
    *
    *    This utility can be used, for example, to construct
    *    a wire frame globe using a series of polylines at
    *    10 or 20 degree increments.
    *-----------------------------------------------------*/
   void gmsBuildLatitudeLine
           (double                   latitude,
            gmsEarthModelType        whichModel,
            gms_3D_WorldPolylineType *ptrToPolyline);


   /*-----------------------------------------------------
    * gmsRotateThreeDimWorldPointAbout_X_Axis
    *
    * Description:
    *    This utility applies a 3-dimensional rotation of
    *    the specified point around the x-axis of the ECEF
    *    coordinate system.  The caller specifies the degrees
    *    of rotation.
    *-----------------------------------------------------*/
   gms_3D_WorldPointType gmsRotateThreeDimWorldPointAbout_X_Axis
                              (gms_3D_WorldPointType thePoint,
                               double                angleOfRotation);


   /*-----------------------------------------------------
    * gmsRotateThreeDimWorldPointAbout_Y_Axis
    *
    * Description:
    *    This utility applies a 3-dimensional rotation of
    *    the specified point around the y-axis of the ECEF
    *    coordinate system.  The caller specifies the degrees
    *    of rotation.
    *-----------------------------------------------------*/
   gms_3D_WorldPointType gmsRotateThreeDimWorldPointAbout_Y_Axis
                              (gms_3D_WorldPointType thePoint,
                               double                angleOfRotation);


   /*-----------------------------------------------------
    * gmsRotateThreeDimWorldPointAbout_Z_Axis
    *
    * Description:
    *    This utility applies a 3-dimensional rotation of
    *    the specified point around the z-axis of the ECEF
    *    coordinate system.  The caller specifies the degrees
    *    of rotation.
    *-----------------------------------------------------*/
   gms_3D_WorldPointType gmsRotateThreeDimWorldPointAbout_Z_Axis
                              (gms_3D_WorldPointType thePoint,
                               double                angleOfRotation);

#endif
