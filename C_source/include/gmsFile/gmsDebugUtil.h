/*------------------------------------------------------*/
/* File : gmsDebugUtil.h
/* Date : 15-Jun-99
/*
/* Description:
/*    Header file that performs debug functions for
/*    the "Geographic Map System" (GMS).  MS Windows
/*    does not support 'printf' in non-console
/*    applications.  Therefore, this package will
/*    write debug statements to a file "debug.txt".
/*
/* Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
/*------------------------------------------------------*/

#ifndef GMS_DEBUG

   #define GMS_DEBUG

   /*---------------------------------------------*/
   /* gmsWriteDebug
   /*
   /* Description:
   /*    This utility will write the specified
   /*    message to a file "debug.txt".
   /*---------------------------------------------*/
   void gmsWriteDebug
	       (const char *theMessage);

#endif
