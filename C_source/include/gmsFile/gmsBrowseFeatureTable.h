/*----------------------------------------------------------------------------
 * File : gmsBrowseFeatureTable.h
 * Date : 07-Sep-99 : initial definition
 *        05-Oct-99 : Clean-up due to code inspection
 *        17-Nov-99 : get feature tbl routine figures out name of index file
 *
 * Description:
 *    Utilities to read/process any "Feature Table" file from the BROWSE area
 *    of the Digital Chart of the World (DCW) database.  Consider the
 *    following defintions:
 *
 *       Feature:
 *       --------
 *          A model of a real world geographic entity.  [It is] a zero-,
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
 *    OK, so there's some inconsistency in their (NIMA) definitions.  In
 *    general, there are 4 types of "feature" tables managed by this
 *    component.  They are:
 *
 *                   area  (*.AFT files)
 *                   line  (*.LFT files)
 *                   point (*.PFT files)
 *                   text  (*.TFT files)
 *
 *    The table below maps "feature-tables" to "browse-libraries".  A "y"
 *    means a feature table exists for that browse directory.
 *
 *    ___________________________________________________________________
 *    |                           |          Feature Table              |
 *    |      Browse Theme         |-------------------------------------|
 *    |                           | *.AFT  |  *.LFT  |  *.PFT  |  *.TFT | 
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | ONC Compilation Date (CO) |   y    |    n    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Avail of Hypsog Data (DA) |   y    |    n    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Drainage (DA)             |   y    |    y    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Data Volumes (DV)         |   y    |    n    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Geographic Regions (GR)   |   y    |    y    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | ONC Index (IN)            |   y    |    n    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Library Ref (Libref)      |   n    |    n    |   n     |   n    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Political/Oceans (PO)     |   y    |    y    |   n     |   y    |
 *    |___________________________|________|_________|_________|________|
 *    |                           |        |         |         |        |
 *    | Populated Places (PP)     |   n    |    n    |   y     |   y    |
 *    |___________________________|________|_________|_________|________|
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *----------------------------------------------------------------------------*/

#ifndef BROWSE_FEATURE

   #define BROWSE_FEATURE 

   #include <gmsBrowseFeatureTypes.h>

   /*--------------------------*/
   /* 'Generic' Feature Table  */
   /*--------------------------*/
   typedef struct _browseFeatureTableType
      {
       gmsBrowseThematicType browseTheme;
       gmsBrowseFeatureType  browseFeature;
       int                   numRecords;
       void                  *featureRecords;
      } browseFeatureTableType;


   /*---------------------------------------------
    * gmsGetBrowseFeatureTable
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
    *    "gmsFreeBrowseFeatureTable" (see below).
    *---------------------------------------------*/
   browseFeatureTableType *gmsGetBrowseFeatureTable
                             (gmsBrowseThematicType whichTheme,
                              const char            *featureFilePath,
                              gmsBrowseFeatureType  whichFeature);


   /*---------------------------------------------
    * gmsFreeBrowseFeatureTable
    *
    * Description:
    *    This utility frees a "Feature Table" that
    *    had been previously allocated using
    *    "gmsGetBrowseFeatureTable".
    *---------------------------------------------*/
   void gmsFreeBrowseFeatureTable
           (browseFeatureTableType *theFeatureTable);


   /*---------------------------------------------
    * gmsPrintBrowseFeatureTable	
    *
    * Description:
    *    This function will print the "Feature Table"
    *    object to standard out.
    *---------------------------------------------*/
   void gmsPrintBrowseFeatureTable
           (browseFeatureTableType *theFeatureTable);

#endif

