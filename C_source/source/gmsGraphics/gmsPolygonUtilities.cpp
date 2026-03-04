//-----------------------------------------------------------------------------
// File : gmsPolygonUtilities.cpp
// Date : 20-Jan-00 : initial definition
//        31-Jan-00 : build polygon info based on area feature
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

#include <stdio.h>
#include <stdlib.h>

#include <gmsPolygonUtilities.h>
#include <gmsTypesAndConstants.h>
#include <gmsScreenCoordUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define other types
//---------------------------------


//---------------------------------
// Declare some constants
//---------------------------------

static const gmsPolygonInfoType    Null_Polygon_Info = {0, NULL};

static const gms_3D_WorldImageType Null_3D_Polygons = {0, NULL};

static const gms_2D_WorldImageType Null_2D_Polygons = {0, NULL};


//---------------------------------
// Declare Local Functions
//---------------------------------

static int getNumEdgeRecordsInPolygon
                (const int           startId,
                 const int           faceId,
                 const edgeTableType *edgTbl);

static void build_PO_PolygonInfoObject
               (const edgeTableType *edgTbl,
                const ringTableType *rngTbl,
                const int           numPolygons,
                const bool          *rngTblFlags,
                gmsPolygonInfoType  &polygonInfo_out);

static void buildGeneralPolygonInfoObject
               (const edgeTableType *edgTbl,
                const ringTableType *rngTbl,
                gmsPolygonInfoType  &polygonInfo_out);

static void buildPolygonIdList
              (const int            startId,
               const int            faceId,
               const edgeTableType  *edgTbl,
               gmsPolygonIdListType *polygonList);

static gms_3D_WorldImageType allocateWorldPolygonsFromEdgeTable
                                     (gmsPolygonInfoType polygonInfo,
                                      edgeTableType      *edgTbl);

static gms_2D_WorldImageType allocateFlatWorldPolygonsFromEdgeTable
                                     (gmsPolygonInfoType polygonInfo,
                                      edgeTableType      *edgTbl);

static void putCoordinatesIntoPolygon
               (edgeRecordType           fromEdgRec,
                int                      faceId,
                int                      &pointIndex,
                gms_3D_WorldPolylineType &toPolyline);

static void putFlatCoordinatesIntoPolygon
               (edgeRecordType           fromEdgRec,
                int                      faceId,
                int                      &pointIndex,
                gms_2D_WorldPolylineType &toPolyline);


//-----------------------------------------------------*/
// gmsGet_PO_PolygonInfoFromEdgeTable
//
// Description:
//    This utility builds a record that indicates the
//    polygons (closed geographic areas) in the specified
//    edge table.  The edge table must be from the
//    "Political/Oceans" (PO) theme.  The kind of polygon
//    is indicated by the parameter "featureId".  For
//    example, "land areas" are indicated by 1, and
//    "ocean areas" are indicated by 2.  The "ring table",
//    "face table", and "feature table" are used to
//    determine which polygons (from the edge table) are
//    of interest.
//    
//    The caller must 'deallocate' the data structure
//    when finished with it.  The utility
//    "gmsFreePolygonInfo" is provided for this purpose
//    (see below).
//-----------------------------------------------------*/
gmsPolygonInfoType gmsGet_PO_PolygonInfoFromEdgeTable
                         (edgeTableType    *edgTbl,
                          ringTableType    *rngTbl,
                          faceTableType    *facTbl,
                          featureTableType *areaFeatureTbl,
                          int              featureId)

