/*---------------------------------------------------------------------------*/
/* File: gmsScreenCoordUtils.h
/* Date: 21-Sep-99 : initial definition
/*       14-Oct-99 : operate only in screen coordinates
/*       22-Nov-99 : distinguish flat-earth zoom vs globe zoom
/*       09-Dec-99 : rename file
/*       27-Jan-00 : remove zoom-factor as state, use 'gmsMapStateMgr'
/*
/* Description:
/*    Utilities to convert between real-world coordinates and screen
/*    coordinates.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/
#ifndef GMS_SCREEN_COORD_UTILS

   #define GMS_SCREEN_COORD_UTILS

   #include <gmsTypesAndConstants.h>
   #include <gmsUtilities.h>


   /*-----------------------------------------------------*/
   /* gmsConvertWorldCoordToScreenCoord
   /*
   /* Description:
   /*    This utility converts a given (x, y, z) real-world
   /*    coordinate (in meters) to an (x, y, z) screen
   /*    coordinate (in pixels).
   /*-----------------------------------------------------*/
   gms_3D_ScreenPointType gmsConvertWorldCoordToScreenCoord
                             (gms_3D_WorldPointType theCoord);


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
               gms_3D_ScreenPolylineType *screenPolylineOut);


   /*-----------------------------------------------------*/
   /* gmsConvertFlatWorldCoordToScreenCoord
   /*
   /* Description:
   /*    This utility converts a given (x, y) real-world
   /*    coordinate (in meters) to an (x, y,) screen
   /*    coordinate (in pixels).
   /*-----------------------------------------------------*/
   gms_2D_ScreenPointType gmsConvertFlatWorldCoordToScreenCoord
                             (gms_2D_WorldPointType theCoord);


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
                              bool                   &isValid);


   /*-----------------------------------------------------*/
   /* gmsConvert_2D_WorldPointTo_2D_ScreenPoint
   /*
   /* Description:
   /*    This utility converts a given 2D World point to a
   /*    2D screen coordinate.  This utility is used in
   /*    support of the "flat earth" model.
   /*-----------------------------------------------------*/
   gms_2D_ScreenPointType gmsConvert_2D_WorldPointTo_2D_ScreenPoint
                             (gms_2D_WorldPointType  worldPoint_2D);


   /*-----------------------------------------------------*/
   /* gmsCalculateFlatMapTranslations
   /*
   /* Description:
   /*-----------------------------------------------------*/
   gms_2D_ScreenPointType gmsCalculateFlatMapTranslations ();


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
                gms_2D_ScreenPointType point_2);

#endif
