//----------------------------------------------------------------------------
// File : tile_ref.cpp
// Date : 22-Nov-99 : initial definition
//        09-Dec-99 : update to to file re-names, etc
//        20-Jan-00 : drop use of 'gmsFlatMapClass'
//
// Description:
//    This program exercises the "library reference" and "tile reference"
//    data of both the BROWSE and REGIONAL libraries.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>
#include <tile_ref.h>
/*--------------*/
#include <gmsMinBoundRectTable.h>
#include <gmsUtilities.h>
#include <gmsDebugUtil.h>
/*--------------*/
#include <gmsColors.h>
#include <gmsMapClass.h>
#include <gmsLatLongGridClass.h>
#include <gmsMbrClass.h>
#include <gmsTileRefClass.h>
#include <gmsLibRefClass.h>
#include <gmsMapStateMgr.h>
#include <gmsBitmapClass.h>



typedef enum {
              gmsBrowseLibRef,
              gmsRegionalLibRef
             } gmsKindOfLibRefType;

              
//--------------------
// Global Variables
//--------------------

static HINSTANCE g_hInstance    = (HINSTANCE) 0;

static HWND      g_windowHandle = (HWND) 0;

static int       g_width = 440;

static int       g_height = 360;

static const int g_maxWidth = 16 * 1024;

static const int g_maxHeight = g_maxWidth;

static HWND      g_zoomDlgHandle = (HWND) 0;

static HWND      g_fontDlgHandle = (HWND) 0;

static gmsKindOfLibRefType g_currentLibRef = gmsBrowseLibRef;

static bool                g_tileRefIsShown = false;

static bool                g_ebrIsShown = false;

static bool                g_georefGridIsShown = false;

static HFONT               g_handleToCurrentFont = (HFONT) 0;

static double              g_zoomAmount = 1000.0;

//--------------------
// Global 'objects'
//--------------------
static gmsMapClass g_browseLibRefMap
                          (gmsFlat,
                           gmsGetBrowseFullPath("libref/edg"));

static gmsLibRefClass g_libRefMap (gmsFlat);

static gmsTileRefClass g_tileRefMap (gmsFlat);

static gmsMbrClass     g_regionEbr
                          (gmsFlat,
                           gmsGetDcwFullPath("libref/ebr"));


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

static void performPaint();

static void drawEdges
               (HDC hDC);

static void drawGeorefGrid
               (HDC hDC);

static void drawImage
               (HDC                    hDC,
                COLORREF               whichColor,
                gms_2D_ScreenImageType theImage);

static bool isVisibleLine
               (gms_2D_ScreenPointType point_1,
                gms_2D_ScreenPointType point_2);

static void writeTileLabels
               (HDC                    hDC,
                gms_2D_ScreenImageType latImage,
                gms_2D_ScreenImageType longImage);

static void screenCaptureMapImage ();

static BOOL CALLBACK dlgProcZoomControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static void setNewFont ();

