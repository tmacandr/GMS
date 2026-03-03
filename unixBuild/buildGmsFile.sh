#!/bin/ksh
#===========================================================================
# File        : buildFile.ksh
# Date        : 27-Jul-99 : initial definition (CSUN HP lab)
#               10-Feb-00 : use to build on SUN SparcUltra at Litton DSD
#               11-Feb-00 : back to CSUN HPUX
#               15-Feb-00 : add "smarts" to build on both SUN and HP
#               24-Feb-00 : point to unix version of test driver
# Author      : T. MacAndrew
#
# Description :
#    Script to build and archive the files that read
#    the files from the DCW library.
#
# Copyright (c), Timothy MacAndrew, all rights reserved
#===========================================================================

if test $# -ne 1; then

   echo "Usage: buildFile.ksh <hp | sun | litton>";
   
   exit;

fi

if test "$1" = "litton"; then

   export Compiler_Path=/opt/cots/SUNWspro/SC5.0/bin
 
   export CPP="$Compiler_Path/CC";

elif test "$1" = "hp"; then

   export Compiler_Path=/opt/aCC/bin

   export CPP="$Compiler_Path/aCC"; 

elif test "$1" = "sun"; then 

   export Compiler_Path=/opt/cecs/bin
   
   export CPP="$Compiler_Path/c++";

else

   echo "Usage : buildFile.ksh <hp | sun | litton>";

   exit;

fi

export MV=mv

export AR="ar -r";

export Root=$HOME/thesis

export Base=$Root/C_source;

export H_PATH=-I$Base/include/gmsFile;

export L_PATH=

export L_LIBS=

export LD_LIBRARY_PATH=

export AR_LIB="$Base/lib/libGmsFile.a";

if test -r $AR_LIB; then

   echo "Cleaning old archive file";

   rm $AR_LIB;

fi

#
# GMS macros
# ------------
#    NOT_CPP
#    IS_UNIX
#    BIG_ENDIAN
#
export OPTIONS="-DIS_UNIX -DBIG_ENDIAN";

cd $Base/source/gmsFile;

export FILE_NAME=`ls *.cpp`;

#
# Okay ... go for it
#
for eachFile in $FILE_NAME; do

   export each=`echo $eachFile | sed 's/\.cpp//g'`;

   echo "_______________";
   echo "Compiling $each";

   $CPP $OPTIONS $H_PATH -c $each.cpp;

   if test -r $each.o; then

      echo "Archiving";

      $AR $AR_LIB $each.o;

      rm $each.o;

   else

      echo "File : $each did NOT compile";

      exit;

   fi

done
