//-----------------------------------------------------------------------------
// File : gmsMapStateMgr.cpp
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

#include <gmsTypesAndConstants.h>
#include <gmsScreenCoordUtils.h>
#include <gmsMapStateMgr.h>
    //-----------//
#include <stdio.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real map state object
//---------------------------------

static const gmsMapStateStructType Default_State =
                   {0.0,      // x rotation
                    0.0,      // y rotation
                    0.0,      // z rotation
                    10000.0,  // zoom factor
                    4.0,      // minEllipsoidZoom
                    1,        // width
                    1,        // height
                    1024,     // z translation
                    1200,     // operator distance
                    {0.0f, 0.0f} }; // flatMapCenter

static gmsMapStateStructType theMapState = Default_State;

static char msg[64];




//-----------------------------------------------
// gmsGetMapState
//
// Description:
//-----------------------------------------------
gmsMapStateStructType gmsGetMapState ()

{
   return theMapState;
}


//-----------------------------------------------
// gmsSetWindowDimensions
//
// Description:
//-----------------------------------------------
void gmsSetWindowDimensions
        (int screenWidth,
         int screenHeight)

{
   theMapState.screenWidth = screenWidth;

   theMapState.screenHeight = screenHeight;
}


//-----------------------------------------------
// gmsSetMapZoomFactor
//
// Description:
//-----------------------------------------------
void gmsSetMapZoomFactor
              (double newFactor)

{
   theMapState.zoomFactor = newFactor;

   sprintf(msg, "New Zoom Factor : %.2lf\n", theMapState.zoomFactor);
   gmsWriteDebug(msg);
}


//-----------------------------------------------
// gmsZoomIn
//
// Description:
//-----------------------------------------------
void gmsZoomIn
        (double theAmount)

{
   theMapState.zoomFactor = theMapState.zoomFactor - theAmount;

   if (theMapState.zoomFactor < 1.0)
      theMapState.zoomFactor = 1.0;

   sprintf(msg, "Zoom In  - Factor = %.2lf\n", theMapState.zoomFactor);
   gmsWriteDebug(msg);
}


//-----------------------------------------------
// gmsZoomOut
//
// Description:
//-----------------------------------------------
void gmsZoomOut
        (double theAmount)

{
   theMapState.zoomFactor = theMapState.zoomFactor + theAmount;

   sprintf(msg, "Zoom Out - Factor = %.2lf\n", theMapState.zoomFactor);
   gmsWriteDebug(msg);
}


//-----------------------------------------------
// keepDegreesNormal
//
// Description:
//-----------------------------------------------
static void keepDegreesNormal
               (double &theDeg)

{
   if (theDeg >= 360.0)
      {
       theDeg = theDeg - 360.0;
      }

   if (theDeg <= -360.0)
      {
       theDeg = theDeg + 360.0;
      }
}


//-----------------------------------------------
// gmsSet_X_Rotation
//
// Description:
//-----------------------------------------------
void gmsSet_X_Rotation
           (double xRotation)

{
   theMapState.angle_X_Rotation =
      theMapState.angle_X_Rotation + xRotation;

   keepDegreesNormal (theMapState.angle_X_Rotation);
}


//-----------------------------------------------
// gmsSet_X_Rotation
//
// Description:
//-----------------------------------------------
void gmsSet_Y_Rotation
           (double yRotation)

{
   theMapState.angle_Y_Rotation =
      theMapState.angle_Y_Rotation + yRotation;

   keepDegreesNormal (theMapState.angle_X_Rotation);
}


//-----------------------------------------------
// gmsSet_Z_Rotation
//
// Description:
//-----------------------------------------------
void gmsSet_Z_Rotation
           (double zRotation)

{
   theMapState.angle_Z_Rotation =
      theMapState.angle_Z_Rotation + zRotation;

   keepDegreesNormal (theMapState.angle_Z_Rotation);
}


//-----------------------------------------------
// gmsResetToNeutralOrientation
//
// Description:
//-----------------------------------------------
void gmsResetToNeutralOrientation ()

