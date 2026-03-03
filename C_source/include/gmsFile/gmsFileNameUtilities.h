/*---------------------------------------------------------------------------*/
/* File: gmsFileNameUtilities.h
/* Date: 11-Jan-00 : initial definition
/*       26-Feb-00 : implement tile/theme util to improve performance
/*
/* Description:
/*    Defines utilites to get the names (and number) of the files
/*    of the DCW database.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/

#ifndef GMS_FILE_NAME_UTILS

   #include <gmsTypesAndConstants.h>

   #define GMS_FILE_NAME_UTILS

   typedef struct
      {
       int  numFiles;
       char **filePath;
      } gmsFilePathType;

   typedef struct
      {
       gmsThematicType whichTheme;
       gmsFilePathType edgTbls;
       gmsFilePathType facTbls;
       gmsFilePathType txtTbls;
       gmsFilePathType endTbls;
       gmsFilePathType ebrTbls;
       gmsFilePathType fbrTbls;
       gmsFilePathType rngTbls;
       gmsFilePathType esiTbls;
       gmsFilePathType fsiTbls;
       gmsFilePathType nsiTbls;
       char            *areaFeatureTbl;
       char            *lineFeatureTbl;
       char            *pointFeatureTbl;
       char            *textFeatureTbl;
       char            *areaDocTbl;
       char            *lineDocTbl;
       char            *pointDocTbl;
       char            *fcs;
       char            *charVDT;
       char            *intVDT;
       void            *unused_1;
       void            *unused_2;
      } gmsThemeFilesListType;


   typedef struct
      {
       gmsBrowseThematicType whichTheme;
       char                  *edgTbl;
       char                  *facTbl;
       char                  *txtTbl;
       char                  *endTbl;
       char                  *ebrTbl;
       char                  *fbrTbl;
       char                  *rngTbl;
       char                  *esiTbl;
       char                  *fsiTbl;
       char                  *nsiTbl;
       char                  *areaFeatureTbl;
       char                  *lineFeatureTbl;
       char                  *pointFeatureTbl;
       char                  *textFeatureTbl;
       char                  *areaDocTbl;
       char                  *lineDocTbl;
       char                  *pointDocTbl;
       char                  *fcs;
       char                  *charVDT;
       char                  *intVDT;
      } gmsBrowseFilesListType;


   /*-----------------------------------------------------*/
   /* gmsGetListOfThemeFiles
   /*
   /* Description:
   /*    This utility builds a data structure that lists
   /*    each of the files (with full path) in a specified
   /*    theme.
   /*
   /*    The caller must deallocate the list using the
   /*    utility 'gmsFreeListOfThemeFiles' (see below).
   /*
   /*    WARNING:
   /*    --------
   /*       This is a very expensive (time) routine.  The
   /*       data structure is for an entire theme and since
   /*       it has to be determined which files exist then
   /*       build the corresponding data structure, the time
   /*       required is significant.  (AMD K6/300 MHz,
   /*       Windows 98 requires about 60 seconds per theme).
   /*
   /*       If only the files of a specific tile within a
   /*       theme are needed, use the utility
   /*       'gmsGetListOfTileFiles' below.
   /*-----------------------------------------------------*/
   gmsThemeFilesListType *gmsGetListOfThemeFiles
                             (gmsThematicType whichTheme);


   /*-----------------------------------------------------*/
   /* gmsGetListOfTileFiles
   /*
   /* Description:
   /*    This utility builds a data structure that lists
   /*    ONLY the files (with full path) for the specified
   /*    tile and theme.
   /*
   /*    The caller must deallocate the list using the
   /*    utility 'gmsFreeListOfThemeFiles' (see below).
   /*-----------------------------------------------------*/
   gmsThemeFilesListType *gmsGetListOfTileFiles
                             (gmsThematicType whichTheme,
                              char            longTileId,
                              char            latTileId);


   /*-----------------------------------------------------*/
   /* gmsFreeListOfThemeFiles
   /*
   /* Description:
   /*    This utility deallocates the list of theme files
   /*    that was built using 'gmsGetListOfThemeFiles' (see
   /*    above).
   /*-----------------------------------------------------*/
   void gmsFreeListOfThemeFiles
           (gmsThemeFilesListType **theList);


   /*-----------------------------------------------------*/
   /* gmsPrintListOfThemeFiles
   /*
   /* Description:
   /*    This utility prints (to stdout) the list of theme
   /*    files from 'theList' object.
   /*-----------------------------------------------------*/
   void gmsPrintListOfThemeFiles
           (gmsThemeFilesListType *theList);


   /*-----------------------------------------------------*/
   /* gmsGetListOfBrowseFiles
   /*
   /* Description:
   /*    This utility builds a data structure that lists
   /*    each of the files (with full path) in a specified
   /*    BROWSE directory.
   /*
   /*    The caller must deallocate the list using the
   /*    utility 'gmsFreeListOfBrowseFiles' (see below).
   /*-----------------------------------------------------*/
   gmsBrowseFilesListType *gmsGetListOfBrowseFiles
                             (gmsBrowseThematicType whichTheme);


   /*-----------------------------------------------------*/
   /* gmsFreeListOfBrowseFiles
   /*
   /* Description:
   /*    This utility deallocates the list of BROWSE files
   /*    that was built using 'gmsGetListOfThemeFiles' (see
   /*    above).
   /*-----------------------------------------------------*/
   void gmsFreeListOfBrowseFiles
           (gmsBrowseFilesListType **theList);


   /*-----------------------------------------------------*/
   /* gmsPrintListOfBrowseFiles
   /*
   /* Description:
   /*    This utility prints (to stdout) the list of BROWSE
   /*    files from 'theList' object.
   /*-----------------------------------------------------*/
   void gmsPrintListOfBrowseFiles
           (gmsBrowseFilesListType *theList);


   /*-----------------------------------------------------*/
   /* gmsGetThemeDirName
   /*
   /* Description:
   /*    Returns the name of the directory of the specified
   /*    theme.
   /*
   /*    The caller must NOT deallocate the pointer.  The
   /*    next call will overwrite the previous value.
   /*-----------------------------------------------------*/
   char *gmsGetThemeDirName
                 (gmsThematicType whichTheme);


   /*-----------------------------------------------------*/
   /* gmsGetSubtileDirName
   /*
   /* Description:
   /*    Returns the name of the directory of the specified
   /*    subtile.
   /*
   /*    The caller must NOT deallocate the pointer.  The
   /*    next call will overwrite the previous value.
   /*-----------------------------------------------------*/
   char *gmsGetSubtileDirName
                 (gmsSubtileType whichSubtile);


   /*-----------------------------------------------------*/
   /* gmsGetRegionalFileName
   /*
   /* Description:
   /*    Returns the name of a regional (i.e. non-BROWSE)
   /*    DCW file.  The 'whichTheme' parameter is used to
   /*    construct the full name of feature tables.  If
   /*    the 'kindOfFile' is a primitive (e.g. EDG), the
   /*    'whichTheme' parameter is ignored.
   /*-----------------------------------------------------*/
   char *gmsGetRegionalFileName
                 (gmsThematicType whichTheme,
                  gmsDcwFileType  kindOfFile);


   /*-----------------------------------------------------*/
   /* gmsCountNumberOfThemeFiles
   /*
   /* Description:
   /*    Utility that counts the number of files specified
   /*    by 'kindOffile' for a given theme.
   /*-----------------------------------------------------*/
   int gmsCountNumberOfThemeFiles
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile);


   /*-----------------------------------------------------*/
   /* gmsGetBrowseDirName
   /*
   /* Description:
   /*    Returns the name of the directory of the specified
   /*    BROWSE theme.
   /*
   /*    The caller must NOT deallocate the pointer.  The
   /*    next call will overwrite the previous value.
   /*-----------------------------------------------------*/
   char *gmsGetBrowseDirName
                 (gmsBrowseThematicType whichTheme);


   /*-----------------------------------------------------*/
   /* gmsGetBrowseFileName
   /*
   /* Description:
   /*    Returns the name of the specified type of BROWSE
   /*    file.
   /*-----------------------------------------------------*/
   char *gmsGetBrowseFileName
                 (gmsBrowseThematicType whichTheme,
                  gmsDcwFileType        kindOfFile);


   /*-----------------------------------------------------*/
   /* gmsCountNumberOfBrowseFiles
   /*
   /* Description:
   /*    Utility that counts the number of BROWSE files
   /*    specified by 'kindOffile' for a given theme.
   /*-----------------------------------------------------*/
   int gmsCountNumberOfBrowseFiles
               (gmsBrowseThematicType whichTheme,
                gmsDcwFileType        kindOfFile);

#endif
