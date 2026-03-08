//----------------------------------------------------------------------------
// File : soamafr_demo.cpp
// Date : 06-Feb-00 : initial definition - evolve from "polygon_maps"
//
// Description:
//    Windows app used to obtain screen shots for MS thesis.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>

#include <soamafr_demo_resource.h>

#include <stdio.h>
#include <gmsColors.h>
#include <gmsDebugUtil.h>
#include <gmsBitmapClass.h>
#include <gmsLatLongGridClass.h>
#include <gmsTileClass.h>
#include <gmsMapStateMgr.h>
#include <gmsScreenCoordUtils.h>
#include <gmsUtilities.h>


//--------------------
// Global Variables
//--------------------

static HINSTANCE               g_hInstance       = (HINSTANCE) 0;

static HWND                    g_windowHandle    = (HWND) 0;

static HWND                    g_zoomDlgHandle   = (HWND) 0;

static HWND                    g_moveDlgHandle   = (HWND) 0;

static HWND                    g_saveAsDlgHandle = (HWND) 0;

static HFONT                   g_handleToCurrentFont = (HFONT) 0;

static int                     g_width = 460;

static int                     g_height = 400;

static const double            deltaDegrees = 15.0;

static const gmsEarthModelType g_earthModel = gmsFlat;

static gmsLatLongGridClass     theGrid
                                  (deltaDegrees,
                                   g_earthModel);

static gmsTileClass            theTileMap
                                  (g_earthModel,
                                   'N', 'G');

static float         g_moveAmount = 2.0f;

static double        g_zoomAmount = 1000.0;

static bool          g_latLongGridIsShown  = true;

static bool          g_mapLinesAreShown    = true;

static bool          g_PO_polygonsAreShown = false;

static bool          g_DN_polygonsAreShown = false;

static bool          g_DN_linesAreShown    = false;

static bool          g_textIsShown         = false;

static bool          g_citiesAreShown      = false;

static bool          g_HY_linesAreShown    = false;


//--------------------
// Function Prototypes
//--------------------

