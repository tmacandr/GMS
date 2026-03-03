//-----------------------------------------------------------------------------
// File : gmsBrowseMapClass.h
// Date : 12-Jan-00 : initial definition
//        25-Jan-00 : make use of 'gmsDcwTypesAndConstants.h'
//        05-Feb-00 : write more "description"
//
// Description:
//    This class is used to abstract the edge tables of the BROWSE library
//    into a map object.  Edge tables from any browse theme can be converted
//    to 2D screen images for the client.  Also, a grid of latitude and
//    longitude lines can be generated.  Nodes (e.g. cities, towns) and
//    text (i.e. names of countries) can also be obtained from a browse
//    map object.  The object also provides access to the data tables
//    (i.e. the library attribute table, coverage attribute table, etc).
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_BROWSE_MAP

   #define GMS_BROWSE_MAP

   #include <gmsTypesAndConstants.h>
   #include <gmsDcwTypesAndConstants.h>
   #include <gmsTextClass.h>


   class gmsBrowseMapClass
      {
       public:

          //-----------------------------------------------
          // gmsBrowseMapClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents a map based on the BROWSE
          //    library.
          //-----------------------------------------------
          gmsBrowseMapClass
             (gmsEarthModelType whichModel);


          //-----------------------------------------------
          // ~gmsBrowseMapClass
          //
          // Description:
          //    Destructor.  Deletes the (BROWSE) map
          //    object.
          //-----------------------------------------------
          ~gmsBrowseMapClass ();


          //-----------------------------------------------
          // gmsGetBrowseMapImage
          //
          // Description:
          //    Returns a 2D image of the edge table from
          //    the specified BROWSE theme.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetBrowseMapImage
                                    (gmsBrowseThematicType whichTheme);


          //-----------------------------------------------
          // gmsGetLatitudeGrid
          //
          // Description:
          //    Returns a 2D image of the latitude grid for
          //    the map.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetLatitudeGrid ();


          //-----------------------------------------------
          // gmsGetLongitudeGrid
          //
          // Description:
          //    Returns a 2D image of the longitude grid for
          //    the map.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetLongitudeGrid ();



          //-----------------------------------------------
          // gmsGetBrowseNodes
          //
          // Description:
          //    Returns an array of 2D points that were
          //    constructed from the node table of the
          //    specified BROWSE theme.
          //-----------------------------------------------
          gms_2D_ScreenPolylineType gmsGetBrowseNodes
                                       (gmsBrowseThematicType whichTheme);


          //-----------------------------------------------
          // gmsGetTextItems
          //
          // Description:
          //    This method retrieves the text strings
          //    from the specified BROWSE theme.  These
          //    strings are used to annotate the map.
          //-----------------------------------------------
          gmsMapTextArrayType gmsGetTextItems
                                 (gmsBrowseThematicType whichTheme);


          //-----------------------------------------------
          // gmsGetBrowseEbrImage
          //
          // Description:
          //    Returns the "Edge-Bounding-Rectangles" from
          //    the specified browse theme.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetBrowseEbrImage
                                    (gmsBrowseThematicType whichTheme);


          //-----------------------------------------------
          // gmsGetBrowseFbrImage
          //
          // Description:
          //    Returns the "Face-Bounding-Rectangles" from
          //    the specified browse theme.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetBrowseFbrImage
                                    (gmsBrowseThematicType whichTheme);


          //-----------------------------------------------
          // gmsGetDataTable
          //
          // Description:
          //    This method retrieves a text string that
          //    is the data from one of the data tables
          //    from BROWSE library.
          //-----------------------------------------------
          const char *gmsGetDataTable
                        (gmsDataTablesType whichTable);


       private:

          void              *theBrowseMap;

          gmsEarthModelType theModel;

      };

#endif
