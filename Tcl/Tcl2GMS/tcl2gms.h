/*****************************************************************************
 * FILE: tcl2gms.h
 * DATE: 20-Mar-26 - Init def.  Leverage from 'unix_browse.cpp'
 *
 * DESCRIPTION:
 *    Declare a set of functions so the Tcl versions of DCW/VPF browse
 *    applications can invoke and use the packages of the Graphics Mapping
 *    System (GMS).  Starting baseline will be the functions from the
 *             'unix_browse.cpp'
 *****************************************************************************/
#ifndef Tcl_2_GMS

#define Tcl_2_GMS

#include <tcl.h>

/************************************************
 * FUNCTION: Tcl2GMS_Init 
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
int Tcl2GMS_Init(Tcl_Interp *interp);


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
                const char *argv[]);


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
               const char *argv[]);

#endif

/* EOF */