{
         int                   i;
         int                   numPolygons;
         int                   facFgnKey, aftFgnKey;
         int                   tempFeatureId;
         PO_areaFeatureRecType *ptrTo_PO_AFT;
         gmsPolygonInfoType    polygonInfo = {0, NULL};
         bool                  *rngTblFlags;
         const int Max_Face_Records    = facTbl->numRecords;

   if ( (edgTbl == NULL) || (rngTbl == NULL) ||
        (facTbl == NULL) || (areaFeatureTbl == NULL) ) 
      {
       gmsWriteDebug ("*** ERROR : gmsGet_PO_PolygonInfoFromEdgeTable\n");

       return Null_Polygon_Info;
      }

   numPolygons = 0;

   ptrTo_PO_AFT = (PO_areaFeatureRecType *) areaFeatureTbl->featureRecords;

   rngTblFlags = new bool[rngTbl->numRecords];

   for (i = 0; i < rngTbl->numRecords; i++)
      rngTblFlags[i] = false;

   for (i = 0; i < rngTbl->numRecords; i++)
      {
       facFgnKey = rngTbl->ringRecords[i].faceId;

       if (facFgnKey > Max_Face_Records)
          {
           gmsWriteDebug
              ("gmsGet_PO_PolygonInfoFromEdgeTable - bad FACE index\n");

           return polygonInfo;
          }

       facFgnKey--;

       aftFgnKey = facTbl->faceRecords[facFgnKey].fgnKeyToFeatureTbl;

       if (facFgnKey > Max_Face_Records)
          {
           gmsWriteDebug
              ("gmsGet_PO_PolygonInfoFromEdgeTable - bad AFT index\n");

           return polygonInfo;
          }

       aftFgnKey--;

       tempFeatureId = ptrTo_PO_AFT[aftFgnKey].featureType;

       if (tempFeatureId == featureId)
          {
           numPolygons++;

           rngTblFlags[i] = true;
          }
      }

   if (numPolygons == 0)
      {
       delete [] rngTblFlags;

       return Null_Polygon_Info;
      }

   build_PO_PolygonInfoObject
      (edgTbl,
       rngTbl,
       numPolygons,
       rngTblFlags,
       polygonInfo);

   delete [] rngTblFlags;

   return polygonInfo;
}


//-----------------------------------------------------*/
// gmsGet_DN_PolygonInfoFromEdgeTable
//
// Description:
//    This utility builds a record that indicates the
//    polygons (closed geographic areas) in the specified
//    edge table.  The edge table must be from the
//    DRAINAGE (DN) theme.  The "ring table" is used to
//    determine the polygons defined in the edge table.
//
//    The caller must 'deallocate' the data structure
//    when finished with it.  The utility
//    "gmsFreePolygonInfo" is provided for this purpose
//    (see below).
//-----------------------------------------------------*/
gmsPolygonInfoType gmsGet_DN_PolygonInfoFromEdgeTable
                         (edgeTableType *edgTbl,
                          ringTableType *rngTbl)

{
         gmsPolygonInfoType polygonInfo = {0, NULL};

   if ( (edgTbl == NULL) || (rngTbl == NULL) ) 
      {
       gmsWriteDebug
         ("*** ERR : gmsGetGeneralPolygonInfoFromEdgeTable - no client data\n");

       return Null_Polygon_Info;
      }

   buildGeneralPolygonInfoObject
      (edgTbl,
       rngTbl,
       polygonInfo);

   return polygonInfo;
}


//-----------------------------------------------------*/
// gmsFreePolygonInfo
//
// Description:
//    This routine 'deallocates' the specified data
//    structure of polygon info that was allocated
//    with the routine "gmsGetPolygonInfoFromEdgeTable"
//    (see above).
//-----------------------------------------------------*/
void gmsFreePolygonInfo
           (gmsPolygonInfoType *polygonInfo)

{
   if (polygonInfo == NULL)
      return;

   if (polygonInfo->polygonIdList != NULL)
      {
       if (polygonInfo->polygonIdList->edgRecordIds != NULL)
          delete [] polygonInfo->polygonIdList->edgRecordIds;

       delete [] polygonInfo->polygonIdList;
      }

   polygonInfo->polygonIdList = NULL;

   polygonInfo->numPolygons = 0;
}


//-----------------------------------------------------*/
// gmsPrintPolygonInfo
//
// Description:
//    This routine prints the specified polygon info
//    to stdout.  It is for debug purposes.
//-----------------------------------------------------*/
void gmsPrintPolygonInfo
           (gmsPolygonInfoType polygonInfo)

