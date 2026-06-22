//-------------------------------------------------------------------------
// File : dcw_browse.h
// Date : 29-May-26 : initial definition
//        14-Jun-26 : Fold in Tile Ref mechanics
//
// Description:
//    A set of DCW browse utilities for use by the Qt version of the
//    'dcw_browse' application.
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
#ifndef DCW_BROWSE_
#define DCW_BROWSE_

#include <string>

#include <QWidget>

#include <gmsTypesAndConstants.h>
#include <gmsDebugUtil.h>
#include <gmsBrowseMapClass.h>
#include <gmsMapStateMgr.h>
#include <gmsScreenCoordUtils.h>
#include <gmsUtilities.h>
#include <gmsDcwUtilities.h>
#include <gmsTextClass.h>
#include <gmsLatLongGridClass.h>
#include <gmsTileClass.h>

typedef enum
{
    NOAMER,
    EURNASIA,
    SOAMAFR,
    SASAUS,
    //------//
    NUM_TILE_REF_MAPS
} TILE_REF_MAP_T;

typedef enum
{
    mapLines,
    PO_polygons,
    DN_polygons,
    DN_lines,
    text,
    cities,
    HY_lines,
    //-------------//
    NUM_TILE_REF_FEATURES
} TILE_REF_FEATURE_T;


class DCW_Browse : public QWidget
{
    Q_OBJECT

public:

   DCW_Browse(QWidget *parent);

   ~DCW_Browse();

   void init_browse_map
              (const std::string earth_model);

   void set_browse_lat_long_grid_state(const bool is_clicked);

   void set_browse_state
              (const gmsBrowseThematicType which_map,
               const bool                  is_clicked);

   void init_tile_ref_map
              (const TILE_REF_MAP_T    map,
               const gmsEarthModelType model);

   void init_tile_ref_lat_long_grid
              (const gmsEarthModelType model);

   void set_tile_ref_move_factor(const float newMoveFactor);

   void set_tile_ref_map_feature
              (const TILE_REF_MAP_T     map,
               const TILE_REF_FEATURE_T feature,
               const bool               state);

   void set_tile_ref_lat_long_grid(const bool state);

   void set_tile_ref_zoom_factor(const double newZoomFactor);

   void zoom(const std::string z);

   void move(const std::string dir);

protected:

   void paintEvent(QPaintEvent *event) override;

private:

   void draw_browse_maps ();

   void clearMapArea();

   void draw_browse_LibRef ();

   void draw_browse_Drainage ();

   void draw_browse_PoliticalAndOceans ();

   void draw_browse_PopulatedPlaces ();

   void draw_browse_LatLongGrid ();

   //void browse_politicalOceansCallback();

   void draw_browse_Image
           (Qt::GlobalColor        whichColor,
            gms_2D_ScreenImageType theImage);

   void draw_browse_IndependentPoints
           (Qt::GlobalColor           whichColor,
            gms_2D_ScreenPolylineType thePoints);

   gmsEarthModelType browse_model;

   bool g_browse_lat_long_grid_is_on = true;

   bool g_browse_theme_is_shown[Num_Browse_Themes] =
                 { 
                     false, // CO - ONC Compilation Date
                     false, // DV - Data Volume
                     false, // DN - Drainage
                     false, // GR - Geographic Reference
                     false, // DA - Avail of Hypsographic Data
                     false, // IN - ONC Index
                     false, // PO - Political/Oceans
                     false, // PP - Populated Places
                     false  // LibRef
                 };

   double            g_browse_rotationDeg = 20.0;

   gmsBrowseMapClass *g_theBrowseMap = nullptr;

   double            g_browse_zoom_amount = 5000.0;


   const double            tile_ref_deltaDegrees = 15.0;
   const gmsEarthModelType g_tile_ref_earthModel = gmsFlat;
   float                   g_tile_ref_moveAmount = 2.0f;
   double                  g_tile_ref_zoomAmount = 1000.0;

   bool tile_ref_featureIsShown[NUM_TILE_REF_MAPS][NUM_TILE_REF_FEATURES];

   bool tile_ref_lat_long_grid_is_shown = false;

   gmsLatLongGridClass     *tile_ref_lat_long_grid = nullptr;

   gmsTileClass            *tile_ref_map[NUM_TILE_REF_MAPS] =
                              {
                                  nullptr, // NOAMER
                                  nullptr, // EURNASIA
                                  nullptr, // SOAMAFR
                                  nullptr  // SASAUS
                              };

   void draw_tile_ref_maps();

   void draw_tile_ref_UncheckedImage();

   void dlg_tile_ref_ProcZoomControl();

   void dlg_tile_ref_ProcMoveControl();

   void draw_tile_ref_MapLines(const TILE_REF_MAP_T map);

   void draw_tile_ref_PO_Polygons(const TILE_REF_MAP_T map);

   void draw_tile_ref_DN_Polygons(const TILE_REF_MAP_T map);

   void draw_tile_ref_DN_Lines(const TILE_REF_MAP_T map);

   void draw_tile_ref_HY_Lines(const TILE_REF_MAP_T map);

   void draw_tile_ref_TextOfMap(const TILE_REF_MAP_T map);

   void draw_tile_ref_CitiesOfMap(const TILE_REF_MAP_T map);

   void setNewFont ();

   void drawPolygonImage
           (Qt::GlobalColor        borderColor,
            Qt::GlobalColor        fillColor,
            gms_2D_ScreenImageType mapImage);

   void draw_tile_ref_LatLongGrid();

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

#endif

/* EOF */

