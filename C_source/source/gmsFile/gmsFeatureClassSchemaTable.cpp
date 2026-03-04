//----------------------------------------------------------------------------*/
// File : gmsFeatureClassSchemaTable.cpp
// Date : 16-Aug-99 : initial definition
//        20-Sep-99 : use size of record from file ... no padding
//        07-Oct-99 : Clean-up due to code-inspection
//
// Description:
//    Utility to read/process any "Feature Class Schema Table" from the DCW
//    database.
//
//    A "Feature Class Schema Table" defines the feature classes contained
//    in a given coverage/area.  The records of the table specify:
//
//             1) the name of a feature class
//             2) the name of two tables involved
//                in a join-relationship
//             3) the names of the columns used in
//                the join-relationship
//
//    Note that the name of the feature class is repeated so as to specify
//    all relationships in the schema table.  Topological relationships are
//    not specified because they are implied by the types of the tables
//    [i.e. the joined tables].
//
//    Also, if a key in the join-relationship is a compound key, column names
//    will be listed using a backslash character ('\') as a separator.  For
//    example, a primary key composed of two columns would be specified as
//    "NAME\TYPE".
//
//    Furthermore, consider the following definitions:
//
//       Feature Class Schema Table:
//       ---------------------------
//          A table that contains the composition rules of each
//          feature class.  This table describes the definition
//          for each feature class and the way in which each table
//          in a feature class relates to other tables.
//
//       Feature Class:
//       --------------
//          A set of features with a common set of attributes.
//          A feature class consists of a set of tables that
//          includes one or more primitive tables and one or
//          more attribute tables.  A feature class has the same
//          columns of attribute information for each feature.
//          Every feature class has one and only one feature table.
//          There are three types of feature classes: SIMPLE, COMPLEX,
//          and TEXT.  There are three subtypes of SIMPLE feature
//          classes: LINE, POINT, and AREA.
//
//       Feature:
//       --------
//          A model of a real world geographic entity.  It is a
//          zero-, one-, or two-dimensional entity [with a uniform
//          set of attributes].
//
//    Consider the following example.  Two records of the "FCS" file from
//    the PO (Political/Oceans) directory of the BROWSE library are:
//
//                 ID            : 1
//                 Feature-Class : POAREA
//                 Table-1       : POAREA.AFT
//                 Foreign-key   : ID
//                 Table-2       : FAC
//                 Primary-key   : ID
//                 ------------------------
//                 ID            : 2
//                 Feature-Class : POAREA  
//                 Table-1       : FAC
//                 Foreign-key   : ID
//                 Table-2       : POAREA.AFT
//                 Primary-key   : ID
//
//    Reference:
//        1) Mil-Std-600006
//        2) Mil-D-89009
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/

#include <gmsFeatureClassSchemaTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------------------*/
//     Local Variables
//-----------------------------*/
static FILE *fcs_fd = (FILE *) NULL;


//-----------------------------*/
// Declare Local Subprograms
//-----------------------------*/
static void readPastFormatInformation ();

static void buildFcsTable
               (featureClassSchemaType *theFcsTable);

static void printOneFcsRecord
               (featureClassRectType fcsRecord);


//---------------------------------------------*/
// gmsGetFeatureClassSchemaTable
//
// Description:
//    This utility reads the file that contains
//    a "feature Class Schema Table".  A pointer
//    to a newly allocated table is returned to
//    the caller.  It is the caller's responsibility
//    to free the item by using the utility
//    "gmsFreeFeatureClassSchemaTable" (see below).
//---------------------------------------------*/
featureClassSchemaType *gmsGetFeatureClassSchemaTable
                              (const char *fcsTableFilePath)

{
         const char             readOnlyMode[] = { "rb" };
         featureClassSchemaType *theFcsTable;

   if (fcsTableFilePath == NULL)
      return (featureClassSchemaType *) NULL;

   fcs_fd = fopen(fcsTableFilePath,
                  readOnlyMode);

   if (fcs_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open FCS file: %s\n",
              fcsTableFilePath);

       return (featureClassSchemaType *) NULL;
      }

   readPastFormatInformation ();

   theFcsTable =
      (featureClassSchemaType *) malloc (sizeof(featureClassSchemaType));

   gmsClearMemory
      ( (char *) theFcsTable,
        sizeof(featureClassSchemaType));

   buildFcsTable (theFcsTable);

   fclose (fcs_fd);

   return theFcsTable;
}


