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

#include <stdio.h>
#include <string.h>
#include <gmsNamesOfFilesClass.h>
#include <gmsDebugUtil.h>


//---------------------------------
// Define the real map 'object'
//---------------------------------


//---------------------------------
// Declare some local variables/constants
//---------------------------------

static gmsThemeFilesListType  *g_filesList[Num_Themes];

static gmsBrowseFilesListType *g_browseFilesList[Num_Browse_Themes];

//---------------------------------
// Declare Static Member Functions
//---------------------------------




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
gmsNamesOfFilesClass::gmsNamesOfFilesClass
                         (gmsThematicType whichTheme)

{
   gmsNamesOfFilesClass::theTheme = whichTheme;

   if (g_filesList[whichTheme] == NULL)
      {
       g_filesList[whichTheme] = gmsGetListOfThemeFiles (whichTheme);
      }
}


//-----------------------------------------------
// gmsNamesOfFilesClass
//
// Description:
//    Constructor.  Creates an object that
//    contains the list of names of the files
//    ONLY in the specified tile and theme.
//-----------------------------------------------
gmsNamesOfFilesClass::gmsNamesOfFilesClass
                         (gmsThematicType whichTheme,
                          char            longTileId,
                          char            latTileId)

{
   gmsNamesOfFilesClass::theTheme = whichTheme;

   if (g_filesList[whichTheme] == NULL)
      {
       g_filesList[whichTheme] = gmsGetListOfTileFiles
                                    (whichTheme,
                                     longTileId,
                                     latTileId);
      }
}


//-----------------------------------------------
// gmsNamesOfFilesClass
//
// Description:
//    Constructor.  Creates an object that
//    contains the list of names of all the files
//    from the BROWSE library.
//-----------------------------------------------
gmsNamesOfFilesClass::gmsNamesOfFilesClass
                         (gmsBrowseThematicType whichBrowseTheme)

{
   gmsNamesOfFilesClass::theBrowseTheme = whichBrowseTheme;

   if (g_browseFilesList[whichBrowseTheme] == NULL)
      {
       g_browseFilesList[whichBrowseTheme] =
          gmsGetListOfBrowseFiles (whichBrowseTheme);
      }
}


//-----------------------------------------------
// ~gmsNamesOfFilesClass
//
// Description:
//    Destructor.
//
//    Actually, does nothing.  Any file lists
//    are left "hanging around" since another
//    caller may need the data later on.  This
//    will prevent another delay due to the
//    re-construction of the list.
//-----------------------------------------------
gmsNamesOfFilesClass::~gmsNamesOfFilesClass ()

{

}


//-----------------------------------------------
// gmsGetFilesList
//
// Description:
//-----------------------------------------------
gmsThemeFilesListType gmsNamesOfFilesClass::gmsGetFilesList ()

{
         gmsThematicType index;

   index = gmsNamesOfFilesClass::theTheme;

   return *g_filesList[index];
}


//-----------------------------------------------
// gmsFileIsInTile
//
// Description:
//    Utility to determine if the specified file
//    is from the given GEOREF tile.  For example,
//    the file: "/.../PO/N/G/23/EDG" is data for
//    the GEOREF tile "NG".
//-----------------------------------------------
bool gmsNamesOfFilesClass::gmsFileIsInTile
                              (const char *fileName,
                               char       latTileId,
                               char       longTileId)

{
         int i, j;
         int length;

   if (fileName == NULL)
      return false;

   length = strlen (fileName);

   for (i = 0; i < length; i++)
      {
       if (fileName[i] == longTileId)
          {
           j = i + 2;

           if (j < length)
              if (fileName[j] == latTileId)
                 return true;
          }
      }

   return false;
}


//-----------------------------------------------
// gmsGetBrowseFilesList
//
// Description:
//-----------------------------------------------
gmsBrowseFilesListType gmsNamesOfFilesClass::gmsGetBrowseFilesList ()

{
         gmsBrowseThematicType index;

   index = gmsNamesOfFilesClass::theBrowseTheme;

   return *g_browseFilesList[index];
}


        //---------------------------
        //  Static Member Functions
        //---------------------------
