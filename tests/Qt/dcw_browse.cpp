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
#include <gmsDcwUtilities.h>

DCW_Browse::DCW_Browse
               (const std::string earth_model,
                QWidget           *parent)

   : QWidget(parent)

{
   std::cout << "------------------ DCW_Browse - begin ---------\n";

   if ( not g_theBrowseMap )
   {
       gmsEarthModelType model;

       model = toEarthModel(earth_model);

       g_theBrowseMap = new gmsBrowseMapClass (model);

       if ( not g_theBrowseMap )
       {
           std::cout << "***> ERROR - allocate browse map fail\n";

           std::exit(99);
       }

       gmsSetMapZoomFactor(14000.0);
   }

   std::cout << "------------------ DCW_Browse - end ---------\n";
}

DCW_Browse::~DCW_Browse()
{
   if (g_theBrowseMap)
   {
       delete g_theBrowseMap;

       g_theBrowseMap = nullptr;
   }
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
// drawMaps 
//
// Description:
//---------------------------------------------
void DCW_Browse::drawMaps ()
{
   std::cout << "-------------- drawMaps - begin ----------\n";

   clearMapArea ();

   if (g_themeIsShown[gmsBrowse_LibRef])
      drawLibRef ();

   if (g_themeIsShown[gmsBrowse_DN])
      drawDrainage ();

   if (g_themeIsShown[gmsBrowse_PO])
      drawPoliticalAndOceans ();

   if (g_themeIsShown[gmsBrowse_PP])
      drawPopulatedPlaces ();

   drawLatLongGrid ();

   std::cout << "------------------ drawMaps - end -------\n";
}


//---------------------------------------------
// drawLibRef 
//
// Description:
//---------------------------------------------
void DCW_Browse::drawLibRef()

{
   std::cout << "drawLibRef - begin\n";

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

   std::cout << "drawPoliticalAndOceans - end\n";
}


//---------------------------------------------
// drawPopulatedPlaces
//
// Description:
//---------------------------------------------
void DCW_Browse::drawPopulatedPlaces ()

{
   std::cout << "draw Populated Places - Not implemented\n";
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

   int i;
   int j;

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
   std::cout << "vvvvvvvvvvvvvvvv paintEvent - begin vvvvvvvvvvvvvvvv\n";

   Q_UNUSED(event);

   int w = width();
   int h = height();

   gmsSetWindowDimensions(w, h);

   drawMaps ();

   std::cout << "^^^^^^^^^^^^^^^^ paintEvent - end ^^^^^^^^^^^^^^^^^^\n";
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

   update();
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

   update();
}

/* EOF */

