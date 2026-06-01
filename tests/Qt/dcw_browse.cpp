//-------------------------------------------------------------------------
// File : dcw_browse.cpp
// Date : xx-Feb-00 : initial definition
//        24-Feb-00 : blew away stuff at CSUN.  Start over!
//        26-May-26 : Total pivot to be used by Qt 'dcw_browse' app
//
// Description:
//    A set of utilties to be called by Qt program that exercises the
//    "browse" library of DCW database.  Prove concept of portability.
//-------------------------------------------------------------------------
#include <iostream>

#include <stdlib.h>
#include <stdio.h>

#include <QPainter>
#include <QPaintEvent>
#include <QPointF>
#include <QPolygonF>

#include "dcw_browse.h"


DCW_Browse::DCW_Browse(QWidget *parent)

   : QWidget(parent)

{
   gmsSetMapZoomFactor(14000.0);
}

DCW_Browse::~DCW_Browse()
{
}

//---------------------------------------------
// setMapState
//
// Description:
//---------------------------------------------
void DCW_Browse::setMapState
                    (const gmsBrowseThematicType which_map,
                     const bool                  is_clicked)
{
    g_themeIsShown[which_map] = is_clicked;

    drawMaps();
}

//---------------------------------------------
// clearMapArea 
//
// Description:
//---------------------------------------------
void DCW_Browse::clearMapArea ()

{
    std::cout << "clearMapArea\n";

    QPainter painter(this);

    int w = width();
    int h = height();

    QRect rect(0,0, w, h);

    painter.fillRect(rect, Qt::white);

    std::cout << "clearMapArea - end\n";
}


//---------------------------------------------
// drawMaps 
//
// Description:
//---------------------------------------------
void DCW_Browse::drawMaps ()

{
   std::cout << "drawMaps\n";

   clearMapArea ();

   if (g_theBrowseMap == NULL)
   {
      g_theBrowseMap = new gmsBrowseMapClass (gmsEllipsoid);
   }

   if (g_themeIsShown[gmsBrowse_LibRef])
      drawLibRef ();

   if (g_themeIsShown[gmsBrowse_DN])
      drawDrainage ();

   if (g_themeIsShown[gmsBrowse_PO])
      drawPoliticalAndOceans ();

   if (g_themeIsShown[gmsBrowse_PP])
      drawPopulatedPlaces ();

   drawLatLongGrid ();

   update();

   std::cout << "drawMaps - end\n";
}


//---------------------------------------------
// drawLibRef 
//
// Description:
//---------------------------------------------
void DCW_Browse::drawLibRef()

{
   std::cout << "drawLibRef- begin\n";

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_LibRef);

   drawImage
      (Qt::yellow,
       tempImage);

   update();

   std::cout << "drawLibRef- end\n";
}


//---------------------------------------------
// drawDrainage
//
// Description:
//---------------------------------------------
void DCW_Browse::drawDrainage()

{
   std::cout << "drawDrainage - begin\n";

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_DN);

   drawImage
      (Qt::blue,
       tempImage);

   update();

   std::cout << "drawDrainage- end\n";
}


//---------------------------------------------
// drawPoliticalAndOceans
//
// Description:
//---------------------------------------------
void DCW_Browse::drawPoliticalAndOceans()

{
   std::cout << "drawPoliticalAndOceans - begin\n";

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_PO);

   drawImage
      (Qt::red,
       tempImage);

   update();

   std::cout << "drawPoliticalAndOceans - end\n";
}


//---------------------------------------------
// drawPopulatedPlaces
//
// Description:
//---------------------------------------------
void DCW_Browse::drawPopulatedPlaces ()

{
   std::cout << "Not implemented\n";

   update();
}


//---------------------------------------------
// drawLatLongGrid
//
// Description:
//---------------------------------------------
void DCW_Browse::drawLatLongGrid ()

{
   std::cout << "drawLatLongGrid - begin\n";

   gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetLatitudeGrid();

   drawImage
      (Qt::green,
       tempImage);

   tempImage = g_theBrowseMap->gmsGetLongitudeGrid();

   drawImage
      (Qt::green,
       tempImage);

   update();

   std::cout << "drawLatLongGrid - end\n";
}


//---------------------------------------------
// drawImage 
//
// Description:
//---------------------------------------------
void DCW_Browse::drawImage
               (Qt::GlobalColor        whichColor,
                gms_2D_ScreenImageType theImage,
                bool                   isChecked)

{
   std::cout << "drawImage - begin\n";

   int i, j;

   QPainter painter(this);

   painter.setPen(whichColor);

   for (i = 0; i < theImage.numLines; i++)
   {
      for (j = 0; j < (theImage.imageLines[i].numPoints - 1); j++)
      {
          if ( ! isChecked )
          {
              QLine line(theImage.imageLines[i].points[j].x,
                         theImage.imageLines[i].points[j].y,
                         theImage.imageLines[i].points[j + 1].x,
                         theImage.imageLines[i].points[j + 1].y);

              painter.drawLine(line);
          }
          else if (gmsIsVisibleLine
                         (theImage.imageLines[i].points[j],
                          theImage.imageLines[i].points[j + 1]) )
          {
              QLine line(theImage.imageLines[i].points[j].x,
                         theImage.imageLines[i].points[j].y,
                         theImage.imageLines[i].points[j + 1].x,
                         theImage.imageLines[i].points[j + 1].y);

              painter.drawLine(line);
          }
      }
   }

   std::cout << "drawImage - end\n";
}


//---------------------------------------------
// drawIndependentPoints 
//
// Description:
//---------------------------------------------
void DCW_Browse::drawIndependentPoints
               (Qt::GlobalColor           whichColor,
                gms_2D_ScreenPolylineType thePoints)

{
   std::cout << "drawIndependentPoints - begin\n";

   Q_UNUSED(whichColor);
   Q_UNUSED(thePoints);

   std::cout << "Not implemented\n";

   update();

   std::cout << "drawIndependentPoints - end\n";
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
   std::cout << "paintEvent - begin\n";

   Q_UNUSED(event);

   int w = width();
   int h = height();

   gmsSetWindowDimensions(w, h);

   drawMaps ();

   std::cout << "paintEvent - end\n";
}


//---------------------------------------------
// zoom
//
// Description:
//---------------------------------------------
void DCW_Browse::zoom(const ZOOM_T zoom)
{
   if (zoom == IN)
   {
       gmsZoomIn (g_zoomAmount);
   }
   else
   {
       gmsZoomOut (g_zoomAmount);
   }

   drawMaps ();
}


//---------------------------------------------
// move
//
// Description:
//---------------------------------------------
void DCW_Browse::move(const DIRECTION_T dir)
{
   if (dir == NORTH)
   {
      gmsSet_X_Rotation (g_rotationDeg);
   }
   else if (dir == SOUTH)
   {
      gmsSet_X_Rotation (-g_rotationDeg);
   }
   else if (dir == EAST)
   {
      gmsSet_Y_Rotation (-g_rotationDeg);
   }
   else
   {
      gmsSet_Y_Rotation (g_rotationDeg);
   }

   drawMaps ();
}

/* EOF */

