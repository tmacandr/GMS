//----------------------------------------------------------------------------
// File : po_maps.cpp
// Date : 12-Dec-99 : initial definition - evolve from "map_5"
//        25-Jan-00 : move polygon stuff to "polygon_maps" work space
//
// Description:
//    Windows app that displays "stuff" from the "Political/Ocean" (PO)
//    theme.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>

#include <po_maps_resource.h>

#include <gmsFeatureTable.h>

#include <stdio.h>
#include <gmsColors.h>
#include <gmsDebugUtil.h>
#include <gmsBitmapClass.h>
#include <gmsMapClass.h>
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

static double        g_rotationDeg = 20.0;

static bool          g_libRefIsShown = true;

static double        g_zoomAmount = 1000.0;

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

static void draw_PO_Map
               (HDC hDC);

static void drawLibrefMap
               (HDC hDC);

static void draw_PO_MapNG22
               (HDC hDC);

static void drawLatLongGrid
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
       WNDCLASSEX wc;
       MSG        theMsg;
       TCHAR      instanceName[] = TEXT("po_maps_demo");
       BOOL       isOk;

   g_hInstance = hInstance;

   //
   // Build the window classification record
   //
   ZeroMemory (&wc, sizeof(wc));

   wc.cbSize        = sizeof (wc);
   wc.cbClsExtra    = 0;
   wc.cbWndExtra    = 0;
   wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
   wc.hCursor       = LoadCursor (hInstance, IDC_ARROW);
   wc.hIcon         = LoadIcon (hInstance, IDI_APPLICATION);
   wc.hInstance     = hInstance;
   wc.lpfnWndProc   = WndProc;
   wc.lpszClassName = instanceName;
   wc.lpszMenuName  = MAKEINTRESOURCE(IDR_PO_Maps_Main_Menu);
   wc.style         = CS_VREDRAW | CS_HREDRAW;

   //
   // Register the window classification record
   // with Windows
   //
   isOk = RegisterClassEx (&wc);

   if ( ! isOk )
      {
       MessageBox
         (NULL,
          TEXT("Unable to execute on this platform"),
          TEXT("Unable to Execute"),
          MB_ICONERROR);

       return -1;
      }

   //
   // Now create the window
   //
   g_windowHandle = CreateWindowEx
                      (0,
                       instanceName,
                       TEXT("PO Maps : Ellipsoid Model"),
                       WS_OVERLAPPEDWINDOW,
                       100, 100,           // (x, y) location
                       g_width, g_height,  // width/height of window
                       NULL,
                       NULL,
                       hInstance,
                       NULL);

   if ( ! g_windowHandle )
      {
       MessageBox
          (NULL,
           TEXT("Unable to create window"),
           TEXT("Unable to execute"),
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
       //case WM_CREATE:
       //   {

       //    break;
       //   }

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
           gmsSet_X_Rotation (g_rotationDeg);

           break;
          }

       case ID_ROTATE_SOUTH:
          {
           gmsSet_X_Rotation (-g_rotationDeg);

           break;
          }

       case ID_ROTATE_EAST:
          {
           gmsSet_Y_Rotation (-g_rotationDeg);

           break;
          }

       case ID_ROTATE_WEST:
          {
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

       case ID_Text:
          {
           DialogBox
              (g_hInstance,
               MAKEINTRESOURCE(IDD_Text_Features_DIALOG),
               g_windowHandle,
               dlgProcTextFeatures);

           break;
          }

       case ID_LibRef_On_Off:
          {
           g_libRefIsShown = ! g_libRefIsShown;

           break;
          }

       case ID_FILE_SAVE_AS:
          {
           doScreenCapture ();

           break;
          }

       default:
          {
           MessageBox
             (NULL,
              TEXT("Unknown selection"),
              TEXT("Not Implemented?"),
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

               sscanf(charBuffer, "%lf", &g_zoomAmount);

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

   draw_PO_Map (hDC);

   if (g_libRefIsShown)
      drawLibrefMap (hDC);

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
// draw_PO_MapImage
//
// Desciption:
//-------------------------------------------------
static void draw_PO_MapImage
                  (HDC                    hDC,
                   gms_2D_ScreenImageType mapImage)

{
         int i, j;

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
}


//-------------------------------------------------
// draw_PO_Map
//
// Desciption:
//    Routine that draws the points from the edge
//    table
//-------------------------------------------------
static void draw_PO_Map
               (HDC hDC)

{
         HPEN                   newPen;
         HGDIOBJ                oldPen;
         const int              onePixelWide = 0;
         gms_2D_ScreenImageType mapImage;
         char                   k, l, m;
         static const           numSubtiles = 9;
         static const char      *subTiles[numSubtiles] = { {"11"},
                                                           {"12"},
                                                           {"13"},
                                                           {"21"},
                                                           {"22"},
                                                           {"23"},
                                                           {"31"},
                                                           {"32"},
                                                           {"33"} };
         char                   name[64];
         //gms_2D_ScreenPolylineType nodePoints;
         featureTableType       *LFT;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                MAGENTA);

   oldPen = SelectObject (hDC, newPen);


   LFT = gmsGetFeatureTable
            (gms_PO,
             gmsGetDcwFullPath("PO/POLINE.LFT"),
             gmsLine);

   if (LFT == NULL)
      {
       gmsWriteDebug("ERROR : the LFT is bogus\n");

       return;
      }

   for (k = 'N'; k <= 'P'; k++)
      for (l = 'J'; l <= 'K'; l++)
         for (m = 0; m < numSubtiles; m++)
            {
             sprintf(name, "PO/%c/%c/%s/edg",
                      k, l, subTiles[m]);

             sprintf(name, "%s", gmsGetDcwFullPath(name));

                    gmsMapClass theMap
                                   (gmsEllipsoid,
                                    name,
                                    LFT);

             mapImage = theMap.gmsGetMapImage ();

             draw_PO_MapImage (hDC, mapImage);

             //nodePoints = theMap.gmsGetMapNodePoints ();

             //drawIndependentPoints
             //   (hDC,
             //    BLACK,
             //    nodePoints);

             theMap.~gmsMapClass();
            }

   gmsFreeFeatureTable (LFT);

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
         HPEN                   newPen;
         HGDIOBJ                oldPen;
         const int              onePixelWide = 0;
         gms_2D_ScreenImageType mapImage;
         static gmsMapClass     *ptrToLibRefMap = NULL;

   if (ptrToLibRefMap == NULL)
      {
       ptrToLibRefMap = new gmsMapClass
                               (gmsEllipsoid,
                                gmsGetBrowseFullPath("libref/edg") );
      }

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                BLACK);

   oldPen = SelectObject (hDC, newPen);

   mapImage = ptrToLibRefMap->gmsGetMapImage ();

   draw_PO_MapImage (hDC, mapImage);

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
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
         HPEN                       newPen;
         HGDIOBJ                    oldPen;
         const int                  onePixelWide = 0;
         gms_2D_ScreenImageType     gridImage;
         static gmsLatLongGridClass *ptrToGrid = NULL;

   if (ptrToGrid == NULL)
      {
       ptrToGrid = new gmsLatLongGridClass
                              (15.0,
                               gmsEllipsoid);
      }

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                GREEN);

   oldPen = SelectObject (hDC, newPen);

   //
   // Draw the Latitude grid lines
   //
   gridImage = ptrToGrid->gmsGetLatitudeGrid();

   draw_PO_MapImage (hDC, gridImage);

   //
   // Draw the longitude grid lines
   //
   gridImage = ptrToGrid->gmsGetLongitudeGrid();

   draw_PO_MapImage (hDC, gridImage);

   //
   // Clean up
   //
   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
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
