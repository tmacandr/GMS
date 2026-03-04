//-----------------------------------------------------------------------------
// File : gmsNodeClass.h
// Date : 13-Jan-00 : initial definition
//        18-Jan-00 : implement flat-map model
//        25-Jan-00 : add some more debug/diagnosis
//        12-Feb-00 : add <ctype.h> - flagged by SUN C++ compiler
//
// Description:
//    This class defines a "node" object.  As specified in the DCW documentation,
//    a node is a "zero-dimensional geometric primitive that is composed of
//    a single coordinate tuple (pair or triplet)".  The term "zero-dimensional"
//    really means the data is a "point" on a map.  For example, a town
//    or village is often small enough that it is represented as a point.
//    This class permits manipulation of "Entity Nodes" from the DCW database.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
   //----------//
#include <gmsNodeClass.h>
#include <gmsNodeTable.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDcwUtilities.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real map 'object'
//---------------------------------

typedef struct
   {
    nodeTableType             *endTbl;
    gms_3D_WorldPolylineType  nodePoints_3D;
    gms_2D_ScreenPolylineType nodePoints_2D;
   } gmsNodeStructType;


//---------------------------------
// Declare some local variables/constants
//---------------------------------

static char errMsg[128];


//---------------------------------
// Declare Static Member Functions
//---------------------------------
static void build_3D_WorldPointArray
               (nodeTableType            *endTbl,
                gms_3D_WorldPolylineType &nodePoints_3D_out);

static gmsNodeType determineKindOfNode
                      (const char *nodeName);

static void buildMapNodes
               (gmsNodeStructType *ptrToNodes);

static void buildFlatMapNodes
               (gmsNodeStructType *ptrToNodes);




//-----------------------------------------------
// gmsNodeClass
//
// Description:
//    Constructor.  Creates an object that
//    represents an a series of entity nodes.
//-----------------------------------------------
gmsNodeClass::gmsNodeClass
             (gmsEarthModelType whichModel,
              const char        *nodeTblName)

{
         gmsNodeStructType *ptrToNewNodes;
         gmsNodeType       kindOfNode;

   ptrToNewNodes = new gmsNodeStructType;

   if (ptrToNewNodes == NULL)
      {
       gmsWriteDebug ("*** ERROR : 'gmsNodeClass' : new FAIL\n");

       gmsNodeClass::theNode = NULL;

       return;
      }

   kindOfNode = determineKindOfNode
                   (nodeTblName);

   ptrToNewNodes->endTbl = gmsGetNodeTable
                              (nodeTblName,
                               kindOfNode);

   if (ptrToNewNodes->endTbl == NULL)
      {
       sprintf(errMsg, "*** ERROR : 'gmsGetNodeTable' : %s failed\n",
               nodeTblName);

       gmsWriteDebug (errMsg);

       gmsNodeClass::theNode = NULL;

       return;
      }

   if (whichModel == gmsFlat)
      {
       ptrToNewNodes->nodePoints_3D.numPoints = 0;

       ptrToNewNodes->nodePoints_3D.points = NULL;
      }

   else
      {
       ptrToNewNodes->nodePoints_3D.numPoints =
          ptrToNewNodes->endTbl->numRecords;

       ptrToNewNodes->nodePoints_3D.points =
          new gms_3D_WorldPointType[ptrToNewNodes->endTbl->numRecords];

       build_3D_WorldPointArray
          (ptrToNewNodes->endTbl,
           ptrToNewNodes->nodePoints_3D);
      }

   ptrToNewNodes->nodePoints_2D.numPoints = 0;

   ptrToNewNodes->nodePoints_2D.points =
      new gms_2D_ScreenPointType[ptrToNewNodes->endTbl->numRecords];

   gmsNodeClass::theNode = (void *) ptrToNewNodes;

   gmsNodeClass::theModel = whichModel;
}


//-----------------------------------------------
// ~gmsNodeClass
//
// Description:
//    Destructor.  Deletes the 
//-----------------------------------------------
gmsNodeClass::~gmsNodeClass ()

{
         gmsNodeStructType *ptrToNodes;

   if (gmsNodeClass::theNode == NULL)
      return;

   ptrToNodes = (gmsNodeStructType *) gmsNodeClass::theNode;

   if (ptrToNodes->endTbl != NULL)
      gmsFreeNodeTable (ptrToNodes->endTbl);

   if (ptrToNodes->nodePoints_2D.points != NULL)
      delete [] ptrToNodes->nodePoints_2D.points;

   if (ptrToNodes->nodePoints_3D.points != NULL)
      delete [] ptrToNodes->nodePoints_3D.points;

   delete ptrToNodes;

   gmsNodeClass::theNode = NULL;
}


//-----------------------------------------------
// gmsGetNodePoints
//
// Description:
//    Returns an array of 2D points that represent
//    the nodes.  Although the returned struct
//    is a 'polyline', it actually represents
//    a series of independent points.
//-----------------------------------------------
gms_2D_ScreenPolylineType gmsNodeClass::gmsGetNodePoints ()

