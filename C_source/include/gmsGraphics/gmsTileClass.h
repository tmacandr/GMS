//-----------------------------------------------------------------------------
// File : gmsTileClass.h
// Date : 20-Dec-99 : intitial definition
//        25-Jan-00 : continue development ... never really finished
//        26-Jan-00 : pare down to "tile" only instead of whole theme
//        06-Feb-00 : add methods to get polygons
//        06-Feb-00 : rename.  Not theme specific anymore
//
// Description:
//    This class is an abstraction to a GEOREF tile.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_TILE_CLASS

   #define GMS_TILE_CLASS

   #include <gmsTypesAndConstants.h>
   #include <gmsUtilities.h>
   #include <gmsMapClass.h>
   #include <gmsNodeClass.h>
   #include <gmsTextClass.h>
   #include <gms_PO_PolygonMapClass.h>
   #include <gms_DN_PolygonMapClass.h>


   class gmsTileClass
      {
       public:

          //-----------------------------------------------
          // gmsTileClass
          //
          // Description:
          //    Constructor.  An object that represents the
          //    edges, points, text, etc of the specified
          //    tile is built.
          //-----------------------------------------------
          gmsTileClass
                (gmsEarthModelType whichModel,
                 const char        longTileId,
                 const char        latTileId);


          //-----------------------------------------------
          // ~gmsTileClass
          //
          // Description:
          //    Destructor.
          //-----------------------------------------------
          ~gmsTileClass();


          //-----------------------------------------------
          // gmsGetTileMaps
          //
          // Description:
          //    Retrieves the MAP objects of the tile from
          //    the specified theme.  A pointer to an array
          //    of map objects is returned to the caller.
          //    The number of map objects is returned in
          //    the parameter "numObjs".
          //-----------------------------------------------
          gmsMapClass **gmsGetTileMaps
                           (const gmsThematicType whichTheme,
                            int                   &numObjs);


          //-----------------------------------------------
          // gmsGetTile_PO_Polygons
          //
          // Description:
          //    Retrieves the POLITICAL/OCEANS (PO) polygon
          //    objects for the tile.  A pointer to an array
          //    of polygon objects is returned to the caller.
          //    The number of polygon objects is returned in
          //    the parameter "numObjs".
          //-----------------------------------------------
          gms_PO_PolygonMapClass **gmsGetTile_PO_Polygons
                                      (int &numObjs);


          //-----------------------------------------------
          // gmsGetTileText
          //
          // Description:
          //    Retrieves the TEXT objects of the tile from
          //    the specified theme.  A pointer to an array
          //    of text objects is returned to the caller.
          //    The number of text objects is returned in
          //    the parameter "numObjs".
          //-----------------------------------------------
          gmsTextClass **gmsGetTileText
                            (const gmsThematicType whichTheme,
                             int                   &numObjs);


          //-----------------------------------------------
          // gmsGetTileNodes
          //
          // Description:
          //    Retrieves the NODE objects of the tile from
          //    the specified theme.  A pointer to an array
          //    of node objects is returned to the caller.
          //    The number of node objects is returned in
          //    the parameter "numObjs".
          //-----------------------------------------------
          gmsNodeClass **gmsGetTileNodes
                            (const gmsThematicType whichTheme,
                             int                   &numObjs);


          //-----------------------------------------------
          // gmsGetTile_DN_Polygons
          //
          // Description:
          //    Retrieves the DRAINAGE (DN) polygon
          //    objects for the tile.  A pointer to an array
          //    of polygon objects is returned to the caller.
          //    The number of polygon objects is returned in
          //    the parameter "numObjs".
          //-----------------------------------------------
          gms_DN_PolygonMapClass **gmsGetTile_DN_Polygons
                                      (int &numObjs);

       private:

          void *tileObj;
      };

#endif
