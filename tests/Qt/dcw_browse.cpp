//-------------------------------------------------------------------------
// File : dcw_browse.cpp
// Date : xx-Feb-00 : initial definition
//        24-Feb-00 : blew away stuff at CSUN.  Start over!
//        26-May-26 : Total pivot to be used by Qt 'dcw_browse' app
//        14-Jun-26 : Fold in Tile Ref mechanics
//
// Description:
//    A set of utilties to be called by Qt program that exercises the
//    "browse" library of DCW database.  Prove concept of portability.
//
//    Will also manage/implement the Qt graphics of any one of the four
//    DCW (full) libraries (directories):
//                 NOAMER
//                 EURNASIA
//                 SOAMAFR
//                 SASAUS
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-------------------------------------------------------------------------
#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <QPainter>
#include <QPaintEvent>
#include <QPointF>
#include <QPolygonF>

#include "dcw_browse.h"
#include <gmsDcwUtilities.h>

DCW_Browse::DCW_Browse(QWidget *parent)

   : QWidget(parent)

{
    std::cout << "------------------ DCW_Browse - begin ---------\n";

    std::cout << "------------------ DCW_Browse - end ---------\n";
}

DCW_Browse::~DCW_Browse()
{
   std::cout << "------------------ ~DCW_Browse - begin ---------\n";

   if (g_theBrowseMap)
   {
       delete g_theBrowseMap;

       g_theBrowseMap = nullptr;
   }

   if (tile_ref_lat_long_grid)
   {
       delete tile_ref_lat_long_grid;

       tile_ref_lat_long_grid = nullptr;
   }

   for (unsigned int i = 0; i < NUM_TILE_REF_MAPS; i++)
   {
       if (tile_ref_map[i])
       {
           delete tile_ref_map[i];

           tile_ref_map[i] = nullptr;
       }
   }

   std::cout << "------------------ ~DCW_Browse - end ----------\n";
}


void DCW_Browse::init_browse_map
                    (const std::string earth_model)
{
   if ( not g_theBrowseMap )
   {
       browse_model = toEarthModel(earth_model);

       g_theBrowseMap = new gmsBrowseMapClass (browse_model);

       if ( not g_theBrowseMap )
       {
           std::cout << "***> ERROR - allocate browse map fail\n";

           std::exit(99);
       }

       gmsSetMapZoomFactor(14000.0);
   }
}


//---------------------------------------------
// set_browse_lat_long_grid_state
//
// Description:
//---------------------------------------------
void DCW_Browse::set_browse_lat_long_grid_state
                    (const bool is_clicked)
{
    g_browse_lat_long_grid_is_on = is_clicked;

    update();
}


//---------------------------------------------
// set_browse_state
//
// Description:
//---------------------------------------------
void DCW_Browse::set_browse_state
                    (const gmsBrowseThematicType which_map,
                     const bool                  is_clicked)
{
    g_browse_theme_is_shown[which_map] = is_clicked;

    update();
}


//---------------------------------------------
// init_tile_ref_map
//
// Description:
//---------------------------------------------
void DCW_Browse::init_tile_ref_map
              (const TILE_REF_MAP_T    map,
               const gmsEarthModelType model)
{
    std::cout << "init_tile_ref_map - begin\n";

    const double initialZoomFactor = 3000.0;

    gmsSetMapZoomFactor (initialZoomFactor);

    if ( not tile_ref_lat_long_grid )
    {
        tile_ref_lat_long_grid = new gmsLatLongGridClass
                                           (tile_ref_deltaDegrees,
                                            model);
    }

    if ( tile_ref_map[map] )
    {
        std::cout << "***> ERROR - Tile ref map already defined\n";
        return;
    }

    tile_ref_map[map] = new gmsTileClass
                               (model, // earth model
                                'N',   // longTileId
                                'G');  // latTileId

    //gmsMoveFlatMap (gmsMoveNorth, (3 * g_tile_ref_moveAmount));

    //gmsMoveFlatMap (gmsMoveEast, (4 * g_tile_ref_moveAmount));

    std::cout << "init_tile_ref_map - end\n";
}


