//----------------------------------------------------------------------------*/
// File: gmsFileNameUtilities.cpp
// Date: 11-Jan-00 : initial definition
//       11-Feb-00 : SUN and HP C++ compilers complains about init of char *
//       26-Feb-00 : implement tile/theme util to improve performance
//
// Description:
//    Defines utilites to get the names (and number) of the files
//    of the DCW database.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsFileNameUtilities.h>
#include <gmsUtilities.h>
#include <stdlib.h>
   //---------*/
#include <gmsDebugUtil.h>


//------------------------*/
// Constants
//------------------------*/

static const char *dcwFileNames[Num_DCW_Files] =
              { (char *) "EDG",           // gmsEdgeTable             */
                (char *) "FAC",           // gmsFaceTable             */
                (char *) "TXT",           // gmsTextTable             */
                (char *) "END",           // gmsNodeTable             */
                (char *) "EBR",           // gmsEdgeBoundingRectangle */
                (char *) "FBR",           // gmsFaceBoundingRectangle */
                (char *) "RNG",           // gmsRingTable             */
                (char *) "ESI",           // gmsEdgeSpatialIndex      */
                (char *) "FSI",           // gmsFaceSpatialIndex      */
                (char *) "NSI",           // gmsNodeSpatialIndex      */
                (char *) "xxAREA.AFT",    // gmsAreaFeatureTable      */
                (char *) "xxLINE.LFT",    // gmsLineFeatureTable      */
                (char *) "xxPOINT.PFT",   // gmsPointFeatureTable     */
                (char *) "xxTEXT.TFT",    // gmsTextFeatureTable      */
                (char *) "xxAREA.DOC",    // gmsAreaDocTable          */
                (char *) "xxLINE.DOC",    // gmsLineDocTable          */
                (char *) "xxPOINT.DOC",   // gmsPointDocTable         */
                (char *) "FCS",           // gmsFeatureClassSchema    */
                (char *) "CHAR.VDT",      // gmsCharVDT               */
                (char *) "INT.VDT" };     // gmsIntVDT                */


//------------------------*/
// Local Functions
//------------------------*/

static int countThemePrimitiveFiles
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile,
                const char      Long_Start,
                const char      Lat_Start,
                const char      Long_End,
                const char      Lat_End);

static int countGazetteFiles
              (gmsDcwFileType kindOfFile);

static int countLibRefFiles
              (gmsDcwFileType kindOfFile);

static int countTileRefFiles
              (gmsDcwFileType kindOfFile);

static int countBrowseLibRefFiles
              (gmsDcwFileType kindOfFile);

static void constructFilePaths
              (gmsThemeFilesListType *theList);

static void allocateFilePaths
              (gmsThemeFilesListType *theList);

static void constructPrimitiveFilePaths
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile,
                gmsFilePathType *thePathRec,
                const char      Long_Start = 'A',
                const char      Lat_Start  = 'A',
                const char      Long_End   = 'Z',
                const char      Lat_End    = 'M');

static void constructThemeFilePaths
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile,
                char            *filePath);

static void allocateBrowseFilePaths
              (gmsBrowseFilesListType *browseList);

static void constructBrowseFilePaths
              (gmsBrowseFilesListType *browseList);

static void buildOneBrowseFilePath
              (gmsBrowseThematicType whichTheme,
               gmsDcwFileType        kindOfFile,
               char                  *filePath);

static void buildTileFiles
              (char                  longTileId,
               char                  latTileId,
               gmsThemeFilesListType *newList);


//-----------------------------------------------------*/
// gmsGetListOfThemeFiles
//
// Description:
//    This utility builds a data structure that lists
//    each of the files (with full path) in a specified
//    theme.
//
//    The caller must deallocate the list using the
//    utility 'gmsFreeListOfThemeFiles' (see below).
//
//    WARNING:
//    --------
//       This is a very expensive (time) routine.  The
//       data structure is for an entire theme and since
//       it has to be determined which files exist then
//       build the corresponding data structure, the time
//       required is significant.  (AMD K6/300 MHz,
//       Windows 98 requires about 60 seconds per theme).
//
//       If only the files of a specific tile within a
//       theme are needed, use the utility
//       'gmsGetListOfTileFiles' below.
//-----------------------------------------------------*/
gmsThemeFilesListType *gmsGetListOfThemeFiles
                             (gmsThematicType whichTheme)

