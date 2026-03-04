#!/usr/bin/wish
# ************************************************************************
# FILE: dcw_browse.tcl
#
# DESCRIPTION:
#    Attempt to implement the Digital Chart of the World (DCW) browse
#    tool.
#*************************************************************************

set count 0

set g_themeIsShown(CO)     "false"
set g_themeIsShown(DV)     "false"
set g_themeIsShown(DN)     "false"
set g_themeIsShown(GR)     "false"
set g_themeIsShown(DA)     "false"
set g_themeIsShown(IN)     "false"
set g_themeIsShown(PO)     "false"
set g_themeIsShown(PP)     "false"
set g_themeIsShown(LibRef) "true"

set g_rotationDeg  20.0

set gmsBrowseMapClass g_theBrowseMap

set g_zoomAmount  5000.0

set g_width  600

set g_height 560

set theColors "whie black red green blue"


# ***********************************************
# PROCEDURE NAME: some_procedure 
#
# DESCRIPTION:
#    A procedure in Tcl
# ***********************************************
proc some_procedure { label } {

   global count

   $label configure -text "count...: $count"

   set count [expr $count + 1]
}

# ---------------------------------------------
#  createLayout 
# 
#  Description:
#     Function that creates drawingArea for
#     display of map and menu bar, etc
# ---------------------------------------------
proc createLayout { Widget topLevelShell } {

#        Widget mainForm;
#        Widget theMenuBar;

#  mainForm = XmCreateForm
#                (topLevelShell,
#                 (char *) "MainForm",
#                 0, NULL);

#  XtManageChild (mainForm);

#  theMenuBar = XmCreateMenuBar
#                  (mainForm,
#                   (char *) "MenuBar",
#                   0, NULL);

#  XtManageChild (theMenuBar);

#  XtVaSetValues
#     (theMenuBar,
#      XmNtopAttachment,   XmATTACH_FORM,
#      XmNleftAttachment,  XmATTACH_FORM,
#      XmNrightAttachment, XmATTACH_FORM,
#      NULL);

#  g_mapDrawingArea = XmCreateDrawingArea
#                        (mainForm,
#                         (char *) "MapDrawArea",
#                         0, NULL);

#  XtManageChild (g_mapDrawingArea);

#  XtVaSetValues
#     (g_mapDrawingArea,
#      XmNwidth,   g_width,
#      XmNheight,  g_height,
#      XmNtopAttachment,    XmATTACH_WIDGET,
#      XmNtopWidget,        theMenuBar,
#      XmNleftAttachment,   XmATTACH_FORM,
#      XmNrightAttachment,  XmATTACH_FORM,
#      XmNbottomAttachment, XmATTACH_FORM,
#      XtVaTypedArg,        XmNbackground, XmRString, "black", 5 + 1,
#      NULL);

#  gmsSetWindowDimensions
#             (g_width,
#              g_height);

#  XtAddCallback
#     (g_mapDrawingArea,
#      XmNexposeCallback,
#      exposureCallback,
#      NULL);

   defineMenuBar (theMenuBar);

#  gmsSetMapZoomFactor (14000.0);
}


# ---------------------------------------------
#  defineMenuBar 
# 
#  Description:
# ---------------------------------------------
proc defineMenuBar { Widget theParent } {

#        Widget   cascadeB;
#        Widget   exitButton;
#        Widget   thePulldownMenu;
#        XmString label;

   #  
   #  Define the "File" menu button
   #  
#  label = XmStringCreateLocalized ( (char *) "File");

#  cascadeB = XtVaCreateManagedWidget
#                  ("FileCascadeButton",
#                   xmCascadeButtonWidgetClass,
#                   theParent,
#                   XmNlabelString, label,
#                   NULL);

#  XmStringFree (label);

#  thePulldownMenu = XmCreatePulldownMenu
#                       (theParent,
#                        (char *) "FilePulldownMenu",
#                        0, NULL);

   #  don't XtManageChild

#  XtVaSetValues
#     (cascadeB,
#      XmNsubMenuId, thePulldownMenu,
#      NULL);

#  label = XmStringCreateLocalized ( (char *) "Exit");

#  exitButton = XtVaCreateManagedWidget
#                  ("ExitPushButton",
#                   xmPushButtonWidgetClass,
#                   thePulldownMenu,
#                   XmNlabelString, label,
#                   NULL);

#  XmStringFree (label);

#  XtAddCallback
#     (exitButton,
#      XmNactivateCallback,
#      exitCallback,
#      NULL);

   #  
   #  Define the "Ctrl" menu button
   #  
#  label = XmStringCreateLocalized ( (char *) "Ctrl");

#  cascadeB = XtVaCreateManagedWidget
#                  ("CtrlCascadeButton",
#                   xmCascadeButtonWidgetClass,
#                   theParent,
#                   XmNlabelString, label,
#                   NULL);

#  XmStringFree (label);

#  thePulldownMenu = XmCreatePulldownMenu
#                       (theParent,
#                        (char *) "CtrlPulldownMenu",
#                        0, NULL);

   #  don't XtManageChild

#  XtVaSetValues
#     (cascadeB,
#      XmNsubMenuId, thePulldownMenu,
#      NULL);

#  defineCtrlPulldownMenu (thePulldownMenu);

   #  
   #  Define the "Themes" menu button
   #  
#  label = XmStringCreateLocalized ( (char *) "Themes");

#  cascadeB = XtVaCreateManagedWidget
#                  ("ThemesCascadeButton",
#                   xmCascadeButtonWidgetClass,
#                   theParent,
#                   XmNlabelString, label,
#                   NULL);

#  XmStringFree (label);

#  thePulldownMenu = XmCreatePulldownMenu
#                       (theParent,
#                        (char *) "ThemesPulldownMenu",
#                        0, NULL);

   #  don't XtManageChild

#  XtVaSetValues
#     (cascadeB,
#      XmNsubMenuId, thePulldownMenu,
#      NULL);

#  defineThemesPulldownMenu (thePulldownMenu);
}


