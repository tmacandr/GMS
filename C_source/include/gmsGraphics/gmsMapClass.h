//-----------------------------------------------------------------------------
// File : gmsMapClass.h
// Date : 10-Nov-99 : initial definition
//        14-Nov-99 : move lat/long grid to separate class
//        09-Jan-00 : export the edge table
//        19-Jan-00 : provide access to node table
//        20-Jan-00 : put switch for earth model
//        23-Jan-00 : put in then took out polygon functions
//        24-Jan-00 : take out export of 'edge' table.  Can't remember why.
//        24-Jan-00 : make use of '*LINE.LFT' table to clear tile boundaries
//        05-Feb-00 : write more for "description"
//
// Description:
//    This class is used to abstract a given edge table into a map object.
//    This map object is presented to the client as a screen image.  The map
//    object consists of the lines/edges of the map as well as the nodes
//    (e.g. cities, towns) within that map.  A map object can be instantiated
//    as a "raw" image or as a "filtered" image.  A filtered image contains
//    no tile boundary information.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_MAP

   #define GMS_MAP

   #include <gmsTypesAndConstants.h>
   #include <gmsFeatureTable.h>

   class gmsMapClass
      {
       public:

          //-----------------------------------------------
          // gmsMapClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents a map.  The specified edge table
          //    is used 'as is'.  No modifications and
          //    filtering is applied to the data from the
          //    edge table.
          //-----------------------------------------------
          gmsMapClass
             (gmsEarthModelType whichModel,
              const char        *edgeTblName);


          //-----------------------------------------------
          // gmsMapClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents a map.  The 'lineFeatureTblName'
          //    is used to access the corresponding *LINE.LFT
          //    file.
          //
          //    This method makes use of the line feature
          //    table for the following themes when building
          //    the map image:
          //
          //            DN, HY, HS, PO, RR, RD, TS
          //-----------------------------------------------
          gmsMapClass
             (gmsEarthModelType      whichModel,
              const char             *edgeTblName,
              const featureTableType *lineFeatureTbl);


          //-----------------------------------------------
          // ~gmsMapClass
          //
          // Description:
          //    Destructor.  Deletes the map object.
          //-----------------------------------------------
          ~gmsMapClass ();


          //-----------------------------------------------
          // gmsGetMapImage
          //
          // Description:
          //    Retrieves a 2D screen image of the current
          //    map object.  This image represents ALL of
          //    the edges of the current map.  The resulting
          //    lines (in the data structure) are not sorted.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetMapImage ();


          //-----------------------------------------------
          // gmsGetMapNodePoints
          //
          // Description:
          //    Returns an array of 2D points that represent
          //    the entity nodes that correspond with the
          //    edge table.  Although the returned struct
          //    is a 'polyline', it actually represents
          //    a series of independent points.
          //-----------------------------------------------
          gms_2D_ScreenPolylineType gmsGetMapNodePoints ();


       private:

          void              *theMap;

          gmsEarthModelType theModel;

      };

#endif
