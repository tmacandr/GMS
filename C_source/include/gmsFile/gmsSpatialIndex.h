//----------------------------------------------------------------------------*/
// File : gmsSpatialIndex.h
// Date : 07-Aug-99 : initial definition
//        07-Oct-99 : Clean-up due to code-inspection
//
// Description:
//    Utilities to "read" any "Spatial Index" file of the Digital Chart of
//    the World (DCW).
//
//    A "spatial index" file provides the capability to use a set of coord-
//    inates to identify a (geographic) primitive and its associated attributes.
//    The idea is to provide a mechanism that will reduce the search-time to
//    find geographic primitives within the DCW database.  Spatial indexes are
//    implemented for FACE, EDGE, NODE, and TEXT primitives and have the
//    following file names:
//
//               FSI - Face Spatial Index
//               ESI - Edge Spatial Index
//               NSI - Node Spatial Index
//               TSI - Text Spatial Index
//
//    Also, consider:
//
//       Spatial Index:
//       --------------
//          A data structure fiel that allows for the rapid
//          identification of a primitive by using the values
//          of the primitive's coordinates.
//
//    The "spatial index" is one of the four categories of spatial decomposition
//    of a VPF database.  The other three are: 1) tile directory, 2) the
//    minimum-bounding-rectangle (of edge/face primitives), and 3) primitive
//    coordinates.
//
//    See also, Apendix F, Mil-Std-600006, pg 139-150.
//
//    Reference:
//        1) Mil-Std-600006
//        2) Mil-D-89009
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#ifndef SI

   #define SI

   typedef struct _nodeRecType
      {
       long int offsetToNode;
       long int numItemsInNode;
      } nodeRecType;

   typedef struct _boundingRectangleType
      {
       float x1_long;
       float y1_lat;
       float x2_long;
       float y2_lat;
      } boundingRectangleType;

   typedef struct _primitiveRecType
      {
       boundingRectangleType spatialCell;
       int                   primId;
      } primitiveRecType;

   typedef struct _spatialIndexType
      {
       boundingRectangleType outterRectangle;
       int                   numNodes;
       nodeRecType           *nodeRecords;
       int                   numPrimitives;
       primitiveRecType      *primitiveRecords;
      } spatialIndexType;


   //---------------------------------------------*/
   // gmsGetSpatialIndex
   //
   // Description:
   //    This utility reads the file that contains
   //    the "Spatial Index".  A pointer to a newly
   //    allocated table is returned to the caller.
   //    It is the caller's responsibility to free
   //    the item by using the utility
   //    "gmsFreeSpatialIndex" (see below).
   //---------------------------------------------*/
   spatialIndexType *gmsGetSpatialIndex
                           (const char *spatialIndexFilePath);


   //---------------------------------------------*/
   // gmsFreeSpatialIndex
   //
   // Description:
   //    This utility frees a "Spatial Index" that had
   //    been previously allocated using
   //    "gmsGetSpatialIndex".
   //---------------------------------------------*/
   void gmsFreeSpatialIndex
           (spatialIndexType *theSpatialIndex);


   //---------------------------------------------*/
   // gmsPrintSpatialIndex
   //
   // Description:
   //    This function will print the "Spatial Index"
   //    object to standard out.
   //---------------------------------------------*/
   void gmsPrintSpatialIndex
           (spatialIndexType *theSpatialIndex);

#endif