static LRESULT CALLBACK WndProc
                           (HWND   windowHandle,
                            UINT   theMsg,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcZoomControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcMoveControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static void parseMenuButton
               (int whichCommand);

static void performPaint();

static void drawMapLines
               (HDC hDC);

static void draw_PO_Polygons
               (HDC hDC);

static void draw_DN_Polygons
               (HDC hDC);

static void draw_DN_Lines
               (HDC hDC);

static void draw_HY_Lines
               (HDC hDC);

static void drawTextOfMap
               (HDC hDC);

static void drawCitiesOfMap
               (HDC hDC);

static void drawLatLongGrid
               (HDC hDC);

static void doScreenCapture ();

static void setNewFont ();



//-------------------------------------------------
// WinMain
//
// Desciption:
//    Entry point to the program.  Windows calls
//    this routine.
//-------------------------------------------------
int WINAPI WinMain
              (HINSTANCE hInstance,
               HINSTANCE hPrevInst,
               LPSTR     argv,
               int       argc)

{
       WNDCLASSEXA wc;
       MSG         theMsg;
       char        *instanceName = {"soamafr_demo"};
       BOOL        isOk;
       LOGBRUSH    brushAttributes;
       HBRUSH      newBrush;

   g_hInstance = hInstance;

   ZeroMemory (&wc, sizeof(wc));

   brushAttributes.lbColor = BLUE;
   brushAttributes.lbHatch = 0;
   brushAttributes.lbStyle = BS_SOLID;

   newBrush = CreateBrushIndirect
                 (&brushAttributes);

   wc.cbSize        = sizeof (wc);
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hbrBackground = newBrush; // (HBRUSH) GetStockObject (WHITE_BRUSH);
   wc.hCursor       = LoadCursor (hInstance, IDC_ARROW);
   wc.hIcon         = LoadIcon (hInstance, IDI_APPLICATION);
   wc.hInstance     = hInstance;
   wc.lpfnWndProc   = WndProc;
   wc.lpszClassName = instanceName;
   wc.lpszMenuName  = MAKEINTRESOURCE(IDR_SOAMAFR_Main_Menu);
   wc.style         = CS_VREDRAW | CS_HREDRAW;

   isOk = RegisterClassExA (&wc);

   if ( ! isOk )
      {
       MessageBoxA
         (NULL,
          "Unable to execute on this platform",
          "Unable to Execute",
          MB_ICONERROR);

       return -1;
      }

   g_windowHandle = CreateWindowExA
                      (0,
                       instanceName,
                       "Africa - demo",
                       WS_OVERLAPPEDWINDOW,
                       100, 100,           // (x, y) location
                       g_width, g_height,  // width/height of window
                       NULL,
                       NULL,
                       hInstance,
                       NULL);

   if ( ! g_windowHandle )
      {
       MessageBoxA
          (NULL,
           "Unable to create window",
           "Unable to execute",
           MB_ICONERROR);

       return -1;
      }

   ShowWindow (g_windowHandle, argc);

   UpdateWindow (g_windowHandle);

   //
   // Do message pump ...
   //
   BOOL isForDialog;

   while (GetMessage
            (&theMsg,
             NULL,  // use NULL here!  Don't use g_windowHandle!
             0, 0) )
      {
       isForDialog = FALSE;

       if (g_zoomDlgHandle)
          isForDialog = IsDialogMessage (g_zoomDlgHandle, &theMsg);

       if ( ! isForDialog )
          {
           TranslateMessage (&theMsg);

           DispatchMessage (&theMsg);
          }
      }

   theGrid.~gmsLatLongGridClass();

   return theMsg.wParam;
}


            //--------------------
            //    Functions
            //--------------------


//-------------------------------------------------
// WndProc
//
// Desciption:
//    Callback routine to handle events to the
//    window.
//-------------------------------------------------
static LRESULT CALLBACK WndProc
                           (HWND   windowHandle,
                            UINT   theMsg,
                            WPARAM wParam,
                            LPARAM lParam)

{
         LRESULT theResult = 0;

   switch (theMsg)
      {
       case WM_CREATE:
          {
                    const double initialZoomFactor = 3000.0;

           gmsSetMapZoomFactor (initialZoomFactor);

           gmsMoveFlatMap (gmsMoveNorth, (3 * g_moveAmount));

           gmsMoveFlatMap (gmsMoveEast, (4 * g_moveAmount));

           break;
          }

       case WM_SIZE:
          {
           g_width = LOWORD (lParam);

           g_height = HIWORD (lParam);

           gmsSetWindowDimensions
              (g_width,
               g_height);

           break;
          }

      case WM_PAINT:
         {
          performPaint ();

          break;
         }

      case WM_DESTROY:
         {
          PostQuitMessage (0);

          break;
         }

      case WM_COMMAND:
         {
          parseMenuButton (LOWORD(wParam));

          break;
         }

      default:
         {
          theResult = DefWindowProc
                       (windowHandle,
                        theMsg,
                        wParam,
                        lParam);

          break;
         }
      }

   return theResult;
}


//-------------------------------------------------
// parseMenuButton
//
// Desciption:
//    Routine to parse the selections from the
//    main menu bar.
//-------------------------------------------------
static void parseMenuButton
               (int whichCommand)

{
   switch (whichCommand)
      {
       case ID_FILE_SAVE_AS:
          {
           doScreenCapture ();

           break;
          }

       case ID_Set_Font:
          {
           setNewFont ();

           break;
          }

       case ID_FILE_EXIT:
          {
           PostQuitMessage (0);

           break;
          }

       case ID_ZOOM_IN:
          {
           gmsZoomIn (g_zoomAmount);

           break;
          }

       case ID_ZOOM_OUT:
          {
           gmsZoomOut(g_zoomAmount);

           break;
          }

       case ID_MOVE_NORTH:
          {
           gmsMoveFlatMap (gmsMoveNorth, g_moveAmount);

           break;
          }

       case ID_MOVE_SOUTH:
          {
           gmsMoveFlatMap (gmsMoveSouth, g_moveAmount);

           break;
          }

       case ID_MOVE_EAST:
          {
           gmsMoveFlatMap (gmsMoveEast, g_moveAmount);

           break;
          }

       case ID_MOVE_WEST:
          {
           gmsMoveFlatMap (gmsMoveWest, g_moveAmount);

           break;
          }

       case ID_NEUTRAL_ORIENTATION:
          {
           gmsResetToNeutralOrientation();

           break;
          }

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

       case ID_LatLongGrid_OnOff:
          {
           g_latLongGridIsShown = ! g_latLongGridIsShown;

           break;
          }

       case ID_Map_Borders_OnOff:
          {
           g_mapLinesAreShown = ! g_mapLinesAreShown;

           break;
          }

       case ID_PO_Polygons_OnOff:
          {
           g_PO_polygonsAreShown = ! g_PO_polygonsAreShown;

           break;
          }

       case ID_DN_Polygons_OnOff:
          {
           g_DN_polygonsAreShown = ! g_DN_polygonsAreShown;

           break;
          }

       case ID_DN_Lines_OnOff:
          {
           g_DN_linesAreShown = ! g_DN_linesAreShown;

           break;
          }

       case ID_Text_OnOff:
          {
           g_textIsShown = ! g_textIsShown;

           break;
          }

       case ID_Cities_OnOff:
          {
           g_citiesAreShown = ! g_citiesAreShown;

           break;
          }

       case ID_HY_OnOff:
          {
           g_HY_linesAreShown = ! g_HY_linesAreShown;

           break;
          }

       default:
          {
           MessageBoxA
             (NULL,
              "Unknown selection",
              "Not Implemented?",
              MB_OK);

           break;
          }
      }

   InvalidateRect
      (g_windowHandle, 
       NULL,  // the whole rectangle
       TRUE); // erase?
}


//-------------------------------------------------
// dlgProcZoomControl
//
// Desciption:
//    This routine is the callback for the "Zoom-
//    Control-Dialog".  The operator is allowed to
//    enter a zoom-factor.  This routine catches
//    the input and sets the new zoom factor.
//-------------------------------------------------
static BOOL CALLBACK dlgProcZoomControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const int maxNumChars = 16;
         char      charBuffer[maxNumChars];
         UINT      numChars;
         double    newZoomFactor;

   if (message == WM_INITDIALOG)
      {
       sprintf(charBuffer, "%.2lf", g_zoomAmount);

       SetDlgItemTextA
          (dialogHandle,
           IDC_Zoom_Factor_Text,
           charBuffer);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_zoomDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case ID_OK_Zoom_Ctrl:
              {
               numChars = GetDlgItemTextA
                             (dialogHandle,
                              IDC_Zoom_Factor_Text,
                              charBuffer,
                              maxNumChars - 1);

               sscanf(charBuffer, "%lf", &newZoomFactor);

               gmsSetMapZoomFactor (newZoomFactor);

               InvalidateRect
                  (g_windowHandle, 
                   NULL,  // the whole rectangle
                   TRUE); // erase?

               break;
              }

           case ID_CANCEL_Zoom_Ctrl:
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_zoomDlgHandle = (HWND) 0;

               break;
              }
          }

       return TRUE;
      }

   return FALSE;
}