{
         int i, j, lineCount;

   printf("_________________________\n");
   printf("Polygons from edge table:\n");

   printf("Number of Polygons (faces) : %d\n",
          polygonInfo.numPolygons);

   for (i = 0; i < polygonInfo.numPolygons; i++)
      {
       printf("   Edge Records of Polygon (face) %d:\n", i);

       printf("      ---> Face ID   : %d\n",
              polygonInfo.polygonIdList[i].faceId);

       printf("      ---> Num Nodes : %d\n",
              polygonInfo.polygonIdList[i].numNodes);

       printf("      ---> Nodes     : ");

       lineCount = 0;

       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
          {
           printf("%d - ",
                  polygonInfo.polygonIdList[i].edgRecordIds[j]);

           lineCount++;

           if (lineCount == 9)
              {
               lineCount = 0;

               printf("\n                       ");
              }
          }

       printf("\n___________\n");
      }

   printf("_________________________\n");
}


//-----------------------------------------------------*/
// gmsGetNumPolygonsInEdgeTable
//
// Description:
//    This function examines the given edge table and
//    counts the number of polygons "contained" in
//    that table.
//
//    Complexity Analysis:
//    --------------------
//
//          t(n) = O(2 * n)
//
//          t(n) = O(n)
//
//       where n = number of edge records
//-----------------------------------------------------*/
int gmsGetNumPolygonsInEdgeTable
           (edgeTableType *edgTbl)

{
      int i;
      int numPolygons = 0;

   for (i = 0; i < edgTbl->numRecords; i++)
      {
       if (edgTbl->edgeRecords[i].headerInfo.rightFace.id > numPolygons)
          numPolygons = edgTbl->edgeRecords[i].headerInfo.rightFace.id;
      }

   for (i = 0; i < edgTbl->numRecords; i++)
      {
       if (edgTbl->edgeRecords[i].headerInfo.leftFace.id > numPolygons)
          numPolygons = edgTbl->edgeRecords[i].headerInfo.leftFace.id;
      }

   return numPolygons;
}


//-----------------------------------------------------*/
// gmsBuildWorldPolygonsFromEdgeTable
//
// Description:
//    This function uses the given edge table to build
//    one or more polygons in 3D world coordinates.
//    This function will allocate a structure that contains
//    one or more 3D polylines.  Each polyline represents
//    one polygon.
//
//    Note:
//    -----
//       It is the caller's responsibility to
//       'free' the polygon data structure
//       when finished using it.  See the utility
//       'gmsFree_3D_WorldImage' defined in the
//       component 'gmsDcwUtilities.h".
//-----------------------------------------------------*/
gms_3D_WorldImageType gmsBuildWorldPolygonsFromEdgeTable
                               (gmsPolygonInfoType polygonInfo,
                                edgeTableType      *edgTbl)

{
         int                   i, j;
         int                   edgIndex;
         int                   pointIndex;
         gms_3D_WorldImageType newPolygons = Null_3D_Polygons;

   newPolygons = allocateWorldPolygonsFromEdgeTable
                    (polygonInfo,
                     edgTbl);

   if (newPolygons.numLines == 0)
      return Null_3D_Polygons;

   for (i = 0; i < polygonInfo.numPolygons; i++)
      {
       pointIndex = 0;

       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
          {
           edgIndex = polygonInfo.polygonIdList[i].edgRecordIds[j];

           edgIndex--;

           putCoordinatesIntoPolygon
               (edgTbl->edgeRecords[edgIndex],
                polygonInfo.polygonIdList[i].faceId,
                pointIndex,
                newPolygons.imageLines[i]);
          }
      }

   return newPolygons;
}


//-----------------------------------------------------*/
// gmsBuildFlatWorldPolygonsFromEdgeTable
//
// Description:
//    This function uses the given edge table to build
//    one or more polygons in 2D world coordinates using
//    the "flat-earth" model.  This function will allocate
//    a structure that contains one or more 2D polylines.
//    Each polyline represents one polygon.
//
//    Note:
//    -----
//       It is the caller's responsibility to
//       'free' the polygon data structure
//       when finished using it.  See the utility
//       'gmsFree_2D_WorldImage' defined in the
//       component 'gmsDcwUtilities.h".
//-----------------------------------------------------*/
gms_2D_WorldImageType gmsBuildFlatWorldPolygonsFromEdgeTable
                               (gmsPolygonInfoType polygonInfo,
                                edgeTableType      *edgTbl)

