#!/usr/bin/wish
# ************************************************************************
# FILE: dcw_browse.tcl
#
# DESCRIPTION:
#    Attempt to implement the Digital Chart of the World (DCW) browse
#    tool.
#*************************************************************************

package require Tk

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

set g_zoomAmount  5000.0

set g_width  600

set g_height 560

set theColors "whie black red green blue"


# ***********************************************
# PROCEDURE NAME: some_proc 
#
# DESCRIPTION:
#    A procedure in Tcl
# ***********************************************
proc some_proc { label } {

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
proc createLayout { menu_bar } {

   . configure -menu $menu_bar

   #
   # File pulldown
   #
   menu $menu_bar.file -tearoff 0

   $menu_bar add cascade -label "File" -menu $menu_bar.file

   $menu_bar.file add command -label "New"   -command "helpCallback"
   $menu_bar.file add command -label "Hello" -command "some_proc .usr_info_2"
   $menu_bar.file add separator
   $menu_bar.file add command -label "Exit" -command "exitCallback"

   #
   # Ctrl pulldown
   #
   menu $menu_bar.ctrl -tearoff 0

   $menu_bar add cascade -label "Ctrl" -menu $menu_bar.ctrl

   $menu_bar.ctrl add command -label "Zoom In"    -command "helpCallback"
   $menu_bar.ctrl add command -label "Zoom Out"   -command "helpCallback"
   $menu_bar.ctrl add command -label "Move North" -command "exitCallback"
   $menu_bar.ctrl add command -label "Move South" -command "helpCallback"
   $menu_bar.ctrl add command -label "Move East"  -command "helpCallback"
   $menu_bar.ctrl add command -label "Move West"  -command "helpCallback"

   #
   # Themes pulldown
   #
   menu $menu_bar.themes -tearoff 0

   $menu_bar add cascade -label "Themes" -menu $menu_bar.themes

   $menu_bar.themes add command -label "LibRef"      -command "helpCallback"
   $menu_bar.themes add command -label "Population"  -command "helpCallback"
   $menu_bar.themes add command -label "Drainage"    -command "helpCallback"
   $menu_bar.themes add command -label "Polit_Ocean" -command "helpCallback"

   #
   # Help pulldown
   #
   menu $menu_bar.help -tearoff 0

   $menu_bar add cascade -label "Help" -menu $menu_bar.help

   $menu_bar.help add command -label "About" -command "helpCallback"

#  XtAddCallback
#     (g_mapDrawingArea,
#      XmNexposeCallback,
#      exposureCallback,
#      NULL);

   global g_zoomAmount

   set answer [Tcl2GMS_init_map_settings 14000.0 $g_zoomAmount]
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

   clearMapArea

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
#  helpCallback
# 
#  Description:
#     Help callback.
# ---------------------------------------------
proc helpCallback { } {

   tk_messageBox -message "DCW No Help"

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
proc zoomCallback { } {

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

load Tcl2GMS/libtcltogms[info sharedlibextension]

label .usr_info_1 -text "info here"
label .usr_info_2 -text "$count"
label .position   -text "<lat> <long>"

canvas .mapDrawingArea -width $g_width -height $g_height

menu .menu_bar

createLayout .menu_bar

set orange_oval [.mapDrawingArea create oval 0 0 50 50 -fill orange]
set blue_line [.mapDrawingArea create line 50 50 100 100 -fill blue -width 2]

grid .mapDrawingArea -row 0 -column 0
grid .usr_info_1     -row 1 -column 0
grid .usr_info_2     -row 1 -column 1
grid .position       -row 1 -column 2

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
