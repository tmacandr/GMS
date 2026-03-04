//---------------------------------------------------------------------------*/
// File: gmsDcwUtilities.cpp
// Date: 01-Mar-98 : initial definition (from CompSci 465)
//       30-Aug-99 : use in GMS project
//       14-Dec-99 : convert to DCW graphics utilities
//       11-Jan-00 : moved file utils to 'gmsFileNameUtilities'
//       22-Jan-00 : clean up the 'allocate/free' utilities.
//       04-Feb-00 : added utils to get RNG and FAC tbls from EDG name
//
// Description:
//    Defines utilites to assist the GMS "Chart Graphics" toolkit.
//---------------------------------------------------------------------------*/

#include <string.h>
#include <math.h>  // this must come first ... or the compiler blows */
#include <gmsDcwUtilities.h>
#include <gmsUtilities.h>
   //---------*/
#include <gmsDebugUtil.h>


//------------------------*/
// Constants
//------------------------*/


//------------------------*/
// Local Functions
//------------------------*/




//-----------------------------------------------------*/
// gmsAllocate_2D_FlatWorldImageFromEdgeTable
//
// Description:
//    Allocates memory for a 2D image in world coordinates
//    based on the specified "edge-table".  Each Lat/Long
//    pair from the edge table is used to define a 2D
//    world coordinate.
//
//    Client should call "gmsFree_2D_FlatWorldImage" (see
//    below) when finished with the image.  This function
//    uses the 'new' operator.
//-----------------------------------------------------*/
gms_2D_WorldImageType gmsAllocate_2D_FlatWorldImageFromEdgeTable
                             (edgeTableType edgTbl)

{
         gms_2D_WorldImageType new_2D_WorldImage = { 0, NULL };
         int                   totalNumPoints = 0;
         int                   i, index;
         gms_2D_WorldPointType *arrayOfPoints;

   if ( (edgTbl.numRecords == 0) || (edgTbl.edgeRecords == NULL) )
      {
       gmsWriteDebug ("ERR :gmsAllocate_2D_FlatWorldImageFromEdgeTable\n");

       return new_2D_WorldImage;
      }

   new_2D_WorldImage.numLines = edgTbl.numRecords;

   new_2D_WorldImage.imageLines =
      new gms_2D_WorldPolylineType[edgTbl.numRecords];

   for (i = 0; i < edgTbl.numRecords; i++)
      {
       totalNumPoints = totalNumPoints + edgTbl.edgeRecords[i].numCoords;
      }

   arrayOfPoints = new gms_2D_WorldPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_WorldPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_2D_WorldImage.numLines; i++)
      {
       new_2D_WorldImage.imageLines[i].numPoints =
             edgTbl.edgeRecords[i].numCoords;

       new_2D_WorldImage.imageLines[i].points = &arrayOfPoints[index];

       index = index + edgTbl.edgeRecords[i].numCoords;
      }

   return new_2D_WorldImage;
}


//-----------------------------------------------------*/
// gmsAllocate_2D_FlatWorldImageFrom_3D_WorldImage
//
// Description:
//    Allocates memory for a 2D image in world coordinates
//    based on the specified 3D world image.  The number
//    of lines and the number of points per line will
//    be the same between the two images, it's just that
//    the z dimension will be lost.
//
//    Client should call "gmsFree_2D_FlatWorldImage" (see
//    below) when finished with the image.  This function
//    uses the 'new' operator.
//-----------------------------------------------------*/
gms_2D_WorldImageType gmsAllocate_2D_FlatWorldImageFrom_3D_WorldImage
                             (gms_3D_WorldImageType worldImage_3D)

