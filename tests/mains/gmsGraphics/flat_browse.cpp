//----------------------------------------------------------------------------
// File : flat_browse.cpp
// Date : 17-Jan-00 : initial definition
//
// Description:
//    Windows app that displays all (as many as possible, anyway) of
//    the map data from the BROWSE library and using the FLAT-EARTH model.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------

#include <windows.h>

#include <flat_browse_resource.h>

#include <stdio.h>
#include <gmsColors.h>
#include <gmsDebugUtil.h>
#include <gmsBitmapClass.h>
#include <gmsBrowseMapClass.h>
#include <gmsMapStateMgr.h>
#include <gmsUtilities.h>


//--------------------
// Global Variables
//--------------------

const char           *g_instanceName = {"browse_demo"};

static HINSTANCE     g_hInstance    = (HINSTANCE) 0;

static HWND          g_windowHandle = (HWND) 0;

static HWND          g_zoomDlgHandle = (HWND) 0;

static HWND          g_saveAsDlgHandle = (HWND) 0;

static HWND          g_catDlgHandle = (HWND) 0;

static HWND          g_dqtDlgHandle = (HWND) 0;

static HWND          g_grtDlgHandle = (HWND) 0;

static HWND          g_lhtDlgHandle = (HWND) 0;

static HWND          g_dhtDlgHandle = (HWND) 0;

static HWND          g_latDlgHandle = (HWND) 0;

static HFONT         g_handleToCurrentFont = (HFONT) 0;

static bool          g_themeIsShown[Num_Browse_Themes] = { false,
                                                           false,
                                                           false,
                                                           false,
                                                           false,
                                                           false,
                                                           false,
                                                           false,
                                                           true }; // LibRef

static int           g_width = 450;

static int           g_height = 400;

static double        g_rotationDeg = 20.0;

static gmsBrowseMapClass *g_theBrowseMap;

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

static void parseFileMenuButton
               (int whichCommand);

static void parseCtrlMenuButton
               (int whichCommand);

static void parseThemeMenuButton
               (int whichCommand);

static void parseTableMenuButton
               (int whichCommand);

static BOOL CALLBACK dlgProcZoomControl
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcCatDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcDhtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcLatDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcDqtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcGrtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static BOOL CALLBACK dlgProcLhtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam);

static void setNewFont ();

static void performPaint();

static void drawOncCompilationDate
               (HDC hDC);

static void drawDataVolume
               (HDC hDC);

static void drawDrainage
               (HDC hDC);

static void drawGeographicReference
               (HDC hDC);

static void drawAvailOfHypsographicData
               (HDC hDC);

static void drawOncIndex
               (HDC hDC);

static void drawPoliticalAndOceans
               (HDC hDC);

static void drawPopulatedPlaces
               (HDC hDC);

static void drawLibRef
               (HDC hDC);

static void drawLatLongGrid
               (HDC hDC);

static void drawImage
               (HDC                    hDC,
                COLORREF               whichColor,
                gms_2D_ScreenImageType theImage);

static void drawIndependentPoints
               (HDC                       hDC,
                COLORREF                  whichColor,
                gms_2D_ScreenPolylineType thePoints);

static void annotateMap
               (HDC                 hDC,
                COLORREF            whichColor,
                gmsMapTextArrayType theText);

static bool isVisibleLine
               (gms_2D_ScreenPointType point_1,
                gms_2D_ScreenPointType point_2);

