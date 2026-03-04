//-----------------------------------------------------------------------------
// File : gmsLibRefClass.cpp
// Date : 08-Jan-00 : initial definition
//        20-Jan-00 : drop use of 'gmsFlatMapClass'
//        12-Feb-00 : rename private var to prevent collision with parameter
//
// Description:
//    This class defines a "Library Reference" object.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <gmsLibRefClass.h>
#include <gmsEdgeTable.h>
     //------------//
#include <gmsWorldCoordUtils.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real flat map 'object'
//---------------------------------


//---------------------------------
// Declare some constants
//---------------------------------


//---------------------------------
// Declare Static Member Functions
//---------------------------------


//-----------------------------------------------
// gmsLibRefClass
//
// Description:
//    Constructor.  Creates an object that
//    represents a library reference
//    object.
//-----------------------------------------------
gmsLibRefClass::gmsLibRefClass
             (gmsEarthModelType whichModel)

{
   gmsLibRefClass::ptrToLibRef = new gmsMapClass
                                        (whichModel,
                                         gmsGetDcwFullPath("libref/edg"));

   gmsLibRefClass::theModel = whichModel;
}


//-----------------------------------------------
// ~gmsLibRefClass
//
// Description:
//    Destructor.  Deletes the object.
//-----------------------------------------------
gmsLibRefClass::~gmsLibRefClass ()

{
   if (gmsLibRefClass::ptrToLibRef != NULL)
      delete gmsLibRefClass::ptrToLibRef;
}


//-----------------------------------------------
// gmsGetLibRefImage
//
// Description:
//    Returns the 2-D image of the Lib Ref in screen
//    coordinates.
//-----------------------------------------------
gms_2D_ScreenImageType gmsLibRefClass::gmsGetLibRefImage ()

{
   return gmsLibRefClass::ptrToLibRef->gmsGetMapImage();
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


