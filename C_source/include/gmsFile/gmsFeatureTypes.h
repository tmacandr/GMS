//----------------------------------------------------------------------------*/
// File: gmsFeatureTypes.h
// Date: 07-Sep-99 : Initial definition
//       11-Sep-99 : Major surgery to the interface (re : Mil-D-89009)
//       05-Oct-99 : Clean-up due to code-inspection
//       21-Dec-99 : Move enumeration list of themes to global types pkg
//
// Description:
//    Defines the various types/structs used to manage/manipulate "feature"
//    data from the THEMATIC section of the DCW database.  A "feature" is ...
//
//       ... a model of a real world geographic entity.  [It is] a
//       zero-, one-, or two-dimensional entity of uniform attribute
//       scheme from an exhaustive attribute distribution across a
//       plane, or a set of such entities sharing common attribute values.
//       The three subtypes are simple features, complex features,
//       and text features.  The types of simple features are point
//       features, line features, and area features.
//
//    There are 4 types of "feature" tables managed by this utility.  They
//    are:
//
//                   area  (*.AFT files)
//                   line  (*.LFT files)
//                   point (*.PFT files)
//                   text  (*.TFT files)
//
//    Reference:
//        1) Mil-Std-600006
//        2) Mil-D-89009
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/
#ifndef FEATURE_TYPES

   #define FEATURE_TYPES

   #include <gmsTypesAndConstants.h>

   typedef enum { gmsArea,
                  gmsPoint,
                  gmsLine,
                  gmsText } gmsFeatureType;

   #define Num_Features 4

   //------------------------------*/
   // Point Features               */
   //------------------------------*/
   typedef struct _AE_pointFeatureRecType
      {
       int   ID;
       int   kindOfAirport;
       char  nameOfAirport[50 + 1];
       int   airportElevation;
       char  dateOfInfo[20 + 1];
       char  internCivilOrgNum[4 + 1];
       char  DafifRefNum[7 + 1];
       short tileRefId;
       int   END_foreignKey;  // Note: END = Entity Node */
      } AE_pointFeatureRecType;

   typedef struct _CL_pointFeatureRecType
      {
       int   ID;
       char  landmarkLabel[50 + 1];
       short tileRefId;
       int   END_foreignKey;
      } CL_pointFeatureRecType;

   typedef struct _PO_pointFeatureRecType
      {
       int   ID;
       int   type;
       char  regionId[2 + 1];
       char  PO_Id[2 + 1]; 
       short tileRefId;
       int   END_foreignKey;
      } PO_pointFeatureRecType;

   typedef struct _PP_pointFeatureRecType
      {
       int   ID;
       int   type;
       char  name[40 + 1];
       int   specialFontFlag;
       short tileRefId;
       int   END_foreignKey;
      } PP_pointFeatureRecType;

   typedef struct _TS_pointFeatureRecType
      {
       int   ID;
       int   type;
       int   structure;
       short tileRefId;
       int   END_foreignKey;
      } TS_pointFeatureRecType;

   typedef struct _gazetteerPointFeatureRecType
      {
       int  ID;
       char text[50 + 1];
       char databaseLayerName[40 + 1];
      } gazetteerPointFeatureRecType;

   typedef struct _HY_pointFeatureRecType
      {
       int   ID;
       int   type;
       int   kindOfSpotElev;
       short tileRefId;
       int   END_foreignKey;
      } HY_pointFeatureRecType;

   typedef struct _generalPointFeatureRecType
      {
       int   ID;               //---------------------------*/
       int   value;            // Use for:                  */
       short tileRefId;        //    DN, DS, HS, LC, OF     */
       int   END_foreignKey;   //---------------------------*/
      } generalPointFeatureRecType;


   //------------------------------*/
   // Area Features                */
   //------------------------------*/
   typedef struct _CL_areaFeatureRecType
      {
       int   ID;
       char  landmarkLabel[50 + 1];
       short tileId;
       int   faceForeignKey;
      } CL_AreaFeatureRecType;

   typedef struct _DQ_areaFeatureRecType
      {
       int   ID;
       char  ONC_sheetId[6 + 1];
       char  ONC_compilationDate[20 + 1];
       char  revisionDate[20 + 1];
       char  printDate[20 +1 ];
       char  comments[40 + 1];
       int   absoluteHorizAccuracy;
       int   absoluteVertAccuracy;
       short tileId;
       int   faceForeignKey;
      } DQ_areaFeatureRecType;

   typedef struct _PO_areaFeatureRecType
      {
       int   ID;
       int   featureType;
       char  region[2 + 1];
       char  identifier[2 + 1];
       char  adminUnits[40 + 1];
       short tileId;
       int   faceForeignKey;
      } PO_areaFeatureRecType;

   typedef struct _tileRef_areaFeatureRecType
      {
       int  ID;
       char tileName[6 + 1];
      } tileRef_areaFeatureRecType;

   typedef struct _generalAreaFeatureRecType
      {
       int   ID;                      //------------------------*/
       int   type;                    // Used for:              */
       short tileId;                  //    DN, HY, LC, PP, VG  */
       int   faceForeignKey;          //------------------------*/
      } generalAreaFeatureRecType;


   //------------------------------*/
   // Line Feature                 */
   //------------------------------*/
   typedef struct _CL_lineFeatureRecType
      {
       int   ID;
       char  landmarkLabel[50 + 1];
       short tileId;
       int   edgeForeignKey;
      } CL_lineFeatureRecType;

   typedef struct _DQ_lineFeatureRecType
      {
       int    ID;
       int    lineId;
       char   first_ONC_sheet[3 + 1];
       char   second_ONC_sheet[3 + 1];
       short  tileId;
       int    edgeForeignKey;
      } DQ_lineFeatureRecType;

   typedef struct _OF_PH_UT_lineFeatureRecType
      {
       int   ID;
       int   type;
       short tileId;
       int   edgeForeignKey;
      } OF_PH_UT_lineFeatureRecType;

   //------------------------------------*/
   // Line-Feature-Table for LibRef:
   //    This was NOT included in
   //    Mil-D-89009
   //
   //    Library Reference Lines:
   //       ID=I,1,P,Row Identifier,-,-,:;
   //------------------------------------*/

   typedef struct _generalLineFeatureRecType
      {
       int   ID;
       int   type;                   //--------------------------------*/
       int   status;                 // Use for:                       */
       short tileId;                 //    DN, HY, HS, PO, RR, RD, TS  */
       int   edgeForeignKey;         //--------------------------------*/
      } generalLineFeatureRecType;


   //------------------------------*/
   // Text Feature (for all themes)*/
   //------------------------------*/

   #define NUM_CHARS 128

   typedef struct _textFeatureRecType
      {
       int   ID;
       int   kindOfText;
       float heightOfText;
       int   numChars;
       char  theText[NUM_CHARS];
       float textGap;
       int   symbol;
       short tileId;
       int   textForeignKey;
      } textFeatureRecType;

#endif