{
         int                   i, j;
         int                   edgIndex;
         int                   pointIndex;
         gms_2D_WorldImageType new_2D_Polygons = Null_2D_Polygons;

   new_2D_Polygons = allocateFlatWorldPolygonsFromEdgeTable
                       (polygonInfo,
                        edgTbl);

   if (new_2D_Polygons.numLines == 0)
      return Null_2D_Polygons;

   for (i = 0; i < polygonInfo.numPolygons; i++)
      {
       pointIndex = 0;

       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
          {
           edgIndex = polygonInfo.polygonIdList[i].edgRecordIds[j];

           edgIndex--;

           putFlatCoordinatesIntoPolygon
              (edgTbl->edgeRecords[edgIndex],
               polygonInfo.polygonIdList[i].faceId,
               pointIndex,
               new_2D_Polygons.imageLines[i]);
          }
      }

   return new_2D_Polygons;
}


//-----------------------------------------------------*/
// gmsAllocateScreenPolygonsFromEdgeTable
//
// Description:
//    This function uses the given edge table to allocate
//    a corresponding data structure that will contain
//    one or more polygons in 2D screen coordinates.  Only
//    space is allocated, no image data is constructed.
//    Each polyline represents one polygon.
//
//    Note:
//    -----
//       It is the caller's responsibility to
//       'free' the polygon data structure
//       when finished using it.  See the utility
//       'gmsFree_2D_ScreenImage' defined in the
//       component 'gmsDcwUtilities.h".
//-----------------------------------------------------*/
gms_2D_ScreenImageType gmsAllocateScreenPolygonsFromEdgeTable
                                (gmsPolygonInfoType polygonInfo,
                                 edgeTableType      *edgTbl)

{
         int                     i, j;
         int                     totalNumPoints = 0;
         int                     index;
         int                     edgIndex;
         gms_2D_ScreenPointType  *arrayOfPoints;
         gms_2D_ScreenImageType  new_2D_ScreenPolygons = {0, NULL};

   if ( (edgTbl == NULL) ||
        (polygonInfo.numPolygons == 0) ||
        (polygonInfo.polygonIdList == NULL) )
      return new_2D_ScreenPolygons;

   new_2D_ScreenPolygons.numLines = polygonInfo.numPolygons;

   new_2D_ScreenPolygons.imageLines =
      new gms_2D_ScreenPolylineType[polygonInfo.numPolygons];

   if (new_2D_ScreenPolygons.imageLines == NULL)
      {
       printf("*** ERROR : alloc of 2D screen polygons failed\n");

       return new_2D_ScreenPolygons;
      }

   for (i = 0; i < polygonInfo.numPolygons; i++)
       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
         {
          index = polygonInfo.polygonIdList[i].edgRecordIds[j];

          index--; // index into a C array

          totalNumPoints = totalNumPoints +
                           edgTbl->edgeRecords[index].numCoords;
         }

   arrayOfPoints = new gms_2D_ScreenPointType[totalNumPoints];

   if (arrayOfPoints == NULL)
      {
       printf("*** ERROR : alloc of 2D screen points failed\n");

       return new_2D_ScreenPolygons;
      }

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < polygonInfo.numPolygons; i++)
      {
       totalNumPoints = 0;

       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
          {
           edgIndex = polygonInfo.polygonIdList[i].edgRecordIds[j];

           edgIndex--;

           totalNumPoints = totalNumPoints +
                            edgTbl->edgeRecords[edgIndex].numCoords;
          }

       new_2D_ScreenPolygons.imageLines[i].numPoints = totalNumPoints;

       new_2D_ScreenPolygons.imageLines[i].points = &arrayOfPoints[index];

       index = index + totalNumPoints;
      }

   return new_2D_ScreenPolygons;
}


//-----------------------------------------------------*/
// gmsBuildScreenImage
//
// Description:
//    This utility builds an image of 2D screen polygons
//    from the given image of 3D world-coord polygons.
//    The source image was constructed using the "ellipsoid"
//    earth-model.  No "allocation" is performed.  The
//    target image (struct) must already have been
//    allocated to accomodate the image.
//-----------------------------------------------------*/
void gmsBuildScreenImage
               (gms_3D_WorldImageType  fromWorldImage,
                gms_2D_ScreenImageType &toScreenImage)