static void doTileIdTest();


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
       TCHAR      instanceName[] = TEXT("tileRefDemo");
       TCHAR      mainMenuName[] = TEXT("Main_Menu");
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
   wc.lpszMenuName  = MAKEINTRESOURCE(IDR_Main_Menu);
   wc.style         = CS_VREDRAW | CS_HREDRAW;

   //
   // Register the window classification record
   // with Windows
   //
   isOk = RegisterClassEx (&wc);

   if ( ! isOk )
      {
       MessageBoxA
          (NULL,
           "Requires NT to run",
           "Unable to Execute",
           MB_ICONERROR);

       return -1;
      }

   //
   // Now create the window
   //
   g_windowHandle = CreateWindowEx
                       (0,
                        instanceName,
                        TEXT("Tile Ref Demo"),
                        WS_OVERLAPPEDWINDOW,
                        50, 50,             // initial (x, y) location
                        g_width, g_height,  // width/height of window
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

   if ( ! g_windowHandle )
      {
       MessageBoxA
          (NULL,
           "Create Window FAILED.",
           "Unable to Execute",
           MB_ICONERROR);

       return -1;
      }

   //
   // Display the window
   //
   ShowWindow (g_windowHandle, argc);

   UpdateWindow (g_windowHandle);

   __try
   {
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
   }
   __except(1)
      {
       gmsDebugPrintMapState ();
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
       case WM_CREATE:
          {
           gmsSetMapZoomFactor (79000.0);

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
           __try
              {
               performPaint ();
              }

           __except(gmsWriteDebug("EXCEPTION\n"), 0)
              {
               gmsDebugPrintMapState ();

               MessageBoxA
                 (NULL,
                  "Exception : during 'performPaint' : check 'debug.txt'",
                  "*** ERROR ***",
                  MB_OK);

               PostQuitMessage (0);
              }

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

       case ID_SAVE_MAP:
          {
           screenCaptureMapImage ();

           break;
          }

       case ID_Set_Font:
          {
           setNewFont ();

           break;
          }

       case ID_ZOOM_IN:
          {
           gmsZoomIn (g_zoomAmount);

           break;
          }

       case ID_ZOOM_OUT:
          {
           gmsZoomOut (g_zoomAmount);

           break;
          }

       case ID_ZOOM_CONTROL:
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

       case ID_MOVE_NORTH:
          {
           gmsMoveFlatMap (gmsMoveNorth);

           break;
          }

       case ID_MOVE_SOUTH:
          {
           gmsMoveFlatMap (gmsMoveSouth);

           break;
          }

       case ID_MOVE_EAST:
          {
           gmsMoveFlatMap (gmsMoveEast);

           break;
          }

       case ID_MOVE_WEST:
          {
           gmsMoveFlatMap (gmsMoveWest);

           break;
          }

       case ID_Browse_Libref:
          {
           g_currentLibRef = gmsBrowseLibRef;

           break;
          }

       case ID_Regional_Libref:
          {
           g_currentLibRef = gmsRegionalLibRef;

           break;
          }

       case ID_Tile_Ref_Edges:
          {
           g_tileRefIsShown = ! g_tileRefIsShown;

           break;
          }

       case ID_Edge_Bounding_Rectangle:
          {
           g_ebrIsShown = ! g_ebrIsShown;

           break;
          }

       case ID_Georef_Grid:
          {
           g_georefGridIsShown = ! g_georefGridIsShown;

           break;
          }

       default:
          {
                 char msg[64];

           sprintf(msg, "Unknown selections : ID = %d", whichCommand);

           MessageBoxA
              (NULL,
               msg,
               "Error",
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

   //SetTextAlign
   //   (hDC,
   //    TA_LEFT);

   drawEdges (hDC);

   if (g_georefGridIsShown)
      {
       drawGeorefGrid (hDC);
      }

   EndPaint (g_windowHandle, &paintStruct);
}


//-------------------------------------------------
// drawEdges
//
// Desciption:
//    Routine that draws the points from the edge
//    table
//-------------------------------------------------
static void drawEdges
               (HDC hDC)

{
         gms_2D_ScreenImageType mapImage;

   if (g_currentLibRef == gmsBrowseLibRef)
      {
       mapImage = g_browseLibRefMap.gmsGetMapImage ();

       drawImage (hDC, DARK_GREEN, mapImage);
      }

   else if (g_currentLibRef == gmsRegionalLibRef)
      {
       mapImage = g_libRefMap.gmsGetLibRefImage ();

       drawImage (hDC, RED, mapImage);
      }

   if (g_tileRefIsShown)
      {
       gmsWriteDebug("---> draw tile-ref\n");

       mapImage = g_tileRefMap.gmsGetTileRefImage ();

       drawImage (hDC, BLUE, mapImage);

       doTileIdTest();

       gmsWriteDebug("__________\n");
      }

   if (g_ebrIsShown)
      {
       mapImage = g_regionEbr.gmsGetMbrImage ();

       drawImage (hDC, MAGENTA, mapImage);
      }
}


//-------------------------------------------------
// drawGeorefGrid
//
// Desciption:
//-------------------------------------------------
static void drawGeorefGrid
               (HDC hDC)

{
         const double                   deltaDeg = 15.0f;
         static gmsLatLongGridClass     latLongGrid (deltaDeg, gmsFlat);
         gms_2D_ScreenImageType         latGridImage;
         gms_2D_ScreenImageType         longGridImage;

   latGridImage = latLongGrid.gmsGetLatitudeGrid ();

   longGridImage = latLongGrid.gmsGetLongitudeGrid ();

   drawImage (hDC, BLUE, latGridImage);

   drawImage (hDC, BLUE, longGridImage);

   writeTileLabels
      (hDC,
       latGridImage,
       longGridImage);
}


//-------------------------------------------------
// drawImage
//
// Desciption:
//-------------------------------------------------
static void drawImage
               (HDC                    hDC,
                COLORREF               whichColor,
                gms_2D_ScreenImageType theImage)

{
         HPEN      newPen;
         HGDIOBJ   oldPen;
         const int onePixelWide = 0;
         int       i, j;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                whichColor);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < theImage.numLines; i++)
      for (j = 0; j < (theImage.imageLines[i].numPoints - 1); j++)
         {
          if ( isVisibleLine
                  (theImage.imageLines[i].points[j],
                   theImage.imageLines[i].points[j + 1]) )
             {
              MoveToEx
                 (hDC,
                  theImage.imageLines[i].points[j].x,
                  theImage.imageLines[i].points[j].y,
                  NULL);

              LineTo
                 (hDC,
                  theImage.imageLines[i].points[j + 1].x,
                  theImage.imageLines[i].points[j + 1].y);
             }
         }

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
         bool isOk;

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
// writeTileLabels
//
// Desciption:
//-------------------------------------------------
static void writeTileLabels
               (HDC                    hDC,
                gms_2D_ScreenImageType latImage,
                gms_2D_ScreenImageType longImage)

{
         char gridLetter = 'A';
         int  i;

   //
   // Do the latitude labels
   //
   for (i = latImage.numLines - 2; i >= 0; i--)
      {
       TextOutA
          (hDC,
           latImage.imageLines[i].points[0].x - 13,
           latImage.imageLines[i].points[0].y + 4,
           &gridLetter,
           1);

       gridLetter++;

       if (gridLetter == 'I')
          gridLetter++;
      }

   //
   // Do the longitude labels
   //
   gridLetter = 'A';

   for (i = 0; i < longImage.numLines - 1; i++)
      {
       TextOutA
          (hDC,
           longImage.imageLines[i].points[0].x + 7,
           longImage.imageLines[i].points[0].y - 20,
           &gridLetter,
           1);

       gridLetter++;

       if ( (gridLetter == 'I') || (gridLetter == 'O') )
          gridLetter++;
      }
}


//-------------------------------------------------
// screenCaptureMapImage
//
// Desciption:
//-------------------------------------------------
static void screenCaptureMapImage ()

{
         static bool          isInitialized = false;
         static OPENFILENAMEA infoFromSaveAsDlg; // Regular ASCII
         static char          *fileFilter   =
            { "Bitmap Files (*.bmp)\0*.bmp\0\0" };
         static char          *defExtension = { "bmp" };
         static char          fileName[MAX_PATH];
         static char          fileTitle[MAX_PATH];
         static BOOL          dlgIsOk;
         char                 currentDir[1042];
         DWORD                status;

   if ( ! isInitialized )
      {
       gmsClearMemory
          ( (char *) &infoFromSaveAsDlg,
            sizeof(infoFromSaveAsDlg) );

       status = GetCurrentDirectoryA
                   (sizeof (currentDir),
                    currentDir);

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
       infoFromSaveAsDlg.lpstrInitialDir   = currentDir;
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
   }

   MessageBoxA
      (NULL,
       fileName,  // <--- full path + file name
       fileTitle, // <--- file name only
       MB_OK);
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
         const int             maxNumChars = 16;
         char                  charBuffer[maxNumChars];
         UINT                  numChars;
         gmsMapStateStructType mapState;

   mapState = gmsGetMapState ();

   if (message == WM_INITDIALOG)
      {
       sprintf(charBuffer, "%.2lf", mapState.zoomFactor);

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

               sscanf(charBuffer, "%lf", &mapState.zoomFactor);

               gmsSetMapZoomFactor (mapState.zoomFactor);

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

   //InvalidateRect
   //   (g_windowHandle, 
   //    NULL,  // the whole rectangle
   //    TRUE); // erase?
      
   MessageBoxA
      (NULL,
       "Set Font",
       "Done",
       MB_OK);
}


//-------------------------------------------------
// doTileIdTest
//
// Desciption:
//-------------------------------------------------
static void doTileIdTest()

{
         int  i;
         char *tileName;
         char msg[64];

   for (i = 1; i <= 769; i++)
      {
       tileName = g_tileRefMap.gmsConvertTileIdToName (i);

       sprintf(msg, "Tile-id %d : Name = %s\n",
               i, tileName);

       gmsWriteDebug(msg);
      }
}
