//----------------------------------------------------------------------------
// File : main.cpp
// Date : 03-Jul-99 : initial definition
//        14-Oct-99 : distinguish between world and screen coordinates
//        09-Dec-99 : update due to renames, changes to I/F, static lib, etc
//
// Description:
//    This program displays the "browse" information.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>
#include <map_1_resource.h>
#include <gmsColors.h>
#include <gmsEdgeTable.h>
#include <gmsScreenCoordUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsMapStateMgr.h>

//--------------------
// Global Variables
//--------------------

static HINSTANCE g_hInstance    = (HINSTANCE) 0;

static HWND      g_windowHandle = (HWND) 0;

static int       g_width = 500;

static int       g_height = 500;

static edgeTableType *theEDG = NULL;

static POINT     g_viewPoint = { (g_width / 2), (g_height / 2) };

static double    g_zoomAmount = 10000.0;


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

static void DrawEdges
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
		 TCHAR      instanceName[] = TEXT("flatEarthDemo");
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
   wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
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
						     50, 50,             // initial (x, y) location
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

    case ID_MOVE_NORTH:
		{
       g_viewPoint.y = g_viewPoint.y + (int) (0.10 * g_height);

		 break;
		}

    case ID_MOVE_SOUTH:
		{
       g_viewPoint.y = g_viewPoint.y - (int) (0.10 * g_height);

		 break;
		}

    case ID_MOVE_EAST:
		{
       g_viewPoint.x = g_viewPoint.x - (int) (0.10 * g_width);

		 break;
		}

    case ID_MOVE_WEST:
		{
       g_viewPoint.x = g_viewPoint.x + (int) (0.10 * g_width);

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
         gms_2D_WorldPointType  startWorldPoint;
         gms_2D_WorldPointType  endWorldPoint;
         gms_2D_ScreenPointType startScreenPoint;
         gms_2D_ScreenPointType endScreenPoint;
         int                    i, j;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                RED);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < theEDG->numRecords; i++)
      for (j = 0; j < (theEDG->edgeRecords[i].numCoords - 1); j++)
      {
       startWorldPoint = gmsConvertLatLongToFlatPoint
                            (theEDG->edgeRecords[i].theCoords[j]);

       startScreenPoint =
          gmsConvertFlatWorldCoordToScreenCoord (startWorldPoint);

       startScreenPoint = gmsTranslateTwoDimPoint
                             (startScreenPoint,
                              g_viewPoint.x,
                              g_viewPoint.y);

       endWorldPoint = gmsConvertLatLongToFlatPoint
                          (theEDG->edgeRecords[i].theCoords[j + 1]);

       endScreenPoint =
          gmsConvertFlatWorldCoordToScreenCoord (endWorldPoint);

       endScreenPoint = gmsTranslateTwoDimPoint
                           (endScreenPoint,
                            g_viewPoint.x,
                            g_viewPoint.y);

       if (gmsIsVisibleLine (startScreenPoint, endScreenPoint) )
          {
           MoveToEx
	           (hDC,
	            startScreenPoint.x, startScreenPoint.y,
               NULL);

           LineTo
              (hDC,
               endScreenPoint.x, endScreenPoint.y);
          }
      }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}
