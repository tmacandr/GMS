#!/bin/bash
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
#===========================================================================

HERE=`pwd`

cd ..

Root=`pwd`

cd $HERE

Base=$Root/C_source

H_PATH=-I$Base/include/gmsFile

AR_LIB="$HERE/lib/libGmsFile.a"

if test -r $AR_LIB; then

   echo "Cleaning old archive file"

   rm $AR_LIB

fi

#
# GMS macros
# ------------
#    NOT_CPP
#    IS_UNIX
#    BIG_ENDIAN
#
OPTIONS="-DIS_UNIX"

cd $Base/source/gmsFile

FILE_NAME=`ls *.cpp`

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
