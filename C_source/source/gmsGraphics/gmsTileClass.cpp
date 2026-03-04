//-----------------------------------------------------------------------------
// File : gmsTileClass.cpp
// Date : 20-Dec-99 : intitial definition (old name : gmsBaseThemeClass).
//        25-Jan-00 : continue development ... never really finished
//        26-Jan-00 : pare down to "tile" only instead of whole theme
//        06-Feb-00 : add methods to get polygons
//        06-Feb-00 : rename.  Not theme specific anymore
//
// Description:
//    This class is an abstraction to a GEOREF tile.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#include <gmsTileClass.h>
#include <gmsDcwUtilities.h>
   //------------//
#include <gmsNamesOfFilesClass.h>
   //------------//
#include <gmsDebugUtil.h>


//---------------------------------
// Declare some types and constants
//---------------------------------

typedef struct
   {
    int         numEdgs;
    gmsMapClass **edgMaps;
   } gmsMapEdgsType;

typedef struct
   {
    int          numTxtObjs;
    gmsTextClass **txtObjs;
   } gmsMapTextType;

typedef struct
   {
    int          numEndObjs;
    gmsNodeClass **endObjs;
   } gmsMapNodesType;

typedef struct
   {
    gmsEarthModelType      theModel;
    char                   latTile;
    char                   longTile;
    gmsMapEdgsType         mapEdgs[Num_Themes];
    gmsMapTextType         mapTxt[Num_Themes];
    gmsMapNodesType        mapNodes[Num_Themes];
    int                    num_PO_Polygons;
    gms_PO_PolygonMapClass **poPolygons;
    int                    num_DN_Polygons;
    gms_DN_PolygonMapClass **dnPolygons;
   } gmsThemeTileObjType;


//---------------------------------
// Declare static member objects
//---------------------------------

static gmsNamesOfFilesClass *g_ptrToFileList[Num_Themes] =
                                 { NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL };

static featureTableType *g_lineFeatureTbl[Num_Themes] =
                                 { NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL };

static featureTableType *g_pointFeatureTbl[Num_Themes] =
                                 { NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL };

static featureTableType *g_areaFeatureTbl[Num_Themes] =
                                 { NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL };

static featureTableType *g_textFeatureTbl[Num_Themes] =
                                 { NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL, NULL, NULL, NULL };

static const int Not_Initialized = -1;


//---------------------------------
// Declare Static member functions
//---------------------------------


//-----------------------------------------------
// gmsTileClass
//
// Description:
//    Constructor.  An object that represents the
//    edges, points, text, etc of the specified
//    tile is built.
//-----------------------------------------------
gmsTileClass::gmsTileClass
                      (gmsEarthModelType whichModel,
                       const char        longTileId,
                       const char        latTileId)

{
         int                 i;
         gmsThemeTileObjType *ptrToNewTile;

   ptrToNewTile = new gmsThemeTileObjType;

   gmsClearMemory
      ( (char *) ptrToNewTile,
        sizeof (gmsThemeTileObjType) );

   ptrToNewTile->theModel   = whichModel;
   ptrToNewTile->latTile    = latTileId;
   ptrToNewTile->longTile   = longTileId;

   ptrToNewTile->num_PO_Polygons = Not_Initialized;

   ptrToNewTile->num_DN_Polygons = Not_Initialized;

   for (i = 0; i < Num_Themes; i++)
      {
       ptrToNewTile->mapEdgs[i].numEdgs     = Not_Initialized;

       ptrToNewTile->mapTxt[i].numTxtObjs   = Not_Initialized;

       ptrToNewTile->mapNodes[i].numEndObjs = Not_Initialized;
      }

   gmsTileClass::tileObj = (void *) ptrToNewTile;
}


//-----------------------------------------------
// ~gmsTileClass
//
// Description:
//    Destructor.  Deletes ... nothing 
//-----------------------------------------------
gmsTileClass::~gmsTileClass ()

{

}