{
         gms_2D_WorldImageType new_2D_WorldImage = { 0, NULL };
         int                   totalNumPoints = 0;
         int                   i, index;
         gms_2D_WorldPointType *arrayOfPoints;

   if ( (worldImage_3D.numLines == 0) || (worldImage_3D.imageLines == NULL) )
      {
       gmsWriteDebug ("ERR gmsAllocate_2D_FlatWorldImageFrom_3D_WorldImage\n");

       return new_2D_WorldImage;
      }

   new_2D_WorldImage.numLines = worldImage_3D.numLines;

   new_2D_WorldImage.imageLines =
      new gms_2D_WorldPolylineType[worldImage_3D.numLines];

   for (i = 0; i < worldImage_3D.numLines; i++)
      {
       totalNumPoints =
          totalNumPoints + worldImage_3D.imageLines[i].numPoints;
      }

   arrayOfPoints = new gms_2D_WorldPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_WorldPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_2D_WorldImage.numLines; i++)
      {
       new_2D_WorldImage.imageLines[i].numPoints =
             worldImage_3D.imageLines[i].numPoints;

       new_2D_WorldImage.imageLines[i].points = &arrayOfPoints[index];

       index = index + worldImage_3D.imageLines[i].numPoints;
      }

   return new_2D_WorldImage;
}


//-----------------------------------------------------*/
// gmsFree_2D_FlatWorldImage
//
// Description:
//    Deallocates memory used for a 2D image in world
//    coordinates.
//-----------------------------------------------------*/
void gmsFree_2D_WorldImage
           (gms_2D_WorldImageType *theImage)

{
   if (theImage->imageLines != NULL)
      {
       if (theImage->imageLines[0].points != NULL)
          delete [] theImage->imageLines[0].points;

       delete [] theImage->imageLines;

       theImage->imageLines = NULL;
      }

   theImage->numLines = 0;
}


//-----------------------------------------------------*/
// gmsAllocate_2D_ScreenImageFromEdgeTable
//
// Description:
//    Allocates memory for a 2D image based on the
//    structure of the specified "edge-table".
//
//    Client should call "gmsFree_2D_ScreenImage" (see
//    below) when finished with the image.
//-----------------------------------------------------*/
gms_2D_ScreenImageType gmsAllocate_2D_ScreenImageFromEdgeTable
                             (edgeTableType edgTbl)

{
         gms_2D_ScreenImageType new_2D_Image = { 0, NULL };
         int                    totalNumPoints = 0;
         int                    i, index;
         gms_2D_ScreenPointType *arrayOfPoints;

   if ( (edgTbl.numRecords == 0) || (edgTbl.edgeRecords == NULL) )
      {
       gmsWriteDebug ("* ERROR : allocate_2D_ScreenImage : empty edg tbl\n");

       return new_2D_Image;
      }

   //
   // Allocate a table of 'polyline' records
   //
   new_2D_Image.numLines = edgTbl.numRecords;

   new_2D_Image.imageLines =
      new gms_2D_ScreenPolylineType[edgTbl.numRecords];

   //
   // Now allocate the points
   //
   for (i = 0; i < edgTbl.numRecords; i++)
      {
       totalNumPoints = totalNumPoints + edgTbl.edgeRecords[i].numCoords;
      }

   arrayOfPoints = new gms_2D_ScreenPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_2D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_2D_Image.numLines; i++)
      {
       new_2D_Image.imageLines[i].numPoints =
             edgTbl.edgeRecords[i].numCoords;

       new_2D_Image.imageLines[i].points = &arrayOfPoints[index];

       index = index + edgTbl.edgeRecords[i].numCoords;
      }

   return new_2D_Image;
}


//-----------------------------------------------------*/
// gmsFree_2D_ScreenImage
//
// Description:
//    Deallocates memory used for a 2D image.
//-----------------------------------------------------*/
void gmsFree_2D_ScreenImage
           (gms_2D_ScreenImageType *theImage)

{
   if (theImage->imageLines != NULL)
      {
       if (theImage->imageLines[0].points != NULL)
          delete [] theImage->imageLines[0].points;

       delete [] theImage->imageLines;

       theImage->imageLines = NULL;
      }

   theImage->numLines = 0;
}


