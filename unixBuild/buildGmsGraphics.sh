#!/bin/bash
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
#========================================================

HERE=`pwd`

cd ..

ROOT=`pwd`

H_PATH="-I$ROOT/C_source/include/gmsGraphics \
        -I$ROOT/C_source/include/gmsFile"

cd $HERE

AR_LIB="$HERE/lib/libGmsGraph.a"

if test -r $AR_LIB; then

   echo "---> Deleting old archive lib"

   rm $AR_LIB

fi

#
# GMS macros
# ------------
#    NOT_CPP
#    IS_UNIX
#
OPTIONS="-DIS_UNIX -Wall"

cd $ROOT/C_source/source/gmsGraphics

FILE_NAME=`ls *.cpp | sed '/gmsBitmap/d'`

#
# Okay ... go for it
#
for eachFile in $FILE_NAME; do

   each=`echo $eachFile | sed 's/\.cpp//g'`

   echo "_______________"
   echo "Compiling $each"

   gcc $OPTIONS $H_PATH -c $each.cpp

   if test -r $each.o; then

      echo "Archiving"

      ar -r $AR_LIB $each.o

      rm $each.o

   else

      echo "File : $each did NOT compile"

      exit

   fi

done

