//-----------------------------------------------------------------------------
// File : gmsLatLongGridClass.h
// Date : 14-Nov-99 : initial definition
//        20-Jan-00 : combine with "gmsFlatLatLongGridClass".  Both models.
//        28-Jan-00 : mods to support close zoom
//        05-Feb-00 : write more "description"
//
// Description:
//    This class defines a map object that contains a grid of lat/long
//    reference lines.  The client may instantiate a grid for either a flat-
//    earth map or an ellipsoid model.  One method is provided to retrieve
//    a screen image of a latitude grid and another method is provided for
//    a longitude grid.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_LAT_LONG_GRID

   #define GMS_LAT_LONG_GRID

   #include <gmsTypesAndConstants.h>

   class gmsLatLongGridClass
      {
       public:

          //-----------------------------------------------
          // gmsLatLongGridClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents a 2D image of a grid of a
          //    grid of lat/long lines.  The width
          //    and height parameters are used to set the
          //    center point of the image.
          //-----------------------------------------------
          gmsLatLongGridClass
             (double            deltaDeg,
              gmsEarthModelType whichModel);


          //-----------------------------------------------
          // ~gmsLatLongGridClass
          //
          // Description:
          //    Destructor.  Deletes the object.
          //-----------------------------------------------
          ~gmsLatLongGridClass ();


          //-----------------------------------------------
          // gmsGetLatitudeGrid
          //
          // Description:
          //    Returns the screen image of the grid of
          //    latitude lines.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetLatitudeGrid ();


          //-----------------------------------------------
          // gmsGetLongitudeGrid
          //
          // Description:
          //    Returns the screen image of the grid of
          //    longitude lines.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetLongitudeGrid ();


       private:

          void              *theGrid;

          gmsEarthModelType theModel;
      };

#endif
