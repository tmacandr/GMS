/*----------------------------------------------------------------------------
 * File : gmsFeatureTable.h
 * Date : 25-Aug-99 : initial definition
 *        11-Sep-99 : major surgery to the interface (re : Mil-D-89009)
 *        07-Oct-99 : Clean-up due to code-inspection
 *        17-Nov-99 : get feature tbl routine figures out index file
 *
 * Description:
 *    Utilities to read/process any "Feature Table" file (except from BROWSE)
 *    of the Digital Chart of the World (DCW) database.  Consider the following
 *    definitions:
 *
 *       Feature:
 *       --------
 *          A model of a real world geographic entity.  [It is a] zero-,
 *          one-, or two-dimensional entity of uniform attribute scheme
 *          from an exhaustive attribute distribution across a plane,
 *          or a set of such entities sharing common attribute values.
 *          The three subtypes are simple features, complex features,
 *          and text features.  The types of simple features are point
 *          features, line features, and area features.
 *
 *       Feature Class:
 *       --------------
 *          A set of features that shares a homogeneous set of attributes.
 *          A feature class consists of a set of tables that includes one
 *          or more primitive tables and one or more attribute tables.
 *          A feature class has the same columns of attribute information
 *          for each feature.  Every feature class has one and only one
 *          feature table.  The two types of feature classes are the simple
 *          feature class and the complex feature class.  The subtypes of
 *          the simple feature class are the "point", "line", "area", and
 *          "text" feature classes.
 *
 *    Ok, so there's some inconsistency in the definitions.  In
 *    general, there are 4 types of "feature" tables managed by this
 *    component.  They are:
 *
 *                   area  (*.AFT files)
 *                   line  (*.LFT files)
 *                   point (*.PFT files)
 *                   text  (*.TFT files)
 *
 *    The table below maps "feature-tables" to "themes".  A "y" means a
 *    feature table exists for that theme (i.e. directory).
 *
 *    ___________________________________________________________________
 *    |                           |          Feature Table              |
 *    |      DCW Theme            |-------------------------------------|
 *    |                           | *.AFT  |  *.LFT  |  *.PFT  |  *.TFT | 
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Aeronautical (AE)         |   n    |    n    |   y     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Cultural Landmarks (CL)   |   y    |    y    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Data Quality (DQ)         |   y    |    y    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Drainage (DN)             |   y    |    y    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | DN - Supplemental (DS)    |   n    |    n    |   y     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Hypsography (HY)          |   y    |    n    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | HY - Supplemental (HS)    |   n    |    y    |   y     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Land Cover (LC)           |   y    |    n    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Ocean Features (OF)       |   n    |    y    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Physiography (PH)         |   n    |    y    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Political/Oceans (PO)     |   y    |    y    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Populated Places (PP)     |   y    |    n    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Railroads (RR)            |   n    |    y    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Roads (RD)                |   n    |    y    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Transportation Struct (TS)|   n    |    y    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Utilites (UT)             |   n    |    y    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Vegetation (VG)           |   y    |    n    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Gazeteer                  |   n    |    n    |   y     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Tile Reference            |   y    |    n    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Library Reference         |   n    |    y    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *
 *    Reference:
 *        1) Mil-Std-600006
 *        2) Mil-D-89009
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *----------------------------------------------------------------------------*/

#ifndef FEATURE

   #define FEATURE 

   #include <gmsFeatureTypes.h>

   /*--------------------------*/
   /* 'Generic' Feature Table  */
   /*--------------------------*/
   typedef struct _featureTableType
      {
       gmsThematicType whichTheme;
       gmsFeatureType  whichFeature;
       int             numRecords;
       void            *featureRecords;
      } featureTableType;


   /*---------------------------------------------
    * gmsGetFeatureTable
    *
    * Description:
    *    This utility reads the specified "feature
    *    table" and builds a corresponding data
    *    structure.  The kind of data-structure is
    *    specified by the parameter "whichTheme".
    *    A pointer to a newly allocated table is
    *    returned to the caller.  The caller is then
    *    able to type-cast the "void *" to the
    *    corresponding data-structure.
    *
    *    Note:
    *       If the file to be read is a "text-feature
    *       table, then the name of the "text-index"
    *       file ("*.tfx") is determined by this
    *       routine.
    *
    *    It is the caller's responsibility to free
    *    the item by using the utility
    *    "gmsFreeFeatureTable" (see below).
    *---------------------------------------------*/
   featureTableType *gmsGetFeatureTable
                           (gmsThematicType whichTheme,
                            const char      *featureFilePath,
                            gmsFeatureType  whichFeature);


   /*---------------------------------------------
    * gmsFreeFeatureTable
    *
    * Description:
    *    This utility frees a "Feature Table" that
    *    had been previously allocated using
    *    "gmsGetFeatureTable".
    *---------------------------------------------*/
   void gmsFreeFeatureTable
           (featureTableType *theFeatureTable);


   /*---------------------------------------------
    * gmsPrintFeatureTable	
    *
    * Description:
    *    This function will print the "Feature Table"
    *    object to standard out.
    *---------------------------------------------*/
   void gmsPrintFeatureTable
           (featureTableType *theTbl);

#endif

