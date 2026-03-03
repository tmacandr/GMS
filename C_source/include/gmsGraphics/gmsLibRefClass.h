//-----------------------------------------------------------------------------
// File : gmsLibRefClass.h
// Date : 08-Jan-00 : initial definition
//        20-Jan-00 : drop use of 'gmsFlatMapClass'
//        05-Feb-00 : write more "description"
//        12-Feb-00 : rename private var to prevent collision with parameter
//
// Description:
//    This class defines a "Library Reference" object.  The LibRef theme is
//    unique in that it contains high level data for a whole region (i.e. one
//    of the DCW regions NOAMER, SOAMAFR, EURNASIA, SASAUS).  The caller need
//    only specify the earth model to be used.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_LIBREF_CLASS

   #define GMS_LIBREF_CLASS

   #include <gmsTypesAndConstants.h>
   #include <gmsMapClass.h>

   class gmsLibRefClass
      {
       public:

          //-----------------------------------------------
          // gmsLibRefClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents a library reference
          //    object.
          //-----------------------------------------------
          gmsLibRefClass
             (gmsEarthModelType whichModel);


          //-----------------------------------------------
          // ~gmsLibRefClass
          //
          // Description:
          //    Destructor.  Deletes the object.
          //-----------------------------------------------
          ~gmsLibRefClass ();


          //-----------------------------------------------
          // gmsGetLibRefImage
          //
          // Description:
          //    Returns the 2-D image of the edge table from
          //    Lib Reference.  The image is in screen
          //    coordinates.
          //-----------------------------------------------
          gms_2D_ScreenImageType gmsGetLibRefImage ();

       private:

          gmsMapClass       *ptrToLibRef;

          gmsEarthModelType theModel;
      };

#endif