# ---------------------------------------------
#  defineCtrlPulldownMenu 
# 
#  Description:
# ---------------------------------------------
proc defineCtrlPulldownMenu { Widget theParent } {

#        const int      Num_Buttons = 6;
#        char           *buttonLabels[Num_Buttons] =
#                          { (char *) "Zoom_In",
#                            (char *) "Zoom_Out",
#                            (char *) "Move_North",
#                            (char *) "Move_South",
#                            (char *) "Move_East",
#                            (char *) "Move_West" };
#        XtCallbackProc buttonCallbacks[Num_Buttons] =
#                          {zoomCallback,
#                           zoomCallback,
#                           moveCallback,      
#                           moveCallback,      
#                           moveCallback,      
#                           moveCallback};
#        char           instName[32];
#        Widget         tempButton;
#        XmString       label;
#        int            i;

#  for (i = 0; i < Num_Buttons; i++)
#  {
#      label = XmStringCreateLocalized (buttonLabels[i]);

#      sprintf(instName, "%s_instance", buttonLabels[i]);

#      tempButton = XtVaCreateManagedWidget
#                      (instName,
#                       xmPushButtonWidgetClass,
#                       theParent,
#                       XmNlabelString, label,
#                       NULL);

#      XmStringFree (label);

#      XtAddCallback
#         (tempButton,
#          XmNactivateCallback,
#          buttonCallbacks[i],
#          (XtPointer) i);
#  }
}


# ---------------------------------------------
#  defineThemesPulldownMenu 
# 
#  Description:
# ---------------------------------------------
proc defineThemesPulldownMenu { Widget theParent } {

#        const int      Num_Buttons = 4;
#        char           *buttonLabels[Num_Buttons] =
#                          { (char *) "LibRef",
#                            (char *) "Population",
#                            (char *) "Drainage",
#                            (char *) "Polit_Ocean" };
#        XtCallbackProc buttonCallbacks[Num_Buttons] =
#                          {libRefCallback,
#                           populatedPlacesCallback,
#                           drainageCallback,
#                           politicalOceansCallback };
#        char           instName[32];
#        Widget         tempButton;
#        XmString       label;
#        int            i;

#  for (i = 0; i < Num_Buttons; i++)
#  {
#      label = XmStringCreateLocalized (buttonLabels[i]);

#      sprintf(instName, "%s_instance", buttonLabels[i]);

#      tempButton = XtVaCreateManagedWidget
#                      (instName,
#                       xmPushButtonWidgetClass,
#                       theParent,
#                       XmNlabelString, label,
#                       NULL);

#      XmStringFree (label);

#      XtAddCallback
#         (tempButton,
#          XmNactivateCallback,
#          buttonCallbacks[i],
#          NULL);
#  }
}


# ---------------------------------------------
#  allocateColors 
# 
#  Description:
# ---------------------------------------------
proc allocateColors { } {

#        int      i;
#        char     *namesOfColors[Num_Colors] =
#                    { (char *) "white",
#                      (char *) "black",
#                      (char *) "red",
#                      (char *) "green",
#                      (char *) "blue" };
#        XColor   notUsed;
#        Colormap theDefaultColorMap;

#  theDefaultColorMap = DefaultColormapOfScreen (ptrToScreen);

#  for (i = 0; i < Num_Colors; i++)
#     {
#      XAllocNamedColor
#         (ptrToDisplay,
#          theDefaultColorMap,
#          namesOfColors[i],
#          &theColors[i],
#          &notUsed);
#     }
}


