//----------------------------------------------------------------------------
// File : map_4.cpp
// Date : 09-Oct-99 : initial definition
//        18-Oct-99 : separate world coords from screen coords
//        09-Dec-99 : update due to file renames, static lib, etc
//
// Description:
//    This program displays several edge tables (from the THEMATIC libraries)
//    graphically.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>
#include <map_4_resource.h>
#include <gmsUtilities.h>
#include <gmsColors.h>
#include <gmsEdgeTable.h>
#include <gmsScreenCoordUtils.h>
#include <gmsTransformationUtils.h>
#include <gmsWorldCoordUtils.h>
#include <gmsLatLongGridClass.h>


//--------------------
// Global Variables
//--------------------

static HINSTANCE     g_hInstance    = (HINSTANCE) 0;

static HWND          g_windowHandle = (HWND) 0;

static int           g_width = 0;

static int           g_height = 0;

static int           g_currentEdge = 0;

static const int     Num_Edge_Tables = 31;

static edgeTableType *theEDG[Num_Edge_Tables];

static double        g_currentZoomFactor = -1.0;

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

static const int       numFacePoints = 4;

static twoDimCoordType globeFace[numFacePoints] = { {0.0  ,  90.0},
                                                    {0.0  , -90.0},
                                                    {90.0 ,  0.0},
                                                    {-90.0,  0.0} };

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

static void DrawLatLongLines
               (HDC hDC);

static char *edgeTables
               (int index);




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
		 TCHAR      instanceName[] = TEXT("edgeDemo");
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
					        TEXT("Edges Demo"),
						     WS_OVERLAPPEDWINDOW,
						     100, 50,  // (x, y) location
						     600, 400,  // width/height of window
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
   if (whichCommand == ID_FILE_EXIT)
	   {
		 PostQuitMessage (0);
	   }

	else if ( (whichCommand >= ID_browse_libref) ||
             (whichCommand <= ID_libref) )
		{
       g_currentEdge = whichCommand - ID_browse_libref;
		}

	else
		{
		 MessageBox
		 	(NULL,
			 TEXT("Unknown selection"),
			 TEXT("Error"),
			 MB_OK);
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
		 HDC               hDC;
		 PAINTSTRUCT       paintStruct;

   hDC = BeginPaint (g_windowHandle, &paintStruct);

   if (theEDG[g_currentEdge] == NULL)
      {
       theEDG[g_currentEdge] = gmsGetEdgeTable
                                  (edgeTables(g_currentEdge));
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
         edgeTableType          *whichEDG = theEDG[g_currentEdge];
         HPEN                   newPen;
         HGDIOBJ                oldPen;
         const int              onePixelWide = 0;
         gms_3D_WorldPointType  worldPoint_3D;
         gms_3D_ScreenPointType screenPoint_3D;
         gms_2D_ScreenPointType screenPoint_2D;
         int                    i, j;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                RED);

   oldPen = SelectObject (hDC, newPen);

   for (i = 0; i < whichEDG->numRecords; i++)
      for (j = 0; j < (whichEDG->edgeRecords[i].numCoords - 1); j++)
         {
          worldPoint_3D = gmsConvertLatLongToSphericalPoint
                                   (whichEDG->edgeRecords[i].theCoords[j]);

          screenPoint_3D = gmsConvertWorldCoordToScreenCoord (worldPoint_3D);

          screenPoint_3D = gmsTranslateThreeDimPoint
                              (screenPoint_3D,
                               (g_width / 2),
                               (g_height / 2),
                               1024);

          screenPoint_2D = gmsConvertToProjectedPoint
                                  (screenPoint_3D,
                                   g_width,
                                   g_height);

          MoveToEx
	          (hDC,
	           screenPoint_2D.x, screenPoint_2D.y,
              NULL);

          worldPoint_3D = gmsConvertLatLongToSphericalPoint
                               (whichEDG->edgeRecords[i].theCoords[j + 1]);

          screenPoint_3D = gmsConvertWorldCoordToScreenCoord
                                  (worldPoint_3D);

          screenPoint_3D = gmsTranslateThreeDimPoint
                                  (screenPoint_3D,
                                   (g_width / 2),
                                   (g_height / 2),
                                   1024);

          screenPoint_2D = gmsConvertToProjectedPoint
                                  (screenPoint_3D,
                                   g_width,
                                   g_height);

          LineTo
	          (hDC,
	           screenPoint_2D.x, screenPoint_2D.y);
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
         static gmsLatLongGridClass       theLatLongGrid (10.0, gmsEllipsoid);
         static gms_2D_ScreenImageType    latImage;
         static gms_2D_ScreenImageType    lonImage;
         int                              i;

   newPen = CreatePen
               (PS_SOLID,
                onePixelWide,
                GREEN);

   oldPen = SelectObject (hDC, newPen);

   latImage = theLatLongGrid.gmsGetLatitudeGrid ();

   for (i = 0; i < latImage.numLines; i++)
      {
       //Polyline
       //   (hDC,
       //    (POINT *) &latImage.imageLines[i].points,
       //    latImage.imageLines[i].numPoints);
      }

   lonImage = theLatLongGrid.gmsGetLongitudeGrid ();

   for (i = 0; i < lonImage.numLines; i++)
      {
       //Polyline
       //   (hDC,
       //    (POINT *) &lonImage.imageLines[i].points,
       //    lonImage.imageLines[i].numPoints);
      }

   SelectObject (hDC, oldPen);

   DeleteObject (newPen);
}


//-------------------------------------------------
// edgeTables
//
// Desciption:
//-------------------------------------------------
static char *edgeTables (int index)

{
      static char *theFile;
      static char *files[Num_Edge_Tables] =
                     { {"libref/edg"},
                       {"po/z/c/12/edg"},
                       {"po/r/e/13/edg"},
                       {"po/n/a/11/edg"},
                       {"pp/p/j/33/edg"},
                       {"rd/r/e/13/edg"},
                       {"hy/u/a/11/edg"},
                       /* Level 3 - NO feature tbl */
                       {"po/edg"},
                       {"dv/edg"},
                       {"da/edg"},
                       {"gr/edg"},
                       {"in/edg"},
                       {"dn/edg"},
                       {"co/edg"},
                       {"vg/g/h/23/edg"},
                       {"pp/q/j/21/edg"},
                       {"lc/r/j/12/edg"},
                       {"tileref/edg"},
                       /* Level 3 - WITH feature tbl */
                       {"po/u/c/22/edg"},
                       {"hy/r/h/31/edg"},
                       {"dq/r/h/31/edg"},
                       {"dn/r/h/31/edg"},
                       {"cl/q/j/21/edg"},
                       /* Level 1-2                  */
                       {"ut/n/j/13/edg"},
                       {"ts/n/j/13/edg"},
                       {"rr/n/j/13/edg"},
                       {"rd/n/j/13/edg"},
                       {"ph/u/a/11/edg"},
                       {"of/q/h/23/edg"},
                       {"hs/r/e/11/edg"},
                       /* LibRef                     */
                       {"libref/edg"} };

   if ( (index == 0) || ( (index > 6) && (index < 14) ) )

      theFile = gmsGetBrowseFullPath (files[index]);

   else

      theFile = gmsGetDcwFullPath (files[index]);

   return theFile;
}