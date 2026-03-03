/*----------------------------------------------------------------------------*/
/* File : gmsFaceTable.h
/* Date : 07-Aug-99 : initial definition
/*        21-Sep-99 : browse record is different from regional record
/*        12-Oct-99 : improve comments
/*        13-Oct-99 : routine figures out which format of record
/*
/* Description:
/*    Utilities to "read" any "Face Table" file of the Digital Chart of
/*    the World (DCW).  Consider the following definition:
/*
/*       Face:
/*       -----
/*          A region enclosed by an edge or set of
/*          edges.  Faces are topologically linked
/*          to their surrounding edges as well as to
/*          the other faces that surround them.  Faces
/*          are always non-overlappoing, exhausting
/*          the area of a plane.
/*
/*    There are two types of face tables.  The first is used for the BROWSE
/*    library.  Its format is:
/*
/*       BROWSE Face Record:
/*       -------------------
/*             ID      =I,1,P,Row ID                   ,-,-,
/*             RING_PTR=I,1,F,Foreign Key to Ring Table,-,-,
/*
/*    The second type is used for the other REGIONAL libraries.  Its format
/*    is:
/*
/*       REGIONAL Face Record:
/*       ---------------------
/*             ID      =I,1,P,Row ID                           ,-,-
/*             *.AFT_ID=I,1,F,Foreign Key to Area Feature Table,-,-,
/*             RING_PTR=I,1,F,Foreign Key to Ring Table        ,-,-
/*
/*             where * is the name of the corresponding feature class.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef FAC

   #define FAC

   typedef struct _faceRecordType
      {
       int ID;
       int fgnKeyToFeatureTbl; /* -1 if face is browse */
       int fgnKeyToRngTable;
      } faceRecordType;

   typedef struct _faceTableType
      {
       int            numRecords;
       faceRecordType *faceRecords;
      } faceTableType;


   /*---------------------------------------------*/
   /* gmsGetFaceTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Face Table".  This routine determines
   /*    which format of face-record to use to build
   /*    the table based on the specified file.
   /*
   /*    A pointer to a newly allocated table is
   /*    returned to the caller.  It is the caller's
   /*    responsibility to free the item by using
   /*    the utility
   /*    "gmsFreeFaceTable" (see below).
   /*---------------------------------------------*/
   faceTableType *gmsGetFaceTable
                           (const char  *faceTableFilePath);


   /*---------------------------------------------*/
   /* gmsFreeFaceTable
   /*
   /* Description:
   /*    This utility frees a "Face Table" that had
   /*    been previously allocated using
   /*    "gmsGetFaceTable".
   /*---------------------------------------------*/
   void gmsFreeFaceTable
           (faceTableType *theFaceTable);


   /*---------------------------------------------*/
   /* gmsPrintFaceTable	
   /*
   /* Description:
   /*    This function will print the "Face Table"
   /*    object to standard out.
   /*---------------------------------------------*/
   void gmsPrintFaceTable
           (faceTableType *theFaceTable);

#endif