{
         gms_2D_ScreenPointType screenPnt_2D;
         gms_3D_WorldPointType  worldPnt_3D;
         int                    numPoints;
         int                    i, j;
         bool                   isValid;

   if (fromWorldImage.numLines == 0)
      {
       gmsWriteDebug
          ("*** ERR - buildScreenImage : no lines in image\n");

       toScreenImage.numLines = 0;

       return;
      }

   for (i = 0; i < fromWorldImage.numLines; i++)
      {
       numPoints = 0;

       for (j = 0; j < fromWorldImage.imageLines[i].numPoints; j++)
          {
           worldPnt_3D = fromWorldImage.imageLines[i].points[j];

           screenPnt_2D = gmsConvert_3D_WorldPointTo_2D_ScreenPoint
                             (worldPnt_3D,
                              isValid);

           if (isValid)
              {
               toScreenImage.imageLines[i].points[numPoints] = screenPnt_2D;

               numPoints++;
              }
          }

       toScreenImage.imageLines[i].numPoints = numPoints;
      }
}


//-----------------------------------------------------*/
// gmsBuildFlatScreenImage
//
// Description:
//    This utility builds an image of 2D screen polygons
//    from the given image of 2D flat-world-coord polygons.
//    The source image was constructed using the "flat"
//    earth-model.  No "allocation" is performed.  The
//    target image (struct) must already have been
//    allocated to accomodate the image.
//-----------------------------------------------------*/
void gmsBuildFlatScreenImage
               (gms_2D_WorldImageType  fromFlatWorldImage,
                gms_2D_ScreenImageType &toScreenImage)