//--------------------------------------------- 
// set_tile_ref_map_feature
//
// Description:
//---------------------------------------------
void DCW_Browse::set_tile_ref_map_feature
              (const TILE_REF_MAP_T     map,
               const TILE_REF_FEATURE_T feature,
               const bool               state)
{
    std::cout << "set_tile_ref_map_feature - begin\n";

    Q_UNUSED(map);

    tile_ref_featureIsShown[feature] = state;

    update();

    std::cout << "set_tile_ref_map_feature - end\n";
}


//---------------------------------------------
// zoom
//
// Description:
//---------------------------------------------
void DCW_Browse::zoom(const std::string z)
{
   if (z == "in")
   {
       gmsZoomIn (g_browse_zoom_amount);
   }
   else
   {
       gmsZoomOut (g_browse_zoom_amount);
   }

   update();
}


//---------------------------------------------
// move
//
// Description:
//---------------------------------------------
void DCW_Browse::move(const std::string dir)
{
   if (browse_model == gmsFlat)
   {
       if (dir == "north")
       {
            gmsMoveFlatMap (gmsMoveNorth, g_tile_ref_moveAmount);
       }
       else if (dir == "south")
       {
            gmsMoveFlatMap (gmsMoveSouth, g_tile_ref_moveAmount);
       }
       else if (dir == "east")
       {
            gmsMoveFlatMap (gmsMoveEast, g_tile_ref_moveAmount);
       }
       else if (dir == "west")
       {
            gmsMoveFlatMap (gmsMoveWest, g_tile_ref_moveAmount);
       }
       else if (dir == "neutral")
       {
            gmsResetToNeutralOrientation();
       }
       else
       {
            std::cout << "***> ERROR - move - flat model\n"
                      << "***>       - Uknown direction: " << dir << "\n";
       }
   }
   else // sphere or ellipse
   {
       if (dir == "north")
       {
          gmsSet_X_Rotation (g_browse_rotationDeg);
       }
       else if (dir == "south")
       {
          gmsSet_X_Rotation (-g_browse_rotationDeg);
       }
       else if (dir == "east")
       {
          gmsSet_Y_Rotation (-g_browse_rotationDeg);
       }
       else if (dir == "west")
       {
          gmsSet_Y_Rotation (g_browse_rotationDeg);
       }
       else
       {
           std::cout << "***> ERROR - move - sphere/ellipse model\n"
                     << "***>       - Uknown direction: " << dir << "\n";
       }
   }

   update();
}


//---------------------------------------------
// clearMapArea 
//
// Description:
//---------------------------------------------
void DCW_Browse::clearMapArea ()

{
    std::cout << "clearMapArea - begin\n";

    QPainter painter(this);

    int w = width();
    int h = height();

    QRect rect(0,0, w, h);

    painter.fillRect(rect, Qt::white);

    std::cout << "clearMapArea - end\n";
}


//---------------------------------------------
// draw_browse_maps 
//
// Description:
//---------------------------------------------
void DCW_Browse::draw_browse_maps ()
{
   std::cout << "-------------- draw_browse_maps - begin ----------\n";

   clearMapArea ();

   if (g_browse_theme_is_shown[gmsBrowse_LibRef])
      draw_browse_LibRef ();

   if (g_browse_theme_is_shown[gmsBrowse_DN])
      draw_browse_Drainage ();

   if (g_browse_theme_is_shown[gmsBrowse_PO])
      draw_browse_PoliticalAndOceans ();

   if (g_browse_theme_is_shown[gmsBrowse_PP])
      draw_browse_PopulatedPlaces ();

   if (g_browse_lat_long_grid_is_on)
      draw_browse_LatLongGrid ();

   std::cout << "------------------ draw_browse_maps - end -------\n";
}


//---------------------------------------------
// draw_browse_LibRef 
//
// Description:
//---------------------------------------------
void DCW_Browse::draw_browse_LibRef()

{
   std::cout << "draw_browse_LibRef - begin\n";

   if ( not g_theBrowseMap )
   {
       std::cout << "***> ERROR - browse map is NULL\n";
       std::exit(89);
   }

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_LibRef);

   drawImage
       (Qt::darkMagenta,
        tempImage);

   std::cout << "draw_browse_LibRef- end\n";
}


//---------------------------------------------
// draw_browse_Drainage
//
// Description:
//---------------------------------------------
void DCW_Browse::draw_browse_Drainage()