static void doScreenCapture
               (HWND whichWindow);




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
       WNDCLASSEXA wc_16bit;
       MSG         theMsg;
       BOOL        isOk;

   g_hInstance = hInstance;

   //
   // Build the window classification record
   //
   ZeroMemory (&wc_16bit, sizeof(wc_16bit));

   wc_16bit.cbSize        = sizeof (wc_16bit);
   wc_16bit.cbClsExtra    = 0;
   wc_16bit.cbWndExtra    = 0;
   wc_16bit.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
   wc_16bit.hCursor       = LoadCursor (hInstance, IDC_ARROW);
   wc_16bit.hIcon         = LoadIcon (hInstance, IDI_APPLICATION);
   wc_16bit.hInstance     = hInstance;
   wc_16bit.lpfnWndProc   = WndProc;
   wc_16bit.lpszClassName = g_instanceName;
   wc_16bit.lpszMenuName  = MAKEINTRESOURCE(IDR_Browse_Main_Menu);
   wc_16bit.style         = CS_VREDRAW | CS_HREDRAW;

   //
   // Register the window classification record
   // with Windows
   //
   isOk = RegisterClassExA (&wc_16bit);

   if ( ! isOk )
      {
       MessageBoxA
         (NULL,
          "Unable to execute on this platform",
          "Unable to Execute",
          MB_ICONERROR);

       return -1;
      }

   //
   // Now create the window
   //
   g_windowHandle = CreateWindowExA
                      (0,
                       g_instanceName,
                       "Browse - All Features",
                       WS_OVERLAPPEDWINDOW,
                       100, 100,           // (x, y) location
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

       if (g_catDlgHandle)
          isForDialog = IsDialogMessage (g_catDlgHandle, &theMsg);

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
                 const double initialZoomFactor = 79000.0;

           gmsSetMapZoomFactor (initialZoomFactor);

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
   if (whichCommand < Max_File_ID)
      {
       parseFileMenuButton (whichCommand);
      }

   else if (whichCommand < Max_Ctrl_ID)
      {
       parseCtrlMenuButton (whichCommand);
      }

   else if (whichCommand < Max_Theme_ID)
      {
       parseThemeMenuButton (whichCommand);
      }

   else if (whichCommand < Max_Table_ID)
      {
       parseTableMenuButton (whichCommand);
      }

   else
      {
       MessageBoxA
          (NULL,
           "Unknown selection",
           "Not Implemented?",
           MB_OK);
      }

   InvalidateRect
      (g_windowHandle, 
       NULL,  // the whole rectangle
       TRUE); // erase?
}


//-------------------------------------------------
// parseFileMenuButton
//
// Desciption:
//    Routine to parse the button selection from
//    the FILE pulldown menu.
//-------------------------------------------------
static void parseFileMenuButton
               (int whichCommand)

{
   switch (whichCommand)
      {
       case ID_File_Set_Font:
          {
           setNewFont ();

           break;
          }

       case ID_FILE_EXIT:
          {
           PostQuitMessage (0);

           break;
          }

       case ID_FILE_SAVE_AS:
          {
           doScreenCapture (g_windowHandle);

           break;
          }
      }
}


//-------------------------------------------------
// parseCtrlMenuButton
//
// Desciption:
//    Routine to parse the button selection from
//    the CTRL pulldown menu.
//-------------------------------------------------
static void parseCtrlMenuButton
               (int whichCommand)

{
   switch (whichCommand)
      {
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
      }
}


//-------------------------------------------------
// parseThemeMenuButton
//
// Desciption:
//    Routine to parse the button selection from
//    the THEME pulldown menu.
//-------------------------------------------------
static void parseThemeMenuButton
               (int whichCommand)

{
   switch (whichCommand)
      {
       case ID_Lib_Ref:
          {
           g_themeIsShown[(int) gmsBrowse_LibRef] =
              ! g_themeIsShown[(int) gmsBrowse_LibRef];

           break;
          }

       case ID_ONC_Compilation:
          {
           g_themeIsShown[(int) gmsBrowse_CO] =
              ! g_themeIsShown[(int) gmsBrowse_CO];

           break;
          }

       case ID_Data_Availability:
          {
           g_themeIsShown[(int) gmsBrowse_DA] =
              ! g_themeIsShown[(int) gmsBrowse_DA];

           break;
          }

       case ID_Drainage:
          {
           g_themeIsShown[(int) gmsBrowse_DN] =
              ! g_themeIsShown[(int) gmsBrowse_DN];

           break;
          }

       case ID_Data_Volume:
          {
           g_themeIsShown[(int) gmsBrowse_DV] =
              ! g_themeIsShown[(int) gmsBrowse_DV];

           break;
          }

       case ID_Geographic_Ref:
          {
           g_themeIsShown[(int) gmsBrowse_GR] =
              ! g_themeIsShown[(int) gmsBrowse_GR];

           break;
          }

       case ID_ONC_Index:
          {
           g_themeIsShown[(int) gmsBrowse_IN] =
              ! g_themeIsShown[(int) gmsBrowse_IN];

           break;
          }

       case ID_Political_Oceans:
          {
           g_themeIsShown[(int) gmsBrowse_PO] =
              ! g_themeIsShown[(int) gmsBrowse_PO];

           break;
          }

       case ID_Populated_Places:
          {
           g_themeIsShown[(int) gmsBrowse_PP] =
              ! g_themeIsShown[(int) gmsBrowse_PP];

           break;
          }
     }
}