{
   theMapState.angle_X_Rotation = 0.0;

   theMapState.angle_Y_Rotation = 0.0;

   theMapState.angle_Z_Rotation = 0.0;

   theMapState.flatMapCenter.Lat = 0.0f;

   theMapState.flatMapCenter.Long = 0.0f;
}


//-----------------------------------------------
// gmsResetToDefaultMapState
//
// Description:
//-----------------------------------------------
void gmsResetToDefaultMapState
           (int screenWidth,
            int screenHeight)

{
   theMapState = Default_State;

   theMapState.screenWidth = screenWidth;

   theMapState.screenHeight = screenHeight;
}


//-----------------------------------------------
// gmsMoveFlatMap
//
// Description:
//    Allows the client to re-set the center
//    point of the flat map(s).
//-----------------------------------------------
void gmsMoveFlatMap
             (gmsFlatMapMoveType whichDirection,
              const float        byDegrees)

{
            const float percentChange = 0.1f;
            float       deltaLong = byDegrees;
            float       deltaLat  = byDegrees;

   if (byDegrees <= 0.0) // i.e. the caller did not say by how much
      {
       deltaLat = 90.0f * percentChange;

       deltaLong = 180.0f * percentChange;
      }

   if (whichDirection == gmsMoveNorth)
      {
       theMapState.flatMapCenter.Lat =
           theMapState.flatMapCenter.Lat - deltaLat;
      }

   else if (whichDirection == gmsMoveSouth)
      {
       theMapState.flatMapCenter.Lat =
           theMapState.flatMapCenter.Lat + deltaLat;
      }

   else if (whichDirection == gmsMoveEast)
      {
       theMapState.flatMapCenter.Long =
           theMapState.flatMapCenter.Long - deltaLong;
      }

   else // gmsMoveSouth
      {
       theMapState.flatMapCenter.Long =
           theMapState.flatMapCenter.Long + deltaLong;
      }
}


//-----------------------------------------------
// gmsSetCenterOfFlatMap
//
// Description:
//    Allows the client to set the center
//    point of the flat map(s).
//-----------------------------------------------
void gmsSetCenterOfFlatMap
             (twoDimCoordType newCenter)

{
   theMapState.flatMapCenter = newCenter;

   theMapState.angle_X_Rotation = newCenter.Long;

   theMapState.angle_Y_Rotation = newCenter.Lat;
}


//-----------------------------------------------
// gmsDebugPrintMapState
//
// Description:
//    Debug routine that prints the current map
//    state to the debug text file.
//-----------------------------------------------
void gmsDebugPrintMapState ()

{
         char msg[128];

   gmsWriteDebug("______________________\n");
   gmsWriteDebug("Current Map State:\n");

   sprintf(msg, "angle_X_Rotation  : %.2lf\n",
           theMapState.angle_X_Rotation); gmsWriteDebug(msg);

   sprintf(msg, "angle_Y_Rotation  : %.2lf\n",
           theMapState.angle_Y_Rotation); gmsWriteDebug(msg);

   sprintf(msg, "angle_Z_Rotation  : %.2lf\n",
           theMapState.angle_Z_Rotation); gmsWriteDebug(msg);

   sprintf(msg, "zoomFactor        : %.2lf\n",
           theMapState.zoomFactor); gmsWriteDebug(msg);

   sprintf(msg, "Min Ellipsoid Zoom: %.2lf\n",
           theMapState.minEellipsoidZoom); gmsWriteDebug(msg);

   sprintf(msg, "screenWidth       : %d\n",
           theMapState.screenWidth); gmsWriteDebug(msg);

   sprintf(msg, "screenHeight      : %d\n",
           theMapState.screenHeight); gmsWriteDebug(msg);

   sprintf(msg, "zTranslation      : %d\n",
           theMapState.zTranslation); gmsWriteDebug(msg);

   sprintf(msg, "operatorDistance  : %d\n",
           theMapState.operatorDistance); gmsWriteDebug(msg);

   sprintf(msg, "flatMapCenter     : (%.2f, %.2f) (long, lat)\n",
           theMapState.flatMapCenter.Long,
           theMapState.flatMapCenter.Lat); gmsWriteDebug(msg);

   gmsWriteDebug("______________________\n");
}