{
   std::cout << "draw_browse_Drainage - begin\n";

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_DN);

   drawImage
      (Qt::blue,
       tempImage);

   std::cout << "draw_browse_Drainage- end\n";
}


//---------------------------------------------
// draw_browse_PoliticalAndOceans
//
// Description:
//---------------------------------------------
void DCW_Browse::draw_browse_PoliticalAndOceans()

{
   std::cout << "draw_browse_PoliticalAndOceans - begin\n";

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_PO);

   drawImage
      (Qt::red,
       tempImage);

   std::cout << "draw_browse_PoliticalAndOceans - end\n";
}


//---------------------------------------------
// draw_browse_PopulatedPlaces
//
// Description:
//---------------------------------------------
void DCW_Browse::draw_browse_PopulatedPlaces()

{
   std::cout << "draw Populated Places - Not implemented\n";
}


//---------------------------------------------
// draw_browse_LatLongGrid
//
// Description:
//---------------------------------------------
void DCW_Browse::draw_browse_LatLongGrid ()

{
   std::cout << "draw_browse_LatLongGrid - begin\n";

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetLatitudeGrid();

   drawImage
      (Qt::green,
       tempImage);

   tempImage = g_theBrowseMap->gmsGetLongitudeGrid();

   drawImage
      (Qt::green,
       tempImage);

   std::cout << "draw_browse_LatLongGrid - end\n";
}


//---------------------------------------------
// draw_browse_IndependentPoints 
//
// Description:
//---------------------------------------------
void DCW_Browse::draw_browse_IndependentPoints
               (Qt::GlobalColor           whichColor,
                gms_2D_ScreenPolylineType thePoints)

{
   std::cout << "draw_browse_IndependentPoints - begin\n";

   Q_UNUSED(whichColor);
   Q_UNUSED(thePoints);

   std::cout << "Not implemented\n";

   std::cout << "draw_browse_IndependentPoints - end\n";
}



#if 0
void What_Are_These()
{
       case ID_ZOOM_DIALOG:
          {
           if (g_zoomDlgHandle == (HWND) 0)
              {
               g_zoomDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_ZOOM_CTRL_DIALOG),
                                     g_windowHandle,
                                     dlgProcZoomControl);
              }

           ShowWindow (g_zoomDlgHandle, SW_SHOW);

           break;
          }

       case ID_MOVE_DIALOG:
          {
           if (g_moveDlgHandle == (HWND) 0)
              {
               g_moveDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_MOVE_CTRL_DIALOG),
                                     g_windowHandle,
                                     dlgProcMoveControl);
              }

           ShowWindow (g_moveDlgHandle, SW_SHOW);

           break;
          }
}
#endif

//-----------------------------------------------
// FUNCTION: set_tile_ref_zoom_factor 
//
// DESCRIPTION:
//    Resets the zoom factor.
//-----------------------------------------------
void DCW_Browse::set_tile_ref_zoom_factor(const double newZoomFactor)
{
    gmsSetMapZoomFactor (newZoomFactor);

    update();
}


//-----------------------------------------------
// FUNCTION: set_tile_ref_move_factor 
//
// DESCRIPTION:
//    Resets the distance used for map 'move'
//    operations.
//-----------------------------------------------
void DCW_Browse::set_tile_ref_move_factor(const float newMoveFactor)
{
    g_tile_ref_moveAmount = newMoveFactor;

    update();
}


//-------------------------------------------------
// draw_tile_ref_maps
//
// Desciption:
//    Draws each of the selected TILE REF items to
//    the canvas.
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_maps()
{
   #if 0
   if (g_handleToCurrentFont != (HFONT) 0)
      SelectObject (hDC, g_handleToCurrentFont);
   #endif

   if (tile_ref_featureIsShown[PO_polygons])
      draw_tile_ref_PO_Polygons ();

   if (tile_ref_featureIsShown[DN_polygons])
      draw_tile_ref_DN_Polygons ();

   if (tile_ref_featureIsShown[DN_lines])
      draw_tile_ref_DN_Lines ();

   if (tile_ref_featureIsShown[HY_lines])
      draw_tile_ref_HY_Lines ();

   if (tile_ref_featureIsShown[mapLines])
      draw_tile_ref_MapLines ();

   if (tile_ref_featureIsShown[text])
      draw_tile_ref_TextOfMap ();

   if (tile_ref_featureIsShown[cities])
      draw_tile_ref_CitiesOfMap ();

   if (tile_ref_featureIsShown[latLongGrid])
      draw_tile_ref_LatLongGrid ();
}


