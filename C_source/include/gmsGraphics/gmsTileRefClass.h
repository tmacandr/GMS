//-----------------------------------------------------------------------------
// File : gmsTileRefClass.h
// Date : 08-Jan-00 : initial definition
//        20-Jan-00 : drop use of 'gmsFlatMapClass'
//        12-Feb-00 : rename priv attrib - prevent collision with in parameter
//
// Description:
//    This class defines a "Tile Reference" object.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_TILEREF_CLASS

   #define GMS_TILEREF_CLASS

   #include <gmsTypesAndConstants.h>
   #include <gmsMapClass.h>
   #include <gmsFeatureTable.h>

   class gmsTileRefClass
      {
       public:

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
          gmsTileRefClass
             (gmsEarthModelType whichModel);


          //-----------------------------------------------
          // ~gmsTileRefClass
          //
          // Description:
          //    Destructor.  Deletes the object.
          //-----------------------------------------------
          ~gmsTileRefClass ();


          //-----------------------------------------------
          // gmsGetTileRefImage
          //
          // Description:
          //    Returns the 2-D image of the edges of the
          //    tile-ref in screen coordinates.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetTileRefImage ();


          //-----------------------------------------------
          // gmsConvertTileIdToName
          //
          // Description:
          //    This function converts a "Tile-ID" (an
          //    integer) to the corresponding name of
          //    the tile ("XYxy").
          //-----------------------------------------------
          char *gmsConvertTileIdToName
                  (int tileId);

       private:

          gmsMapClass       *ptrToTileRef;

          gmsEarthModelType theModel;

          featureTableType  *areaFeatureTbl;
      };

#endif
