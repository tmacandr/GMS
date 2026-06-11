//----------------------------------------------------------------------------
// File : dcw_library_mgr.cpp
// Date : 08-Jun-26 : initial definition - evolve from "soamafr_demo.cpp"
//
// Description:
//    Class to manage/implement the Qt graphics of any one of the four
//    DCW (full) libraries (directories):
//                 NOAMER
//                 EURNASIA
//                 SOAMAFR
//                 SASAUS
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <iostream>

#include <stdio.h>

#include <QPainter>
#include <QPaintEvent>
#include <QPointF>
#include <QPolygonF>

#include "dcw_library_mgr.h"

#include <gmsDebugUtil.h>
#include <gmsLatLongGridClass.h>
#include <gmsTileClass.h>
#include <gmsMapStateMgr.h>
#include <gmsScreenCoordUtils.h>
#include <gmsUtilities.h>


DCW_Library_Mgr::DCW_Library_Mgr
                    (const std::string       lib_name,
                     const gmsEarthModelType model,
                     const std::string       root_directory,
                     QWidget                 *parent)

   : QWidget(parent)

{
    Q_UNUSED(lib_name);
    Q_UNUSED(model);
    Q_UNUSED(root_directory);

    const double initialZoomFactor = 3000.0;

    gmsSetMapZoomFactor (initialZoomFactor);

    gmsMoveFlatMap (gmsMoveNorth, (3 * g_moveAmount));

    gmsMoveFlatMap (gmsMoveEast, (4 * g_moveAmount));
}


DCW_Library_Mgr::~DCW_Library_Mgr()
{
   #if 0
   theGrid.~gmsLatLongGridClass();
   #endif
}


void DCW_Library_Mgr::zoom(const std::string z)
{
    if (z == "in")
    {
        gmsZoomIn (g_zoomAmount);
    }
    else
    {
        gmsZoomOut(g_zoomAmount);
    }

    update();
}

void DCW_Library_Mgr::move(const std::string dir)
{
    if (dir == "north")
    {
        gmsMoveFlatMap (gmsMoveNorth, g_moveAmount);
    }
    else if (dir == "sourth")
    {
        gmsMoveFlatMap (gmsMoveSouth, g_moveAmount);
    }
    else if (dir == "east")
    {
        gmsMoveFlatMap (gmsMoveEast, g_moveAmount);
    }
    else if (dir == "west")
    {
        gmsMoveFlatMap (gmsMoveWest, g_moveAmount);
    }
    else if (dir == "neutral")
    {
        gmsResetToNeutralOrientation();
    }
    else
    {
        std::cout << "***> ERROR - move\n"
                  << "***>       - Uknown direction: " << dir << "\n";
    }

    update();
}

void DCW_Library_Mgr::set_map_feature
           (const MAP_FEATURE_T feature,
            const bool          state)
{
    featureIsShown[feature] = state;

    update();
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
// FUNCTION: set_zoom_factor 
//
// DESCRIPTION:
//    Resets the zoom factor.
//-----------------------------------------------
void DCW_Library_Mgr::set_zoom_factor(const double newZoomFactor)
{
    gmsSetMapZoomFactor (newZoomFactor);

    update();
}


//-----------------------------------------------
// FUNCTION: set_move_factor 
//
// DESCRIPTION:
//    Resets the distance used for map 'move'
//    operations.
//-----------------------------------------------
void DCW_Library_Mgr::set_move_factor(const float newMoveFactor)
{
    g_moveAmount = newMoveFactor;

    update();
}


//-------------------------------------------------
// paint 
//
// Desciption:
//    Routine to "paint" the graphics into the
//    window.  This routine is invoked due to a
//    WM_PAINT event.  This indicates that the
//    "canvas" needs to be refreshed.
//-------------------------------------------------
void DCW_Library_Mgr::paintEvent(QPaintEvent *event)
{
   Q_UNUSED(event);

   int w = width();
   int h = height();

   gmsSetWindowDimensions(w, h);

   drawMaps();
}

//-------------------------------------------------
// drawMaps
//
// Desciption:
//    Draws each of the selected LIBRARY items to
//    the canvas.
//-------------------------------------------------
void DCW_Library_Mgr::drawMaps()
{
   #if 0
   if (g_handleToCurrentFont != (HFONT) 0)
      SelectObject (hDC, g_handleToCurrentFont);
   #endif

   if (featureIsShown[PO_polygons])
      draw_PO_Polygons ();

   if (featureIsShown[DN_polygons])
      draw_DN_Polygons ();

   if (featureIsShown[DN_lines])
      draw_DN_Lines ();

   if (featureIsShown[HY_lines])
      draw_HY_Lines ();

   if (featureIsShown[mapLines])
      drawMapLines ();

   if (featureIsShown[text])
      drawTextOfMap ();

   if (featureIsShown[cities])
      drawCitiesOfMap ();

   if (featureIsShown[latLongGrid])
      drawLatLongGrid ();
}


//-------------------------------------------------
// drawIndependentPoints
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::drawIndependentPoints
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
void DCW_Library_Mgr::annotateMap
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
void DCW_Library_Mgr::drawImage
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
// drawMapLines
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::drawMapLines()
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
// draw_PO_Polygons
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::draw_PO_Polygons()
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
// draw_DN_Polygons
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::draw_DN_Polygons()
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
// draw_DN_Lines
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::draw_DN_Lines()
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
// draw_HY_Lines
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::draw_HY_Lines()
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
// drawTextOfMap
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::drawTextOfMap()
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
// drawCitiesOfMap
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::drawCitiesOfMap()
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
// drawLatLongGrid
//
// Desciption:
//    Routine that draws lat/long lines to enhance
//    the ellipsoid aspect of the globe.
//-------------------------------------------------
void DCW_Library_Mgr::drawLatLongGrid ()

{
   #if 0
         gms_2D_ScreenImageType gridImage;

   gridImage = theGrid.gmsGetLatitudeGrid();

   drawImage
      (Qt::green,
       gridImage);

   gridImage = theGrid.gmsGetLongitudeGrid();

   drawImage
      (Qt::green,
       gridImage);
   #endif
}

#if 0
//-------------------------------------------------
// doScreenCapture
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::doScreenCapture ()
{
   std::cout << "===> doScreenCapture - NOT implemented\n";
}
#endif

//-------------------------------------------------
// setNewFont
//
// Desciption:
//-------------------------------------------------
void DCW_Library_Mgr::setNewFont ()
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

/* EOF */

