//-------------------------------------------------------------------------
// File : flat_browse.cpp
// Date : 10-Feb-00 : initial definition
//        24-Feb-00 : blew away stuff at CSUN.  Start over!
//
// Description:
//    A X/Motif program that exercises the "browse" library of DCW
//    database.  Prove concept of portability.
//-------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
   //---------//
#include <Xm/Form.h>
#include <Xm/DrawingA.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeB.h>
#include <Xm/PushB.h>
   //---------//
#include <gmsDebugUtil.h>
#include <gmsBrowseMapClass.h>
#include <gmsMapStateMgr.h>
#include <gmsScreenCoordUtils.h>
#include <gmsUtilities.h>


static Widget    g_mapDrawingArea = (Widget) 0;

static GC        theGC;

static Display   *ptrToDisplay;

static Screen    *ptrToScreen;

static XGCValues valuesOfGC;

static bool      g_themeIsShown[Num_Browse_Themes] = { false,
                                                       false,
                                                       false,
                                                       false,
                                                       false,
                                                       false,
                                                       false,
                                                       false,
                                                       true }; // LibRef

static double        g_rotationDeg = 20.0;

static gmsBrowseMapClass *g_theBrowseMap;

static double        g_zoomAmount = 5000.0;

static int           g_width = 600;

static int           g_height = 560;

enum
   {
    WHITE = 0,
    BLACK,
    RED,
    GREEN,
    BLUE,
    Num_Colors
   };

static XColor theColors[Num_Colors];



//---------------------------//
// Local Routines
//---------------------------//

static void createLayout
                  (Widget topLevelShell);

static void defineMenuBar
                  (Widget theParent);

static void defineCtrlPulldownMenu
                  (Widget theParent);

static void defineThemesPulldownMenu
                  (Widget theParent);

static void allocateColors ();

static void drawMaps ();

static void drawLibRef ();

static void drawDrainage ();

static void drawPoliticalAndOceans ();

static void drawPopulatedPlaces ();

static void drawLatLongGrid ();

static void drawImage
               (int                    whichColor,
                gms_2D_ScreenImageType theImage,
                bool                   isChecked = true);

static void drawIndependentPoints
               (int                       whichColor,
                gms_2D_ScreenPolylineType thePoints);


//---------------------------//
// Xt Callback Routines
//---------------------------//

static void exitCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs);

static void exposureCallback
                  (Widget    daWidget,
                   XtPointer client_data,
                   XtPointer cbs);

static void zoomCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs);

static void moveCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs);

static void populatedPlacesCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs);

static void drainageCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs);

static void politicalOceansCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs);

static void libRefCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs);


//---------------------------------------------
// main 
//
// Description:
//    Main program.
//---------------------------------------------
int main
        (int argc,
         char **argv)

{
         Widget       topLevel;
         XtAppContext theAppContext;

   XtSetLanguageProc (NULL, NULL, NULL);

   topLevel = XtVaAppInitialize
                 (&theAppContext,
                  "FlatBrowse",
                  NULL, 0,
                  &argc,
                  argv,
                  NULL,
                  NULL);

   XtVaSetValues
      (topLevel,
       XmNtitle, (char *) "Flat Brose Demo",
       XmNallowShellResize, True,
       NULL);

   createLayout (topLevel);

   XtRealizeWidget (topLevel);

   ptrToDisplay = XtDisplay (g_mapDrawingArea);

   ptrToScreen = XtScreen (g_mapDrawingArea);

   valuesOfGC.foreground = WhitePixelOfScreen (ptrToScreen);

   theGC = XCreateGC
              (ptrToDisplay,
               RootWindowOfScreen (ptrToScreen),
               GCForeground,
               &valuesOfGC);

   allocateColors ();

   XtAppMainLoop (theAppContext);

   return 0;
}


             //---------------------------//
             // Local Routines
             //---------------------------//


//---------------------------------------------
// createLayout 
//
// Description:
//    Function that creates drawingArea for
//    display of map and menu bar, etc
//---------------------------------------------
static void createLayout
                  (Widget topLevelShell)

