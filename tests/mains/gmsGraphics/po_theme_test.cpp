//----------------------------------------------------------------------------
// File : po_theme_test.cpp
// Date : 25-Jan-00 : initial definition
//        27-Jan-00 : update due to re-architect of "zoom"
//        06-Feb-00 : change dependecy to "gmsTileClass"
//
// Description:
//    Windows app that exercises the "gms_PO_ThemeClass" component..
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>

#include <po_theme_test_resource.h>

#include <stdio.h>
#include <gmsColors.h>
#include <gmsDebugUtil.h>
   //---------//
#include <gmsTileClass.h>
#include <gmsBitmapClass.h>
#include <gmsLatLongGridClass.h>
#include <gmsMapStateMgr.h>
#include <gmsUtilities.h>


//--------------------
// Global Variables
//--------------------

static HINSTANCE     g_hInstance    = (HINSTANCE) 0;

static HWND          g_windowHandle = (HWND) 0;

static HWND          g_saveAsDlgHandle = (HWND) 0;

static HWND          g_zoomDlgHandle = (HWND) 0;

static HFONT         g_handleToCurrentFont = (HFONT) 0;

static int           g_width = 560;

static int           g_height = 500;

static double        g_rotationDeg = 20.0;

static double        g_zoomAmount = 1000.0;

static bool          g_libRefIsShown = true;

static bool          g_poMapsAreShown = false;

static bool          g_poTextIsShown = false;

static bool          g_poNodesAreShown = false;

static gmsTileClass *ptrToTileObject = NULL;

static bool          g_dnMapsAreShown = false;

static bool          g_dnTextIsShown = false;

static bool          g_dnNodesAreShown = false;

static bool          g_hyMapsAreShown = false;

static bool          g_hyTextIsShown = false;

static bool          g_hyNodesAreShown = false;

static bool          g_ppMapsAreShown = false;

static bool          g_ppTextIsShown = false;

static bool          g_ppNodesAreShown = false;

static bool          g_clMapsAreShown = false;

static bool          g_clTextIsShown = false;

static bool          g_clNodesAreShown = false;

static bool          g_rdMapsAreShown = false;

static bool          g_rdTextIsShown = false;

static bool          g_rdNodesAreShown = false;

static bool          g_ofMapsAreShown = false;

static bool          g_ofTextIsShown = false;

static bool          g_ofNodesAreShown = false;

const char               latTileId = 'G';

const char               longTileId = 'N';


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

static void performPaint();

static void drawMaps
               (HDC             hDC,
                gmsThematicType whichTheme);

static void drawNodes
               (HDC             hDC,
                gmsThematicType whichTheme);

static void drawText
               (HDC             hDC,
                gmsThematicType whichTheme);

static void drawLibrefMap
               (HDC hDC);

static void drawLatLongGrid
               (HDC hDC);

