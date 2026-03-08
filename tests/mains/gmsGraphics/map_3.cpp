//----------------------------------------------------------------------------
// File : map_3.cpp
// Date : 23-Oct-99 : gutted ... restart from map_2 but for ellipsoid model
//        27-Oct-99 : implement "save as" function
//        09-Dec-99 : update due to file renames, static lib, I/F changes
//        27-Jan-00 : update due to re-architect of "zoom"
//
// Description:
//    This program displays the "browse" information as a 3-D globe using
//    the WGS84 ellipsoid as the model.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>

#include <map_3_resource.h>
#include <gmsColors.h>
#include <gmsEdgeTable.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsBitmapClass.h>
#include <gmsMapStateMgr.h>
#include <gmsDebugUtil.h>


//--------------------
// Global Variables
//--------------------

static HINSTANCE     g_hInstance    = (HINSTANCE) 0;

static HWND          g_windowHandle = (HWND) 0;

static HWND          g_zoomDlgHandle = (HWND) 0;

static HWND          g_saveAsDlgHandle = (HWND) 0;

static int           g_width = 0;

static int           g_height = 0;

static double        nextAngleOf_X_Rotation = 0.0;

static double        nextAngleOf_Y_Rotation = 0.0;

static edgeTableType *theEDG = NULL;

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

static void drawEdges
               (HDC hDC);

static void drawLatLongLines
               (HDC hDC);

static void performSaveAs ();



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
       TCHAR      instanceName[] = TEXT("map_3_Demo");
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
   wc.lpszMenuName  = MAKEINTRESOURCE(IDR_Map_3_Main_Menu);
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
          TEXT("Requires NT to run"),
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
                       TEXT("Ellipsoid Model"),
                       WS_OVERLAPPEDWINDOW,
                       100, 100,  // (x, y) location
                       450, 400,  // width/height of window
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
    case WM_SIZE:
       {
        g_width = LOWORD (lParam);

        g_height = HIWORD (lParam);

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
           gmsZoomOut (g_zoomAmount);

           break;
          }

       case ID_ROTATE_NORTH:
          {
           nextAngleOf_X_Rotation = nextAngleOf_X_Rotation + 20.0;

           break;
          }

       case ID_ROTATE_WEST:
          {
           nextAngleOf_Y_Rotation = nextAngleOf_Y_Rotation + 20.0;

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
           performSaveAs ();

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

   if (theEDG == NULL)
      {
       theEDG = gmsGetEdgeTable
                   (gmsGetBrowseFullPath("libref/edg"));
      }

   drawEdges (hDC);

   drawLatLongLines (hDC);

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
         HPEN                   newPen;
         HGDIOBJ                oldPen;
         const int              onePixelWide = 0;
         gms_3D_WorldPointType  worldPnt_3D;
         gms_3D_ScreenPointType firstScreenPnt_3D;
         gms_3D_ScreenPointType secondScreenPnt_3D;
         gms_2D_ScreenPointType firstScreenPoint_2D, secondScreenPoint_2D;
         int                    i, j;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                RED);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < theEDG->numRecords; i++)
      for (j = 0; j < (theEDG->edgeRecords[i].numCoords - 1); j++)
         {
          worldPnt_3D = gmsConvertLatLongToEllipsoidPoint
                           (theEDG->edgeRecords[i].theCoords[j]);

          firstScreenPnt_3D = gmsConvertWorldCoordToScreenCoord (worldPnt_3D);

          firstScreenPnt_3D = gmsRotateThreeDimPointAbout_Y_Axis
                                     (firstScreenPnt_3D,
                                      nextAngleOf_Y_Rotation);

          firstScreenPnt_3D = gmsRotateThreeDimPointAbout_X_Axis
                                     (firstScreenPnt_3D,
                                      nextAngleOf_X_Rotation);

          worldPnt_3D = gmsConvertLatLongToEllipsoidPoint
                           (theEDG->edgeRecords[i].theCoords[j + 1]);

          secondScreenPnt_3D = gmsConvertWorldCoordToScreenCoord (worldPnt_3D);

          secondScreenPnt_3D = gmsRotateThreeDimPointAbout_Y_Axis
                                     (secondScreenPnt_3D,
                                      nextAngleOf_Y_Rotation);

          secondScreenPnt_3D = gmsRotateThreeDimPointAbout_X_Axis
                                     (secondScreenPnt_3D,
                                      nextAngleOf_X_Rotation);

          /*----------------*/
          /* Draw the line  */
          /*----------------*/
          if ((firstScreenPnt_3D.z >= 0) && (secondScreenPnt_3D.z >= 0))
             {
              firstScreenPnt_3D = gmsTranslateThreeDimPoint
                                         (firstScreenPnt_3D,
                                          (g_width / 2),
                                          (g_height / 2),
                                          1024);

              firstScreenPoint_2D = gmsConvertToProjectedPoint
                                       (firstScreenPnt_3D,
                                        g_width,
                                        g_height);

              secondScreenPnt_3D = gmsTranslateThreeDimPoint
                                          (secondScreenPnt_3D,
                                           (g_width / 2),
                                           (g_height / 2),
                                           1024);

              secondScreenPoint_2D = gmsConvertToProjectedPoint
                                        (secondScreenPnt_3D,
                                         g_width,
                                         g_height);

              if (gmsIsVisibleLine
                     (firstScreenPoint_2D,
                      secondScreenPoint_2D) )
                 {
                  MoveToEx
                     (hDC,
                      firstScreenPoint_2D.x, firstScreenPoint_2D.y,
                      NULL);

                  LineTo
                     (hDC,
                      secondScreenPoint_2D.x, secondScreenPoint_2D.y);
                 }
             }
         }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// drawLatLongLines