{
         gms_2D_WorldPointType  worldPoint;
         gms_2D_ScreenPointType screenPoint;
         int                    i, j;
         gmsMapStateStructType  mapState;
         gms_2D_ScreenPointType translations;

   if (fromFlatWorldImage.numLines == 0)
      {
       gmsWriteDebug
          ("*** ERR - buildFlatScreenImage : no lines in image\n");

       toScreenImage.numLines = 0;

       return;
      }

   mapState = gmsGetMapState ();

   translations = gmsCalculateFlatMapTranslations ();

   for (i = 0; i < fromFlatWorldImage.numLines; i++)
      {
       toScreenImage.imageLines[i].numPoints =
          fromFlatWorldImage.imageLines[i].numPoints;

       for (j = 0; j < fromFlatWorldImage.imageLines[i].numPoints; j++)
          {
           worldPoint = fromFlatWorldImage.imageLines[i].points[j];

           screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

           screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

           toScreenImage.imageLines[i].points[j] = screenPoint;
          }
      }
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------------*/
// getNumEdgeRecordsInPolygon
//
// Description:
//-----------------------------------------------------*/
static int getNumEdgeRecordsInPolygon
                (const int           startId,
                 const int           faceId,
                 const edgeTableType *edgTbl)

{
         int numEdgRecords = 0;
         int nextId;

   if (edgTbl == NULL)
      {
       gmsWriteDebug ("*** ERR : getNumEdgeRecordsInPolygon : null edg tbl\n");

       return 0;
      }

   nextId = startId;

   do
      {
       if (edgTbl->edgeRecords[nextId - 1].headerInfo.rightFace.id ==
           edgTbl->edgeRecords[nextId - 1].headerInfo.leftFace.id)
          {
           gmsWriteDebug("*** ERR : getNumEdgeRecordsIn - invalid edg tbl\n");

           return 0;
          }

       if (edgTbl->edgeRecords[nextId - 1].headerInfo.rightFace.id == faceId)

          nextId = edgTbl->edgeRecords[nextId - 1].headerInfo.rightEdge.id;

       else if (edgTbl->edgeRecords[nextId - 1].headerInfo.leftFace.id == faceId)

          nextId = edgTbl->edgeRecords[nextId - 1].headerInfo.leftEdge.id;

       else
          {
           gmsWriteDebug("*** ERR : getNumEdgeRecordsInPolygon - invalid\n");

           break;
          }

       numEdgRecords++;
      }
   while (nextId != startId);

   return numEdgRecords;
}


//-----------------------------------------------------*/
// buildPolygonIdList
//
// Description:
//-----------------------------------------------------*/
static void buildPolygonIdList
              (const int            startId,
               const int            faceId,
               const edgeTableType  *edgTbl,
               gmsPolygonIdListType *polygonList)

{
         int index = 0;
         int nextId;

   if ( (edgTbl == NULL) || (polygonList == NULL) )
      {
       gmsWriteDebug ("*** ERR : buildPolygonIdList : null components\n");

       return;
      }

   if (polygonList->numNodes == 0)
      return;

   nextId = startId;

   do
      {
       polygonList->edgRecordIds[index] = nextId;

       if (edgTbl->edgeRecords[nextId - 1].headerInfo.rightFace.id ==
           edgTbl->edgeRecords[nextId - 1].headerInfo.leftFace.id)
          {
           gmsWriteDebug("*** ERR : buildPolygonIdList - invalid edg tbl\n");

           polygonList->numNodes = 0;

           polygonList->faceId = -1;

           return;
          }

       if (edgTbl->edgeRecords[nextId - 1].headerInfo.rightFace.id == faceId)

          nextId = edgTbl->edgeRecords[nextId - 1].headerInfo.rightEdge.id;

       else if (edgTbl->edgeRecords[nextId - 1].headerInfo.leftFace.id == faceId)

          nextId = edgTbl->edgeRecords[nextId - 1].headerInfo.leftEdge.id;

       else
          {
           gmsWriteDebug("*** ERR : buildPolygonIdList - invalid\n");

           break;
          }

       index++;
      }
   while (nextId != startId);
}


//-----------------------------------------------------*/
// allocateWorldPolygonsFromEdgeTable
//
// Description:
//-----------------------------------------------------*/
static gms_3D_WorldImageType allocateWorldPolygonsFromEdgeTable
                                     (gmsPolygonInfoType polygonInfo,
                                      edgeTableType      *edgTbl)

{
         int                   i, j;
         int                   totalNumPoints = 0;
         int                   index;
         int                   edgIndex;
         gms_3D_WorldPointType *arrayOfPoints;
         gms_3D_WorldImageType newPolygons = Null_3D_Polygons;

   if ( (edgTbl == NULL) ||
        (polygonInfo.numPolygons == 0) ||
        (polygonInfo.polygonIdList == NULL) )
      return Null_3D_Polygons;

   newPolygons.numLines = polygonInfo.numPolygons;

   newPolygons.imageLines =
      new gms_3D_WorldPolylineType[polygonInfo.numPolygons];

   if (newPolygons.imageLines == NULL)
      {
       printf("*** ERROR : alloc of 3D world polygons failed\n");

       return Null_3D_Polygons;
      }

   for (i = 0; i < polygonInfo.numPolygons; i++)
       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
         {
          index = polygonInfo.polygonIdList[i].edgRecordIds[j];

          index--; // index into a C array

          totalNumPoints = totalNumPoints +
                           edgTbl->edgeRecords[index].numCoords;
         }

   arrayOfPoints = new gms_3D_WorldPointType[totalNumPoints];

   if (arrayOfPoints == NULL)
      {
       printf("*** ERROR : alloc of 3D world points failed\n");

       return Null_3D_Polygons;
      }

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_3D_WorldPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < polygonInfo.numPolygons; i++)
      {
       totalNumPoints = 0;

       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
          {
           edgIndex = polygonInfo.polygonIdList[i].edgRecordIds[j];

           edgIndex--;

           totalNumPoints = totalNumPoints +
                            edgTbl->edgeRecords[edgIndex].numCoords;
          }

       newPolygons.imageLines[i].numPoints = totalNumPoints;

       newPolygons.imageLines[i].points = &arrayOfPoints[index];

       index = index + totalNumPoints;
      }

   return newPolygons;
}


//-----------------------------------------------------*/
// allocateFlatWorldPolygonsFromEdgeTable
//
// Description:
//-----------------------------------------------------*/
static gms_2D_WorldImageType allocateFlatWorldPolygonsFromEdgeTable
                                     (gmsPolygonInfoType polygonInfo,
                                      edgeTableType      *edgTbl)

