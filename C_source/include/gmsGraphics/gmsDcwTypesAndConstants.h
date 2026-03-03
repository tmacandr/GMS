/*---------------------------------------------------------------------------*/
/* File: gmsDcwTypesAndConstants.h
/* Date: 25-Jan-00 : initial definition
/*       27-Jan-00 : add <stdio.h> dependency for NULL
/*
/* Description:
/*    Common DCW types and useful constants.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/
#ifndef GMS_DCW_TYPES

   #define GMS_DCW_TYPES

   #include <stdio.h> /* get access to "NULL" */
   #include <gmsTypesAndConstants.h>


   /*-------------------------------------------------------*/
   /*                 Type Declarations
   /*-------------------------------------------------------*/

   typedef enum 
      {
          gms_DatabaseHeaderTable,
          gms_LibraryAttributeTable,
          gms_CoverageAttributeTable,
          gms_DataQualityTable,
          gms_GeographicReferenceTable,
          gms_LibraryHeaderTable
      } gmsDataTablesType;


   static const gms_2D_ScreenImageType Null_2D_Screen_Image = {0, NULL};

   static const gms_3D_ScreenImageType Null_3D_Screen_Image = {0, NULL};

   static const gms_2D_WorldImageType  Null_2D_World_Image = {0, NULL};

   static const gms_3D_WorldImageType  Null_3D_World_Image = {0, NULL};

   static const int                    Num_Subtiles_Per_Tile = 9;

#endif
