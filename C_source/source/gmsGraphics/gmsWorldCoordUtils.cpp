/*---------------------------------------------------------------------------*/
/* File: gmsWorldCoordUtils.cpp
/* Date: 13-Oct-99 : initial definition
/*       19-Oct-99 : latitude lines go from -90.0 Long to 90.0 Long
/*       20-Oct-99 : implement (x,y,z) to (lat,long) utilities
/*       11-Nov-99 : polyline types changed 'dynamic' size
/*       22-Nov-99 : added "tweak" factors to stretch flat-earth alogrithm
/*       09-Dec-99 : rename file
/*       10-Dec-99 : check only for "sphere" and "ellipsoid" for model
/*
/* Description:
/*    This component defines a series of utilities to convert lat/long
/*    coordinates to "real-world" coordinates.  The output values
/*    are "Earth-Centered-Earth-Fixed" coordinates which is a
/*    cartesion-like coordinate (x, y, z).
/*
/*    The coorinate system of the ECEF system is defined as follows.  The
/*    earth is centered on a 3-D Cartesian coordinate system.  The north-pole
/*    is at the top, south-pole at the bottom and defines the y-axis.  The
/*    prime-meridian is at the front.  The x-axis goes from the point
/*    (-90.0 long, 0.0 lat) to (90.0 long, 0.0 lat) through the center of
/*    the earth.  The z-axis goes from the point (0.0 long, 0.0 lat) to the
/*    point (180.0 long, 0.0 lat) through the center of the earth.  The
/*    earth may be rotated, translated, etc, but these transformations are
/*    all relative to this initial frame of reference.
/*
/*    These utilities use real-world values as input and output.  There are
/*    no screen/CRT specific operations performed by these utilities.
/*    The input (Long, Lat) values are expressed in degrees.  Output
/*    values are expressed in meters.
/*
/*    Flat-Earth Coordinate System:
/*    -----------------------------
/*       This coordinate system models the Earth as a flat rectangle.
/*
/*    Spherical Coordinate System:
/*    ----------------------------
/*       This coordinate system models the Earth as a (perfect) sphere.
/*
/*    Ellipsoid Coordinate System:
/*    ----------------------------
/*       This coordinate system models the Earth as an ellipsoid.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <gmsWorldCoordUtils.h>
#include <gmsDebugUtil.h>


/*----------------------*/
/* Constants/Globals    */
/*----------------------*/

static const double Earth_Radius_Meters = Semi_Major_Axis_Meters;

static const double Long_To_Flat_X = PI * Earth_Radius_Meters / Max_Long;

static const double b_ToTheFourth = b_squared * b_squared;

static const double a_Sqrd_Over_b_Sqrd = a_squared / b_squared;

static const double smallestPosMeter = 0.01;

static const double smallestNegMeter = -0.01;




/*-----------------------------------------------------*/
/* gmsConvertLatLongToFlatPoint
/*
/* Description:
/*    Converts the specified (Lat/Long) coordinate to
/*    an (x, y) real-world coordinate using a "flat-
/*    earth" algorithm.
/*
/*    This algorithm is a variant on the cylindrical-
/*    projection method (see "gmsConvertLatLongTo-
/*    CylindricalPoint" below).  The shpere (i.e. the
/*    Earth) is placed in the middle of a cylinder.  All
/*    points on the sphere are projected onto the wall
/*    of the cylinder.  The cylinder is then un-ravelled
/*    and laid flat ... thereby giving a flat Earth.
/*
/*    Like the cylindrical model, the circumference of
/*    the cylinder is the same as the sphere.  The
/*    range of the x-coordinate is:
/*
/*                 - Pi * Re .. Pi * Re
/*
/*    Also, the height of the cylinder is twice the
/*    radius of the sphere which is 2 * Re.  Hence,
/*    the range of the y-coordinate is:
/*
/*                  - Re .. Re
/*
/*    The value of x is therefore calculated from
/*
/*            x = longitude * (Pi * Re / 180)
/*
/*    However, unlike the cylindrical model, the value
/*    of the y-coordinate is calculated using the
/*    latitude value as a percentage of the maximum
/*    latitude, or:
/*
/*            y = (latitude / Max_Latitude) * Re
/*
/*    Reference:
/*    ----------
/*       "Introduction to Map Projections",
/*       P. W. McDonnel Jr., Marcel Dekker Inc,
/*       New York, 1979, pg 21-23.
/*-----------------------------------------------------*/
gms_2D_WorldPointType gmsConvertLatLongToFlatPoint
                               (twoDimCoordType theCoord)