# ---------------------------------------------
#  clearMapArea 
# 
#  Description:
# ---------------------------------------------
proc clearMapArea { } {

#  valuesOfGC.foreground = BlackPixelOfScreen (ptrToScreen);

#  XChangeGC
#     (ptrToDisplay,
#      theGC,
#      GCForeground,
#      &valuesOfGC);
 
#  XFillRectangle
#     (ptrToDisplay,
#      XtWindow (g_mapDrawingArea),
#      theGC,
#      0, 0,
#      g_width, g_height);
}


# ---------------------------------------------
#  drawMaps 
# 
#  Description:
# ---------------------------------------------
proc drawMaps { } {

#  clearMapArea ();

#  if (g_theBrowseMap == NULL)
#     g_theBrowseMap = new gmsBrowseMapClass (gmsEllipsoid);

#  if (g_themeIsShown[gmsBrowse_LibRef])
#     drawLibRef ();

#  if (g_themeIsShown[gmsBrowse_DN])
#     drawDrainage ();

#  if (g_themeIsShown[gmsBrowse_PO])
#     drawPoliticalAndOceans ();

#  if (g_themeIsShown[gmsBrowse_PP])
#     drawPopulatedPlaces ();

#  drawLatLongGrid ();
}


# ---------------------------------------------
#  drawLibRef 
# 
#  Description:
# ---------------------------------------------
proc drawLibRef { } {

#        gms_2D_ScreenImageType tempImage;

#  tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_LibRef);

#  drawImage
#     (WHITE,
#      tempImage);
}


# ---------------------------------------------
#  drawDrainage
# 
#  Description:
# ---------------------------------------------
proc drawDrainage { }  {

#      gms_2D_ScreenImageType tempImage;

#  tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_DN);

#  drawImage
#     (BLUE,
#      tempImage);
}


# ---------------------------------------------
#  drawPoliticalAndOceans
# 
#  Description:
# ---------------------------------------------
proc drawPoliticalAndOceans { } {

#      gms_2D_ScreenImageType tempImage;

#  tempImage = g_theBrowseMap->gmsGetBrowseMapImage (gmsBrowse_PO);

#  drawImage
#     (RED,
#      tempImage);
}


# ---------------------------------------------
#  drawPopulatedPlaces
# 
#  Description:
# ---------------------------------------------
proc drawPopulatedPlaces { } {

}


# ---------------------------------------------
#  drawLatLongGrid
# 
#  Description:
# ---------------------------------------------
proc drawLatLongGrid { } {

#      gms_2D_ScreenImageType tempImage;

#  tempImage = g_theBrowseMap->gmsGetLatitudeGrid();

#  drawImage
#     (GREEN,
#      tempImage);

#  tempImage = g_theBrowseMap->gmsGetLongitudeGrid();

#  drawImage
#     (GREEN,
#      tempImage);
}


# ---------------------------------------------
#  drawImage 
# 
#  Description:
# ---------------------------------------------
proc drawImage { whichColor gms_2D_ScreenImageType theImage isChecked } {

#        int i, j;

#  valuesOfGC.foreground = theColors[whichColor].pixel;
   #  WhitePixelOfScreen (ptrToScreen);

#  XChangeGC
#     (ptrToDisplay,
#      theGC,
#      GCForeground,
#      &valuesOfGC);

#  for (i = 0; i < theImage.numLines; i++)
#  {
#     for (j = 0; j < (theImage.imageLines[i].numPoints - 1); j++)
#     {
#         if ( ! isChecked )
#         {
#             XDrawLine
#                (ptrToDisplay,
#                 XtWindow (g_mapDrawingArea),
#                 theGC,
#                 theImage.imageLines[i].points[j].x,
#                 theImage.imageLines[i].points[j].y,
#                 theImage.imageLines[i].points[j + 1].x,
#                 theImage.imageLines[i].points[j + 1].y);
#         }
#         else if (gmsIsVisibleLine
#                        (theImage.imageLines[i].points[j],
#                         theImage.imageLines[i].points[j + 1]) )
#         {
#             XDrawLine
#                (ptrToDisplay,
#                 XtWindow (g_mapDrawingArea),
#                 theGC,
#                 theImage.imageLines[i].points[j].x,
#                 theImage.imageLines[i].points[j].y,
#                 theImage.imageLines[i].points[j + 1].x,
#                 theImage.imageLines[i].points[j + 1].y);
#         }
#     }
#  }
}


# ---------------------------------------------
#  drawIndependentPoints 
# 
#  Description:
# ---------------------------------------------
proc drawIndependentPoints { whichColor gms_2D_ScreenPolylineType thePoints } {

}


             # --------------------------
             #  Callback Routines
             # --------------------------


