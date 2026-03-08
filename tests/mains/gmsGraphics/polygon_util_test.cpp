/*----------------------------------------------------------------------------
 * File : polygon_util_test.c
 * Date : 21-Jan-00 : initial defintion
 *
 * Description:
 *    Console application to exercise the "gmsPolygonUtilities" component.
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <gmsPolygonUtilities.h>
#include <gmsEdgeTable.h>
#include <gmsRingTable.h>
#include <gmsUtilities.h>
#include <gmsDcwUtilities.h>

static const int Num_Files = 18;

static char *edgFiles[Num_Files] = {
                                     {"po/n/g/11/edg"},
                                     {"po/n/g/12/edg"},
                                     {"po/n/g/13/edg"},
                                     {"po/n/g/21/edg"},
                                     {"po/n/g/22/edg"},
                                     {"po/n/g/23/edg"},
                                     {"po/n/g/31/edg"},
                                     {"po/n/g/32/edg"},
                                     {"po/n/g/33/edg"},
                                     {"po/n/h/11/edg"},
                                     {"po/n/h/12/edg"},
                                     {"po/n/h/13/edg"},
                                     {"po/n/h/21/edg"},
                                     {"po/n/h/22/edg"},
                                     {"po/n/h/23/edg"},
                                     {"po/n/h/31/edg"},
                                     {"po/n/h/32/edg"},
                                     {"po/n/h/33/edg"}
                                   };

static char *rngFiles[Num_Files] = {
                                     {"po/n/g/11/rng"},
                                     {"po/n/g/12/rng"},
                                     {"po/n/g/13/rng"},
                                     {"po/n/g/21/rng"},
                                     {"po/n/g/22/rng"},
                                     {"po/n/g/23/rng"},
                                     {"po/n/g/31/rng"},
                                     {"po/n/g/32/rng"},
                                     {"po/n/g/33/rng"},
                                     {"po/n/h/11/rng"},
                                     {"po/n/h/12/rng"},
                                     {"po/n/h/13/rng"},
                                     {"po/n/h/21/rng"},
                                     {"po/n/h/22/rng"},
                                     {"po/n/h/23/rng"},
                                     {"po/n/h/31/rng"},
                                     {"po/n/h/32/rng"},
                                     {"po/n/h/33/rng"}
                                   };


/*---------------------------------------------*/
/* Main - test driver
/*
/* Description:
/*---------------------------------------------*/
int main (int  argc,
          char *argv)

{
         int                    i;
         edgeTableType          *edgTbl;
         ringTableType          *rngTbl;
         faceTableType          *facTbl;
         featureTableType       *areaFeatureTbl;
         gmsPolygonInfoType     polygonInfo;
         gms_3D_WorldImageType  polygon_3D;
         gms_2D_WorldImageType  polygon_2D;
         gms_2D_ScreenImageType screenPolygon;
         const int              Land_Areas = 1;

   printf("_______________________________________\n");
   printf("Begin : Polygon Utilities Test\n");

   areaFeatureTbl = gmsGetFeatureTable
                           (gms_PO,
                            gmsGetDcwFullPath("PO/POAREA.AFT"),
                            gmsArea);

   for (i = 0; i < Num_Files; i++)
      {
       printf("_____________________________\n");
       printf("---> Get edg tbl : %s\n", edgFiles[i]);

       edgTbl = gmsGetEdgeTable
                   (gmsGetDcwFullPath(edgFiles[i]));

       printf("---> Get RNG tbl : %s\n", rngFiles[i]);

       rngTbl = gmsGetRingTable
                   (gmsGetDcwFullPath(rngFiles[i]));

       facTbl = gmsGetPtrToFaceTable (gmsGetDcwFullPath(edgFiles[i]));

       printf("---> Get POLYGON INFO\n");

       polygonInfo = gmsGet_PO_PolygonInfoFromEdgeTable
                         (edgTbl,
                          rngTbl,
                          facTbl,
                          areaFeatureTbl,
                          Land_Areas);

       printf("---> Print polygon info\n");

       gmsPrintPolygonInfo (polygonInfo);

       printf("---> Call 'gmsBuildWorldPolygonsFromEdgeTable'\n");

       polygon_3D = gmsBuildWorldPolygonsFromEdgeTable
                               (polygonInfo,
                                edgTbl);

       printf("---> Call 'gmsFree_3D_WorldImage'\n");

       gmsFree_3D_WorldImage (&polygon_3D);

       printf("---> Call 'gmsBuildFlatWorldPolygonsFromEdgeTable'\n");

       polygon_2D = gmsBuildFlatWorldPolygonsFromEdgeTable
                               (polygonInfo,
                                edgTbl);

       printf("---> Call 'gmsFree_2D_WorldImage'\n");

       gmsFree_2D_WorldImage (&polygon_2D);

       printf("---> Call 'gmsAllocateScreenPolygonsFromEdgeTable'\n");

       screenPolygon = gmsAllocateScreenPolygonsFromEdgeTable
                               (polygonInfo,
                                edgTbl);

       printf("---> Call 'gmsFree_2D_ScreenImage'\n");

       gmsFree_2D_ScreenImage (&screenPolygon);

       printf("---> Free polygon info\n");

       gmsFreePolygonInfo (&polygonInfo);

       printf("---> Free edg tbl\n");

       gmsFreeEdgeTable (edgTbl);

       printf("---> Free RNG tbl\n");

       gmsFreeRingTable (rngTbl);

       gmsFreeFaceTable (facTbl);
      }

   printf("End   : Polygon Utilities Test\n");
   printf("_______________________________________\n");

   return 0;
}
