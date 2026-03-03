/*----------------------------------------------------------------------------*/
/* File : gmsValueDescriptionTable.c
/* Date : 24-Aug-99 : Initial definition
/*        19-Sep-99 : Browse char VDT different from main themes
/*        07-Oct-99 : Clean-up due to code-inspection
/*
/* Description:
/*    These utilities are used to "read/process" any "Value Description
/*    Table" file of the Digital Chart of the World (DCW) database.  There
/*    are two types of "Value Description Tables".  The first is an "Integer
/*    VDT" and the other is a "character VDT".  Each record in the "Integer"
/*    VDT has the format:
/*
/*           Integer Value Description Record
/*               ID         =I,1,N,Row Identifier,-,-,:
/*               TABLE      =T,12,P,Name of Feature Table,-,-,:
/*               ATTRIBUTE  =T,16,P,Attribute Name,-,-,:
/*               VALUE      =I,1,P,Attribute Value,-,-,:
/*               DESCRIPTION=T,50,N,Attribute Value Description,-,-,:
/*
/*    There are two sub-types for the "Character" VDT.  For the BROWSE
/*    library the record format is:
/*
/*           Character Value Description Record
/*               ID         =I,1,N,Row Identifier,-,-,:
/*               TABLE      =T,12,P,Name of Feature Table,-,-,:
/*               ATTRIBUTE  =T,16,P,Attribute Name,-,-,:
/*               VALUE      =T,3,P,Attribute Value,-,-,:
/*               DESCRIPTION=T,50,N,Attribute Value Description,-,-,:;
/*
/*    However, for the main themes (PO only?), the record format is:
/*
/*           Character Value Description Table;-;
/*              ID         =I,1,N,Row Identifier,-,-,:
/*              TABLE      =T,12,P,Feature Table Name,-,-,:
/*              ATTRIBUTE  =T,16,P,Attribute Name,-,-,:
/*              VALUE      =T,2,P,Attribute Value,-,-,:
/*              DESCRIPTION=T,50,N,Attribute Value Description,-,-,:;
/*
/*    Reference:
/*        1) Mil-Std-600006
/*        2) Mil-D-89009
/*
/* Copyright (c) 1999 - 2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#include <gmsValueDescriptionTable.h>
#include <gmsUtilities.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


/*-----------------------------*/
/*     Local Variables       --*/
/*-----------------------------*/
static FILE *vdt_fd = (FILE *) NULL;


/*-----------------------------*/
/* Declare Local Subprograms --*/
/*-----------------------------*/
static void buildIntValueDescriptionTable
               (intValueDescriptionTableType *theVDT);

static void buildCharValueDescriptionTable
               (charValueDescriptionTableType *theVDT,
                bool                          isBrowse);

static void printIntVdtRecord 
                (intVdtRecordType theVdtRec);

static void printCharVdtRecord 
                (charVdtRecordType theVdtRec);

static void readPastFormatInformation ();


/*---------------------------------------------*/
/* gmsGetIntValueDescriptionTable
/*
/* Description:
/*    This utility reads the file that contains
/*    an INTEGER "Value Description Table".  A
/*    pointer to a newly allocated table is
/*    returned to the caller.  It is the caller's
/*    responsibility to free the item by using the
/*    utility "gmsFreeIntValueDescriptionTable"
/*    (see below).
/*---------------------------------------------*/
intValueDescriptionTableType *gmsGetIntValueDescriptionTable
                                    (const char *intVdtFilePath)

{
         const char                   readOnlyMode[] = { "rb" };
         intValueDescriptionTableType *theIntVDT;

   if (intVdtFilePath == NULL)
      return (intValueDescriptionTableType *) NULL;

   vdt_fd = fopen(intVdtFilePath,
                  readOnlyMode);

   if (vdt_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open Value Descrip file: %s\n",
              intVdtFilePath);

       return (intValueDescriptionTableType *) NULL;
      }

   theIntVDT = (intValueDescriptionTableType *)
               malloc (sizeof(intValueDescriptionTableType));

   gmsClearMemory
      ((char *) theIntVDT,
       sizeof(intValueDescriptionTableType));

   buildIntValueDescriptionTable (theIntVDT);

   fclose (vdt_fd);

   return theIntVDT;
}


/*---------------------------------------------*/
/* gmsFreeIntValueDescriptionTable
/*
/* Description:
/*    This utility frees an "Integer Value Description
/*    Table" that had been previously allocated
/*    using "gmsGetIntValueDescriptionTable"
/*   (see above).
/*---------------------------------------------*/
void gmsFreeIntValueDescriptionTable
           (intValueDescriptionTableType *theVDT)

{
   if (theVDT == NULL)
      return;

   if (theVDT->vdtRecords != NULL)
      free (theVDT->vdtRecords);

   free (theVDT);
}