//-------------------------------------------------
// parseTableMenuButton
//
// Desciption:
//    Routine to parse the button selection from
//    the TABLE pulldown menu.
//-------------------------------------------------
static void parseTableMenuButton
               (int whichCommand)

{
   switch (whichCommand)
      {
       case ID_Database_Hdr_Tbl:
          {
           if (g_dhtDlgHandle == (HWND) 0)
              {
               g_dhtDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_DHT_DIALOG),
                                     g_windowHandle,
                                     dlgProcDhtDialog);
              }

           ShowWindow (g_dhtDlgHandle, SW_SHOW);

           break;
          }

       case ID_Lib_Attribute_Tbl:
          {
           if (g_latDlgHandle == (HWND) 0)
              {
               g_latDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_LAT_DIALOG),
                                     g_windowHandle,
                                     dlgProcLatDialog);
              }

           ShowWindow (g_latDlgHandle, SW_SHOW);

           break;
          }

       case ID_Coverage_Attrib_Tbl:
          {
           if (g_catDlgHandle == (HWND) 0)
              {
               g_catDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_CAT_DIALOG),
                                     g_windowHandle,
                                     dlgProcCatDialog);
              }

           ShowWindow (g_catDlgHandle, SW_SHOW);

           break;
          }

       case ID_Data_Quality_Tbl:
          {
           if (g_dqtDlgHandle == (HWND) 0)
              {
               g_dqtDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_DQT_DIALOG),
                                     g_windowHandle,
                                     dlgProcDqtDialog);
              }

           ShowWindow (g_dqtDlgHandle, SW_SHOW);

           break;
          }

       case ID_Geo_Ref_Tbl:
          {
           if (g_grtDlgHandle == (HWND) 0)
              {
               g_grtDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_GRT_DIALOG),
                                     g_windowHandle,
                                     dlgProcGrtDialog);
              }

           ShowWindow (g_grtDlgHandle, SW_SHOW);

           break;
          }

       case ID_Lib_Header_Tbl:
          {
           if (g_lhtDlgHandle == (HWND) 0)
              {
               g_lhtDlgHandle = CreateDialog
                                    (g_hInstance,
                                     MAKEINTRESOURCE(IDD_LHT_DIALOG),
                                     g_windowHandle,
                                     dlgProcLhtDialog);
              }

           ShowWindow (g_lhtDlgHandle, SW_SHOW);

           break;
          }
      }
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
       sprintf(charBuffer, "%.2lf", g_zoomAmount);

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
// dlgProcCatDialog
//
// Desciption:
//-------------------------------------------------
static BOOL CALLBACK dlgProcCatDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const char *catTbl = g_theBrowseMap->gmsGetDataTable
                                 (gms_CoverageAttributeTable);

   if (message == WM_INITDIALOG)
      {
       SetDlgItemTextA
          (dialogHandle,
           IDC_CAT_Edit,
           catTbl);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_catDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_catDlgHandle = (HWND) 0;

               break;
              }

           case IDC_CAT_Bitmap:
              {
               doScreenCapture (dialogHandle);

               break;
              }
          }

       return TRUE;
      }

   return FALSE;
}


//-------------------------------------------------
// dlgProcDhtDialog
//
// Desciption:
//-------------------------------------------------
static BOOL CALLBACK dlgProcDhtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const char *dhtTbl = g_theBrowseMap->gmsGetDataTable
                                 (gms_DatabaseHeaderTable);

   if (message == WM_INITDIALOG)
      {
       SetDlgItemTextA
          (dialogHandle,
           IDC_DHT_Edit,
           dhtTbl);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_dhtDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_dhtDlgHandle = (HWND) 0;

               break;
              }

           case IDC_DHT_Bitmap:
              {
               doScreenCapture (dialogHandle);

               break;
              }
          }

       return TRUE;
      }

   return FALSE;
}


//-------------------------------------------------
// dlgProcLatDialog
//
// Desciption:
//-------------------------------------------------
static BOOL CALLBACK dlgProcLatDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const char *latTbl = g_theBrowseMap->gmsGetDataTable
                                 (gms_LibraryAttributeTable);

   if (message == WM_INITDIALOG)
      {
       SetDlgItemTextA
          (dialogHandle,
           IDC_LAT_Edit,
           latTbl);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_latDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_latDlgHandle = (HWND) 0;

               break;
              }

           case IDC_LAT_Bitmap:
              {
               doScreenCapture (dialogHandle);

               break;
              }
          }

       return TRUE;
      }

   return FALSE;
}


