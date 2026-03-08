//----------------------------------------------------------------------------*/
// File : tile_ref.c
// Date : 11-Jan-00 : initial defintion
//        12-Feb-00 : add defensive coding
//        25-Feb-00 : add test for tile/theme only list of files
//
// Description:
//    Console application to exercise the functions of the
//    "gmsFileNameUtilities" component.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <gmsUtilities.h>
#include <gmsFileNameUtilities.h>


static const int Num_Tests = 10;

static int getTestSelection ();

static void performTest
               (int whichTest);



//-------------------------------------*/
// main
//
// Description:
//    Main test driver.
//-------------------------------------*/
int main (int  argc,
          char **argv)

{
         int whichTest = 1;

   printf("_______________________________\n");
   printf("Begin : File Name Test\n");

   while ( (whichTest >= 0) && (whichTest <= Num_Tests) )
      {
       whichTest = getTestSelection ();

       performTest (whichTest);
      }

   printf("End : File Name Test\n");
   printf("_______________________________\n");

   return 0;
}

//---------------------------------------------*/
// getTestSelection
//
// Description:
//---------------------------------------------*/
static int getTestSelection ()

{
         static const char *theTests[Num_Tests] =
                               { {"Theme Dir Name"},
                                 {"Subtile Dir"},
                                 {"Regional Files"},
                                 {"Count Theme Files"},
                                 {"Browse Dir Name"},
                                 {"Browse File Name"},
                                 {"Count Browse Files"},
                                 {"Construct List of Theme Files"},
                                 {"Construct List of Tile/Theme Files"},
                                 {"Construct List of Browse Files"} };
         int i;

   printf("________________\n");

   for (i = 0; i < Num_Tests; i++)
      printf("%d) %s\n", i, theTests[i]);

   printf("Which Test :");

   scanf("%d", &i);

   printf("\n");

   return i;
}


//---------------------------------------------*/
// performTest
//
// Description:
//    Accesses and prints each of the files from
//    the tile-ref directory.
//---------------------------------------------*/
static void performTest
               (int whichTest)

{
         char                   *name;
         char                   *theme;
         int                    i, j;
         int                    count;
         gmsThemeFilesListType  *fileList;
         gmsBrowseFilesListType *browseList;

   switch (whichTest)
      {
       case 0:
          {
           printf("_________________\n");
           printf("Theme Dir Name:\n");

           for (i = 0; i < Num_Themes; i++)
              {
               name = gmsGetThemeDirName ( (gmsThematicType) i);

               printf("%d) %s\n", i, name);
              }

           break;
          }

       case 1:
          {
           printf("_________________\n");
           printf("Subtile Dir:\n");

           for (i = 0; i < Num_Subtiles; i++)
              {
               name = gmsGetSubtileDirName ( (gmsSubtileType) i);

               printf("%d) %s\n", i, name);
              }

           break;
          }

       case 2:
          {
           printf("_________________\n");
           printf("Theme Files:\n");

           for (i = 0; i < Num_Themes; i++)
              for (j = 0; j < Num_DCW_Files; j++)
                 {
                  name = gmsGetRegionalFileName
                               ( (gmsThematicType) i,
                                 (gmsDcwFileType) j);

                  printf("%s\n", name);
                 }

           break;
          }

       case 3:
          {
           printf("_________________\n");
           printf("Count Theme Files:\n");

           for (i = 0; i < Num_Themes; i++)
              {
               printf("---***---\n");

               theme = gmsGetThemeDirName ( (gmsThematicType) i);

               for (j = 0; j < Num_DCW_Files; j++)
                  {
                   name = gmsGetRegionalFileName
                                ( (gmsThematicType) i,
                                  (gmsDcwFileType) j);

                   count = gmsCountNumberOfThemeFiles
                                ( (gmsThematicType) i,
                                  (gmsDcwFileType) j);

                   printf("Num of %s files in %s theme = %d\n",
                          name, theme, count);
                  }
              }

           break;
          }

       case 4:
          {
           printf("_________________\n");
           printf("Browse Dir Name:\n");

           for (i = 0; i < Num_Browse_Themes; i++)
              {
               name = gmsGetBrowseDirName
                            ( (gmsBrowseThematicType) i);

               printf("%d) %s\n", i, name);
              }

           break;
          }

       case 5:
          {
           printf("_________________\n");
           printf("Browse File Name:\n");

           for (i = 0; i < Num_Browse_Themes; i++)
              for (j = 0; j < Num_DCW_Files; j++)
                 {
                  name = gmsGetBrowseFileName
                               ( (gmsBrowseThematicType) i,
                                 (gmsDcwFileType) j);

                  printf("%s\n", name);
                 }

           break;
          }

       case 6:
          {
           printf("_________________\n");
           printf("Count Browse Files:\n");

           for (i = 0; i < Num_Browse_Themes; i++)
              {
               theme = gmsGetBrowseDirName
                            ( (gmsBrowseThematicType) i);

               for (j = 0; j < Num_DCW_Files; j++)
                  {
                   name = gmsGetBrowseFileName
                               ( (gmsBrowseThematicType) i,
                                 (gmsDcwFileType) j);

                   count = gmsCountNumberOfBrowseFiles
                                ( (gmsBrowseThematicType) i,
                                  (gmsDcwFileType) j);

                   printf("Num of %s files in %s theme = %d\n",
                          name, theme, count);
                  }
              }

           break;
          }

       case 7:
          {
           printf("_________________\n");
           printf("Construct List of Theme Files:\n");

           for (i = 0; i < Num_Themes; i++)
              {
               name = gmsGetThemeDirName ( (gmsThematicType) i);

               printf("THEME ===> %s\n", name);

               fileList = gmsGetListOfThemeFiles ( (gmsThematicType) i);

               gmsPrintListOfThemeFiles (fileList);

               gmsFreeListOfThemeFiles (&fileList);
              }

           break;
          }

       case 8:
          {
           printf("_________________\n");
           printf("Construct List of Tile/Theme Files:\n");

                 const char Lat_Tile = 'G';
                 const char Long_Tile = 'N';

           for (i = 0; i < Num_Themes; i++)
              {
               name = gmsGetThemeDirName ( (gmsThematicType) i);

               printf("THEME ===> %s\n", name);

               fileList = gmsGetListOfTileFiles
                             ((gmsThematicType) i,
                              Long_Tile,
                              Lat_Tile);

               gmsPrintListOfThemeFiles (fileList);

               gmsFreeListOfThemeFiles (&fileList);
              }

           break;
          }

       case 9:
          {
           printf("_________________\n");
           printf("Construct List of Browse Files:\n");

           for (i = 0; i < Num_Browse_Themes; i++)
              {
               name = gmsGetBrowseDirName ( (gmsBrowseThematicType) i);

               if (name == NULL)
                  continue;

               printf("BROWSE DIR ===> %s\n", name);

               browseList = gmsGetListOfBrowseFiles
                               ( (gmsBrowseThematicType) i);

               gmsPrintListOfBrowseFiles (browseList);

               gmsFreeListOfBrowseFiles (&browseList);
              }

           break;
          }

       default:
          {
           printf("No test selected\n");

           break;
          }
      }
}
