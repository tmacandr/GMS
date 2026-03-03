/*---------------------------------------------------------------------------*/
/* File: gmsDcwUtilities.h
/* Date: 01-Mar-98 : initial definition (from CompSci 465)
/*       30-Aug-99 : use in GMS project
/*       14-Dec-99 : convert to DCW graphics utilities
/*       11-Jan-00 : moved file utils to 'gmsFileNameUtilities'
/*       22-Jan-00 : clean up the 'allocate/free' utilities.
/*       04-Feb-00 : added utils to get RNG and FAC tbls from EDG name
/*
/* Description:
/*    Defines utilites to assist the GMS "Chart Graphics" toolkit.
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*---------------------------------------------------------------------------*/

#ifndef GMS_DCW_UTILS

   #include <gmsTypesAndConstants.h>
   #include <gmsEdgeTable.h>
   #include <gmsFaceTable.h>
   #include <gmsRingTable.h>

   #define GMS_DCW_UTILS


   /*-----------------------------------------------------*/
   /* gmsAllocate_2D_FlatWorldImageFromEdgeTable
   /*
   /* Description:
   /*    Allocates memory for a 2D image in world coordinates
   /*    based on the specified "edge-table".  Each Lat/Long
   /*    pair from the edge table is used to define a 2D
   /*    world coordinate.
   /*
   /*    Client should call "gmsFree_2D_FlatWorldImage" (see
   /*    below) when finished with the image.  This function
   /*    uses the 'new' operator.
   /*-----------------------------------------------------*/
   gms_2D_WorldImageType gmsAllocate_2D_FlatWorldImageFromEdgeTable
                             (edgeTableType edgTbl);


   /*-----------------------------------------------------*/
   /* gmsAllocate_2D_FlatWorldImageFrom_3D_WorldImage
   /*
   /* Description:
   /*    Allocates memory for a 2D image in world coordinates
   /*    based on the specified 3D world image.  The number
   /*    of lines and the number of points per line will
   /*    be the same between the two images, it's just that
   /*    the z dimension will be lost.
   /*
   /*    Client should call "gmsFree_2D_FlatWorldImage" (see
   /*    below) when finished with the image.  This function
   /*    uses the 'new' operator.
   /*-----------------------------------------------------*/
   gms_2D_WorldImageType gmsAllocate_2D_FlatWorldImageFrom_3D_WorldImage
                                (gms_3D_WorldImageType worldImage_3D);


   /*-----------------------------------------------------*/
   /* gmsFree_2D_FlatWorldImage
   /*
   /* Description:
   /*    Deallocates memory used for a 2D image in world
   /*    coordinates.
   /*-----------------------------------------------------*/
   void gmsFree_2D_WorldImage
           (gms_2D_WorldImageType *theImage);


   /*-----------------------------------------------------*/
   /* gmsAllocate_2D_ScreenImageFromEdgeTable
   /*
   /* Description:
   /*    Allocates memory for a 2D image based on the
   /*    structure of the specified "edge-table".
   /*
   /*    Client should call "gmsFree_2D_ScreenImage" when
   /*    finished with the image.  This function uses
   /*    the 'new' operator.
   /*-----------------------------------------------------*/
   gms_2D_ScreenImageType gmsAllocate_2D_ScreenImageFromEdgeTable
                             (edgeTableType edgTbl);


   /*-----------------------------------------------------*/
   /* gmsFree_2D_ScreenImage
   /*
   /* Description:
   /*    Deallocates memory used for a 2D image.
   /*-----------------------------------------------------*/
   void gmsFree_2D_ScreenImage
           (gms_2D_ScreenImageType *theImage);


   /*-----------------------------------------------------*/
   /* gmsAllocate_3D_WorldImageFromEdgeTable
   /*
   /* Description:
   /*    Allocates memory for a 3D image in world coordinates
   /*    based on the specified "edge-table".
   /*
   /*    Client should call "gmsFree_3D_WorldImage" (see
   /*    below) when finished with the image.  This function
   /*    uses the 'new' operator.
   /*-----------------------------------------------------*/
   gms_3D_WorldImageType gmsAllocate_3D_WorldImageFromEdgeTable
                             (edgeTableType edgTbl);


   /*-----------------------------------------------------*/
   /* gmsFree_3D_WorldImage
   /*
   /* Description:
   /*    Deallocates memory used for a 3D image in world
   /*    coordinates.
   /*-----------------------------------------------------*/
   void gmsFree_3D_WorldImage
           (gms_3D_WorldImageType *theImage);


   /*-----------------------------------------------------*/
   /* gmsAllocate_3D_ScreenImageFromWorldImage
   /*
   /* Description:
   /*    Allocates memory for a 3D image in screen coordinates
   /*    based on the specified "3D world image".
   /*
   /*    Client should call "gmsFree_3D_ScreenImage" (see
   /*    below) when finished with the image.  This function
   /*    uses the 'new' operator.
   /*-----------------------------------------------------*/
   gms_3D_ScreenImageType gmsAllocate_3D_ScreenImageFromEdgeTable
                             (gms_3D_WorldImageType worldImg);


   /*-----------------------------------------------------*/
   /* gmsFree_3D_ScreenImage
   /*
   /* Description:
   /*    Deallocates memory used for a 3D image in screen
   /*    coordinates.
   /*-----------------------------------------------------*/
   void gmsFree_3D_ScreenImage
           (gms_3D_ScreenImageType *theImage);


   /*-----------------------------------------------------*/
   /* gmsGetPtrToRingTable
   /*
   /* Description:
   /*    This utility uses the given NAME of an edge table
   /*    to determine the name of the corresponding RING
   /*    table.  Then, the ring table is retrieved and a
   /*    pointer to that ring table is returned to the
   /*    caller.
   /*-----------------------------------------------------*/
   ringTableType *gmsGetPtrToRingTable
                         (const char *edgTblName);


   /*-----------------------------------------------------*/
   /* gmsGetPtrToFaceTable
   /*
   /* Description:
   /*    This utility uses the given NAME of an edge table
   /*    to determine the name of the corresponding FACE
   /*    table.  Then, the face table is retrieved and a
   /*    pointer to that face table is returned to the
   /*    caller.
   /*-----------------------------------------------------*/
   faceTableType *gmsGetPtrToFaceTable
                         (const char *edgTblName);


   /*-----------------------------------------------------*/
   /* gmsDraw_2D_Circle
   /*
   /* Description:
   /*    Left-over from CompSci 465.  This function calculates
   /*    the points of a circle using the "Bresenham-Michener"
   /*    algorithm.  Caller specifies the center point of
   /*    the circle and its radius.
   /*
   /*    Caller must invoke "gmsFree_2D_ScreenImage" when
   /*    finished (see above).  This function uses
   /*    the 'new' operator.
   /*-----------------------------------------------------*/
   gms_2D_ScreenImageType gmsDraw_2D_Circle
                             (int center_x,
                              int center_y,
                              int radius);

#endif
