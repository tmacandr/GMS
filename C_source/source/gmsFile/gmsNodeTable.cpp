/*----------------------------------------------------------------------------*/
/* File : gmsNodeTable.cpp
/* Date : 18-Aug-99 : Initial definition
/*        20-Sep-99 : Browse, gazette, themes ... different
/*        05-Oct-99 : Clean-up due to code-inspection
/*        16-Nov-99 : mod to make ANSIC C compiler work
/*
/* Description:
/*    Utilities to read/process any "Node Table" file of the Digital Chart of
/*    the World (DCW) database.  Consider the following definitions:
/*
/*       Node:
/*       -----
/*          A zero-demensional geometric primitive that is
/*          composed of a single coordinatge tuple (pair or
/*          triplet).  There are two types of nodes:
/*                     1) Entity Nodes
/*                     2) Connected Nodes
/*
/*       Entity Node:
/*       ------------
/*          A node that represents isolated features that are
/*          zero-dimensional at a particular scale.  Entity nodes
/*          are topologically linked to a containing face when face
/*          topology is present.  Entity nodes can reside at any
/*          location, whether or not there is another primitive at
/*          that same location.
/*
/*       Connected Node:
/*       ---------------
/*          A node that represents linked features that are zero
/*          dimensional at a specified scale.  Connected nodes
/*          are always found at the ends of edges and are topologically
/*          linked to the edges.  Connected nodes are used in two
/*          ways:
/*                 1) to define edges topologically (always).
/*                    Such nodes are referred to as START and
/*                    END nodes.
/*                 2) to represent point features that are
/*                    found at a juncture of linear features,
/*                    such as overpasses, locks in a canal,
/*                    etc.  Attributes are associated with
/*                    the point features of the connected nodes.
/*                    If many edges intersect a node, only one
/*                    edge will be maintained per node.  Other
/*                    edges are linked using "Winged-Edge
/*                    Topology".
/*
/*    There are three classes of "node tables".  One is specific to the
/*    BROWSE library.  The second is specific for the GAZETTE library.
/*    The other is specific to the remaining 17 themes.
/*
/*    A BROWSE node record is:
/*    ------------------------
/*       Level 1-2 topology Entity Node Primitive Table
/*          ID             =I,1,P,Row ID,-,-,:
/*          CONTAINING_FACE=X,1,N,Null column,-,-,:(null)
/*          FIRST_EDGE     =X,1,N,Null column,-,-,:(null)
/*          COORDINATE     =C,1,N,Node x and y location,-,-,:
/*
/*    A GAZETTE node record is:
/*    -------------------------
/*       Entity Node Primitives;-;
/*       ID             =I, 1,P,Row Identifier,-,-,:
/*       CONTAINING_FACE=I, 1,F,Foreign Key to Face Table,-,-,:
/*       FIRST_EDGE     =X, 1,N,Foreign Key to Edge Table (null),-,-,:
/*       COORDINATE     =C, 1,N,Coordinates of Node,-,-,:;
/*
/*    A thematic node record is:
/*    --------------------------
/*       Entity Node Primitives;-;
/*          ID             =I,1,P,Row Identifier,-,-,:
/*          XXX.PFT_ID     =I,1,F,Foreign Key to Point Feature Table,-,-,:
/*          CONTAINING_FACE=I,1,F,Foreign Key to Face Table,-,-,:
/*          FIRST_EDGE     =X,1,N,Foreign Key to Edge Table (null),-,-,:
/*          COORDINATE     =C,1,N,Coordinates of Node,-,-,:;
/*
/*       where XXX = the corresponding name of the 'point-feature class'.
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsNodeTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------*/
/*     Local Variables
/*-----------------------------*/
static FILE *node_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms
/*-----------------------------*/
static void readPastFormatInformation ();

static void buildNodeTable
               (nodeTableType *theNodeTbl);

static void printNodeTable
               (nodeTableType *theNodeTbl);

static void debugPrintContentsOfFile ();


