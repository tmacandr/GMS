//----------------------------------------------------------------------------
// File : graphics_main.cpp
// Date : 03-Jul-99 : initial definition
//
// Description:
//    This program demonstrates the following graphics functions from the
//    Win API:
//                LineTo
//                PolyLine
//                Arc
//                Rectangle
//                Ellipse
//                RoundRect
//                Pie
//                Chord
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <math.h>
#include <windows.h>
#include <gmsCircleUtilities.h>
#include <graphics_resource.h>
#include <gmsColors.h>


//--------------------
// Global Variables
//--------------------

static HINSTANCE g_hInstance    = (HINSTANCE) 0;

static HWND      g_windowHandle = (HWND) 0;

static int       g_width = 0;

static int       g_height = 0;

typedef struct _currentStateType
{
 bool LineIsOn;
 bool PolylineIsOn;
 bool ArcIsOn;
 bool RectangleIsOn;
 bool EllipseIsOn;
 bool RoundRectangleIsOn;
 bool PieIsOn;
 bool ChordIsOn;
 bool SphereIsOn;
} currentStateType;

static currentStateType currentState;


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

static void DrawRectangle
               (HDC hDC);

static void DrawEllipse
               (HDC hDC);

static void DrawArc 
               (HDC hDC);

static void DrawChord
               (HDC hDC);

static void DrawPie
               (HDC hDC);

static void DrawPolyline
               (HDC hDC);

static void DrawRoundRectangle
               (HDC hDC);

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
		 TCHAR      instanceName[] = TEXT("graphicsDemo");
		 TCHAR      mainMenuName[] = TEXT("Main_Menu");
		 BOOL       isOk;

   g_hInstance = hInstance;

   ZeroMemory (&currentState, sizeof(currentState));

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
					        TEXT("Graphics Demo"),
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
	 	 MessageBox
		 	(NULL,
			 TEXT("Exiting"),
			 TEXT("C-ya"),
			 MB_OK);

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

	case ID_DRAW_LINE:
		{
		 currentState.LineIsOn = ! currentState.LineIsOn;

		 break;
		}

    case ID_DRAW_POLYLINE:
		{
		 currentState.PolylineIsOn = ! currentState.PolylineIsOn;

		 break;
		}

    case ID_DRAW_ARC:
		{
		 currentState.ArcIsOn = ! currentState.ArcIsOn;

		 break;
		}

    case ID_DRAW_RECTANGLE:
		{
		 currentState.RectangleIsOn = ! currentState.RectangleIsOn;

		 break;
		}

    case ID_DRAW_ELLIPSE:
		{
		 currentState.EllipseIsOn = ! currentState.EllipseIsOn;

		 break;
		}

    case ID_DRAW_ROUNDRECTANGLE:
		{
		 currentState.RoundRectangleIsOn = ! currentState.RoundRectangleIsOn;

		 break;
		}

    case ID_DRAW_PIE:
		{
		 currentState.PieIsOn = ! currentState.PieIsOn;

		 break;
		}

    case ID_DRAW_CHORD:
		{
		 currentState.ChordIsOn = ! currentState.ChordIsOn;

		 break;
		}

    case ID_DRAW_SPHERE:
		{
		 currentState.SphereIsOn = ! currentState.SphereIsOn;

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

   //
   // Draw an X across the canvas
   //
   if ( currentState.LineIsOn )
   {
    MoveToEx
	   (hDC,
	    0, 0,
		 NULL);

    LineTo
	   (hDC,
	    g_width, g_height);

    MoveToEx
	   (hDC,
	    g_width, 0,
		 NULL);

    LineTo
	   (hDC,
	    0, g_height);
   }

   if ( currentState.RectangleIsOn )
	   DrawRectangle (hDC);

   if ( currentState.EllipseIsOn )
	   DrawEllipse (hDC);

   if ( currentState.ArcIsOn )
	   DrawArc (hDC);

   if (currentState.ChordIsOn )
	   DrawChord (hDC);

   if (currentState.PieIsOn )
	   DrawPie (hDC);
   
   if (currentState.PolylineIsOn )
       DrawPolyline (hDC);
   
   if (currentState.RoundRectangleIsOn )
	   DrawRoundRectangle (hDC);

   if (currentState.SphereIsOn )
	   DrawSphere (hDC);

   EndPaint (g_windowHandle, &paintStruct);
}


//-------------------------------------------------
// DrawRectangle
//
// Desciption:
//    Routine that draws a filled rectangle on the
//    canvas.
//-------------------------------------------------
static void DrawRectangle
               (HDC hDC)

