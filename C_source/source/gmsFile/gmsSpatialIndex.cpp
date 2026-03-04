//----------------------------------------------------------------------------*/
// File : gmsSpatialIndex.cpp
// Date : 07-Aug-99 : initial definition
//        07-Oct-99 : Clean-up due to code-inspection
//        08-Oct-99 : Algo in Appendix F (Mil-Std-600006) is hosed - fix
//        11-Feb-00 : rename "si_fileDesc" variable re HP C++ compiler
//                    choked on it
//
// Description:
//    Utilities to "read" any "Spatial Index" file of the Digital Chart of
//    the World (DCW).
//
//    A "spatial index" file provides the capability to use a set of coord-
//    inates to identify a (geographic) primitive and its associated attributes.
//    The idea is to provide a mechanism that will reduce the search-time to
//    find geographic primitives within the DCW database.  Spatial indexes are
//    implemented for FACE, EDGE, NODE, and TEXT primitives and have the
//    following file names:
//
//               FSI - Face Spatial Index
//               ESI - Edge Spatial Index
//               NSI - Node Spatial Index
//               TSI - Text Spatial Index
//
//    Also, consider:
//
//       Spatial Index:
//       --------------
//          A data structure fiel that allows for the rapid
//          identification of a primitive by using the values
//          of the primitive's coordinates.
//
//    The "spatial index" is one of the four categories of spatial decomposition
//    of a VPF database.  The other three are: 1) tile directory, 2) the
//    minimum-bounding-rectangle (of edge/face primitives), and 3) primitive
//    coordinates.
//
//    See also, Apendix F, Mil-Std-600006, pg 139-150.
//
//    Reference:
//        1) Mil-Std-600006
//        2) Mil-D-89009
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsSpatialIndex.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------*/
//     Local Variables
//-----------------------------*/
static FILE *si_fileDesc = (FILE *) NULL;


//-----------------------------*/
// Declare Local Subprograms
//-----------------------------*/
static void buildSpatialIndex
               (spatialIndexType *theSpatialIndex);

static boundingRectangleType convertSpatialIndexValuesToLatLong
                                (unsigned char         cell_x1_as_byte,
                                 unsigned char         cell_y1_as_byte,
                                 unsigned char         cell_x2_as_byte,
                                 unsigned char         cell_y2_as_byte,
                                 boundingRectangleType outterRectangle);


//---------------------------------------------*/
// gmsGetSpatialIndex
//
// Description:
//    This utility reads the file that contains
//    the "Spatial Index".  A pointer to a newly
//    allocated table is returned to the caller.
//    It is the caller's responsibility to free
//    the item by using the utility
//    "gmsFreeSpatialIndex" (see below).
//---------------------------------------------*/
spatialIndexType *gmsGetSpatialIndex
                           (const char *spatialIndexFilePath)

{
         const char       readOnlyMode[] = { "rb" };
         spatialIndexType *theSpatialIndex;

   if (spatialIndexFilePath == NULL)
      return (spatialIndexType *) NULL;

   si_fileDesc = fopen
              (spatialIndexFilePath,
               readOnlyMode);

   if (si_fileDesc == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open Spatial Index file: %s\n",
              spatialIndexFilePath);

       return (spatialIndexType *) NULL;
      }

   theSpatialIndex =
      (spatialIndexType *) malloc (sizeof(spatialIndexType));

   gmsClearMemory
      ( (char *) theSpatialIndex,
        sizeof(spatialIndexType));

   buildSpatialIndex (theSpatialIndex);

   fclose (si_fileDesc);

   return theSpatialIndex;
}


//---------------------------------------------*/
// gmsFreeSpatialIndex
//
// Description:
//    This utility frees a "Spatial Index" that had
//    been previously allocated using
//    "gmsGetSpatialIndex".
//---------------------------------------------*/
void gmsFreeSpatialIndex
           (spatialIndexType *theSpatialIndex)

{
   if (theSpatialIndex == NULL)
      return;

   if (theSpatialIndex->nodeRecords != NULL)
      free (theSpatialIndex->nodeRecords);

   if (theSpatialIndex->primitiveRecords != NULL)
      free (theSpatialIndex->primitiveRecords);

   free (theSpatialIndex);
}


//---------------------------------------------*/
// gmsPrintSpatialIndex
//
// Description:
//    This function will print the "Spatial Index"
//    object to standard out.
//---------------------------------------------*/
void gmsPrintSpatialIndex
           (spatialIndexType *theSpatialIndex)

{
         int i;

   printf("________________________________________\n");
   printf("Spatial Table:\n");

   if (theSpatialIndex == NULL)
      return;

   printf("  Num of Primitives      : %d\n",
          theSpatialIndex->numPrimitives);
   printf("  Min Bound Rec (X1, Y1) : (%f, %f)\n",
          theSpatialIndex->outterRectangle.x1_long,
          theSpatialIndex->outterRectangle.y1_lat);
   printf("  Min Bound Rec (X2, Y2) : (%f, %f)\n",
          theSpatialIndex->outterRectangle.x2_long,
          theSpatialIndex->outterRectangle.y2_lat);
   printf("  Num of Nodes           : %d\n",
          theSpatialIndex->numNodes);

   if (theSpatialIndex->nodeRecords == NULL)
      return;

   for (i = 0; i < theSpatialIndex->numNodes; i++)
      {
       printf("   %d) Offset = %ld  -  Num_Items = %ld\n",
              i,
              theSpatialIndex->nodeRecords[i].offsetToNode,
              theSpatialIndex->nodeRecords[i].numItemsInNode);
      }

   if (theSpatialIndex->primitiveRecords == NULL)
      return;

   for (i = 0; i < theSpatialIndex->numPrimitives; i++)
      {
       printf("   %d) ID = %d - (x1, y1)/(x2, y2) = (%.3f, %.3f)/(%.3f, %.3f)\n",
              i,
              theSpatialIndex->primitiveRecords[i].primId,
              theSpatialIndex->primitiveRecords[i].spatialCell.x1_long,
              theSpatialIndex->primitiveRecords[i].spatialCell.y1_lat,
              theSpatialIndex->primitiveRecords[i].spatialCell.x2_long,
              theSpatialIndex->primitiveRecords[i].spatialCell.y2_lat);
      }

}


     //-----------------------*/
     //   Local Subprograms
     //-----------------------*/