//-------------------------------------------------
// dlgProcDqtDialog
//
// Desciption:
//-------------------------------------------------
static BOOL CALLBACK dlgProcDqtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const char *dqtTbl = g_theBrowseMap->gmsGetDataTable
                                 (gms_DataQualityTable);

   if (message == WM_INITDIALOG)
      {
       SetDlgItemTextA
          (dialogHandle,
           IDC_DQT_Edit,
           dqtTbl);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_dqtDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_dqtDlgHandle = (HWND) 0;

               break;
              }

           case IDC_DQT_Bitmap:
              {
               doScreenCapture (dialogHandle);

               break;
              }
          }

       return TRUE;
      }

   return FALSE;
}


//-------------------------------------------------
// dlgProcGrtDialog
//
// Desciption:
//-------------------------------------------------
static BOOL CALLBACK dlgProcGrtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const char *grtTbl = g_theBrowseMap->gmsGetDataTable
                                 (gms_GeographicReferenceTable);

   if (message == WM_INITDIALOG)
      {
       SetDlgItemTextA
          (dialogHandle,
           IDC_GRT_Edit,
           grtTbl);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_grtDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_grtDlgHandle = (HWND) 0;

               break;
              }

           case IDC_GRT_Bitmap:
              {
               doScreenCapture (dialogHandle);

               break;
              }
          }

       return TRUE;
      }

   return FALSE;
}


//-------------------------------------------------
// dlgProcLhtDialog
//
// Desciption:
//-------------------------------------------------
static BOOL CALLBACK dlgProcLhtDialog
                           (HWND   dialogHandle,
                            UINT   message,
                            WPARAM wParam,
                            LPARAM lParam)

{
         const char *lhtTbl = g_theBrowseMap->gmsGetDataTable
                                 (gms_LibraryHeaderTable);

   if (message == WM_INITDIALOG)
      {
       SetDlgItemTextA
          (dialogHandle,
           IDC_LHT_Edit,
           lhtTbl);

       return TRUE;
      }

   else if (message == WM_CLOSE)
      {
       DestroyWindow (dialogHandle);

       g_lhtDlgHandle = (HWND) 0;

       return TRUE;
      }

   else if (message == WM_COMMAND)
      {
       switch LOWORD(wParam)
          {
           case IDCANCEL:
              {
               DestroyWindow (dialogHandle);

               g_lhtDlgHandle = (HWND) 0;

               break;
              }

           case IDC_LHT_Bitmap:
              {
               doScreenCapture (dialogHandle);

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

   if (g_theBrowseMap == NULL)
      g_theBrowseMap = new gmsBrowseMapClass (gmsFlat);

   hDC = BeginPaint (g_windowHandle, &paintStruct);

   if (g_handleToCurrentFont != (HFONT) 0)
      SelectObject (hDC, g_handleToCurrentFont);

   drawLatLongGrid (hDC);

   if (g_themeIsShown[gmsBrowse_LibRef])
      drawLibRef (hDC);

   if (g_themeIsShown[gmsBrowse_CO])
      drawOncCompilationDate (hDC);

   if (g_themeIsShown[gmsBrowse_DV])
      drawDataVolume (hDC);

   if (g_themeIsShown[gmsBrowse_DN])
      drawDrainage (hDC);

   if (g_themeIsShown[gmsBrowse_GR])
      drawGeographicReference (hDC);

   if (g_themeIsShown[gmsBrowse_DA])
      drawAvailOfHypsographicData (hDC);

   if (g_themeIsShown[gmsBrowse_IN])
      drawOncIndex (hDC);

   if (g_themeIsShown[gmsBrowse_PO])
      drawPoliticalAndOceans (hDC);

   if (g_themeIsShown[gmsBrowse_PP])
      drawPopulatedPlaces (hDC);

   EndPaint (g_windowHandle, &paintStruct);
}


//-------------------------------------------------
// drawOncCompilationDate
//
// Desciption:
//-------------------------------------------------
static void drawOncCompilationDate
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_CO);

   drawImage
      (hDC,
       MAGENTA,
       tempImage);
}


//-------------------------------------------------
// drawDataVolume
//
// Desciption:
//-------------------------------------------------
static void drawDataVolume
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_DV);

   drawImage
      (hDC,
       SeaGreen,
       tempImage);
}


