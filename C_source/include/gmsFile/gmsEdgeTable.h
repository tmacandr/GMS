/*----------------------------------------------------------------------------*/
/* File : gmsEdgeTable.h
/* Date : 26-Jul-99 : Initial Definition
/*        01-Aug-99 : Convert to utilites on port to Sun Solaris
/*        18-Aug-99 : "stumbled" on attribute "Number-of-Coordinates"  
/*        07-Oct-99 : Clean-up due to code-inspection
/*        12-Oct-99 : There are 4 variations to the edge record
/*
/* Description:
/*    Utilities to "read" any "Edge Table" file of the Digital Chart of the
/*    World (DCW).  Consider the following definitions:
/*
/*       Edge:
/*       -----
/*          A one-dimensional primitive used to represent the
/*          location of a linear feature and/or the borders
/*          of faces.  Depending upon the level of topology,
/*          edges may be topologically linked to nodes, [other]
/*          edges, and faces.  Edges are composed of an ordered
/*          collection of two or more coordinate tuples (pairs
/*          or tiplets).  At least two of the coordinate tuples
/*          must be distinct.  The orientation of an edge can be
/*          recognized by the ordering of the coordinate tuples
/*          [i.e. the first tuple is the start node and the last
/*          tuple is the end node].
/*
/*       Coordinate:
/*       -----------
/*          A specified position in Cartesian space.  The
/*          value takes the form of a short or long floating
/*          point value.  A Z value (if any) is ignored during
/*          the enforcement and use of planar graphs.
/*
/*       Entity:
/*       -------
/*          The terminating node of an edge.
/*
/*    There are three types of edge records in the DCW database.  The first
/*    describes edge records at level-3 topology.  The format is:
/*
/*       Topology level 3 Edge Primitive Table;-;
/*       -----------------------------------------
/*             ID          =I,1,P,Row ID,-,-,:
/*             START_NODE  =I,1,N,"Start/Left Node",-,-,:
/*             END_NODE    =I,1,N,"End/Right Node",-,-,:
/*             RIGHT_FACE  =K,1,F,Right Face,-,-,:
/*             LEFT_FACE   =K,1,F,Left Face,-,-,:
/*             RIGHT_EDGE  =K,1,F,Right Edge from End Node,-,-,:
/*             LEFT_EDGE   =K,1,F,Left Edge from Start Node,-,-,:
/*             [num-coords]=I,1,N,Number of coords in array
/*             COORDINATES =C,*,N,Edge Coordinates,-,-,:;
/*
/*          This format applies to the following themes from REGIONAL
/*          libraries:
/*
/*                    VG, PP, LC, and TileRef
/*
/*          Also, this format applies to the following themes from the
/*          BROWSE library:
/*
/*                    B-PO      B-GR
/*                    B-LibRef  B-IN
/*                    B-DV      B-DN
/*                    B-DA      B-CO
/*
/*    The second variant makes use of the optional attribute called "Foreign
/*    Key to Line Feature Table".  It too is a level-3 topology record.  The
/*    format is:
/*
/*       Level 3 topology with Fgn Key to Feature Table:
/*       -----------------------------------------------
/*             ID           =I,1,P,Row Identifier,-,-,:
/*             *LINE.LFT_ID =I,1,F,Foreign Key to <Line> Feature Table,-,-,:
/*             START_NODE   =I,1,F,Start/Left Node,-,-,:
/*             END_NODE     =I,1,F,End/Right Node,-,-,:
/*             RIGHT_FACE   =K,1,F,Right Face,-,-,:
/*             LEFT_FACE    =K,1,F,Left Face,-,-,:
/*             RIGHT_EDGE   =K,1,F,Right Edge from End Node,-,-,:
/*             LEFT_EDGE    =K,1,F,Left Edge from Start Node,-,-,:
/*             [num-coords] =I,1,N,Number of coords in array
/*             COORDINATES  =C,*,N,Coordinates of Edge,-,-,:;
/*
/*          This format applies to the following themes from REGIONAL
/*          libraries:
/*
/*                    PO, HY, DQ, DN, CL
/*
/*    The third variant describes edge records at level 1-2 topology.  However,
/*    this format also makes use of the optional attribute "Foreign Key to
/*    Line Feature Table"  The format is:
/*
/*       Level 1-2 topology - Edge Primitives:
/*       -------------------------------------
/*             ID           =I,1,P,Row Identifier,-,-,:
/*             *LINE.LFT_ID =I,1,F,Foreign Key to Line Feature Table,-,-,:
/*             START_NODE   =I,1,F,Start/Left Node,-,-,:
/*             END_NODE     =I,1,F,End/Right Node,-,-,:
/*             RIGHT_EDGE   =K,1,F,Right Edge from End Node,-,-,:
/*             LEFT_EDGE    =K,1,F,Left Edge from Start Node,-,-,:
/*             [num-coords] =I,1,N,Number of coords in array
/*             COORDINATES  =C,*,N,Coordinates of Edge,-,-,:;
/*
/*          This format applies to the following themes from REGIONAL
/*          libraries:
/*
/*                    UT, TS, RR, RD, PH, OF, HS
/*
/*    The third variant describes edge records from the LIBREF theme of a
/*    REGIONAL library.  It is a level 1-2 record but does NOT make use of
/*    the optional attribute "Foreign Key to Line Feature Table".  The
/*    format is:
/*
/*       LibRef - Edge Primitive;-;
/*       --------------------------
/*             ID           =I,1,P,Row Identifier,-,-,:
/*             START_NODE   =I,1,F,Start/Left Node,-,-,:
/*             END_NODE     =I,1,F,End/Right Node,-,-,:
/*             RIGHT_EDGE   =K,1,F,Right Edge from End Node,-,-,:
/*             LEFT_EDGE    =K,1,F,Left Edge from Start Node,-,-,:
/*             [num-coords] =I,1,N,Number of coords in array
/*             COORDINATES  =C,*,N,Coordinates of Edge,-,-,:;
/*
/*          This format applies ONLY to the LibRef theme from a
/*          REGIONAL library.
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef EDG

   #define EDG

   #include <gmsUtilities.h>

   typedef struct _edgeRecordHeaderType
      {
       int            ID;
       int            fgnKeyToFeatureTbl;  /* -1 if BROWSE record */
       int            startNode;
       int            endNode;
       idTripletType  rightFace;           /* cleared if not level-3 */
       idTripletType  leftFace;            /* cleared if not level-3 */
       idTripletType  rightEdge;
       idTripletType  leftEdge;
      } edgeRecordHeaderType;

   typedef struct _edgeRecordType
      {
       edgeRecordHeaderType headerInfo;
       int                  numCoords;
       twoDimCoordType      *theCoords;
      } edgeRecordType;

   typedef struct _edgeTableType
      {
       int            numRecords;
       edgeRecordType *edgeRecords;
      } edgeTableType;


   /*---------------------------------------------*/
   /* gmsGetEdgeTable
   /*
   /* Description:
   /*    This utility reads the file that contains
   /*    the "Edge Table" (EDG).  This routine will
   /*    determine which "Edge Index" (EDX) file to
   /*    use based on the name of the EDG file.
   /*    Furthermore, this routine will determine
   /*    which of the 4 formats of edge records is
   /*    applicable.
   /*
   /*    It is the caller's responsibility
   /*    to free the item by using the utility
   /*    "gmsFreeEdgeTable" (see below).
   /*---------------------------------------------*/
   edgeTableType *gmsGetEdgeTable
                           (const char *edgeTableFilePath);


   /*---------------------------------------------*/
   /* gmsFreeEdgeTable
   /*
   /* Description:
   /*    This utility frees an "Edge Table" that had
   /*    been previously allocated using
   /*    "gmsGetEdgeTable".
   /*---------------------------------------------*/
   void gmsFreeEdgeTable
           (edgeTableType *theEdgTbl);


   /*---------------------------------------------*/
   /* gmsPrintEdgeTable	
   /*
   /* Description:
   /*    This function will print the "Edge Table
   /*    object to standard out.
   /*---------------------------------------------*/
   void gmsPrintEdgeTable
           (edgeTableType *theEdgTbl);

#endif
