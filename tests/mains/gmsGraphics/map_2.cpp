//----------------------------------------------------------------------------
// File : map_2.cpp
// Date : 21-Sep-99 : initial definition
//        15-Oct-99 : rework - screen vs world coordinates
//        23-Oct-99 : implement rotation (x and y)
//        09-Dec-99 : update due to file renames, I/F changes, etc
//
// Description:
//    This program displays the "browse" information as a 3-D globe.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>
#include <map_2_resource.h>
#include <gmsColors.h>
#include <gmsEdgeTable.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsMapStateMgr.h>
#include <gmsDebugUtil.h>


//--------------------
// Global Variables
//--------------------

static HINSTANCE     g_hInstance    = (HINSTANCE) 0;

static HWND          g_windowHandle = (HWND) 0;

static HWND          g_zoomDlgHandle = (HWND) 0;

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

static void ParseMenuButton
               (int whichCommand);

static BOOL CALLBACK DlgProcZoomControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static void PerformPaint();

static void DrawEdges
               (HDC hDC);

static void DrawLatLongLines
               (HDC hDC);




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
       TCHAR      instanceName[] = TEXT("map_2_Demo");
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
                       TEXT("Map 2"),
                       WS_OVERLAPPEDWINDOW,
                       200, 200,  // (x, y) location
                       400, 300,  // width/height of window
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
       PerformPaint ();

       break;
      }

   case WM_DESTROY:
      {
       PostQuitMessage (0);

       break;
      }

   case WM_COMMAND:
      {
       ParseMenuButton (LOWORD(wParam));

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
// ParseMenuButton
//
// Desciption:
//    Routine to parse the selections from the
//    main menu bar.
//-------------------------------------------------
static void ParseMenuButton
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
           g_zoomDlgHandle = CreateDialog
                                (g_hInstance,
                                 MAKEINTRESOURCE(IDD_ZOOM_CTRL_DIALOG),
                                 g_windowHandle,
                                 DlgProcZoomControl);

           ShowWindow (g_zoomDlgHandle, SW_SHOW);

           break;
          }

       default:
          {
           MessageBox
             (NULL,
              TEXT("Unknown selection"),
              TEXT("Error"),
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
// DlgProcZoomControl
//
// Desciption:
//    This routine is the callback for the "Zoom-
//    Control-Dialog".  The operator is allowed to
//    enter a zoom-factor.  This routine catches
//    the input and sets the new zoom factor.
//-------------------------------------------------
static BOOL CALLBACK DlgProcZoomControl
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
// PerformPaint
//
// Desciption:
//    Routine to "paint" the graphics into the
//    window.  This routine is invoked due to a
//    WM_PAINT event.  This indicates that the
//    "canvas" needs to be refreshed.
//-------------------------------------------------
static void PerformPaint()

{
       HDC         hDC;
       PAINTSTRUCT paintStruct;

   hDC = BeginPaint (g_windowHandle, &paintStruct);

   if (theEDG == NULL)
      {
       theEDG = gmsGetEdgeTable
                   (gmsGetBrowseFullPath("libref/edg"));
      }

   DrawEdges (hDC);

   EndPaint (g_windowHandle, &paintStruct);
}


//-------------------------------------------------
// DrawEdges
//
// Desciption:
//    Routine that draws the points from the edge
//    table
//-------------------------------------------------
static void DrawEdges
               (HDC hDC)

{
         HPEN                   newPen;
         HGDIOBJ                oldPen;
         const int              onePixelWide = 0;
         gms_3D_WorldPointType  worldPoint_3D;
         gms_3D_ScreenPointType firstScreenPnt_3D;
         gms_3D_ScreenPointType secondScreenPnt_3D;
         gms_2D_ScreenPointType screenPoint_2D;
         int                    i, j;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                RED);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < theEDG->numRecords; i++)
      for (j = 0; j < (theEDG->edgeRecords[i].numCoords - 1); j++)
         {
          /*--------------*/
          /* First point  */
          /*--------------*/
          worldPoint_3D = gmsConvertLatLongToSphericalPoint
                                   (theEDG->edgeRecords[i].theCoords[j]);

          firstScreenPnt_3D = gmsConvertWorldCoordToScreenCoord
                                     (worldPoint_3D);

          firstScreenPnt_3D = gmsRotateThreeDimPointAbout_X_Axis
                                     (firstScreenPnt_3D,
                                      nextAngleOf_X_Rotation);

          firstScreenPnt_3D = gmsRotateThreeDimPointAbout_Y_Axis
                                     (firstScreenPnt_3D,
                                      nextAngleOf_Y_Rotation);

          /*--------------*/
          /* Second point */
          /*--------------*/
          worldPoint_3D = gmsConvertLatLongToSphericalPoint
                                 (theEDG->edgeRecords[i].theCoords[j + 1]);

          secondScreenPnt_3D = gmsConvertWorldCoordToScreenCoord
                                      (worldPoint_3D);

          secondScreenPnt_3D = gmsRotateThreeDimPointAbout_X_Axis
                                      (secondScreenPnt_3D,
                                       nextAngleOf_X_Rotation);

          secondScreenPnt_3D = gmsRotateThreeDimPointAbout_Y_Axis
                                      (secondScreenPnt_3D,
                                       nextAngleOf_Y_Rotation);

          /*----------------*/
          /* Draw the line  */
          /*----------------*/
          if ((firstScreenPnt_3D.z >= 0.0) && (secondScreenPnt_3D.z >= 0.0))
             {
              firstScreenPnt_3D = gmsTranslateThreeDimPoint
                                         (firstScreenPnt_3D,
                                          (g_width / 2),
                                          (g_height / 2),
                                          1024);

              screenPoint_2D = gmsConvertToProjectedPoint
                                      (firstScreenPnt_3D,
                                       g_width,
                                       g_height);

              MoveToEx
                 (hDC,
                  screenPoint_2D.x, screenPoint_2D.y,
                  NULL);

              secondScreenPnt_3D = gmsTranslateThreeDimPoint
                                          (secondScreenPnt_3D,
                                           (g_width / 2),
                                           (g_height / 2),
                                           1024);

              screenPoint_2D = gmsConvertToProjectedPoint
                                      (secondScreenPnt_3D,
                                       g_width,
                                       g_height);

              LineTo
                 (hDC,
                  screenPoint_2D.x, screenPoint_2D.y);
             }
         }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);

   DrawLatLongLines (hDC);
}


