//----------------------------------------------------------------------------*/
// File : gmsNodeTable.h
// Date : 18-Aug-99 : initial definition
//        20-Sep-99 : browse, gazette, themes ... different
//
// Description:
//    Utilities to read/process any "Node Table" file of the Digital Chart of
//    the World (DCW) database.  Consider the following definitions:
//
//       Node:
//       -----
//          A zero-dimensional geometric primitive that is
//          composed of a single coordinate tuple (pair or
//          triplet).  There are two types of nodes:
//                     1) Entity Nodes
//                     2) Connected Nodes
//
//       Entity Node:
//       ------------
//          A node that represents isolated features that are
//          zero-dimensional at a particular scale.  Entity nodes
//          are topologically linked to a containing face when face
//          topology is present.  Entity nodes can reside at any
//          location, whether or not there is another primitive at
//          that same location.
//
//       Connected Node:
//       ---------------
//          A node that represents linked features that are zero
//          dimensional at a specified scale.  Connected nodes
//          are always found at the ends of edges and are topologically
//          linked to the edges.  Connected nodes are used in two
//          ways:
//                 1) to define edges topologically (always).
//                    Such nodes are referred to as START and
//                    END nodes.
//                 2) to represent point features that are
//                    found at a juncture of linear features,
//                    such as overpasses, locks in a canal,
//                    etc.  Attributes are associated with
//                    the point features of the connected nodes.
//                    If many edges intersect a node, only one
//                    edge will be maintained per node.  Other
//                    edges are linked using "Winged-Edge
//                    Topology".
//
//    There are three classes of "node tables".  One is specific to the
//    BROWSE library.  The second is specific for the GAZETTE library.
//    The other is specific to the remaining 17 themes.
//
//    A BROWSE node record is:
//    ------------------------
//       Level 1-2 topology Entity Node Primitive Table
//          ID             =I,1,P,Row ID,-,-,:
//          CONTAINING_FACE=X,1,N,Null column,-,-,:(null)
//          FIRST_EDGE     =X,1,N,Null column,-,-,:(null)
//          COORDINATE     =C,1,N,Node x and y location,-,-,:
//
//    A GAZETTE node record is:
//    -------------------------
//       Entity Node Primitives;-;
//       ID             =I, 1,P,Row Identifier,-,-,:
//       CONTAINING_FACE=I, 1,F,Foreign Key to Face Table,-,-,:
//       FIRST_EDGE     =X, 1,N,Foreign Key to Edge Table (null),-,-,:
//       COORDINATE     =C, 1,N,Coordinates of Node,-,-,:;
//
//    A thematic node record is:
//    --------------------------
//       Entity Node Primitives;-;
//          ID             =I,1,P,Row Identifier,-,-,:
//          XXX.PFT_ID     =I,1,F,Foreign Key to Point Feature Table,-,-,:
//          CONTAINING_FACE=I,1,F,Foreign Key to Face Table,-,-,:
//          FIRST_EDGE     =X,1,N,Foreign Key to Edge Table (null),-,-,:
//          COORDINATE     =C,1,N,Coordinates of Node,-,-,:;
//
//       where XXX = the corresponding name of the 'point-feature class'.
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#ifndef NODE

   #define NODE 

   #include <gmsUtilities.h>

   typedef enum { gmsBrowseNode,
                  gmsGazetteNode,
                  gmsThemeNode } gmsNodeType;

   typedef struct _nodeRecordType
      {
       int             ID;
       int             fgnKeyToPointFeatureTbl; // -1 if not theme  */
       int             containingFace;          // -1 if browse     */
       // void         firstEdge; */
       twoDimCoordType coordinate;
      } nodeRecordType;

   typedef struct _nodeTableType
      {
       gmsNodeType    kindOfNode;
       int            numRecords;
       nodeRecordType *nodeRecords;
      } nodeTableType;


   //---------------------------------------------*/
   // gmsGetNodeTable
   //
   // Description:
   //    This utility reads the file that contains
   //    the "Node Table".  A pointer to a newly
   //    allocated table is returned to the caller.
   //    It is the caller's responsibility to free
   //    the item by using the utility
   //    "gmsFreeNodeTable" (see below).
   //---------------------------------------------*/
   nodeTableType *gmsGetNodeTable
                           (const char  *nodeTableFilePath,
                            gmsNodeType kindOfNode);


   //---------------------------------------------*/
   // gmsFreeNodeTable
   //
   // Description:
   //    This utility frees a "Node Table" that had
   //    been previously allocated using
   //    "gmsGetNodeTable".
   //---------------------------------------------*/
   void gmsFreeNodeTable
           (nodeTableType *theNodeTable);


   //---------------------------------------------*/
   // gmsPrintNodeTable	
   //
   // Description:
   //    This function will print the "Node Table"
   //    object to standard out.
   //---------------------------------------------*/
   void gmsPrintNodeTable
           (nodeTableType *theNodeTable);

#endif
