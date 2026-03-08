//----------------------------------------------------------------------------
// File : sphere.cpp
// Date : 24-Sep-99 : initial definition
//        09-Dec-99 : this is busted!  Someday I'll get it to work again
//
// Description:
//    This program displays a sphere.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>
#include <sphere_resource.h>
#include <gmsColors.h>
#include <gmsScreenCoordUtils.h>

//--------------------
// Global Variables
//--------------------

static HINSTANCE     g_hInstance    = (HINSTANCE) 0;

static HWND          g_windowHandle = (HWND) 0;

static int           g_width = 0;

static int           g_height = 0;

static double        g_currentZoomFactor = -1.0;


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

static void PerformPaint();

static void DrawSphere
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
		 TCHAR      instanceName[] = TEXT("sphereDemo");
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
					        TEXT("Sphere Demo"),
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
   while (GetMessage
	         (&theMsg,
			    NULL,  // use NULL here!  Don't use g_windowHandle!
			    0, 0) )
   {
	   TranslateMessage (&theMsg);

	   DispatchMessage (&theMsg);
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
       //if (g_currentZoomFactor = -1)
       //   g_currentZoomFactor = gmsGetZoomFactorOfGlobe();

       //g_currentZoomFactor = g_currentZoomFactor - 150.0;

       //gmsSetZoomFactorOfGlobe (g_currentZoomFactor);

		 break;
		}

    case ID_ZOOM_OUT:
		{
       //if (g_currentZoomFactor = -1)
       //   g_currentZoomFactor = gmsGetZoomFactorOfGlobe();

       //g_currentZoomFactor = g_currentZoomFactor + 150.0;

       //gmsSetZoomFactorOfGlobe (g_currentZoomFactor);

		 break;
		}

    case ID_ROTATE:
		{
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

   DrawSphere (hDC);

   EndPaint (g_windowHandle, &paintStruct);
}


//-------------------------------------------------
// DrawSphere
//
// Desciption:
//    Routine that draws the sphere.
//-------------------------------------------------
static void DrawSphere
               (HDC hDC)

{

         HPEN            newPen;
         HGDIOBJ         oldPen;
         const int       onePixelWide = 0;
         gmsPolylineType thePolyline;
         double          degrees;
         const int       distanceFromScreen = 1200;
         const int       numPoints = 30;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                RED);

   oldPen = SelectObject (hDC, newPen);

   for (degrees = -90.0; degrees <= 90.0; degrees = degrees + 15.0)
      {
       thePolyline = gmsGetProjectedLongitudeLine
                        (degrees,
                         numPoints,
                         g_width,
                         g_height,
                         distanceFromScreen);

       Polyline
          (hDC,
           (POINT *) &thePolyline.thePoints,
           numPoints);
      }

   for (degrees = -70.0; degrees <= 70.0; degrees = degrees + 15.0)
      {
       thePolyline = gmsGetProjectedLatitudeLine
                        (degrees,
                         numPoints,
                         g_width,
                         g_height,
                         distanceFromScreen);

       Polyline
          (hDC,
           (POINT *) &thePolyline.thePoints,
           numPoints);
      }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}