{
	     POINT    point_1, point_2;
        HGDIOBJ  oldBrush;
		  HGDIOBJ  newBrush;

	point_1.x = (int) (0.5 * g_width);
	point_1.y = (int) (0.5 * g_height);

	point_2.x = (int) (0.75 * g_width);
	point_2.y = (int) (0.75 * g_height);

	newBrush = CreateSolidBrush (ORANGE);

	oldBrush = SelectObject
		          (hDC,
		           newBrush);

	Rectangle
       (hDC,
	     point_1.x, point_1.y,
		  point_2.x, point_2.y);

	SelectObject (hDC, oldBrush);

	DeleteObject (newBrush);
}


//-------------------------------------------------
// DrawEllipse
//
// Desciption:
//    Routine that draws a filled ellipse on the
//    canvas.
//-------------------------------------------------
static void DrawEllipse
               (HDC hDC)

{
	     POINT    point_1, point_2;
        HGDIOBJ  oldBrush;
		  HGDIOBJ  newBrush;

	point_1.x = (int) (0.25 * g_width);
	point_1.y = (int) (0.25 * g_height);

	point_2.x = (int) (0.5 * g_width);
	point_2.y = (int) (0.5 * g_height);

	newBrush = CreateSolidBrush (YELLOW);

	oldBrush = SelectObject
		          (hDC,
		           newBrush);

	Ellipse
       (hDC,
	     point_1.x, point_1.y,
		  point_2.x, point_2.y);

	SelectObject (hDC, oldBrush);

	DeleteObject (newBrush);
}


//-------------------------------------------------
// DrawArc
//
// Desciption:
//    Routine that draws a filled  on the
//    canvas.
//-------------------------------------------------
static void DrawArc 
               (HDC hDC)

{
      RECT      theBounds;
      POINT     startPoint;
      POINT     stopPoint;
      HPEN      newPen;
      HGDIOBJ   oldPen;
      const int onePixelWide = 0;

   theBounds.left   = g_width - 50;
   theBounds.top    = (int) (0.1 * g_height);
   theBounds.right  = g_width - 5;
   theBounds.bottom = theBounds.top + 50;

   startPoint.x = g_width;
   startPoint.y = 0;

   stopPoint.x = g_width - 30;
   stopPoint.y = (int) (0.50 * g_height);

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                DARK_GREEN);

   oldPen = SelectObject (hDC, newPen);

   Arc (hDC,
        theBounds.left, theBounds.top,
        theBounds.right, theBounds.bottom,
        startPoint.x, startPoint.y,
        stopPoint.x,  stopPoint.y);

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// DrawChord
//
// Desciption:
//    Routine that draws a filled  on the
//    canvas.
//-------------------------------------------------
static void DrawChord
               (HDC hDC)

{
         RECT      theBounds;
         POINT     startPoint;
         POINT     stopPoint;
         HBRUSH    newBrush;
         HGDIOBJ   oldBrush;
         HPEN      newPen;
         HGDIOBJ   oldPen;
         const int onePixelWide = 0;

   theBounds.left   = (int) (0.20 * g_width);
   theBounds.top    = (int) (0.50 * g_height);
   theBounds.right  = (int) (0.60 * g_width);
   theBounds.bottom = (int) (0.90 * g_height);   

   startPoint.x = (int) (0.25 * g_width);
   startPoint.y = 0;

   stopPoint.x = 0;
   stopPoint.y = (int) (0.66 * g_height);

   newBrush = CreateSolidBrush (WEIRD_RED);

   oldBrush = SelectObject (hDC, newBrush);

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                BLUE);

   oldPen = SelectObject (hDC, newPen);

   Chord
      (hDC,
       theBounds.left, theBounds.top,
       theBounds.right, theBounds.bottom,
       startPoint.x, startPoint.y,
       stopPoint.x, stopPoint.y);

   SelectObject (hDC, oldBrush);

   DeleteObject (newBrush);

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// DrawPie
//
// Desciption:
//    Routine that draws a filled  on the
//    canvas.
//-------------------------------------------------
static void DrawPie
               (HDC hDC)

