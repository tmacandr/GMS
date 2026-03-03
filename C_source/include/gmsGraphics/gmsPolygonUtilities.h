//-----------------------------------------------------------------------------
// File : gmsPolygonUtilities.h
// Date : 20-Jan-00 : initial definition
//        04-Feb-00 : add util for DN areas
//
// Description:
//    This component defines a series of utilities to build polygons from
//    DCW edge tables.  Edges can be organized to define polygons that
//    represent closed geographic regions such as countries, islands, and
//    bodies of water.  The polygons can be used by an application/client
//    to define "fill-areas" to graphically enhance the image of a map.
//
//    See also "Winged-Edge Topology". 
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_POLYGON_UTILS

   #define GMS_POLYGON_UTILS

   #include <gmsTypesAndConstants.h>
   #include <gmsEdgeTable.h>
   #include <gmsRingTable.h>
   #include <gmsFaceTable.h>
   #include <gmsFeatureTable.h>


   typedef struct
      {
       int faceId;
       int numNodes;
       int *edgRecordIds;
      } gmsPolygonIdListType;

   typedef struct
      {
       int                  numPolygons;
       gmsPolygonIdListType *polygonIdList;
      } gmsPolygonInfoType;


   /*-----------------------------------------------------*/
   /* gmsGet_PO_PolygonInfoFromEdgeTable
   /*
   /* Description:
   /*    This utility builds a record that indicates the
   /*    polygons (closed geographic areas) in the specified
   /*    edge table.  The edge table must be from the
   /*    "Political/Oceans" (PO) theme.  The kind of polygon
   /*    is indicated by the parameter "featureId".  For
   /*    example, "land areas" are indicated by 1, and
   /*    "ocean areas" are indicated by 2.  The "ring table",
   /*    "face table", and "feature table" are used to
   /*    determine which polygons (from the edge table) are
   /*    of interest.
   /*    
   /*    The caller must 'deallocate' the data structure
   /*    when finished with it.  The utility
   /*    "gmsFreePolygonInfo" is provided for this purpose
   /*    (see below).
   /*-----------------------------------------------------*/
   gmsPolygonInfoType gmsGet_PO_PolygonInfoFromEdgeTable
                         (edgeTableType    *edgTbl,
                          ringTableType    *rngTbl,
                          faceTableType    *facTbl,
                          featureTableType *areaFeatureTbl,
                          int              featureId);


   /*-----------------------------------------------------*/
   /* gmsGet_DN_PolygonInfoFromEdgeTable
   /*
   /* Description:
   /*    This utility builds a record that indicates the
   /*    polygons (closed geographic areas) in the specified
   /*    edge table.  The edge table must be from the
   /*    DRAINAGE (DN) theme.  The "ring table" is used to
   /*    determine the polygons defined in the edge table.
   /*
   /*    The caller must 'deallocate' the data structure
   /*    when finished with it.  The utility
   /*    "gmsFreePolygonInfo" is provided for this purpose
   /*    (see below).
   /*-----------------------------------------------------*/
   gmsPolygonInfoType gmsGet_DN_PolygonInfoFromEdgeTable
                         (edgeTableType *edgTbl,
                          ringTableType *rngTbl);


   /*-----------------------------------------------------*/
   /* gmsFreePolygonInfo
   /*
   /* Description:
   /*    This routine 'deallocates' the specified data
   /*    structure of polygon info that was allocated
   /*    with the routine "gmsGetPolygonInfoFromEdgeTable"
   /*    (see above).
   /*-----------------------------------------------------*/
   void gmsFreePolygonInfo
           (gmsPolygonInfoType *polygonInfo);


   /*-----------------------------------------------------*/
   /* gmsPrintPolygonInfo
   /*
   /* Description:
   /*    This routine prints the specified polygon info
   /*    to stdout.  It is for debug purposes.
   /*-----------------------------------------------------*/
   void gmsPrintPolygonInfo
           (gmsPolygonInfoType polygonInfo);


   /*-----------------------------------------------------*/
   /* gmsGetNumPolygonsInEdgeTable
   /*
   /* Description:
   /*    This function examines the given edge table and
   /*    counts the number of polygons "contained" in
   /*    that table.
   /*-----------------------------------------------------*/
   int gmsGetNumPolygonsInEdgeTable
           (edgeTableType *edgTbl);


   /*-----------------------------------------------------*/
   /* gmsBuildWorldPolygonsFromEdgeTable
   /*
   /* Description:
   /*    This function uses the given edge table to build
   /*    one or more polygons in 3D world coordinates.
   /*    This function will allocate a structure that contains
   /*    one or more 3D polylines.  Each polyline represents
   /*    one polygon.
   /*
   /*    Note:
   /*    -----
   /*       It is the caller's responsibility to
   /*       'free' the polygon data structure
   /*       when finished using it.  See the utility
   /*       'gmsFree_3D_WorldImage' defined in the
   /*       component 'gmsDcwUtilities.h".
   /*-----------------------------------------------------*/
   gms_3D_WorldImageType gmsBuildWorldPolygonsFromEdgeTable
                               (gmsPolygonInfoType polygonInfo,
                                edgeTableType      *edgTbl);


   /*-----------------------------------------------------*/
   /* gmsBuildFlatWorldPolygonsFromEdgeTable
   /*
   /* Description:
   /*    This function uses the given edge table to build
   /*    one or more polygons in 2D world coordinates using
   /*    the "flat-earth" model.  This function will allocate
   /*    a structure that contains one or more 2D polylines.
   /*    Each polyline represents one polygon.
   /*
   /*    Note:
   /*    -----
   /*       It is the caller's responsibility to
   /*       'free' the polygon data structure
   /*       when finished using it.  See the utility
   /*       'gmsFree_2D_WorldImage' defined in the
   /*       component 'gmsDcwUtilities.h".
   /*-----------------------------------------------------*/
   gms_2D_WorldImageType gmsBuildFlatWorldPolygonsFromEdgeTable
                               (gmsPolygonInfoType polygonInfo,
                                edgeTableType      *edgTbl);


   /*-----------------------------------------------------*/
   /* gmsAllocateScreenPolygonsFromEdgeTable
   /*
   /* Description:
   /*    This function uses the given edge table to allocate
   /*    a corresponding data structure that will contain
   /*    one or more polygons in 2D screen coordinates.  Only
   /*    space is allocated, no image data is constructed.
   /*    Each polyline represents one polygon.
   /*
   /*    Note:
   /*    -----
   /*       It is the caller's responsibility to
   /*       'free' the polygon data structure
   /*       when finished using it.  See the utility
   /*       'gmsFree_2D_ScreenImage' defined in the
   /*       component 'gmsDcwUtilities.h".
   /*-----------------------------------------------------*/
   gms_2D_ScreenImageType gmsAllocateScreenPolygonsFromEdgeTable
                                (gmsPolygonInfoType polygonInfo,
                                 edgeTableType      *edgTbl);


   /*-----------------------------------------------------*/
   /* gmsBuildScreenImage
   /*
   /* Description:
   /*    This utility builds an image of 2D screen polygons
   /*    from the given image of 3D world-coord polygons.
   /*    The source image was constructed using the "ellipsoid"
   /*    earth-model.  No "allocation" is performed.  The
   /*    target image (struct) must already have been
   /*    allocated to accomodate the image.
   /*-----------------------------------------------------*/
   void gmsBuildScreenImage
               (gms_3D_WorldImageType  fromWorldImage,
                gms_2D_ScreenImageType &toScreenImage);


   /*-----------------------------------------------------*/
   /* gmsBuildFlatScreenImage
   /*
   /* Description:
   /*    This utility builds an image of 2D screen polygons
   /*    from the given image of 2D flat-world-coord polygons.
   /*    The source image was constructed using the "flat"
   /*    earth-model.  No "allocation" is performed.  The
   /*    target image (struct) must already have been
   /*    allocated to accomodate the image.
   /*-----------------------------------------------------*/
   void gmsBuildFlatScreenImage
               (gms_2D_WorldImageType  fromFlatWorldImage,
                gms_2D_ScreenImageType &toScreenImage);


#endif
