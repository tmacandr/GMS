#!/bin/ksh
#========================================================
# File        : buildGraphics.ksh
# Date        : 27-Jul-99 : inital definition - CSUN HP
#               10-Feb-00 : use on SUN Sparc Ultra at Litton DSD
#               11-Feb-00 : port back to CSUN HP
#               15-Feb-00 : build "smarts" to run on HP, SUN, Litton
#               24-Feb-00 : filter out "gmsBitmapClass" as Windows only
# Author      : T. MacAndrew
#
# Description :
#    Script to build and archive the files of the Graphics
#    toolkit.
#
# Copyright (c), Timothy MacAndrew, all rights reserved
#========================================================

if test $# -ne 1; then

   echo "Usage: buildGraphics.ksh <hp | sun | litton>";
   
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

   echo "Usage : buildGraphics.ksh <hp | sun | litton>";

   exit;

fi

export MV=mv

export AR="ar -r";

export Root="$HOME/thesis/C_source";

export H_PATH="-I$Root/include/gmsGraphics
               -I$Root/include/gmsFile";

export L_PATH=

export L_LIBS=

export LD_LIBRARY_PATH=

export AR_LIB="$Root/lib/libGmsGraph.a";

if test -r $AR_LIB; then

   echo "---> Deleting old archive lib";

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

cd $Root/source/gmsGraphics;

export FILE_NAME=`ls *.cpp | sed '/gmsBitmap/d'`;

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