{
         Widget mainForm;
         Widget theMenuBar;

   mainForm = XmCreateForm
                 (topLevelShell,
                  (char *) "MainForm",
                  0, NULL);

   XtManageChild (mainForm);

   theMenuBar = XmCreateMenuBar
                   (mainForm,
                    (char *) "MenuBar",
                    0, NULL);

   XtManageChild (theMenuBar);

   XtVaSetValues
      (theMenuBar,
       XmNtopAttachment,   XmATTACH_FORM,
       XmNleftAttachment,  XmATTACH_FORM,
       XmNrightAttachment, XmATTACH_FORM,
       NULL);

   g_mapDrawingArea = XmCreateDrawingArea
                         (mainForm,
                          (char *) "MapDrawArea",
                          0, NULL);

   XtManageChild (g_mapDrawingArea);

   XtVaSetValues
      (g_mapDrawingArea,
       XmNwidth,   g_width,
       XmNheight,  g_height,
       XmNtopAttachment,    XmATTACH_WIDGET,
       XmNtopWidget,        theMenuBar,
       XmNleftAttachment,   XmATTACH_FORM,
       XmNrightAttachment,  XmATTACH_FORM,
       XmNbottomAttachment, XmATTACH_FORM,
       XtVaTypedArg,        XmNbackground, XmRString, "black", 5 + 1,
       NULL);

   gmsSetWindowDimensions
              (g_width,
               g_height);

   XtAddCallback
      (g_mapDrawingArea,
       XmNexposeCallback,
       exposureCallback,
       NULL);

   defineMenuBar (theMenuBar);

   gmsSetMapZoomFactor (55000.0);
}


//---------------------------------------------
// defineMenuBar 
//
// Description:
//---------------------------------------------
static void defineMenuBar 
                  (Widget theParent)

{
         Widget   cascadeB;
         Widget   exitButton;
         Widget   thePulldownMenu;
         XmString label;

   //
   // Define the "File" menu button
   //
   label = XmStringCreateLocalized ( (char *) "File");

   cascadeB = XtVaCreateManagedWidget
                   ("FileCascadeButton",
                    xmCascadeButtonWidgetClass,
                    theParent,
                    XmNlabelString, label,
                    NULL);

   XmStringFree (label);

   thePulldownMenu = XmCreatePulldownMenu
                        (theParent,
                         (char *) "FilePulldownMenu",
                         0, NULL);

   // don't XtManageChild

   XtVaSetValues
      (cascadeB,
       XmNsubMenuId, thePulldownMenu,
       NULL);

   label = XmStringCreateLocalized ( (char *) "Exit");

   exitButton = XtVaCreateManagedWidget
                   ("ExitPushButton",
                    xmPushButtonWidgetClass,
                    thePulldownMenu,
                    XmNlabelString, label,
                    NULL);

   XmStringFree (label);

   XtAddCallback
      (exitButton,
       XmNactivateCallback,
       exitCallback,
       NULL);

   //
   // Define the "Ctrl" menu button
   //
   label = XmStringCreateLocalized ( (char *) "Ctrl");

   cascadeB = XtVaCreateManagedWidget
                   ("CtrlCascadeButton",
                    xmCascadeButtonWidgetClass,
                    theParent,
                    XmNlabelString, label,
                    NULL);

   XmStringFree (label);

   thePulldownMenu = XmCreatePulldownMenu
                        (theParent,
                         (char *) "CtrlPulldownMenu",
                         0, NULL);

   // don't XtManageChild

   XtVaSetValues
      (cascadeB,
       XmNsubMenuId, thePulldownMenu,
       NULL);

   defineCtrlPulldownMenu (thePulldownMenu);

   //
   // Define the "Themes" menu button
   //
   label = XmStringCreateLocalized ( (char *) "Themes");

   cascadeB = XtVaCreateManagedWidget
                   ("ThemesCascadeButton",
                    xmCascadeButtonWidgetClass,
                    theParent,
                    XmNlabelString, label,
                    NULL);

   XmStringFree (label);

   thePulldownMenu = XmCreatePulldownMenu
                        (theParent,
                         (char *) "ThemesPulldownMenu",
                         0, NULL);

   // don't XtManageChild

   XtVaSetValues
      (cascadeB,
       XmNsubMenuId, thePulldownMenu,
       NULL);

   defineThemesPulldownMenu (thePulldownMenu);
}


//---------------------------------------------
// defineCtrlPulldownMenu 
//
// Description:
//---------------------------------------------
static void defineCtrlPulldownMenu
               (Widget theParent)

{
         const int      Num_Buttons = 6;
         char           *buttonLabels[Num_Buttons] =
                           { (char *) "Zoom_In",
                             (char *) "Zoom_Out",
                             (char *) "Move_North",
                             (char *) "Move_South",
                             (char *) "Move_East",
                             (char *) "Move_West" };
         XtCallbackProc buttonCallbacks[Num_Buttons] =
                           {zoomCallback,
                            zoomCallback,
                            moveCallback,      
                            moveCallback,      
                            moveCallback,      
                            moveCallback};
         char           instName[32];
         Widget         tempButton;
         XmString       label;
         int            i;

   for (i = 0; i < Num_Buttons; i++)
      {
       label = XmStringCreateLocalized (buttonLabels[i]);

       sprintf(instName, "%s_instance", buttonLabels[i]);

       tempButton = XtVaCreateManagedWidget
                       (instName,
                        xmPushButtonWidgetClass,
                        theParent,
                        XmNlabelString, label,
                        NULL);

       XmStringFree (label);

       XtAddCallback
          (tempButton,
           XmNactivateCallback,
           buttonCallbacks[i],
           (XtPointer) i);
      }
}


