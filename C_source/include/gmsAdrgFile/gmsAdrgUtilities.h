/*----------------------------------------------------------------------------*/
/* File : gmsUtilities.cpp
/* Date : 03-Dec-99 : Initial defintion
/*
/* Description:
/*    Utilities that support the ADRG portion of the "Geographic Map System"
/*    (GMS) being developed for MS Project at CSUN.
/*
/*    Reference:
/*        1) Mil-A-89007  : ARC Digitized Raster Graphics
/*        2) Mil-Std-2411 : Raster Gridded Product Format
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*----------------------------------------------------------------------------*/

#ifndef GMS_ADRG_UTILITIES

   #define GMS_ADRG_UTILITIES

   /*---------------------------------------------*/
   /* gmsGetAdrgFullPath
   /*
   /* Description:
   /*    This utility catenates the specified string
   /*    with the string that is the root path to
   /*    the ADRG files.  A new string is returned
   /*    to the caller.
   /*
   /*    NOTE:
   /*       The caller must NOT free the string
   /*       returned.  Subsequent calls overwrite
   /*       the previous string.
   /*---------------------------------------------*/
   char *gmsGetAdrgFullPath
            (const char *whichFile);

#endif
