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
#include <gmsTextClass.h>

#include <QWidget>


typedef enum
{
    mapLines,
    PO_polygons,
    DN_polygons,
    DN_lines,
    text,
    cities,
    HY_lines,
    latLongGrid,
    //-------------//
    NUM_MAP_FEATURES
} MAP_FEATURE_T;


class DCW_Library_Mgr  : public QWidget
{
    Q_OBJECT

public:
    DCW_Library_Mgr(const std::string       lib_name,
                    const gmsEarthModelType model,
                    const std::string       root_directory,
                    QWidget                 *parent = nullptr);

    ~DCW_Library_Mgr();


   //-----------------------------------------------
   // FUNCTION: zoom 
   //
   // DESCRIPTION:
   //    Zoom "in" or "out" on center point of map
   //-----------------------------------------------
   void zoom(const std::string zoom);


   //-----------------------------------------------
   // FUNCTION: set_zoom_factor 
   //
   // DESCRIPTION:
   //    Resets the zoom factor.
   //-----------------------------------------------
   void set_zoom_factor(const double newZoomFactor);


   //-----------------------------------------------
   // FUNCTION: move
   //
   // DESCRIPTION:
   //    Move center point of map to:
   //           "north"
   //           "south"
   //           "east"
   //           "west"
   //           "neutral"
   //-----------------------------------------------
   void move(const std::string dir);


   //-----------------------------------------------
   // FUNCTION: set_move_factor 
   //
   // DESCRIPTION:
   //    Resets the distance used for map 'move'
   //    operations.
   //-----------------------------------------------
   void set_move_factor(const float newMoveFactor);


   //-----------------------------------------------
   // FUNCTION: set_map_feature
   //
   // DESCRIPTION:
   //    Set the specified map feature ON or OFF.
   //-----------------------------------------------
   void set_map_feature
           (const MAP_FEATURE_T feature,
            const bool          state);

protected:

   void paintEvent(QPaintEvent *event) override;

private:

   int g_width  = 0;
   int g_height = 0;

   const double            deltaDegrees = 15.0;
   const gmsEarthModelType g_earthModel = gmsFlat;
   float                   g_moveAmount = 2.0f;
   double                  g_zoomAmount = 1000.0;

   bool featureIsShown[NUM_MAP_FEATURES] =
            {
                false, // mapLines
                false, // PO_polygons
                false, // DN_polygons
                false, // DN_lines
                false, // text
                false, // cities
                false, // HY_lines
                false  // latLongGrid
            };

   void drawMaps();

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

   void drawPolygonImage
           (Qt::GlobalColor        borderColor,
            Qt::GlobalColor        fillColor,
            gms_2D_ScreenImageType mapImage);

   void drawLatLongGrid();

   void drawImage
           (Qt::GlobalColor        whichColor,
            gms_2D_ScreenImageType theImage);

   void drawIndependentPoints
           (Qt::GlobalColor           whichColor,
            gms_2D_ScreenPolylineType thePoints);

   void annotateMap
           (Qt::GlobalColor     whichColor,
            gmsMapTextArrayType theText);

};

#endif // DCW_LIBRARY_MGR_

/* EOF */