//-------------------------------------------------
// drawIndependentPoints
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::drawIndependentPoints
               (Qt::GlobalColor           whichColor,
                gms_2D_ScreenPolylineType thePoints)

{
Q_UNUSED(whichColor);
Q_UNUSED(thePoints);
   #if 0
         const int onePixelWide = 0;
         int       i;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                whichColor);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < thePoints.numPoints; i++)
   {
       Rectangle
          (hDC,
           thePoints.points[i].x,
           thePoints.points[i].y,
           thePoints.points[i].x + 4,
           thePoints.points[i].y + 4);
   }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
   #endif
}


//-------------------------------------------------
// annotateMap
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::annotateMap
                    (Qt::GlobalColor     whichColor,
                     gmsMapTextArrayType theText)

{
Q_UNUSED(whichColor);
Q_UNUSED(theText);

   #if 0
      int i;

   for (i = 0; i < theText.numTextRecords; i++)
      {
       TextOutA
          (hDC,
           theText.textRecords[i].theCoord.x,
           theText.textRecords[i].theCoord.y,
           theText.textRecords[i].theString,
           theText.textRecords[i].numChars);
      }
   #endif
}


//-------------------------------------------------
// drawImage
//
// Desciption:
//    Duplicate of what's in 'dcw_browse.cpp'.
//-------------------------------------------------
void DCW_Browse::drawImage
               (Qt::GlobalColor        whichColor,
                gms_2D_ScreenImageType theImage)
{
   std::cout << "library - drawImage - begin\n";

   int i;
   int j;

   QPainter painter(this);

   painter.setPen(whichColor);

   for (i = 0; i < theImage.numLines; i++)
   {
      for (j = 0; j < (theImage.imageLines[i].numPoints - 1); j++)
      {
          // is this still needed?
          bool is_visible = gmsIsVisibleLine
                               (theImage.imageLines[i].points[j],
                                theImage.imageLines[i].points[j + 1]); 

          if (is_visible)
          {
              QLine line(theImage.imageLines[i].points[j].x,
                         theImage.imageLines[i].points[j].y,
                         theImage.imageLines[i].points[j + 1].x,
                         theImage.imageLines[i].points[j + 1].y);

              painter.drawLine(line);
          }
      }
   }

   std::cout << "library - drawImage - end\n";
}


//-------------------------------------------------
// drawPolygonImage
//
// Desciption:
//-------------------------------------------------
void drawPolygonImage
               (Qt::GlobalColor        borderColor,
                Qt::GlobalColor        fillColor,
                gms_2D_ScreenImageType mapImage)

{
Q_UNUSED(borderColor);
Q_UNUSED(fillColor);
Q_UNUSED(mapImage);

   #if 0
         int       i;
         HPEN      newPen;
         HGDIOBJ   oldPen;
         HBRUSH    newBrush;
         HGDIOBJ   oldBrush;
         LOGBRUSH  brushAttributes;
         const int onePixelWide = 0;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                borderColor);

   oldPen = SelectObject (hDC, newPen);

   brushAttributes.lbColor = fillColor;
   brushAttributes.lbHatch = 0;
   brushAttributes.lbStyle = BS_SOLID;

   newBrush = CreateBrushIndirect (&brushAttributes);

   oldBrush = SelectObject (hDC, newBrush);

   for (i = 0; i < mapImage.numLines; i++)
   {
       Polygon
          (hDC,
           (POINT *) mapImage.imageLines[i].points,
           mapImage.imageLines[i].numPoints);
   }

   SelectObject (hDC, oldBrush);

   DeleteObject (newBrush);

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
   #endif
}


