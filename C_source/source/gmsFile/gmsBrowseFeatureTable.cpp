/*----------------------------------------------------------------------------*/
/* File : gmsBrowseFeatureTable.cpp
/* Date : 07-Sep-99 : initial definition
/*        05-Oct-99 : Clean-up due to code inspection
/*        17-Nov-99 : get feature tbl routine figures out name of index file
/*
/* Description:
/*    Utilities to read/process any "Feature Table" file from the BROWSE area
/*    of the Digital Chart of the World (DCW) database.  Consider the
/*    following defintions:
/*
/*       Feature:
/*       --------
/*          A model of a real world geographic entity.  [It is] a zero-,
/*          one-, or two-dimensional entity of uniform attribute scheme
/*          from an exhaustive attribute distribution across a plane,
/*          or a set of such entities sharing common attribute values.
/*          The three subtypes are simple features, complex features,
/*          and text features.  The types of simple features are point
/*          features, line features, and area features.
/*
/*       Feature Class:
/*       --------------
/*          A set of features that shares a homogeneous set of attributes.
/*          A feature class consists of a set of tables that includes one
/*          or more primitive tables and one or more attribute tables.
/*          A feature class has the same columns of attribute information
/*          for each feature.  Every feature class has one and only one
/*          feature table.  The two types of feature classes are the simple
/*          feature class and the complex feature class.  The subtypes of
/*          the simple feature class are the "point", "line", "area", and
/*          "text" feature classes.
/*
/*    OK, so there's some inconsistency in their (NIMA) definitions.  In general,
/*    there are 4 types of "feature" tables managed by this component.  They
/*    are:
/*
/*                   area  (*.AFT files)
/*                   line  (*.LFT files)
/*                   point (*.PFT files)
/*                   text  (*.TFT files)
/*
/*    The table below maps "feature-tables" to "browse-libraries".  A "y"
/*    means a feature table exists for that browse directory.
/*
/*    ___________________________________________________________________
/*    |                           |          Feature Table              |
/*    |      Browse Theme         |-------------------------------------|
/*    |                           | *.AFT  |  *.LFT  |  *.PFT  |  *.TFT | 
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | ONC Compilation Date (CO) |   y    |    n    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Avail of Hypsog Data (DA) |   y    |    n    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Drainage (DA)             |   y    |    y    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Data Volumes (DV)         |   y    |    n    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Geographic Regions (GR)   |   y    |    y    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | ONC Index (IN)            |   y    |    n    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Library Ref (Libref)      |   n    |    n    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Political/Oceans (PO)     |   y    |    y    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Populated Places (PP)     |   n    |    n    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsBrowseFeatureTable.h>
#include <gmsIndexFile.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*-----------------------------*/
/*     Local Variables
/*-----------------------------*/
static FILE *feature_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms
/*-----------------------------*/
static void buildBrowseLineOrAreaFeatureTable
               (gmsBrowseThematicType  whichTheme,
                browseFeatureTableType *theTbl);

static void buildBrowsePointFeatureTable
               (browseFeatureTableType *theTbl);

static void buildBrowseTextFeatureTable
               (browseFeatureTableType *theTbl,
                indexTableType         *theTFX);

static void printGRFeatureTable
              (int                    numRecords,
               browseGrFeatureRecType *buffer);

static void printINAreaFeatureTable
               (int                        numRecords,
                browseInAreaFeatureRecType *buffer);

static void printBaseFeatureTable
               (int                  numRecords,
                browseFeatureRecType *buffer);

static void printPointFeatureTable
               (int                      numRecords,
                browsePointFeatureRecType *buffer);

static void printTextFeatureTable
               (int                      numRecords,
                browseTextFeatureRecType *buffer);


/*---------------------------------------------*/
/* gmsGetBrowseFeatureTable
/*
/* Description:
/*    This utility reads the specified "feature
/*    table" and builds a corresponding data
/*    structure.  The kind of data-structure is
/*    specified by the parameter "whichTheme".
/*    A pointer to a newly allocated table is
/*    returned to the caller.  The caller is then
/*    able to type-cast the "void *" to the
/*    corresponding data-structure.
/*
/*    Note:
/*       If the file to be read is a "text-feature
/*       table, then the name of the "text-index"
/*       file ("*.tfx") must be specified.  If the
/*       file is not a "text-feature" table, then
/*       (simply) pass a null string.
/*
/*    It is the caller's responsibility to free
/*    the item by using the utility
/*    "gmsFreeBrowseFeatureTable" (see below).
/*---------------------------------------------*/
browseFeatureTableType *gmsGetBrowseFeatureTable
                           (gmsBrowseThematicType whichTheme,
                            const char            *featureFilePath,
                            gmsBrowseFeatureType  whichFeature)