{
         gmsThemeFilesListType *newList;

   if ( (whichTheme < gms_AE) || (whichTheme > gms_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetListOfThemeFiles : invalid theme\n");

       return (gmsThemeFilesListType *) NULL;
      }

   newList = (gmsThemeFilesListType *)
             malloc (sizeof(gmsThemeFilesListType));

   gmsClearMemory
      ( (char *) newList,
        sizeof(gmsThemeFilesListType) );

   newList->whichTheme = whichTheme;

   allocateFilePaths (newList);

   constructFilePaths (newList);

   return newList;
}


//-----------------------------------------------------*/
// gmsGetListOfTileFiles
//
// Description:
//    This utility builds a data structure that lists
//    ONLY the files (with full path) for the specified
//    tile and theme.
//
//    The caller must deallocate the list using the
//    utility 'gmsFreeListOfThemeFiles' (see below).
//-----------------------------------------------------*/
gmsThemeFilesListType *gmsGetListOfTileFiles
                             (gmsThematicType whichTheme,
                              char            longTileId,
                              char            latTileId)

{
         gmsThemeFilesListType *newList;

   if ( (whichTheme < gms_AE) || (whichTheme > gms_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetListOfTileFiles : invalid theme\n");

       return (gmsThemeFilesListType *) NULL;
      }

   newList = (gmsThemeFilesListType *)
             malloc (sizeof(gmsThemeFilesListType));

   gmsClearMemory
      ( (char *) newList,
        sizeof(gmsThemeFilesListType) );

   newList->whichTheme = whichTheme;

   buildTileFiles
      (longTileId,
       latTileId,
       newList);

   return newList;
}


//-----------------------------------------------------*/
// gmsFreeListOfThemeFiles
//
// Description:
//    This utility deallocates the list of theme files
//    that was built using 'gmsGetListOfThemeFiles' (see
//    above).
//-----------------------------------------------------*/
void gmsFreeListOfThemeFiles
         (gmsThemeFilesListType **theList)

{
   if (theList == NULL)
      return;

   if (theList[0] == NULL)
      return;

   if (theList[0]->unused_1 != NULL)
      free (theList[0]->unused_1);

   if (theList[0]->unused_2 != NULL)
      free (theList[0]->unused_2);

   free (theList[0]);

   theList[0] = NULL;
}


//-----------------------------------------------------*/
// gmsPrintListOfThemeFiles
//
// Description:
//    This utility prints (to stdout) the list of theme
//    files from 'theList' object.
//-----------------------------------------------------*/
void gmsPrintListOfThemeFiles
         (gmsThemeFilesListType *theList)

{
         int i;

   if (theList == NULL)
     {
      printf("---> No files in theme\n");

      return;
     }

   printf("________________________________\n");
   printf("Files in %s Theme:\n",
          gmsGetThemeDirName(theList->whichTheme));

   printf("Edge Tables:\n");

   for (i = 0; i < theList->edgTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->edgTbls.filePath[i]);

   printf("Face Tables:\n");

   for (i = 0; i < theList->facTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->facTbls.filePath[i]);

   printf("Text Tables:\n");

   for (i = 0; i < theList->txtTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->txtTbls.filePath[i]);

   printf("Node Tables:\n");

   for (i = 0; i < theList->endTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->endTbls.filePath[i]);

   printf("Edge Bounding Tables:\n");

   for (i = 0; i < theList->ebrTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->ebrTbls.filePath[i]);

   printf("Face Bounding Tables:\n");

   for (i = 0; i < theList->fbrTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->fbrTbls.filePath[i]);

   printf("Ring Tables:\n");

   for (i = 0; i < theList->rngTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->rngTbls.filePath[i]);

   printf("Edge Spatial Index Tables:\n");

   for (i = 0; i < theList->esiTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->esiTbls.filePath[i]);

   printf("Face Spatial Index Tables:\n");

   for (i = 0; i < theList->fsiTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->fsiTbls.filePath[i]);

   printf("Node Spatial Index Tables:\n");

   for (i = 0; i < theList->nsiTbls.numFiles; i++)
      printf("   %d) %s\n", i, theList->nsiTbls.filePath[i]);

   printf("Area Feature Table: ");

   if (theList->areaFeatureTbl == NULL)
      printf("-none-\n");
   else
      printf("%s\n", theList->areaFeatureTbl);

   printf("Line Feature Table: ");

   if (theList->lineFeatureTbl == NULL)
      printf("-none-\n");
   else
      printf("%s\n", theList->lineFeatureTbl);

   printf("Point Feature Table: ");

   if (theList->pointFeatureTbl == NULL)
      printf("-none-\n");
   else
      printf("%s\n", theList->pointFeatureTbl);

   printf("Feature Class Schema Table: ");

   if (theList->fcs == NULL)
      printf("-none-\n");
   else
      printf("%s\n", theList->fcs);

   printf("Integer Value-Description-Table: ");

   if (theList->intVDT == NULL)
      printf("-none-\n");
   else
      printf("%s\n", theList->intVDT);

   printf("Character Value-Description-Table: ");

   if (theList->charVDT == NULL)
      printf("-none-\n");
   else
      printf("%s\n", theList->charVDT);

   printf("________________________________\n");
}


//-----------------------------------------------------*/
// gmsGetListOfBrowseFiles
//
// Description:
//    This utility builds a data structure that lists
//    each of the files (with full path) in a specified
//    BROWSE directory.
//
//    The caller must deallocate the list using the
//    utility 'gmsFreeListOfBrowseFiles' (see below).
//-----------------------------------------------------*/
gmsBrowseFilesListType *gmsGetListOfBrowseFiles
                             (gmsBrowseThematicType whichTheme)

{
         gmsBrowseFilesListType *newBrowseList;

   if ( (whichTheme < gmsBrowse_CO) || (whichTheme > gmsBrowse_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetListOfBrowseFiles : invalid theme\n");

       return (gmsBrowseFilesListType *) NULL;
      }

   newBrowseList = (gmsBrowseFilesListType *)
                   malloc (sizeof(gmsBrowseFilesListType));

   gmsClearMemory
      ( (char *) newBrowseList,
        sizeof(gmsBrowseFilesListType) );

   newBrowseList->whichTheme = whichTheme;

   allocateBrowseFilePaths (newBrowseList);

   constructBrowseFilePaths (newBrowseList);

   return newBrowseList;
}


//-----------------------------------------------------*/
// gmsFreeListOfBrowseFiles
//
// Description:
//    This utility deallocates the list of BROWSE files
//    that was built using 'gmsGetListOfThemeFiles' (see
//    above).
//-----------------------------------------------------*/
void gmsFreeListOfBrowseFiles
           (gmsBrowseFilesListType **theList)

{
   if (theList == NULL)
     {
      return;
     }

   if (theList[0] == NULL)
      return;

   free (theList[0]);

   *theList = NULL;
}


//-----------------------------------------------------*/
// gmsPrintListOfBrowseFiles
//
// Description:
//    This utility prints (to stdout) the list of BROWSE
//    files from 'theList' object.
//-----------------------------------------------------*/
void gmsPrintListOfBrowseFiles
           (gmsBrowseFilesListType *theList)

{
   if (theList == NULL)
     {
      printf("---> No files in BROWSE theme\n");

      return;
     }

   printf("________________________________\n");
   printf("BROWSE Files in %s Directory:\n",
          gmsGetBrowseDirName(theList->whichTheme));

   printf("Edge Table          : %s\n", theList->edgTbl);
   printf("Face Table          : %s\n", theList->facTbl);
   printf("Text Table          : %s\n", theList->txtTbl);
   printf("Node Table          : %s\n", theList->endTbl);
   printf("Edge Bounding Rect  : %s\n", theList->ebrTbl);
   printf("Face Bounding Rect  : %s\n", theList->fbrTbl);
   printf("Ring Table          : %s\n", theList->rngTbl);
   printf("Edg Spatial Index   : %s\n", theList->esiTbl);
   printf("Fac Spatial Index   : %s\n", theList->fsiTbl);
   printf("Node Spatial Index  : %s\n", theList->nsiTbl);
   printf("Are Feature Table   : %s\n", theList->areaFeatureTbl);
   printf("Line Feature Table  : %s\n", theList->lineFeatureTbl);
   printf("Point Feature Table : %s\n", theList->pointFeatureTbl);
   printf("Text Feature Table  : %s\n", theList->textFeatureTbl);
   printf("Area .doc Table     : %s\n", theList->areaDocTbl);
   printf("Line .doc Table     : %s\n", theList->lineDocTbl);
   printf("Point .doc Table    : %s\n", theList->pointDocTbl);
   printf("Feature Class Schema: %s\n", theList->fcs);
   printf("char VDT            : %s\n", theList->charVDT);
   printf("int VDT             : %s\n", theList->intVDT);
}


//-----------------------------------------------------*/
// gmsGetThemeDirName
//
// Description:
//    Returns the name of the directory of the specified
//    theme.
//
//    The caller must NOT deallocate the pointer.  The
//    next call will overwrite the previous value.
//-----------------------------------------------------*/
char *gmsGetThemeDirName
              (gmsThematicType whichTheme)

{
         static const char *Theme_Dir_Names[Num_Themes] =
                         { (char *) "AE", // Aeronautical Coverage      */
                           (char *) "CL", // Cultural Landmarks         */
                           (char *) "DQ", // Data Quality               */
                           (char *) "DN", // Drainage                   */
                           (char *) "DS", // Drainage - supplemental    */
                           (char *) "HY", // Hypsography                */
                           (char *) "HS", // Hypsography - supplemental */
                           (char *) "LC", // Land Coverage              */
                           (char *) "OF", // Ocean Features             */
                           (char *) "PH", // Physiography               */
                           (char *) "PO", // Political Oceans           */
                           (char *) "PP", // Populated Places           */
                           (char *) "RR", // Railroads                  */
                           (char *) "RD", // Roads                      */
                           (char *) "TS", // Transportation Sturcture   */
                           (char *) "UT", // Utilities                  */
                           (char *) "VG", // Vegetation                 */
                           (char *) "Gazette",
                           (char *) "TileRef",
                           (char *) "LibRef" };
         static char answer[8];

   if ( (whichTheme < gms_AE) || (whichTheme > gms_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetThemeDirName : invalid theme\n");

       return (char *) NULL;
      }

   sprintf(answer, "%s", Theme_Dir_Names[whichTheme]);

   return answer;
}


//-----------------------------------------------------*/
// gmsGetSubtileDirName
//
// Description:
//    Returns the name of the directory of the specified
//    subtile.
//
//    The caller must NOT deallocate the pointer.  The
//    next call will overwrite the previous value.
//-----------------------------------------------------*/
char *gmsGetSubtileDirName
              (gmsSubtileType whichSubtile)

{
         static const int  Num_Subtile_Names = 9;
         static const char *Subtile_Dir_Names[Num_Subtile_Names] =
                              { (char *) "11",
                                (char *) "12",
                                (char *) "13",
                                (char *) "21",
                                (char *) "22",
                                (char *) "23",
                                (char *) "31",
                                (char *) "32",
                                (char *) "33" };
         static char subtileName[4];

   if ( (whichSubtile < gms_11) || (whichSubtile > gms_33) )
      {
       gmsWriteDebug("*** ERR : gmsGetSubtileDirName : invalid subtile\n");

       return (char *) NULL;
      }

   sprintf(subtileName, "%s", Subtile_Dir_Names[whichSubtile]);

   return subtileName;
}


//-----------------------------------------------------*/
// gmsGetRegionalFileName
//
// Description:
//    Returns the name of a regional (i.e. non-BROWSE)
//    DCW file.  The 'whichTheme' parameter is used to
//    construct the full name of feature tables.  If
//    the 'kindOfFile' is a primitive (e.g. EDG), the
//    'whichTheme' parameter is ignored.
//-----------------------------------------------------*/
char *gmsGetRegionalFileName
              (gmsThematicType whichTheme,
               gmsDcwFileType  kindOfFile)

{
         static char regFileName[32];
         char        *themeDirName;

   if ( (kindOfFile < gmsEdgeTable) || (kindOfFile > gmsIntVDT) )
      {
       gmsWriteDebug("*** ERR : gmsGetThemeFileName : invalid file\n");

       return (char *) NULL;
      }

   sprintf(regFileName, "%s", dcwFileNames[kindOfFile]);

   if ( (kindOfFile >= gmsEdgeTable) &&
        (kindOfFile <= gmsNodeSpatialIndex) )
      {
       return regFileName;
      }

   if (whichTheme == gms_Gazetteer)
   {
       if (kindOfFile == gmsPointFeatureTable)
       {
          sprintf(regFileName, "GAZETTE.PFT");
       }
       return regFileName;
   }

   else if (whichTheme == gms_TileRef)
      {
       if (kindOfFile == gmsAreaFeatureTable)
       {
          sprintf(regFileName, "TILEREF.AFT");
       } 
       return regFileName;
      }

   else if (whichTheme == gms_LibRef)
   {
       if (kindOfFile == gmsLineFeatureTable)
       {
          sprintf(regFileName, "LIBREF.LFT");
       }
       return regFileName;
   }

   if ( (kindOfFile >= gmsAreaFeatureTable)  &&
        (kindOfFile <= gmsPointDocTable) )
      {
       themeDirName = gmsGetThemeDirName(whichTheme);

       regFileName[0] = themeDirName[0];
       regFileName[1] = themeDirName[1];
      }

   return regFileName;
}


//-----------------------------------------------------*/
// gmsCountNumberOfThemeFiles
//
// Description:
//    Utility that counts the number of files specified
//    by 'kindOffile' for a given theme.
//-----------------------------------------------------*/
int gmsCountNumberOfThemeFiles
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile)

{
         static char *themeName;
         static char *regionalFileName;
         static char filePath[256];
         int         numFiles;

   if (whichTheme == gms_Gazetteer)
      {
       numFiles = countGazetteFiles (kindOfFile);
      }

   else if (whichTheme == gms_TileRef)
      {
       numFiles = countTileRefFiles (kindOfFile);
      }

   else if (whichTheme == gms_LibRef)
      {
       numFiles = countLibRefFiles (kindOfFile);
      }

   else if ( (kindOfFile >= gmsAreaFeatureTable) &&
             (kindOfFile <= gmsIntVDT) )
      {
       themeName = gmsGetThemeDirName(whichTheme);

       regionalFileName = gmsGetRegionalFileName
                             (whichTheme,
                              kindOfFile);

       sprintf(filePath, "%s/%s",
               gmsGetDcwFullPath(themeName),
               regionalFileName);

       numFiles = (int) gmsFileExists(filePath);
      }

   else
      {
       numFiles = countThemePrimitiveFiles
                     (whichTheme,
                      kindOfFile,
                      'A', 'A',
                      'Z', 'M');
      }

   return numFiles;
}


//-----------------------------------------------------*/
// gmsGetBrowseDirName
//
// Description:
//    Returns the name of the directory of the specified
//    BROWSE theme.
//
//    The caller must NOT deallocate the pointer.  The
//    next call will overwrite the previous value.
//-----------------------------------------------------*/
char *gmsGetBrowseDirName
                 (gmsBrowseThematicType whichTheme)

{
         static char *browseDirs[Num_Browse_Themes] =
                        { (char *) "CO",       // gmsBrowse_CO     */
                          (char *) "DV",       // gmsBrowse_DV     */
                          (char *) "DN",       // gmsBrowse_DN     */
                          (char *) "GR",       // gmsBrowse_GR     */
                          (char *) "DA",       // gmsBrowse_DA     */
                          (char *) "IN",       // gmsBrowse_IN     */
                          (char *) "PO",       // gmsBrowse_PO     */
                          (char *) "PP",       // gmsBrowse_PP     */
                          (char *) "LIBREF" }; // gmsBrowse_LibRef */

   if ( (whichTheme < gmsBrowse_CO) || (whichTheme > gmsBrowse_LibRef) )
      {
       gmsWriteDebug("*** ERR : gmsGetBrowseDirName : invalid theme\n");

       return (char *) NULL;
      }

   return browseDirs[whichTheme];
}


//-----------------------------------------------------*/
// gmsGetBrowseFileName
//
// Description:
//    Returns the name of the specified type of BROWSE
//    file.
//-----------------------------------------------------*/
char *gmsGetBrowseFileName
                 (gmsBrowseThematicType whichTheme,
                  gmsDcwFileType        kindOfFile)

{
         char        *themeName;
         static char browseFileName[64];

   if (kindOfFile == gmsFeatureClassSchema)
   {
      sprintf(browseFileName, "FCS");
      return browseFileName;
   }

   if (kindOfFile == gmsCharVDT)
   {
      sprintf(browseFileName, "CHAR.VDT");
      return browseFileName;
   }

   if (kindOfFile == gmsIntVDT)
   {
      sprintf(browseFileName, "INT.VDT");
      return browseFileName;
   }

   sprintf(browseFileName, "%s", dcwFileNames[kindOfFile]);

   if ( (kindOfFile >= gmsEdgeTable) &&
        (kindOfFile <= gmsNodeSpatialIndex) )
      {
       return browseFileName;
      }

   if ( (kindOfFile == gmsAreaFeatureTable) ||
        (kindOfFile == gmsLineFeatureTable) ||
        (kindOfFile == gmsPointFeatureTable) ||
        (kindOfFile == gmsTextFeatureTable) )
      {
       themeName = gmsGetBrowseDirName (whichTheme);

       browseFileName[0] = themeName[0];
       browseFileName[1] = themeName[1];

       return browseFileName;
      }

   return (char *) NULL;
}


//-----------------------------------------------------*/
// gmsCountNumberOfBrowseFiles
//
// Description:
//    Utility that counts the number of BROWSE files
//    specified by 'kindOffile' for a given theme.
//-----------------------------------------------------*/
int gmsCountNumberOfBrowseFiles
               (gmsBrowseThematicType whichTheme,
                gmsDcwFileType        kindOfFile)

{
         char *dirName;
         char *browseName;
         char filePath[96];
         int  numFiles = 0;

   if (whichTheme == gmsBrowse_LibRef)
      {
       numFiles = countBrowseLibRefFiles (kindOfFile);
      }

   else
      {
       dirName = gmsGetBrowseDirName(whichTheme);

       browseName = gmsGetBrowseFileName(whichTheme, kindOfFile);

       if ( (dirName == NULL) || (browseName == NULL) )
          return 0;

       sprintf(filePath, "%s/%s",
               gmsGetBrowseFullPath(dirName),
               browseName);

       numFiles = (int) gmsFileExists(filePath);
      }

   return numFiles;
}


           //------------------------*/
           //    Local Functions
           //------------------------*/


//-----------------------------------------------------*/
// countThemePrimitiveFiles
//
// Description:
//    Utility that counts the number of primitive files
//    specified by 'kindOffile' for a given theme.
//-----------------------------------------------------*/
static int countThemePrimitiveFiles
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile,
                const char      Long_Start,
                const char      Lat_Start,
                const char      Long_End,
                const char      Lat_End)

{
         char           longDir;
         char           latDir;
         static char    tempPath_1[128];
         static char    tempPath_2[256];
         static char    filePath[512];
         gmsSubtileType subtile;
         bool           fileExists;
         bool           dirExists;
         int            numFiles = 0;

   for (longDir = Long_Start; longDir <= Long_End; longDir++)
      {
       if ( (longDir == 'I') || (longDir == 'O') )
          longDir++;

       sprintf(tempPath_1, "%s/%c",
               gmsGetDcwFullPath(gmsGetThemeDirName(whichTheme)),
               longDir);

       dirExists = gmsDirectoryExists (tempPath_1);

       if ( ! dirExists )
          {
           continue;  // i.e. go to the top of the LONGITUDE loop */
          }

       for (latDir = Lat_Start; latDir <= Lat_End; latDir++)
          {
           if (latDir == 'I')
              latDir++;

           sprintf(tempPath_2, "%s/%c",
                   tempPath_1,
                   latDir);

           dirExists = gmsDirectoryExists (tempPath_2);

           if ( ! dirExists )
              {
               continue;  // i.e. go to the top of the LATITUDE loop */
              }

           subtile = gms_11;

           while (subtile <= gms_33)
              {
               sprintf(filePath, "%s/%s/%s",
                       tempPath_2,
                       gmsGetSubtileDirName(subtile),
                       dcwFileNames[kindOfFile]);

               fileExists = gmsFileExists(filePath);

               if (fileExists)
                  numFiles++;

               subtile =  (gmsSubtileType) (subtile + 1);
              }
          }
      }

   return numFiles;
}


//-----------------------------------------------------*/
// countGazetteFiles
//
// Description:
//    Utility that returns the count of the specified
//    file from the "Gazette" library.  The values
//    are hard-coded for simplicity.
//-----------------------------------------------------*/
static int countGazetteFiles
              (gmsDcwFileType kindOfFile)

{
      const int gazetteCount[Num_DCW_Files] =
                        { 0,   // gmsEdgeTable             */
                          0,   // gmsFaceTable             */
                          0,   // gmsTextTable             */
                          1,   // gmsNodeTable             */
                          0,   // gmsEdgeBoundingRectangle */
                          0,   // gmsFaceBoundingRectangle */
                          0,   // gmsRingTable             */
                          0,   // gmsEdgeSpatialIndex      */
                          0,   // gmsFaceSpatialIndex      */
                          1,   // gmsNodeSpatialIndex      */
                          0,   // gmsAreaFeatureTable      */
                          0,   // gmsLineFeatureTable      */
                          1,   // gmsPointFeatureTable     */
                          0,   // gmsTextFeatureTable      */
                          0,   // gmsAreaDocTable          */
                          0,   // gmsLineDocTable          */
                          0,   // gmsPointDocTable         */
                          1,   // gmsFeatureClassSchema    */
                          0,   // gmsCharVDT               */
                          0 }; // gmsIntVDT                */

   if ( (kindOfFile < gmsEdgeTable) || (kindOfFile > gmsIntVDT) )
      {
       gmsWriteDebug("*** ERR : countGazetteFiles : invalid file\n");

       return 0;
      }

   return gazetteCount[kindOfFile];
}


//-----------------------------------------------------*/
// countLibRefFiles
//
// Description:
//    Utility that returns the count of the specified
//    file from the "Lib Ref" library.  The values
//    are hard-coded for simplicity.
//-----------------------------------------------------*/
static int countLibRefFiles
              (gmsDcwFileType kindOfFile)

{
      const int libRefCount[Num_DCW_Files] =
                        { 1,   // gmsEdgeTable             */
                          0,   // gmsFaceTable             */
                          0,   // gmsTextTable             */
                          0,   // gmsNodeTable             */
                          1,   // gmsEdgeBoundingRectangle */
                          0,   // gmsFaceBoundingRectangle */
                          0,   // gmsRingTable             */
                          0,   // gmsEdgeSpatialIndex      */
                          0,   // gmsFaceSpatialIndex      */
                          0,   // gmsNodeSpatialIndex      */
                          0,   // gmsAreaFeatureTable      */
                          1,   // gmsLineFeatureTable      */
                          0,   // gmsPointFeatureTable     */
                          0,   // gmsTextFeatureTable      */
                          0,   // gmsAreaDocTable          */
                          0,   // gmsLineDocTable          */
                          0,   // gmsPointDocTable         */
                          1,   // gmsFeatureClassSchema    */
                          0,   // gmsCharVDT               */
                          0 }; // gmsIntVDT                */

   if ( (kindOfFile < gmsEdgeTable) || (kindOfFile > gmsIntVDT) )
      {
       gmsWriteDebug("*** ERR : countLibRefFiles : invalid file\n");

       return 0;
      }

   return libRefCount[kindOfFile];
}


//-----------------------------------------------------*/
// countTileRefFiles
//
// Description:
//    Utility that returns the count of the specified
//    file from the "Tile Ref" library.  The values
//    are hard-coded for simplicity.
//-----------------------------------------------------*/
static int countTileRefFiles
              (gmsDcwFileType kindOfFile)

{
      const int tileRefCount[Num_DCW_Files] =
                        { 1,   // gmsEdgeTable             */
                          1,   // gmsFaceTable             */
                          0,   // gmsTextTable             */
                          0,   // gmsNodeTable             */
                          1,   // gmsEdgeBoundingRectangle */
                          1,   // gmsFaceBoundingRectangle */
                          1,   // gmsRingTable             */
                          0,   // gmsEdgeSpatialIndex      */
                          0,   // gmsFaceSpatialIndex      */
                          0,   // gmsNodeSpatialIndex      */
                          1,   // gmsAreaFeatureTable      */
                          0,   // gmsLineFeatureTable      */
                          0,   // gmsPointFeatureTable     */
                          0,   // gmsTextFeatureTable      */
                          0,   // gmsAreaDocTable          */
                          0,   // gmsLineDocTable          */
                          0,   // gmsPointDocTable         */
                          1,   // gmsFeatureClassSchema    */
                          0,   // gmsCharVDT               */
                          0 }; // gmsIntVDT                */

   if ( (kindOfFile < gmsEdgeTable) || (kindOfFile > gmsIntVDT) )
      {
       gmsWriteDebug("*** ERR : countTileRefFiles : invalid file\n");

       return 0;
      }

   return tileRefCount[kindOfFile];
}


//-----------------------------------------------------*/
// countBrowseLibRefFiles
//
// Description:
//    Utility that returns the count of the specified
//    file from the BROWSE "Lib Ref" library.  In
//    reality, the browse libref only contains an
//    EDG (and the corresponding EDX) file.
//-----------------------------------------------------*/
static int countBrowseLibRefFiles
              (gmsDcwFileType kindOfFile)

{
   if (kindOfFile == gmsEdgeTable)
      return 1;
   else
      return 0;
}


//-----------------------------------------------------*/
// allocateFilePaths
//
// Description:
//    This function allocates a character buffer that
//    will be used to contain the paths to the files
//    in a theme.  The buffer is contiguous and its
//    size is:
//
//         Num Rows    = Num files (in theme)
//         Num Columns = <some max width, e.g. 64 chars>
//
//    Each attribute of the 'theList' is set to point
//    to a row of the buffer.
//-----------------------------------------------------*/
static void allocateFilePaths
              (gmsThemeFilesListType *theList)

{
         int       fileCount[Num_DCW_Files];
         int       totalNumFiles = 0;
         int       i, midIndex, pathIndex;
         char      **midBuffer;
         const int Num_Primitive_Files = 10;
         char      *pathBuffer;
         const int Max_Path_Chars = 64;

   for (i = 0; i < Num_DCW_Files; i++)
      {
       fileCount[i] = gmsCountNumberOfThemeFiles
                         (theList->whichTheme,
                          (gmsDcwFileType) i);

       totalNumFiles = totalNumFiles + fileCount[i];
      }

   pathBuffer = (char *) malloc (totalNumFiles * Max_Path_Chars);

   if (pathBuffer == (char *) NULL)
      {
       gmsWriteDebug("*** ERR : allocateFilePaths : 'malloc' FAILED.\n");

       return;
      }

   gmsClearMemory
      (pathBuffer,
       totalNumFiles * Max_Path_Chars);

   totalNumFiles = 0;

   for (i = 0; i < Num_Primitive_Files; i++)
      {
       totalNumFiles = totalNumFiles + fileCount[i];
      }

   midBuffer = (char **) malloc ( totalNumFiles * sizeof(char *) );

   pathIndex = 0;

   midIndex = 0;

   //--------------------------*/
   // gmsEdgeTable             */
   //--------------------------*/
   theList->edgTbls.numFiles = fileCount[(int) gmsEdgeTable];

   if (theList->edgTbls.numFiles > 0)
      {
       theList->edgTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->edgTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsFaceTable             */
   //--------------------------*/
   theList->facTbls.numFiles = fileCount[(int) gmsFaceTable];

   if (theList->facTbls.numFiles > 0)
      {
       theList->facTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->facTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsTextTable             */
   //--------------------------*/
   theList->txtTbls.numFiles = fileCount[(int) gmsTextTable];

   if (theList->txtTbls.numFiles > 0)
      {
       theList->txtTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->txtTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsNodeTable             */
   //--------------------------*/
   theList->endTbls.numFiles = fileCount[(int) gmsNodeTable];

   if (theList->endTbls.numFiles > 0)
      {
       theList->endTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->endTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsEdgeBoundingRectangle */
   //--------------------------*/
   theList->ebrTbls.numFiles = fileCount[(int) gmsEdgeBoundingRectangle];

   if (theList->ebrTbls.numFiles > 0)
      {
       theList->ebrTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->ebrTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsFaceBoundingRectangle */
   //--------------------------*/
   theList->fbrTbls.numFiles = fileCount[(int) gmsFaceBoundingRectangle];

   if (theList->fbrTbls.numFiles > 0)
      {
       theList->fbrTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->fbrTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsRingTable             */
   //--------------------------*/
   theList->rngTbls.numFiles = fileCount[(int) gmsRingTable];

   if (theList->rngTbls.numFiles > 0)
      {
       theList->rngTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->rngTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsEdgeSpatialIndex      */
   //--------------------------*/
   theList->esiTbls.numFiles = fileCount[(int) gmsEdgeSpatialIndex];

   if (theList->esiTbls.numFiles > 0)
      {
       theList->esiTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->esiTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsFaceSpatialIndex      */
   //--------------------------*/
   theList->fsiTbls.numFiles = fileCount[(int) gmsFaceSpatialIndex];

   if (theList->fsiTbls.numFiles > 0)
      {
       theList->fsiTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->fsiTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsNodeSpatialIndex      */
   //--------------------------*/
   theList->nsiTbls.numFiles = fileCount[(int) gmsNodeSpatialIndex];

   if (theList->nsiTbls.numFiles > 0)
      {
       theList->nsiTbls.filePath = &midBuffer[midIndex];

       for (i = 0; i < theList->nsiTbls.numFiles; i++)
          {
           midBuffer[midIndex] = &pathBuffer[pathIndex];

           midIndex++;

           pathIndex = pathIndex + Max_Path_Chars;
          }
      }

   //--------------------------*/
   // gmsAreaFeatureTable      */
   //--------------------------*/
   if (fileCount[(int) gmsAreaFeatureTable] == 1)
      {
       theList->areaFeatureTbl = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsLineFeatureTable      */
   //--------------------------*/
   if (fileCount[(int) gmsLineFeatureTable] == 1)
      {
       theList->lineFeatureTbl = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsPointFeatureTable     */
   //--------------------------*/
   if (fileCount[(int) gmsPointFeatureTable] == 1)
      {
       theList->pointFeatureTbl = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsTextFeatureTable      */
   //--------------------------*/
   if (fileCount[(int) gmsTextFeatureTable] == 1)
      {
       theList->textFeatureTbl = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsAreaDocTable          */
   //--------------------------*/
   if (fileCount[(int) gmsAreaDocTable] == 1)
      {
       theList->areaDocTbl = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsLineDocTable          */
   //--------------------------*/
   if (fileCount[(int) gmsLineDocTable] == 1)
      {
       theList->lineDocTbl = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsPointDocTable         */
   //--------------------------*/
   if (fileCount[(int) gmsPointDocTable] == 1)
      {
       theList->pointDocTbl = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsFeatureClassSchema    */
   //--------------------------*/
   if (fileCount[(int) gmsFeatureClassSchema] == 1)
      {
       theList->fcs = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsCharVDT               */
   //--------------------------*/
   if (fileCount[(int) gmsCharVDT] == 1)
      {
       theList->charVDT = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsIntVDT                */
   //--------------------------*/
   if (fileCount[(int) gmsIntVDT] == 1)
      {
       theList->intVDT = &pathBuffer[pathIndex];

       pathIndex = pathIndex + Max_Path_Chars;
      }

   theList->unused_1 = pathBuffer;

   theList->unused_2 = midBuffer;
}


//-----------------------------------------------------*/
// constructFilePaths
//
// Description:
//-----------------------------------------------------*/
static void constructFilePaths
              (gmsThemeFilesListType *theList)

{
   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsEdgeTable,
       &(theList->edgTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsFaceTable,
       &(theList->facTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsTextTable,
       &(theList->txtTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsNodeTable,
       &(theList->endTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsEdgeBoundingRectangle,
       &(theList->ebrTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsFaceBoundingRectangle,
       &(theList->fbrTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsRingTable,
       &(theList->rngTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsEdgeSpatialIndex,
       &(theList->esiTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsFaceSpatialIndex,
       &(theList->fsiTbls));

   constructPrimitiveFilePaths
      (theList->whichTheme,
       gmsNodeSpatialIndex,
       &(theList->nsiTbls));

   constructThemeFilePaths
      (theList->whichTheme,
       gmsAreaFeatureTable,
       theList->areaFeatureTbl);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsLineFeatureTable,
       theList->lineFeatureTbl);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsPointFeatureTable,
       theList->pointFeatureTbl);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsTextFeatureTable,
       theList->textFeatureTbl);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsAreaDocTable,
       theList->areaDocTbl);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsLineDocTable,
       theList->lineDocTbl);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsPointDocTable,
       theList->pointDocTbl);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsFeatureClassSchema,
       theList->fcs);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsCharVDT,
       theList->charVDT);

   constructThemeFilePaths
      (theList->whichTheme,
       gmsIntVDT,
       theList->intVDT);
}


//-----------------------------------------------------*/
// constructPrimitiveFilePaths
//
// Description:
//    Utility that determines the existence of the
//    specified primitive files within the specified
//    theme.  If the file exists, it is added to the
//    file-path record.
//-----------------------------------------------------*/
static void constructPrimitiveFilePaths
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile,
                gmsFilePathType *thePathRec,
                const char      Long_Start,
                const char      Lat_Start,
                const char      Long_End,
                const char      Lat_End)

{
         char           longDir;
         char           latDir;
         char           *themeName;
         static char    tempPath_1[128];
         static char    tempPath_2[256];
         static char    filePath[512];
         gmsSubtileType subtile;
         bool           fileExists;
         bool           dirExists;
         int            numFiles = 0;

   if (thePathRec->numFiles <= 0)
      return;

   themeName = gmsGetThemeDirName(whichTheme);

   if ( (whichTheme == gms_Gazetteer) ||
        (whichTheme == gms_TileRef) ||
        (whichTheme == gms_LibRef) )
      {
       sprintf(thePathRec->filePath[0],
               "%s/%s",
               gmsGetDcwFullPath(themeName),
               dcwFileNames[kindOfFile]);

       thePathRec->numFiles = 1;

       return;
      }

   for (longDir = Long_Start; longDir <= Long_End; longDir++)
      {
       if ( (longDir == 'I') || (longDir == 'O') )
          longDir++;

       sprintf(tempPath_1, "%s/%c",
               gmsGetDcwFullPath(themeName),
               longDir);

       dirExists = gmsDirectoryExists (tempPath_1);

       if ( ! dirExists )
          {
           continue;  // i.e. go to the top of the LONGITUDE loop */
          }

       for (latDir = Lat_Start; latDir <= Lat_End; latDir++)
          {
           if (latDir == 'I')
              latDir++;

           sprintf(tempPath_2, "%s/%c",
                   tempPath_1,
                   latDir);

           dirExists = gmsDirectoryExists (tempPath_2);

           if ( ! dirExists )
              {
               continue;  // i.e. go to the top of the LATITUDE loop */
              }

           subtile = gms_11;

           while (subtile <= gms_33)
              {
               sprintf(filePath, "%s/%s/%s",
                       tempPath_2,
                       gmsGetSubtileDirName(subtile),
                       dcwFileNames[kindOfFile]);

               fileExists = gmsFileExists(filePath);

               if (fileExists)
                  {
                   sprintf(thePathRec->filePath[numFiles],
                           "%s",
                           filePath);

                   numFiles++;
                  }

               subtile =  (gmsSubtileType) (subtile + 1);
              }
          }
      }

   thePathRec->numFiles = numFiles;
}


//-----------------------------------------------------*/
// constructThemeFilePaths
//
// Description:
//    Utility that determines the existence of a non-
//    primitive file in a theme directory.  If it
//    exists, it is written to the parameter 'filePath'.
//-----------------------------------------------------*/
static void constructThemeFilePaths
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile,
                char            *filePath)

{
         static char *themeName;
         static char *regionalFileName;

   if (filePath == NULL)
      return;

   if ( (kindOfFile >= gmsAreaFeatureTable) && (kindOfFile <= gmsIntVDT) )
      {
       themeName = gmsGetThemeDirName(whichTheme);

       regionalFileName = gmsGetRegionalFileName
                             (whichTheme,
                              kindOfFile);

       sprintf(filePath, "%s/%s",
               gmsGetDcwFullPath(themeName),
               regionalFileName);
      }
}


//-----------------------------------------------------*/
// allocateBrowseFilePaths
//
// Description:
//-----------------------------------------------------*/
static void allocateBrowseFilePaths
              (gmsBrowseFilesListType *browseList)

{
         char      *browseBuffer;
         const int Path_Len = 56;
         int       i;

   browseBuffer = (char *) malloc (Num_DCW_Files * Path_Len);

   if (browseBuffer == NULL)
      {
       gmsWriteDebug("*** ERR : allocateBrowseFilePaths - malloc FAIL\n");

       return;
      }

   gmsClearMemory
      (browseBuffer,
       Num_DCW_Files * Path_Len);

   i = 0;

   browseList->edgTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->facTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->txtTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->endTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->ebrTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->fbrTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->rngTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->esiTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->fsiTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->nsiTbl          = &browseBuffer[i]; i = i + Path_Len;
   browseList->areaFeatureTbl  = &browseBuffer[i]; i = i + Path_Len;
   browseList->lineFeatureTbl  = &browseBuffer[i]; i = i + Path_Len;
   browseList->pointFeatureTbl = &browseBuffer[i]; i = i + Path_Len;
   browseList->textFeatureTbl  = &browseBuffer[i]; i = i + Path_Len;
   browseList->areaDocTbl      = &browseBuffer[i]; i = i + Path_Len;
   browseList->lineDocTbl      = &browseBuffer[i]; i = i + Path_Len;
   browseList->pointDocTbl     = &browseBuffer[i]; i = i + Path_Len;
   browseList->fcs             = &browseBuffer[i]; i = i + Path_Len;
   browseList->charVDT         = &browseBuffer[i]; i = i + Path_Len;
   browseList->intVDT          = &browseBuffer[i]; i = i + Path_Len;
}


//-----------------------------------------------------*/
// constructBrowseFilePaths
//
// Description:
//-----------------------------------------------------*/
static void constructBrowseFilePaths
              (gmsBrowseFilesListType *browseList)

{
   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsEdgeTable,
       browseList->edgTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsFaceTable,
       browseList->facTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsTextTable,
       browseList->txtTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsNodeTable,
       browseList->endTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsEdgeBoundingRectangle,
       browseList->ebrTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsFaceBoundingRectangle,
       browseList->fbrTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsRingTable,
       browseList->rngTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsEdgeSpatialIndex,
       browseList->esiTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsFaceSpatialIndex,
       browseList->fsiTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsNodeSpatialIndex,
       browseList->nsiTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsAreaFeatureTable,
       browseList->areaFeatureTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsLineFeatureTable,
       browseList->lineFeatureTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsPointFeatureTable,
       browseList->pointFeatureTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsTextFeatureTable,
       browseList->textFeatureTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsAreaDocTable,
       browseList->areaDocTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsLineDocTable,
       browseList->lineDocTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsPointDocTable,
       browseList->pointDocTbl);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsFeatureClassSchema,
       browseList->fcs);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsCharVDT,
       browseList->charVDT);

   buildOneBrowseFilePath
      (browseList->whichTheme,
       gmsIntVDT,
       browseList->intVDT);
}


//-----------------------------------------------------*/
// buildOneBrowseFilePath
//
// Description:
//-----------------------------------------------------*/
static void buildOneBrowseFilePath
              (gmsBrowseThematicType whichTheme,
               gmsDcwFileType        kindOfFile,
               char                  *filePath)

{
         char *dirName;
         char *browseName;
         char tempPath[64];

   if (filePath == NULL)
      return;

   if ( (whichTheme == gmsBrowse_LibRef) && (kindOfFile != gmsEdgeTable) )
      {
       return;
      }

   dirName = gmsGetBrowseDirName(whichTheme);

   browseName = gmsGetBrowseFileName(whichTheme, kindOfFile);

   if ( (dirName == NULL) || (browseName == NULL) )
       return;

   sprintf(tempPath, "%s/%s",
           gmsGetBrowseFullPath(dirName),
           browseName);

   if ( gmsFileExists(tempPath) )
      sprintf(filePath, "%s", tempPath);
}


//-----------------------------------------------------*/
// allocateTileFilePaths
//
// Description:
//    This function allocates a character buffer that
//    will be used to contain the paths to the files
//    in one tile of a theme.  The buffer is contiguous
//    and its size is:
//
//         Num Rows    = Num files (in tile of theme)
//         Num Columns = <some max width, e.g. 64 chars>
//
//    Each attribute of the 'theList' is set to point
//    to a row of the buffer.
//-----------------------------------------------------*/
static void allocateTileFilePaths
              (gmsThemeFilesListType *theList)

{
         int       i, midIndex, pathIndex;
         char      **midBuffer;
         char      *pathBuffer;
         const int Num_Primitive_Files = 10;
         const int Num_Feature_Files = 10;
         const int Max_Path_Chars = 64;
         int       totalNumFiles;

   totalNumFiles = (Num_Subtiles * Num_Primitive_Files) + Num_Feature_Files;

   pathBuffer = (char *) malloc (totalNumFiles * Max_Path_Chars);

   if (pathBuffer == (char *) NULL)
      {
       gmsWriteDebug("*** ERR : allocateTileFilePaths : 'malloc' FAILED.\n");

       return;
      }

   gmsClearMemory
      (pathBuffer,
       totalNumFiles * Max_Path_Chars);

   midBuffer = (char **) malloc ( totalNumFiles * sizeof(char *) );

   pathIndex = 0;

   midIndex = 0;

   //--------------------------*/
   // gmsEdgeTable             */
   //--------------------------*/
   theList->edgTbls.numFiles = Num_Subtiles;

   theList->edgTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->edgTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsFaceTable             */
   //--------------------------*/
   theList->facTbls.numFiles = Num_Subtiles;

   theList->facTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->facTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsTextTable             */
   //--------------------------*/
   theList->txtTbls.numFiles = Num_Subtiles;

   theList->txtTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->txtTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsNodeTable             */
   //--------------------------*/
   theList->endTbls.numFiles = Num_Subtiles;

   theList->endTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->endTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsEdgeBoundingRectangle */
   //--------------------------*/
   theList->ebrTbls.numFiles = Num_Subtiles;

   theList->ebrTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->ebrTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsFaceBoundingRectangle */
   //--------------------------*/
   theList->fbrTbls.numFiles = Num_Subtiles;

   theList->fbrTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->fbrTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsRingTable             */
   //--------------------------*/
   theList->rngTbls.numFiles = Num_Subtiles;

   theList->rngTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->rngTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsEdgeSpatialIndex      */
   //--------------------------*/
   theList->esiTbls.numFiles = Num_Subtiles;

   theList->esiTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->esiTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsFaceSpatialIndex      */
   //--------------------------*/
   theList->fsiTbls.numFiles = Num_Subtiles;

   theList->fsiTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->fsiTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsNodeSpatialIndex      */
   //--------------------------*/
   theList->nsiTbls.numFiles = Num_Subtiles;

   theList->nsiTbls.filePath = &midBuffer[midIndex];

   for (i = 0; i < theList->nsiTbls.numFiles; i++)
      {
       midBuffer[midIndex] = &pathBuffer[pathIndex];

       midIndex++;

       pathIndex = pathIndex + Max_Path_Chars;
      }

   //--------------------------*/
   // gmsAreaFeatureTable      */
   //--------------------------*/
   theList->areaFeatureTbl = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsLineFeatureTable      */
   //--------------------------*/
   theList->lineFeatureTbl = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsPointFeatureTable     */
   //--------------------------*/
   theList->pointFeatureTbl = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsTextFeatureTable      */
   //--------------------------*/
   theList->textFeatureTbl = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsAreaDocTable          */
   //--------------------------*/
   theList->areaDocTbl = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsLineDocTable          */
   //--------------------------*/
   theList->lineDocTbl = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsPointDocTable         */
   //--------------------------*/
   theList->pointDocTbl = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsFeatureClassSchema    */
   //--------------------------*/
   theList->fcs = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsCharVDT               */
   //--------------------------*/
   theList->charVDT = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   //--------------------------*/
   // gmsIntVDT                */
   //--------------------------*/
   theList->intVDT = &pathBuffer[pathIndex];

   pathIndex = pathIndex + Max_Path_Chars;

   theList->unused_1 = pathBuffer;

   theList->unused_2 = midBuffer;
}


//-----------------------------------------------------*/
// constructTileThemeFilePaths
//
// Description:
//    Utility that determines the existence of a non-
//    primitive file in one tile of a theme directory.
//    If it exists, it is written to the parameter
//    'filePath'.
//-----------------------------------------------------*/
static void constructTileThemeFilePaths
               (gmsThematicType whichTheme,
                gmsDcwFileType  kindOfFile,
                char            *filePath)

{
         static char *themeName;
         static char *regionalFileName;
         static char tempBuffer[128];

   if ( (kindOfFile >= gmsAreaFeatureTable) && (kindOfFile <= gmsIntVDT) )
      {
       themeName = gmsGetThemeDirName(whichTheme);

       regionalFileName = gmsGetRegionalFileName
                             (whichTheme,
                              kindOfFile);

       sprintf(tempBuffer, "%s/%s",
               gmsGetDcwFullPath(themeName),
               regionalFileName);

       if ( gmsFileExists(tempBuffer) )
          {
           sprintf(filePath, "%s", tempBuffer);
          }
      }
}


//-----------------------------------------------------*/
// buildTileFiles
//
// Description:
//-----------------------------------------------------*/
static void buildTileFiles
              (char                  longTileId,
               char                  latTileId,
               gmsThemeFilesListType *newList)

{
   allocateTileFilePaths (newList);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsEdgeTable,
       &(newList->edgTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsFaceTable,
       &(newList->facTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsTextTable,
       &(newList->txtTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsNodeTable,
       &(newList->endTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsEdgeBoundingRectangle,
       &(newList->ebrTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsFaceBoundingRectangle,
       &(newList->fbrTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsRingTable,
       &(newList->rngTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsEdgeSpatialIndex,
       &(newList->esiTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsFaceSpatialIndex,
       &(newList->fsiTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   constructPrimitiveFilePaths
      (newList->whichTheme,
       gmsNodeSpatialIndex,
       &(newList->nsiTbls),
       longTileId, latTileId,
       longTileId, latTileId);

   //---------------*/
   // Feature files */
   //---------------*/
   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsAreaFeatureTable,
       newList->areaFeatureTbl);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsLineFeatureTable,
       newList->lineFeatureTbl);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsPointFeatureTable,
       newList->pointFeatureTbl);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsTextFeatureTable,
       newList->textFeatureTbl);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsAreaDocTable,
       newList->areaDocTbl);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsLineDocTable,
       newList->lineDocTbl);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsPointDocTable,
       newList->pointDocTbl);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsFeatureClassSchema,
       newList->fcs);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsCharVDT,
       newList->charVDT);

   constructTileThemeFilePaths
      (newList->whichTheme,
       gmsIntVDT,
       newList->intVDT);

   if (newList->areaFeatureTbl[0] == '\0')
      newList->areaFeatureTbl = NULL;

   if (newList->lineFeatureTbl[0] == '\0')
      newList->lineFeatureTbl = NULL;

   if (newList->pointFeatureTbl[0] == '\0')
      newList->pointFeatureTbl = NULL;

   if (newList->textFeatureTbl[0] == '\0')
      newList->textFeatureTbl = NULL;

   if (newList->areaDocTbl[0] == '\0')
      newList->areaDocTbl = NULL;

   if (newList->lineDocTbl[0] == '\0')
      newList->lineDocTbl = NULL;

   if (newList->pointDocTbl[0] == '\0')
      newList->pointDocTbl = NULL;

   if (newList->fcs[0] == '\0')
      newList->fcs = NULL;

   if (newList->charVDT[0] == '\0')
      newList->charVDT = NULL;

   if (newList->intVDT[0] == '\0')
      newList->intVDT = NULL;
}
