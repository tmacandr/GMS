/*----------------------------------------------------------------------------
 * File: gmsBrowseFeatureTypes.h
 * Date: 07-Sep-99 : Initial definition
 *       05-Oct-99 : Clean-up due to code inspection
 *       21-Dec-99 : Moved enum list of themes to global pkg
 *
 * Description:
 *    Defines the various types/structs used to manage/manipulate "feature"
 *    data from the BROWSE section of the DCW database.  A "feature" is ...
 *
 *       ... a model of a real world geographic entity.  [It is] a
 *       zero-, one-, or two-dimensional entity of uniform attribute
 *       scheme from an exhaustive attribute distribution across a
 *       plane, or a set of such entities sharing common attribute values.
 *       The three subtypes are simple features, complex features,
 *       and text features.  The types of simple features are point
 *       features, line features, and area features.
 *
 *    There are 4 types of "feature" tables managed by this utility.  They
 *    are:
 *
 *                   area  (*.AFT files)
 *                   line  (*.LFT files)
 *                   point (*.PFT files)
 *                   text  (*.TFT files)
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *----------------------------------------------------------------------------*/
#ifndef BROWSE_FEATURE_TYPES

   #define BROWSE_FEATURE_TYPES

   #include <gmsTypesAndConstants.h>

   #define Num_Browse_Features 4

   typedef enum { gmsBrowseArea,
                  gmsBrowsePoint,
                  gmsBrowseLine,
                  gmsBrowseText } gmsBrowseFeatureType;


   typedef struct _browseFeatureRecType
      {
       int ID;
       int value;
      } browseFeatureRecType;

   #define Size_Of_Browse_Feature_Record 8


   /*------------------------------*/
   /* Used for both LINE and AREA  */
   /* GR feature tables            */
   /*------------------------------*/
   typedef struct _browseGrFeatureRecType
      {
       int  ID;
       char region_1;
       char region_2;
       char region_3;
       char region_4;
      } browseGrFeatureRecType;

   #define Size_Of_Browse_GR_Feature_Record 8


   typedef struct _browseInAreaFeatureRecType
      {
       int  ID;
       char ONC_SheetId[6 + 1];
      } browseInAreaFeatureRecType;

   #define Size_Of_Browse_IN_Area_Feature_Record 10


   typedef struct _browsePointFeatureRecType
      {
       int   ID;
       int   type;
       char  name[40 + 1];
      } browsePointFeatureRecType;

   #define Size_Of_Browse_Point_Record 48


   /*------------------------------*/
   /* All Text Feature Records     */
   /*------------------------------*/

   #define NUM_CHARS 128

   typedef struct _browseTextFeatureRecType
      {
       int   ID;
       int   kindOfText;
       float heightOfText;
       char  theText[NUM_CHARS];
       float textGap;
       int   symbol;
      } browseTextFeatureRecType;

#endif

