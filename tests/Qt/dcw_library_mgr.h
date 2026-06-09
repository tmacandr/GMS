//----------------------------------------------------------------------------
// File : dcw_library_mgr.h
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

#ifndef DCW_LIBRARY_MGR_

#include <string>

#include <gmsTypesAndConstants.h>

#include <QWidget>


class DCW_Library_Mgr  : public QWidget
{
    Q_OBJECT

public:
    DCW_Library_Mgr(const std::string       lib_name,
                    const gmsEarthModelType model,
                    const std::string       root_directory);

    ~DCW_Library_Mgr();

   void zoom(const ZOOM_T zoom);

   void move(const DIRECTION_T dir);

protected:

   void paintEvent(QPaintEvent *event) override;

private:

   int g_width  = 0;
   int g_height = 0;

   const double deltaDegrees = 15.0;
   const gmsEarthModelType g_earthModel = gmsFlat;
   float                   g_moveAmount = 2.0f;
   double                  g_zoomAmount = 1000.0;
   bool                    g_mapLinesAreShown    = true;
   bool                    g_PO_polygonsAreShown = false;
   bool                    g_DN_polygonsAreShown = false;
   bool                    g_DN_linesAreShown    = false;
   bool                    g_textIsShown         = false;
   bool                    g_citiesAreShown      = false;
   bool                    g_HY_linesAreShown    = false;

   void drawMap();

   void clearMapArea();

   void drawUncheckedImage();

   void dlgProcZoomControl();

   void dlgProcMoveControl();

   void performPaint();

   void drawMapLines();

   void draw_PO_Polygons();

   void draw_DN_Polygons();

   void draw_DN_Lines();

   void draw_HY_Lines();

   void drawTextOfMap();

   void drawCitiesOfMap();

   void setNewFont ();

   void drawPolygonImage();

   void drawLatLongGrid();
};

#endif // DCW_LIBRARY_MGR_

/* EOF */

