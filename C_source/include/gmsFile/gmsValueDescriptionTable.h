/*----------------------------------------------------------------------------*/
/* File : gmsValueDescriptionTable.h
/* Date : 24-Aug-99 : Initial definition
/*        19-Sep-99 : Browse char VDT different from main themes
/*        07-Oct-99 : Clean-up due to code-inspection
/*        16-Nov-99 : Access "gmsUtilities.h" for 'bool' def.  ANSI C comiler.
/*
/* Description:
/*    These utilities are used to "read/process" any "Value Description
/*    Table" file of the Digital Chart of the World (DCW) database.  There
/*    are two types of "Value Description Tables".  The first is an "Integer
/*    VDT" and the other is a "character VDT".  Each record in the "Integer"
/*    VDT has the format:
/*
/*           Integer Value Description Record
/*               ID         =I,1,N,Row Identifier,-,-,:
/*               TABLE      =T,12,P,Name of Feature Table,-,-,:
/*               ATTRIBUTE  =T,16,P,Attribute Name,-,-,:
/*               VALUE      =I,1,P,Attribute Value,-,-,:
/*               DESCRIPTION=T,50,N,Attribute Value Description,-,-,:
/*
/*    There are two sub-types for the "Character" VDT.  For the BROWSE
/*    library the record format is:
/*
/*           Character Value Description Record
/*               ID         =I,1,N,Row Identifier,-,-,:
/*               TABLE      =T,12,P,Name of Feature Table,-,-,:
/*               ATTRIBUTE  =T,16,P,Attribute Name,-,-,:
/*               VALUE      =T,3,P,Attribute Value,-,-,:
/*               DESCRIPTION=T,50,N,Attribute Value Description,-,-,:;
/*
/*    However, for the main themes, the record format is:
/*
/*           Character Value Description Table;-;
/*              ID         =I,1,N,Row Identifier,-,-,:
/*              TABLE      =T,12,P,Feature Table Name,-,-,:
/*              ATTRIBUTE  =T,16,P,Attribute Name,-,-,:
/*              VALUE      =T,2,P,Attribute Value,-,-,:
/*              DESCRIPTION=T,50,N,Attribute Value Description,-,-,:;
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef VDT

   #define VDT 

   #include <gmsUtilities.h>  /* only to access 'bool' def for ANSI C */

   typedef struct _intVdtRecordType
      {
       int  ID;
       char table[12 + 1];
       char attribute[16 + 1];
       int  value;
       char description[50 + 1];
      } intVdtRecordType;

   typedef struct _intValueDescriptionTableType
      {
       int              numRecords;
       intVdtRecordType *vdtRecords;
      } intValueDescriptionTableType;

   typedef struct _charVdtRecordType
      {
       int  ID;
       char table[12 + 1];
       char attribute[16 + 1];
       char value[3 + 1];       /* big enough for both */
       char description[50 + 1];
      } charVdtRecordType;

   typedef struct _charValueDescriptionTableType
      {
       int              numRecords;
       charVdtRecordType *vdtRecords;
      } charValueDescriptionTableType;


   /*---------------------------------------------*/
   /* gmsGetIntValueDescriptionTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    an INTEGER "Value Description Table".  A
   /*    pointer to a newly allocated table is
   /*    returned to the caller.  It is the caller's
   /*    responsibility to free the item by using the
   /*    utility "gmsFreeValueDescriptionTable"
   /*    (see below).
   /*---------------------------------------------*/
   intValueDescriptionTableType *gmsGetIntValueDescriptionTable
                                    (const char *intVdtFilePath);


   /*---------------------------------------------*/
   /* gmsFreeIntValueDescriptionTable
   /*
   /* Description:
   /*    This utility frees an "Integer Value Description
   /*    Table" that had been previously allocated
   /*    using "gmsGetIntValueDescriptionTable"
   /*   (see above).
   /*---------------------------------------------*/
   void gmsFreeIntValueDescriptionTable 
           (intValueDescriptionTableType *theVDT);


   /*---------------------------------------------*/
   /* gmsPrintIntValueDescriptionTable
   /*
   /* Description:
   /*    This function will print the "Integer Value
   /*    Description Table" object to standard out.
   /*---------------------------------------------*/
   void gmsPrintIntValueDescriptionTable 
           (intValueDescriptionTableType *theVDT);


   /*---------------------------------------------*/
   /* gmsGetCharValueDescriptionTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    an CHARACTER "Value Description Table".  The
   /*    caller must also specify if the file is from
   /*    the BROWSE library.  A pointer to a newly
   /*    allocated table is returned to the caller.
   /*    It is the caller's responsibility to free the
   /*    item by using the utility
   /*    "gmsFreeCharValueDescriptionTable" (see below).
   /*---------------------------------------------*/
   charValueDescriptionTableType *gmsGetCharValueDescriptionTable
                                    (const char *charVdtFilePath,
                                     bool       isBrowse);


   /*---------------------------------------------*/
   /* gmsFreeCharValueDescriptionTable
   /*
   /* Description:
   /*    This utility frees an "Character Value
   /*    Description Table" that had been previously
   /*    allocated using "gmsGetCharValueDescriptionTable"
   /*   (see above).
   /*---------------------------------------------*/
   void gmsFreeCharValueDescriptionTable
           (charValueDescriptionTableType *theVDT);


   /*---------------------------------------------*/
   /* gmsPrintCharValueDescriptionTable
   /*
   /* Description:
   /*    This function will print the "Character Value
   /*    Description Table" object to standard out.
   /*---------------------------------------------*/
   void gmsPrintCharValueDescriptionTable
           (charValueDescriptionTableType *theVDT);

#endif
