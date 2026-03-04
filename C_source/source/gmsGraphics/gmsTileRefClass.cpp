//-----------------------------------------------------------------------------
// File : gmsTileRefClass.cpp
// Date : 08-Jan-00 : initial definition
//        20-Jan-00 : drop use of 'gmsFlatMapClass'
//        12-Feb-00 : rename priv attrib - prevent collision with in parameter
//
// Description:
//    This class defines a "Tile Reference" object.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <gmsTileRefClass.h>
#include <gmsUtilities.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Declare some variables/constants
//---------------------------------

static char errString[128];


//---------------------------------
// Declare Static Member Functions
//---------------------------------


//-----------------------------------------------
// gmsTileRefClass
//
// Description:
//    Constructor.  Creates an object that
//    represents a Tile-Ref object.  The width
//    and height parameters are used to set the
//    center point of the image.  The parameter
//    "whichModel" indicates flat, sphere, or
//    ellipsoid model.
//-----------------------------------------------
gmsTileRefClass::gmsTileRefClass
             (gmsEarthModelType whichModel)

{
   gmsTileRefClass::ptrToTileRef = new gmsMapClass
                                          (whichModel,
                                           gmsGetDcwFullPath("tileref/edg"));

   gmsTileRefClass::theModel = whichModel;

   gmsTileRefClass::areaFeatureTbl =
      gmsGetFeatureTable
            (gms_TileRef,
             gmsGetDcwFullPath("tileref/tileref.aft"),
             gmsArea);
}


//-----------------------------------------------
// ~gmsTileRefClass
//
// Description:
//    Destructor.  Deletes the object.
//-----------------------------------------------
gmsTileRefClass::~gmsTileRefClass ()

{
   if (gmsTileRefClass::ptrToTileRef != NULL)
      delete gmsTileRefClass::ptrToTileRef;

   if (gmsTileRefClass::areaFeatureTbl != NULL)
      gmsFreeFeatureTable (gmsTileRefClass::areaFeatureTbl);
}


//-----------------------------------------------
// gmsGetMbrImage
//
// Description:
//    Returns the 2-D image of the edges of the
//    tile-ref in screen coordinates.
//-----------------------------------------------
gms_2D_ScreenImageType gmsTileRefClass::gmsGetTileRefImage ()

{
   return gmsTileRefClass::ptrToTileRef->gmsGetMapImage();
}


//-----------------------------------------------
// gmsConvertTileIdToName
//
// Description:
//    This function converts a "Tile-ID" (an
//    integer) to the corresponding name of
//    the tile ("XYxy").
//-----------------------------------------------
char *gmsTileRefClass::gmsConvertTileIdToName
                          (int tileId)

{
         tileRef_areaFeatureRecType *tileRefAFT;
         const int                  Min_Tile_Id = 1;

   if ( (tileId <= Min_Tile_Id) || // 1st record is garbage for some reason
        (tileId > gmsTileRefClass::areaFeatureTbl->numRecords) )
      {
       sprintf
        (errString,
         "ERR gmsConvertTileIdToName : bad tile-id %d  Min = %d  Max = %d\n",
         tileId, Min_Tile_Id,
         gmsTileRefClass::areaFeatureTbl->numRecords);

       gmsWriteDebug(errString);

       return (char *) NULL;
      }

   tileRefAFT = (tileRef_areaFeatureRecType *)
                gmsTileRefClass::areaFeatureTbl->featureRecords;

   if (tileId != tileRefAFT[tileId - 1].ID)
      {
       sprintf
          (errString,
           "ERR : gmsConvertTileIdToName : Tile-Id = %d but AFT ID = %d\n",
           tileId, tileRefAFT[tileId - 1].ID);

       gmsWriteDebug(errString);

       return (char *) NULL;
      }

   return tileRefAFT[tileId - 1].tileName;
}




        //---------------------------
        //  Static Member Functions
        //---------------------------


