//-----------------------------------------------------------------------------
// File : gmsMbrClass.h
// Date : 10-Dec-99 : initial definition
//        18-Jan-00 : implement ellipsoid earth model
//
// Description:
//    This class defines a "Minimum Bounding Rectangle" object.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_MBR_CLASS

   #define GMS_MBR_CLASS

   #include <gmsTypesAndConstants.h>

   class gmsMbrClass
      {
       public:

          //-----------------------------------------------
          // gmsMbrClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents a minimum bounding rectangle
          //    object.  The MBR to be constructed is
          //    specified by the "mbrTblName".  The width
          //    and height parameters are used to set the
          //    center point of the image.  The parameter
          //    "whichModel" indicates flat, sphere, or
          //    ellipsoid model.
          //-----------------------------------------------
          gmsMbrClass
             (gmsEarthModelType whichModel,
              const char        *mbrTblName);


          //-----------------------------------------------
          // ~gmsMbrClass
          //
          // Description:
          //    Destructor.  Deletes the object.
          //-----------------------------------------------
          ~gmsMbrClass ();


          //-----------------------------------------------
          // gmsGetMbrImage
          //
          // Description:
          //    Returns the 2-D image of the MBR in screen
          //    coordinates.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetMbrImage ();

       private:

          void              *theMbr;

          gmsEarthModelType earthModel;
      };

#endif
