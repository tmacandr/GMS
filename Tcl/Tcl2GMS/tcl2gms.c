/*****************************************************************************
 * FILE: tcl2gms.c
 * DATE: 20-Mar-26 - Init def.  Leverage from 'unix_browse.cpp'
 *
 * DESCRIPTION:
 *    Declare a set of functions so the Tcl versions of DCW/VPF browse
 *    applications can invoke and use the packages of the Graphics Mapping
 *    System (GMS).  Starting baseline will be the functions from the
 *             'unix_browse.cpp'
 *    Note:
 *       Has to be in C.  The C++ compiler name-mangles the object code
 *       and the internal Tcl "glue" code expects straight-up function
 *       names.
 *
 *       I forget how to tell the C++ compiler to not name-mangle, if
 *       possible.
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
   //---------//
#if 0
#include <gmsDebugUtil.h>
#include <gmsBrowseMapClass.h>
#include <gmsMapStateMgr.h>
#include <gmsScreenCoordUtils.h>
#include <gmsUtilities.h>
#endif

#include "tcl2gms.h"

/************************************************
 * FUNCTION: Tg_Init 
 *
 * DESCRIPTION:
 *    Function called by Tcl script to initialize
 *    the interface between the Tcl interpreter
 *    (vis-a-vis Tcl shell) and the C functions
 *    defined by this package.
 *
 *    Once this initialization function is
 *    invoked from the Tcl script, the functions
 *    below can be invoked.
 ***********************************************/
int Tg_Init(Tcl_Interp *interp)
{
   const char *s = Tcl_InitStubs(interp, TCL_VERSION, 0);

   if (s == NULL)
   {
      printf("***> ERROR - Tcl init stubs fail\n");
      return TCL_ERROR;
   }

   /*
    * Tcl mechanics: "Provide the package" 'tcl2gms'
    */
   int r = Tcl_PkgProvide(interp, "Tcl2GMS", "1.0"); 

   if (r == TCL_ERROR)
   {
       printf("***> ERROR - Package Provide FAIL\n");
       return TCL_ERROR;
   }

   /*
    * Create the new Tcl command "Tcl2GMS_init_map_settings" and link
    * it to the corresponding C function
    */
   Tcl_CreateCommand
          (interp,
           "Tcl2GMS_init_map_settings",
           Tcl2GMS_init_map_settings,
           (ClientData) NULL,
           (Tcl_CmdDeleteProc *) NULL);

   Tcl_CreateCommand
          (interp,
           "Tcl2GMS_drawLibRef",
           Tcl2GMS_drawLibRef,
           (ClientData) NULL,
           (Tcl_CmdDeleteProc *) NULL);

   return TCL_OK;
}


/************************************************
 * FUNCTION: Tcl2GMS_init_map_settings
 *
 * DESCRIPTION:
 *   Initializes GMS mapping parameters.
 ***********************************************/
int Tcl2GMS_init_map_settings
               (ClientData clientData,
                Tcl_Interp *interp,
                int        argc,
                const char *argv[])
{
   int answer = 1234;

   printf("---> get_integer\n"
          "--->    argc...: %d\n",
          argc);

   for (int i = 0; i < argc; i++)
   {
       printf("--->    argv[%d]...: %s\n",
              i,
              argv[i]);
   }

   printf("---> answer: %d\n", answer);

   Tcl_Obj *result_obj;

   result_obj = Tcl_NewIntObj(answer);

   Tcl_SetObjResult(interp, result_obj);

   return TCL_OK;
}

/************************************************
 * FUNCTION: Tcl2GMS_drawLibRef 
 *
 * DESCRIPTION:
 *   Draw LibRef map to canvas.
 ***********************************************/
int Tcl2GMS_drawLibRef
              (ClientData clientData, 
               Tcl_Interp *interp, 
               int        argc,
               const char *argv[])
{
   float answer = 3.14159f;

   printf("---> get_float\n"
          "--->    argc...: %d\n",
          argc);

   for (int i = 0; i < argc; i++)
   {
       printf("--->    argv[%d]...: %s\n",
              i,
              argv[i]);
   }

   printf("---> get_float: %f\n", answer);

   Tcl_Obj *result_obj;

   result_obj = Tcl_NewDoubleObj((double) answer);

   Tcl_SetObjResult(interp, result_obj);

   return TCL_OK;
}

/* EOF */