# ---------------------------------------------
#  exitCallback
# 
#  Description:
#     Callback to handle selection of the
#     pushbutton
# ---------------------------------------------
proc exitCallback { } {

   exit
}


# ---------------------------------------------
#  exposureCallback 
# 
#  Description:
# ---------------------------------------------
proc exposureCallback { Widget    daWidget, XtPointer client_data, XtPointer cbs} {
#         XWindowAttributes theWindowAttributes;

#   XGetWindowAttributes
#      (ptrToDisplay,
#       XtWindow (daWidget),
#       &theWindowAttributes);

#   g_width  = theWindowAttributes.width;
#   g_height = theWindowAttributes.height;

#   gmsSetWindowDimensions
#         (g_width,
#          g_height);

#   drawMaps ();
}


# ---------------------------------------------
#  zoomCallback
# 
#  Description:
# ---------------------------------------------
proc zoomCallback { Widget    pbWidget, XtPointer client_data, XtPointer cbs } {

#         const int index = (int) client_data;

#   if (index == 0)
#      gmsZoomIn (g_zoomAmount);
#   else
#      gmsZoomOut (g_zoomAmount);

#   drawMaps ();
}


# ---------------------------------------------
#  moveCallback 
# 
#  Description:
# ---------------------------------------------
proc moveCallback { Widget    pbWidget, XtPointer client_data, XtPointer cbs } {

#         const int index = (int) client_data;

#   if (index == 2)
#      gmsSet_X_Rotation (g_rotationDeg);

#   else if (index == 3)
#      gmsSet_X_Rotation (-g_rotationDeg);

#   else if (index == 4)
#      gmsSet_Y_Rotation (-g_rotationDeg);

#   else
#      gmsSet_Y_Rotation (g_rotationDeg);

#   drawMaps ();
}


# ---------------------------------------------
#  libRefCallback 
# 
#  Description:
# ---------------------------------------------
proc libRefCallback { Widget    pbWidget, XtPointer client_data, XtPointer cbs} {
#   g_themeIsShown[(int) gmsBrowse_LibRef] =
#         ! g_themeIsShown[(int) gmsBrowse_LibRef];

#   drawMaps ();
}


# ---------------------------------------------
#  populatedPlacesCallback 
# 
#  Description:
# ---------------------------------------------
proc populatedPlacesCallback { Widget pbWidget, XtPointer client_data, XtPointer cbs } {

#  g_themeIsShown[(int) gmsBrowse_PP] =
#        ! g_themeIsShown[(int) gmsBrowse_PP];

#  drawMaps ();
}


# ---------------------------------------------
#  drainageCallback 
# 
#  Description:
# ---------------------------------------------
proc drainageCallback { Widget pbWidget, XtPointer client_data, XtPointer cbs } {
#  g_themeIsShown[(int) gmsBrowse_DN] =
#        ! g_themeIsShown[(int) gmsBrowse_DN];

#  drawMaps ();
}


# ---------------------------------------------
#  politicalOceansCallback
# 
#  Description:
# ---------------------------------------------
proc politicalOceansCallback { Widget pbWidget, XtPointer client_data, XtPointer cbs } {

#  g_themeIsShown[(int) gmsBrowse_PO] =
#        ! g_themeIsShown[(int) gmsBrowse_PO];

#  drawMaps ();
}



# ***********************************************
# PROCEDURE NAME: main ... sort of ...
#
# DESCRIPTION:
#    Executing commands begins here ...
# ***********************************************
puts "Begin - DCW Flat Browse Demo"

label .msg -text "  "

button .hello_button -text "DCW" -command "some_procedure .msg"

button .exit_button -text "Exit" -command "exitCallback"

canvas .mapDrawingArea -width 550 -height 600

set orange_oval [.mapDrawingArea create oval 0 0 50 50 -fill orange]
set blue_line [.mapDrawingArea create line 50 50 100 100 -fill blue -width 2]

grid .mapDrawingArea -row 1 -column 0
grid .msg            -row 2 -column 0
grid .hello_button   -row 0 -column 0
grid .exit_button    -row 0 -column 1


#  createLayout (topLevel);
#
#  XtRealizeWidget (topLevel);

#  ptrToDisplay = XtDisplay (g_mapDrawingArea);

#  ptrToScreen = XtScreen (g_mapDrawingArea);

#  valuesOfGC.foreground = WhitePixelOfScreen (ptrToScreen);

#  theGC = XCreateGC
#             (ptrToDisplay,
#              RootWindowOfScreen (ptrToScreen),
#              GCForeground,
#              &valuesOfGC);

#  allocateColors ();

#  XtAppMainLoop (theAppContext);
   
puts "End - DCW Flat Broswe"