{
         const char             readOnlyMode[] = { "rb" };
         browseFeatureTableType *featureTbl;
         indexTableType         *theTFX = NULL;
         int                    lenOfString;
         char                   textIndexFilePath[1024];

   if (featureFilePath == NULL)
      return (browseFeatureTableType *) NULL;

   feature_fd = fopen(featureFilePath,
                      readOnlyMode);

   if (feature_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open 'feature' file: %s\n",
              featureFilePath);

       return (browseFeatureTableType *) NULL;
      }

   featureTbl = (browseFeatureTableType *)
                malloc (sizeof(browseFeatureTableType));

   if ( (whichFeature == gmsBrowseArea) ||
        (whichFeature == gmsBrowseLine) )
      {
       buildBrowseLineOrAreaFeatureTable
               (whichTheme,
                featureTbl);
      }

   else if (whichFeature == gmsBrowsePoint)
      {
       buildBrowsePointFeatureTable (featureTbl);
      }

   else if (whichFeature == gmsBrowseText)
      {
       lenOfString = strlen (featureFilePath);

       sprintf (textIndexFilePath, "%s", featureFilePath);

       lenOfString--;

       if (textIndexFilePath[lenOfString] == 't')
          textIndexFilePath[lenOfString] = 'x';
       else
          textIndexFilePath[lenOfString] = 'X';

       theTFX = gmsGetIndexFile (textIndexFilePath);

       if (theTFX == NULL)
          {
           printf("---> ERROR : 'BROWSE TXT FEATURE INDEX' file: %s is NULL\n",
                  textIndexFilePath);

           fclose (feature_fd);

           return (browseFeatureTableType *) NULL;
          }

       buildBrowseTextFeatureTable
               (featureTbl,
                theTFX);

       gmsFreeIndexFile (theTFX);
      }

   else
      {
       printf("*** ERROR : unknown feature for BROWSE feature-table.\n");
      }

   fclose (feature_fd);

   featureTbl->browseTheme = whichTheme;

   featureTbl->browseFeature = whichFeature;

   return featureTbl;
}


/*---------------------------------------------*/
/* gmsFreeBrowseFeatureTable
/*
/* Description:
/*    This utility frees a "Feature Table" that had
/*    been previously allocated using
/*    "gmsGetFeatureTable".
/*---------------------------------------------*/
void gmsFreeBrowseFeatureTable
           (browseFeatureTableType *theFeatureTable)

{
   if (theFeatureTable == NULL)
      return;

   if (theFeatureTable->featureRecords != NULL)
      free (theFeatureTable->featureRecords);

   free (theFeatureTable);
}


/*---------------------------------------------*/
/* gmsPrintBrowseFeatureTable	
/*
/* Description:
/*    This function will print the "Feature Table"
/*    object to standard out.
/*---------------------------------------------*/
void gmsPrintBrowseFeatureTable
           (browseFeatureTableType *theFeatureTable)

