/*--------------------------------------------------------------------------*/
/* File : gmsGeoReferenceTable.h
/* Date : 11-Jul-99 : Initial Definition
/*        01-Aug-99 : Convert to utility on port to Sun Solaris
/*
/* Description:
/*    Utilities to "read" the "Geographic Reference Table" file of the
/*    Digital Chart of the World (DCW).
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*--------------------------------------------------------------------------*/

#ifndef GRT

   #define GRT

   typedef struct _GeoRefTblType
      {
       int      ID;
       char     dataType[3 + 1];
       char     units[3 + 1];
       char     ellipsoid[15 + 1];
       char     ellipsoidDetails[50 + 1];
       char     datumVerticalReference[15 + 1];
       char     verticalDatumCode[3 + 1];
       char     soundingDatum[15 + 1];
       char     soundingDatumCode[3 + 1];
       char     datumGeodeticName[15 + 1];
       char     datumGeodeticCode[3 + 1];
       char     projectionName[20 + 1];
      } GeoRefTableType;


   /*-------------------------------------------*/
   /* gmsGetGeoReferenceTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Geographic Reference Table".  A pointer
   /*    to a newly allocated table is returned to
   /*    the caller.  It is the caller's responsibility
   /*    to free the item by using the utility
   /*    "gmsFreeGeoReferenceTable" (see below).
   /*-------------------------------------------*/
   GeoRefTableType *gmsGetGeoReferenceTable
                             (const char *filePath);


   /*-------------------------------------------*/
   /* gmsFreeGeoReferenceTable 
   /*
   /* Description:
   /*    This utility frees a "Geographic Reference
   /*    Table" that had been previously allocated
   /*    using "gmsGetGeoReferenceTable".
   /*-------------------------------------------*/
   void gmsFreeGeoReferenceTable
           (GeoRefTableType *theGRT);


   /*-------------------------------------------*/
   /* gmsPrintGeoReferenceTable	
   /*
   /* Description:
   /*    This function will print the "grt" object
   /*    to standard out.
   /*-------------------------------------------*/
   void gmsPrintGeoReferenceTable 
           (GeoRefTableType *theGRT);

#endif