/*---------------------------------------------*/
/* gmsGetNodeTable
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Node Table".  A pointer to a newly
/*    allocated table is returned to the caller.
/*    It is the caller's responsibility to free
/*    the item by using the utility
/*    "gmsFreeNodeTable" (see below).
/*---------------------------------------------*/
nodeTableType *gmsGetNodeTable
                     (const char  *nodeTableFilePath,
                      gmsNodeType kindOfNode)

{
         const char    readOnlyMode[] = { "rb" };
         nodeTableType *theNodeTbl;

   if (nodeTableFilePath == NULL)
      return (nodeTableType *) NULL;

   node_fd = fopen(nodeTableFilePath,
                   readOnlyMode);

   if (node_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open NODE file: %s\n",
              nodeTableFilePath);

       return (nodeTableType *) NULL;
      }

   theNodeTbl = (nodeTableType *) malloc (sizeof(nodeTableType));

   gmsClearMemory
      ( (char *) theNodeTbl,
        sizeof(nodeTableType) );

   theNodeTbl->kindOfNode = kindOfNode;

   buildNodeTable (theNodeTbl);

   fclose (node_fd);

   return theNodeTbl;
}


/*---------------------------------------------*/
/* gmsFreeNodeTable
/*
/* Description:
/*    This utility frees a "Node Table" that had
/*    been previously allocated using
/*    "gmsGetNodeTable".
/*---------------------------------------------*/
void gmsFreeNodeTable
           (nodeTableType *theNodeTable)

{
   if (theNodeTable == NULL)
      return;

   if (theNodeTable->nodeRecords != NULL)
      free (theNodeTable->nodeRecords);

   free (theNodeTable);
}


/*---------------------------------------------*/
/* gmsPrintNodeTable	
/*
/* Description:
/*    This function will print the "Node Table"
/*    object to standard out.
/*---------------------------------------------*/
void gmsPrintNodeTable
           (nodeTableType *theNodeTable)

{
         #define           Num_Node_Libs 3
         static const int  numNodeLibs = Num_Node_Libs;
         static const char *lookupTable[Num_Node_Libs] = { "Browse",
                                                           "Gazette",
                                                           "Theme" };

   printf("________________________________________\n");
   printf("Node Table:\n");

   if (theNodeTable == NULL)
      return;

   printf("Num Records  : %d\n", theNodeTable->numRecords);

   if (theNodeTable->numRecords <= 0)
      return;

   if ( (theNodeTable->kindOfNode >= 0) &&
        (theNodeTable->kindOfNode <= numNodeLibs) )
      {
       printf("Kind of Node : %s\n", lookupTable[theNodeTable->kindOfNode]);
      }

   if (theNodeTable->nodeRecords == NULL)
      return;

   printNodeTable (theNodeTable);
}


     /*-----------------------*/
     /*   Local Subprograms
     /*-----------------------*/


/*---------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the "Node Table"
/*    file.
/*---------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (node_fd);

   tempChar = gmsReadInteger (node_fd);

   tempChar = fgetc (node_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (node_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (node_fd);
      }
}


/*---------------------------------------------*/
/* buildNodeTable
/*
/* Description:
/*    This function will read the actual data
/*    from the Node Table file.  The data read
/*    will be used to populate the attributes of
/*    the object.
/*---------------------------------------------*/
static void buildNodeTable
               (nodeTableType *theNodeTbl)