static bool isVisibleLine
               (gms_2D_ScreenPointType point_1,
                gms_2D_ScreenPointType point_2);

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
       char        *instanceName = {"po_theme_test"};
       BOOL        isOk;

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
   wc.lpszMenuName  = MAKEINTRESOURCE(IDR_PO_Theme_Test_Main_Menu);
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
                       "PO Theme Test",
                       WS_OVERLAPPEDWINDOW,
                       100, 50,            // (x, y) location
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
       case WM_CREATE:
          {
           ptrToTileObject = new gmsTileClass
                                    (gmsEllipsoid,
                                     longTileId, latTileId);
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

       case ID_File_Set_Font:
          {
           setNewFont ();

           break;
          }

       case ID_ZOOM_IN:
          {
           gmsZoomIn(g_zoomAmount);

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

       case ID_PO_Text:
          {
           g_poTextIsShown = ! g_poTextIsShown;

           break;
          }

       case ID_PO_Edges:
          {
           g_poMapsAreShown = ! g_poMapsAreShown;

           break;
          }
       case ID_PO_Nodes:
          {
           g_poNodesAreShown = ! g_poNodesAreShown;

           break;
          }

       case ID_DN_Text:
          {
           g_dnTextIsShown = ! g_dnTextIsShown;

           break;
          }

       case ID_DN_Edges:
          {
           g_dnMapsAreShown = ! g_dnMapsAreShown;

           break;
          }
       case ID_DN_Nodes:
          {
           g_dnNodesAreShown = ! g_dnNodesAreShown;

           break;
          }

       case ID_HY_Text:
          {
           g_hyTextIsShown = ! g_hyTextIsShown;

           break;
          }

       case ID_HY_Edges:
          {
           g_hyMapsAreShown = ! g_hyMapsAreShown;

           break;
          }
       case ID_HY_Nodes:
          {
           g_hyNodesAreShown = ! g_hyNodesAreShown;

           break;
          }

       case ID_PP_Text:
          {
           g_ppTextIsShown = ! g_ppTextIsShown;

           break;
          }

       case ID_PP_Edges:
          {
           g_ppMapsAreShown = ! g_ppMapsAreShown;

           break;
          }

       case ID_PP_Nodes:
          {
           g_ppNodesAreShown = ! g_ppNodesAreShown;

           break;
          }

       case ID_CL_Text:
          {
           g_clTextIsShown = ! g_clTextIsShown;

           break;
          }

       case ID_CL_Edges:
          {
           g_clMapsAreShown = ! g_clMapsAreShown;

           break;
          }

       case ID_CL_Nodes:
          {
           g_clNodesAreShown = ! g_clNodesAreShown;

           break;
          }

       case ID_RD_Text:
          {
           g_rdTextIsShown = ! g_rdTextIsShown;

           break;
          }

       case ID_RD_Edges:
          {
           g_rdMapsAreShown = ! g_rdMapsAreShown;

           break;
          }

       case ID_RD_Nodes:
          {
           g_rdNodesAreShown = ! g_rdNodesAreShown;

           break;
          }

       case ID_OF_Text:
          {
           g_ofTextIsShown = ! g_ofTextIsShown;

           break;
          }

       case ID_OF_Edges:
          {
           g_ofMapsAreShown = ! g_ofMapsAreShown;

           break;
          }

       case ID_OF_Nodes:
          {
           g_ofNodesAreShown = ! g_ofNodesAreShown;

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

   if (g_poMapsAreShown)
      drawMaps (hDC, gms_PO);

   if (g_poNodesAreShown)
      drawNodes (hDC, gms_PO);

   if (g_poTextIsShown)
      drawText (hDC, gms_PO);

      //---------//

   if (g_dnMapsAreShown)
      drawMaps (hDC, gms_DN);

   if (g_dnNodesAreShown)
      drawNodes (hDC, gms_DN);

   if (g_dnTextIsShown)
      drawText (hDC, gms_DN);

      //---------//

   if (g_hyMapsAreShown)
      drawMaps (hDC, gms_HY);

   if (g_hyNodesAreShown)
      drawNodes (hDC, gms_HY);

   if (g_hyTextIsShown)
      drawText (hDC, gms_HY);

      //---------//

   if (g_ppMapsAreShown)
      drawMaps
         (hDC, gms_PP);

   if (g_ppNodesAreShown)
      drawNodes
         (hDC, gms_PP);

   if (g_ppTextIsShown)
      drawText
         (hDC, gms_PP);

      //---------//

   if (g_clMapsAreShown)
      drawMaps (hDC, gms_CL);

   if (g_clNodesAreShown)
      drawNodes (hDC, gms_CL);

   if (g_clTextIsShown)
      drawText (hDC, gms_CL);

      //---------//

   if (g_rdMapsAreShown)
      drawMaps (hDC, gms_RD);

   if (g_rdNodesAreShown)
      drawNodes (hDC, gms_RD);

   if (g_rdTextIsShown)
      drawText (hDC, gms_RD);

      //---------//

   if (g_ofMapsAreShown)
      drawMaps (hDC, gms_OF);

   if (g_ofNodesAreShown)
      drawNodes (hDC, gms_OF);

   if (g_ofTextIsShown)
      drawText (hDC, gms_OF);

      //---------//

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
// drawImage
//
// Desciption:
//-------------------------------------------------
static void drawImage
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
// drawMaps
//
// Desciption:
//    Routine that draws the points from the edge
//    table
//-------------------------------------------------
static void drawMaps
               (HDC             hDC,
                gmsThematicType whichTheme)

{
         HPEN                     newPen;
         HGDIOBJ                  oldPen;
         const int                onePixelWide = 0;
         gmsMapClass              **ptrToMaps;
         int                      numMaps;
         gms_2D_ScreenImageType   mapImage;
         int                      i;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                MAGENTA);

   oldPen = SelectObject (hDC, newPen);

   ptrToMaps = ptrToTileObject->gmsGetTileMaps (whichTheme, numMaps);

   for (i = 0; i < numMaps; i++)
      {
       mapImage = ptrToMaps[i]->gmsGetMapImage ();

       drawImage (hDC, mapImage);
      }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// drawNodes
//
// Desciption:
//-------------------------------------------------
static void drawNodes
               (HDC             hDC,
                gmsThematicType whichTheme)

{
         HPEN                      newPen;
         HGDIOBJ                   oldPen;
         const int                 onePixelWide = 0;
         gmsNodeClass              **ptrToNodes;
         int                       numNodes;
         gms_2D_ScreenPolylineType nodePoints;
         int                       i;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                DODGER_BLUE);

   oldPen = SelectObject (hDC, newPen);

   ptrToNodes = ptrToTileObject->gmsGetTileNodes (whichTheme, numNodes);

   for (i = 0; i < numNodes; i++)
      {
       nodePoints = ptrToNodes[i]->gmsGetNodePoints ();

       drawIndependentPoints
          (hDC,
           BLACK,
           nodePoints);
      }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// drawText
//
// Desciption:
//-------------------------------------------------
static void drawText
               (HDC             hDC,
                gmsThematicType whichTheme)

{
         HPEN                newPen;
         HGDIOBJ             oldPen;
         const int           onePixelWide = 0;
         gmsTextClass        **ptrToTextObjs;
         int                 numTxtObjs;
         gmsMapTextArrayType txtItem;
         int                 i, j;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                BLUE);

   oldPen = SelectObject (hDC, newPen);

   ptrToTextObjs = ptrToTileObject->gmsGetTileText (whichTheme, numTxtObjs);

   for (i = 0; i < numTxtObjs; i++)
      {
       txtItem = ptrToTextObjs[i]->gmsGetTextItems ();

       for (j = 0; j < txtItem.numTextRecords; j++)
          {
           TextOutA
              (hDC,
               txtItem.textRecords[j].theCoord.x,
               txtItem.textRecords[j].theCoord.y,
               txtItem.textRecords[j].theString,
               txtItem.textRecords[j].numChars);
          }
      }

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

   drawImage (hDC, mapImage);

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

   drawImage (hDC, gridImage);

   //
   // Draw the longitude grid lines
   //
   gridImage = ptrToGrid->gmsGetLongitudeGrid();

   drawImage (hDC, gridImage);

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
