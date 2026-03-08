//----------------------------------------------------------------------------
// File : polygon_maps.cpp
// Date : 25-Jan-00 : initial definition - evolve from "po_maps"
//        29-Jan-00 : switch to flat-earth model for better zoom-in
//
// Description:
//    Windows app to test/exercise routines that generate polygons of
//    maps.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>

#include <polygon_maps_resource.h>

#include <gmsFeatureTable.h>

#include <stdio.h>
#include <gmsColors.h>
#include <gmsDebugUtil.h>
#include <gmsBitmapClass.h>
#include <gmsMapClass.h>
#include <gms_PO_PolygonMapClass.h>
#include <gms_DN_PolygonMapClass.h>
#include <gmsLatLongGridClass.h>
#include <gmsMapStateMgr.h>
#include <gmsUtilities.h>


//--------------------
// Global Variables
//--------------------

static HINSTANCE     g_hInstance    = (HINSTANCE) 0;

static HWND          g_windowHandle = (HWND) 0;

static HWND          g_zoomDlgHandle = (HWND) 0;

static HWND          g_saveAsDlgHandle = (HWND) 0;

static int           g_width = 450;

static int           g_height = 400;

static const double  deltaDegrees = 15.0;

static const gmsEarthModelType g_earthModel = gmsFlat;

static gmsLatLongGridClass theGrid
                              (deltaDegrees,
                               g_earthModel);

static double        g_rotationDeg = 20.0;

static double        g_zoomAmount = 1000.0;

static bool          g_libRefIsShown = true;

static bool          g_latLongGridIsShown = true;

static bool          g_polygonMapIsShown = false;

static bool          g_linesOfSubtileAreShown = false;

static bool          g_linesOf_NG_AreShown = false;

static bool          g_polygonsOfSubtileAreShown = false;

static bool          g_DN_isShown = false;

static bool          g_HY_PolygonsAreShown = false;

static bool          g_HY_LinesAreShown = false;

static bool          g_PP_isShown = false;

static const int      Max_Colors  = 9;

static const COLORREF colors[Max_Colors] = { ORANGE,
                                             DARK_GREEN,
                                             DODGER_BLUE,
                                             pink,
                                             BLUE,
                                             CYAN,
                                             RED,
                                             GREEN,
                                             DARK_BLUE };

static featureTableType  *g_PO_AFT = (featureTableType *) NULL;

static featureTableType  *g_PO_LFT = (featureTableType *) NULL;

static featureTableType  *g_HY_LFT = (featureTableType *) NULL;

static const             numSubtiles = 9;

static const char        *subTiles[numSubtiles] = { {"11"},
                                                    {"12"},
                                                    {"13"},
                                                    {"21"},
                                                    {"22"},
                                                    {"23"},
                                                    {"31"},
                                                    {"32"},
                                                    {"33"} };


//--------------------
// Function Prototypes
//--------------------

static LRESULT CALLBACK WndProc
                           (HWND   windowHandle,
                            UINT   theMsg,
                            WPARAM wParam,
                            LPARAM lParam);

static void parseMenuButton
               (int whichCommand);

static BOOL CALLBACK dlgProcZoomControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcTextFeatures
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static void performPaint();

static void drawLibrefMap
               (HDC hDC);

static void drawPolygonMap
               (HDC hDC);

static void drawLinesOfOneSubtile
               (HDC        hDC,
                const char *themeSubtilePath);

static void draw_PO_LinesOfSubtile_NG
               (HDC hDC);

static void draw_HY_LinesOfSubtile_NG
               (HDC hDC);

static void drawPolygonsOfOneSubtile
               (HDC        hDC,
                const char *themeSubtilePath);

static void drawLatLongGrid
               (HDC hDC);

static void draw_DN_Polygons
               (HDC hDC);

static void draw_HY_Polygons
               (HDC hDC);

static void draw_PP_Polygons
               (HDC hDC);

static bool isVisibleLine
               (gms_2D_ScreenPointType point_1,
                gms_2D_ScreenPointType point_2);