{
         int                   i, j;
         int                   totalNumPoints = 0;
         int                   index;
         int                   edgIndex;
         gms_2D_WorldPointType *arrayOfPoints;
         gms_2D_WorldImageType newFlatPolygons = Null_2D_Polygons;

   if ( (edgTbl == NULL) ||
        (polygonInfo.numPolygons == 0) ||
        (polygonInfo.polygonIdList == NULL) )
      return Null_2D_Polygons;

   newFlatPolygons.numLines = polygonInfo.numPolygons;

   newFlatPolygons.imageLines =
      new gms_2D_WorldPolylineType[polygonInfo.numPolygons];

   if (newFlatPolygons.imageLines == NULL)
      {
       printf("*** ERROR : alloc of 2D world polygons failed\n");

       return Null_2D_Polygons;
      }

   for (i = 0; i < polygonInfo.numPolygons; i++)
       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
         {
          index = polygonInfo.polygonIdList[i].edgRecordIds[j];

          index--; // index into a C array

          totalNumPoints = totalNumPoints +
                           edgTbl->edgeRecords[index].numCoords;
         }

   arrayOfPoints = new gms_2D_WorldPointType[totalNumPoints];

   if (arrayOfPoints == NULL)
      {
       printf("*** ERROR : alloc of 2D world points failed\n");

       return Null_2D_Polygons;
      }

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_WorldPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < polygonInfo.numPolygons; i++)
      {
       totalNumPoints = 0;

       for (j = 0; j < polygonInfo.polygonIdList[i].numNodes; j++)
          {
           edgIndex = polygonInfo.polygonIdList[i].edgRecordIds[j];

           edgIndex--;

           totalNumPoints = totalNumPoints +
                            edgTbl->edgeRecords[edgIndex].numCoords;
          }

       newFlatPolygons.imageLines[i].numPoints = totalNumPoints;

       newFlatPolygons.imageLines[i].points = &arrayOfPoints[index];

       index = index + totalNumPoints;
      }

   return newFlatPolygons;
}


//-----------------------------------------------------*/
// putCoordinatesIntoPolygon
//
// Description:
//-----------------------------------------------------*/
static void putCoordinatesIntoPolygon
               (edgeRecordType           fromEdgRec,
                int                      faceId,
                int                      &pointIndex,
                gms_3D_WorldPolylineType &toPolyline)

{
         int                   k;
         twoDimCoordType       latLongCoord;
         gms_3D_WorldPointType worldPnt;

   if (fromEdgRec.headerInfo.rightFace.id == faceId)
      {
       for (k = 0; k < fromEdgRec.numCoords; k++)
          {
           latLongCoord = fromEdgRec.theCoords[k];

           worldPnt = gmsConvertLatLongToEllipsoidPoint (latLongCoord);

           toPolyline.points[pointIndex] = worldPnt;

           pointIndex++;
          }
      }

   else
      {
       //--------------------------------------*/
       // put the coordinates in reverse order */
       //--------------------------------------*/
       for (k = (fromEdgRec.numCoords - 1); k >= 0; k--)
          {
           latLongCoord = fromEdgRec.theCoords[k];

           worldPnt = gmsConvertLatLongToEllipsoidPoint (latLongCoord);

           toPolyline.points[pointIndex] = worldPnt;

           pointIndex++;
          }
      }
}


//-----------------------------------------------------*/
// putFlatCoordinatesIntoPolygon
//
// Description:
//-----------------------------------------------------*/
static void putFlatCoordinatesIntoPolygon
               (edgeRecordType           fromEdgRec,
                int                      faceId,
                int                      &pointIndex,
                gms_2D_WorldPolylineType &toPolyline)

{
         int                   k;
         twoDimCoordType       latLongCoord;
         gms_2D_WorldPointType worldPnt;

   if (fromEdgRec.headerInfo.rightFace.id == faceId)
      {
       for (k = 0; k < fromEdgRec.numCoords; k++)
          {
           latLongCoord = fromEdgRec.theCoords[k];

           worldPnt = gmsConvertLatLongToFlatPoint (latLongCoord);

           toPolyline.points[pointIndex] = worldPnt;

           pointIndex++;
          }
      }

   else
      {
       //--------------------------------------*/
       // put the coordinates in reverse order */
       //--------------------------------------*/
       for (k = (fromEdgRec.numCoords - 1); k >= 0; k--)
          {
           latLongCoord = fromEdgRec.theCoords[k];

           worldPnt = gmsConvertLatLongToFlatPoint (latLongCoord);

           toPolyline.points[pointIndex] = worldPnt;

           pointIndex++;
          }
      }
}