//-------------------------------------------------
// dlgProcMoveControl
//
// Desciption:
//    This routine is the callback for the
//    "Features-->Text" dialog.  The operator is
//    allowed to specify the text features to be
//    drawn on the map.
//-------------------------------------------------
static BOOL CALLBACK dlgProcMoveControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const int maxNumChars = 16;
         char      charBuffer[maxNumChars];
         UINT      numChars;

   if (message == WM_INITDIALOG)
      {
       sprintf(charBuffer, "%.2f", g_moveAmount);

       SetDlgItemTextA
          (dialogHandle,
           IDC_Move_Value_Text,
           charBuffer);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_moveDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case ID_OK_Move:
              {
               numChars = GetDlgItemTextA
                             (dialogHandle,
                              IDC_Move_Value_Text,
                              charBuffer,
                              maxNumChars - 1);

               sscanf(charBuffer, "%f", &g_moveAmount);

               InvalidateRect
                  (g_windowHandle, 
                   NULL,  // the whole rectangle
                   TRUE); // erase?

               break;
              }

           case ID_CANCEL_Move:
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_moveDlgHandle = (HWND) 0;

               break;
              }
          }

       return TRUE;
      }

   return FALSE;
}


//-------------------------------------------------
// performPaint
//
// Desciption:
//    Routine to "paint" the graphics into the
//    window.  This routine is invoked due to a
//    WM_PAINT event.  This indicates that the
//    "canvas" needs to be refreshed.
//-------------------------------------------------
static void performPaint()

{
       HDC         hDC;
       PAINTSTRUCT paintStruct;

   hDC = BeginPaint (g_windowHandle, &paintStruct);

   if (g_handleToCurrentFont != (HFONT) 0)
      SelectObject (hDC, g_handleToCurrentFont);

   if (g_PO_polygonsAreShown)
      draw_PO_Polygons (hDC);

   if (g_DN_polygonsAreShown)
      draw_DN_Polygons (hDC);

   if (g_DN_linesAreShown)
      draw_DN_Lines (hDC);

   if (g_HY_linesAreShown)
      draw_HY_Lines (hDC);

   if (g_mapLinesAreShown)
      drawMapLines (hDC);

   if (g_textIsShown)
      drawTextOfMap (hDC);

   if (g_citiesAreShown)
      drawCitiesOfMap (hDC);

   if (g_latLongGridIsShown)
      drawLatLongGrid (hDC);

   EndPaint (g_windowHandle, &paintStruct);
}