static void doScreenCapture ();



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
       char        *instanceName = {"polygon_maps_demo"};
       BOOL        isOk;
       LOGBRUSH    brushAttributes;
       HBRUSH      newBrush;

   g_hInstance = hInstance;

   //
   // Build the window classification record
   //
   ZeroMemory (&wc, sizeof(wc));

   brushAttributes.lbColor = DODGER_BLUE;
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
   wc.lpszMenuName  = MAKEINTRESOURCE(IDR_Polygon_Maps_Main_Menu);
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
                       "Polygon Maps : Ellipsoid Model",
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

   //
   // Display the window
   //
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
           if (g_PO_AFT == NULL)
              g_PO_AFT = gmsGetFeatureTable
                           (gms_PO,
                            gmsGetDcwFullPath("PO/POAREA.AFT"),
                            gmsArea);

           if (g_PO_LFT == NULL)
              g_PO_LFT = gmsGetFeatureTable
                           (gms_PO,
                            gmsGetDcwFullPath("PO/POLINE.LFT"),
                            gmsLine);

           if (g_HY_LFT == NULL)
              g_HY_LFT = gmsGetFeatureTable
                           (gms_HY,
                            gmsGetDcwFullPath("HY/HYLINE.LFT"),
                            gmsLine);

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

       case ID_ROTATE_NORTH:
          {
           if (g_earthModel == gmsFlat)

              gmsMoveFlatMap (gmsMoveNorth, 2.0);

           else

              gmsSet_X_Rotation (g_rotationDeg);

           break;
          }

       case ID_ROTATE_SOUTH:
          {
           if (g_earthModel == gmsFlat)

              gmsMoveFlatMap (gmsMoveSouth, 2.0);

           else

              gmsSet_X_Rotation (-g_rotationDeg);

           break;
          }

       case ID_ROTATE_EAST:
          {
           if (g_earthModel == gmsFlat)

              gmsMoveFlatMap (gmsMoveEast, 2.0);

           else

              gmsSet_Y_Rotation (-g_rotationDeg);

           break;
          }

       case ID_ROTATE_WEST:
          {
           if (g_earthModel == gmsFlat)

              gmsMoveFlatMap (gmsMoveWest, 2.0);

           else

              gmsSet_Y_Rotation (g_rotationDeg);

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

       case ID_LibRef_OnOff:
          {
           g_libRefIsShown = ! g_libRefIsShown;

           break;
          }

       case ID_LatLongGrid:
          {
           g_latLongGridIsShown = ! g_latLongGridIsShown;

           break;
          }

       case ID_Polygon_Map:
          {
           g_polygonMapIsShown = !g_polygonMapIsShown;

           break;
          }

       case ID_Subtile_Lines:
          {
           g_linesOfSubtileAreShown = !g_linesOfSubtileAreShown;

           break;
          }

       case ID_Lines_Of_NG:
          {
           g_linesOf_NG_AreShown = !g_linesOf_NG_AreShown;

           break;
          }

       case ID_Subtile_Polygons:
          {
           g_polygonsOfSubtileAreShown = ! g_polygonsOfSubtileAreShown;

           break;
          }

       case ID_DN_OnOff:
          {
           g_DN_isShown = ! g_DN_isShown;

           break;
          }

       case ID_HY_OnOff:
          {
           g_HY_PolygonsAreShown = ! g_HY_PolygonsAreShown;

           break;
          }

       case ID_PP_OnOff:
          {
           g_PP_isShown = ! g_PP_isShown;

           break;
          }

       case ID_HY_Lines_OnOff:
          {
           g_HY_LinesAreShown = ! g_HY_LinesAreShown;

           break;
          }

       case ID_FILE_SAVE_AS:
          {
           doScreenCapture ();

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
// dlgProcTextFeatures
//
// Desciption:
//    This routine is the callback for the
//    "Features-->Text" dialog.  The operator is
//    allowed to specify the text features to be
//    drawn on the map.
//-------------------------------------------------
static BOOL CALLBACK dlgProcTextFeatures
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
   if (message == WM_INITDIALOG)
      {
       return true;
      }

   else if (message == WM_COMMAND)
      {
       switch (LOWORD(wParam))
          {
           case ID_Text_Features_OK:
              {
               return true;
              }

           case ID_Text_Features_CANCEL:
           case IDCANCEL:
              {
               EndDialog (dialogHandle, 0);

               return true;
              }
          }
      }

   return false;
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

   if (g_polygonMapIsShown)
      drawPolygonMap (hDC);

   if (g_polygonsOfSubtileAreShown)
      drawPolygonsOfOneSubtile
         (hDC,
          "PO/N/G/21");

   if (g_linesOfSubtileAreShown)
      drawLinesOfOneSubtile
         (hDC,
          "PO/N/G/21");

   if (g_libRefIsShown)
      drawLibrefMap (hDC);

   if (g_linesOf_NG_AreShown)
      draw_PO_LinesOfSubtile_NG (hDC);

   if (g_latLongGridIsShown)
      drawLatLongGrid (hDC);

   if (g_DN_isShown)
      draw_DN_Polygons (hDC);

   if (g_HY_PolygonsAreShown)
      draw_HY_Polygons (hDC);

   if (g_HY_LinesAreShown)
      draw_HY_LinesOfSubtile_NG (hDC);

   if (g_PP_isShown)
      draw_PP_Polygons (hDC);

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
          if (isVisibleLine
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
// drawLibrefMap
//
// Desciption:
//    Routine that draws the points from the edge
//    table
//-------------------------------------------------
static void drawLibrefMap
               (HDC hDC)

{
         gms_2D_ScreenImageType mapImage;
         static gmsMapClass     *ptrToLibRefMap = NULL;

   if (ptrToLibRefMap == NULL)
      {
       ptrToLibRefMap = new gmsMapClass
                               (g_earthModel,
                                gmsGetBrowseFullPath("libref/edg") );
      }

   mapImage = ptrToLibRefMap->gmsGetMapImage ();

   drawImage
      (hDC,
       BLACK,
       mapImage);
}


//-------------------------------------------------
// drawLinesOfOneSubtile
//
// Desciption:
//-------------------------------------------------
static void drawLinesOfOneSubtile
               (HDC        hDC,
                const char *themeSubtilePath)

{
         gms_2D_ScreenImageType mapImage;
         char                   edgName[64];

   sprintf(edgName, "%s/EDG", gmsGetDcwFullPath(themeSubtilePath));

         gmsMapClass theMap
                        (g_earthModel,
                         edgName,
                         g_PO_LFT);

   mapImage = theMap.gmsGetMapImage ();

   drawImage
      (hDC,
       DODGER_BLUE,
       mapImage);

   theMap.~gmsMapClass();
}


//-------------------------------------------------
// draw_PO_LinesOfSubtile_NG
//
// Desciption:
//-------------------------------------------------
static void draw_PO_LinesOfSubtile_NG
               (HDC hDC)

{
         gms_2D_ScreenImageType mapImage;
         char                   k, l, m;
         char                   edgName[64];
         int                    colorIndex = 0;

   for (k = 'N'; k <= 'N'; k++)
      for (l = 'G'; l <= 'G'; l++)
         for (m = 0; m < numSubtiles; m++)
            {
             sprintf(edgName, "PO/%c/%c/%s/edg",
                      k, l, subTiles[m]);

             sprintf(edgName, "%s", gmsGetDcwFullPath(edgName));

                    gmsMapClass theMap
                                   (g_earthModel,
                                    edgName,
                                    g_PO_LFT);

             mapImage = theMap.gmsGetMapImage ();

             drawImage
                (hDC,
                 BLACK, // colors[colorIndex],
                 mapImage);

             colorIndex++;

             if (colorIndex > Max_Colors)
                colorIndex = 0;

             theMap.~gmsMapClass ();
            }
}


//-------------------------------------------------
// draw_HY_LinesOfSubtile_NG
//
// Desciption:
//-------------------------------------------------
static void draw_HY_LinesOfSubtile_NG
               (HDC hDC)

{
         gms_2D_ScreenImageType mapImage;
         int                    m;
         char                   edgName[64];
         int                    colorIndex = 0;
         static gmsMapClass     *theMaps[numSubtiles];

   for (m = 0; m < numSubtiles; m++)
      {
       sprintf(edgName, "HY/N/G/%s/edg",
               subTiles[m]);

       sprintf(edgName, "%s", gmsGetDcwFullPath(edgName));

       if (theMaps[m] == NULL)
          {
           theMaps[m] = new gmsMapClass
                               (g_earthModel,
                                edgName,
                                g_HY_LFT);
          }

       mapImage = theMaps[m]->gmsGetMapImage ();

       drawImage
          (hDC,
           ORANGE,
           mapImage);
      }
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
// drawPolygonMap
//
// Desciption:
//-------------------------------------------------
static void drawPolygonMap
               (HDC hDC)

{
         gms_2D_ScreenImageType mapImage;
         char                   k, l, m;
         char                   edgName[64];
         int                    colorIndex = 0;

   for (k = 'N'; k <= 'N'; k++)
      for (l = 'G'; l <= 'G'; l++)
         for (m = 0; m < numSubtiles; m++)
            {
             sprintf(edgName, "PO/%c/%c/%s/edg",
                      k, l, subTiles[m]);

             sprintf(edgName, "%s", gmsGetDcwFullPath(edgName));

                    gms_PO_PolygonMapClass theMap
                                          (g_earthModel,
                                           edgName,
                                           g_PO_AFT);

             mapImage = theMap.gmsGetLandAreas ();

             drawPolygonImage
                (hDC,
                 DARK_GREEN, // RED, // colors[colorIndex],
                 DARK_GREEN,
                 mapImage);

             //mapImage = theMap.gmsGetOceanAreas ();

             //drawPolygonImage
             //   (hDC,
             //    WHITE,
             //    DODGER_BLUE,
             //    mapImage);

             colorIndex++;

             if (colorIndex > Max_Colors)
                colorIndex = 0;

             theMap.~gms_PO_PolygonMapClass();
            }
}


//-------------------------------------------------
// drawPolygonsOfOneSubtile
//
// Desciption:
//-------------------------------------------------
static void drawPolygonsOfOneSubtile
               (HDC        hDC,
                const char *themeSubtilePath)

{
         HPEN                   newPen;
         HGDIOBJ                oldPen;
         const int              onePixelWide = 0;
         gms_2D_ScreenImageType mapImage;
         char                   edgName[64], rngName[64];
         static int             polygonIndex = 0;

   sprintf(edgName, "%s/EDG", gmsGetDcwFullPath(themeSubtilePath));

         gms_PO_PolygonMapClass theMap
                               (g_earthModel,
                                edgName,
                                g_PO_AFT);

   mapImage = theMap.gmsGetLandAreas ();

   newPen = CreatePen
                (PS_SOLID,
                 onePixelWide,
                 DARK_GREEN);

   oldPen = SelectObject (hDC, newPen);

   if (mapImage.numLines == 0)
      {
             char msg[256];

       sprintf(msg, "Polygon %d no points : %s  - %s",
               polygonIndex,
               edgName, rngName);

       MessageBoxA
          (NULL,
           msg,
           "Null Polygon",
           MB_OK);
      }

   else
      {
       Polygon
         (hDC,
          (POINT *) mapImage.imageLines[polygonIndex].points,
          mapImage.imageLines[polygonIndex].numPoints);
      }

   polygonIndex++;

   if (polygonIndex == mapImage.numLines)
      polygonIndex = 0;

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);

   theMap.~gms_PO_PolygonMapClass();
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

   drawImage
      (hDC,
       GREEN,
       gridImage);

   //
   // Draw the longitude grid lines
   //
   gridImage = theGrid.gmsGetLongitudeGrid();

   drawImage
      (hDC,
       GREEN,
       gridImage);
}


//-------------------------------------------------
// draw_DN_Polygons
//
// Desciption:
//-------------------------------------------------
static void draw_DN_Polygons
               (HDC hDC)

{
         gms_2D_ScreenImageType mapImage;
         char                   k, l, m;
         char                   edgName[64];
         int                    colorIndex = 0;

   for (k = 'N'; k <= 'N'; k++)
      for (l = 'G'; l <= 'G'; l++)
         for (m = 0; m < numSubtiles; m++)
            {
             sprintf(edgName, "DN/%c/%c/%s/edg",
                      k, l, subTiles[m]);

             sprintf(edgName, "%s", gmsGetDcwFullPath(edgName));

                    gms_DN_PolygonMapClass theMap
                                              (g_earthModel,
                                               edgName);

             mapImage = theMap.gmsGetInlandWaterAreas ();

             drawPolygonImage
                (hDC,
                 BLUE,
                 BLUE,
                 mapImage);

             theMap.~gms_DN_PolygonMapClass ();
            }
}


//-------------------------------------------------
// draw_HY_Polygons
//
// Desciption:
//-------------------------------------------------
static void draw_HY_Polygons
               (HDC hDC)

{

}


//-------------------------------------------------
// draw_PP_Polygons
//
// Desciption:
//-------------------------------------------------
static void draw_PP_Polygons
               (HDC hDC)

{

}


//-------------------------------------------------
// isVisibleLine
//
// Desciption:
//-------------------------------------------------
static bool isVisibleLine
               (gms_2D_ScreenPointType point_1,
                gms_2D_ScreenPointType point_2)

{
         bool             isOk;
         static const int g_maxWidth  = 16 * 1024;
         static const int g_maxHeight = 16 * 1024;

   isOk = ( (point_1.x > -g_maxWidth) &&
            (point_1.x < g_maxWidth) &&
            (point_2.x > -g_maxWidth) &&
            (point_2.x < g_maxWidth) &&
                //----------//
            (point_1.y > -g_maxHeight) &&
            (point_1.y < g_maxHeight) &&
            (point_2.y > -g_maxHeight) &&
            (point_2.y < g_maxHeight) );

   return isOk;
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