{
         static const char *themeLookupTable[Num_Browse_Themes] =
                              { "CO - ONC Compilation Date",
                                "DV - Data Volumes",
                                "DN - Drainage",
                                "GR - Geographic Regions",
                                "DA - Avail of Hypsographic Data",
                                "IN - ONC Index",
                                "PO - Political/Oceans",
                                "PP - Populated Places",
                                "Library Reference" };

         static const char *featureLookupTable[Num_Browse_Features] =
                              { "Area",
                                "Point",
                                "Line",
                                "Text" };

   printf("________________________________________\n");
   printf("Feature Table:\n");

   if (theFeatureTable == NULL)
      return;

   printf("Num Records : %d\n", theFeatureTable->numRecords);

   if ((theFeatureTable->browseTheme >= 0) &&
       (theFeatureTable->browseTheme <= Num_Browse_Themes))
      {
       printf("Theme       : %s\n",
              themeLookupTable[theFeatureTable->browseTheme]);
      }

   if ((theFeatureTable->browseFeature >= 0) &&
       (theFeatureTable->browseFeature <= Num_Browse_Features))
      {
       printf("Feature     : %s\n",
          featureLookupTable[theFeatureTable->browseFeature]);
      }

   if (theFeatureTable->featureRecords == NULL)
      return;

   if ( (theFeatureTable->browseFeature == gmsBrowseArea) ||
        (theFeatureTable->browseFeature == gmsBrowseLine) )
      {
       if (theFeatureTable->browseTheme == gmsBrowse_GR)
          {
           printGRFeatureTable
              (theFeatureTable->numRecords,
               (browseGrFeatureRecType *) theFeatureTable->featureRecords);
          }
       else if ( (theFeatureTable->browseFeature == gmsBrowseArea) &&
                 (theFeatureTable->browseTheme == gmsBrowse_IN) )
          {
           printINAreaFeatureTable
              (theFeatureTable->numRecords,
               (browseInAreaFeatureRecType *) theFeatureTable->featureRecords);
          }
       else
          {
           printBaseFeatureTable
              (theFeatureTable->numRecords,
               (browseFeatureRecType *) theFeatureTable->featureRecords);
          }
      }

   else if (theFeatureTable->browseFeature == gmsBrowsePoint)
      {
       printPointFeatureTable
          (theFeatureTable->numRecords,
           (browsePointFeatureRecType *) theFeatureTable->featureRecords);
      }

   else if (theFeatureTable->browseFeature == gmsBrowseText)
      {
       printTextFeatureTable
          (theFeatureTable->numRecords,
           (browseTextFeatureRecType *) theFeatureTable->featureRecords);
      }

   else
      {
       printf("*** ERROR : Unknown BROWSE feature - unable to print\n");
      }
}


     /*-----------------------*/
     /*   Local Subprograms   */
     /*-----------------------*/


/*---------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the "Feature Table"
/*    file.
/*---------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (feature_fd);

   tempChar = fgetc (feature_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (feature_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (feature_fd);
      }
}


/*---------------------------------------------*/
/* getSizeOfFile
/*
/* Description:
/*    This function determines the number of
/*    bytes of data in the file AFTER the header.
/*    The file-descriptor is returned to the top
/*    of the file after this routine completes.
/*---------------------------------------------*/
static int getSizeOfFile ()

{
         int tempChar;
         int numBytes = 0;

   rewind (feature_fd);

   readPastFormatInformation ();

   tempChar = fgetc (feature_fd);

   while (tempChar != EOF)
      {
       numBytes++;

       tempChar = fgetc (feature_fd);
      }

   rewind (feature_fd);

   return numBytes;
}


/*---------------------------------------------*/
/* buildBrowseLineOrAreaFeatureTable 
/*
/* Description:
/*    This routin is used to build either a
/*    "area" or "line" featue table.
/*---------------------------------------------*/
static void buildBrowseLineOrAreaFeatureTable
               (gmsBrowseThematicType  whichTheme,
                browseFeatureTableType *theTbl)

