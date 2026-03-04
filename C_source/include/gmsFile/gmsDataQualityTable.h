/*--------------------------------------------------------------------------
 * File : gmsDataQualityTable.h
 * Date : 12-Jul-99 : Initial Definition
 *        01-Aug-99 : Convert to utilities on port to Sun Solaris
 *        07-Oct-99 : Clean-up due to code-inspection
 *
 * Description:
 *    Utilities to "read" the "Data Quality Table" file from the BROWSE
 *    library of the Digital Chart of the World (DCW).
 *
 *    The "Data Quality Table" contains information on the completeness,
 *    consistency, date status, attribute accuracy, and positional accuracy
 *    of the data.
 *
 *    Reference:
 *        1) Mil-Std-600006
 *        2) Mil-D-89009
 *
 * Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
 *--------------------------------------------------------------------------*/

#ifndef DQT

   #define DQT

   typedef struct _DataQualityTableType
      {
       int  ID;
       char VPF_level[8 + 1];
       char VPF_levelName[20 + 1];
       char featureCompletePercent[256];
       char attributeCompletePercent[256];
       char logicalConsistency[256];
       char editionNumber[8 + 1];
       char creationDate[20 + 1];
       char revisionDate[20 + 1];
       char specificationName[20 + 1];
       char specificationDate[20 + 1];
       char dateOfEarliestSrc[20 + 1];
       char dateOfLatestSrc[20 + 1];
       char stdDevOfQuantAttrib[256];
       char percentReliabOfQualAttrib[256];
       char collectionSpecName[256];
       char nameOfIncludedSrcFile[12 + 1];
       char absoluteHorizAccuracy[256];
       char absoluteHorizUnits[20 + 1];
       char absoluteVertAccuracy[256];
       char absoluteVertUnits[20 + 1];
       char relativeHorizAccuracy[256];
       char relativeHorizUnits[20 + 1];
       char relativeVertAccuracy[256];
       char relativeVertUnits[20 + 1];
       char comments[256];
      } DataQualityTableType;


   /*-------------------------------------------
    * gmsGetDataQualityTable
    *
    * Description:
    *    This utility reads the file that contains
    *    the "Data Quality Table".  A pointer
    *    to a newly allocated table is returned to
    *    the caller.  It is the caller's responsibility
    *    to free the item by using the utility
    *    "gmsFreeDataQualityTable" (see below).
    *-------------------------------------------*/
   DataQualityTableType *gmsGetDataQualityTable
                                   (const char *filePath);


   /*-------------------------------------------
    * gmsFreeDataQualityTable
    *
    * Description:
    *    This utility frees a "Data Quality
    *    Table" that had been previously allocated
    *    using "gmsGetDataQualityTable".
    *-------------------------------------------*/
   void gmsFreeDataQualityTable
           (DataQualityTableType *theDQT);


   /*-------------------------------------------
    * gmsPrintDataQualityTable	
    *
    * Description:
    *    This function will print the "Dqt" object
    *    to standard out.
    *-------------------------------------------*/
   void gmsPrintDataQualityTable
           (DataQualityTableType *theDQT);

#endif