{
         RECT    theBounds;
         POINT   startPoint;
         POINT   endPoint;
         HBRUSH  newBrush;
         HGDIOBJ oldBrush;
         HPEN    newPen;
         HGDIOBJ oldPen;
         const int onePixelWide = 0;

   //
   // This just sets up an arbitrary geometry
   //
   theBounds.left   = (int) (0.50 * g_width);
   theBounds.top    = (int) (0.30 * g_height);
   theBounds.right  = (int) (0.85 * g_width);
   theBounds.bottom = (int) (0.55 * g_height);   

   startPoint.x = (int) (0.75 * g_width);
   startPoint.y = 0;

   endPoint.x = 0;
   endPoint.y = (int) (0.45 * g_height);

   //
   // Now do the graphics operations
   //
   newBrush = CreateSolidBrush (TURQOUISE);

   oldBrush = SelectObject (hDC, newBrush);

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                RED);

   oldPen = SelectObject (hDC, newPen);

   Pie (hDC,
        theBounds.left, theBounds.top,
        theBounds.right, theBounds.bottom,
        startPoint.x, startPoint.y,
        endPoint.x, endPoint.y);

   SelectObject (hDC, oldBrush);

   DeleteObject (newBrush);

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// DrawPolyline
//
// Desciption:
//    Routine that draws a filled  on the
//    canvas.
//-------------------------------------------------
static void DrawPolyline
               (HDC hDC)

{
	     const int numPoints = 10;
        POINT     thePoints[numPoints] =
                      { {0, 0},
						      {5, 5},
		                  {10, 10},
                        {15, 10},
                        {20, 25},
                        {25, 25},
						      {30, 35},
		                  {35, 35},
                        {40, 50},
                        {g_width, g_height},
					       };
         HPEN      newPen;
         HGDIOBJ   oldPen;
         const int onePixelWide = 0;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                GREEN);

   oldPen = SelectObject (hDC, newPen);

   Polyline
	  (hDC,
	   thePoints,
	   numPoints);

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// DrawRoundRectangle
//
// Desciption:
//    Routine that draws a filled  on the
//    canvas.
//-------------------------------------------------
static void DrawRoundRectangle
               (HDC hDC)

{
       int       x1, y1;
		 int       x2, y2;
		 int       roundWidth, roundHeight;
		 HBRUSH    newBrush;
		 HGDIOBJ   oldBrush;

   x1 = (int) (0.1 * g_width);
   y1 = (int) (0.1 * g_height);

   x2 = (int) (0.4 * g_width);
   y2 = (int) (0.4 * g_height),

   roundWidth  = (int) (0.1 * g_width);
   roundHeight = (int) (0.1 * g_height);

   newBrush = CreateSolidBrush (BLUE);

   oldBrush = SelectObject (hDC, newBrush);

   RoundRect
	   (hDC,
	    x1, y1,
       x2, y2,
		 roundWidth, roundHeight);

   SelectObject (hDC, oldBrush);

   DeleteObject (newBrush);
}


//-------------------------------------------------
// DrawSphere
//
// Desciption:
//    Routine that draws a sphere  on the
//    canvas.
//-------------------------------------------------
static void DrawSphere
               (HDC hDC)

{
         const int xCenter = (g_width / 2);
         const int yCenter = (g_height / 2);
         const int x = xCenter + 10;
         const int y = yCenter + 10;

   gmsDraw_2D_Circle
      (hDC,
       xCenter, yCenter,
       x, y);
}


//-------------------------------------------------
// DrawCone
//
// Desciption:
//    Routine that draws a cone on the
//    canvas.
//-------------------------------------------------
static void DrawCone
               (HDC hDC)

{
         double       x, y, z;
         double       u;
         double       w;
         const double Two_Pi = 2.0 * 3.14159;
         const double delta_u = 0.01;
         const double delta_w = 0.01;
         const double alpha = 0.15;
         const double b = 0.5 * g_width;
         const double a = 0.5 * g_height;
         const double k = 1024.0;
         const int numPoints = 100;
         POINT     thePoints[numPoints];
         int       index;
         HPEN      newPen;
         HGDIOBJ   oldPen;
         const int onePixelWide = 0;

   for (u = 0.0; u < 1.0; u = u + delta_u)
      {
       index = 0;

       for (w = 0.0; w < 1.0; w = w + delta_w)
          {
           //------------------
           // Unit dimensions
           //------------------
           x = u * cos (Two_Pi * w);

           y = u * sin (Two_Pi * w);

           z = alpha * w;

           //------------------
           // Screen dimensions
           //------------------
           x = x * g_width;

           y = y * g_height;

           z = z * 1024;  // I guess

           //------------------
           // Translate to middle of screen
           //------------------
           x = x + (long) (0.5 * g_width);

           y = y + (long) (0.5 * g_height);

           //------------------
           // Projection
           //------------------
           thePoints[index].x = (long) ( (b * z + k * y) / (k + z) );

           thePoints[index].y = (long) ( (a * z + k * x) / (k + z) );

           index++;
          }

       newPen = CreatePen
                   (PS_SOLID,
                    onePixelWide,
                    HotPink);

       oldPen = SelectObject (hDC, newPen);

       Polyline
          (hDC,
           thePoints,
           numPoints);

       SelectObject (hDC, oldPen);

       DeleteObject (newPen);
      }
}