{
         int                        numBytes   = 0;
         int                        numRecords = 0;
         int                        i;
         browseGrFeatureRecType     *GR_buffer;
         browseInAreaFeatureRecType *IN_buffer;
         browseFeatureRecType       *buffer;

   numBytes = getSizeOfFile ();

   if (whichTheme == gmsBrowse_GR)
      {
       numRecords = numBytes / Size_Of_Browse_GR_Feature_Record;

       theTbl->numRecords = numRecords;

       numBytes = numRecords * sizeof(browseGrFeatureRecType);

       theTbl->featureRecords = (void *) malloc (numBytes);

       if (theTbl->featureRecords == NULL)
          {
           printf("ERROR : malloc of GR Browse Feature Tbl failed.\n");

           return;
          }

       GR_buffer = (browseGrFeatureRecType *) theTbl->featureRecords;

       gmsClearMemory
          ( (char *) GR_buffer,
            numBytes);

       rewind (feature_fd);

       readPastFormatInformation ();

       /*----------------------------------*/
       /* Note:
       /*    This is used to build either a
       /*    LINE or AREA (GR) feature table.
       /*----------------------------------*/
       for (i = 0; i < numRecords; i++)
          {
           GR_buffer[i].ID = gmsReadInteger (feature_fd);

           GR_buffer[i].region_1 = (char) fgetc (feature_fd);

           GR_buffer[i].region_2 = (char) fgetc (feature_fd);

           GR_buffer[i].region_3 = (char) fgetc (feature_fd);

           GR_buffer[i].region_4 = (char) fgetc (feature_fd);
          }
      }

   else if (whichTheme == gmsBrowse_IN)
      {
       numRecords = numBytes / Size_Of_Browse_IN_Area_Feature_Record;

       theTbl->numRecords = numRecords;

       numBytes = numRecords * sizeof(browseInAreaFeatureRecType);

       theTbl->featureRecords = (void *) malloc (numBytes);

       if (theTbl->featureRecords == NULL)
          {
           printf("ERROR : malloc of IN browse Feature Tbl failed.\n");

           return;
          }

       IN_buffer = (browseInAreaFeatureRecType *) theTbl->featureRecords;

       gmsClearMemory
          ( (char *) IN_buffer,
            numBytes);

       rewind (feature_fd);

       readPastFormatInformation ();

       for (i = 0; i < numRecords; i++)
          {
           IN_buffer[i].ID = gmsReadInteger (feature_fd);

           fgets
              (IN_buffer[i].ONC_SheetId,
               sizeof(IN_buffer[i].ONC_SheetId),
               feature_fd);
          }
      }

   else
      {
       numRecords = numBytes / Size_Of_Browse_Feature_Record;

       theTbl->numRecords = numRecords;

       numBytes = numRecords * sizeof(browseFeatureRecType);

       theTbl->featureRecords = (void *) malloc (numBytes);

       if (theTbl->featureRecords == NULL)
          {
           printf("ERROR : malloc of Browse Feature Tbl failed.\n");

           return;
          }

       buffer = (browseFeatureRecType *) theTbl->featureRecords;

       gmsClearMemory
          ( (char *) buffer,
            numBytes);

       rewind (feature_fd);

       readPastFormatInformation ();

       for (i = 0; i < numRecords; i++)
          {
           buffer[i].ID    = gmsReadInteger (feature_fd);

           buffer[i].value = gmsReadInteger (feature_fd);
          }
      }
}


/*---------------------------------------------*/
/* buildBrowsePointFeatureTable 
/*
/* Description:
/*    This utility constructs a POINT feature
/*    table from the browse library.
/*---------------------------------------------*/
static void buildBrowsePointFeatureTable
               (browseFeatureTableType *theTbl)

{
         int                       numBytes   = 0;
         int                       numRecords = 0;
         int                       i;
         browsePointFeatureRecType *pointBuffer;

   numBytes = getSizeOfFile ();

   numRecords = numBytes / Size_Of_Browse_Point_Record;

   theTbl->numRecords = numRecords;

   numBytes = numRecords * sizeof(browsePointFeatureRecType);

   theTbl->featureRecords = (void *) malloc (numBytes);

   pointBuffer = (browsePointFeatureRecType *) theTbl->featureRecords;

   gmsClearMemory
          ( (char *) pointBuffer,
            numBytes);

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < numRecords; i++)
      {
       pointBuffer[i].ID   = gmsReadInteger (feature_fd);

       pointBuffer[i].type = gmsReadInteger (feature_fd);

       fgets
          (pointBuffer[i].name,
           sizeof(pointBuffer[i].name),
           feature_fd);
      }
}


/*---------------------------------------------*/
/* buildBrowseTextFeatureTable
/*
/* Description:
/*    This utility is used to construct any of
/*    the TEXT feature tables from the BROWSE
/*    library.
/*---------------------------------------------*/
static void buildBrowseTextFeatureTable
               (browseFeatureTableType *theTbl,
                indexTableType         *theTFX)

