/*--------------------------------------------------------------------------*/
/* File : gmsCoverageAttribTable.h
/* Date : 12-Jul-99 : Initial Definition
/*        01-Aug-99 : Ported to Sun Solaris
/*        07-Oct-99 : Clean-up due to code-inspection
/*
/* Description:
/*    Utilities to "read" the "Coverage Attribute Table" file from the
/*    BROWSE library of the Digital Chart of the World (DCW) database.
/*
/*    This table contains four columns (i.e. records with 4 attributes):
/*          1) Row ID
/*          2) Coverage Name
/*          3) Coverage Description
/*          4) Topology Level
/*    The coverage description is equivalent to the name of the thematic
/*    layer.
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*--------------------------------------------------------------------------*/

#ifndef CAT

   #define CAT

   typedef struct _CoverageRecordType
      {
       int      ID;
       char     coverageName[8 + 1];
       char     description[50 + 1];
       int      level;
      } CoverageRecordType;

   typedef struct _CovAttribTableType
      {
       int                numRecords;
       CoverageRecordType *theCATRecords;
      } CovAttribTableType;


   /*-------------------------------------------*/
   /* gmsGetCoverageAttribTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Coverage Attribute Table".  A pointer
   /*    to a newly allocated table is returned to
   /*    the caller.  It is the caller's responsibility
   /*    to free the item by using the utility
   /*    "gmsFreeCoverageAttribTable" (see below).
   /*-------------------------------------------*/
   CovAttribTableType *gmsGetCoverageAttribTable
                                 (const char *filePath);


   /*-------------------------------------------*/
   /* gmsFreeCoverageAttribTable 
   /*
   /* Description:
   /*    This utility frees a "Coverage Attribute
   /*    Table" that had been previously allocated
   /*    using "gmsGetCoverageAttribTable".
   /*-------------------------------------------*/
   void gmsFreeCoverageAttribTable
           (CovAttribTableType *theCAT);


   /*-------------------------------------------*/
   /* gmsPrintCoverageAttribTable	
   /*
   /* Description:
   /*    This function will print the "Cat" object
   /*    to standard out.
   /*-------------------------------------------*/
   void gmsPrintCoverageAttribTable
           (CovAttribTableType *theCAT);

#endif
