//----------------------------------------------------------------------------*/
// File : gmsDebugUtil.cpp
// Date : 15-Jun-99 : Initial definition
//        05-Oct-99 : Clean-up due to code inspection
//        11-Feb-00 : C++ compiler complains about init of char *
//
// Description:
//    Component that writes debug information for
//    the "Geographic Map System" (GMS).  MS Windows
//    does not support 'printf' in non-console
//    applications.  Therefore, this package will
//    write debug statements to a file "debug.txt".
//
// Copyright (c) 1999-2026, Timothy MacAndrew, all rights reserved
//----------------------------------------------------------------------------*/


#include <stdio.h>


//--------------------------*/
// Declare Local Objects
//--------------------------*/
static FILE *fileDescriptor = (FILE *) NULL;

static char nameOfFile[] = { "debug.txt" }; 


//--------------------------------------------*/
// gmsWriteDebug
//
// Description:
//    This utility will write the specified
//    message to a file "debug.txt".
//--------------------------------------------*/
void gmsWriteDebug
               (const char *theMessage)

{
   
   if (fileDescriptor == (FILE *) NULL)
   {
    fileDescriptor = fopen (nameOfFile, "w+");
   }

   if (fileDescriptor != (FILE *) NULL)
   {
    fprintf(fileDescriptor,
            "%s", theMessage);

    fflush (fileDescriptor);
   }
}
