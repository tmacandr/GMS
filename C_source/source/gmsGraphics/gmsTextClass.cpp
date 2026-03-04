//-----------------------------------------------------------------------------
// File : gmsTextClass.cpp
// Date : 13-Jan-00 : initial definition
//
// Description:
//    This class defines a "text" object.  A text table from the DCW data-
//    base is abstracted by this object.  The text strings obtained from
//    this object are used to annotate a particular map.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <gmsTextClass.h>
#include <gmsTextTable.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDcwUtilities.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real text 'object'
//---------------------------------

typedef struct
   {
    textTableType            *txtTbl;
    gms_3D_WorldPolylineType textPoints_3D;
    gmsMapTextArrayType      textItems_2D;
   } gmsTextObjectType;


//---------------------------------
// Declare some constants
//---------------------------------


//---------------------------------
// Declare Static Member Functions
//---------------------------------
static void build_3D_WorldPointArray
               (textTableType            *txtTbl,
                gms_3D_WorldPolylineType &pointArray_3D_out);

static void buildTextItems
               (gmsTextObjectType *ptrToTxt);

static void buildFlatMapTextItems
               (gmsTextObjectType *ptrToTxt);




//-----------------------------------------------
// gmsTextClass
//
// Description:
//    Constructor.  Creates an object that
//    represents a series of text items from
//    a DCW text table.
//-----------------------------------------------
gmsTextClass::gmsTextClass
             (gmsEarthModelType whichModel,
              const char        *txtTblName)

{
         gmsTextObjectType *ptrToNewTxtObj;

   ptrToNewTxtObj = new gmsTextObjectType;

   ptrToNewTxtObj->txtTbl = gmsGetTextTable (txtTblName);

   if (ptrToNewTxtObj->txtTbl == NULL)
      {
       gmsWriteDebug ("*** ERROR : 'gmsGetTextTable' failed\n");

       return;
      }

   if (whichModel == gmsFlat)
      {
       ptrToNewTxtObj->textPoints_3D.numPoints = 0;

       ptrToNewTxtObj->textPoints_3D.points = NULL;
      }

   else
      {
       ptrToNewTxtObj->textPoints_3D.numPoints =
          ptrToNewTxtObj->txtTbl->numRecords;

       ptrToNewTxtObj->textPoints_3D.points =
          new gms_3D_WorldPointType[ptrToNewTxtObj->txtTbl->numRecords];

       build_3D_WorldPointArray
          (ptrToNewTxtObj->txtTbl,
           ptrToNewTxtObj->textPoints_3D);
      }

   ptrToNewTxtObj->textItems_2D.numTextRecords = 0;

   ptrToNewTxtObj->textItems_2D.textRecords =
      new gmsMapTextStructType[ptrToNewTxtObj->txtTbl->numRecords];

   gmsTextClass::theTxt = (void *) ptrToNewTxtObj;

   gmsTextClass::theModel = whichModel;
}


//-----------------------------------------------
// ~gmsTextClass
//
// Description:
//    Destructor.  Deletes the text object.
//-----------------------------------------------
gmsTextClass::~gmsTextClass ()

{
         gmsTextObjectType *ptrToTxt;

   if (gmsTextClass::theTxt == NULL)
      return;

   ptrToTxt = (gmsTextObjectType *) gmsTextClass::theTxt;

   if (ptrToTxt->txtTbl != NULL)
      gmsFreeTextTable (ptrToTxt->txtTbl);

   if (ptrToTxt->textPoints_3D.points != NULL)
      {
       delete [] ptrToTxt->textPoints_3D.points;
      }

   if (ptrToTxt->textItems_2D.textRecords != NULL)
      {
       delete [] ptrToTxt->textItems_2D.textRecords;
      }

   delete ptrToTxt;

   gmsTextClass::theTxt = NULL;
}