/*---------------------------------------------*/
/* gmsPrintIntValueDescriptionTable
/*
/* Description:
/*    This function will print the "Integer Value
/*    Description Table" object to standard out.
/*---------------------------------------------*/
void gmsPrintIntValueDescriptionTable
           (intValueDescriptionTableType *theVDT)

{
         int i;

   printf("________________________________________\n");
   printf("Integer ValueDescription Table:\n");

   if (theVDT == NULL)
      return;

   printf("Num Records : %d\n", theVDT->numRecords);

   if (theVDT->vdtRecords == NULL)
      return;

   for (i = 0; i < theVDT->numRecords; i++)
      printIntVdtRecord (theVDT->vdtRecords[i]);
}



/*---------------------------------------------*/
/* gmsGetCharValueDescriptionTable
/*
/* Description:
/*    This utility reads the file that contains
/*    an CHARACTER "Value Description Table".  The
/*    caller must also specify if the file is from
/*    the BROWSE library.  A pointer to a newly
/*    allocated table is returned to the caller.
/*    It is the caller's responsibility to free the
/*    item by using the utility
/*    "gmsFreeCharValueDescriptionTable" (see below).
/*---------------------------------------------*/
charValueDescriptionTableType *gmsGetCharValueDescriptionTable
                                    (const char *charVdtFilePath,
                                     bool       isBrowse)

{
         const char                    readOnlyMode[] = { "rb" };
         charValueDescriptionTableType *theCharVDT;

   if (charVdtFilePath == NULL)
      return (charValueDescriptionTableType *) NULL;

   vdt_fd = fopen(charVdtFilePath,
                  readOnlyMode);

   if (vdt_fd == (FILE *) NULL)
      {
       printf("---> ERROR : unable to open Value Descrip file: %s\n",
              charVdtFilePath);

       return (charValueDescriptionTableType *) NULL;
      }

   theCharVDT = (charValueDescriptionTableType *)
                malloc (sizeof(charValueDescriptionTableType));

   gmsClearMemory
      ((char *) theCharVDT,
       sizeof(charValueDescriptionTableType));

   buildCharValueDescriptionTable
      (theCharVDT,
       isBrowse);

   fclose (vdt_fd);

   return theCharVDT;
}


/*---------------------------------------------*/
/* gmsFreeCharValueDescriptionTable
/*
/* Description:
/*    This utility frees an "Character Value
/*    Description Table" that had been previously
/*    allocated using "gmsGetCharValueDescriptionTable"
/*   (see above).
/*---------------------------------------------*/
void gmsFreeCharValueDescriptionTable
           (charValueDescriptionTableType *theVDT)

{
   if (theVDT == NULL)
      return;

   if (theVDT->vdtRecords != NULL)
      free (theVDT->vdtRecords);

   free (theVDT);
}


/*---------------------------------------------*/
/* gmsPrintCharValueDescriptionTable
/*
/* Description:
/*    This function will print the "Character Value
/*    Description Table" object to standard out.
/*---------------------------------------------*/
void gmsPrintCharValueDescriptionTable
           (charValueDescriptionTableType *theVDT)

{
         int i;

   printf("________________________________________\n");
   printf("Character ValueDescription Table:\n");

   if (theVDT == NULL)
      return;

   printf("Num Records : %d\n", theVDT->numRecords);

   if (theVDT->vdtRecords == NULL)
      return;

   for (i = 0; i < theVDT->numRecords; i++)
      printCharVdtRecord (theVDT->vdtRecords[i]);
}


     /*-----------------------*/
     /*   Local Subprograms
     /*-----------------------*/


/*---------------------------------------------*/
/* buildIntValueDescriptionTable
/*
/* Description:
/*    This function will read the actual data
/*    from the Integer-Value-Description-Table file.
/*    The data read will be used to populate the
/*    attributes of the object.
/*---------------------------------------------*/
static void buildIntValueDescriptionTable
               (intValueDescriptionTableType *theVDT)

{
         int       tempChar;
         int       numBytes = 0;
         int       numRecords;
         int       index;
         const int Size_Of_Int_VDT_Rec = 86; /* no padding */

   /*-------------------------*/
   /* How big is the file?
   /*-------------------------*/
   rewind (vdt_fd);

   readPastFormatInformation ();

   tempChar = fgetc(vdt_fd);

   while ( tempChar != EOF )
      {
       numBytes++;

       tempChar = fgetc(vdt_fd);
      }

   numRecords = numBytes / Size_Of_Int_VDT_Rec;

   theVDT->numRecords = numRecords;

   /*-------------------------*/
   /* Build the data structure
   /*-------------------------*/
   rewind (vdt_fd);

   readPastFormatInformation ();

   numBytes = numRecords * sizeof(intVdtRecordType);

   theVDT->vdtRecords = (intVdtRecordType *) malloc (numBytes);

   for (index = 0; index < numRecords; index++)
      {
       theVDT->vdtRecords[index].ID = gmsReadInteger (vdt_fd);

       fgets
          (theVDT->vdtRecords[index].table,
           sizeof(theVDT->vdtRecords[index].table),
           vdt_fd);

       fgets
          (theVDT->vdtRecords[index].attribute,
           sizeof(theVDT->vdtRecords[index].attribute),
           vdt_fd);

       theVDT->vdtRecords[index].value = gmsReadInteger (vdt_fd);

       fgets
          (theVDT->vdtRecords[index].description,
           sizeof(theVDT->vdtRecords[index].description),
           vdt_fd);
      }
}


