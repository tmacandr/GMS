//----------------------------------------------------------------------------
// File : map_6.cpp
// Date : 21-Dec-99 : work in Rhode Island - evolve from map_5
//
// Description:
//    Windows app that makes more use of the utilities from the GMS Graphics
//    Library.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>

#include <map_6_resource.h>

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

static bool          poEdgeIsShown = false;

static int           g_width = 450;

static int           g_height = 400;

static gmsMapClass   theMap
                        (g_width,
                         g_height,
                         gmsGetBrowseFullPath("libref/edg"), false);

static gmsLatLongGridClass theGrid(g_width, g_height);

static double        g_rotationDeg = 20.0;


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

static void drawImage
               (HDC                    hDC,
                COLORREF               whichColor,
                gms_2D_ScreenImageType theImage);

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
       TCHAR      instanceName[] = TEXT("map_6_demo");
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
   wc.lpszMenuName  = MAKEINTRESOURCE(IDR_Map_6_Main_Menu);
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
                       TEXT("Map 6 : Rhode Island"),
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

   theMap.~gmsMapClass();

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
           gmsZoomIn();

           break;
          }

       case ID_ZOOM_OUT:
          {
           gmsZoomOut();

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

       case ID_FILE_SAVE_AS:
          {
           doScreenCapture ();

           break;
          }

       case ID_Political_Borders:
          {
           poEdgeIsShown = ! poEdgeIsShown;

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
         double    newZoomFactor;

   if (message == WM_INITDIALOG)
      {
       sprintf(charBuffer, "%.2lf", 10.0);

       // use 16-bit API for both Windows98 and NT

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
       HDC                    hDC;
       PAINTSTRUCT            paintStruct;
       gms_2D_ScreenImageType tempImage;

   hDC = BeginPaint (g_windowHandle, &paintStruct);

   tempImage = theMap.gmsGetMapImage ();

   drawImage
      (hDC,
       BLACK,
       tempImage);

   tempImage = theGrid.gmsGetLatitudeGrid();

   drawImage
      (hDC,
       GREEN,
       tempImage);

   tempImage = theGrid.gmsGetLongitudeGrid();

   drawImage
      (hDC,
       GREEN,
       tempImage);

   if (poEdgeIsShown)
      {
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
         char                   latChar, longChar;
         int                    i;

       for (i = 0; i < numSubtiles; i++)
          {
           sprintf(name, "po/r/e/%s/edg", subTiles[i]);

           gmsMapClass poMap
                          (g_width,
                           g_height,
                           gmsGetDcwFullPath(name),
                           true);

           tempImage = poMap.gmsGetMapImage ();

           drawImage
              (hDC,
               RED,
               tempImage);
          }

       for (longChar = 'M'; longChar <= 'N'; longChar++)
          for (latChar = 'F'; latChar <= 'G'; latChar++)
             for (i = 0; i < numSubtiles; i++)
                {
                 sprintf(name, "po/%c/%c/%s/edg",
                         longChar, latChar, subTiles[i]);

                 gmsMapClass poMap
                                (g_width,
                                 g_height,
                                 gmsGetDcwFullPath(name),
                                 true);

                 tempImage = poMap.gmsGetMapImage ();

                 drawImage
                    (hDC,
                     RED,
                     tempImage);
                }
      }

   EndPaint (g_windowHandle, &paintStruct);
}


//-------------------------------------------------
// drawImage
//
// Desciption:
//    Routine that draws the points from the edge
//    table
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
          if (isVisibleLine
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