//
// Desciption:
//    Routine that draws lat/long lines to enhance
//    the ellipsoid aspect of the globe.
//-------------------------------------------------
static void drawLatLongLines
               (HDC hDC)

{
         HPEN                             newPen;
         HGDIOBJ                          oldPen;
         const int                        onePixelWide = 0;
         static gms_3D_WorldPolylineType  the_3D_WorldPolyline = {0, NULL};
         static gms_3D_ScreenPolylineType the_3D_ScreenPolyline;
         gms_3D_ScreenPointType           first_3D_ScreenPoint;
         gms_3D_ScreenPointType           second_3D_ScreenPoint;
         gms_2D_ScreenPointType           first_2D_ScreenPoint;
         gms_2D_ScreenPointType           second_2D_ScreenPoint;
         const int                        Num_Points = 64;
         double                           degrees;
         int                              i;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                GREEN);

   oldPen = SelectObject (hDC, newPen);

   if (the_3D_WorldPolyline.points == NULL)
      {
       the_3D_WorldPolyline.numPoints = Num_Points;

       the_3D_WorldPolyline.points = new gms_3D_WorldPointType[Num_Points];

       the_3D_ScreenPolyline.numPoints = Num_Points;

       the_3D_ScreenPolyline.points = new gms_3D_ScreenPointType[Num_Points];
      }

   for (degrees = -180.0; degrees <= 165.0; degrees = degrees + 15.0)
      {
       gmsBuildLongitudeLine
          (degrees,
           gmsEllipsoid,
           &the_3D_WorldPolyline);

       gmsConvertWorldPolylineToScreenPolyline
              (&the_3D_WorldPolyline,
               &the_3D_ScreenPolyline);

       for (i = 0; i < Num_Points - 1; i++)
          {
           first_3D_ScreenPoint = gmsRotateThreeDimPointAbout_X_Axis
                                    (the_3D_ScreenPolyline.points[i],
                                     nextAngleOf_X_Rotation);

           first_3D_ScreenPoint = gmsRotateThreeDimPointAbout_Y_Axis
                                    (first_3D_ScreenPoint,
                                     nextAngleOf_Y_Rotation);
 
           second_3D_ScreenPoint = gmsRotateThreeDimPointAbout_X_Axis
                                      (the_3D_ScreenPolyline.points[i + 1],
                                       nextAngleOf_X_Rotation);

           second_3D_ScreenPoint = gmsRotateThreeDimPointAbout_Y_Axis
                                      (second_3D_ScreenPoint,
                                       nextAngleOf_Y_Rotation);

           if ( (first_3D_ScreenPoint.z >= 0) &&
                (second_3D_ScreenPoint.z >= 0) )
              {
               first_3D_ScreenPoint = gmsTranslateThreeDimPoint
                                         (first_3D_ScreenPoint,
                                          (g_width / 2),
                                          (g_height / 2),
                                          1024);

               first_2D_ScreenPoint = gmsConvertToProjectedPoint
                                         (first_3D_ScreenPoint,
                                          g_width,
                                          g_height);

               second_3D_ScreenPoint = gmsTranslateThreeDimPoint
                                          (second_3D_ScreenPoint,
                                           (g_width / 2),
                                           (g_height / 2),
                                           1024);

               second_2D_ScreenPoint = gmsConvertToProjectedPoint
                                          (second_3D_ScreenPoint,
                                           g_width,
                                           g_height);

               if (gmsIsVisibleLine
                      (first_2D_ScreenPoint,
                       second_2D_ScreenPoint) )
                  {
                   MoveToEx
                      (hDC,
                       first_2D_ScreenPoint.x,
                       first_2D_ScreenPoint.y,
                       NULL);

                   LineTo
                     (hDC,
                      second_2D_ScreenPoint.x,
                      second_2D_ScreenPoint.y);
                  }
              }
          }
      }

   for (degrees = -70.0; degrees <= 70.0; degrees = degrees + 15.0)
      {
       gmsBuildLatitudeLine
          (degrees,
           gmsEllipsoid,
           &the_3D_WorldPolyline);

       gmsConvertWorldPolylineToScreenPolyline
              (&the_3D_WorldPolyline,
               &the_3D_ScreenPolyline);

       for (i = 0; i < Num_Points - 1; i++)
          {
           first_3D_ScreenPoint = gmsRotateThreeDimPointAbout_X_Axis
                                    (the_3D_ScreenPolyline.points[i],
                                     nextAngleOf_X_Rotation);

           first_3D_ScreenPoint = gmsRotateThreeDimPointAbout_Y_Axis
                                    (first_3D_ScreenPoint,
                                     nextAngleOf_Y_Rotation);
 
           second_3D_ScreenPoint = gmsRotateThreeDimPointAbout_X_Axis
                                      (the_3D_ScreenPolyline.points[i + 1],
                                       nextAngleOf_X_Rotation);

           second_3D_ScreenPoint = gmsRotateThreeDimPointAbout_Y_Axis
                                      (second_3D_ScreenPoint,
                                       nextAngleOf_Y_Rotation);

           if ( (first_3D_ScreenPoint.z >= 0) &&
                (second_3D_ScreenPoint.z >= 0) )
              {
               first_3D_ScreenPoint = gmsTranslateThreeDimPoint
                                         (first_3D_ScreenPoint,
                                          (g_width / 2),
                                          (g_height / 2),
                                          1024);

               first_2D_ScreenPoint = gmsConvertToProjectedPoint
                                         (first_3D_ScreenPoint,
                                          g_width,
                                          g_height);

               second_3D_ScreenPoint = gmsTranslateThreeDimPoint
                                          (second_3D_ScreenPoint,
                                           (g_width / 2),
                                           (g_height / 2),
                                           1024);

               second_2D_ScreenPoint = gmsConvertToProjectedPoint
                                          (second_3D_ScreenPoint,
                                           g_width,
                                           g_height);

               if (gmsIsVisibleLine
                      (first_2D_ScreenPoint,
                       second_2D_ScreenPoint) )
                  {
                   MoveToEx
                      (hDC,
                       first_2D_ScreenPoint.x,
                       first_2D_ScreenPoint.y,
                       NULL);

                   LineTo
                      (hDC,
                       second_2D_ScreenPoint.x,
                       second_2D_ScreenPoint.y);
                  }
              }
          }
      }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// performSaveAs
//
// Desciption:
//-------------------------------------------------
static void performSaveAs ()

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
       gmsClearMemory
          ( (char *) &infoFromSaveAsDlg,
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
