/*----------------------------------------------------------------------------*/
/* File : gmsMinBoundRectTable.h
/* Date : 06-Aug-99 : initial definition
/*
/* Description:
/*    Utilities to "read" any "Minimum-Bounding-Rectangle" file of the Digital
/*    Chart of the World (DCW).  This includes:
/*
/*                Edge Bounding Rectangles
/*                Face Bounding Rectangles
/*
/*    Consider the following definition(s):
/*
/*       Minimum Bounding Rectangle:
/*       ---------------------------
/*          A rectangle of coordinate tuples that defines
/*          the minimum and maximum coordinates of an entity.
/*          Abbreviated as MBR.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef MBR

   #define MBR

   /*--------------------------------------------*/
   /* Format : Minimum Bounding Rectangle;
   /*       ID  =I,1,P,Row ID              ,-,-,:
   /*       XMIN=F,1,N,Minimum X Coordinate,-,-,:
   /*       YMIN=F,1,N,Minimum Y Coordinate,-,-,:
   /*       XMAX=F,1,N,Maximum X Coordinate,-,-,:
   /*       YMAX=F,1,N,Maximum Y Coordinate,-,-,:
   /*--------------------------------------------*/

   typedef struct _minBoundRectType
      {
       int   ID;
       float xMin;
       float yMin;
       float xMax;
       float yMax;
      } minBoundRectType;

   typedef struct _minBoundRectTableType
      {
       int               numRecords;
       minBoundRectType *mbrRecords;
      } minBoundRectTableType;


   /*---------------------------------------------*/
   /* gmsGetMbrTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Minimum Bounding Rectangle Table".  A
   /*    pointer to a newly allocated table is returned
   /*    to the caller.  It is the caller's responsibility
   /*    to free the item by using the utility
   /*    "gmsFreeMbrTable" (see below).
   /*---------------------------------------------*/
   minBoundRectTableType *gmsGetMbrTable
                           (const char *mbrTableFilePath);


   /*---------------------------------------------*/
   /* gmsFreeMbrTable
   /*
   /* Description:
   /*    This utility frees a "Edge Table" that had
   /*    been previously allocated using
   /*    "gmsGetMbrTable".
   /*---------------------------------------------*/
   void gmsFreeMbrTable
           (minBoundRectTableType *theMbrTable);


   /*---------------------------------------------*/
   /* gmsPrintMbrTable	
   /*
   /* Description:
   /*    This function will print the "MBR Table"
   /*    object to standard out.
   /*---------------------------------------------*/
   void gmsPrintMbrTable
           (minBoundRectTableType *theMbrTable);

#endif