//---------------------------------------------
// defineThemesPulldownMenu 
//
// Description:
//---------------------------------------------
static void defineThemesPulldownMenu
               (Widget theParent)

{
         const int      Num_Buttons = 4;
         char           *buttonLabels[Num_Buttons] =
                           { (char *) "LibRef",
                             (char *) "Population",
                             (char *) "Drainage",
                             (char *) "Polit_Ocean" };
         XtCallbackProc buttonCallbacks[Num_Buttons] =
                           {libRefCallback,
                            populatedPlacesCallback,
                            drainageCallback,
                            politicalOceansCallback };
         char           instName[32];
         Widget         tempButton;
         XmString       label;
         int            i;

   for (i = 0; i < Num_Buttons; i++)
      {
       label = XmStringCreateLocalized (buttonLabels[i]);

       sprintf(instName, "%s_instance", buttonLabels[i]);

       tempButton = XtVaCreateManagedWidget
                       (instName,
                        xmPushButtonWidgetClass,
                        theParent,
                        XmNlabelString, label,
                        NULL);

       XmStringFree (label);

       XtAddCallback
          (tempButton,
           XmNactivateCallback,
           buttonCallbacks[i],
           NULL);
      }
}


//---------------------------------------------
// allocateColors 
//
// Description:
//---------------------------------------------
static void allocateColors ()

{
         int      i;
         char     *namesOfColors[Num_Colors] =
                     { (char *) "white",
                       (char *) "black",
                       (char *) "red",
                       (char *) "green",
                       (char *) "blue" };
         XColor   notUsed;
         Colormap theDefaultColorMap;

   theDefaultColorMap = DefaultColormapOfScreen (ptrToScreen);

   for (i = 0; i < Num_Colors; i++)
      {
       XAllocNamedColor
          (ptrToDisplay,
           theDefaultColorMap,
           namesOfColors[i],
           &theColors[i],
           &notUsed);
      }
}


//---------------------------------------------
// clearMapArea 
//
// Description:
//---------------------------------------------
static void clearMapArea ()

{
   valuesOfGC.foreground = BlackPixelOfScreen (ptrToScreen);

   XChangeGC
      (ptrToDisplay,
       theGC,
       GCForeground,
       &valuesOfGC);
 
   XFillRectangle
      (ptrToDisplay,
       XtWindow (g_mapDrawingArea),
       theGC,
       0, 0,
       g_width, g_height);
}


//---------------------------------------------
// drawMaps 
//
// Description:
//---------------------------------------------
static void drawMaps ()

{
   clearMapArea ();

   if (g_theBrowseMap == NULL)
      g_theBrowseMap = new gmsBrowseMapClass (gmsFlat);

   if (g_themeIsShown[gmsBrowse_LibRef])
      drawLibRef ();

   if (g_themeIsShown[gmsBrowse_DN])
      drawDrainage ();

   if (g_themeIsShown[gmsBrowse_PO])
      drawPoliticalAndOceans ();

   if (g_themeIsShown[gmsBrowse_PP])
      drawPopulatedPlaces ();

   drawLatLongGrid ();
}


//---------------------------------------------
// drawLibRef 
//
// Description:
//---------------------------------------------
static void drawLibRef ()

{
         gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_LibRef);

   drawImage
      (WHITE,
       tempImage);
}


//---------------------------------------------
// drawDrainage
//
// Description:
//---------------------------------------------
static void drawDrainage ()

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_DN);

   drawImage
      (BLUE,
       tempImage);
}


//---------------------------------------------
// drawPoliticalAndOceans
//
// Description:
//---------------------------------------------
static void drawPoliticalAndOceans ()

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_PO);

   drawImage
      (RED,
       tempImage);
}


//---------------------------------------------
// drawPopulatedPlaces
//
// Description:
//---------------------------------------------
static void drawPopulatedPlaces ()

{

}


//---------------------------------------------
// drawLatLongGrid
//
// Description:
//---------------------------------------------
static void drawLatLongGrid ()

{
       gms_2D_ScreenImageType tempImage;

   tempImage = g_theBrowseMap->gmsGetLatitudeGrid();

   drawImage
      (GREEN,
       tempImage);

   tempImage = g_theBrowseMap->gmsGetLongitudeGrid();

   drawImage
      (GREEN,
       tempImage);
}