//-------------------------------------------------
// DrawLatLongLines
//
// Desciption:
//    Routine that draws lat/long lines to enhance
//    the spherical aspect of the globe.
//-------------------------------------------------
static void DrawLatLongLines
               (HDC hDC)

{
         HPEN                             newPen;
         HGDIOBJ                          oldPen;
         const int                        onePixelWide = 0;
         static gms_3D_WorldPolylineType  the_3D_WorldPolyline = {0, NULL};
         static gms_3D_ScreenPolylineType the_3D_ScreenPolyline;
         static gms_2D_ScreenPolylineType the_2D_ScreenPolyline;
         const int                        Num_Points = 64;
         double                           degrees;

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

       the_2D_ScreenPolyline.numPoints = Num_Points;

       the_2D_ScreenPolyline.points = new gms_2D_ScreenPointType[Num_Points];
      }

   for (degrees = -90.0; degrees <= 90.0; degrees = degrees + 15.0)
      {
       gmsBuildLongitudeLine
          (degrees,
           gmsSphere,
           &the_3D_WorldPolyline);

       gmsConvertWorldPolylineToScreenPolyline
              (&the_3D_WorldPolyline,
               &the_3D_ScreenPolyline);

       gmsTranslate_3D_Polyline
           (&the_3D_ScreenPolyline,
            (g_width / 2),
            (g_height / 2),
            1024);

       gmsConvertToProjectedPolyline
           (&the_3D_ScreenPolyline,
            g_width,
            g_height,
            &the_2D_ScreenPolyline);

       Polyline
          (hDC,
           (POINT *) the_2D_ScreenPolyline.points,
           Num_Points);
      }

   for (degrees = -70.0; degrees <= 70.0; degrees = degrees + 15.0)
      {
       gmsBuildLatitudeLine
          (degrees,
           gmsSphere,
           &the_3D_WorldPolyline);

       gmsConvertWorldPolylineToScreenPolyline
              (&the_3D_WorldPolyline,
               &the_3D_ScreenPolyline);

       gmsTranslate_3D_Polyline
           (&the_3D_ScreenPolyline,
            (g_width / 2),
            (g_height / 2),
            1024);

       gmsConvertToProjectedPolyline
           (&the_3D_ScreenPolyline,
            g_width,
            g_height,
            &the_2D_ScreenPolyline);

       Polyline
          (hDC,
           (POINT *) &the_2D_ScreenPolyline.points[0],
           Num_Points);
      }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}
