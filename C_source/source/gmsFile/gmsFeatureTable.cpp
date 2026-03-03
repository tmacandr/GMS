/*----------------------------------------------------------------------------*/
/* File : gmsFeatureTable.c
/* Date : 25-Aug-99 : initial definition
/*        11-Sep-99 : major surgery to the interface (re : Mil-D-89009)
/*        07-Oct-99 : Clean-up due to code-inspection
/*        17-Nov-99 : get feature tbl routine figures out index file
/*
/* Description:
/*    Utilities to read/process any "Feature Table" file (except from BROWSE)
/*    of the Digital Chart of the World (DCW) database.  Consider the following
/*    definitions:
/*
/*       Feature:
/*       --------
/*          A model of a real world geographic entity.  [It is a] zero-,
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
/*    Ok, so there's some inconsistency in their (NIMA) defintions.  In
/*    general, there are 4 types of "feature" tables managed by this
/*    component.  They are:
/*
/*                   area  (*.AFT files)
/*                   line  (*.LFT files)
/*                   point (*.PFT files)
/*                   text  (*.TFT files)
/*
/*    The table below maps "feature-tables" to "themes".  A "y" means a
/*    feature table exists for that theme (i.e. directory).
/*
/*    ___________________________________________________________________
/*    |                           |          Feature Table              |
/*    |      DCW Theme            |-------------------------------------|
/*    |                           | *.AFT  |  *.LFT  |  *.PFT  |  *.TFT | 
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Aeronautical (AE)         |   n    |    n    |   y     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Cultural Landmarks (CL)   |   y    |    y    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Data Quality (DQ)         |   y    |    y    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Drainage (DN)             |   y    |    y    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | DN - Supplemental (DS)    |   n    |    n    |   y     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Hypsography (HY)          |   y    |    n    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | HY - Supplemental (HS)    |   n    |    y    |   y     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Land Cover (LC)           |   y    |    n    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Ocean Features (OF)       |   n    |    y    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Physiography (PH)         |   n    |    y    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Political/Oceans (PO)     |   y    |    y    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Populated Places (PP)     |   y    |    n    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Railroads (RR)            |   n    |    y    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Roads (RD)                |   n    |    y    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Transportation Struct (TS)|   n    |    y    |   y     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Utilites (UT)             |   n    |    y    |   n     |   y    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Vegetation (VG)           |   y    |    n    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Gazeteer                  |   n    |    n    |   y     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Tile Reference            |   y    |    n    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*    |                           |        |         |         |        |
/*    | Library Reference         |   n    |    y    |   n     |   n    |
/*    |___________________________|________|_________|_________|________|
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsFeatureTable.h>
#include <gmsIndexFile.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*-----------------------------*/
/*     Local Constants         */
/*-----------------------------*/
#define Size_Of_AE_Point_Record 99 /* ... if there were no unused bytes */
#define Size_Of_CL_Point_Record 60
#define Size_Of_PO_Point_Record 18
#define Size_Of_PP_Point_Record 58
#define Size_Of_TS_Point_Record 18
#define Size_Of_Gazetteer_Point_Record 94
#define Size_Of_HY_Point_Record        18
#define Size_Of_General_Point_Record   14
#define Size_Of_CL_Area_Record         60
#define Size_Of_DQ_Area_Record         124
#define Size_Of_PO_Area_Record         58
#define Size_Of_Tile_Ref_Area_Record   10
#define Size_Of_General_Area_Record    58
#define Size_Of_CL_Line_Record         60
#define Size_Of_DQ_Line_Record         20
#define Size_Of_OF_PH_UT_Line_Record   14
#define Size_Of_General_Line_Record    18


/*-----------------------------*/
/*     Local Variables         */
/*-----------------------------*/
static FILE *feature_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms
/*-----------------------------*/
static void buildAreaFeatureTable
               (gmsThematicType  whichTheme,
                featureTableType *theTbl);

static void buildPointFeatureTable
               (gmsThematicType  whichTheme,
                featureTableType *theTbl);

static void buildLineFeatureTable
               (gmsThematicType  whichTheme,
                featureTableType *theTbl);

static void buildTextFeatureTable
               (featureTableType *theTbl,
                indexTableType   *theTFX);

static void build_AE_PointFeatureTable
               (featureTableType *theTbl);

static void build_CL_PointFeatureTable
               (featureTableType *theTbl);

static void build_PO_PointFeatureTable
               (featureTableType *theTbl);

static void build_PP_PointFeatureTable
               (featureTableType *theTbl);

static void build_TS_PointFeatureTable
               (featureTableType *theTbl);

static void buildGazetteerPointFeatureTable
               (featureTableType *theTbl);

static void build_HY_PointFeatureTable
               (featureTableType *theTbl);

static void buildGeneralPointFeatureTable
               (featureTableType *theTbl);

static void build_CL_AreaFeatureTable
               (featureTableType *theTbl);

static void build_DQ_AreaFeatureTable
               (featureTableType *theTbl);

static void build_PO_AreaFeatureTable
               (featureTableType *theTbl);

static void build_TileRef_AreaFeatureTable
               (featureTableType *theTbl);

static void buildGeneralAreaFeatureTable
               (featureTableType *theTbl);

static void build_CL_LineFeatureTable
               (featureTableType *theTbl);

static void build_DQ_LineFeatureTable
               (featureTableType *theTbl);

static void build_OF_PH_UT_LineFeatureTable
               (featureTableType *theTbl);

static void buildGeneralLineFeatureTable
               (featureTableType *theTbl);

      /*-----------------------*/
      /* Print utilities       */
      /*-----------------------*/

static void printTextFeatureTable
               (int                numRecs,
                textFeatureRecType *theTbl);

static void print_AE_PointFeatureTable
               (int                    numRecs,
                AE_pointFeatureRecType *theTbl);

static void print_CL_PointFeatureTable
               (int                    numRecs,
                CL_pointFeatureRecType *theTbl);

static void print_PO_PointFeatureTable
               (int                    numRecs,
                PO_pointFeatureRecType *theTbl);

static void print_PP_PointFeatureTable
               (int                    numRecs,
                PP_pointFeatureRecType *theTbl);

static void print_TS_PointFeatureTable
               (int                    numRecs,
                TS_pointFeatureRecType *theTbl);

