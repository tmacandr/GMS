/*----------------------------------------------------------------------------*/
/* File : gmsIndexFile.cpp
/* Date : 13-Jul-99 : Initial Definition
/*        01-Aug-99 : Convert to utilities on port to Sun Solaris
/*        05-Oct-99 : Clean-up from code inspection
/*
/* Description:
/*    Utilities to "read" an "index" file from the Digital Chart of the
/*    World (DCW) database.  Index files are implemented for tables with
/*    variable length records, primitive tables, and feature tables in
/*    tiled databases that require query response capabilities.  In the
/*    DCW, tables that have an associated index are all those with variable-
/*    length records.
/*
/*    These utilities are used for indexing "variable-length" fields.
/*    These utilities can be applied to tables of:
/*
/*                 face
/*                 edge
/*                 node
/*                 text
/*
/*    primitives.
/*
/*    For "spatial-indexing" of tables, refer to the utilities defined in
/*    "gmsSpatialIndex.h".  For "thematic-indexing" of "feature-tables"
/*    (i.e. area, line, point, and text feature tables) refer to the
/*    utilities defined in: "gmsThematicIndex.h".
/*
/*    Consider the following definitions:
/*
/*       Variable-length Index Files:
/*       ----------------------------
/*          The contents of such files contain starting
/*          positions (in bytes) of records in support
/*          of "Text" files and "Coordinate" files.  The
/*          data is an array that specifies the byte-offset
/*          from the beginning of the file to the start
/*          of each record.
/*
/*       Index:
/*       ------
/*          A mechanism used to quickly identify a particular
/*          record or group of records based on a tables primary
/*          key.
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsIndexFile.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*-----------------------------*/
/*     Local Variables         */
/*-----------------------------*/
static FILE *index_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms   */
/*-----------------------------*/
static void buildIndexRecord
               (indexTableType *theIndexTable);


/*---------------------------------------------*/
/* gmsGetIndexFile
/*
/* Description:
/*    This utility reads the file that contains
/*    the "Index File".  A pointer
/*    to a newly allocated table is returned to
/*    the caller.  It is the caller's responsibility
/*    to free the item by using the utility
/*    "gmsFreeIndexFile" (see below).
/*---------------------------------------------*/
indexTableType *gmsGetIndexFile
                             (const char *filePath)

{
         const char     readOnlyMode[] = { "rb" };
         indexTableType *theIndexTable;

   if (filePath == NULL)
      return (indexTableType *) NULL;

   index_fd = fopen(filePath,
                    readOnlyMode);

   if (index_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open index file:%s\n", filePath);

       return (indexTableType *) NULL;
      }

   theIndexTable = (indexTableType *) malloc (sizeof(indexTableType));

   buildIndexRecord (theIndexTable);

   fclose (index_fd);

   return theIndexTable;
}


/*---------------------------------------------*/
/* gmsFreeIndexFile
/*
/* Description:
/*    This utility frees a "Index File" that had
/*    been previously allocated using
/*    "gmsGetIndexFile".
/*---------------------------------------------*/
void gmsFreeIndexFile
           (indexTableType *theIndexTable)

{
   if (theIndexTable == NULL)
      return;

   if (theIndexTable->indexList != NULL)
      free (theIndexTable->indexList);

   free (theIndexTable);
}


/*---------------------------------------------*/
/* gmsPrintIndexRecord	
/*
/* Description:
/*    This function will print "theIndexes"
/*    record (to stdout).
/*---------------------------------------------*/
void gmsPrintIndexRecord
        (indexTableType *theIndexTable)

{
         int i;

   printf("______________________________\n");
   printf("Index Record:\n");

   if (theIndexTable == NULL)
      return;

   printf("---> Num Records in Table    : %d\n",
          theIndexTable->numberRecordsInTable);
   printf("---> Size of Header in Table : %d bytes\n",
          theIndexTable->bytesInTableHeader);

   printf("Record \t Offset \t Size of Record (bytes)\n");
   printf("------------------------------------------\n");

   if (theIndexTable->indexList == NULL)
      return;

   for (i = 0;
        i < theIndexTable->numberRecordsInTable;
        i++)
      {
       printf("  %d \t %ld \t\t %d\n",
              i,
              theIndexTable->indexList[i].byteOffset,
              theIndexTable->indexList[i].numBytes);
      }

   printf("______________________________\n");
}


     /*-----------------------*/
     /*   Local Subprograms
     /*-----------------------*/


/*---------------------------------------------*/
/* buildIndexRecord
/*
/* Description:
/*    This function will read the actual data
/*    from the Index file.  The data read will
/*    be used to build the object (i.e. the
/*    list of indexes).
/*---------------------------------------------*/
static void buildIndexRecord
               (indexTableType *theIndexTable)

{
         int i;
         int numChars = 0;

   theIndexTable->numberRecordsInTable = gmsReadInteger (index_fd);

   theIndexTable->bytesInTableHeader = gmsReadInteger (index_fd);

   /*-----------------------------*/
   /* Allocate a buffer big enough
   /* to hold the data.
   /*-----------------------------*/
   numChars = theIndexTable->numberRecordsInTable * (sizeof(int) * 2);

   theIndexTable->indexList = (indexRecordType *) malloc(numChars);

   gmsClearMemory
      ( (char *) theIndexTable->indexList,
       numChars);

   for (i = 0; i < theIndexTable->numberRecordsInTable; i++)
      {
       theIndexTable->indexList[i].byteOffset =
          (long) gmsReadInteger (index_fd);

       theIndexTable->indexList[i].numBytes = gmsReadInteger (index_fd);
      }
}
