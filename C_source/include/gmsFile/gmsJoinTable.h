/*----------------------------------------------------------------------------*/
/* File : gmsJoinTable.h
/* Date : 26-Aug-99 : initial definition
/*
/* Description:
/*    Utilities to read/process any "Join Table" file of the Digital Chart of
/*    the World (DCW) database.  A "Join Table" is a file of n records of the
/*    following format:
/*
/*       1) Primary Key to primitive is 32-bit integer
/*
/*       2) Primary key to primitive is 16-bit integer
/*
/*       3) Primary key to primitive is triplet-ID record
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef JOIN

   #define JOIN 

   typedef struct _joinRecordType
      {
       int       ID;
       int       featureKey;
       short int tileReferenceId;
       int       primaryKeyOfPrimitive;
      } joinRecordType;

   typedef struct _joinTableType
      {
       int            numRecords;
       joinRecordType *joinRecords;
      } joinTableType;


   /*---------------------------------------------*/
   /* gmsGetJoinTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Join Table".  A pointer to a newly
   /*    allocated table is returned to the caller.
   /*    It is the caller's responsibility to free
   /*    the item by using the utility
   /*    "gmsFreeJoinTable" (see below).
   /*---------------------------------------------*/
   joinTableType *gmsGetJoinTable
                           (const char *joinTableFilePath);


   /*---------------------------------------------*/
   /* gmsFreeJoinTable
   /*
   /* Description:
   /*    This utility frees a "Join Table" that had
   /*    been previously allocated using
   /*    "gmsGetJoinTable".
   /*---------------------------------------------*/
   void gmsFreeJoinTable
           (joinTableType *theJoinTable);


   /*---------------------------------------------*/
   /* gmsPrintJoinTable	
   /*
   /* Description:
   /*    This function will print the "Join Table"
   /*    object to standard out.
   /*---------------------------------------------*/
   void gmsPrintJoinTable
           (joinTableType *theJoinTable);

#endif
