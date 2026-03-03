//-----------------------------------------------------------------------------
// File : gmsMapStateMgr.h
// Date : 14-Nov-99 : initial definition
//        09-Dec-99 : update due to rename of utils
//        10-Dec-99 : add mgmt of flat map attributes
//        27-Jan-00 : removed distinction of flat/ellipsoid zoom
//        28-Jan-00 : defined "ellipsoid zoom minimum"
//        29-Jan-00 : added "how much" to move flat map function
//
// Description:
//    This class defines a package that manages the current state of the
//    map (currently under view).  This manager allows for several map objects
//    to stay synchronized to a common state.  The state managed by this
//    package are the following attributes:
//
//                      zoom factor
//                      x, y, and z rotations
//                      screen width and height
//                      z translation value (a constant?)
//                      operator distance from screen (a constant?)
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_MAP_MGR

   #define GMS_MAP_MGR

   #include <gmsTypesAndConstants.h>
   #include <gmsUtilities.h>

   typedef struct
      {
       double                 angle_X_Rotation;
       double                 angle_Y_Rotation;
       double                 angle_Z_Rotation;
       double                 zoomFactor;
       double                 minEellipsoidZoom;
       int                    screenWidth;
       int                    screenHeight;
       int                    zTranslation;
       int                    operatorDistance;
       twoDimCoordType        flatMapCenter;
      } gmsMapStateStructType;

   typedef enum { gmsMoveNorth,
                  gmsMoveSouth,
                  gmsMoveEast,
                  gmsMoveWest } gmsFlatMapMoveType;


   //-----------------------------------------------
   // gmsGetMapState
   //
   // Description:
   //    This allows the client to ask for the
   //    current "state" of the map.  The object
   //    returned shows the current state.
   //-----------------------------------------------
   gmsMapStateStructType gmsGetMapState ();


   //-----------------------------------------------
   // gmsSetWindowDimensions
   //
   // Description:
   //    Allows the client to update the map state
   //    when the size of the window (where the
   //    map is drawn) is changed.
   //-----------------------------------------------
   void gmsSetWindowDimensions
           (int screenWidth,
            int screenHeight);


   //-----------------------------------------------
   // gmsSetMapZoomFactor
   //
   // Description:
   //    Allows the client to change the zoom factor.
   //-----------------------------------------------
   void gmsSetMapZoomFactor
               (double newFactor);


   //-----------------------------------------------
   // gmsZoomIn
   //
   // Description:
   //    Allows the client to zoom in on the map.
   //-----------------------------------------------
   void gmsZoomIn
           (double theAmount);


   //-----------------------------------------------
   // gmsZoomOut
   //
   // Description:
   //    Allows the client to pan out on the current
   //    map.
   //-----------------------------------------------
   void gmsZoomOut
           (double theAmount);


   //-----------------------------------------------
   // gmsSet_X_Rotation
   //
   // Description:
   //    Sets the rotation factor around the X axis
   //    for spherical and ellipsoid maps.
   //-----------------------------------------------
   void gmsSet_X_Rotation
           (double xRotation);


   //-----------------------------------------------
   // gmsSet_Y_Rotation
   //
   // Description:
   //    Sets the rotation factor around the Y axis
   //    for spherical and ellipsoid maps.
   //-----------------------------------------------
   void gmsSet_Y_Rotation
           (double yRotation);


   //-----------------------------------------------
   // gmsSet_Z_Rotation
   //
   // Description:
   //    Sets the rotation factor around the Z axis
   //    for spherical and ellipsoid maps.
   //-----------------------------------------------
   void gmsSet_Z_Rotation
           (double zRotation);


   //-----------------------------------------------
   // gmsResetToNeutralOrientation
   //
   // Description:
   //    Resets rotation values to neutral, i.e. the
   //    start state.
   //-----------------------------------------------
   void gmsResetToNeutralOrientation ();


   //-----------------------------------------------
   // gmsResetToDefaultMapState
   //
   // Description:
   //    Resets all attributes of the state of the
   //    map(s) back to their start state.
   //-----------------------------------------------
   void gmsResetToDefaultMapState
           (int screenWidth,
            int screenHeight);


   //-----------------------------------------------
   // gmsMoveFlatMap
   //
   // Description:
   //    Allows the client to re-set the center
   //    point of the flat map(s).
   //-----------------------------------------------
   void gmsMoveFlatMap
             (gmsFlatMapMoveType whichDirection,
              const float        byDegrees = 0.0);


   //-----------------------------------------------
   // gmsSetCenterOfFlatMap
   //
   // Description:
   //    Allows the client to set the center
   //    point of the flat map(s).
   //-----------------------------------------------
   void gmsSetCenterOfFlatMap
             (twoDimCoordType newCenter);


   //-----------------------------------------------
   // gmsDebugPrintMapState
   //
   // Description:
   //    Debug routine that prints the current map
   //    state to the debug text file.
   //-----------------------------------------------
   void gmsDebugPrintMapState ();

#endif
