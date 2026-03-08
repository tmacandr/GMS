/*----------------------------------------------------------------------------
 * File : util_test.c
 * Date : 15-Dec-99 : initial defintion
 *        21-Dec-99 : subtest to count DCW files
 *
 * Description:
 *    Console application to exercise the "gmsDcwUtilities" component.
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsTileRefClass.h>
#include <gmsFileNameUtilities.h>
#include <gmsDcwUtilities.h>
    //---------
#include <gmsEdgeTable.h>
#include <gmsWorldCoordUtils.h>


static const int Num_Tests = 4;

static int selectTest ();

static void tileIdTest ();

static void dcwFileCountTest ();

static void coordConversionTest ();

static void worldCoordTest ();


/*---------------------------------------------
 * Main - test driver
 *
 * Description:
 *---------------------------------------------*/
int main (int  argc,
          char *argv)

{
         int whichTest;

   printf("_______________________________________\n");
   printf("Begin : DCW Utilities Test\n");

   do
      {
       whichTest = selectTest ();

       if (whichTest == 0)
          tileIdTest ();

       else if (whichTest == 1)
          dcwFileCountTest ();

       else if (whichTest == 2)
          coordConversionTest ();

       else if (whichTest == 3)
          worldCoordTest ();
      }
   while ( (whichTest >= 0) && (whichTest < Num_Tests));

   printf("End   : DCW Utilities Test\n");
   printf("_______________________________________\n");

   return 0;
}


//---------------------------------------------*/
// selectTest
//
// Description:
//---------------------------------------------*/
static int selectTest ()

{
         int               i;
         int               theSelection;
         static const char *namesOfTests[Num_Tests] =
                              {
                                {"Tile-Id Test"},
                                {"DCW file count Test"},
                                {"Coord Conversion Test"},
                                {"World Coordinate Test"},
                              };

   printf("---> Select Sub-test to perform:\n");

   for (i = 0; i < Num_Tests; i++)
      {
       printf("%d) %s\n", i, namesOfTests[i]);
      }

   printf("---> Your selection: ");
   scanf("%d", &theSelection);
   printf("\n");

   return theSelection;
}


//---------------------------------------------*/
// tileIdTest
//
// Description:
//    Exercises the 'gmsConvertTileIdToName'
//    utility.
//---------------------------------------------*/
static void tileIdTest ()

{
         char            *tileName;
         int             i;
         gmsTileRefClass *ptrToTileRef;
         const int       Max_Tile_Id = 780;

   printf("______________\n");
   printf("Begin : 'tile-id to name' sub-test\n");

   ptrToTileRef = new gmsTileRefClass (gmsFlat);

   for (i = 0; i < Max_Tile_Id; i++)
      {
       tileName = ptrToTileRef->gmsConvertTileIdToName (i);

       printf("----------------\n");
       printf("---> Tile-id = %d : %s\n", i, tileName);
      }

   ptrToTileRef->~gmsTileRefClass ();

   printf("Begin : 'tile-id to name' sub-test\n");
   printf("______________\n");
}


//---------------------------------------------*/
// dcwFileCountTest
//
// Description:
//---------------------------------------------*/
static void dcwFileCountTest ()

{
            gmsThematicType i;
            gmsDcwFileType  j;
            int             numFiles;

   printf("______________\n");
   printf("Begin : 'DCW file count' sub-test\n");

   for (i = gms_AE; i <= gms_LibRef; i = (gmsThematicType) (i + 1))
      {
       printf("_________________\n");

       for (j = gmsEdgeTable;
            j <= gmsNodeSpatialIndex;
            j = (gmsDcwFileType) (j + 1))
          {
           numFiles = gmsCountNumberOfThemeFiles (i, j);

           printf("---> Number of %s files in %s theme = %d\n",
                  gmsGetRegionalFileName (i, j),
                  gmsGetThemeDirName(i),
                  numFiles);
          }

       for (j = gmsAreaFeatureTable;
            j <= gmsIntVDT;
            j = (gmsDcwFileType) (j + 1))
          {
           numFiles = gmsCountNumberOfThemeFiles (i, j);

           printf("---> Number of %s files in %s theme = %d\n",
                  gmsGetRegionalFileName (i, j),
                  gmsGetThemeDirName(i),
                  numFiles);
          }
      }

   printf("End   : 'DCW file count' sub-test\n");
   printf("______________\n");

}


//---------------------------------------------*/
// coordConversionTest
//
// Description:
//---------------------------------------------*/
static void coordConversionTest ()

{
         edgeTableType         *edgTbl;
         int                   i, j;
         twoDimCoordType       latLong_in, latLong_out;
         gms_3D_WorldPointType worldPoint_3D;
         float                 latDiff, longDiff;
         const float           tolerance = 0.000001f;

   printf("__________________________________\n");
   printf("Begin : Coordinate Conversion Test\n");

   edgTbl = gmsGetEdgeTable (gmsGetDcwFullPath("po/n/g/22/edg"));

   for (i = 0; i < edgTbl->numRecords; i++)
      for (j = 0; j < edgTbl->edgeRecords[i].numCoords; j++)
         {
          latLong_in = edgTbl->edgeRecords[i].theCoords[j];

          worldPoint_3D = gmsConvertLatLongToEllipsoidPoint (latLong_in);

          latLong_out = gmsConvertEllipsoidPointToLatLong (worldPoint_3D);

          latDiff = latLong_out.Lat - latLong_in.Lat;

          if (latDiff < 0.0)
             latDiff = -latDiff;

          longDiff = latLong_out.Long - latLong_in.Long;

          if (longDiff < 0.0)
             longDiff = -longDiff;

          if ( (latDiff > tolerance) || (longDiff > tolerance) )
             {
              printf("\n\n---> Lat/Long values not equal - test FAILS\n");

              printf("Expected (lat, long) = (%.7f, %.7f)\n",
                     latLong_in.Lat, latLong_in.Long);

              printf("Actual (lat, long)   = (%.7f, %.7f)\n",
                     latLong_out.Lat, latLong_out.Long);

              printf("Differences : lat = %.7f  long = %.7f\n",
                     latDiff, longDiff);

              printf("_________________________\n\n");

              gmsFreeEdgeTable (edgTbl);

              return;
             }
         }

   gmsFreeEdgeTable (edgTbl);

   printf("---> All re-converted values are within tolerance\n");

   printf("End   : Coordinate Conversion Test\n");
   printf("__________________________________\n");
}


//---------------------------------------------
// worldCoordTest
//
// Description:
//---------------------------------------------
static void worldCoordTest ()

{
         int                   i;
         const int             Num_Coords = 5;
         const twoDimCoordType latLongs[Num_Coords] = { {0.0,     0.0},
                                                        {0.0,    90.0},
                                                        {0.0,   -90.0},
                                                        {90.0,    0.0},
                                                        {-90.0,   0.0} };
         gms_3D_WorldPointType worldPoint_3D;

   printf("______________\n");
   printf("Begin : 'world-coord' sub-test\n");

   for (i = 0; i < Num_Coords; i++)
     {
      worldPoint_3D = gmsConvertLatLongToEllipsoidPoint (latLongs[i]);

      printf
         ("(Long, Lat) = (%.2f, %.2f) <===> (x, y, z) = (%.2f, %.2f, %.2f) m\n",
          latLongs[i].Long, latLongs[i].Lat,
          worldPoint_3D.x_meters,
          worldPoint_3D.y_meters,
          worldPoint_3D.z_meters);
     }

   printf("Begin : 'world-coord' sub-test\n");
   printf("______________\n");
}