//-------------------------------------------------
// draw_tile_ref_MapLines
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_MapLines()
{
   #if 0
         int                    i;
         int                    numMaps;
         gmsMapClass            **ptrToMaps;
         gms_2D_ScreenImageType mapImage;

   ptrToMaps = theTileMap.gmsGetTileMaps
                             (gms_PO,
                              numMaps);

   for (i = 0; i < numMaps; i++)
   {
       mapImage = ptrToMaps[i]->gmsGetMapImage ();

       drawImage
          (Qt::red,
           mapImage);
   }
   #endif
}


//-------------------------------------------------
// draw_tile_ref_PO_Polygons
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_PO_Polygons()
{
   #if 0
         int                    i;
         int                    numPolygonObjs;
         gms_PO_PolygonMapClass **ptrToPolygonObjs;
         gms_2D_ScreenImageType polygonImage;

   ptrToPolygonObjs = theTileMap.gmsGetTile_PO_Polygons (numPolygonObjs);

   for (i = 0; i < numPolygonObjs; i++)
   {
       polygonImage = ptrToPolygonObjs[i]->gmsGetLandAreas ();

       drawPolygonImage
          (DARK_Qt::green,
           DARK_Qt::green,
           polygonImage);
   }
   #endif
}


//-------------------------------------------------
// draw_tile_ref_DN_Polygons
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_DN_Polygons()
{
   #if 0
         int                    i;
         int                    numPolygonObjs;
         gms_DN_PolygonMapClass **ptrToPolygonObjs;
         gms_2D_ScreenImageType theImage;

   ptrToPolygonObjs = theTileMap.gmsGetTile_DN_Polygons (numPolygonObjs);

   for (i = 0; i < numPolygonObjs; i++)
   {
       theImage = ptrToPolygonObjs[i]->gmsGetInlandWaterAreas ();

       drawPolygonImage
          (Qt::blue,
           Qt::blue,
           theImage);
   }
   #endif
}


//-------------------------------------------------
// draw_tile_ref_DN_Lines
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_DN_Lines()
{
   #if 0
         int                    i;
         int                    numMaps;
         gmsMapClass            **ptrToMaps;
         gms_2D_ScreenImageType theImage;

   ptrToMaps = theTileMap.gmsGetTileMaps
                             (gms_DN,
                              numMaps);

   for (i = 0; i < numMaps; i++)
   {
       theImage = ptrToMaps[i]->gmsGetMapImage ();

       drawImage
          (Qt::magenta,
           theImage);
   }
   #endif
}


//-------------------------------------------------
// draw_tile_ref_HY_Lines
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_HY_Lines()
{
   #if 0
         int                    i;
         int                    numMaps;
         gmsMapClass            **ptrToMaps;
         gms_2D_ScreenImageType theImage;

   ptrToMaps = theTileMap.gmsGetTileMaps
                             (gms_HY,
                              numMaps);

   for (i = 0; i < numMaps; i++)
   {
       theImage = ptrToMaps[i]->gmsGetMapImage ();

       drawImage
          (Qt::yellow,
           theImage);
   }
   #endif
}


//-------------------------------------------------
// draw_tile_ref_TextOfMap
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_TextOfMap()
{
   #if 0
         int                  i;
         int                  numMaps;
         gmsTextClass         **ptrToText;
         gmsMapTextArrayType textData;

   ptrToText = theTileMap.gmsGetTileText
                             (gms_PO,
                              numMaps);

   for (i = 0; i < numMaps; i++)
   {
       textData = ptrToText[i]->gmsGetTextItems ();

       // what's this?
       textData.numTextRecords = textData.numTextRecords / 2;

       annotateMap
          (Qt::black,
           textData);
   }
   #endif
}


//-------------------------------------------------
// draw_tile_ref_CitiesOfMap
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_CitiesOfMap()
{
   #if 0
         int                       i;
         int                       numObjs;
         gmsNodeClass              **ptrToNodes;
         gms_2D_ScreenPolylineType nodeImage;

   ptrToNodes = theTileMap.gmsGetTileNodes
                             (gms_PO,
                              numObjs);

   for (i = 0; i < numObjs; i++)
   {
       nodeImage = ptrToNodes[i]->gmsGetNodePoints ();

       drawIndependentPoints
          (Qt::black,
           nodeImage);
   }

         int                  numMaps;
         gmsTextClass         **ptrToText;
         gmsMapTextArrayType textData;

   ptrToText = theTileMap.gmsGetTileText
                             (gms_PP,
                              numMaps);

   for (i = 0; i < numMaps; i++)
   {
       textData = ptrToText[i]->gmsGetTextItems ();

       annotateMap
          (Qt::black,
           textData);
   }
   #endif
}