//---------------------------------------------
// drawImage 
//
// Description:
//---------------------------------------------
static void drawImage
               (int                    whichColor,
                gms_2D_ScreenImageType theImage,
                bool                   isChecked)

{
         int i, j;

   valuesOfGC.foreground = theColors[whichColor].pixel;
   // WhitePixelOfScreen (ptrToScreen);

   XChangeGC
      (ptrToDisplay,
       theGC,
       GCForeground,
       &valuesOfGC);

   for (i = 0; i < theImage.numLines; i++)
      for (j = 0; j < (theImage.imageLines[i].numPoints - 1); j++)
         {
          if ( ! isChecked )
             {
              XDrawLine
                 (ptrToDisplay,
                  XtWindow (g_mapDrawingArea),
                  theGC,
                  theImage.imageLines[i].points[j].x,
                  theImage.imageLines[i].points[j].y,
                  theImage.imageLines[i].points[j + 1].x,
                  theImage.imageLines[i].points[j + 1].y);
             }

          else if (gmsIsVisibleLine
                         (theImage.imageLines[i].points[j],
                          theImage.imageLines[i].points[j + 1]) )
             {
              XDrawLine
                 (ptrToDisplay,
                  XtWindow (g_mapDrawingArea),
                  theGC,
                  theImage.imageLines[i].points[j].x,
                  theImage.imageLines[i].points[j].y,
                  theImage.imageLines[i].points[j + 1].x,
                  theImage.imageLines[i].points[j + 1].y);
             }
         }
}


//---------------------------------------------
// drawIndependentPoints 
//
// Description:
//---------------------------------------------
static void drawIndependentPoints
               (int                       whichColor,
                gms_2D_ScreenPolylineType thePoints)

{

}


             //---------------------------//
             // Xt Callback Routines
             //---------------------------//


//---------------------------------------------
// exitCallback
//
// Description:
//    Callback to handle selection of the
//    pushbutton
//---------------------------------------------
static void exitCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs)

{
   exit (0);
}


//---------------------------------------------
// exposureCallback 
//
// Description:
//---------------------------------------------
static void exposureCallback
                  (Widget    daWidget,
                   XtPointer client_data,
                   XtPointer cbs)
{
         XWindowAttributes theWindowAttributes;

   XGetWindowAttributes
      (ptrToDisplay,
       XtWindow (daWidget),
       &theWindowAttributes);

   g_width  = theWindowAttributes.width;
   g_height = theWindowAttributes.height;

   gmsSetWindowDimensions
         (g_width,
          g_height);

   drawMaps ();
}


//---------------------------------------------
// zoomCallback
//
// Description:
//---------------------------------------------
static void zoomCallback 
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs)

{
         const int index = (int) client_data;

   if (index == 0)
      gmsZoomIn (g_zoomAmount);
   else
      gmsZoomOut (g_zoomAmount);

   drawMaps ();
}


//---------------------------------------------
// moveCallback 
//
// Description:
//---------------------------------------------
static void moveCallback 
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs)

{
         const int index = (int) client_data;

   if (index == 2)
      gmsMoveFlatMap (gmsMoveNorth);
   else if (index == 3)
      gmsMoveFlatMap (gmsMoveSouth);
   else if (index == 4)
      gmsMoveFlatMap (gmsMoveEast);
   else
      gmsMoveFlatMap (gmsMoveWest);

   drawMaps ();
}


//---------------------------------------------
// libRefCallback 
//
// Description:
//---------------------------------------------
static void libRefCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs)

{
   g_themeIsShown[(int) gmsBrowse_LibRef] =
         ! g_themeIsShown[(int) gmsBrowse_LibRef];

   drawMaps ();
}


//---------------------------------------------
// populatedPlacesCallback 
//
// Description:
//---------------------------------------------
static void populatedPlacesCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs)

{
   g_themeIsShown[(int) gmsBrowse_PP] =
         ! g_themeIsShown[(int) gmsBrowse_PP];

   drawMaps ();
}


//---------------------------------------------
// drainageCallback 
//
// Description:
//---------------------------------------------
static void drainageCallback
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs)

{
   g_themeIsShown[(int) gmsBrowse_DN] =
         ! g_themeIsShown[(int) gmsBrowse_DN];

   drawMaps ();
}


//---------------------------------------------
// politicalOceansCallback
//
// Description:
//---------------------------------------------
static void politicalOceansCallback 
                  (Widget    pbWidget,
                   XtPointer client_data,
                   XtPointer cbs)

{
   g_themeIsShown[(int) gmsBrowse_PO] =
         ! g_themeIsShown[(int) gmsBrowse_PO];

   drawMaps ();
}