//-------------------------------------------------
// drawDrainage
//
// Desciption:
//-------------------------------------------------
static void drawDrainage
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_DN);

   drawImage
      (hDC,
       BLUE, // DodgerBlue,
       tempImage);

   //tempImage = g_theBrowseMap->gmsGetBrowseEbrImage (gmsBrowse_DN);

   //drawImage
   //   (hDC,
   //    DARK_RED,
   //    tempImage);
}


//-------------------------------------------------
// drawGeographicReference
//
// Desciption:
//-------------------------------------------------
static void drawGeographicReference
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;
       gmsMapTextArrayType    tempText;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_GR);

   drawImage
      (hDC,
       IndianRed,
       tempImage);

   tempText = g_theBrowseMap->gmsGetTextItems (gmsBrowse_GR);

   annotateMap
      (hDC,
       IndianRed,
       tempText);
}


//-------------------------------------------------
// drawAvailOfHypsographicData
//
// Desciption:
//-------------------------------------------------
static void drawAvailOfHypsographicData
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_DA);

   drawImage
      (hDC,
       navajo,
       tempImage);
}


//-------------------------------------------------
// drawOncIndex
//
// Desciption:
//-------------------------------------------------
static void drawOncIndex
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;
       gmsMapTextArrayType    tempText;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_IN);

   drawImage
      (hDC,
       red4,
       tempImage);

   tempText = g_theBrowseMap->gmsGetTextItems (gmsBrowse_IN);

   annotateMap
      (hDC,
       red4,
       tempText);
}


//-------------------------------------------------
// drawPoliticalAndOceans
//
// Desciption:
//-------------------------------------------------
static void drawPoliticalAndOceans
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;
       gmsMapTextArrayType    tempText;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_PO);

   drawImage
      (hDC,
       RED,
       tempImage);

   tempText = g_theBrowseMap->gmsGetTextItems (gmsBrowse_PO);

   annotateMap
      (hDC,
       RED,
       tempText);
}


//-------------------------------------------------
// drawPopulatedPlaces
//
// Desciption:
//-------------------------------------------------
static void drawPopulatedPlaces
               (HDC hDC)

{
       gms_2D_ScreenPolylineType tempPoints;
       gmsMapTextArrayType       tempText;

   tempPoints = g_theBrowseMap->gmsGetBrowseNodes (gmsBrowse_PP);

   drawIndependentPoints
      (hDC,
       BLACK,
       tempPoints);

   tempText = g_theBrowseMap->gmsGetTextItems (gmsBrowse_PP);

   annotateMap
      (hDC,
       BLACK,
       tempText);
}


//-------------------------------------------------
// drawLibRef
//
// Desciption:
//-------------------------------------------------
static void drawLibRef
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_LibRef);

   drawImage
      (hDC,
       BLACK,
       tempImage);
}


//-------------------------------------------------
// drawLatLongGrid
//
// Desciption:
//-------------------------------------------------
static void drawLatLongGrid
               (HDC hDC)

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetLatitudeGrid();

   drawImage
      (hDC,
       GREEN,
       tempImage);

   tempImage = g_theBrowseMap->gmsGetLongitudeGrid();

   drawImage
      (hDC,
       GREEN,
       tempImage);
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
// annotateMap
//
// Desciption:
//-------------------------------------------------
static void annotateMap
               (HDC                 hDC,
                COLORREF            whichColor,
                gmsMapTextArrayType theText)

{
      int i;

   for (i = 0; i < theText.numTextRecords; i++)
      {
       TextOutA
          (hDC,
           theText.textRecords[i].theCoord.x,
           theText.textRecords[i].theCoord.y,
           theText.textRecords[i].theString,
           theText.textRecords[i].numChars);
      }
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
static void doScreenCapture
               (HWND whichWindow)

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

   infoFromSaveAsDlg.hwndOwner      = whichWindow;
   infoFromSaveAsDlg.lpstrFile      = fileName;
   infoFromSaveAsDlg.lpstrFileTitle = fileTitle;

   dlgIsOk = GetSaveFileNameA (&infoFromSaveAsDlg);

   if ( ! dlgIsOk )
      {
       return;  // i.e. CANCEL
      }

   {
         gmsBitmapClass theBitmap(whichWindow);

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
