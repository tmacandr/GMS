//-----------------------------------------------------------------------------
// File : gmsNodeClass.h
// Date : 13-Jan-00 : initial definition
//        18-Jan-00 : implement flat-map model
//
// Description:
//    This class defines a "node" object.  As specified in the DCW documentation,
//    a node is a "zero-dimensional geometric primitive that is composed of
//    a single coordinate tuple (pair or triplet)".  The term "zero-dimensional"
//    really means the data is a "point" on a map.  For example, a town
//    or village is often small enough that it is represented as a point.
//    This class permits manipulation of "Entity Nodes" from the DCW database.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_NODE_CLASS

   #define GMS_NODE_CLASS

   #include <gmsTypesAndConstants.h>

   class gmsNodeClass
      {
       public:

          //-----------------------------------------------
          // gmsNodeClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    represents an a series of entity nodes.
          //-----------------------------------------------
          gmsNodeClass
             (gmsEarthModelType whichModel,
              const char        *nodeTblName);


          //-----------------------------------------------
          // ~gmsNodeClass
          //
          // Description:
          //    Destructor.  Deletes the node object.
          //-----------------------------------------------
          ~gmsNodeClass ();


          //-----------------------------------------------
          // gmsGetNodePoints
          //
          // Description:
          //    Returns an array of 2D points that represent
          //    the nodes.  Although the returned struct
          //    is a 'polyline', it actually represents
          //    a series of independent points.
          //-----------------------------------------------
          gms_2D_ScreenPolylineType gmsGetNodePoints ();

       private:

          void              *theNode;

          gmsEarthModelType theModel;

      };

#endif