//---------------------------------------------*/
// gmsFreeFeatureClassSchemaTable
//
// Description:
//    This utility frees a "Feature Class Schema
//    Table" that had been previously allocated
//    using "gmsGetFeatureClassSchemaTable".
//---------------------------------------------*/
void gmsFreeFeatureClassSchemaTable
           (featureClassSchemaType *theFCS)

{
   if (theFCS == NULL)
      return;

   if (theFCS->featureRecords != NULL)
      free (theFCS->featureRecords);

   free (theFCS);
}


//---------------------------------------------*/
// gmsPrintFeatureClassSchemaTable
//
// Description:
//    This function will print the "Feature Class
//    Schema Table" object to standard out.
//---------------------------------------------*/
void gmsPrintFeatureClassSchemaTable
           (featureClassSchemaType *theFCS)

{
         int i;

   printf("________________________________________\n");
   printf("Feature Class Schema Table:\n");

   if (theFCS == NULL)
      return;

   printf("Num Records : %d\n", theFCS->numRecords);

   if (theFCS->featureRecords == NULL)
      return;

   printf("ID  Class     Tbl-1         Fgn-Key           Tbl-2       Pri-Key\n");
   printf("-----------------------------------------------------------------\n");

   for (i = 0; i < theFCS->numRecords; i++)
      {
       printOneFcsRecord (theFCS->featureRecords[i]);
      }
}


     //-----------------------*/
     //   Local Subprograms
     //-----------------------*/


//---------------------------------------------*/
// readPastFormatInformation
//
// Description:
//    This function will read the format data
//    located at the front of the FCS Table
//    file.
//---------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (fcs_fd);

   tempChar = fgetc (fcs_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (fcs_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (fcs_fd);
      }
}


//---------------------------------------------*/
// buildFcsTable
//
// Description:
//    This function will read the actual data
//    from the Feature Class Schema Table file.
//    The data read will be used to populate the
//    attributes of the object.
//---------------------------------------------*/
static void buildFcsTable
               (featureClassSchemaType *theFcsTable)

{
         int       numBytes = 0;
         int       numRecords;
         int       tempChar;
         int       index;
         const int Size_Of_FCS_Record = 68; // i.e. w.o padding */

   readPastFormatInformation ();

   tempChar = fgetc(fcs_fd);

   while ( tempChar != EOF )
      {
       numBytes++;

       tempChar = fgetc(fcs_fd);
      }

   numRecords = numBytes / Size_Of_FCS_Record;

   rewind (fcs_fd);

   readPastFormatInformation ();

   numBytes = numRecords * sizeof(featureClassRectType);

   theFcsTable->featureRecords = (featureClassRectType *) malloc (numBytes);

   theFcsTable->numRecords = numRecords;

   for (index = 0; index < numRecords; index++)
      {
       theFcsTable->featureRecords[index].ID = gmsReadInteger (fcs_fd);

       fgets
          (theFcsTable->featureRecords[index].featureClass,
           sizeof(theFcsTable->featureRecords[index].featureClass),
           fcs_fd);

       fgets
          (theFcsTable->featureRecords[index].firstRelatedTable,
           sizeof(theFcsTable->featureRecords[index].firstRelatedTable),
           fcs_fd);

       fgets
          (theFcsTable->featureRecords[index].foreignKeyFromTable_1,
           sizeof(theFcsTable->featureRecords[index].foreignKeyFromTable_1),
           fcs_fd);

       fgets
          (theFcsTable->featureRecords[index].secondRelatedTable,
           sizeof(theFcsTable->featureRecords[index].secondRelatedTable),
           fcs_fd);

       fgets
          (theFcsTable->featureRecords[index].primaryKeyFromTable_2,
           sizeof(theFcsTable->featureRecords[index].primaryKeyFromTable_2),
           fcs_fd);
      }
}


//---------------------------------------------*/
// printOneFcsRecord
//
// Description:
//    This function will print the specified
//    "featureClassRectType" record to stdout.
//---------------------------------------------*/
static void printOneFcsRecord 
                  (featureClassRectType fcsRecord)

{
   printf("%d   %s  %s  %s  %s  %s\n",
          fcsRecord.ID,
          fcsRecord.featureClass,
          fcsRecord.firstRelatedTable,
          fcsRecord.foreignKeyFromTable_1,
          fcsRecord.secondRelatedTable,
          fcsRecord.primaryKeyFromTable_2);
}