/*---------------------------------------------*/
/* buildCharValueDescriptionTable
/*
/* Description:
/*    This function will read the actual data
/*    from the Character-Value-Description-Table file.
/*    The data read will be used to populate the
/*    attributes of the object.
/*---------------------------------------------*/
static void buildCharValueDescriptionTable
               (charValueDescriptionTableType *theVDT,
                bool                          isBrowse)

{
         int       tempChar;
         int       numBytes = 0;
         int       numRecords;
         int       index;
         const int numBrowseValueChars = 3 + 1;
         const int numThemeValueChars = 2 + 1;
         const int Size_Of_Browse_Char_VDT_Rec = 85; /* no padding */
         const int Size_Of_Theme_Char_VDT_Rec  = 84; /* no padding */

   /*-------------------------*/
   /* How big is the file?
   /*-------------------------*/
   rewind (vdt_fd);

   readPastFormatInformation ();

   tempChar = fgetc(vdt_fd);

   while ( tempChar != EOF )
      {
       numBytes++;

       tempChar = fgetc(vdt_fd);
      }

   if (isBrowse)
      numRecords = numBytes / Size_Of_Browse_Char_VDT_Rec;
   else
      numRecords = numBytes / Size_Of_Theme_Char_VDT_Rec;

   /*-------------------------*/
   /* Build the data structure
   /*-------------------------*/
   rewind (vdt_fd);

   readPastFormatInformation ();

   numBytes = numRecords * sizeof(charVdtRecordType);

   theVDT->vdtRecords = (charVdtRecordType *) malloc (numBytes);

   theVDT->numRecords = numRecords;

   for (index = 0; index < numRecords; index++)
      {
       theVDT->vdtRecords[index].ID = gmsReadInteger (vdt_fd);

       fgets
          (theVDT->vdtRecords[index].table,
           sizeof(theVDT->vdtRecords[index].table),
           vdt_fd); 

       fgets
          (theVDT->vdtRecords[index].attribute,
           sizeof(theVDT->vdtRecords[index].attribute),
           vdt_fd);

       if (isBrowse)
          {
           fgets
              (theVDT->vdtRecords[index].value,
               numBrowseValueChars,
               vdt_fd);
          }

       else
          {
           fgets
              (theVDT->vdtRecords[index].value,
               numThemeValueChars,
               vdt_fd);
          }

       fgets
          (theVDT->vdtRecords[index].description,
           sizeof(theVDT->vdtRecords[index].description),
           vdt_fd);
      }
}


/*---------------------------------------------*/
/* printIntVdtRecord
/*
/* Description:
/*    Utility that prints the contents of the
/*    specified "INTEGER-value-description-
/*    record" to stdout.
/*---------------------------------------------*/
static void printIntVdtRecord 
                (intVdtRecordType theVdtRec)

{
   printf("____________\n");
   printf("ID          : %d\n", theVdtRec.ID);
   printf("Table       : %s\n", theVdtRec.table);
   printf("Attribute   : %s\n", theVdtRec.attribute);
   printf("Value       : %d\n", theVdtRec.value);
   printf("Description : %s\n", theVdtRec.description);
}


/*---------------------------------------------*/
/* printCharVdtRecord
/*
/* Description:
/*    Utility that prints the contents of the
/*    specified "CHARACTER-value-description-
/*    record" to stdout.
/*---------------------------------------------*/
static void printCharVdtRecord 
                (charVdtRecordType theVdtRec)

{
   printf("____________\n");
   printf("ID          : %d\n", theVdtRec.ID);
   printf("Table       : %s\n", theVdtRec.table);
   printf("Attribute   : %s\n", theVdtRec.attribute);
   printf("Value       : %s\n", theVdtRec.value);
   printf("Description : %s\n", theVdtRec.description);
}


/*-------------------------------------------*/
/* readPastFormatInformation
/*
/* Description:
/*    This function will read the format data
/*    located at the front of the Value Description
/*    Table.
/*-------------------------------------------*/
static void readPastFormatInformation ()

{
         int tempChar;

   rewind (vdt_fd);

   tempChar = fgetc (vdt_fd);

   while (tempChar != EOF)
      {
       if ( (char) tempChar == ':')
          {
           tempChar = fgetc (vdt_fd);

           if ( (char) tempChar == ';')
              break;
          }

       tempChar = fgetc (vdt_fd);
      }
}