//-----------------------------------------------------*/
// build_PO_PolygonInfoObject
//
// Description:
//-----------------------------------------------------*/
static void build_PO_PolygonInfoObject
               (const edgeTableType *edgTbl,
                const ringTableType *rngTbl,
                const int           numPolygons,
                const bool          *rngTblFlags,
                gmsPolygonInfoType  &polygonInfo_out)

{
         int i, index;
         int numNodes;
         int totalNumNodes = 0;
         int *ptrToIds;

   polygonInfo_out.numPolygons = numPolygons;

   polygonInfo_out.polygonIdList = new gmsPolygonIdListType[numPolygons];

   index = 0;

   totalNumNodes = 0;

   for (i = 0; i < rngTbl->numRecords; i++)
      {
       if (rngTblFlags[i])
          {
           numNodes = getNumEdgeRecordsInPolygon
                         (rngTbl->ringRecords[i].startEdge,
                          rngTbl->ringRecords[i].faceId,
                          edgTbl);

           polygonInfo_out.polygonIdList[index].faceId =
              rngTbl->ringRecords[i].faceId;

           polygonInfo_out.polygonIdList[index].numNodes = numNodes;

           totalNumNodes = totalNumNodes + numNodes;

           index++;
          }
      }

   ptrToIds = new int[totalNumNodes];

   index = 0;

   for (i = 0; i < polygonInfo_out.numPolygons; i++)
      {
       polygonInfo_out.polygonIdList[i].edgRecordIds = &ptrToIds[index];

       index = index + polygonInfo_out.polygonIdList[i].numNodes;
      }

   index = 0;

   for (i = 0; i < rngTbl->numRecords; i++)
      {
       if (rngTblFlags[i])
          {
           buildPolygonIdList
              (rngTbl->ringRecords[i].startEdge,
               rngTbl->ringRecords[i].faceId,
               edgTbl,
               &(polygonInfo_out.polygonIdList[index]));

           index++;
          }
      }
}


//-----------------------------------------------------*/
// buildGeneralPolygonInfoObject
//
// Description:
//-----------------------------------------------------*/
static void buildGeneralPolygonInfoObject
               (const edgeTableType *edgTbl,
                const ringTableType *rngTbl,
                gmsPolygonInfoType  &polygonInfo_out)

{
         int       i, index;
         int       numNodes;
         int       totalNumNodes = 0;
         int       *ptrToIds;
         const int numPolygons = rngTbl->numRecords;

   polygonInfo_out.numPolygons = numPolygons;

   polygonInfo_out.polygonIdList = new gmsPolygonIdListType[numPolygons];

   totalNumNodes = 0;

   for (i = 0; i < rngTbl->numRecords; i++)
      {
       numNodes = getNumEdgeRecordsInPolygon
                         (rngTbl->ringRecords[i].startEdge,
                          rngTbl->ringRecords[i].faceId,
                          edgTbl);

       polygonInfo_out.polygonIdList[i].faceId =
              rngTbl->ringRecords[i].faceId;

       polygonInfo_out.polygonIdList[i].numNodes = numNodes;

       totalNumNodes = totalNumNodes + numNodes;
      }

   ptrToIds = new int[totalNumNodes];

   index = 0;

   for (i = 0; i < polygonInfo_out.numPolygons; i++)
      {
       polygonInfo_out.polygonIdList[i].edgRecordIds = &ptrToIds[index];

       index = index + polygonInfo_out.polygonIdList[i].numNodes;
      }

   for (i = 0; i < rngTbl->numRecords; i++)
      {
       buildPolygonIdList
              (rngTbl->ringRecords[i].startEdge,
               rngTbl->ringRecords[i].faceId,
               edgTbl,
               &(polygonInfo_out.polygonIdList[i]));
      }
}
