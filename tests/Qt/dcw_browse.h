//-------------------------------------------------------------------------
// File : dcw_browse.h
// Date : 29-May-26 : initial definition
//
// Description:
//    A set of DCW browse utilities for use by the Qt version of the
//    'dcw_browse' application.
//-------------------------------------------------------------------------
#ifndef DCW_BROWSE_
#define DCW_BROWSE_

#include <QWidget>

#include <gmsDebugUtil.h>
#include <gmsBrowseMapClass.h>
#include <gmsMapStateMgr.h>
#include <gmsScreenCoordUtils.h>
#include <gmsUtilities.h>


class DCW_Browse : public QWidget
{
    Q_OBJECT

public:

   DCW_Browse
      (const std::string earth_model,
       QWidget           *parent = nullptr);

   ~DCW_Browse();

   void setLongLongGridState(const bool is_clicked);

   void setMapState(const gmsBrowseThematicType which_map,
                    const bool                  is_clicked);

   void zoom(const std::string z);

   void move(const std::string dir);

protected:

   void paintEvent(QPaintEvent *event) override;

private:

   void drawMaps ();

   void clearMapArea();

   void drawLibRef ();

   void drawDrainage ();

   void drawPoliticalAndOceans ();

   void drawPopulatedPlaces ();

   void drawLatLongGrid ();

   void politicalOceansCallback();

   void drawImage
           (Qt::GlobalColor        whichColor,
            gms_2D_ScreenImageType theImage);

   void drawIndependentPoints
           (Qt::GlobalColor           whichColor,
            gms_2D_ScreenPolylineType thePoints);

   gmsEarthModelType model;

   bool g_latLongGridIsOn = true;

   bool g_themeIsShown[Num_Browse_Themes] =
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

   double            g_rotationDeg = 20.0;

   gmsBrowseMapClass *g_theBrowseMap = nullptr;

   double            g_zoomAmount = 5000.0;

};

#endif

/* EOF */