//-------------------------------------------------
// drawIndependentPoints
//
// Desciption:
//-------------------------------------------------
static void drawIndependentPoints
               (HDC                       hDC,
                COLORREF                  whichColor,
                gms_2D_ScreenPolylineType thePoints)

{
         HPEN      newPen;
         HGDIOBJ   oldPen;
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
}


//-------------------------------------------------
// annotateMap
//
// Desciption:
//-------------------------------------------------
static void annotateMap
               (HDC                 hDC,
                COLORREF            whichColor,
                gmsMapTextArrayType theText)

{
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
}


//-------------------------------------------------
// drawImage
//
// Desciption:
//-------------------------------------------------
static void drawImage
               (HDC                    hDC,
                COLORREF               whichColor,
                gms_2D_ScreenImageType mapImage)

{
         int       i, j;
         HPEN      newPen;
         HGDIOBJ   oldPen;
         const int onePixelWide = 0;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                whichColor);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < mapImage.numLines; i++)
      for (j = 0; j < (mapImage.imageLines[i].numPoints - 1); j++)
         {
          if (gmsIsVisibleLine
                 (mapImage.imageLines[i].points[j],
                  mapImage.imageLines[i].points[j + 1]) )
             {
              MoveToEx
                 (hDC,
                  mapImage.imageLines[i].points[j].x,
                  mapImage.imageLines[i].points[j].y,
                  NULL);

              LineTo
                 (hDC,
                  mapImage.imageLines[i].points[j + 1].x,
                  mapImage.imageLines[i].points[j + 1].y);
             }
         }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// drawImage
//
// Desciption:
//-------------------------------------------------
static void drawUncheckedImage
               (HDC                    hDC,
                COLORREF               whichColor,
                gms_2D_ScreenImageType mapImage)

{
         int       i, j;
         HPEN      newPen;
         HGDIOBJ   oldPen;
         const int onePixelWide = 0;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                whichColor);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < mapImage.numLines; i++)
      for (j = 0; j < (mapImage.imageLines[i].numPoints - 1); j++)
         {
          MoveToEx
             (hDC,
              mapImage.imageLines[i].points[j].x,
              mapImage.imageLines[i].points[j].y,
              NULL);

          LineTo
             (hDC,
              mapImage.imageLines[i].points[j + 1].x,
              mapImage.imageLines[i].points[j + 1].y);
         }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// drawPolygonImage
//
// Desciption:
//-------------------------------------------------
static void drawPolygonImage
               (HDC                    hDC,
                COLORREF               borderColor,
                COLORREF               fillColor,
                gms_2D_ScreenImageType mapImage)

{
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
}


//-------------------------------------------------
// drawMapLines
//
// Desciption:
//-------------------------------------------------
static void drawMapLines
               (HDC hDC)

{
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
          (hDC,
           RED,
           mapImage);
      }
}


//-------------------------------------------------
// draw_PO_Polygons
//
// Desciption:
//-------------------------------------------------
static void draw_PO_Polygons
               (HDC hDC)

{
         int                    i;
         int                    numPolygonObjs;
         gms_PO_PolygonMapClass **ptrToPolygonObjs;
         gms_2D_ScreenImageType polygonImage;

   ptrToPolygonObjs = theTileMap.gmsGetTile_PO_Polygons (numPolygonObjs);

   for (i = 0; i < numPolygonObjs; i++)
      {
       polygonImage = ptrToPolygonObjs[i]->gmsGetLandAreas ();

       drawPolygonImage
          (hDC,
           DARK_GREEN,
           DARK_GREEN,
           polygonImage);
      }
}


//-------------------------------------------------
// draw_DN_Polygons
//
// Desciption:
//-------------------------------------------------
static void draw_DN_Polygons
               (HDC hDC)

{
         int                    i;
         int                    numPolygonObjs;
         gms_DN_PolygonMapClass **ptrToPolygonObjs;
         gms_2D_ScreenImageType theImage;

   ptrToPolygonObjs = theTileMap.gmsGetTile_DN_Polygons (numPolygonObjs);

   for (i = 0; i < numPolygonObjs; i++)
      {
       theImage = ptrToPolygonObjs[i]->gmsGetInlandWaterAreas ();

       drawPolygonImage
          (hDC,
           DODGER_BLUE,
           DODGER_BLUE,
           theImage);
      }
}


//-------------------------------------------------
// draw_DN_Lines
//
// Desciption:
//-------------------------------------------------
static void draw_DN_Lines
               (HDC hDC)

{
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
          (hDC,
           BLUE,
           theImage);
      }
}


