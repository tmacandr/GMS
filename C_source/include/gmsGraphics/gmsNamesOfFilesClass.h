//-----------------------------------------------------------------------------
// File : gmsNamesOfFilesClass.h
// Date : 26-Jan-00 : initial definition
//        26-Feb-00 : implement tile/theme util to improve performance
//
// Description:
//    This class defines an object that contains the list of all the files
//    of a theme.  This component provides a thin abstraction on the
//    component 'gmsFileNameUtilities'.  The reason for this is that the
//    processing time required to build a list is significant.  To prevent
//    an application from unnecessarily creating multiple lists for the
//    same theme, this component implements a "singleton". 
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//-----------------------------------------------------------------------------

#ifndef GMS_FILES_NAMES_CLASS

   #define GMS_FILES_NAMES_CLASS

   #include <gmsTypesAndConstants.h>
   #include <gmsFileNameUtilities.h>

   class gmsNamesOfFilesClass
      {
       public:

          //-----------------------------------------------
          // gmsNamesOfFilesClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    contains the list of names of ALL the files
          //    in the specified theme.
          //
          //    WARNING:
          //       This function is very time consuming.
          //       This routine can take about 60 seconds
          //       to complete.  For a tile-only list of
          //       files, use the second constructor.
          //-----------------------------------------------
          gmsNamesOfFilesClass
             (gmsThematicType whichTheme);


          //-----------------------------------------------
          // gmsNamesOfFilesClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    contains the list of names of the files
          //    ONLY in the specified tile and theme.
          //-----------------------------------------------
          gmsNamesOfFilesClass
             (gmsThematicType whichTheme,
              char            longTileId,
              char            latTileId);


          //-----------------------------------------------
          // gmsNamesOfFilesClass
          //
          // Description:
          //    Constructor.  Creates an object that
          //    contains the list of names of all the files
          //    from the BROWSE library.
          //-----------------------------------------------
          gmsNamesOfFilesClass
             (gmsBrowseThematicType whichBrowseTheme);


          //-----------------------------------------------
          // gmsNamesOfFilesClass
          //
          // Description:
          //    Destructor.  Deletes the file-names object.
          //-----------------------------------------------
          ~gmsNamesOfFilesClass();


          //-----------------------------------------------
          // gmsGetFilesList
          //
          // Description:
          //-----------------------------------------------
          gmsThemeFilesListType gmsGetFilesList ();


          //-----------------------------------------------
          // gmsFileIsInTile
          //
          // Description:
          //    Utility to determine if the specified file
          //    is from the given GEOREF tile.  For example,
          //    the file: "/.../PO/N/G/23/EDG" is data for
          //    the GEOREF tile "NG".
          //-----------------------------------------------
          bool gmsFileIsInTile
                  (const char *fileName,
                   char       latTileId,
                   char       longTileId);


          //-----------------------------------------------
          // gmsGetBrowseFilesList
          //
          // Description:
          //-----------------------------------------------
          gmsBrowseFilesListType gmsGetBrowseFilesList ();


       private:

          gmsThematicType       theTheme;

          gmsBrowseThematicType theBrowseTheme;
      };

#endif
