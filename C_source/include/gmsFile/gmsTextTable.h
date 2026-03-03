/*----------------------------------------------------------------------------*/
/* File : gmsTextTable.h
/* Date : 17-Aug-99 : Initial definition
/*        07-Oct-99 : Clean-up due to code-inspection
/*        12-Oct-99 : Corrected some comments
/*        13-Oct-99 : routine figures out which format of record
/*
/* Description:
/*    Utilities to "read/process" any "Text Table" file of the Digital Chart
/*    of the World (DCW) database.  There are two type of TEXT tables.  One
/*    is for the BROWSE library and the second is for the REGIONAL libraries.
/*
/*       Text Feature:
/*       -------------
/*          A cartographic entity that relates a textual
/*          description to a zero- or one-dimensional location.
/*          A text feature usually contains information such as
/*          font, color, and height [i.e. primitives].
/*
/*       Text Feature Class:
/*       -------------------
/*          A collection of text features that maintains a
/*          homogeneous set of attributes.  [i.e. it is a set
/*          of text primitives].
/*
/*    A BROWSE "Text Table" is an array of records of the format:
/*
/*       Browse : Text Primitive:
/*       ------------------------
/*          ID           =I, 1, P, Row ID,-,-,:
/*          <num-chars>  =I, 1, N, Number of chars in STRING:
/*          STRING       =T, *, N, Text String,-,-,:
/*          <num-coords> =I, 1, N, Number of coordinates in SHAPE_LINE:
/*          SHAPE_LINE   =C, *, N, Shape Coordinate String,-,-,:
/*
/*    A "Text Table" from a REGIONAL library has the format:
/*
/*       Regional : Text Primitive:
/*       -----------------------
/*          ID          =I,1,P,Row Identifier,-,-,:
/*          *TEXT.TFT_ID=I,1,F,Foreign Key to Text Feature Table,-,-,:
/*          <num-chars> =I,1,N,Number of chars in STRING:
/*          STRING      =T,*,N,Text String,-,-,:
/*          <num-coords>=I,1,N,Number of coordinates in SHAPE_LINE:
/*          SHAPE_LINE  =C,*,N,Shape of Text String,-,-,:;
/*
/*          where the * in "*TEXT.TFT_ID" corresponds to the theme
/*          of the text feature table.
/*
/*    The "string" is an array of n characters.  The "shape-line" is an array
/*    of coordinates (2-dimensional) that indicate the placement of the
/*    string (on the map).  The "shape-line" has 1 .. n coordinates.  If
/*    only one coordinate is defined, then it is the lower-left point in
/*    the string.  If two coordinates are defined, the second coordinate
/*    defines the lower right of the string.  If 3 or more coordinates are
/*    defined, then the array defines a curve and the 2 .. (n - 1) points
/*    define the control points of the curve.
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef TXT

   #define TXT

   #include <gmsUtilities.h>

   typedef struct _textRecordType
      {
       int             ID;
       int             fgnKeyToFeatureTable; /* -1 if browse       */
       int             numChars;
       char            *theString;           /* is null terminated */
       int             numCoords;
       twoDimCoordType *theCoords;
      } textRecordType;

   typedef struct _textTableType
      {
       int            numRecords;
       textRecordType *textRecords;
      } textTableType;


   /*---------------------------------------------*/
   /* gmsGetTextTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Text Table".  This routine determines
   /*    the corresponding "Text Index File" (TXX)
   /*    based on the specified "Text Table".  Also,
   /*    this routine determines which format to use
   /*    for the records of the Text Table.
   /*
   /*    It is the caller's responsibility to free
   /*    the item by using the utility
   /*    "gmsFreeTextTable" (see below).
   /*---------------------------------------------*/
   textTableType *gmsGetTextTable
                           (const char *textTableFilePath);


   /*---------------------------------------------*/
   /* gmsFreeTextTable
   /*
   /* Description:
   /*    This utility frees a "Text Table" that had
   /*    been previously allocated using
   /*    "gmsGetTextTable" (see above).
   /*---------------------------------------------*/
   void gmsFreeTextTable
           (textTableType *theTextTbl);


   /*---------------------------------------------*/
   /* gmsPrintTextTable	
   /*
   /* Description:
   /*    This function will print the "Text Table
   /*    object to standard out.
   /*---------------------------------------------*/
   void gmsPrintTextTable
           (textTableType *theTextTbl);

#endif