//-------------------------------------------------
// draw_HY_Lines
//
// Desciption:
//-------------------------------------------------
static void draw_HY_Lines
               (HDC hDC)

{
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
          (hDC,
           YELLOW,
           theImage);
      }
}


//-------------------------------------------------
// drawTextOfMap
//
// Desciption:
//-------------------------------------------------
static void drawTextOfMap
               (HDC hDC)

{
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

       textData.numTextRecords = textData.numTextRecords / 2;

       annotateMap
          (hDC,
           BLACK,
           textData);
      }
}


//-------------------------------------------------
// drawCitiesOfMap
//
// Desciption:
//-------------------------------------------------
static void drawCitiesOfMap
               (HDC hDC)

{
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
          (hDC,
           BLACK,
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
          (hDC,
           BLACK,
           textData);
      }
}


//-------------------------------------------------
// drawLatLongGrid
//
// Desciption:
//    Routine that draws lat/long lines to enhance
//    the ellipsoid aspect of the globe.
//-------------------------------------------------
static void drawLatLongGrid
               (HDC hDC)

{
         gms_2D_ScreenImageType gridImage;

   //
   // Draw the Latitude grid lines
   //
   gridImage = theGrid.gmsGetLatitudeGrid();

   drawUncheckedImage
      (hDC,
       GREEN,
       gridImage);

   //
   // Draw the longitude grid lines
   //
   gridImage = theGrid.gmsGetLongitudeGrid();

   drawUncheckedImage
      (hDC,
       GREEN,
       gridImage);
}


//-------------------------------------------------
// doScreenCapture
//
// Desciption:
//-------------------------------------------------
static void doScreenCapture ()

{
         static bool          isInitialized = false;
         static OPENFILENAMEA infoFromSaveAsDlg; // Regular ASCII
         static char          *fileFilter   =
            { "Bitmap Files (*.dib)\0*.dib\0 GIF Files (*.gif)\0*.gif\0\0" };
         static char          *defExtension = { "dib" };
         static char          fileName[MAX_PATH];
         static char          fileTitle[MAX_PATH];
         static BOOL          dlgIsOk;

   if ( ! isInitialized )
      {
       ZeroMemory
          ( (void *) &infoFromSaveAsDlg,
            sizeof(infoFromSaveAsDlg) );

       infoFromSaveAsDlg.lStructSize       = sizeof (OPENFILENAMEA);
       infoFromSaveAsDlg.hwndOwner         = g_windowHandle;
       infoFromSaveAsDlg.hInstance         = NULL;
       infoFromSaveAsDlg.lpstrFilter       = fileFilter;
       infoFromSaveAsDlg.lpstrCustomFilter = NULL;
       infoFromSaveAsDlg.nMaxCustFilter    = 0;
       infoFromSaveAsDlg.nFilterIndex      = 0;;
       infoFromSaveAsDlg.lpstrFile         = NULL;
       infoFromSaveAsDlg.nMaxFile          = MAX_PATH;
       infoFromSaveAsDlg.lpstrFileTitle    = NULL;
       infoFromSaveAsDlg.nMaxFileTitle     = MAX_PATH;
       infoFromSaveAsDlg.lpstrInitialDir   = NULL;
       infoFromSaveAsDlg.lpstrTitle        = NULL;
       infoFromSaveAsDlg.Flags             = 0;
       infoFromSaveAsDlg.nFileOffset       = 0;
       infoFromSaveAsDlg.nFileExtension    = 0;
       infoFromSaveAsDlg.lpstrDefExt       = defExtension;
       infoFromSaveAsDlg.lCustData         = (LONG) 0;
       infoFromSaveAsDlg.lpfnHook          = NULL;
       infoFromSaveAsDlg.lpTemplateName    = NULL;

       isInitialized = true;
      }

   infoFromSaveAsDlg.lpstrFile      = fileName;
   infoFromSaveAsDlg.lpstrFileTitle = fileTitle;

   dlgIsOk = GetSaveFileNameA (&infoFromSaveAsDlg);

   if ( ! dlgIsOk )
      {
       return;  // i.e. CANCEL
      }

   {
         gmsBitmapClass theBitmap(g_windowHandle);

    theBitmap.gmsSaveBitmap (fileName);

    theBitmap.gmsPrintContentsOfBitmap("C:\\Temp\\test_bitmap.txt");
   }

   MessageBoxA
      (NULL,
       fileName,  // <--- full path + file name
       fileTitle, // <--- file name only
       MB_OK);
}


//-------------------------------------------------
// setNewFont
//
// Desciption:
//-------------------------------------------------
static void setNewFont ()

{
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
}