//-----------------------------------------------
// gmsGetTextItems
//
// Description:
//    This method builds an array of text strings
//    that annotate a map.
//-----------------------------------------------
gmsMapTextArrayType gmsTextClass::gmsGetTextItems ()

{
         gmsTextObjectType *ptrToTxt;

   ptrToTxt = (gmsTextObjectType *) gmsTextClass::theTxt;

   if (gmsTextClass::theModel == gmsFlat)
      {
       buildFlatMapTextItems (ptrToTxt);
      }

   else
      {
       buildTextItems (ptrToTxt);
      }

   return ptrToTxt->textItems_2D;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// build_3D_WorldPointArray
//
// Description:
//    This routine uses the (lat/long) coordinates
//    from the text table to build an array of 3D
//    world points.  These points represent the
//    location of text strings that are used to
//    annotate a map.  These points have no reference
//    to the screen coordinate system.
//-----------------------------------------------
static void build_3D_WorldPointArray
               (textTableType            *txtTbl,
                gms_3D_WorldPolylineType &pointArray_3D_out)

{
         int i;

    for (i = 0; i < txtTbl->numRecords; i++)
      {
       pointArray_3D_out.points[i] =
          gmsConvertLatLongToEllipsoidPoint
             (txtTbl->textRecords[i].theCoords[0]); // 1st coord only
      }
}


//-----------------------------------------------
// buildTextItems
//
// Description:
//    This method builds the array of text strings
//    that annotate a map.
//-----------------------------------------------
static void buildTextItems
               (gmsTextObjectType *ptrToTxt)

{
         gms_2D_ScreenPointType    screenPnt_2D;
         gms_3D_ScreenPointType    screenPnt_3D;
         int                       i;
         int                       numPoints;
         gmsMapStateStructType     mapState;

   mapState = gmsGetMapState ();

   numPoints = 0;

   for (i = 0; i < ptrToTxt->textPoints_3D.numPoints; i++)
      {
       screenPnt_3D = gmsConvertWorldCoordToScreenCoord
                            (ptrToTxt->textPoints_3D.points[i]);

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

           ptrToTxt->textItems_2D.textRecords[numPoints].theCoord =
              screenPnt_2D;

           ptrToTxt->textItems_2D.textRecords[numPoints].numChars =
              ptrToTxt->txtTbl->textRecords[i].numChars;

           ptrToTxt->textItems_2D.textRecords[numPoints].theString =
              ptrToTxt->txtTbl->textRecords[i].theString;

           numPoints++;
          }
      }

   ptrToTxt->textItems_2D.numTextRecords = numPoints;
}


//-----------------------------------------------
// buildFlatMapTextItems
//
// Description:
//-----------------------------------------------
static void buildFlatMapTextItems
               (gmsTextObjectType *ptrToTxt)

{
         int                    i;
         gms_2D_WorldPointType  worldPoint;
         gms_2D_ScreenPointType screenPoint;
         gms_2D_ScreenPointType translations;

   translations = gmsCalculateFlatMapTranslations ();

   for (i = 0; i < ptrToTxt->txtTbl->numRecords; i++)
      {
       worldPoint = gmsConvertLatLongToFlatPoint
                       (ptrToTxt->txtTbl->textRecords[i].theCoords[0]);

       screenPoint = gmsConvertFlatWorldCoordToScreenCoord (worldPoint);

       screenPoint = gmsTranslateTwoDimPoint
                                (screenPoint,
                                 translations.x,
                                 translations.y);

       ptrToTxt->textItems_2D.textRecords[i].theCoord =
              screenPoint;

       ptrToTxt->textItems_2D.textRecords[i].numChars =
              ptrToTxt->txtTbl->textRecords[i].numChars;

       ptrToTxt->textItems_2D.textRecords[i].theString =
              ptrToTxt->txtTbl->textRecords[i].theString;
      }

   ptrToTxt->textItems_2D.numTextRecords = ptrToTxt->txtTbl->numRecords;
}
