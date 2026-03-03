/*---------------------------------------------------------------------------*/
/* File: gmsTypesAndConstants.h
/* Date: 27-Sep-99 : initial definition
/*       14-Oct-99 : distinguish between screen coords and real-world coords
/*       09-Dec-99 : had to remove constants "a" and "b" due to VC++ bug!
/*       10-Dec-99 : earth model type moved here
/*       21-Dec-99 : moved enum lists of (browse and regional) themes here
/*       21-Dec-99 : added subtile and file-type enumerations
/*       07-Jan-00 : re-order enums in 'gmsDcwFileType', add more 'doc'
/*
/* Description:
/*    Common types and useful constants.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/
#ifndef GMS_TYPES

   #define GMS_TYPES

   /*-------------------------------------------------------*/
   /*                 Type Declarations
   /*-------------------------------------------------------*/

   typedef enum { gmsFlat,
                  gmsSphere,
                  gmsEllipsoid } gmsEarthModelType;

   typedef enum { gms_AE,        /* Aeronautical Coverage      */
                  gms_CL,        /* Cultural Landmarks         */
                  gms_DQ,        /* Data Quality               */
                  gms_DN,        /* Drainage                   */
                  gms_DS,        /* Drainage - supplemental    */
                  gms_HY,        /* Hypsography                */
                  gms_HS,        /* Hypsography - supplemental */
                  gms_LC,        /* Land Coverage              */
                  gms_OF,        /* Ocean Features             */
                  gms_PH,        /* Physiography               */
                  gms_PO,        /* Political Oceans           */
                  gms_PP,        /* Populated Places           */
                  gms_RR,        /* Railroads                  */
                  gms_RD,        /* Roads                      */
                  gms_TS,        /* Transportation Sturcture   */
                  gms_UT,        /* Utilities                  */
                  gms_VG,        /* Vegetation                 */
                  gms_Gazetteer,
                  gms_TileRef,
                  gms_LibRef
                } gmsThematicType;

   #define Num_Themes 20

   typedef enum { gmsBrowse_CO,    /* BROWSE - ONC Compilation Date       */
                  gmsBrowse_DV,    /* BROWSE - Data Volume                */
                  gmsBrowse_DN,    /* BROWSE - Drainage                   */
                  gmsBrowse_GR,    /* BROWSE - Geographic Reference       */
                  gmsBrowse_DA,    /* BROWSE - Avail of Hypsographic Data */
                  gmsBrowse_IN,    /* BROWSE - ONC Index                  */
                  gmsBrowse_PO,    /* BROWSE - Political/Oceans           */
                  gmsBrowse_PP,    /* BROWSE - Populated Places           */
                  gmsBrowse_LibRef
                } gmsBrowseThematicType;

   #define Num_Browse_Themes 9

   typedef enum { gms_11,
                  gms_12,
                  gms_13,
                  gms_21,
                  gms_22,
                  gms_23,
                  gms_31,
                  gms_32,
                  gms_33
                } gmsSubtileType;

   #define Num_Subtiles 9

   typedef enum { /*----------------*/
                  /* Primitives     */
                  /*----------------*/
                  gmsEdgeTable,
                  gmsFaceTable,
                  gmsTextTable,
                  gmsNodeTable,
                  gmsEdgeBoundingRectangle,
                  gmsFaceBoundingRectangle,
                  gmsRingTable,
                  gmsEdgeSpatialIndex,
                  gmsFaceSpatialIndex,
                  gmsNodeSpatialIndex,
                  /*----------------*/
                  /* Theme files    */
                  /*----------------*/
                  gmsAreaFeatureTable,
                  gmsLineFeatureTable,
                  gmsPointFeatureTable,
                  gmsTextFeatureTable,
                  gmsAreaDocTable,
                  gmsLineDocTable,
                  gmsPointDocTable,
                  gmsFeatureClassSchema,
                  gmsCharVDT,
                  gmsIntVDT
                } gmsDcwFileType;

   #define Num_DCW_Files 20

   typedef struct
     {
      int x;
      int y;
      int z;
     } gms_3D_ScreenPointType; /* i.e. pixel coordinates */

   typedef struct
      {
       int                    numPoints;
       gms_3D_ScreenPointType *points;
      } gms_3D_ScreenPolylineType;

   typedef struct _gms_3D_ScreenImageType
      {
       int                       numLines;
       gms_3D_ScreenPolylineType *imageLines;
      } gms_3D_ScreenImageType;

   /*------------------*/

   typedef struct
     {
      int x;
      int y;
     } gms_2D_ScreenPointType; /* i.e. pixel coordinates */

   typedef struct
      {
       int                    numPoints;
       gms_2D_ScreenPointType *points;
      } gms_2D_ScreenPolylineType;

   typedef struct _gms_2D_ScreenImageType
      {
       int                       numLines;
       gms_2D_ScreenPolylineType *imageLines;
      } gms_2D_ScreenImageType;

   /*------------------*/

   typedef struct
     {
      float x_meters;
      float y_meters;
     } gms_2D_WorldPointType;  /* i.e. real-world coordinates */

   typedef struct
      {
       int                   numPoints;
       gms_2D_WorldPointType *points;
      } gms_2D_WorldPolylineType;

   typedef struct
      {
       int                      numLines;
       gms_2D_WorldPolylineType *imageLines;
      } gms_2D_WorldImageType;

   /*------------------*/

   typedef struct
     {
      float x_meters;
      float y_meters;
      float z_meters;
     } gms_3D_WorldPointType;  /* i.e. real-world coordinates */

   typedef struct
      {
       int                   numPoints;
       gms_3D_WorldPointType *points;
      } gms_3D_WorldPolylineType;

   typedef struct _gms_3D_WorldImageType
      {
       int                      numLines;
       gms_3D_WorldPolylineType *imageLines;
      } gms_3D_WorldImageType;


   /*-------------------------------------------------------*/
   /*                   General Constants:
   /*-------------------------------------------------------*/
   static const float Max_Lat  = 90.00f;

   static const float Max_Long = 180.00f;

   static const float Min_Lat  = -90.00f;

   static const float Min_Long = -180.00f;

   static const double PI = 3.1415926535;

   static const double degToRad = PI / 180.00;

   static const double radToDeg = 180.00 / PI;

   static const double Meters_Per_Km = 1000.0;

   static const double Km_Per_Meter = 0.001;


   /*-------------------------------------------------------*/
   /* Ellipsoid constants:
   /* --------------------
   /*     Ellipsoid          = WGS 84
   /*     Ellipsoid Details  = A=6378137,B=6356752 Meters
   /*     Datum Vertical Ref = MEAN SEA LEVEL
   /*     Vertical Datum Code= 015
   /*     Sounding Datuam    = MEAN SEA LEVEL
   /*     Sounding Datum Code= 015
   /*     Datum Geodetic Name= WGS 84
   /*     Datum Geodetic Code= WGE
   /*     Projection Name    = ROBINSON
   /*-------------------------------------------------------*/
   static const float Semi_Major_Axis_Meters = 6378137.0f;

   static const float a_squared = 
                         Semi_Major_Axis_Meters * Semi_Major_Axis_Meters;

   static const float Semi_Minor_Axis_Meters = 6356752.0f;

   static const float b_squared =
                         Semi_Minor_Axis_Meters * Semi_Minor_Axis_Meters;

   static const float Axis_Ratio_Sqrd = b_squared / a_squared;

   static const float Earth_Flattening =
             (Semi_Major_Axis_Meters - Semi_Minor_Axis_Meters) /
                         Semi_Major_Axis_Meters;

   static const float f = Earth_Flattening;

   static const float f_squared = f * f;

   static const float First_Eccentricity = 0.081819791f; /* by hand */

   static const float epsilon = First_Eccentricity;

   static const float First_Eccent_Sqrd = 2 * f - f_squared;

   static const float epsilon_sqrd = First_Eccent_Sqrd;

#endif