//---------------------------------------------*/
// buildSpatialIndex
//
// Description:
//    This function will read the actual data
//    from the Spatial Index file.  The data read
//    will be used to populate the attributes of
//    the object.
//---------------------------------------------*/
static void buildSpatialIndex
               (spatialIndexType *theSpatialIndex)

{
         int           numBytes = 0;
         int           index;
         unsigned char cell_x1_as_byte;
         unsigned char cell_y1_as_byte;
         unsigned char cell_x2_as_byte;
         unsigned char cell_y2_as_byte;

   theSpatialIndex->numPrimitives = gmsReadInteger (si_fileDesc);

   theSpatialIndex->outterRectangle.x1_long = gmsReadFloat (si_fileDesc);
   theSpatialIndex->outterRectangle.y1_lat  = gmsReadFloat (si_fileDesc);
   theSpatialIndex->outterRectangle.x2_long = gmsReadFloat (si_fileDesc);
   theSpatialIndex->outterRectangle.y2_lat  = gmsReadFloat (si_fileDesc);

   theSpatialIndex->numNodes = gmsReadInteger (si_fileDesc);

   //-----------------------------*/
   // Build the array of node-data
   //-----------------------------*/
   numBytes = theSpatialIndex->numNodes * sizeof(nodeRecType);

   theSpatialIndex->nodeRecords = (nodeRecType *) malloc (numBytes);

   gmsClearMemory
      ( (char *) theSpatialIndex->nodeRecords,
        numBytes);

   for (index = 0; index < theSpatialIndex->numNodes; index++)
      {
       theSpatialIndex->nodeRecords[index].offsetToNode =
          gmsReadInteger (si_fileDesc);

       theSpatialIndex->nodeRecords[index].numItemsInNode =
          gmsReadInteger (si_fileDesc);
      }

   //----------------------------------*/
   // Build the array of primitive-data
   //----------------------------------*/
   numBytes = theSpatialIndex->numPrimitives * sizeof(primitiveRecType);

   theSpatialIndex->primitiveRecords =
      (primitiveRecType *) malloc (numBytes);

   gmsClearMemory
      ( (char *) theSpatialIndex->primitiveRecords,
        numBytes);

   for (index = 0; index < theSpatialIndex->numPrimitives; index++)
      {
       cell_x1_as_byte = (unsigned char) fgetc (si_fileDesc);

       cell_y1_as_byte = (unsigned char) fgetc (si_fileDesc);

       cell_x2_as_byte = (unsigned char) fgetc (si_fileDesc);

       cell_y2_as_byte = (unsigned char) fgetc (si_fileDesc);

       theSpatialIndex->primitiveRecords[index].primId =
          gmsReadInteger (si_fileDesc);

       theSpatialIndex->primitiveRecords[index].spatialCell =
          convertSpatialIndexValuesToLatLong
                (cell_x1_as_byte,
                 cell_y1_as_byte,
                 cell_x2_as_byte,
                 cell_y2_as_byte,
                 theSpatialIndex->outterRectangle);
      }
}


//---------------------------------------------*/
// convertSpatialIndexValuesToLatLong
//
// Description:
//    See also : Mil-Std-600006, Appendix F, pg
//    140-144.
//
//    The coordinate system for the spatial index
//    is based on 1-byte integers.  Each value
//    of a "cell" therefore ranges from 0 .. 255.
//    The value represents how much of the "bounding
//    rectangle" is covered by the cell in question.
//    The byte value is converted to a Lat/Long
//    float value as:
//
//                              CellLatYi
//      Cell Lat Yi = MinLat + ----------- * deltaLat
//                                255
//
//    and
//
//                               CellLonXi
//      Cell Lon Xi = MinLong + ---------- * deltaLong
//                                  255
//
//    Note:
//    -----
//       This algorithm is different from the
//       one in Appendix F of Mil-Std-6000006.
//       The algo in Appendix F was hosed.
//---------------------------------------------*/
static boundingRectangleType convertSpatialIndexValuesToLatLong
                                (unsigned char         cell_x1_as_byte,
                                 unsigned char         cell_y1_as_byte,
                                 unsigned char         cell_x2_as_byte,
                                 unsigned char         cell_y2_as_byte,
                                 boundingRectangleType outterRectangle)

{
         boundingRectangleType theAnswer;
         const float minLong  = outterRectangle.x1_long;
         const float minLat   = outterRectangle.y1_lat;
         const float deltaLat = outterRectangle.y2_lat - minLat;
         const float deltaLon = outterRectangle.x2_long - minLong;
         const float maxByte  = 255;

   theAnswer.x1_long = minLong + (cell_x1_as_byte / maxByte) * deltaLon;

   theAnswer.y1_lat = minLat + (cell_y1_as_byte / maxByte) * deltaLat;

   theAnswer.x2_long = minLong + (cell_x2_as_byte / maxByte) * deltaLon;

   theAnswer.y2_lat = minLat + (cell_y2_as_byte / maxByte) * deltaLat;

   return theAnswer;
}
