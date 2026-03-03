/*--------------------------------------------------------------------------*/
/* File : gmsDbHeaderTable.h
/* Date : 07-Apr-99 : Initial Definition
/*        14-Jun-99 : Move from Turbo-C to VC++
/*        07-Jul-99 : Converted to class
/*        01-Aug-99 : Converted back to utility on port to Sun Solaris
/*        05-Oct-99 : Clean-up due to code inspection
/*
/* Description:
/*    Utilities to "read" the "Database Header Table" file of the
/*    Digital Chart of the World (DCW).
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*--------------------------------------------------------------------------*/

#ifndef DBH

   #define DBH

   typedef struct _DbHeaderTableType
      {
       int  ID;
       char databaseName[8 + 1];
       char databaseDesc[100 + 1];
       char mediaStandard[20 + 1];
       char originator[50 + 1];
       char addressee[100 + 1];
       char mediaVolumes;
       char seqNumbers;
       char numDataSets;
       char securityClass;
       char downgrading[3 + 1];
       char downgradeDate[20 + 1];
       char releasability[20 + 1];
       char otherStdName[50 + 1];
       char otherStdDate[20 + 1];
       char otherStdVersion[10 + 1];
       char transmittalId;
       char editionNumber[10 + 1];
       char editionDate[20 + 1];
      } DbHeaderTableType;


   /*-------------------------------------------*/
   /* gmsGetDbHeaderTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Database Header Table".  A pointer
   /*    to a newly allocated table is returned to
   /*    the caller.  It is the caller's responsibility
   /*    to free the item by using the utility
   /*    "gmsFreeDbHeaderTable" (see below).
   /*-------------------------------------------*/
   DbHeaderTableType *gmsGetDbHeaderTable
                                (const char *filePath);


   /*-------------------------------------------*/
   /* gmsFreeDbHeaderTable
   /*
   /* Description:
   /*    This utility frees a "Database Header
   /*    Table" that had been previously allocated
   /*    using "gmsGetDbHeaderTable".
   /*-------------------------------------------*/
   void gmsFreeDbHeaderTable
           (DbHeaderTableType *theDHT);


   /*-------------------------------------------*/
   /* gmsPrintDbHeaderTable	
   /*
   /* Description:
   /*    This function will print the "dht" object
   /*    to standard out.
   /*-------------------------------------------*/
   void gmsPrintDbHeaderTable
           (DbHeaderTableType *theDHT);

#endif
