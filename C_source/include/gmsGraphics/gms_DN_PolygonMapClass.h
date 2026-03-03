//-----------------------------------------------------------------------------
// File : gms_DN_PolygonMapClass.h
// Date : 04-Feb-00 : initial definition
//
// Description:
//    This class defines a "map of polygons" from the DRAINAGE (DN) theme.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_DN_POLYGON_MAP

   #define GMS_DN_POLYGON_MAP

   #include <gmsTypesAndConstants.h>
   #include <gmsFeatureTable.h>


   class gms_DN_PolygonMapClass
      {
       public:

          //-----------------------------------------------
          // gms_DN_PolygonMapClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents polygons from the edge table
          //    from the DN theme.
          //-----------------------------------------------
          gms_DN_PolygonMapClass
                (gmsEarthModelType whichModel,
                 const char        *edgeTblName);


          //-----------------------------------------------
          // gms_DN_PolygonMapClass
          //
          // Description:
          //    Destructor.  Deletes the map object.
          //-----------------------------------------------
          ~gms_DN_PolygonMapClass ();


          //-----------------------------------------------
          // gmsGetInlandWaterAreas
          //
          // Description:
          //    Retrieves a 2D screen image of polygons of
          //    inland water areas (e.g. lakes, ponds).  The
          //    lines in the returned data structure represent
          //    one or more polygons.  Each polygon
          //    represents an inland water area
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetInlandWaterAreas();

       private:

          void              *dnPolygonMap;

          gmsEarthModelType theModel;
      };

#endif