//-----------------------------------------------------*/
// gmsAllocate_3D_WorldImageFromEdgeTable
//
// Description:
//    Allocates memory for a 3D image in world coordinates
//    based on the specified "edge-table".
//
//    Client should call "gmsFree_3D_WorldImage" (see
//    below) when finished with the image.
//-----------------------------------------------------*/
gms_3D_WorldImageType gmsAllocate_3D_WorldImageFromEdgeTable
                             (edgeTableType edgTbl)

{
         gms_3D_WorldImageType  new_3D_WorldImage = { 0, NULL };
         int                    totalNumPoints = 0;
         int                    i, index;
         gms_3D_WorldPointType  *arrayOfPoints;

   if ( (edgTbl.numRecords == 0) || (edgTbl.edgeRecords == NULL) )
      {
       gmsWriteDebug ("*** ERROR : allocate_3D_WorldImage : empty edg tbl\n");

       return new_3D_WorldImage;
      }

   //
   // Allocate a table of 'polyline' records
   //
   new_3D_WorldImage.numLines = edgTbl.numRecords;

   new_3D_WorldImage.imageLines =
      new gms_3D_WorldPolylineType[edgTbl.numRecords];

   //
   // Now allocate the points
   //
   for (i = 0; i < edgTbl.numRecords; i++)
      {
       totalNumPoints = totalNumPoints + edgTbl.edgeRecords[i].numCoords;
      }

   arrayOfPoints = new gms_3D_WorldPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfPoints,
        (sizeof(gms_3D_WorldPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_3D_WorldImage.numLines; i++)
      {
       new_3D_WorldImage.imageLines[i].numPoints =
             edgTbl.edgeRecords[i].numCoords;

       new_3D_WorldImage.imageLines[i].points = &arrayOfPoints[index];

       index = index + edgTbl.edgeRecords[i].numCoords;
      }

   return new_3D_WorldImage;
}


//-----------------------------------------------------*/
// gmsFree_3D_WorldImage
//
// Description:
//    Deallocates memory used for a 3D image in world
//    coordinates.
//-----------------------------------------------------*/
void gmsFree_3D_WorldImage
           (gms_3D_WorldImageType *theImage)

{
   if (theImage->imageLines != NULL)
      {
       if (theImage->imageLines[0].points != NULL)
          delete [] theImage->imageLines[0].points;

       delete [] theImage->imageLines;

       theImage->imageLines = NULL;
      }

   theImage->numLines = 0;
}


//-----------------------------------------------------*/
// gmsAllocate_3D_ScreenImageFromWorldImage 
//
// Description:
//    Allocates memory for a 3D image in screen coordinates
//    based on the specified "3D world image".
//
//    Client should call "gmsFree_3D_ScreenImage" (see
//    below) when finished with the image.
//-----------------------------------------------------*/
gms_3D_ScreenImageType gmsAllocate_3D_ScreenImageFromEdgeTable
                             (gms_3D_WorldImageType worldImg)

{
         gms_3D_ScreenImageType  new_3D_ScreenImage = { 0, NULL };
         int                     totalNumPoints = 0;
         int                     i, index;
         gms_3D_ScreenPointType  *arrayOfScreenPoints;

   if ( (worldImg.numLines == 0) || (worldImg.imageLines == NULL) )
      return new_3D_ScreenImage;

   new_3D_ScreenImage.numLines = worldImg.numLines;

   new_3D_ScreenImage.imageLines =
      new gms_3D_ScreenPolylineType[worldImg.numLines];

   for (i = 0; i < worldImg.numLines; i++)
      {
       totalNumPoints = totalNumPoints + worldImg.imageLines[i].numPoints;
      }

   arrayOfScreenPoints = new gms_3D_ScreenPointType[totalNumPoints];

   gmsClearMemory
      ( (char *) arrayOfScreenPoints,
        (sizeof(gms_3D_ScreenPointType) * totalNumPoints) );

   index = 0;

   for (i = 0; i < new_3D_ScreenImage.numLines; i++)
      {
       new_3D_ScreenImage.imageLines[i].numPoints =
             worldImg.imageLines[i].numPoints;

       new_3D_ScreenImage.imageLines[i].points =
             &arrayOfScreenPoints[index];

       index = index + worldImg.imageLines[i].numPoints;
      }

   return new_3D_ScreenImage;
}


//-----------------------------------------------------*/
// gmsFree_3D_ScreenImage
//
// Description:
//    Deallocates memory used for a 3D image in screen
//    coordinates.
//-----------------------------------------------------*/
void gmsFree_3D_ScreenImage
           (gms_3D_ScreenImageType *theImage)

{
   if (theImage->imageLines != NULL)
      {
       if (theImage->imageLines[0].points != NULL)
          delete [] theImage->imageLines[0].points;

       delete [] theImage->imageLines;

       theImage->imageLines = NULL;
      }

   theImage->numLines = 0;
}


//-----------------------------------------------------*/
// gmsGetPtrToRingTable
//
// Description:
//    This utility uses the given NAME of an edge table
//    to determine the name of the corresponding RING
//    table.  Then, the ring table is retrieved and a
//    pointer to that ring table is returned to the
//    caller.
//-----------------------------------------------------*/
ringTableType *gmsGetPtrToRingTable
                         (const char *edgTblName)

{
         int           length;
         char          *rngTblName;
         ringTableType *ptrToRngTbl;

   if (edgTblName == NULL)
      return (ringTableType *) NULL;

   length = strlen(edgTblName);

   rngTblName = new char[length + 4];

   sprintf(rngTblName, "%s", edgTblName);

   rngTblName[length - 3] = 'R';
   rngTblName[length - 2] = 'N';
   rngTblName[length - 1] = 'G';

   //{
   //       char msg[96];

   // sprintf(msg, "---> Ring Tbl : %s\n", rngTblName);

   // gmsWriteDebug(msg);
   //}

   ptrToRngTbl = gmsGetRingTable (rngTblName);

   delete [] rngTblName;

   return ptrToRngTbl;
}


//-----------------------------------------------------*/
// gmsGetPtrToFaceTable
//
// Description:
//    This utility uses the given NAME of an edge table
//    to determine the name of the corresponding FACE
//    table.  Then, the face table is retrieved and a
//    pointer to that face table is returned to the
//    caller.
//-----------------------------------------------------*/
faceTableType *gmsGetPtrToFaceTable
                         (const char *edgTblName)

{
         int           length;
         char          *facTblName;
         faceTableType *ptrToFacTbl;

   if (edgTblName == NULL)
      return (faceTableType *) NULL;

   length = strlen(edgTblName);

   facTblName = new char[length + 4];

   sprintf(facTblName, "%s", edgTblName);

   facTblName[length - 3] = 'F';
   facTblName[length - 2] = 'A';
   facTblName[length - 1] = 'C';

   //{
   //       char msg[96];

   // sprintf(msg, "---> Face Tbl : %s\n", facTblName);

   // gmsWriteDebug(msg);
   //}

   ptrToFacTbl = gmsGetFaceTable (facTblName);

   delete [] facTblName;

   return ptrToFacTbl;
}


//-----------------------------------------------------*/
// gmsDraw_2D_Circle
//
// Description:
//    Left-over from CompSci 465.  This function calculates
//    the points of a circle using the "Bresenham-Michener"
//    algorithm.  Caller specifies the center point of
//    the circle and its fltRadius.
//
//    Caller is responsible for "freeing" the points of
//    the image when done.
//-----------------------------------------------------*/
gms_2D_ScreenImageType gmsDraw_2D_Circle
                             (int center_x,
                              int center_y,
                              int radius)

{
         const float radiusSquared = (float) (radius * radius);
         int         tempX, tempY;
         int         loopLimit;
         int         octant_8_X, octant_8_Y;
         int         value_x, value_y;
         int         distance_S, distance_T;
         float       epsilon_S, epsilon_T;
         int         modX, modY;
         const int   numOctants = 8;
         int         i, numPoints = 0;
         gms_2D_ScreenImageType theCircle = {0, NULL};

   loopLimit = center_x + (int) (0.71 * radius);

   theCircle.numLines = numOctants; // one polyline for each octant */

   theCircle.imageLines = new gms_2D_ScreenPolylineType[numOctants];

   for (i = 0; i < numOctants; i++)
      {
       theCircle.imageLines[i].numPoints = 0;

       theCircle.imageLines[i].points = new gms_2D_ScreenPointType[radius];
      }

   tempX = center_x;
   tempY = center_y + radius;

   modX = 2 * center_x;
   modY = 2 * center_y;

   octant_8_X = center_x + radius;
   octant_8_Y = center_y;

   numPoints = 0;

   while (tempX <= loopLimit)
      {
       //---------------------------*/
       // Draw octant VII of circle
       //---------------------------*/
       theCircle.imageLines[7 - 1].points[numPoints].x = tempX;
       theCircle.imageLines[7 - 1].points[numPoints].y = tempY;

       //---------------------------*/
       // Draw octant VIII of circle
       //---------------------------*/
       theCircle.imageLines[8 - 1].points[numPoints].x = octant_8_X;
       theCircle.imageLines[8 - 1].points[numPoints].y = octant_8_Y;

       //---------------------------*/
       // Draw quadrant II of circle
       //---------------------------*/
       theCircle.imageLines[2 - 1].points[numPoints].x = tempX;
       theCircle.imageLines[2 - 1].points[numPoints].y = (modY - tempY);

       //---------------------------*/
       // Draw quadrant I of circle
       //---------------------------*/
       theCircle.imageLines[1 - 1].points[numPoints].x = octant_8_X;
       theCircle.imageLines[1 - 1].points[numPoints].y = (modY - octant_8_Y);

       //---------------------------*/
       // Draw quadrant VI of circle
       //---------------------------*/
       theCircle.imageLines[6 - 1].points[numPoints].x = (modX - tempX);
       theCircle.imageLines[6 - 1].points[numPoints].y = tempY;

       //---------------------------*/
       // Draw quadrant V of circle
       //---------------------------*/
       theCircle.imageLines[5 - 1].points[numPoints].x = (modX - octant_8_X);
       theCircle.imageLines[5 - 1].points[numPoints].y = octant_8_Y;

       //---------------------------*/
       // Draw quadrant III of circle
       //---------------------------*/
       theCircle.imageLines[3 - 1].points[numPoints].x = (modX - tempX);
       theCircle.imageLines[3 - 1].points[numPoints].y = (modY - tempY);

       //---------------------------*/
       // Draw quadrant IV of circle
       //---------------------------*/
       theCircle.imageLines[4 - 1].points[numPoints].x = (modX - octant_8_X);
       theCircle.imageLines[4 - 1].points[numPoints].y = (modY - octant_8_Y);

       value_x = tempX + 1 - center_x;

       value_x = value_x * value_x;

       value_y = tempY - 1 - center_y;

       value_y = value_y * value_y;

       distance_S = value_x + value_y;

       value_y = tempY - center_y;

       value_y = value_y * value_y;

       distance_T = value_x + value_y;

       epsilon_S = radiusSquared - distance_S;

       if (epsilon_S < 0.0)
          epsilon_S = - epsilon_S;

       epsilon_T = radiusSquared - distance_T;

       if (epsilon_T < 0.0)
          epsilon_T = - epsilon_T;

       tempX++;

       octant_8_Y++;

       if (epsilon_S < epsilon_T)
          {
           tempY--;

           octant_8_X--;
          }

       numPoints++;
      }

   for (i = 0; i < numOctants; i++)
      {
       theCircle.imageLines[i].numPoints = numPoints;
      }

   return theCircle;
}


           //------------------------*/
           //    Local Functions
           //------------------------*/



