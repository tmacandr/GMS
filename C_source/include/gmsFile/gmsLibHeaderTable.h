/*----------------------------------------------------------------------------*/
/* File : gmsLibHeaderTable.h
/* Date : 26-Jul-99 : Initial Definition
/*        01-Aug-99 : Convert to utilities on port to Sun Solaris
/*        05-Oct-99 : Clean-up due to code inspection
/*
/* Description:
/*    Utilities to "read" the "Library Header Table" file of the Digital
/*    Chart of the World (DCW).  The "Library Header Table" contains the
/*    data set, source and security information for the library.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef LHT

   #define LHT

   typedef struct _libHeaderTableType
      {
       int      ID;
       char     productType[12 + 1];
       char     libraryName[12 + 1];
       char     description[100 + 1];
       char     dataStructCode;
       int      scale;
       char     sourceSeries[15 + 1];
       char     sourceID[30 + 1];
       char     sourceEdition[20 + 1];
       char     sourceName[100 + 1];
       char     sourceDate[20 + 1];
       char     securityClassif;
       char     downgrading[3 + 1];
       char     downgradeDate[20 + 1];
       char     releasability[20 + 1];
      } libHeaderTableType;


   /*---------------------------------------------*/
   /* gmsGetLibHeaderTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Library Header Table".  A pointer
   /*    to a newly allocated table is returned to
   /*    the caller.  It is the caller's responsibility
   /*    to free the item by using the utility
   /*    "gmsFreeLibHeaderTable" (see below).
   /*---------------------------------------------*/
   libHeaderTableType *gmsGetLibHeaderTable
                                 (const char *filePath);


   /*---------------------------------------------*/
   /* gmsFreeLibHeaderTable 
   /*
   /* Description:
   /*    This utility frees a "Library Header
   /*    Table" that had been previously allocated
   /*    using "gmsGetLibHeaderTable".
   /*---------------------------------------------*/
   void gmsFreeLibHeaderTable
                  (libHeaderTableType *theLHT);


   /*---------------------------------------------*/
   /* gmsPrintLibHeaderTable	
   /*
   /* Description:
   /*    This function will print the "Lht" object
   /*    to standard out.
   /*---------------------------------------------*/
   void gmsPrintLibHeaderTable
           (libHeaderTableType *theLHT);

#endif