{
         int                      numBytes   = 0;
         int                      numRecords = 0;
         int                      i;
         int                      numChars;
         browseTextFeatureRecType *textBuffer;

   numRecords = theTFX->numberRecordsInTable;

   theTbl->numRecords = numRecords;

   numBytes = numRecords * sizeof(browseTextFeatureRecType);

   theTbl->featureRecords = (void *) malloc (numBytes);

   textBuffer = (browseTextFeatureRecType *) theTbl->featureRecords;

   gmsClearMemory
      ((char *) textBuffer,
       numBytes);

   fseek
      (feature_fd,
       theTFX->indexList[0].byteOffset,
       SEEK_SET);

   for (i = 0; i < numRecords; i++)
     {
      textBuffer[i].ID           = gmsReadInteger (feature_fd);

      textBuffer[i].kindOfText   = gmsReadInteger (feature_fd);

      textBuffer[i].heightOfText = gmsReadFloat (feature_fd);

      numChars = gmsReadInteger (feature_fd);

      if (numChars > NUM_CHARS)
         {
          printf("*** ERROR : %d is too many chars in text feature table\n",
                 numChars);

          return;
         }

      fgets
         (textBuffer[i].theText,
          numChars + 1,
          feature_fd);

      textBuffer[i].textGap = gmsReadFloat (feature_fd);

      textBuffer[i].symbol  = gmsReadInteger (feature_fd);
     }
}


     /*-----------------------*/
     /*   Print Utilities     */
     /*-----------------------*/


/*---------------------------------------------*/
/* printGRFeatureTable
/*
/* Description:
/*    This function will print the specified
/*    browse "GR feature records" to stdout.
/*    This utility is used for both LINE and
/*    AREA GR feature tables.
/*---------------------------------------------*/
static void printGRFeatureTable
              (int                    numRecords,
               browseGrFeatureRecType *buffer)

{
         int i;

   printf("               Regions\n");
   printf("       -------------------------------\n");
   printf(" ID    |    1   |    2    |    3  |  4\n");
   printf("--------------------------------------\n");

   if (buffer == NULL)
      return;

   for (i = 0; i < numRecords; i++)
      {
       printf("%d    %c          %c          %c         %c\n",
              buffer[i].ID,
              buffer[i].region_1,
              buffer[i].region_2,
              buffer[i].region_3,
              buffer[i].region_4);
      }
}


/*---------------------------------------------*/
/* printINAreaFeatureTable 
/*
/* Description:
/*    This function will print the specified
/*    browse "IN feature records" to stdout.
/*---------------------------------------------*/
static void printINAreaFeatureTable
              (int                        numRecords,
               browseInAreaFeatureRecType *buffer)

{
         int i;

   printf("ID  ONC-Sheet-ID\n");
   printf("----------------\n");

   if (buffer == NULL)
      return;

   for (i = 0; i < numRecords; i++)
      {
       printf("%d     %s\n",
              buffer[i].ID,
              buffer[i].ONC_SheetId);
      }
}


/*---------------------------------------------*/
/* printBaseFeatureTable 
/*
/* Description:
/*    This function will print the specified
/*    browse "feature records" (generic) to stdout.
/*---------------------------------------------*/
static void printBaseFeatureTable
              (int                  numRecords,
               browseFeatureRecType *buffer)

{
         int i;

   printf("ID \t Value\n");
   printf("-----------\n");

   if (buffer == NULL)
      return;

   for (i = 0; i < numRecords; i++)
      {
       printf("%d \t %d\n", buffer[i].ID, buffer[i].value);
      }
}


/*---------------------------------------------*/
/* printPointFeatureTable 
/*
/* Description:
/*    This function will print the specified
/*    browse "Point feature records" to stdout.
/*---------------------------------------------*/
static void printPointFeatureTable
          (int                       numRecords,
           browsePointFeatureRecType *buffer)

{
         int i;

   printf("ID \t Type \t Name\n");
   printf("---------------------------------------\n");

   if (buffer == NULL)
      return;

   for (i = 0; i < numRecords; i++)
      {
       printf("%d \t %d\t %s\n",
              buffer[i].ID,
              buffer[i].type,
              buffer[i].name);
      }
}


/*---------------------------------------------*/
/* printTextFeatureTable
/*
/* Description:
/*    This function will print the specified
/*    browse "Text feature records" to stdout.
/*---------------------------------------------*/
static void printTextFeatureTable
          (int                      numRecords,
           browseTextFeatureRecType *buffer)

{
         int i;

   printf("ID \t Kind \t Height \t Text \t\t Gap \t Symbol\n");
   printf("---------------------------------------------------------------\n");

   if (buffer == NULL)
      return;

   for (i = 0; i < numRecords; i++)
      {
       printf("%d \t %d\t %f \t %s  %f \t %d\n",
              buffer[i].ID,
              buffer[i].kindOfText,
              buffer[i].heightOfText,
              buffer[i].theText,
              buffer[i].textGap,
              buffer[i].symbol);
      }
}