//-----------------------------------------------
// gmsGetTileMaps
//
// Description:
//    Retrieves the MAP objects of the tile from
//    the specified theme.  A pointer to an array
//    of map objects is returned to the caller.
//    The number of map objects is returned in
//    the parameter "numObjs".
//-----------------------------------------------
gmsMapClass **gmsTileClass::gmsGetTileMaps
                               (const gmsThematicType whichTheme,
                                int                   &numObjs)

{
         gmsThemeTileObjType *ptrToTileObj;

   if ( (whichTheme < gms_AE) || (whichTheme > gms_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetTileMaps : invalid theme\n");

       return (gmsMapClass **) NULL;
      }

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   if (ptrToTileObj->mapEdgs[whichTheme].numEdgs == Not_Initialized)
      {
       initializeThemeData
          (whichTheme,
           ptrToTileObj->longTile,
           ptrToTileObj->latTile);

       buildMapObjects
          (whichTheme);
      }

   numObjs = ptrToTileObj->mapEdgs[whichTheme].numEdgs;

   return ptrToTileObj->mapEdgs[whichTheme].edgMaps;
}


//-----------------------------------------------
// gmsGetTile_PO_Polygons
//
// Description:
//    Retrieves the POLITICAL/OCEANS (PO) polygon
//    objects for the tile.  A pointer to an array
//    of polygon objects is returned to the caller.
//    The number of polygon objects is returned in
//    the parameter "numObjs".
//-----------------------------------------------
gms_PO_PolygonMapClass **gmsTileClass::gmsGetTile_PO_Polygons
                                               (int &numObjs)

{
         gmsThemeTileObjType *ptrToTileObj;

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   if (ptrToTileObj->num_PO_Polygons == Not_Initialized)
      {
       initializeThemeData
          (gms_PO,
           ptrToTileObj->longTile,
           ptrToTileObj->latTile);

       build_PO_PolygonObjects ();
      }

   numObjs = ptrToTileObj->num_PO_Polygons;

   return ptrToTileObj->poPolygons;
}


//-----------------------------------------------
// gmsGetTileText
//
// Description:
//    Retrieves the TEXT objects of the tile from
//    the specified theme.  A pointer to an array
//    of text objects is returned to the caller.
//    The number of text objects is returned in
//    the parameter "numObjs".
//-----------------------------------------------
gmsTextClass **gmsTileClass::gmsGetTileText
                                (const gmsThematicType whichTheme,
                                 int                   &numObjs)

{
         gmsThemeTileObjType *ptrToTileObj;

   if ( (whichTheme < gms_AE) || (whichTheme > gms_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetTileText : invalid theme\n");

       return (gmsTextClass **) NULL;
      }

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   if (ptrToTileObj->mapTxt[whichTheme].numTxtObjs == Not_Initialized)
      {
       initializeThemeData
          (whichTheme,
           ptrToTileObj->longTile,
           ptrToTileObj->latTile);

       buildTextObjects (whichTheme);
      }

   numObjs = ptrToTileObj->mapTxt[whichTheme].numTxtObjs;

   return ptrToTileObj->mapTxt[whichTheme].txtObjs;
}


//-----------------------------------------------
// gmsGetTileNodes
//
// Description:
//    Retrieves the NODE objects of the tile from
//    the specified theme.  A pointer to an array
//    of node objects is returned to the caller.
//    The number of node objects is returned in
//    the parameter "numObjs".
//-----------------------------------------------
gmsNodeClass **gmsTileClass::gmsGetTileNodes
                                (const gmsThematicType whichTheme,
                                 int                   &numObjs)

{
         gmsThemeTileObjType *ptrToTileObj;

   if ( (whichTheme < gms_AE) || (whichTheme > gms_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetTileNodes : invalid theme\n");

       return (gmsNodeClass **) NULL;
      }

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   if (ptrToTileObj->mapNodes[whichTheme].numEndObjs == Not_Initialized)
      {
       initializeThemeData
          (whichTheme,
           ptrToTileObj->longTile,
           ptrToTileObj->latTile);

       buildNodeObjects (whichTheme);
      }

   numObjs = ptrToTileObj->mapNodes[whichTheme].numEndObjs;

   return ptrToTileObj->mapNodes[whichTheme].endObjs;
}


//-----------------------------------------------
// gmsGetTile_DN_Polygons
//
// Description:
//    Retrieves the DRAINAGE (DN) polygon
//    objects for the tile.  A pointer to an array
//    of polygon objects is returned to the caller.
//    The number of polygon objects is returned in
//    the parameter "numObjs".
//-----------------------------------------------
gms_DN_PolygonMapClass **gmsTileClass::gmsGetTile_DN_Polygons
                                          (int &numObjs)

{
         gmsThemeTileObjType *ptrToTileObj;

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   if (ptrToTileObj->num_DN_Polygons == Not_Initialized)
      {
       initializeThemeData
          (gms_DN,
           ptrToTileObj->longTile,
           ptrToTileObj->latTile);

       build_DN_PolygonObjects ();
      }

   numObjs = ptrToTileObj->num_DN_Polygons;

   return ptrToTileObj->dnPolygons;
}


        //---------------------------
        //  Static Member Functions
        //---------------------------


//-----------------------------------------------
// initializeThemeData
//
// Description:
//-----------------------------------------------
void gmsTileClass::initializeThemeData
               (gmsThematicType whichTheme,
                char            longTileId,
                char            latTileId)

{
         gmsThemeFilesListType filesList;

   if (g_ptrToFileList[whichTheme] != NULL)
      return;

   g_ptrToFileList[whichTheme] = new gmsNamesOfFilesClass
                                        (whichTheme,
                                         longTileId,
                                         latTileId);

   filesList = g_ptrToFileList[whichTheme]->gmsGetFilesList ();

   if (g_lineFeatureTbl[whichTheme] == NULL)
      g_lineFeatureTbl[whichTheme] = gmsGetFeatureTable
                                        (whichTheme,
                                         filesList.lineFeatureTbl,
                                         gmsLine);

   if (g_pointFeatureTbl[whichTheme] == NULL)
      g_pointFeatureTbl[whichTheme] = gmsGetFeatureTable
                                         (whichTheme,
                                          filesList.pointFeatureTbl,
                                          gmsPoint);

   if (g_areaFeatureTbl[whichTheme] == NULL)
      g_areaFeatureTbl[whichTheme] = gmsGetFeatureTable
                                         (whichTheme,
                                          filesList.areaFeatureTbl,
                                          gmsArea);

   if (g_textFeatureTbl[whichTheme] == NULL)
      g_textFeatureTbl[whichTheme] = gmsGetFeatureTable
                                         (whichTheme,
                                          filesList.textFeatureTbl,
                                          gmsText);
}


//-----------------------------------------------
// buildMapObjects
//
// Description:
//-----------------------------------------------
void gmsTileClass::buildMapObjects
               (const gmsThematicType whichTheme)

{
         int                   numEdgFiles;
         int                   i;
         bool                  isInTile;
         int                   count = 0;
         gmsThemeFilesListType filesList;

   filesList = g_ptrToFileList[whichTheme]->gmsGetFilesList ();

   numEdgFiles = filesList.edgTbls.numFiles;

   count = 0;

   gmsThemeTileObjType *ptrToTileObj;

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   for (i = 0; i < numEdgFiles; i++)
      {
       isInTile = g_ptrToFileList[whichTheme]->gmsFileIsInTile
                         (filesList.edgTbls.filePath[i],
                          ptrToTileObj->latTile,
                          ptrToTileObj->longTile);

       if (isInTile)
          count++;
      }

   ptrToTileObj->mapEdgs[whichTheme].edgMaps =
         (gmsMapClass **) new gmsMapClass*[count];

   ptrToTileObj->mapEdgs[whichTheme].numEdgs = count;

   for (i = 0; i < count; i++)
      ptrToTileObj->mapEdgs[whichTheme].edgMaps[i] = NULL;

   count = 0;

   for (i = 0; i < numEdgFiles; i++)
      {
       isInTile = g_ptrToFileList[whichTheme]->gmsFileIsInTile
                      (filesList.edgTbls.filePath[i],
                       ptrToTileObj->latTile,
                       ptrToTileObj->longTile);

       if (isInTile)
          {
           ptrToTileObj->mapEdgs[whichTheme].edgMaps[count] =
              new gmsMapClass
                     (ptrToTileObj->theModel,
                      filesList.edgTbls.filePath[i],
                      g_lineFeatureTbl[whichTheme]);

           if (ptrToTileObj->mapEdgs[whichTheme].edgMaps[count] != NULL)
              count++;
          }
      }

   ptrToTileObj->mapEdgs[whichTheme].numEdgs = count;
}


//-----------------------------------------------
// buildTextObjects
//
// Description:
//-----------------------------------------------
void gmsTileClass::buildTextObjects
               (const gmsThematicType whichTheme)

{
         int                   numTxtFiles;
         int                   i;
         bool                  isInTile;
         int                   count = 0;
         gmsThemeFilesListType filesList;

   filesList = g_ptrToFileList[whichTheme]->gmsGetFilesList ();

   numTxtFiles = filesList.txtTbls.numFiles;

   gmsThemeTileObjType *ptrToTileObj;

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   count = 0;

   for (i = 0; i < numTxtFiles; i++)
      {
       isInTile = g_ptrToFileList[whichTheme]->gmsFileIsInTile
                         (filesList.txtTbls.filePath[i],
                          ptrToTileObj->latTile,
                          ptrToTileObj->longTile);

       if (isInTile)
          count++;
      }

   ptrToTileObj->mapTxt[whichTheme].txtObjs =
         (gmsTextClass **) new gmsTextClass*[count];

   ptrToTileObj->mapTxt[whichTheme].numTxtObjs = count;

   for (i = 0; i < count; i++)
      ptrToTileObj->mapTxt[whichTheme].txtObjs[i] = NULL;

   count = 0;

   for (i = 0; i < numTxtFiles; i++)
      {
       isInTile = g_ptrToFileList[whichTheme]->gmsFileIsInTile
                      (filesList.txtTbls.filePath[i],
                       ptrToTileObj->latTile,
                       ptrToTileObj->longTile);

       if (isInTile)
          {
           ptrToTileObj->mapTxt[whichTheme].txtObjs[count] =
              new gmsTextClass
                     (ptrToTileObj->theModel,
                      filesList.txtTbls.filePath[i]);

           if (ptrToTileObj->mapTxt[whichTheme].txtObjs[count] != NULL)
              count++;
          }
      }

   ptrToTileObj->mapTxt[whichTheme].numTxtObjs = count;
}


//-----------------------------------------------
// buildNodeObjects
//
// Description:
//-----------------------------------------------
void gmsTileClass::buildNodeObjects
               (const gmsThematicType whichTheme)

{
         int                   numEndFiles;
         int                   i;
         bool                  isInTile;
         int                   count = 0;
         gmsThemeFilesListType filesList;

   filesList = g_ptrToFileList[whichTheme]->gmsGetFilesList ();

   numEndFiles = filesList.endTbls.numFiles;

   gmsThemeTileObjType *ptrToTileObj;

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   count = 0;

   for (i = 0; i < numEndFiles; i++)
      {
       isInTile = g_ptrToFileList[whichTheme]->gmsFileIsInTile
                         (filesList.endTbls.filePath[i],
                          ptrToTileObj->latTile,
                          ptrToTileObj->longTile);

       if (isInTile)
          count++;
      }

   ptrToTileObj->mapNodes[whichTheme].endObjs =
       (gmsNodeClass **) new gmsNodeClass*[count];

   ptrToTileObj->mapNodes[whichTheme].numEndObjs = count;

   for (i = 0; i < count; i++)
      ptrToTileObj->mapNodes[whichTheme].endObjs[i] = NULL;

   count = 0;

   for (i = 0; i < numEndFiles; i++)
      {
       isInTile = g_ptrToFileList[whichTheme]->gmsFileIsInTile
                      (filesList.endTbls.filePath[i],
                       ptrToTileObj->latTile,
                       ptrToTileObj->longTile);

       if (isInTile)
          {
           ptrToTileObj->mapNodes[whichTheme].endObjs[count] =
              new gmsNodeClass
                     (ptrToTileObj->theModel,
                      filesList.endTbls.filePath[i]);

           if (ptrToTileObj->mapNodes[whichTheme].endObjs[count] != NULL)
              count++;
          }
      }

   ptrToTileObj->mapNodes[whichTheme].numEndObjs = count;
}


//-----------------------------------------------
// build_PO_PolygonObjects
//
// Description:
//-----------------------------------------------
void gmsTileClass::build_PO_PolygonObjects(void)

{
         int                   numEdgFiles;
         int                   i;
         bool                  isInTile;
         int                   count = 0;
         gmsThemeFilesListType filesList;

   filesList = g_ptrToFileList[gms_PO]->gmsGetFilesList ();

   numEdgFiles = filesList.edgTbls.numFiles;

   gmsThemeTileObjType *ptrToTileObj;

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   count = 0;

   for (i = 0; i < numEdgFiles; i++)
      {
       isInTile = g_ptrToFileList[gms_PO]->gmsFileIsInTile
                         (filesList.edgTbls.filePath[i],
                          ptrToTileObj->latTile,
                          ptrToTileObj->longTile);

       if (isInTile)
          count++;
      }

   ptrToTileObj->poPolygons      = new gms_PO_PolygonMapClass *[count];

   ptrToTileObj->num_PO_Polygons = count;

   for (i = 0; i < count; i++)
      {
       ptrToTileObj->poPolygons[i] = NULL;
      }

   count = 0;

   for (i = 0; i < numEdgFiles; i++)
      {
       isInTile = g_ptrToFileList[gms_PO]->gmsFileIsInTile
                      (filesList.edgTbls.filePath[i],
                       ptrToTileObj->latTile,
                       ptrToTileObj->longTile);

       if (isInTile)
          {
           ptrToTileObj->poPolygons[count] =
              new gms_PO_PolygonMapClass
                     (ptrToTileObj->theModel,
                      filesList.edgTbls.filePath[i],
                      g_areaFeatureTbl[gms_PO]);

           if (ptrToTileObj->poPolygons[count] != NULL)
              count++;
          }
      }

   ptrToTileObj->num_PO_Polygons = count;
}


//-----------------------------------------------
// build_DN_PolygonObjects
//
// Description:
//-----------------------------------------------
void gmsTileClass::build_DN_PolygonObjects(void)

{
         int                   numEdgFiles;
         int                   i;
         bool                  isInTile;
         int                   count = 0;
         gmsThemeFilesListType filesList;

   filesList = g_ptrToFileList[gms_DN]->gmsGetFilesList ();

   numEdgFiles = filesList.edgTbls.numFiles;

   gmsThemeTileObjType *ptrToTileObj;

   ptrToTileObj = (gmsThemeTileObjType *) gmsTileClass::tileObj;

   count = 0;

   for (i = 0; i < numEdgFiles; i++)
      {
       isInTile = g_ptrToFileList[gms_DN]->gmsFileIsInTile
                         (filesList.edgTbls.filePath[i],
                          ptrToTileObj->latTile,
                          ptrToTileObj->longTile);

       if (isInTile)
          count++;
      }

   ptrToTileObj->dnPolygons = (gms_DN_PolygonMapClass **)
                                  new gms_DN_PolygonMapClass*[count];

   ptrToTileObj->num_DN_Polygons = count;

   for (i = 0; i < count; i++)
      ptrToTileObj->dnPolygons[i] = NULL;

   count = 0;

   for (i = 0; i < numEdgFiles; i++)
      {
       isInTile = g_ptrToFileList[gms_DN]->gmsFileIsInTile
                      (filesList.edgTbls.filePath[i],
                       ptrToTileObj->latTile,
                       ptrToTileObj->longTile);

       if (isInTile)
          {
           ptrToTileObj->dnPolygons[count] =
              new gms_DN_PolygonMapClass
                     (ptrToTileObj->theModel,
                      filesList.edgTbls.filePath[i]);

           if (ptrToTileObj->dnPolygons[count] != NULL)
              count++;
          }
      }

   ptrToTileObj->num_DN_Polygons = count;
}