static void printGazetteerPointFeatureTable
               (int                          numRecs,
                gazetteerPointFeatureRecType *theTbl);

static void print_HY_PointFeatureTable
               (int                    numRecs,
                HY_pointFeatureRecType *theTbl);

static void printGeneralPointFeatureTable
               (int                        numRecs,
                generalPointFeatureRecType *theTbl);

static void print_CL_AreaFeatureTable
               (int                   numRecs,
                CL_AreaFeatureRecType *theTbl);

static void print_DQ_AreaFeatureTable
               (int                   numRecs,
                DQ_areaFeatureRecType *theTbl);

static void print_PO_AreaFeatureTable
               (int                   numRecs,
                PO_areaFeatureRecType *theTbl);

static void print_TileRef_AreaFeatureTable
               (int                        numRecs,
                tileRef_areaFeatureRecType *theTbl);

static void printGeneralAreaFeatureTable
               (int                        numRecs,
                generalAreaFeatureRecType *theTbl);

static void print_CL_LineFeatureTable
               (int                   numRecs,
                CL_lineFeatureRecType *theTbl);

static void print_DQ_LineFeatureTable
               (int                   numRecs,
                DQ_lineFeatureRecType *theTbl);

static void print_OF_PH_UT_LineFeatureTable
               (int                         numRecs,
                OF_PH_UT_lineFeatureRecType *theTbl);

static void printGeneralLineFeatureTable
               (int                       numRecs,
                generalLineFeatureRecType *theTbl);


/*---------------------------------------------*/
/* gmsGetFeatureTable
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
/*       file ("*.tfx") is determined by this
/*       routine.
/*
/*    It is the caller's responsibility to free
/*    the item by using the utility
/*    "gmsFreeFeatureTable" (see below).
/*---------------------------------------------*/
featureTableType *gmsGetFeatureTable
                           (gmsThematicType whichTheme,
                            const char      *featureFilePath,
                            gmsFeatureType  whichFeature)

{
         const char       readOnlyMode[] = { "rb" };
         featureTableType *theFeatureTbl;
         indexTableType   *theTFX = NULL;
         int              lenString;
         char             textIndexFilePath[1024];

   if (featureFilePath == NULL)
      return (featureTableType *) NULL;

   feature_fd = fopen(featureFilePath,
                      readOnlyMode);

   if (feature_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open 'feature' file: %s\n",
              featureFilePath);

       return (featureTableType *) NULL;
      }

   theFeatureTbl = (featureTableType *) malloc (sizeof(featureTableType));

   gmsClearMemory
      ( (char *) theFeatureTbl,
        sizeof(featureTableType) );

   switch (whichFeature)
      {
       case gmsArea:
          {
           buildAreaFeatureTable
               (whichTheme,
                theFeatureTbl);

           break;
          }

       case gmsPoint:
          {
           buildPointFeatureTable
               (whichTheme,
                theFeatureTbl);

           break;
          }

       case gmsLine:
          {
           buildLineFeatureTable
               (whichTheme,
                theFeatureTbl);

           break;
          }

       case gmsText:
          {
           lenString = strlen (featureFilePath);

           sprintf(textIndexFilePath, "%s", featureFilePath);

           lenString--;

           if (featureFilePath[lenString] == 'T')

              textIndexFilePath[lenString] = 'X';

           else

              textIndexFilePath[lenString] = 'x';

           theTFX = gmsGetIndexFile (textIndexFilePath);

           if (theTFX == NULL)
              {
               printf("---> ERROR : 'TEXT FEATURE INDEX': %s is NULL\n",
                      textIndexFilePath);

               fclose (feature_fd);

               return (featureTableType *) NULL;
              }

           buildTextFeatureTable
              (theFeatureTbl,
               theTFX);

           gmsFreeIndexFile (theTFX);

           break;
          }

       default:
          {
           printf("*** ERROR : unknown feature specified for table.\n");

           break;
          }
      }

   fclose (feature_fd);

   theFeatureTbl->whichTheme = whichTheme;

   theFeatureTbl->whichFeature = whichFeature;

   return theFeatureTbl;
}


/*---------------------------------------------*/
/* gmsFreeFeatureTable
/*
/* Description:
/*    This utility frees a "Feature Table" that had
/*    been previously allocated using
/*    "gmsGetFeatureTable".
/*---------------------------------------------*/
void gmsFreeFeatureTable
           (featureTableType *theFeatureTable)

{
   if (theFeatureTable == NULL)
      return;

   if (theFeatureTable->featureRecords != NULL)
      free (theFeatureTable->featureRecords);

   free (theFeatureTable);
}


/*---------------------------------------------*/
/* gmsPrintFeatureTable	
/*
/* Description:
/*    This function will print the "Feature Table"
/*    object to standard out.
/*---------------------------------------------*/
void gmsPrintFeatureTable
           (featureTableType *theTbl)

