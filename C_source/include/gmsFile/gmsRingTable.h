/*----------------------------------------------------------------------------*/
/* File : gmsRingTable.h
/* Date : 18-Aug-99 : Initial definition
/*        05-Oct-99 : Clean-up due to code inspection
/*
/* Description:
/*    Utilities to "read" any "Ring Table" file of the Digital Chart of the
/*    World (DCW) database.  A "Ring Table" is a file of n records of the
/*    following format:
/*
/*             ID        =I, 1, P, Row ID,-,-,:
/*             FACE_ID   =I, 1, F, Foreign key to face table,-,-,:
/*             START_EDGE=I, 1, F, Foreign Key to Edge Table,-,-,:
/*
/*    A ring table contains one reference to the edge table for each ring of
/*    a face.  The first row in the ring table for each face refers to the
/*    outer ring of that face.  Outer rings are traversed in clockwise
/*    direction.  Each inner ring has one reference to a first edge on
/*    that ring.  The ring table maintains an order relationship for its
/*    rows.  The first record of a new face id will always be defined as the
/*    outer ring.  Any repeating records with an identical face value will
/*    define inner rings.
/*
/*    Also, consider the following:
/*
/*       Ring:
/*       -----
/*          A connected set of edges that composes a closed
/*          face border.  Any single ring is only referenced
/*          to and by a single face.  If the same set of edges
/*          is shared by two different faces, two rings that
/*          correspond to the two faces are created from the
/*          single edge set.  Rings only occur at level 3
/*          topology (when faces are also present).
/*
/*       Face:
/*       -----
/*          A region enclosed by an edge or set of edges.
/*          Faces are topologically linked to their surrounding
/*          edges as well as to the other faces that surround them.
/*          Faces are always nonoverlapping, exhausting the area
/*          of a plane
/*
/*       Edge:
/*       -----
/*          A one-dimensional primitive used to represent the
/*          location of a linear feature and/or the borders of
/*          faces.  Depending upon the level of topology, edges
/*          may be linked to nodes, [other] edges, and faces.
/*          Edges are composed of an ordered collection of two or
/*          more coordinate tuples (pairs or triplets).  At least
/*          two of the coordinate tuples must be distinct.  The
/*          orientation of an edge can be recognized by the
/*          ordering of the coordinate tuples.  [i.e. the first
/*          coordinate is the START node, the last coordinate is
/*          the END node.]
/*
/*    Reference:
/*        1) Mil-Std-600006: Section 5.3.2.3(b) - pg 54
/*        2) Mil-D-89009   : Section 6.4 - pg 57-68
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef RNG

   #define RNG

   typedef struct _ringRecordType
      {
       int ID;
       int faceId;
       int startEdge;
      } ringRecordType;

   typedef struct _ringTableType
      {
       int            numRecords;
       ringRecordType *ringRecords;
      } ringTableType;


   /*---------------------------------------------*/
   /* gmsGetRingTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Ring Table".  A pointer to a newly
   /*    allocated table is returned to the caller.
   /*    It is the caller's responsibility to free
   /*    the item by using the utility
   /*    "gmsFreeRingTable" (see below).
   /*---------------------------------------------*/
   ringTableType *gmsGetRingTable
                           (const char *ringTableFilePath);


   /*---------------------------------------------*/
   /* gmsFreeRingTable
   /*
   /* Description:
   /*    This utility frees a "Ring Table" that had
   /*    been previously allocated using
   /*    "gmsGetRingTable".
   /*---------------------------------------------*/
   void gmsFreeRingTable
           (ringTableType *theRingTable);


   /*---------------------------------------------*/
   /* gmsPrintRingTable	
   /*
   /* Description:
   /*    This function will print the "Ring Table"
   /*    object to standard out.
   /*---------------------------------------------*/
   void gmsPrintRingTable
           (ringTableType *theRingTable);

#endif