{
         gmsNodeStructType               *ptrToNodes;
         const gms_2D_ScreenPolylineType nullPoints = {0, NULL};

   if (gmsNodeClass::theNode == NULL)
      {
       gmsWriteDebug ("*** ERR : gmsGetNodePoints : obj is NULL\n");

       return nullPoints;
      }

   ptrToNodes = (gmsNodeStructType *) gmsNodeClass::theNode;

   if (gmsNodeClass::theModel == gmsFlat)
      {
       buildFlatMapNodes (ptrToNodes);
      }

   else
      {
       buildMapNodes (ptrToNodes);
      }

   return ptrToNodes->nodePoints_2D;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// build_3D_WorldPointArray
//
// Description:
//    This routine uses the (lat/long) coordinates
//    from the given node table to build an array
//    of 3D world points.  These world points are
//    not referenced to the screen coordinate.
//-----------------------------------------------
static void build_3D_WorldPointArray
               (nodeTableType            *endTbl,
                gms_3D_WorldPolylineType &nodePoints_3D_out)

{
         int  i;

   for (i = 0; i < endTbl->numRecords; i++)
      {
       nodePoints_3D_out.points[i] = gmsConvertLatLongToEllipsoidPoint
                                        (endTbl->nodeRecords[i].coordinate);
      }
}


//-----------------------------------------------
// determineKindOfNode
//
// Description:
//    This routine uses the full path of the node
//    table to determine its kind.  This function
//    returns either:
//
//             gmsBrowseNode
//             gmsGazetteNode
//             gmsThemeNode
//-----------------------------------------------
static gmsNodeType determineKindOfNode
                      (const char *nodeName)

{
         int          length;
         int          i, j;
         char         *copy;
         const char   *browseString  = { "BROWSE" };
         const int    browseLen      = strlen(browseString);
         const char   *gazetteString = { "GAZETTE" };
         const int    gazetteLen     = strlen(gazetteString);
         char         *slice;

   length = (int) strlen (nodeName);

   copy = new char[length]; // would be nice if mod 4

   for (i = 0; i < length; i++)
      {
       copy[i] = toupper(nodeName[i]);
      }

   //
   // Search for the string "BROWSE"
   //
   i = 0;

   while ( (int) strlen(&copy[i]) >= browseLen)
      {
       slice = &copy[i];

       for (j = 0; j < browseLen; j++)
          {
           if (slice[j] != browseString[j])

           break; // out of the 'for loop'
          }

       if (j == browseLen)
          {
           delete [] copy;

           return gmsBrowseNode;
          }

       i++;
      }

   //
   // Search for the string "GAZETTE"
   //
   i = 0;

   while ( (int) strlen(&copy[i]) >= gazetteLen)
      {
       slice = &copy[i];

       for (j = 0; j < gazetteLen; j++)
          {
           if (slice[j] != gazetteString[j])

           break; // out of the 'for loop'
          }

       if (j == gazetteLen)
          {
           delete [] copy;

           return gmsGazetteNode;
          }

       i++;
      }

   delete [] copy;

   return gmsThemeNode;
}


//-----------------------------------------------
// buildMapNodes
//
// Description:
//-----------------------------------------------
static void buildMapNodes
               (gmsNodeStructType *ptrToNodes)


{
         gms_2D_ScreenPointType          screenPnt_2D;
         gms_3D_ScreenPointType          screenPnt_3D;
         int                             i;
         int                             numPoints;
         gmsMapStateStructType           mapState;

   numPoints = 0;

   mapState = gmsGetMapState ();

   for (i = 0; i < ptrToNodes->nodePoints_3D.numPoints; i++)
      {
       screenPnt_3D = gmsConvertWorldCoordToScreenCoord
                            (ptrToNodes->nodePoints_3D.points[i]);

       screenPnt_3D = gmsRotateThreeDimPointAbout_Y_Axis
                            (screenPnt_3D,
                             mapState.angle_Y_Rotation);

       screenPnt_3D = gmsRotateThreeDimPointAbout_X_Axis
                            (screenPnt_3D,
                             mapState.angle_X_Rotation);

       if (screenPnt_3D.z >= 0)
          {
           screenPnt_3D = gmsTranslateThreeDimPoint
                                 (screenPnt_3D,
                                  (mapState.screenWidth / 2),
                                  (mapState.screenHeight / 2),
                                  mapState.zTranslation);

           screenPnt_2D = gmsConvertToProjectedPoint
                                 (screenPnt_3D,
                                  mapState.screenWidth,
                                  mapState.screenHeight);

           ptrToNodes->nodePoints_2D.points[numPoints] = screenPnt_2D;

           numPoints++;
          }
      }

   ptrToNodes->nodePoints_2D.numPoints = numPoints;
}


//-----------------------------------------------
// buildFlatMapNodes
//
// Description:
//-----------------------------------------------
static void buildFlatMapNodes
               (gmsNodeStructType *ptrToNodes)

{
         int                    i;
         gms_2D_WorldPointType  worldPoint;
         gms_2D_ScreenPointType screenPoint;
         gms_2D_ScreenPointType translations;

   translations = gmsCalculateFlatMapTranslations ();

   for (i = 0; i < ptrToNodes->endTbl->numRecords; i++)
      {
       worldPoint = gmsConvertLatLongToFlatPoint
                       (ptrToNodes->endTbl->nodeRecords[i].coordinate);

       screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

       screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

       ptrToNodes->nodePoints_2D.points[i] = screenPoint;
      }

   ptrToNodes->nodePoints_2D.numPoints = ptrToNodes->endTbl->numRecords;
}