{
         int       numBytes = 0;
         int       numRecords;
         int       tempChar;
         int       index;
         const int Size_Of_Thematic_Node_Rec = 20; /* i.e. w/o padding */
         const int Size_Of_Browse_Node_Rec   = 12; /* i.e. w/o padding */
         const int Size_Of_Gazette_Node_Rec  = 16; /* i.e. w/o padding */

   readPastFormatInformation ();

   tempChar = fgetc (node_fd);

   while ( tempChar != EOF )
      {
       numBytes++;

       tempChar = fgetc(node_fd);
      }

   if (theNodeTbl->kindOfNode == gmsBrowseNode)

      numRecords = numBytes / Size_Of_Browse_Node_Rec;

   else if (theNodeTbl->kindOfNode == gmsGazetteNode)

      numRecords = numBytes / Size_Of_Gazette_Node_Rec;

   else if (theNodeTbl->kindOfNode == gmsThemeNode)

      numRecords = numBytes / Size_Of_Thematic_Node_Rec;

   else
      {
       printf("*** ERROR : unknown theme for NODE table\n");

       return;
      }

   rewind (node_fd);

   readPastFormatInformation ();

   numBytes = numRecords * sizeof(nodeRecordType);

   theNodeTbl->nodeRecords = (nodeRecordType *) malloc (numBytes);

   gmsClearMemory
      ( (char *) theNodeTbl->nodeRecords,
        numBytes);

   theNodeTbl->numRecords = numRecords;

   for (index = 0; index < numRecords; index++)
      {
       theNodeTbl->nodeRecords[index].ID =
          gmsReadInteger (node_fd);

       if (theNodeTbl->kindOfNode == gmsBrowseNode)
          {
           theNodeTbl->nodeRecords[index].containingFace = -1;

           theNodeTbl->nodeRecords[index].fgnKeyToPointFeatureTbl = -1;
          }

       else if (theNodeTbl->kindOfNode == gmsGazetteNode)
          {
           theNodeTbl->nodeRecords[index].containingFace =
              gmsReadInteger (node_fd);

           theNodeTbl->nodeRecords[index].fgnKeyToPointFeatureTbl = -1;
          }

       else if (theNodeTbl->kindOfNode == gmsThemeNode)
          {
           theNodeTbl->nodeRecords[index].fgnKeyToPointFeatureTbl =
              gmsReadInteger (node_fd);

           theNodeTbl->nodeRecords[index].containingFace =
              gmsReadInteger (node_fd);
          }

       else if (theNodeTbl->kindOfNode == gmsThemeNode)
          {
           printf("*** ERROR : unknown theme for NODE table\n");

           return;
          }

       theNodeTbl->nodeRecords[index].coordinate =
          gmsReadTwoDimCoord (node_fd);
      }
}


/*---------------------------------------------*/
/* printNodeTable
/*
/* Description:
/*    This function will print the specified
/*    "Node Table" to stdout.
/*---------------------------------------------*/
static void printNodeTable 
                  (nodeTableType *theNodeTbl)

{
         int i;

   printf("ID \t Face \t Fgn-Key \t Coordinates (Long,Lat)\n");
   printf("---------------------------------------------------------\n");

   for (i = 0; i < theNodeTbl->numRecords; i++)
      printf("%d) \t %d \t %d \t (%f, %f)\n",
             theNodeTbl->nodeRecords[i].ID,
             theNodeTbl->nodeRecords[i].containingFace,
             theNodeTbl->nodeRecords[i].fgnKeyToPointFeatureTbl,
             theNodeTbl->nodeRecords[i].coordinate.Long,
             theNodeTbl->nodeRecords[i].coordinate.Lat);
}


/*---------------------------------------------*/
/* debugPrintContentsOfFile
/*
/* Description:
/*    Debug utility to print the contents of the
/*    "Node Table" file (in hex-byte format) to
/*    stdout. 
/*---------------------------------------------*/
static void debugPrintContentsOfFile ()

{
         int tempChar;
         int numBytes = 0;
         int lineCount = 0;
         int lengthOfHeader = 0;
         int i;

   printf("_________________________________________\n");
   printf("Bytes of the Node Table\n");

   rewind (node_fd);

   lengthOfHeader = gmsReadInteger (node_fd);

   printf("Header:\n");

   for (i = 0; i < lengthOfHeader; i++)
      {
       tempChar = fgetc (node_fd);

       printf("%.2x ", tempChar);

       if ( lineCount == 3)
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;
      }

   printf("\n");

   printf("Data:\n");

   lineCount = 0;

   do
      {
       tempChar = fgetc (node_fd); 

       if ( tempChar == EOF ) break;

       numBytes++;

       printf("%.2x ", tempChar);

       if ( lineCount == 3 )
          {
           printf("\n");

           lineCount = 0;
          }
       else
          lineCount++;
      }
   while ( true );

   printf("---> Total Num of 'node' bytes = %d\n", numBytes);
   printf("_________________________________________\n");

   rewind (node_fd);
}