{
         const double          percentLat = theCoord.Lat / Max_Lat;
         gms_2D_WorldPointType theAnswer;
         const float           x_helpFactor = (float) 0.85;
         const float           y_helpFactor = (float) 1.66;

   theAnswer.x_meters = (float) (theCoord.Long * Long_To_Flat_X);

   theAnswer.x_meters = theAnswer.x_meters * x_helpFactor;

   theAnswer.y_meters = (float) (percentLat * Earth_Radius_Meters);

   theAnswer.y_meters = theAnswer.y_meters * y_helpFactor;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvertLatLongToCylindricalPoint
/*
/* Description:
/*    Converts the specified (Lat/Long) coordinate to
/*    an (x, y) real-world coordinate using a "cylindrical"
/*    model of the Earth.
/*
/*    This algorithm is based on the idea of placing a
/*    shpere (i.e. the Earth) in the middle of a cylinder.
/*    All points on the sphere are then (parallel) projected
/*    onto the wall of the cylinder.  The cylinder is then
/*    un-ravelled and laid flat ... thereby giving a flat
/*    Earth.  The un-ravelled cylinder is a Cartesian plane.
/*
/*    Since the circumference of the sphere is the same as
/*    the cylinder's, then the width of the Cartesian plane
/*    is 2 * PI * Re (where Re is the radius of the sphere).
/*    Hence, the range of x-coordinate is:
/*
/*                 - Pi * Re .. Pi * Re
/*
/*    Furthermore, the height of the cylinder is twice the
/*    radius of the sphere which is 2 * Re.  Hence, the
/*    range of the y-coordinate is:
/*
/*                  - Re .. Re
/*
/*    The value of x is therefore calculated from
/*
/*            x = longitude * (Pi * Re / 180)
/*
/*    Also, the value of y is therefore
/*
/*            y = Re * sin(latitude)
/*
/*    Reference:
/*    ----------
/*       "Introduction to Map Projections",
/*       P. W. McDonnel Jr., Marcel Dekker Inc,
/*       New York, 1979, pg 21-23.
/*-----------------------------------------------------*/
gms_2D_WorldPointType gmsConvertLatLongToCylindricalPoint
                               (twoDimCoordType theCoord)

{
         const double          sinLat = sin(theCoord.Lat * degToRad);
         double                temp;
         gms_2D_WorldPointType theAnswer;

   theAnswer.x_meters = (float) (theCoord.Long * Long_To_Flat_X);

   temp = Earth_Radius_Meters * sinLat;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.y_meters = (float) temp;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvertLatLongToSphericalPoint
/*
/* Description:
/*    This utility converts the specified (Lat, Long)
/*    coordinate to a 3-D point (x, y, z) of a sphere
/*    whose origin is (0, 0, 0).
/*
/*    The following algorithm is used:
/*
/*       x = Re * cos(PHI) * sin(LAMBDA)
/*
/*       y = Re * sin(PHI)
/*
/*       z = Re * cos(PHI) * cos(LAMBDA)
/*
/*    where:
/*       PHI    = latitude
/*       LAMBDA = longitude
/*       Re     = Radius of (spherical) Earth = 6378137.0 meters
/*-----------------------------------------------------*/
gms_3D_WorldPointType gmsConvertLatLongToSphericalPoint
                           (twoDimCoordType theCoord)

{
         const double          latRadians  = (theCoord.Lat * degToRad);
         const double          longRadians = (theCoord.Long * degToRad);
         const double          cosLat  = cos(latRadians);
         const double          cosLong = cos(longRadians);
         const double          sinLat  = sin(latRadians);
         const double          sinLong = sin(longRadians);
         double                temp;
         gms_3D_WorldPointType theAnswer;

   temp = Earth_Radius_Meters * cosLat * sinLong;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.x_meters = (float) temp;

   temp = Earth_Radius_Meters * sinLat;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.y_meters = (float) temp;

   temp = Earth_Radius_Meters * cosLat * cosLong;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.z_meters = (float) temp;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvertSphericalPointToLatLong
/*
/* Description:
/*    This utility converts the specified (x,y, z)
/*    point from a sphere to the corresponding (Lat, Long)
/*    coordinate.
/*
/*    The following algorithm is used:
/*
/*                       y
/*       PHI = arcsin (-----)
/*                       Re
/*
/*                         x
/*       LAMBDA = arctan (---)
/*                         z
/*
/*    where:
/*       PHI    = latitude
/*       LAMBDA = longitude
/*       Re     = Radius of (spherical) Earth = 6378137.0 meters
/*-----------------------------------------------------*/
twoDimCoordType gmsConvertSphericalPointToLatLong
                            (gms_3D_WorldPointType thePoint)

{
      twoDimCoordType theAnswer;
      double          tempDegrees;

   tempDegrees = asin(thePoint.y_meters / Earth_Radius_Meters) * radToDeg;

   if ( (tempDegrees < 0.0001) && (tempDegrees > -0.0001) )
      tempDegrees = 0.0;

   theAnswer.Lat = (float) tempDegrees;

   if ( (theAnswer.Lat > 89.9990) || (theAnswer.Lat < -89.9990) )
      {
       /*--------------------------*/
       /* Point is (essentially)   */
       /* at the north or south    */
       /* poles.                   */
       /*--------------------------*/
       theAnswer.Long = 0.0;
      }

   else if ( (thePoint.z_meters > 0.001) || (thePoint.z_meters < -0.001) )
      {
       /*--------------------------*/
       /* z-component of the point */
       /* is not 0.  Divide is safe*/
       /*--------------------------*/
       tempDegrees = atan (thePoint.x_meters / thePoint.z_meters) * radToDeg;

       if ( (tempDegrees < 0.001) && (tempDegrees > -0.001) )
          tempDegrees = 0.0;

       if (tempDegrees < 0.0)
          tempDegrees = -tempDegrees;

       if (thePoint.x_meters < 0.0)
          tempDegrees = -tempDegrees;

       if ( (thePoint.x_meters >= 0.0) && (thePoint.z_meters < 0.0) )
          {
           /*--------------------------*/
           /* This is quadrant II of   */
           /* the (x,z) plane.         */
           /*--------------------------*/
           tempDegrees = 180.00 - tempDegrees;
          }

       else if ( (thePoint.x_meters < 0.0) && (thePoint.z_meters < 0.0) )
          {
           /*--------------------------*/
           /* This is quadrant III of  */
           /* the (x,z) plane.         */
           /*--------------------------*/
           tempDegrees = -180.00 - tempDegrees; /* i.e. make it more neg */
         }

       theAnswer.Long = (float) tempDegrees;
      }

   else
      {
       /*--------------------------------*/
       /* z is (or very close to) 0.00.  */
       /* Thus, prevent divide-by-zero.  */
       /*--------------------------------*/
       if (thePoint.x_meters < 0.0)
          tempDegrees = -90.0;
       else
          tempDegrees = 90.0;

       theAnswer.Long = (float) tempDegrees;
      }

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvertLatLongToEllipsoidPoint
/*
/* Description:
/*    This utility converts the specified (Lat, Long)
/*    coordinate to a 3-D point (x, y, z) of an ellipsoid.
/*    The following algorithm is used:
/*
/*       x = (N + h) * cos(PHI) * sin(LAMBDA)
/*
/*             b * b
/*       y = (------- * N + h) * sin(PHI)
/*             a * a
/*
/*       z = (N + h) * cos(PHI) * cos(LAMBDA)
/*
/*    where:
/*       PHI    = geodetic latitude
/*       LAMBDA = longitude
/*       h      = altitude
/*       a      = semi-major axis = 6378137.0 meters
/*       b      = semi-minor axis = 6356752.0 meters
/*       N      = a / sqrt(1 - e * e * sin(PHI) * sin(PHI))
/*       e      = first eccentricity = sqrt(2 * f - f * f)
/*       f      = earth-flattening   = (a - b) / a
/*
/*    References:
/*    -----------
/*       1) "NIMA TR 8350.2 - Department of Defense
/*          World Geodetic System 1984 - 3rd ed.",
/*          National Imagery and Mapping Agency,
/*          4600 Sangamore Rd, Bethesda, Maryland,
/*          20816-5003, 04-Jul-97, pg 4-4.
/*          (see also : www.nima.gov)
/*       2) "Map Projections for Geodesists, Cartographers
/*          and Geographers", P. Richardus, R. K. Adler,
/*          American Elsevier Publishing Co. Inc., New
/*          York, 1972, pg 13-20. (Call Num : GA110R52)
/*-----------------------------------------------------*/
gms_3D_WorldPointType gmsConvertLatLongToEllipsoidPoint
                           (twoDimCoordType theCoord)

{
         const double          latRadians  = (theCoord.Lat * degToRad);
         const double          longRadians = (theCoord.Long * degToRad);
         const double          cosLat  = cos(latRadians);
         const double          cosLong = cos(longRadians);
         const double          sinLat  = sin(latRadians);
         const double          sinLong = sin(longRadians);
         const double          sinLatSqrd = sinLat * sinLat;
         double                temp;
         double                N;
         gms_3D_WorldPointType theAnswer;

   temp = sqrt (1.0 - epsilon_sqrd * sinLatSqrd);

   N = (Semi_Major_Axis_Meters / temp);

   /* x-meters */
   temp = N * cosLat * sinLong;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.x_meters = (float) temp;

   /* y-meters */
   temp = (Axis_Ratio_Sqrd * N) * sinLat;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.y_meters = (float) temp;

   /* z-meters */
   temp = N * cosLat * cosLong;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.z_meters = (float) temp;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsConvertEllipsoidPointToLatLong
/*
/* Description:
/*    This utility converts the specified (x,y, z)
/*    point from an ellipsoid to the corresponding
/*    (Lat, Long) coordinate.
/*
/*    The following algorithm is used:
/*
/*                      4              2
/*       temp = sqrt ( b  + (y * a * e) )
/*
/*                      a * y
/*       PHI = arcsin (-------)
/*                      temp
/*
/*    Therefore:
/*                             x
/*       LAMBDA = arcsin(--------------)
/*                        N * cos(PHI)
/*
/*    where:
/*       PHI    = geodetic latitude
/*       LAMBDA = longitude
/*       h      = altitude
/*       a      = semi-major axis = 6378137.0 meters
/*       b      = semi-minor axis = 6356752.0 meters
/*       N      = a / sqrt(1 - e * e * sin(PHI) * sin(PHI))
/*       e      = first eccentricity = sqrt(2 * f - f * f)
/*       f      = earth-flattening   = (a - b) / a
/*
/*    References:
/*    -----------
/*       1) "NIMA TR 8350.2 - Department of Defense
/*          World Geodetic System 1984 - 3rd ed.",
/*          National Imagery and Mapping Agency,
/*          4600 Sangamore Rd, Bethesda, Maryland,
/*          20816-5003, 04-Jul-97, pg 4-4.
/*          (see also : www.nima.gov)
/*       2) "Map Projections for Geodesists, Cartographers
/*          and Geographers", P. Richardus, R. K. Adler,
/*          American Elsevier Publishing Co. Inc., New
/*          York, 1972, pg 13-20. (Call Num : GA110R52)
/*-----------------------------------------------------*/
twoDimCoordType gmsConvertEllipsoidPointToLatLong
                            (gms_3D_WorldPointType thePoint)

{
         const double    numerator     = Semi_Major_Axis_Meters *
                                            thePoint.y_meters;
         const double    temp_1        = numerator * epsilon;
         const double    temp_2        = temp_1 * temp_1;
         const double    denominator   = sqrt (b_ToTheFourth + temp_2);
         const double    sinLatRadians = numerator / denominator;
         const double    latRadians    = asin (sinLatRadians);
         double          N;
         double          temp;
         twoDimCoordType theAnswer;

   temp = latRadians * radToDeg;

   if ( (temp < 0.01) && (temp > -0.01) )
      temp = 0.0;

   theAnswer.Lat = (float) temp;

   N = a_Sqrd_Over_b_Sqrd * ( 1.0 / sinLatRadians) * thePoint.y_meters;

   temp = asin ( thePoint.x_meters / (N * cos(latRadians) ) );

   if ( (temp < 0.01) && (temp > -0.01) )
      temp = 0.0;

   theAnswer.Long = (float) (temp * radToDeg);

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsBuildLongitudeLine
/*
/* Description:
/*    This utility generates a polyline that represents
/*    a longitudinal line on the globe (i.e. a meridian
/*    line from north-pole to south-pole).  The caller
/*    specifies a longitude value between (-180.0 to 180.0).
/*
/*    The caller allocates a polyline object (which also
/*    indicates the number of points in the polyline).
/*    The caller passes that (empty) object into this
/*    utility and an array of real-world coordinates is
/*    written to the polyline object.  Hence, it is the
/*    caller's resposibility to "free" the polyline once
/*    finished.
/*
/*    This utility can be used, for example, to construct
/*    a wire frame globe using a series of polylines at
/*    10 or 20 degree increments.
/*-----------------------------------------------------*/
void gmsBuildLongitudeLine
           (double                   longitude,
            gmsEarthModelType        whichModel,
            gms_3D_WorldPolylineType *ptrToPolyline)

{
      twoDimCoordType tempCoord;
      float           deltaLat;
      int             i;

   if (ptrToPolyline == NULL)
      {
       gmsWriteDebug ("*** ERROR : gmsBuildLongitudeLine : null polyline\n");

       return;
      }

   if (ptrToPolyline->numPoints == 0)
      {
       gmsWriteDebug ("*** ERROR : gmsBuildLongitudeLine : numPoints = 0?\n");

       return;
      }

   if ( (longitude > Max_Long) || (longitude < Min_Long) )
      {
             char msg[128];

       sprintf(msg, "*** ERROR : gmsBuildLongitudeLine - lon = %f too big\n",
               longitude);

       gmsWriteDebug (msg);

       printf(msg);

       return;   
      }

   deltaLat = (float) 180.0 / ptrToPolyline->numPoints;

   tempCoord.Lat = Max_Lat;

   tempCoord.Long = (float) longitude;

   if (whichModel == gmsSphere)
      {
       for (i = 0; i < ptrToPolyline->numPoints; i++)
          {
           ptrToPolyline->points[i] =
              gmsConvertLatLongToSphericalPoint (tempCoord);

           tempCoord.Lat = tempCoord.Lat - deltaLat;

           if (tempCoord.Lat < Min_Lat)
              tempCoord.Lat = Min_Lat;
          }

        tempCoord.Lat = Min_Lat;

        ptrToPolyline->points[i - 1] =
           gmsConvertLatLongToSphericalPoint (tempCoord);
      }

   else if (whichModel == gmsEllipsoid)
      {
       for (i = 0; i < ptrToPolyline->numPoints; i++)
          {
           ptrToPolyline->points[i] =
              gmsConvertLatLongToEllipsoidPoint (tempCoord);

           tempCoord.Lat = tempCoord.Lat - deltaLat;

           if (tempCoord.Lat < Min_Lat)
              tempCoord.Lat = Min_Lat;
          }

        tempCoord.Lat = Min_Lat;

        ptrToPolyline->points[i - 1] =
           gmsConvertLatLongToEllipsoidPoint (tempCoord);
      }
}


/*-----------------------------------------------------*/
/* gmsBuildLatitudeLine
/*
/* Description:
/*    This utility generates a polyline that represents
/*    a laditudinal line on the globe (i.e. a parallel
/*    line from east to west).  The caller specifies a
/*    latitude value between (-90.0 to 90.0).
/*
/*    The caller allocates a polyline object (which also
/*    indicates the number of points in the polyline).
/*    The caller passes that (empty) object into this
/*    utility and an array of real-world coordinates is
/*    written to the polyline object.  Hence, it is the
/*    caller's resposibility to "free" the polyline once
/*    finished.
/*
/*    This utility can be used, for example, to construct
/*    a wire frame globe using a series of polylines at
/*    10 or 20 degree increments.
/*-----------------------------------------------------*/
void gmsBuildLatitudeLine
           (double                   latitude,
            gmsEarthModelType        whichModel,
            gms_3D_WorldPolylineType *ptrToPolyline)

{
         twoDimCoordType tempCoord;
         float           deltaLong;
         int             i;

   if (ptrToPolyline == NULL)
      {
       gmsWriteDebug ("*** ERROR : gmsBuildLatitudeLine : null polyline\n");

       return;
      }

   if (ptrToPolyline->numPoints == 0)
      {
       gmsWriteDebug ("*** ERROR : gmsBuildLatitudeLine : numPoints = 0?\n");

       return;
      }

   if ( (latitude > Max_Lat) || (latitude < Min_Lat) )
      {
             char msg[128];

       sprintf(msg, "*** ERROR : gmsBuildLatitudeLine - lat = %f too big\n",
               latitude);

       gmsWriteDebug (msg);

       printf(msg);

       return;   
      }

   deltaLong = (float) 360.0 / ptrToPolyline->numPoints;

   tempCoord.Long = -180.0;

   tempCoord.Lat = (float) latitude;

   if (whichModel == gmsSphere)
      {
       for (i = 0; i < ptrToPolyline->numPoints; i++)
          {
           ptrToPolyline->points[i] =
              gmsConvertLatLongToSphericalPoint (tempCoord);

           tempCoord.Long = tempCoord.Long + deltaLong;

           if (tempCoord.Long > 180.0)
              tempCoord.Long = 180.0;
          }

        tempCoord.Long = 180.0;

        ptrToPolyline->points[i - 1] =
           gmsConvertLatLongToSphericalPoint (tempCoord);
      }

   else if (whichModel == gmsEllipsoid)
      {
       for (i = 0; i < ptrToPolyline->numPoints; i++)
          {
           ptrToPolyline->points[i] =
              gmsConvertLatLongToEllipsoidPoint (tempCoord);

           tempCoord.Long = tempCoord.Long + deltaLong;

           if (tempCoord.Long > 180.0)
              tempCoord.Long = 180.0;
          }

        tempCoord.Long = 180.0;

        ptrToPolyline->points[i - 1] =
           gmsConvertLatLongToEllipsoidPoint (tempCoord);
      }
}


/*-----------------------------------------------------*/
/* gmsRotateThreeDimWorldPointAbout_X_Axis
/*
/* Description:
/*    This utility applies a 3-dimensional rotation of
/*    the specified point around the x-axis of the ECEF
/*    coordinate system.
/*
/*    The caller specifies the degrees of rotation.  The
/*    transformation matrix is:
/*
/*                | 1      0      0      0 |
/*                |                        |
/*                | 0   cos(d)  -sin(d)  0 |
/*           Tx = |                        |
/*                | 0   sin(d)  cos(d)   0 |
/*                |                        |
/*                | 0      0      0      1 |
/*
/*    where d is the angle-of-rotation.
/*-----------------------------------------------------*/
gms_3D_WorldPointType gmsRotateThreeDimWorldPointAbout_X_Axis
                           (gms_3D_WorldPointType thePoint,
                            double                angleOfRotation)

{
         const double          theSin = sin(angleOfRotation * degToRad);
         const double          theCos = cos(angleOfRotation * degToRad);
         double                temp;
         gms_3D_WorldPointType theAnswer;

   theAnswer.x_meters = thePoint.x_meters;

   temp = thePoint.y_meters * theCos + thePoint.z_meters * theSin;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.y_meters = (float) temp;

   temp = -thePoint.y_meters * theSin + thePoint.z_meters * theCos;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.z_meters = (float) temp;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsRotateThreeDimWorldPointAbout_Y_Axis
/*
/* Description:
/*    This utility applies a 3-dimensional rotation of
/*    the specified point around the y-axis of the ECEF
/*    coordinate system.  The caller specifies the degrees
/*    of rotation.  The transformation matrix is:
/*
/*                | cos(d) 0   -sin(d)  0 |
/*                |                       |
/*                | 0      1     0      0 |
/*           Ty = |                       |
/*                | sin(d) 0   cos(d)   0 |
/*                |                       |
/*                | 0      0     0      1 |
/*
/*    where d is the angle-of-rotation.
/*-----------------------------------------------------*/
gms_3D_WorldPointType gmsRotateThreeDimWorldPointAbout_Y_Axis
                           (gms_3D_WorldPointType thePoint,
                            double                angleOfRotation)

{
         const double          theSin = sin(angleOfRotation * degToRad);
         const double          theCos = cos(angleOfRotation * degToRad);
         double                temp;
         gms_3D_WorldPointType theAnswer;

   temp = thePoint.x_meters * theCos + thePoint.z_meters * theSin;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.x_meters = (float) temp;

   theAnswer.y_meters = thePoint.y_meters;

   temp = -thePoint.x_meters * theSin + thePoint.z_meters * theCos;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.z_meters = (float) temp;

   return theAnswer;
}


/*-----------------------------------------------------*/
/* gmsRotateThreeDimWorldPointAbout_Z_Axis
/*
/* Description:
/*    This utility applies a 3-dimensional rotation of
/*    the specified point around the z-axis of the ECEF
/*    coordinate system.  The caller specifies the degrees
/*    of rotation.  The transformation matrix is:
/*
/*                | cos(d) -sin(d) 0    0 |
/*                |                       |
/*                | sin(d) cos(d)  0    0 |
/*           Tx = |                       |
/*                |   0      0     1    0 |
/*                |                       |
/*                |   0      0     1    1 |
/*
/*    where d is the angle-of-rotation about the z-axis.
/*-----------------------------------------------------*/
gms_3D_WorldPointType gmsRotateThreeDimWorldPointAbout_Z_Axis
                           (gms_3D_WorldPointType thePoint,
                            double                angleOfRotation)

{
         const double          theSin = sin(angleOfRotation * degToRad);
         const double          theCos = cos(angleOfRotation * degToRad);
         double                temp;
         gms_3D_WorldPointType theAnswer;

   temp = thePoint.x_meters * theCos + thePoint.y_meters * theSin;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.x_meters = (float) temp;

   temp = -thePoint.x_meters * theSin + thePoint.y_meters * theCos;

   if ( (temp < smallestPosMeter) && (temp > smallestNegMeter) )
      temp = 0.0;

   theAnswer.y_meters = (float) temp;

   theAnswer.z_meters = thePoint.z_meters;

   return theAnswer;
}