//-------------------------------------------------
// draw_tile_ref_LatLongGrid
//
// Desciption:
//    Routine that draws lat/long lines to enhance
//    the ellipsoid aspect of the globe.
//-------------------------------------------------
void DCW_Browse::draw_tile_ref_LatLongGrid ()

{
#if 0
   std::cout << "draw_tile_ref_LatLongGrid - begin\n";

   gms_2D_ScreenImageType gridImage;

   gridImage = theGrid->gmsGetLatitudeGrid();

   drawImage
      (Qt::green,
       gridImage);

   gridImage = theGrid->gmsGetLongitudeGrid();

   drawImage
      (Qt::green,
       gridImage);

   std::cout << "draw_tile_ref_LatLongGrid - end\n";
#endif
}

#if 0
//-------------------------------------------------
// doScreenCapture
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::doScreenCapture ()
{
   std::cout << "===> doScreenCapture - NOT implemented\n";
}
#endif

//-------------------------------------------------
// setNewFont
//
// Desciption:
//-------------------------------------------------
void DCW_Browse::setNewFont ()
{
   #if 0
         static bool       isInitialized = false;
         static CHOOSEFONT infoFromFontDlg;
         static LOGFONT    logFont;
         HGDIOBJ           systemFontObj;
         HFONT             handleToNewFont;
         BOOL              dlgIsOk;

   if ( ! isInitialized )
   {
       infoFromFontDlg.lStructSize            = sizeof(infoFromFontDlg);
       infoFromFontDlg.hwndOwner              = g_windowHandle;
       infoFromFontDlg.hDC                    = NULL; 
       infoFromFontDlg.lpLogFont              = &logFont;
       infoFromFontDlg.iPointSize             = 0;
       infoFromFontDlg.Flags                  = CF_INITTOLOGFONTSTRUCT |
                                                CF_SCREENFONTS         |
                                                CF_EFFECTS;
       infoFromFontDlg.rgbColors              = 0;
       infoFromFontDlg.lCustData              = 0; 
       infoFromFontDlg.lpfnHook               = NULL;
       infoFromFontDlg.lpTemplateName         = NULL;
       infoFromFontDlg.hInstance              = NULL;
       infoFromFontDlg.lpszStyle              = NULL; 
       infoFromFontDlg.nFontType              = 0;
       infoFromFontDlg.___MISSING_ALIGNMENT__ = 0; 
       infoFromFontDlg.nSizeMin               = 0;
       infoFromFontDlg.nSizeMax               = 0; 

       systemFontObj = GetStockObject (SYSTEM_FONT);

       GetObject
          (systemFontObj,
           sizeof(logFont),
           (LPVOID) &logFont);

       g_handleToCurrentFont = CreateFontIndirect (&logFont);

       isInitialized = true;
      }

   dlgIsOk = ChooseFont (&infoFromFontDlg);

   if ( ! dlgIsOk )
   {
       return;  // i.e. CANCEL
   }

   handleToNewFont = CreateFontIndirect (&logFont);

   DeleteObject (g_handleToCurrentFont);

   g_handleToCurrentFont = handleToNewFont;
      
   MessageBoxA
      (NULL,
       "Set Font",
       "Done",
       MB_OK);
   #endif
}


//---------------------------------------------
// paintEvent
//
// Description:
//    Virtual function override of default
//    'paint' method to 'QtWidget' parent.
//---------------------------------------------
void DCW_Browse::paintEvent(QPaintEvent *event)
{
   std::cout << "vvvvvvvvvvvvvvvv paintEvent - begin vvvvvvvvvvvvvvvv\n";

   Q_UNUSED(event);

   int w = width();
   int h = height();

   gmsSetWindowDimensions(w, h);

   draw_browse_maps ();

   draw_tile_ref_maps();

   std::cout << "^^^^^^^^^^^^^^^^ paintEvent - end ^^^^^^^^^^^^^^^^^^\n";
}

/* EOF */