{
         static const char *themeLookupTable[Num_Themes] =
                              { "AE - Aeronautical Features",
                                "CL - Cultural Landmarks",
                                "DQ - Data Quality",
                                "DN - Drainage",
                                "DS - Drainage (supplemental)",
                                "HY - Hypsographic",
                                "HS - Hypsographic (supplemental)",
                                "LC - Land Coverage",
                                "OF - Ocean Features",
                                "PH - Physiography",
                                "PO - Political/Oceans",
                                "PP - Populated Places",
                                "RR - Railroads",
                                "RD - Roads",
                                "TS - Transportation Structures",
                                "UT - Utilities",
                                "VG - Vegetation",
                                "Gazetteer",
                                "Tile Ref",
                                "Library Reference" };

         static const char *featureLookupTable[Num_Features] =
                              { "Area",
                                "Point",
                                "Line",
                                "Text" };

   printf("________________________________________\n");
   printf("Feature Table:\n");

   if (theTbl == NULL)
      return;

   printf("Num Records : %d\n",
          theTbl->numRecords);

   if (theTbl->numRecords <= 0)
      return;

   if (theTbl->featureRecords == NULL)
      return;

   if ((theTbl->whichTheme >= 0) &&
       (theTbl->whichTheme <= Num_Themes) )
      {
       printf("Theme       : %s\n",
              themeLookupTable[theTbl->whichTheme]);
      }

   if ((theTbl->whichFeature >= 0) &&
       (theTbl->whichFeature <= Num_Features) )
      {
       printf("Feature     : %s\n",
          featureLookupTable[theTbl->whichFeature]);
      }

   if (theTbl->whichFeature == gmsText)
      {
       printTextFeatureTable
               (theTbl->numRecords,
                (textFeatureRecType *) theTbl->featureRecords);
      }

   else if (theTbl->whichFeature == gmsPoint)
      {
       switch (theTbl->whichTheme)
          {
           case gms_AE:
              {
               print_AE_PointFeatureTable
                  (theTbl->numRecords,
                   (AE_pointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_CL:
              {
               print_CL_PointFeatureTable
                  (theTbl->numRecords,
                   (CL_pointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_PO:
              {
               print_PO_PointFeatureTable
                  (theTbl->numRecords,
                   (PO_pointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_PP:
              {
               print_PP_PointFeatureTable
                  (theTbl->numRecords,
                   (PP_pointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_TS:
              {
               print_TS_PointFeatureTable
                  (theTbl->numRecords,
                   (TS_pointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_Gazetteer:
              {
               printGazetteerPointFeatureTable
                  (theTbl->numRecords,
                   (gazetteerPointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_HY:
              {
               print_HY_PointFeatureTable
                  (theTbl->numRecords,
                   (HY_pointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_DN:
           case gms_DS:
           case gms_HS:
           case gms_LC:
           case gms_OF:
              {
               printGeneralPointFeatureTable
                  (theTbl->numRecords,
                   (generalPointFeatureRecType *) theTbl->featureRecords);

               break;
              }

           default:
              {
               printf("*** ERROR : unknown theme for Point Feature Table\n");;

               break;
              }
          } /* end switch */
      }

   else if (theTbl->whichFeature == gmsArea)
      {
       switch (theTbl->whichTheme)
          {
           case gms_CL:
              {
               print_CL_AreaFeatureTable
                  (theTbl->numRecords,
                   (CL_AreaFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_DQ:
              {
               print_DQ_AreaFeatureTable
                  (theTbl->numRecords,
                   (DQ_areaFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_PO:
              {
               print_PO_AreaFeatureTable
                  (theTbl->numRecords,
                   (PO_areaFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_TileRef:
              {
               print_TileRef_AreaFeatureTable
                  (theTbl->numRecords,
                   (tileRef_areaFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_DN:
           case gms_HY:
           case gms_LC:
           case gms_PP:
           case gms_VG:
              {
               printGeneralAreaFeatureTable
                  (theTbl->numRecords,
                   (generalAreaFeatureRecType *) theTbl->featureRecords);

               break;
              }

           default:
              {
               printf("*** ERROR : unknown theme for Area Feature Table\n");;

               break;
              }
          } /* end switch */
      }

   else if (theTbl->whichFeature == gmsLine)
      {
       switch (theTbl->whichTheme)
          {
           case gms_CL:
              {
               print_CL_LineFeatureTable
                  (theTbl->numRecords,
                   (CL_lineFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_DQ:
              {
               print_DQ_LineFeatureTable
                  (theTbl->numRecords,
                   (DQ_lineFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_OF:
           case gms_PH:
           case gms_UT:
              {
               print_OF_PH_UT_LineFeatureTable
                  (theTbl->numRecords,
                   (OF_PH_UT_lineFeatureRecType *) theTbl->featureRecords);

               break;
              }

           case gms_DN:
           case gms_HY:
           case gms_HS:
           case gms_PO:
           case gms_RR:
           case gms_RD:
           case gms_TS:
              {
               printGeneralLineFeatureTable
                  (theTbl->numRecords,
                   (generalLineFeatureRecType *) theTbl->featureRecords);

               break;
              }

           default:
              {
               printf("*** ERROR : unknown theme for Line Feature Table\n");;

               break;
              }
          } /* end switch */
      }
}


     /*-----------------------*/
     /*   Local Subprograms
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

   tempChar = gmsReadInteger (feature_fd);

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
/* getNumBytesOfData
/*
/* Description:
/*    This function determines the number of
/*    bytes of data in the file AFTER the header.
/*    The file-descriptor is returned to the top
/*    of the file after this routine completes.
/*---------------------------------------------*/
static int getNumBytesOfData ()

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
/* allocateBufferForFeatureTable
/*
/* Description:
/*    This utility allocates a buffer to accomodate
/*    the data from a feature table.  The number
/*    of records from the file (the feature table)
/*    is determined.  Then a buffer is allocated
/*    for these records.  The the attributes of the
/*    feature table object ("theTbl") is constructed.
/*---------------------------------------------*/
static void allocateBufferForFeatureTable
               (int              exactSizeOfRecord,
                int              structSizeOfRecord,
                featureTableType *theTbl)

{
         int numBytes;
         int numRecords;

   numBytes = getNumBytesOfData ();

   numRecords = numBytes / exactSizeOfRecord;

   theTbl->numRecords = numRecords;       

   numBytes = numRecords * structSizeOfRecord;

   theTbl->featureRecords = (void *) malloc (numBytes);

   gmsClearMemory
      ((char *) theTbl->featureRecords,
       numBytes);
}


/*---------------------------------------------*/
/* buildAreaFeatureTable
/*
/* Description:
/*    Utility that constructs an AREA feature
/*    table.  This applies to the following
/*    themes:
/*
/*               CL - Cultural Landmarks
/*               DQ - Data Quality
/*               PO - Political/Oceans
/*               DN - Drainage
/*               HY - Hypsographic
/*               LC - Land Coverage
/*               PP - Populated Places
/*               VG - Vegetation
/*               TileRef
/*---------------------------------------------*/
static void buildAreaFeatureTable
               (gmsThematicType  whichTheme,
                featureTableType *theTbl)

{
   switch (whichTheme)
      {
       case gms_CL:
          {
           build_CL_AreaFeatureTable (theTbl);

           break;
          }

       case gms_DQ:
          {
           build_DQ_AreaFeatureTable (theTbl);

           break;
          }

       case gms_PO:
          {
           build_PO_AreaFeatureTable (theTbl);

           break;
          }

       case gms_TileRef:
          {
           build_TileRef_AreaFeatureTable (theTbl);

           break;
          }

       case gms_DN:
       case gms_HY:
       case gms_LC:
       case gms_PP:
       case gms_VG:
          {
           buildGeneralAreaFeatureTable (theTbl);

           break;
          }

       default:
          {
           printf("*** ERROR : theme is unknown for AREA feature table\n");

           break;
          }
      } /* end switch */
}


/*---------------------------------------------*/
/* buildPointFeatureTable
/*
/* Description:
/*    Utility that constructs a POINT feature
/*    table.  This applies to the following
/*    themes:
/*
/*               AE - Aeronautical
/*               CL - Cultural Landmarks
/*               TS - Transportation Structures
/*               PO - Political/Oceans
/*               DN - Drainage
/*               DS - Drainage (supplemental)
/*               HY - Hypsographic
/*               HS - Hypsograhpic (supplemental)
/*               LC - Land Coverage
/*               PP - Populated Places
/*               OF - Ocean Features
/*               Gazetteer
/*---------------------------------------------*/
static void buildPointFeatureTable
               (gmsThematicType  whichTheme,
                featureTableType *theTbl)

{
   switch (whichTheme)
      {
       case gms_AE:
          {
           build_AE_PointFeatureTable (theTbl);

           break;
          }

       case gms_CL:
          {
           build_CL_PointFeatureTable (theTbl);

           break;
          }

       case gms_PO:
          {
           build_PO_PointFeatureTable (theTbl);

           break;
          }

       case gms_PP:
          {
           build_PP_PointFeatureTable (theTbl);

           break;
          }

       case gms_TS:
          {
           build_TS_PointFeatureTable (theTbl);

           break;
          }

       case gms_Gazetteer:
          {
           buildGazetteerPointFeatureTable (theTbl);

           break;
          }

       case gms_HY:
          {
           build_HY_PointFeatureTable (theTbl);

           break;
          }

       case gms_DN:
       case gms_DS:
       case gms_HS:
       case gms_LC:
       case gms_OF:
          {
           buildGeneralPointFeatureTable (theTbl);

           break;
          }

       default:
          {
           printf("*** ERROR : theme is unknown for POINT feature table\n");

           break;
          }
      } /* end switch */
}


/*---------------------------------------------*/
/* buildLineFeatureTable
/*
/* Description:
/*    Utility that constructs a LINE feature
/*    table.  This applies to the following
/*    themes:
/*
/*               CL - Cultural Landmarks
/*               DQ - Data Quality
/*               OF - Ocean Features
/*               PH - Physiography
/*               UT - Utilities
/*               DN - Drainage
/*               HY - Hypsographic
/*               HS - Hypsographic (supplemental)
/*               PO - Political/Oceans
/*               RR - Railroads
/*               RD - Roads
/*               TS - Transportation Structures
/*---------------------------------------------*/
static void buildLineFeatureTable
               (gmsThematicType  whichTheme,
                featureTableType *theTbl)

{
   switch (whichTheme)
      {
       case gms_CL:
          {
           build_CL_LineFeatureTable (theTbl);

           break;
          }

       case gms_DQ:
          {
           build_DQ_LineFeatureTable (theTbl);

           break;
          }

       case gms_OF:
       case gms_PH:
       case gms_UT:
          {
           build_OF_PH_UT_LineFeatureTable (theTbl);

           break;
          }

       case gms_DN:
       case gms_HY:
       case gms_HS:
       case gms_PO:
       case gms_RR:
       case gms_RD:
       case gms_TS:
          {
           buildGeneralLineFeatureTable (theTbl);

           break;
          }

       default:
          {
           printf("*** ERROR : theme is unknown for LINE feature table\n");

           break;
          }
      } /* end switch */
}


/*---------------------------------------------*/
/* buildTextFeatureTable
/*
/* Description:
/*    Utility that constructs a TEXT feature
/*    table.  This applies to ALL themes.
/*---------------------------------------------*/
static void buildTextFeatureTable
               (featureTableType *theTbl,
                indexTableType   *theTFX)

{
         int                 numBytes;
         int                 numRecords;
         int                 i;
         textFeatureRecType  *textBuffer;

   numRecords = theTFX->numberRecordsInTable;

   theTbl->numRecords = numRecords;

   numBytes = numRecords * sizeof(textFeatureRecType);

   theTbl->featureRecords = (void *) malloc (numBytes);

   textBuffer = (textFeatureRecType *) theTbl->featureRecords;

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

      textBuffer[i].numChars     = gmsReadInteger (feature_fd);

      if (textBuffer[i].numChars > NUM_CHARS)
         {
          printf("*** ERROR : %d is too many chars in text feature table\n",
                 textBuffer[i].numChars);

          return;
         }

      fgets
         (textBuffer[i].theText,
          (textBuffer[i].numChars + 1),
          feature_fd);

      textBuffer[i].textGap        = gmsReadFloat (feature_fd);

      textBuffer[i].symbol         = gmsReadInteger (feature_fd);

      textBuffer[i].tileId         = gmsReadShortInteger (feature_fd);

      textBuffer[i].textForeignKey = gmsReadInteger (feature_fd);
     }
}


/*---------------------------------------------*/
/* build_AE_PointFeatureTable
/*
/* Description:
/*    Utility that constructs an Aeronautical (AE)
/*    POINT feature table.
/*---------------------------------------------*/
static void build_AE_PointFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         AE_pointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_AE_Point_Record,
          sizeof(AE_pointFeatureRecType),
          theTbl);

   buffer = (AE_pointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (feature_fd);

       buffer[i].kindOfAirport = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].nameOfAirport,
           sizeof (buffer[i].nameOfAirport),
           feature_fd);

       buffer[i].airportElevation = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].dateOfInfo,
           sizeof (buffer[i].dateOfInfo),
           feature_fd);

       fgets
          (buffer[i].internCivilOrgNum,
           sizeof (buffer[i].internCivilOrgNum),
           feature_fd);

       fgets
          (buffer[i].DafifRefNum,
           sizeof (buffer[i].DafifRefNum),
           feature_fd);

       buffer[i].tileRefId      = gmsReadShortInteger (feature_fd);

       buffer[i].END_foreignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_CL_PointFeatureTable
/*
/* Description:
/*    Utility that constructs an Cultural-Landmarks
/*    (CL) POINT feature table.
/*---------------------------------------------*/
static void build_CL_PointFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         CL_pointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_CL_Point_Record,
          sizeof(CL_pointFeatureRecType),
          theTbl);

   buffer = (CL_pointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].landmarkLabel,
           sizeof (buffer[i].landmarkLabel),
           feature_fd);

       buffer[i].tileRefId      = gmsReadShortInteger (feature_fd);

       buffer[i].END_foreignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_PO_PointFeatureTable
/*
/* Description:
/*    Utility that constructs an Political/Oceans
/*    (PO) POINT feature table.
/*---------------------------------------------*/
static void build_PO_PointFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         PO_pointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_PO_Point_Record,
          sizeof(PO_pointFeatureRecType),
          theTbl);

   buffer = (PO_pointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID   = gmsReadInteger (feature_fd);

       buffer[i].type = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].regionId,
           sizeof (buffer[i].regionId),
           feature_fd);

       fgets
          (buffer[i].PO_Id,
           sizeof (buffer[i].PO_Id),
           feature_fd);

       buffer[i].tileRefId      = gmsReadShortInteger (feature_fd);

       buffer[i].END_foreignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_PP_PointFeatureTable
/*
/* Description:
/*    Utility that constructs an Populated Places
/*    (PP) POINT feature table.
/*---------------------------------------------*/
static void build_PP_PointFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         PP_pointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_PP_Point_Record,
          sizeof(PP_pointFeatureRecType),
          theTbl);

   buffer = (PP_pointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID   = gmsReadInteger (feature_fd);

       buffer[i].type = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].name,
           sizeof (buffer[i].name),
           feature_fd);

       buffer[i].specialFontFlag = gmsReadInteger (feature_fd);

       if (buffer[i].specialFontFlag < 0)
          buffer[i].specialFontFlag = -1;

       buffer[i].tileRefId       = gmsReadShortInteger (feature_fd);

       buffer[i].END_foreignKey  = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_TS_PointFeatureTable
/*
/* Description:
/*    Utility that constructs an Transportation-
/*    Structure (TS) POINT feature table.
/*---------------------------------------------*/
static void build_TS_PointFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         TS_pointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_TS_Point_Record,
          sizeof(TS_pointFeatureRecType),
          theTbl);

   buffer = (TS_pointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID             = gmsReadInteger (feature_fd);

       buffer[i].type           = gmsReadInteger (feature_fd);

       buffer[i].structure      = gmsReadInteger (feature_fd);

       buffer[i].tileRefId      = gmsReadShortInteger (feature_fd);

       buffer[i].END_foreignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* buildGazetteerPointFeatureTable
/*
/* Description:
/*    Utility that constructs a "Gazetteer"
/*    POINT feature table.
/*---------------------------------------------*/
static void buildGazetteerPointFeatureTable
               (featureTableType *theTbl)

{
         int                          i;
         gazetteerPointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_Gazetteer_Point_Record,
          sizeof(gazetteerPointFeatureRecType),
          theTbl);

   buffer = (gazetteerPointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].text,
           sizeof (buffer[i].text),
           feature_fd);

       fgets
          (buffer[i].databaseLayerName,
           sizeof (buffer[i].databaseLayerName),
           feature_fd);
      }
}


/*---------------------------------------------*/
/* build_HY_PointFeatureTable
/*
/* Description:
/*    Utility that builds a table of 'hypsographic'
/*    records:
/*
/*       Hypsographic Points;
/*       --------------------
/*         ID      =I,1,P,Row Identifier,-,-,:
/*         HYPTTYPE=I,1,N,Type of Spot Elevation,INT.VDT,-,:
/*         HYPTVAL =I,1,N,Spot Elevation Value,-,-,:
/*         TILE_ID =S,1,F,Tile Reference Identifier,-,HYPOINT.PTI,:
/*         END_ID  =I,1,F,Entity Node Primitive Foreign Key,-,-,:;
/*
/*    The reference document is : HYPOINT.DOC
/*---------------------------------------------*/
static void build_HY_PointFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         HY_pointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_HY_Point_Record,
          sizeof(HY_pointFeatureRecType),
          theTbl);

   buffer = (HY_pointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID             = gmsReadInteger (feature_fd);

       buffer[i].type           = gmsReadInteger (feature_fd);

       buffer[i].kindOfSpotElev = gmsReadInteger (feature_fd);

       buffer[i].tileRefId      = gmsReadShortInteger (feature_fd);

       buffer[i].END_foreignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* buildGeneralPointFeatureTable
/*
/* Description:
/*    Utility that builds POINT feature records for
/*    the following themes:
/*           DN - Drainage
/*           DS - Drainage supplemental
/*           HY - Hypsographic
/*           LC - Land Coverage
/*           OF - Ocean Features
/*---------------------------------------------*/
static void buildGeneralPointFeatureTable
               (featureTableType *theTbl)

{
         int                        i;
         generalPointFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_General_Point_Record,
          sizeof(generalPointFeatureRecType),
          theTbl);

   buffer = (generalPointFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID             = gmsReadInteger (feature_fd);

       buffer[i].value          = gmsReadInteger (feature_fd);

       buffer[i].tileRefId      = gmsReadShortInteger (feature_fd);

       buffer[i].END_foreignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_CL_AreaFeatureTable
/*
/* Description:
/*    Utility that constructs an Cultural-Landmarks
/*    (CL) AREA feature table.
/*---------------------------------------------*/
static void build_CL_AreaFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         CL_AreaFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_CL_Area_Record,
          sizeof(CL_AreaFeatureRecType),
          theTbl);

   buffer = (CL_AreaFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].landmarkLabel,
           sizeof (buffer[i].landmarkLabel),
           feature_fd);

       buffer[i].tileId         = gmsReadShortInteger (feature_fd);

       buffer[i].faceForeignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_DQ_AreaFeatureTable
/*
/* Description:
/*    Utility that constructs a Data-Quality
/*    (DQ) AREA feature table.
/*---------------------------------------------*/
static void build_DQ_AreaFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         DQ_areaFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_DQ_Area_Record,
          sizeof(DQ_areaFeatureRecType),
          theTbl);

   buffer = (DQ_areaFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].ONC_sheetId,
           sizeof (buffer[i].ONC_sheetId),
           feature_fd);

       fgets
          (buffer[i].ONC_compilationDate,
           sizeof (buffer[i].ONC_compilationDate),
           feature_fd);

       fgets
          (buffer[i].revisionDate,
           sizeof (buffer[i].revisionDate),
           feature_fd);

       fgets
          (buffer[i].printDate,
           sizeof (buffer[i].printDate),
           feature_fd);

       fgets
          (buffer[i].comments,
           sizeof (buffer[i].comments),
           feature_fd);

       buffer[i].absoluteHorizAccuracy = gmsReadInteger (feature_fd);

       if (buffer[i].absoluteHorizAccuracy < 0)
          buffer[i].absoluteHorizAccuracy = -1;

       buffer[i].absoluteVertAccuracy  = gmsReadInteger (feature_fd);

       if (buffer[i].absoluteVertAccuracy < 0)
          buffer[i].absoluteVertAccuracy = -1;

       buffer[i].tileId                = gmsReadShortInteger (feature_fd);

       buffer[i].faceForeignKey        = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_PO_AreaFeatureTable
/*
/* Description:
/*    Utility that constructs a Political/Oceans
/*    (PO) AREA feature table.
/*---------------------------------------------*/
static void build_PO_AreaFeatureTable
               (featureTableType *theTbl)

{
         int                    i;
         PO_areaFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_PO_Area_Record,
          sizeof(PO_areaFeatureRecType),
          theTbl);

   buffer = (PO_areaFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID          = gmsReadInteger (feature_fd);

       buffer[i].featureType = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].region,
           sizeof (buffer[i].region),
           feature_fd);

       fgets
          (buffer[i].identifier,
           sizeof (buffer[i].identifier),
           feature_fd);

       fgets
          (buffer[i].adminUnits,
           sizeof (buffer[i].adminUnits),
           feature_fd);

       buffer[i].tileId         = gmsReadShortInteger (feature_fd);

       buffer[i].faceForeignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_TileRef_AreaFeatureTable
/*
/* Description:
/*    Utility that constructs a Tile-Reference
/*    AREA feature table.
/*---------------------------------------------*/
static void build_TileRef_AreaFeatureTable
               (featureTableType *theTbl)

{
         int                        i;
         tileRef_areaFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_Tile_Ref_Area_Record,
          sizeof(tileRef_areaFeatureRecType),
          theTbl);

   buffer = (tileRef_areaFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].tileName,
           sizeof (buffer[i].tileName),
           feature_fd);
      }
}


/*---------------------------------------------*/
/* buildGeneralAreaFeatureTable
/*
/* Description:
/*    Utility that constructs an AREA feature
/*    table for the following themes:
/*
/*             DN - Drainage
/*             HY - Hypsographic
/*             LC - Land Coverage
/*             PP - Populated Places
/*             VG - Vegetation
/*---------------------------------------------*/
static void buildGeneralAreaFeatureTable
               (featureTableType *theTbl)

{
         int                       i;
         generalAreaFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_General_Area_Record,
          sizeof(generalAreaFeatureRecType),
          theTbl);

   buffer = (generalAreaFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID             = gmsReadInteger (feature_fd);

       buffer[i].type           = gmsReadInteger (feature_fd);

       if (buffer[i].type < 0)
          buffer[i].type = -1;

       buffer[i].tileId         = gmsReadShortInteger (feature_fd);

       buffer[i].faceForeignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_CL_LineFeatureTable
/*
/* Description:
/*    Utility that constructs a Cultural-Landmarks
/*    (CL) LINE feature table.
/*---------------------------------------------*/
static void build_CL_LineFeatureTable
               (featureTableType *theTbl)

{
         int                   i;
         CL_lineFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_CL_Line_Record,
          sizeof(CL_lineFeatureRecType),
          theTbl);

   buffer = (CL_lineFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].landmarkLabel,
           sizeof (buffer[i].landmarkLabel),
           feature_fd);

       buffer[i].tileId         = gmsReadShortInteger (feature_fd);

       buffer[i].edgeForeignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_DQ_LineFeatureTable
/*
/* Description:
/*    Utility that constructs a Data-Quality
/*    (DQ) LINE feature table.
/*---------------------------------------------*/
static void build_DQ_LineFeatureTable
               (featureTableType *theTbl)

{
         int                   i;
         DQ_lineFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_DQ_Line_Record,
          sizeof(DQ_lineFeatureRecType),
          theTbl);

   buffer = (DQ_lineFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID     = gmsReadInteger (feature_fd);

       buffer[i].lineId = gmsReadInteger (feature_fd);

       fgets
          (buffer[i].first_ONC_sheet,
           sizeof (buffer[i].first_ONC_sheet),
           feature_fd);

       fgets
          (buffer[i].second_ONC_sheet,
           sizeof (buffer[i].second_ONC_sheet),
           feature_fd);

       buffer[i].tileId         = gmsReadShortInteger (feature_fd);

       buffer[i].edgeForeignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* build_OF_PH_UT_LineFeatureTable
/*
/* Description:
/*    Utility that constructs a LINE feature
/*    table for the following themes:
/*
/*            OF - Ocean Features
/*            PH - Physiography
/*            UT - Utilities
/*---------------------------------------------*/
static void build_OF_PH_UT_LineFeatureTable
               (featureTableType *theTbl)

{
         int                         i;
         OF_PH_UT_lineFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_OF_PH_UT_Line_Record,
          sizeof(OF_PH_UT_lineFeatureRecType),
          theTbl);

   buffer = (OF_PH_UT_lineFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID             = gmsReadInteger (feature_fd);

       buffer[i].type           = gmsReadInteger (feature_fd);

       buffer[i].tileId         = gmsReadShortInteger (feature_fd);

       buffer[i].edgeForeignKey = gmsReadInteger (feature_fd);
      }
}


/*---------------------------------------------*/
/* buildGeneralLineFeatureTable
/*
/* Description:
/*    This utility build a feature table for the
/*    following themes:
/*          DN - Drainage
/*          HY - Hysographic
/*          HS - Hysographic supplemental
/*          PO - Political/Oceans
/*          RR - Railroads
/*          RD - Roads
/*          TS - Transportation Structure
/*---------------------------------------------*/
static void buildGeneralLineFeatureTable
               (featureTableType *theTbl)

{
         int                       i;
         generalLineFeatureRecType *buffer;

   allocateBufferForFeatureTable
         (Size_Of_General_Line_Record,
          sizeof(generalLineFeatureRecType),
          theTbl);

   buffer = (generalLineFeatureRecType *) theTbl->featureRecords;

   rewind (feature_fd);

   readPastFormatInformation ();

   for (i = 0; i < theTbl->numRecords; i++)
      {
       buffer[i].ID             = gmsReadInteger (feature_fd);

       buffer[i].type           = gmsReadInteger (feature_fd);

       if (buffer[i].type < 0)
          buffer[i].type = -1;

       buffer[i].status         = gmsReadInteger (feature_fd);

       if (buffer[i].status < 0)
          buffer[i].status = -1;

       buffer[i].tileId         = gmsReadShortInteger (feature_fd);

       buffer[i].edgeForeignKey = gmsReadInteger (feature_fd);
      }
}


           /*-----------------------*/
           /* Print utilities       */
           /*-----------------------*/


/*---------------------------------------------*/
/* printTextFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void printTextFeatureTable
               (int                numRecs,
                textFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("_____________\n");
       printf("ID             : %d\n", theTbl[i].ID);
       printf("Kind-of-text   : %d\n", theTbl[i].kindOfText);
       printf("Text Height    : %f\n", theTbl[i].heightOfText);
       printf("Text (%d chars): %s\n",
              theTbl[i].numChars,
              theTbl[i].theText);
       printf("Text Gap       : %f\n", theTbl[i].textGap);
       printf("Symbol         : %d\n", theTbl[i].symbol);
       printf("Tile-ID        : %d\n", theTbl[i].tileId);
       printf("Text-Fgn-Key   : %d\n", theTbl[i].textForeignKey);
      }
}


/*---------------------------------------------*/
/* print_AE_PointFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_AE_PointFeatureTable
               (int                    numRecs,
                AE_pointFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("____________\n");
       printf("ID                  : %d\n",
              theTbl[i].ID);
       printf("Kind of Airport     : %d\n",
              theTbl[i].kindOfAirport);
       printf("Name of Airport     : %s\n",
              theTbl[i].nameOfAirport);
       printf("Elevation           : %d\n",
              theTbl[i].airportElevation);
       printf("Date of info        : %s\n",
              theTbl[i].dateOfInfo);
       printf("Inter Civil Org Num : %s\n",
              theTbl[i].internCivilOrgNum);
       printf("DAFIF Ref Num       : %s\n",
              theTbl[i].DafifRefNum);
       printf("Tile-Ref-ID         : %d\n",
              theTbl[i].tileRefId);
       printf("END-Fgn-Key         : %d\n",
              theTbl[i].END_foreignKey);
      }
}


/*---------------------------------------------*/
/* print_CL_PointFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_CL_PointFeatureTable
               (int                    numRecs,
                CL_pointFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("____________\n");
       printf("ID             : %d\n", theTbl[i].ID);
       printf("Landmark Label : %s\n", theTbl[i].landmarkLabel);
       printf("Tile-Ref-ID    : %d\n", theTbl[i].tileRefId);
       printf("END-Fgn-Key    : %d\n", theTbl[i].END_foreignKey);
      }
}


/*---------------------------------------------*/
/* print_PO_PointFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_PO_PointFeatureTable
               (int                    numRecs,
                PO_pointFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("____________\n");
       printf("ID              : %d\n", theTbl[i].ID);
       printf("Type            : %d\n", theTbl[i].type);
       printf("Region ID       : %s\n", theTbl[i].regionId);
       printf("PO ID           : %s\n", theTbl[i].PO_Id);
       printf("Tile Ref ID     : %d\n", theTbl[i].tileRefId);
       printf("END foreign key : %d\n", theTbl[i].END_foreignKey);
      }
}


/*---------------------------------------------*/
/* print_PP_PointFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_PP_PointFeatureTable
               (int                    numRecs,
                PP_pointFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("_________________\n");
       printf("ID         : %d\n", theTbl[i].ID);
       printf("Type       : %d\n", theTbl[i].type);
       printf("Name       : %s\n", theTbl[i].name);
       printf("Font Flg   : %d\n", theTbl[i].specialFontFlag);
       printf("Tile-ref ID: %d\n", theTbl[i].tileRefId);
       printf("END Fgn Key: %d\n", theTbl[i].END_foreignKey);
      }
}


/*---------------------------------------------*/
/* print_TS_PointFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_TS_PointFeatureTable
               (int                    numRecs,
                TS_pointFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("____________\n");
       printf("ID          : %d\n", theTbl[i].ID);
       printf("Type        : %d\n", theTbl[i].type);
       printf("Structure   : %d\n", theTbl[i].structure);
       printf("Tile-Ref-ID : %d\n", theTbl[i].tileRefId);
       printf("END-Fgn-Key : %d\n", theTbl[i].END_foreignKey);
      }
}


/*---------------------------------------------*/
/* printGazetteerPointFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void printGazetteerPointFeatureTable
               (int                          numRecs,
                gazetteerPointFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("____________\n");
       printf("ID                  : %d\n", theTbl[i].ID);
       printf("Text                : %s\n", theTbl[i].text);
       printf("Database Layer Name : %s\n",
              theTbl[i].databaseLayerName);
      }
}


/*---------------------------------------------*/
/* print_HY_PointFeatureTable
/*
/* Description:
/*    Prints out the records from a 'hysographic
/*    point feature' table.
/*---------------------------------------------*/
static void print_HY_PointFeatureTable
               (int                    numRecs,
                HY_pointFeatureRecType *theTbl)

{
         int i;

   printf("ID \t Type \t Elev-kind \t Tile-Ref-Id \t END-Fgn-Key\n");
   printf("-----------------------------------------------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("%d \t %d \t %d \t\t %d \t\t %d\n",
              theTbl[i].ID,
              theTbl[i].type,
              theTbl[i].kindOfSpotElev,
              theTbl[i].tileRefId,
              theTbl[i].END_foreignKey);
      }
}


/*---------------------------------------------*/
/* printGeneralPointFeatureTable
/*
/* Description:
/*    Prints out 'point feature' records for the
/*    following themes:
/*              DN - Drainage
/*              DS - Drainage supplemental
/*              HS - Hypsographic supplemental
/*              LC - Land Coverage
/*              OF - Ocean Features
/*---------------------------------------------*/
static void printGeneralPointFeatureTable
               (int                        numRecs,
                generalPointFeatureRecType *theTbl)

{
         int i;

   printf("ID \t Value \t Tile-Ref-ID \t END-Fgn-Key\n");
   printf("------------------------------------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("%d \t %d \t %d \t\t %d\n",
              theTbl[i].ID,
              theTbl[i].value,
              theTbl[i].tileRefId,
              theTbl[i].END_foreignKey);
      }
}


/*---------------------------------------------*/
/* print_CL_AreaFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_CL_AreaFeatureTable
               (int                   numRecs,
                CL_AreaFeatureRecType *theTbl)

{
         int i;

   printf("ID Landmark-Label \t\t\t\t Tile-Id \t Face-Fgn-Key\n");
   printf("--------------------------------------------------------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("%d  %s %d \t %d\n",
              theTbl[i].ID,
              theTbl[i].landmarkLabel,
              theTbl[i].tileId,
              theTbl[i].faceForeignKey);
      }
}


/*---------------------------------------------*/
/* print_DQ_AreaFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_DQ_AreaFeatureTable
               (int                   numRecs,
                DQ_areaFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("_____________\n");
       printf("ID              : %d\n", theTbl[i].ID);
       printf("ONC Sheet-ID    : %s\n", theTbl[i].ONC_sheetId);
       printf("ONC Compile Date: %s\n", theTbl[i].ONC_compilationDate);
       printf("Revision Date   : %s\n", theTbl[i].revisionDate);
       printf("Print Date      : %s\n", theTbl[i].printDate);
       printf("Comments        : %s\n", theTbl[i].comments);
       printf("Abs Horiz Accur : %d\n", theTbl[i].absoluteHorizAccuracy);
       printf("Abs Verti Accur : %d\n", theTbl[i].absoluteVertAccuracy);
       printf("Tile-ID         : %d\n", theTbl[i].tileId);
       printf("Face-Fgn-Key    : %d\n", theTbl[i].faceForeignKey);
      }
}


/*---------------------------------------------*/
/* print_PO_AreaFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_PO_AreaFeatureTable
               (int                   numRecs,
                PO_areaFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("_____________\n");
       printf("ID           : %d\n", theTbl[i].ID);
       printf("Feature Type : %d\n", theTbl[i].featureType);
       printf("Region       : %s\n", theTbl[i].region);
       printf("Identifier   : %s\n", theTbl[i].identifier);
       printf("Admin Units  : %s\n", theTbl[i].adminUnits);
       printf("Tile-ID      : %d\n", theTbl[i].tileId);
       printf("Face-Fgn-Key : %d\n", theTbl[i].faceForeignKey);
      }
}


/*---------------------------------------------*/
/* print_TileRef_AreaFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_TileRef_AreaFeatureTable
               (int                        numRecs,
                tileRef_areaFeatureRecType *theTbl)

{
         int i;

   printf("ID \t Tile-Name\n");
   printf("---------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("%d \t %s\n",
              theTbl[i].ID,
              theTbl[i].tileName);
      }
}


/*---------------------------------------------*/
/* printGeneralAreaFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void printGeneralAreaFeatureTable
               (int                       numRecs,
                generalAreaFeatureRecType *theTbl)

{
         int i;

   printf("ID \t Type \t Tile-Id \t Face-Fgn-Key\n");
   printf("-------------------------------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("%d \t %d \t %d \t\t %d\n",
              theTbl[i].ID,
              theTbl[i].type,
              theTbl[i].tileId,
              theTbl[i].faceForeignKey);
      }
}


/*---------------------------------------------*/
/* print_CL_LineFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_CL_LineFeatureTable
               (int                   numRecs,
                CL_lineFeatureRecType *theTbl)

{
         int i;

   printf("ID \t Landmark-label \t Tile-Id \t Edg-Fgn-Key\n");
   printf("--------------------------------------------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("____________\n");
       printf("ID             : %d\n", theTbl[i].ID);
       printf("Landmark-Label : %s\n", theTbl[i].landmarkLabel);
       printf("Tile-ID        : %d\n", theTbl[i].tileId);
       printf("Edg-Fgn-Key    : %d\n", theTbl[i].edgeForeignKey);
      }
}


/*---------------------------------------------*/
/* print_DQ_LineFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_DQ_LineFeatureTable
               (int                   numRecs,
                DQ_lineFeatureRecType *theTbl)

{
         int i;

   for (i = 0; i < numRecs; i++)
      {
       printf("_____________\n");
       printf("ID            : %d\n", theTbl[i].ID);
       printf("Line-ID       : %d\n", theTbl[i].lineId);
       printf("1st ONC sheet : %s\n", theTbl[i].first_ONC_sheet);
       printf("2nd ONC sheet : %s\n", theTbl[i].second_ONC_sheet);
       printf("Tile-ID       : %d\n", theTbl[i].tileId);
       printf("Edge-Fgn-Key  : %d\n", theTbl[i].edgeForeignKey);
      }
}


/*---------------------------------------------*/
/* print_OF_PH_UT_LineFeatureTable
/*
/* Description:
/*---------------------------------------------*/
static void print_OF_PH_UT_LineFeatureTable
               (int                         numRecs,
                OF_PH_UT_lineFeatureRecType *theTbl)

{
         int i;

   printf("ID \t Type \t Tile-Id \t Edg-Fgn-Key\n");
   printf("------------------------------------------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("%d \t %d \t %d \t\t %d\n",
              theTbl[i].ID,
              theTbl[i].type,
              theTbl[i].tileId,
              theTbl[i].edgeForeignKey);
      }
}


/*---------------------------------------------*/
/* printGeneralLineFeatureTable
/*
/* Description:
/*    Utility to print the contents of feature
/*    tables of the following themes:
/*           DN - Drainage
/*           HY - Hypsographic
/*           HS - Hysographic supplemental
/*           PO - Political/Oceans
/*           RR - Railroads
/*           RD - Roads
/*           TS - Transportation Structures
/*---------------------------------------------*/
static void printGeneralLineFeatureTable
               (int                       numRecs,
                generalLineFeatureRecType *theTbl)

{
         int i;

   printf("ID \t Type \t Status Tile-Id Edg-Fgn-Key\n");
   printf("----------------------------------------------\n");

   for (i = 0; i < numRecs; i++)
      {
       printf("%d \t %d \t %d \t %d \t %d\n",
              theTbl[i].ID,
              theTbl[i].type,
              theTbl[i].status,
              theTbl[i].tileId,
              theTbl[i].edgeForeignKey);
      }
}
