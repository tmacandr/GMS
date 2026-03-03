/*----------------------------------------------------------------------------*/
/* File : gmsFeatureClassSchemaTable.h
/* Date : 16-Aug-99 : initial definition
/*        07-Oct-99 : Clean-up due to code-inspection
/*
/* Description:
/*    Utility to read/process any "Feature Class Schema Table" from the DCW
/*    database.
/*
/*    A "Feature Class Schema Table" defines the feature classes contained
/*    in a given coverage/area.  The records of the table specify:
/*
/*             1) the name of a feature class
/*             2) the name of two tables involved
/*                in a join-relationship
/*             3) the names of the columns used in
/*                the join-relationship
/*
/*    Note that the name of the feature class is repeated so as to specify
/*    all relationships in the schema table.  Topological relationships are
/*    not specified because they are implied by the types of the tables
/*    [i.e. the joined tables].
/*
/*    Also, if a key in the join-relationship is a compound key, column names
/*    will be listed using a backslash character ('\') as a separator.  For
/*    example, a primary key composed of two columns would be specified as
/*    "NAME\TYPE".
/*
/*    Furthermore, consider the following definitions:
/*
/*       Feature Class Schema Table:
/*       ---------------------------
/*          A table that contains the composition rules of each
/*          feature class.  This table describes the definition
/*          for each feature class and the way in which each table
/*          in a feature class relates to other tables.
/*
/*       Feature Class:
/*       --------------
/*          A set of features with a common set of attributes.
/*          A feature class consists of a set of tables that
/*          includes one or more primitive tables and one or
/*          more attribute tables.  A feature class has the same
/*          columns of attribute information for each feature.
/*          Every feature class has one and only one feature table.
/*          There are three types of feature classes: SIMPLE, COMPLEX,
/*          and TEXT.  There are three subtypes of SIMPLE feature
/*          classes: LINE, POINT, and AREA.
/*
/*       Feature:
/*       --------
/*          A model of a real world geographic entity.  It is a
/*          zero-, one-, or two-dimensional entity [with a uniform
/*          set of attributes].
/*
/*    Consider the following example.  Two records of the "FCS" file from
/*    the PO (Political/Oceans) directory of the BROWSE library are:
/*
/*                 ID            : 1
/*                 Feature-Class : POAREA
/*                 Table-1       : POAREA.AFT
/*                 Foreign-key   : ID
/*                 Table-2       : FAC
/*                 Primary-key   : ID
/*                 ------------------------
/*                 ID            : 2
/*                 Feature-Class : POAREA  
/*                 Table-1       : FAC
/*                 Foreign-key   : ID
/*                 Table-2       : POAREA.AFT
/*                 Primary-key   : ID
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef FCS

   #define FCS

   /*--------------------------------------------*/
   /* Format : Feature Class Schema Table
   /*--------------------------------------------*/

   typedef struct _featureClassRectType
      {
       int  ID;
       char featureClass[8 + 1];
       char firstRelatedTable[12 + 1];
       char foreignKeyFromTable_1[16 + 1];
       char secondRelatedTable[12 + 1];
       char primaryKeyFromTable_2[16 + 1]; 
      } featureClassRectType;

   typedef struct _featureClassSchemaType
      {
       int                  numRecords;
       featureClassRectType *featureRecords;
      } featureClassSchemaType;


   /*---------------------------------------------*/
   /* gmsGetFeatureClassSchemaTable 
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    a "feature Class Schema Table".  A pointer
   /*    to a newly allocated table is returned to
   /*    the caller.  It is the caller's responsibility
   /*    to free the item by using the utility
   /*    "gmsFreeFeatureClassSchemaTable" (see below).
   /*---------------------------------------------*/
   featureClassSchemaType *gmsGetFeatureClassSchemaTable
                              (const char *fcsTableFilePath);


   /*---------------------------------------------*/
   /* gmsFreeFeatureClassSchemaTable
   /*
   /* Description:
   /*    This utility frees a "Feature Class Schema
   /*    Table" that had been previously allocated
   /*    using "gmsGetFeatureClassSchemaTable".
   /*---------------------------------------------*/
   void gmsFreeFeatureClassSchemaTable 
           (featureClassSchemaType *theFCS);


   /*---------------------------------------------*/
   /* gmsPrintFeatureClassSchemaTable	
   /*
   /* Description:
   /*    This function will print the "Feature Class
   /*    Schema Table" object to standard out.
   /*---------------------------------------------*/
   void gmsPrintFeatureClassSchemaTable 
           (featureClassSchemaType *theFCS);

#endif
