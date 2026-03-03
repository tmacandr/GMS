//-----------------------------------------------------------------------------
// File : gms_PO_PolygonMapClass.cpp
// Date : 23-Jan-00 : initial definition
//        30-Jan-00 : rename.  For "PO" theme only
//
// Description:
//    This class defines a "map of polygons" from the PO theme.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_PO_POLYGON_MAP

   #define GMS_PO_POLYGON_MAP

   #include <gmsTypesAndConstants.h>
   #include <gmsFeatureTable.h>


   class gms_PO_PolygonMapClass
      {
       public:

          //-----------------------------------------------
          // gms_PO_PolygonMapClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents polygons from the edge table
          //    from the PO theme.
          //-----------------------------------------------
          gms_PO_PolygonMapClass
                (gmsEarthModelType whichModel,
                 const char        *edgeTblName,
                 featureTableType  *areaFeatureTbl);


          //-----------------------------------------------
          // ~gms_PO_PolygonMapClass
          //
          // Description:
          //    Destructor.  Deletes the map object.
          //-----------------------------------------------
          ~gms_PO_PolygonMapClass ();


          //-----------------------------------------------
          // gmsGetLandAreas
          //
          // Description:
          //    Retrieves a 2D screen image of polygons of
          //    LAND areas.  The lines in the returned data
          //    structure represent one or more polygons.
          //    Each polygon represents a land area
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetLandAreas ();


          //-----------------------------------------------
          // gmsGetOceanAreas
          //
          // Description:
          //    Retrieves a 2D screen image of polygons of
          //    OCEAN areas.  The lines in the returned data
          //    structure represent one or more polygons.
          //    Each polygon represents a part of an ocean.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetOceanAreas ();

       private:

          void              *thePolygonMap;

          gmsEarthModelType theModel;
      };

#endif
